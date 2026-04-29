package bosch.bh.testprograms.ivtb.example.teststeps

import bosch.bh.include.exceptions.TestException
import bosch.bh.include.platform.rtmodules.AverageReturn
import bosch.bh.include.platform.rtmodules.soft.AverageFunctionSoft
import bosch.bh.include.platform.teststep.TestFunction
import bosch.bh.spex.sdk.common.PVLong
import bosch.bh.spex.sdk.tl.MemLong
import bosch.bh.spex.sdk.tl.ResultValueLong
import bosch.bh.spex.sdk.tl.ResultValueText
import bosch.bh.spex.sdk.tl.SetValueLong
import bosch.bh.testprograms.ivtb.global.IVTBTestStep
import bosch.bh.ts3000.procs.StepReturn
import bosch.bh.ts3000.pvserver.Inject

// **************************************************************
// *
// * File name      : $RCSfile: AV_B10_Switch_Time_TAN_T2K.inc $
// * Module version : $Revision: 1.5 $
// * Version date   : $Date: 2009/10/01 10:51:22CEST $
// * Author(s)      : $Author: Velte Markus (VM/EMH9-DE) (VEM2SI) $
// *
// **************************************************************
// *
// * $Log: AV_B10_Switch_Time_TAN_T2K.inc  $
// * Revision 1.5 2009/10/01 10:51:22CEST Velte Markus (VM/EMH9-DE) (VEM2SI) 
// * Neu: "  GL_STU_U_OFFSET_T = 0"
// * Revision 1.4 2009/01/20 08:51:02CET Velte Markus (CC/ECH7) (VEM2SI) 
// * New comments: good test / bad test
// * New parameter for sub procedure
// * Revision 1.3 2008/12/18 09:42:05CET Velte Markus (CC/ECH7) (VEM2SI) 
// * Shut down pressures after test step.
// * Revision 1.2 2008/12/12 10:44:39CET Velte Markus (CC/ECH7) (VEM2SI) 
// * Bugfix during implementation.
// * Revision 1.1 2008/12/04 15:23:14CET Velte Markus (CC/ECH7) (VEM2SI) 
// * Initial revision
// * Member added to project x:/sw_mgt/TestSpec/MV/HYDR/GEN09/AV/TESTSTEPS/AV_B10_Switch_Time_TAN_T2K/project.pj
// * 
// **************************************************************

// **************************************************************
// * Deklaration     
// **************************************************************

class AV_B10_Switch_Time_TAN_T2K extends IVTBTestStep {

  @Inject
  SetValueLong M_10_MODF_T
  @Inject
  SetValueLong M_10_U_BATT_T
  @Inject
  SetValueLong M_10_I_MV_T
  @Inject
  SetValueLong M_10_P_VOR_T
  @Inject
  SetValueLong M_10_P_RAD_T
  @Inject
  SetValueLong M_10_P_DIFF_T
  @Inject
  SetValueLong P_10_P_SCHW_T
  @Inject
  SetValueLong D_10_T_TAN_T

  @Inject
  ResultValueLong M_10_MODF
  @Inject
  ResultValueLong M_10_U_BATT
  @Inject
  ResultValueLong M_10_I_MV
  @Inject
  ResultValueLong M_10_P_VOR
  @Inject
  ResultValueLong M_10_P_RAD
  @Inject
  ResultValueLong M_10_P_DIFF
  @Inject
  ResultValueLong P_10_P_SCHW
  @Inject
  ResultValueLong D_10_T_TAN

  @Inject
  ResultValueText GL_ERROR_TXT

  @Inject
  MemLong vStation
  @Inject
  MemLong vModeFlag


  @Inject
  MemLong vAO1_Set

  @Inject
  PVLong vSTUSwitch
  @Inject
  PVLong Pump2
  @Inject
  PVLong Y22
  @Inject
  PVLong Y23
  @Inject
  PVLong Y24
  @Inject
  PVLong Y29

  @Inject
  PVLong Pump1
  @Inject
  PVLong Y11
  @Inject
  PVLong Y12
  @Inject
  PVLong Y13
  @Inject
  PVLong Y14
  @Inject
  PVLong Y15
  @Inject
  PVLong Y16
  @Inject
  PVLong Y17
  @Inject
  PVLong Y18
  @Inject
  PVLong Y19
  @Inject
  PVLong Y21
  @Inject
  PVLong Y25
  @Inject
  PVLong Y26
  @Inject
  PVLong Y27
  @Inject
  PVLong Y28
  @Inject
  PVLong Y30

  @Inject
  PVLong P12
  @Inject
  PVLong P22

  @Inject
  PVLong to1

// **************************************************************
// * Test Step
// **************************************************************

  @Override
  StepReturn apply(StepReturn stepReturn) {
    int LOC_Count_Cyc                               // Dummy for all modules

    int LOC_Ind_Aft_Vorl                            // Open time modules
    int LOC_Time_Aft_Vorl                           // Open time modules

    int LOC_Pressure_Set
    int[] M_LOC_P_DIFF_T = new Integer[2]

    try {

      vAO1_Set.set 1000 // Test step coding

      println(" 10 Switch Time TAN ")

      M_10_MODF.set(0)

      if (vModeFlag == 0) {
        intercept_All() // @todo: was'n das'n?
      }

      set(vSTUSwitch)

      waitTime(25)

      if (vStation.get() == GL_Test_Bench_Int) { // @@todo: sag a mal a zahl

        set(Pump2, Y22, Y23, Y24, Y29)

        reset(Pump1, Y11, Y12, Y13, Y14, Y15, Y16, Y17, Y18, Y19, Y21, Y25, Y26, Y27, Y28, Y30)

        wait(250)
      }

      GL_STU_U_OFFSET_T.set(0) // @todo: ist das eine Toleranz?

      Set_STU_Voltage_for_specific_current(GL_STU_U_OFFSET_T, M_10_U_BATT_T, M_10_I_MV_T)

      M_LOC_P_DIFF_T[0] = M_10_P_DIFF_T[0] + GL_CL_PULS_KOMP
      M_LOC_P_DIFF_T[1] = M_10_P_DIFF_T[1] + GL_CL_PULS_KOMP

      // ***********************************
      // ***********************************

      stepReturn << Put_Ramp_Bent_(P2_SET, 200000, M_LOC_P_DIFF_T, 0, 0, 0)

      // Error handling
      if (stepReturn.hasErrors()) {
        String errorText = sprintf("PS10: MODULERROR !! (Put_Ramp_Bent_: M_LOC_P_DIFF_T),)) Error: %d ", stepReturn.errorCode)
        GL_ERROR_TXT.set(errorText)
        M_10_MODF = M_10_MODF + stepReturn.errorCode
        throw new TestException(errorText)
      }

      // ***********************************
      // ***********************************

      // Adjust pressure difference P12 - P22
      stepReturn << Put_Adapt_Stat_(         //       static adjustment
          to1,                   //  01 < Trigger channel
          P2_Set,                 //  02 < Setting channel
          P22,                   //  03 < Measuring channel for to regular value
          P12,                   //  04 < Measuring channel for possibly delta measuring
          0,                     //  05 < Sensor-Relativum pvor_prad
          0,                     //  06 < Differenz-Sollwert-Invertierung
          1,                     //  07 < Mess-Stell-Korrelations-Multiplikator
          1,                     //  08 < Mess-Stell-Korrelations-Divisor
          M_LOC_P_DIFF_T,         //  09 < Setting value before
          M_LOC_P_DIFF_T,       //  10 < Setting value with tolerances
          25,                   //  11 < Stellwert-Toleranz-Einengungsprozentsatz
          100,                   //  12 < Kompensationsanteil in Prozent
          250,                   //  13 < Beruhigungszeit Konstantanteil fuer vorweg
          150,                   //  14 < Beruhigungszeit Konstantanteil zyklisch
          100,                   //  15 < Beruhigungszeit-Gradient (bar/sec)
          4,                     //  16 < Amount of mean values prior to filtering
          1,                     //  17 < Flag for filtering
          49,                     //  18 < Amount of mean values after filtering
          1,                     //  19 < minimale Anzahl geforderter Schritte
          8,                     //  20 < maximale Anzahl erlaubter Schritte
          250000,                 //  21 < new: max. set value	@TODO parametrierbar!
          LOC_Count_Cyc,         //  22 > tatsaechliche Anzahl Adaptions-Schritte
          LOC_Pressure_Set,      //  23 > tatsaechliche Wert auf Stellkanal ausgegeben
          M_10_P_DIFF)           //  24 > zurueckgemessene Regelgroesse

      // Error handling
      if (stepReturn.hasErrors()) {
        String errorText = sprintf("PS10: MODULERROR !! (Put_Adapt_Stat_: M_10_P_DIFF_T), Error: %d ", stepReturn.errorCode)
        GL_ERROR_TXT.set(errorText)
        M_10_MODF = M_10_MODF + stepReturn.errorCode
        throw new TestException(errorText)
      }

      // ***********************************
      // ***********************************

      if (vStation.get() == GL_Test_Bench_Int) { // @todo: sag a mal a zahl
        set(Pump2, Y22, Y23, Y29)
        reset(Pump1, Y11, Y12, Y13, Y14, Y15, Y16, Y17, Y18, Y19, Y21, Y24, Y25, Y26, Y27, Y28, Y30)
        waitTime(250)
      }

      set(to1)

      AverageReturn averageResult = new AverageFunctionSoft().average(
          [P12, P22], 25, 2
      )

      M_10_P_VOR.set(averageResult.averageValues[0] as int)
      M_10_P_RAD.set(averageResult.averageValues[1] as int)

      reset(to1)

      // ***********************************
      // ***********************************

      // Measurement switch time tan prepare
      stepReturn << Open_Time_Pre_(                        //       Measurement open time prepare
          MVS,                                  //  01 < Setting channel
          1,                                    //  02 > Setting value while
          0,                                    //  03 > Setting value after
          P22,                                  //  04 < Main measurement channel
          100000,                              //  05 < Amount during complete measurement
          0,                                    //  06 < Starter index
          BUF_OPEN_TIME,                        //  07 < Measurement value target buffer
          5000,                                //  08 < Amount prior to action within the overall amount
          (M_10_P_RAD - 10000),                  //  09 < 3. limit pressure for data acquisition termination
          1,                                    //  10 < Interrupt locking
          1)                                    //  11 < Parallel task locking


      // Error handling
      if (stepReturn.hasErrors()) {
        String errorText = sprintf("PS10: MODULERROR !! (Open_Time_Pre_: D_10_T_TAN), Error: %d ", stepReturn.errorCode)
        GL_ERROR_TXT.set(errorText)
        M_10_MODF = M_10_MODF + stepReturn.errorCode
        throw new TestException(errorText)
      }

      // ***********************************
      // ***********************************

      // Measurement switch time tan execute
      stepReturn << Open_Time_Exe_(           //       Measurement open time execute
          LOC_Ind_Aft_Vorl,         //  01 > Actual overall index amount
          LOC_Time_Aft_Vorl)       //  02 > Actual execution time


      // Error handling
      if (stepReturn.hasErrors()) {
        String errorText = sprintf("PS10: MODULERROR !! (Open_Time_Exe_: D_10_T_TAN), Error: %d ", stepReturn.errorCode)
        GL_ERROR_TXT.set(errorText)
        M_10_MODF = M_10_MODF + stepReturn.errorCode
        throw new TestException(errorText)
      }

      // ***********************************
      // ***********************************

      // Measurement switch time tan evaluation
      stepReturn << Open_Time_Eva_(            //       Measurement open time evaluation
          100000,                    //  01 < Evaluation of amount
          0,                        //  02 < Starter index
          BUF_OPEN_TIME,            //  03 < Measurement value target buffer
          5000,                    //  04 < Amount of pre-run accesses
          LOC_Time_Aft_Vorl,        //  05 < Amount measuring
          LOC_Ind_Aft_Vorl,         //  06 < Overall measuring time in mys
          -1,                        //  07 < Direction of pressure change
          1,                        //  08 < Value stretching factor
          P_10_P_SCHW_T,            //  09 < Delta pressure
          D_10_T_TAN)              //  10 > Open time

      // Error handling
      if (stepReturn.hasErrors()) {
        String errorText = sprintf("PS10: MODULERROR !! (Open_Time_Eva_: D_10_T_TAN), Error: %d ", stepReturn.errorCode)
        GL_ERROR_TXT.set(errorText)
        M_10_MODF = M_10_MODF + stepReturn.errorCode
        throw new TestException(errorText)
      }

      // ***********************************
      // ***********************************

      // Calculated measurements
      M_10_P_DIFF = M_10_P_RAD - M_10_P_VOR

      // Pseudo measurements
      P_10_P_SCHW = (P_10_P_SCHW_T[0] + P_10_P_SCHW_T[1]) / 2

      intercept_All()

      if (vStation.get() == GL_Test_Bench_Int) { // @todo: sag a mal a zahl
        set(Pump2, Y22, Y23, Y24, Y29)
        reset(Pump1, Y11, Y12, Y13, Y14, Y15, Y16, Y17, Y18, Y19, Y21, Y25, Y26, Y27, Y28, Y30)
        waitTime(250)
      }

      P2_SET = ((GL_P_NULL[0] + GL_P_NULL[1]) / 2)

      // ***********************************
      // ***********************************

      stepReturn << Wait_For_Intern_(P21, 0, GL_P_NULL, 0, GL_Timeout_2500)

      // Error handling
      if (stepReturn.hasErrors()) {
        String errorText = sprintf("PS10: MODULERROR !! (Wait_For_Intern_: P21=GL_P_NULL, Error: %d ", stepReturn.errorCode)
        GL_ERROR_TXT.set(errorText)
        M_10_MODF = M_10_MODF + stepReturn.errorCode
        throw new TestException(errorText)
      }

      // ***********************************
      // ***********************************

      Wait_For_Intern_(P22, 0, GL_P_NULL, 0, GL_Timeout_2500, stepReturn.errorCode)

      // Error handling
      if (stepReturn.errorCode != 0) {
        String errorText = sprintf("PS10: MODULERROR !! (Wait_For_Intern_: P22=GL_P_NULL), Error: %d ", stepReturn.errorCode)
        GL_ERROR_TXT.set(errorText)
        ErrorOutput()
        M_10_MODF = M_10_MODF + stepReturn.errorCode
        throw new TestException(errorText)
      }

      // ***********************************
      // ***********************************

      set(MVS)

      waitTime(25)

      set(to1)

      Mitteln(25, 2,
          U_MV, M_10_U_BATT,
          I_MV_AR, M_10_I_MV)

      reset(to1)

      reset(MVS)

      GL_Step_Break = 1

      Wd_Write_Pos(GL_line, 35, " --> done                     ", 1)


      reset(MVS)

      intercept_All()

      if (vStation.get() == GL_Test_Bench_Int) {
        set(Pump2, Y22, Y23, Y24, Y29)
        reset(Pump1, Y11, Y12, Y13, Y14, Y15, Y16, Y17, Y18, Y19, Y21, Y25, Y26, Y27, Y28, Y30)
        waitTime(250)
      }

      P2_SET = ((GL_P_NULL[0] + GL_P_NULL[1]) / 2)

      // ***********************************
      // ***********************************

      stepReturn << Wait_For_Intern_(P21, 0, GL_P_NULL, 0, GL_Timeout_2500)

      // Error handling
      if (stepReturn.hasErrors()) {
        String errorText = sprintf("PS10: MODULERROR !! (Wait_For_Intern_: P21=GL_P_NULL, Error: %d ", stepReturn.errorCode)
        GL_ERROR_TXT.set(errorText)
        M_10_MODF = M_10_MODF + stepReturn.errorCode
        throw new TestException(errorText)
      }

      // ***********************************
      // ***********************************

      stepReturn << Wait_For_Intern_(P22, 0, GL_P_NULL, 0, GL_Timeout_2500)

      // Error handling
      if (stepReturn.hasErrors()) {
        String errorText = sprintf("PS10: MODULERROR !! (Wait_For_Intern_: P22=GL_P_NULL), Error: %d ", stepReturn.errorCode)
        GL_ERROR_TXT.set(errorText)
        M_10_MODF = M_10_MODF + stepReturn.errorCode
        throw new TestException(errorText)
      }

      // ***********************************
      // ***********************************

      if (IGNOREBREAK_FLAG == 1) {
        GL_Step_Break = 1
      }

      // Testing results
      stepReturn << testResults(
          M_10_MODF,
          M_10_U_BATT,
          M_10_I_MV,
          M_10_P_VOR,
          M_10_P_RAD,
          M_10_P_DIFF,
          P_10_P_SCHW,
          D_10_T_TAN
      )

    } catch (TestException testException) {
      stepReturn << testException
    }

    if (stepReturn.hasErrorsOrFailedTests()) {
      println " --> good test                "
    } else {
      println " --> bad test                 "
    }

    return stepReturn
  }

}

// **************************************************************
// * $RCSfile: AV_B10_Switch_Time_TAN_T2K.inc $
// **************************************************************