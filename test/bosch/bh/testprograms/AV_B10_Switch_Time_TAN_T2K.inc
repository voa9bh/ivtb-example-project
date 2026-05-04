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

#DECLARE SetValue
  INT  M_10_MODF_T,		
  INT	 M_10_U_BATT_T,
	INT  M_10_I_MV_T,
	INT  M_10_P_VOR_T,
	INT	 M_10_P_RAD_T,
	INT  M_10_P_DIFF_T,
	INT  P_10_P_SCHW_T,
	INT  D_10_T_TAN_T;

#DECLARE Result
	INT  M_10_MODF,		
	INT	 M_10_U_BATT,
	INT  M_10_I_MV,
	INT  M_10_P_VOR,
	INT	 M_10_P_RAD,
	INT  M_10_P_DIFF,
	INT  P_10_P_SCHW,
	INT  D_10_T_TAN;

#DECLARE LABELTYP
	B10_END;

// **************************************************************
// * Test Step
// **************************************************************

procedure AV_B10_Switch_Time_TAN_T2K;

#DECLARE Memory
  INT  LOC_Count_Cyc, 															 // Dummy for all modules
  INT  LOC_Time_Cyc,  															 // Dummy for all modules
  INT  LOC_Aver,      															 // Dummy for all modules
  INT  LOC_Min,       															 // Dummy for all modules
	INT  LOC_Max,			  															 // Dummy for all modules

	INT  LOC_Ind_Aft_Vorl,														 // Open time modules
  INT  LOC_Time_Aft_Vorl,	  												 // Open time modules

	INT  LOC_Pressure_Set,
	INT  M_LOC_P_DIFF_T[2];	
	
  AO1_Set = 1000; // Test step coding

  call Increment_Line;
  Wd_Write_Pos (GL_Line, 3, " 10 Switch Time TAN ",1);

	GL_Error_10 		= 0;
	GL_ModError_10 	= 0;
  M_10_MODF		   	= 0;

  if (Flag_Mode == 0);
    call Intercept_All;
  endif;

  // Mode for STU5 
	Set (STU_SWITCH);
  Wait_Time (25);

	if (Station == GL_Test_Bench_Int);
    Set   (                                            Pump2,    Y22,Y23,Y24,                Y29      );
    ReSet (Pump1,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,        Y21,            Y25,Y26,Y27,Y28,      Y30);
    Wait_Time (250);
  endif;
			
  GL_STU_U_OFFSET_T = 0; 

  call Set_STU_Voltage_for_specific_current (GL_STU_U_OFFSET_T, M_10_U_BATT_T, M_10_I_MV_T);	

  M_LOC_P_DIFF_T[0] = M_10_P_DIFF_T[0] + GL_CL_PULS_KOMP;
  M_LOC_P_DIFF_T[1] = M_10_P_DIFF_T[1] + GL_CL_PULS_KOMP;

  // ***********************************
  // ***********************************

  Put_Ramp_Bent_ (P2_SET, 200000, M_LOC_P_DIFF_T, 0, 0, 0, GL_ModError_10);

  // Error handling
  if (GL_ModError_10 <> 0);
    int_to_str_f (
      GL_ModError_10,
      "PS10: MODULERROR !! (Put_Ramp_Bent_: M_LOC_P_DIFF_T), Error: %d ",
      GL_ERROR_TXT,	GL_ModError);
    call ErrorOutput;
    M_10_MODF = M_10_MODF + GL_ModError_10;
    if (IGNOREBREAK_FLAG <> 1);
      goto B10_END;	
    endif;
  endif;

  // ***********************************
  // ***********************************

  // Adjust pressure difference P12 - P22  
  Put_Adapt_Stat_ (    		 //       static adjustment
    to1,               		 //  01 < Trigger channel                    
    P2_Set,            		 //  02 < Setting channel 
    P22,               		 //  03 < Measuring channel for to regular value
    P12,               		 //  04 < Measuring channel for possibly delta measuring
    0,			 		     			 //  05 < Sensor-Relativum pvor_prad
    0,                 		 //  06 < Differenz-Sollwert-Invertierung
    1,                 		 //  07 < Mess-Stell-Korrelations-Multiplikator 
    1,                 		 //  08 < Mess-Stell-Korrelations-Divisor
    M_LOC_P_DIFF_T,  			 //  09 < Setting value before              
    M_LOC_P_DIFF_T, 	   	 //  10 < Setting value with tolerances 
    25,               		 //  11 < Stellwert-Toleranz-Einengungsprozentsatz
    100,               		 //  12 < Kompensationsanteil in Prozent
    250,               		 //  13 < Beruhigungszeit Konstantanteil fuer vorweg
    150,               		 //  14 < Beruhigungszeit Konstantanteil zyklisch
    100,               		 //  15 < Beruhigungszeit-Gradient (bar/sec)
    4,                 		 //  16 < Amount of mean values prior to filtering
    1,                 		 //  17 < Flag for filtering
    49,                		 //  18 < Amount of mean values after filtering
    1,                 		 //  19 < minimale Anzahl geforderter Schritte
    8,                 		 //  20 < maximale Anzahl erlaubter Schritte
    250000,						     //  21 < new: max. set value	@TODO parametrierbar!
    LOC_Count_Cyc,         //  22 > tatsaechliche Anzahl Adaptions-Schritte
    LOC_Pressure_Set,      //  23 > tatsaechliche Wert auf Stellkanal ausgegeben
    M_10_P_DIFF,       		 //  24 > zurueckgemessene Regelgroesse
    GL_ModError_10);       //  25 > Module error 

  // Error handling
  if (GL_ModError_10 <> 0);
    int_to_str_f (
      GL_ModError_10,
      "PS10: MODULERROR !! (Put_Adapt_Stat_: M_10_P_DIFF_T), Error: %d ",
      GL_ERROR_TXT,	GL_ModError);
    call ErrorOutput;
    M_10_MODF = M_10_MODF + GL_ModError_10;
    if (IGNOREBREAK_FLAG <> 1);
      goto B10_END;	
    endif;
  endif;

  // ***********************************
  // ***********************************
 	 
	if (Station == GL_Test_Bench_Int);
    Set   (                                            Pump2,    Y22,Y23,                    Y29      );
    ReSet (Pump1,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,        Y21,        Y24,Y25,Y26,Y27,Y28,      Y30);
    Wait_Time (250);
  endif;

  Set (to1);
  Mitteln (25, 2,
           P12,  M_10_P_VOR,
           P22,  M_10_P_RAD); 
  ReSet (to1);
	
  // ***********************************
  // ***********************************

  // Measurement switch time tan prepare
  Open_Time_Pre_ (    			 							//       Measurement open time prepare
		MVS,		            		 							//  01 < Setting channel
    1,       															//  02 > Setting value while
    0,                   	 	 	 						//  03 > Setting value after 
		P22,		               	 							//  04 < Main measurement channel
		100000,             		 							//  05 < Amount during complete measurement
		0,                  		 							//  06 < Starter index
		BUF_OPEN_TIME,       		 							//  07 < Measurement value target buffer
		5000,               		 							//  08 < Amount prior to action within the overall amount
		(M_10_P_RAD - 10000),									//  09 < 3. limit pressure for data acquisition termination
		1,                  		 							//  10 < Interrupt locking
		1,                  		 							//  11 < Parallel task locking
		GL_ModError_10);         							//  12 > Module error
 
	// Error handling
	if (GL_ModError_10 <> 0);
		int_to_str_f (
		  GL_ModError_10,
			"PS10: MODULERROR !! (Open_Time_Pre_: D_10_T_TAN), Error: %d ",
			GL_ERROR_TXT,	GL_ModError);
		call ErrorOutput;
		M_10_MODF = M_10_MODF + GL_ModError_10;
		if (IGNOREBREAK_FLAG <> 1);
			goto B10_END;	
		endif;
	endif;

  // ***********************************
  // ***********************************

  // Measurement switch time tan execute
  Open_Time_Exe_ (      		 //       Measurement open time execute
		LOC_Ind_Aft_Vorl,		     //  01 > Actual overall index amount
		LOC_Time_Aft_Vorl,	     //  02 > Actual execution time
		GL_ModError);            //  03 > Module error

	// Error handling
	if (GL_ModError_10 <> 0);
		int_to_str_f (
		  GL_ModError_10,
			"PS10: MODULERROR !! (Open_Time_Exe_: D_10_T_TAN), Error: %d ",
			GL_ERROR_TXT,	GL_ModError);
		call ErrorOutput;
		M_10_MODF = M_10_MODF + GL_ModError_10;
		if (IGNOREBREAK_FLAG <> 1);
			goto B10_END;	
		endif;
	endif;

  // ***********************************
  // ***********************************

  // Measurement switch time tan evaluation
	Open_Time_Eva_ (      		 	//       Measurement open time evaluation
		100000,              		 	//  01 < Evaluation of amount
		0,                  		 	//  02 < Starter index
		BUF_OPEN_TIME,       		 	//  03 < Measurement value target buffer
		5000,               		 	//  04 < Amount of pre-run accesses
		LOC_Time_Aft_Vorl,        //  05 < Amount measuring
		LOC_Ind_Aft_Vorl,         //  06 < Overall measuring time in mys
		-1,                  		 	//  07 < Direction of pressure change
		1,                  		 	//  08 < Value stretching factor
		P_10_P_SCHW_T,      		 	//  09 < Delta pressure
		D_10_T_TAN,         		 	//  10 > Open time     
		GL_ModError_10);         	//  11 > Module error

	// Error handling
	if (GL_ModError_10 <> 0);
		int_to_str_f (
		  GL_ModError_10,
			"PS10: MODULERROR !! (Open_Time_Eva_: D_10_T_TAN), Error: %d ",
			GL_ERROR_TXT,	GL_ModError);
		call ErrorOutput;
		M_10_MODF = M_10_MODF + GL_ModError_10;
		if (IGNOREBREAK_FLAG <> 1);
			goto B10_END;	
		endif;
	endif;

  // ***********************************
  // ***********************************

	// Calculated measurements 
	M_10_P_DIFF = M_10_P_RAD - M_10_P_VOR;

 	// Pseudo measurements
	P_10_P_SCHW = (P_10_P_SCHW_T[0] + P_10_P_SCHW_T[1]) / 2;

  call Intercept_All;

  if (Station == GL_Test_Bench_Int);
    Set   (                                            Pump2,    Y22,Y23,Y24,                Y29      );
    ReSet (Pump1,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,        Y21,            Y25,Y26,Y27,Y28,      Y30);
    Wait_Time (250);
  endif;

  P2_SET = ((GL_P_NULL[0] + GL_P_NULL[1]) / 2);

  // ***********************************
  // ***********************************

  Wait_For_Intern_ (P21, 0, GL_P_NULL, 0, GL_Timeout_2500, GL_ModError_10);

  // Error handling
  if (GL_ModError_10 <> 0);
    int_to_str_f (
      GL_ModError_10,
      "PS10: MODULERROR !! (Wait_For_Intern_: P21=GL_P_NULL, Error: %d ",
      GL_ERROR_TXT,	GL_ModError);
    call ErrorOutput;
    M_10_MODF = M_10_MODF + GL_ModError_10;
    if (IGNOREBREAK_FLAG <> 1);
      goto B10_END;	
    endif;
  endif;

  // ***********************************
  // ***********************************

  Wait_For_Intern_ (P22, 0, GL_P_NULL, 0, GL_Timeout_2500, GL_ModError_10);

  // Error handling
  if (GL_ModError_10 <> 0);
    int_to_str_f (
      GL_ModError_10,
      "PS10: MODULERROR !! (Wait_For_Intern_: P22=GL_P_NULL), Error: %d ",
      GL_ERROR_TXT,	GL_ModError);
    call ErrorOutput;
    M_10_MODF = M_10_MODF + GL_ModError_10;
    if (IGNOREBREAK_FLAG <> 1);
      goto B10_END;	
    endif;
  endif;

  // ***********************************
  // ***********************************

  Set (MVS); 	  
	Wait_Time (25);	
  
  Set (to1);
  Mitteln (25, 2, 
           U_MV,    M_10_U_BATT,  
           I_MV_AR, M_10_I_MV); 
  ReSet (to1);

	ReSet (MVS);
	
	GL_Step_Break = 1;

	Wd_Write_Pos (GL_line, 35, " --> done                     ",1);

LABEL B10_END;
  	
  ReSet (MVS); 

  call Intercept_All;

  if (Station == GL_Test_Bench_Int);
    Set   (                                            Pump2,    Y22,Y23,Y24,                Y29      );
    ReSet (Pump1,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,        Y21,            Y25,Y26,Y27,Y28,      Y30);
    Wait_Time (250);
  endif;

  P2_SET = ((GL_P_NULL[0] + GL_P_NULL[1]) / 2);

  // ***********************************
  // ***********************************

  Wait_For_Intern_ (P21, 0, GL_P_NULL, 0, GL_Timeout_2500, GL_ModError_10);

  // Error handling
  if (GL_ModError_10 <> 0);
    int_to_str_f (
      GL_ModError_10,
      "PS10: MODULERROR !! (Wait_For_Intern_: P21=GL_P_NULL, Error: %d ",
      GL_ERROR_TXT,	GL_ModError);
    call ErrorOutput;
    M_10_MODF = M_10_MODF + GL_ModError_10;
    if (IGNOREBREAK_FLAG <> 1);
      goto B10_END;	
    endif;
  endif;

  // ***********************************
  // ***********************************

  Wait_For_Intern_ (P22, 0, GL_P_NULL, 0, GL_Timeout_2500, GL_ModError_10);

  // Error handling
  if (GL_ModError_10 <> 0);
    int_to_str_f (
      GL_ModError_10,
      "PS10: MODULERROR !! (Wait_For_Intern_: P22=GL_P_NULL), Error: %d ",
      GL_ERROR_TXT,	GL_ModError);
    call ErrorOutput;
    M_10_MODF = M_10_MODF + GL_ModError_10;
    if (IGNOREBREAK_FLAG <> 1);
      goto B10_END;	
    endif;
  endif;

  // ***********************************
  // ***********************************

 	if (IGNOREBREAK_FLAG == 1);
		GL_Step_Break = 1;
	endif;

	// Testing results
  Test (GL_Error_10, 1,
        M_10_MODF,			
        M_10_U_BATT,
        M_10_I_MV,
        M_10_P_VOR,
				M_10_P_RAD,
			  M_10_P_DIFF,
			  P_10_P_SCHW,
        D_10_T_TAN);

  if (GL_Error_10 == 0); 
    Wd_Write_Pos (GL_line, 35, " --> good test                ",1);
  else;
    Wd_Write_Pos (GL_line, 35, " --> bad test                 ",1);
  endif;	

endprocedure; 

// **************************************************************
// * $RCSfile: AV_B10_Switch_Time_TAN_T2K.inc $
// **************************************************************