// **************************************************************
// *
// * File name      : $RCSfile: AV_B08_Pressure_Step_03_T2K.inc $
// * Module version : $Revision: 1.6 $
// * Version date   : $Date: 2009/10/01 10:50:36CEST $
// * Author(s)      : $Author: Velte Markus (VM/EMH9-DE) (VEM2SI) $
// *
// **************************************************************
// *
// * $Log: AV_B08_Pressure_Step_03_T2K.inc  $
// * Revision 1.6 2009/10/01 10:50:36CEST Velte Markus (VM/EMH9-DE) (VEM2SI) 
// * Neu: "  GL_STU_U_OFFSET_T = 0"
// * Revision 1.5 2009/01/20 08:51:03CET Velte Markus (CC/ECH7) (VEM2SI) 
// * New comments: good test / bad test
// * New parameter for sub procedure
// * Revision 1.4 2008/12/29 11:03:46CET Velte Markus (CC/ECH7) (VEM2SI) 
// * Shut down pressures after test step.
// * Revision 1.3 2008/12/12 09:00:02CET Velte Markus (CC/ECH7) (VEM2SI) 
// * Bugfix during implementation.
// * Revision 1.2 2008/12/08 15:38:45CET Velte Markus (CC/ECH7) (VEM2SI) 
// * Bugfix.
// * Revision 1.1 2008/12/04 14:42:06CET Velte Markus (CC/ECH7) (VEM2SI) 
// * Initial revision
// * Member added to project x:/sw_mgt/TestSpec/MV/HYDR/GEN09/AV/TESTSTEPS/AV_B08_Pressure_Step_03_T2K/project.pj
// * 
// **************************************************************

// **************************************************************
// * Deklaration     
// **************************************************************

#DECLARE SetValue
  INT  M_08_MODF_T,		
  INT	 M_08_U_BATT_T,
	INT  M_08_I_MV_T,
	INT  M_08_P_VOR_T,
	INT	 M_08_P_RAD_T,
	INT  M_08_P_DIFF_T,
	INT  P_08_T_IPZ_T,
	INT  D_08_P_DST_T;

#DECLARE Result
	INT  M_08_MODF,		
	INT	 M_08_U_BATT,
	INT  M_08_I_MV,
	INT  M_08_P_VOR,
	INT	 M_08_P_RAD,
	INT  M_08_P_DIFF,
  INT  P_08_T_IPZ,
	INT  D_08_P_DST;

#DECLARE LABELTYP
	B08_END;

// **************************************************************
// * Test Step
// **************************************************************

procedure AV_B08_Pressure_Step_03_T2K;
 
#DECLARE Memory
  INT  LOC_Count_Cyc, 				 // Dummy for all modules
  INT  LOC_Time_Cyc,  				 // Dummy for all modules
  INT  LOC_Aver,      				 // Dummy for all modules
  INT  LOC_Min,       				 // Dummy for all modules
	INT  LOC_Max,				  			 // Dummy for all modules

	INT  LOC_DST_Loop,
	INT  LOC_Loop,
	INT  LOC_Pressure_Set,
	INT  M_LOC_P_DIFF_T[2];					 

 	AO1_Set = 600; // Test step coding

  call Increment_Line;
  Wd_Write_Pos (GL_Line, 3, " 08 Pressure Step 03 ",1);

	LOC_Loop 	 = 1;
	D_08_P_DST = 0;

  // Mode for STU5 
	Set (STU_SWITCH);	
  Wait_Time (25);
			
  GL_STU_U_OFFSET_T = 0; 

  call Set_STU_Voltage_for_specific_current (GL_STU_U_OFFSET_T, M_08_U_BATT_T, M_08_I_MV_T);	

	while (LOC_Loop < 4); 

    GL_Error_08 		= 0;
    GL_ModError_08 	= 0;
    M_08_MODF		   	= 0;

    call Intercept_All;

  	if (Station == GL_Test_Bench_Int);
      Set   (                                            Pump2,    Y22,Y23,Y24,                Y29      );
      ReSet (Pump1,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,        Y21,            Y25,Y26,Y27,Y28,      Y30);
      Wait_Time (250);
    endif;
		
		M_LOC_P_DIFF_T[0] = M_08_P_DIFF_T[0] + GL_CL_PULS_KOMP;
	  M_LOC_P_DIFF_T[1] = M_08_P_DIFF_T[1] + GL_CL_PULS_KOMP;

    // ***********************************
    // ***********************************

    if (LOC_Loop == 1);
      Put_Ramp_Bent_ (P2_SET, 200000, M_LOC_P_DIFF_T, 0, 0, 0, GL_ModError_08);

      // Error handling
      if (GL_ModError_08 <> 0);
        int_to_str_f (
          GL_ModError_08,
          "PS08: MODULERROR !! (Put_Ramp_Bent_: M_LOC_P_DIFF_T), Error: %d ",
          GL_ERROR_TXT,	GL_ModError);
        call ErrorOutput;
        M_08_MODF = M_08_MODF + GL_ModError_08;
        if (IGNOREBREAK_FLAG <> 1);
          goto B08_END;	
        endif;
      endif;
    endif;

   	// ***********************************
    // ***********************************

	  // Adjust pressure difference P11 - P21  
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
      75,                		 //  11 < Stellwert-Toleranz-Einengungsprozentsatz
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
      M_08_P_DIFF,       		 //  24 > zurueckgemessene Regelgroesse
	  	GL_ModError_08);       //  25 > Module error 

	  // Error handling
	  if (GL_ModError_08 <> 0);
	  	int_to_str_f (
	  	  GL_ModError_08,
	  		"PS08: MODULERROR !! (Put_Adapt_Stat_: M_08_P_DIFF_T), Error: %d ",
	  		GL_ERROR_TXT,	GL_ModError);
	  	call ErrorOutput;
	  	M_08_MODF = M_08_MODF + GL_ModError_08;
	  	if (IGNOREBREAK_FLAG <> 1);
	  		goto B08_END;	
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
             P12, M_08_P_VOR,
             P22, M_08_P_RAD); 
    ReSet (to1);

    // ***********************************
    // ***********************************

    Put_Pulses_ (
    	MVS,										 // 01 Setting channel 
  		0,											 // 02 Set value 1
  		1,											 // 03 Set value 2
  		10000,									 // 04 Time set value 1
  		P_08_T_IPZ_T,						 // 05 Time set value 2
  		75000);									 // 06 Time set value 1

    // ***********************************
    // ***********************************
      
    Set (to1);
    Mitteln (25, 2, P22, LOC_DST_Loop); 
    ReSet (to1);
		
  	// Calculated measurements 																											
  	M_08_P_DIFF  = M_08_P_RAD - M_08_P_VOR;
  	LOC_DST_Loop = M_08_P_RAD - LOC_DST_Loop;

   	// Pseudo measurements
  	P_08_T_IPZ = (P_08_T_IPZ_T[0] + P_08_T_IPZ_T[1]) / 2;
		
  	// Testing results
    Test (GL_Error_08, 1,
          M_08_MODF,			
          M_08_P_VOR,
  				M_08_P_RAD,
  			  M_08_P_DIFF,
  			  P_08_T_IPZ);
    
  	if (GL_Error_08 > 0);
			GL_Step_Break = 1;
  		goto B08_END;
  	endif;
   
  	// D_08_P_DST in Loop
  	D_08_P_DST = D_08_P_DST + LOC_DST_Loop;

		LOC_Loop = LOC_Loop + 1;

	endwhile;

  call Intercept_All;

  if (Station == GL_Test_Bench_Int);
    Set   (                                            Pump2,    Y22,Y23,Y24,                Y29      );
    ReSet (Pump1,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,        Y21,            Y25,Y26,Y27,Y28,      Y30);
    Wait_Time (250);
  endif;

  P2_SET = 0;

  // ***********************************
  // ***********************************

  Wait_For_Intern_ (P21, 0, GL_P_NULL, 0, GL_Timeout_2500, GL_ModError_08);

  // Error handling
  if (GL_ModError_08 <> 0);
    int_to_str_f (
      GL_ModError_08,
      "PS08: MODULERROR !! (Wait_For_Intern_: P21=GL_P_NULL, Error: %d ",
      GL_ERROR_TXT,	GL_ModError);
    call ErrorOutput;
    M_08_MODF = M_08_MODF + GL_ModError_08;
    if (IGNOREBREAK_FLAG <> 1);
      goto B08_END;	
    endif;
  endif;

  // ***********************************
  // ***********************************

  Wait_For_Intern_ (P22, 0, GL_P_NULL, 0, GL_Timeout_2500, GL_ModError_08);

  // Error handling
  if (GL_ModError_08 <> 0);
    int_to_str_f (
      GL_ModError_08,
      "PS08: MODULERROR !! (Wait_For_Intern_: P22=GL_P_NULL), Error: %d ",
      GL_ERROR_TXT,	GL_ModError);
    call ErrorOutput;
    M_08_MODF = M_08_MODF + GL_ModError_08;
    if (IGNOREBREAK_FLAG <> 1);
      goto B08_END;	
    endif;
  endif;

  // ***********************************
  // ***********************************

  Set (MVS); 	  
	Wait_Time (25);	
  
  Set (to1);
  Mitteln (25, 2, 
           U_MV,    M_08_U_BATT,  
           I_MV_AR, M_08_I_MV); 
  ReSet (to1);

	ReSet (MVS);

	// Calculate D_08_P_DST after 3 measurements
	D_08_P_DST = D_08_P_DST / 3; 
    
  // Testing results
  Test (GL_Error_08, 1,
        M_08_MODF,
        M_08_U_BATT,
        M_08_I_MV,
        D_08_P_DST);

	GL_Step_Break = 1;

	Wd_Write_Pos (GL_line, 35, " --> done                     ",1);

LABEL B08_END;
  	
  ReSet (MVS); 

  call Intercept_All;

  if (Station == GL_Test_Bench_Int);
    Set   (                                            Pump2,    Y22,Y23,Y24,                Y29      );
    ReSet (Pump1,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,        Y21,            Y25,Y26,Y27,Y28,      Y30);
    Wait_Time (250);
  endif;

  P2_SET = 0;

  // ***********************************
  // ***********************************

  Wait_For_Intern_ (P21, 0, GL_P_NULL, 0, GL_Timeout_2500, GL_ModError_08);

  // Error handling
  if (GL_ModError_08 <> 0);
    int_to_str_f (
      GL_ModError_08,
      "PS08: MODULERROR !! (Wait_For_Intern_: P21=GL_P_NULL, Error: %d ",
      GL_ERROR_TXT,	GL_ModError);
    call ErrorOutput;
    M_08_MODF = M_08_MODF + GL_ModError_08;
    if (IGNOREBREAK_FLAG <> 1);
      goto B08_END;	
    endif;
  endif;

  // ***********************************
  // ***********************************

  Wait_For_Intern_ (P22, 0, GL_P_NULL, 0, GL_Timeout_2500, GL_ModError_08);

  // Error handling
  if (GL_ModError_08 <> 0);
    int_to_str_f (
      GL_ModError_08,
      "PS08: MODULERROR !! (Wait_For_Intern_: P22=GL_P_NULL), Error: %d ",
      GL_ERROR_TXT,	GL_ModError);
    call ErrorOutput;
    M_08_MODF = M_08_MODF + GL_ModError_08;
    if (IGNOREBREAK_FLAG <> 1);
      goto B08_END;	
    endif;
  endif;

  // ***********************************
  // ***********************************

 	if (IGNOREBREAK_FLAG == 1);
		GL_Step_Break = 1;
	endif;

  if (GL_Error_08 == 0); 
    Wd_Write_Pos (GL_line, 35, " --> good test                ",1);
  else;
    Wd_Write_Pos (GL_line, 35, " --> bad test                 ",1);
  endif;			

endprocedure; 
 
// **************************************************************
// * $RCSfile: AV_B08_Pressure_Step_03_T2K.inc $
// **************************************************************	