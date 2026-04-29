import groovy.transform.TypeChecked
import org.codehaus.groovy.control.CompilerConfiguration
import org.codehaus.groovy.control.customizers.ASTTransformationCustomizer
import org.codehaus.groovy.control.customizers.SecureASTCustomizer
import bosch.bh.ts3000.tli.DefineExpressionChecker


CompilerConfiguration config=configuration

config.addCompilationCustomizers(new ASTTransformationCustomizer(TypeChecked, extensions: "bosch.bh.ts3000.tli.TLITypeChecker"))

File targetDirectory=config.getTargetDirectory()

File buildDirectory=targetDirectory

while(buildDirectory!=null && buildDirectory.name!="build") {
  buildDirectory=buildDirectory.getParentFile()
}

File mainDirectory=buildDirectory.getParentFile()
File srcDirectory=new File(mainDirectory,"src")
println "found src directory ${srcDirectory}"

def secureAstCustomizer = new SecureASTCustomizer()

secureAstCustomizer.addExpressionCheckers(new DefineExpressionChecker(srcDirectory))

config.addCompilationCustomizers(secureAstCustomizer)
