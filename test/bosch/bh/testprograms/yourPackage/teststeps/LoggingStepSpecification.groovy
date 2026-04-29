package bosch.bh.testprograms.yourPackage.teststeps

import bosch.bh.ts3000.init.UsrAppInit
import bosch.bh.ts3000.procs.TLI
import bosch.bh.ts3000.procs.TLM
import org.apache.logging.log4j.LogManager
import org.apache.logging.log4j.Logger
import spock.lang.Shared
import spock.lang.Specification

class LoggingStepSpecification extends Specification {
  @Shared
  File tmpDir

  def setupSpec() {     // run before the first feature method
    String tempDirString = System.getProperty("java.io.tmpdir")
    tmpDir = new File("${tempDirString}/TestSpock/data")
    if (tmpDir.exists()) {
      tmpDir.deleteDir()
    }
    tmpDir.mkdirs()

    File log4j2File = new File(tmpDir, "log4j2.xml")

    File logsDir = new File(tmpDir, "logs")
    logsDir.mkdirs()

    File historyDir = new File(logsDir, "history")
    historyDir.mkdirs()

    //generate Log4J XML Config File:
    String log4j2Configuration = """<?xml version='1.0' encoding='UTF-8'?>
        <Configuration status='ERROR' packages='bosch.bh.ts3000.procs'>
          <Appenders>
            <Console name='A1' target='SYSTEM_OUT'>
              <PatternLayout pattern='A1> %-4r [%t] %-5p %c %x - %m%n' />
            </Console>  

            <RollingFile name='T1' fileName='${tmpDir.absolutePath}/logs/TLI.log' filePattern='${tmpDir.absolutePath}/logs/history/TLI1-%d{yyyy}-\${sys:tli1.log.file.data}-%i.log' >
              <PatternLayout pattern='%p %m%n' />
              <Policies>
                <TimeBasedTriggeringPolicy />
                <SizeBasedTriggeringPolicy size='250 MB'/>
                <FileRolloverPolicy/>
              </Policies>
            </RollingFile>

          </Appenders>
          <Loggers>
            <Root level='debug'>
              <AppenderRef ref='A1' />
            </Root>

            <Logger name='tli1' additivity='false' level='all'>
              <AppenderRef ref='T1' />              
            </Logger>   

          </Loggers>
        </Configuration>"""

    log4j2File.withWriter("UTF-8") { Writer writer ->
      writer.write(log4j2Configuration)
    }

    UsrAppInit.configureLogging(log4j2File.absolutePath)
  }

  // dummy tlm for testing, real TLM gets Logger via ClassLoader by tli.name
  class MyTLM extends TLM {

    MyTLM(Logger logger) {
      this.setPrintLogger(logger)
    }

    @Override
    void setupLogger() {
      // do not setup the logger (needs class loader)
    }

    @Override
    Object run() {
      return null
    }
  }

  def 'test logging with various log functions'() {
    given:

    System.setProperty("tli1.log.file.data","UNDEFINED")

    Logger logger = LogManager.getLogger("tli1")

    Binding binding=new Binding()
    TLI tli=new TLI() {
      // use our own logger for testing purposes
      Logger getPrintLogger() {
        return logger
      }

      // override getPVObjectName() to make the tli name tli1
      @Override
      protected String getPVObjectName() {
        return "tli1"
      }
    }

    binding.setVariable("_TLI",tli)

    TLM tlm = new MyTLM(logger)
    binding.setVariable("_TLM",tlm)

    File logFile = new File("${tmpDir.absolutePath}/logs/TLI.log")
    File logHistoryDir = new File("${tmpDir.absolutePath}/logs/history")

    tlm.setBinding(binding)

    LoggingStep loggingStep=new LoggingStep()
    loggingStep.setBinding(binding)


    when:
    loggingStep.call()

    List<String> logFileLines=logFile.readLines()
    List<String> logHistoryFiles=logHistoryDir.list()

    then:

    logFileLines.size()>0
    logFileLines[-1].contains("A final log for the LoggingStepSpecification")

    // created 2 rollovers, one undefined, one good
    // you can have a look at the files on C:\Users\<yourUser>\AppData\Local\Temp\TestSpock\data
    logHistoryFiles.findAll {String filename -> filename.contains("UNDEFINED")}.size()==1
    logHistoryFiles.findAll {String filename -> filename.contains("GOOD")}.size()==1


  }
}