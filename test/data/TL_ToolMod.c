/* ==================================================================

    Filename     	  :    $RCSfile: TL_ToolMod.c $ 
    Modulversion  	:    $Revision: 1.17 $
    Versionsdatum 	:    $Date: 2015/09/28 09:53:53MESZ $
    Copyright       :    Robert Bosch GmbH (BhW/TEF)
    Author(s)       :    Hans Riekert,i.O. interaktive Objekte GmbH


    Short description

     - Test language module tools for buffer handling
                                                                      
   ------------------------------------------------------------------     
    $Log: TL_ToolMod.c  $ 
    Revision 1.17 2015/09/28 09:53:53MESZ Kolb Michael (BhP/TEF18) (KM79BH)  
    added soft break flag for velte 
    Revision 1.16 2009/11/16 08:52:42MEZ Kolb Michael (BhP/TEF16) (KM79BH)  
    Fix SAMPLE_RATE changed from 7us to 10us 
    Revision 1.15 2008/01/29 16:21:29CET Jenn Joachim (BhP/TEF10) (jej9bh)  
    New parameters added to measurement  due to board independent(CPU speed) measurement. Therefore a timer has been needed. 
    Revision 1.14 2005/10/06 08:21:57CEST Jenn Joachim (BhP/TEF16) (jej9bh)  
    Changes to improve precition 
    Revision 1.13  2005/09/15 14:33:00Z  jej9bh 
    WIN32: semLib.h only for VXWORKS 
    Revision 1.12  2005/08/05 08:33:05Z  jej9bh 
    Include added (Compiler needs semLib.h for new timer.c) 
    Revision 1.11  2005/04/29 13:05:01Z  JEJ9BH 
    Bug fixed: If module were called from subroutine with setvalues as paramter, reference was interpreted wrong. 
    Revision 1.10  2005/01/11 09:43:19Z  jej9bh 
    Small bug in prt_alr_ran fixed: wrong calculation of mean value. 
    Revision 1.9  2004/05/14 09:20:33Z  jej9bh 
    Now file is compilable for windows platform as well. 
    Revision 1.8  2003/11/12 09:23:42Z  jej9bh 
    Some more bugs removed.  
    Revision 1.7  2003/11/03 09:41:04Z  jej9bh 
    Bugs fixed: - mes_tme_rel time measuring method was wrong and  
    in waitforgradient timeout determination was wrong as well. 
    Revision 1.6  2003/10/30 10:36:03Z  jej9bh 
    Module-Abnahme 2.Block (int. MV)  
    Bugs fixed. 
    Revision 1.4 2003/10/10 12:27:38CEST bim  
    all single line comments removed 
    Revision 1.3 2003/10/01 13:59:10CEST bim  
    tl_buffer.h added. 
    getDimension corrected. 
    prt_alr_xxx functions enhanced. 
    Revision 1.2 2003/09/29 11:27:52CEST bim  
    Folgende Funktionen im Rahmen der MV - Portierung (2. Block) hinzugefuegt: 
    - mes_mit_rel 
    - prt_alr_nug 
    - prt_alr_one 
    - getDirection 
    - get Dimension 
    - interpol_index 
    - tme_ind_anz 
    - abs_double 
    - stan_buf_anz 
    Revision 1.1 2003/07/29 10:31:56CEST bim  
    Initial revision 
    Member added to project //THOR/SIEE84_PROJEKTE/VxWorks/bosch/bh/ts2000/pvobjects/pvobjects.pj 
    Revision 1.4  2003/07/14 16:09:36Z  jej9bh 
    Abgleich mit i.O. 18.7.2003. 
    Revision 1.14  2003/07/14 18:09:36  hmr 
    Anpassung der Fehlernummern, aufr�umen unn�tige HeaderFiles  
    Revision 1.13  2003/06/13 12:02:01  bim 
    rampe_linear Zeitproblem behoben 
    Revision 1.12  2003/06/11 11:46:14  bim 
    Type cast 
    Revision 1.11  2003/05/28 15:59:47  bim 
    Fehler bei RampeLinear behoben 
    Revision 1.10  2003/05/13 10:06:11  bim 
    Fehler in in_anz_tme() behoben 
    Revision 1.9  2003/05/05 15:32:02  bim 
    MV_Portierung implemantiert und noch nicht eingebunden. 
    Revision 1.8  2002/11/15 08:43:37  hmr 
    WIN32 Integration 
    Revision 1.7  2001/12/20 13:51:07  Felix 
    Revision 1.4  2001/11/01 01:57:24  Felix 
    Revision 1.3  2001/11/01 01:18:09  Felix 
    Revision 1.2  2001/11/01 01:03:28  Felix 
   ------------------------------------------------------------------ */    

#define  TL_TOOLMOD_SOURCE

/* ------------------------------------------------------------------ */
/*      INCLUDE - Files                                               */
/* ------------------------------------------------------------------ */
#ifdef WIN32
#include <windows.h>
#include <winbase.h>
#else
#include <semlib.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pvServer.h"
#include "pvTlm.h"
#include "tl_toolmod.h"  
#include "pvtimer.h"
#include "PvBuffer.h"


/* ------------------------------------------------------------------ */
/*      GLOBAL VARIABLES                                              */
/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
/*      MODUL DECLARATION                                             */
/* ------------------------------------------------------------------ */


/* ---------------------------------------------------------------------------
f_title: mesmit
------------------------------------------------------------------------------
kind   : C_Function
purpose: 
mittelnder Messzugriff nach Filterung, falls eingeschaltet       
----------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -
output_para: -
--------------------------------------------------------------------------- */
long mesmit(long hObj, long *p_werpb_ele, int filter_flag, long mittel_zahl)
{
  long          stp;               /* lokaler Zaehler fuer Mittelung     */
  double        summe;
  long          wert1;
  long          wert2;
  long          wert3;
  long          wert;

  if (!mittel_zahl)            /* vorweg sicher stellen, dass min. 1 Aktion */
  {
    mittel_zahl = 1;
  }

  summe = 0;
  if (!filter_flag)
  {                                                       /* ohne Filterung */
    if (mittel_zahl == 1)
    {                                                     /* ohne Mittelung */
      pv_get (hObj, (void *) &wert);
      *p_werpb_ele = wert;
    }
    else
    {                                                      /* mit Mittelung */
      for (stp = 0; (stp < mittel_zahl); stp++)
      {
        pv_get (hObj, (void *) &wert);
        summe = summe + wert;
      }
      if (summe > 0)
      {
        *p_werpb_ele = (long) ((summe + (mittel_zahl/2)) / mittel_zahl);
      }
      else
      {
        *p_werpb_ele = (long) ((summe - (mittel_zahl/2)) / mittel_zahl);
      }
    }
  }
  else
  {                                                        /* mit Filterung */
    if (mittel_zahl == 1)
    {                                                     /* ohne Mittelung */
      pv_get (hObj, (void *) &wert1);
      pv_get (hObj, (void *) &wert2);
      pv_get (hObj, (void *) &wert3);
  
      if (wert1 > wert2)
      {
        if (wert3 > wert1)
        {
          wert = wert1;
        }
        else
        {
          if (wert3 < wert2)
          {
            wert = wert2;
          }
          else
          { 
          wert = wert3;
          }
        }
      }
      else
      {
        if (wert3 > wert2)
        {
          wert = wert2;
        }
        else
        {
          if (wert3 < wert1)
          {
            wert = wert1;
          }
          else
          {
            wert = wert3;
          }
        }
      }
      *p_werpb_ele = wert;
    }
    else 
    {                                        /* mit Mittelung und Filterung */
      for (stp = 0; (stp < mittel_zahl); stp++)
      {
        pv_get (hObj, (void *) &wert1);
        pv_get (hObj, (void *) &wert2);
        pv_get (hObj, (void *) &wert3);
  
        if (wert1 > wert2)
        {
          if (wert3 > wert1)
          {
            wert = wert1;
          }
          else
          {
            if (wert3 < wert2)
            {
              wert = wert2;
            }
            else
            { 
              wert = wert3;
            }
          }
        }
        else
        {
          if (wert3 > wert2)
          {
            wert = wert2;
          }
          else
          {
            if (wert3 < wert1)
            {
              wert = wert1;
            }
            else
            {
              wert = wert3;
            }
          }
        }
        summe = summe + wert;
      }
      if (summe > 0)
      {
        *p_werpb_ele = (long) ((summe + (mittel_zahl/2)) / mittel_zahl);
      }
      else
      {
        *p_werpb_ele = (long) ((summe - (mittel_zahl/2)) / mittel_zahl);
      }
    }
  }
  return 0;
}  /* end mesmit */



/* ---------------------------------------------------------------------------
f_title: mes_mittel_abs
------------------------------------------------------------------------------
kind   : C_function
purpose: 
Berechnet den Get-Wert gemittelt aus vorgegebener Anzahl Messungen.
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -
output_para: -
--------------------------------------------------------------------------- */
long mes_mittel_abs(long hObj, long hTimer, long security, long anz_mes)
{

  long       val;
  long       cnt;
  double     summe;
	int						SAMPLE_RATE=10; 			 /*	about 7 mys; faster isn't really reasonable because of compatibility to D2 and the M36 has only a sum sample rate	of 100kHz */
	long 					loopStartTime;
	long					loopTime;
	long ecode	= 0;
  summe = 0;
	
  for (cnt = 0; (cnt < anz_mes); cnt++)
  {
#if defined(MEN_D002)
	/* do not longer wait ... */
#else
	  ecode = pvAsk(hTimer, 0, security, METH_GET, 0, sizeof(long), (void*) &loopStartTime);										
		if(ecode==0)
		{
#endif

	    pv_get (hObj, (void *) &val);
 	   summe = summe + val;
#if WIN32
#if _DEBUG
  	  Sleep(0);
#endif
#endif
#if defined(MEN_D002)
	/* do not longer wait ... */
#else
		}
		if(ecode == 0)
		{
			loopTime=0;
			/* Ensure a exact waiting time via active waiting */
			while((ecode == 0) && (loopTime<SAMPLE_RATE))
			{
		      ecode = pvAsk( hTimer, 0, security, METH_GETELAPSEDTIME, (void*) &loopStartTime, sizeof(long), (void*) &loopTime);
			}
		}										
#endif
  }

  if (summe > 0)
  {
    return (long)((summe + (anz_mes/2))/ anz_mes);
  }
  else
  {
    return (long)((summe - (anz_mes/2))/ anz_mes);
  }

}  /* end of mes_mittel_abs */





/* ---------------------------------------------------------------------------
f_title: mes_tme_rel
------------------------------------------------------------------------------
kind   : C_function
purpose: 
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -
output_para: -
--------------------------------------------------------------------------- */

long mes_tme_rel(long hMeska, long hMeskb, long anz_vf, long filter, long hTimer,long security, long tme_mes)
{
  long       tme_act;
  long       tme_later;
  long       val1;
  long       val2;
  long       val3;
  long       mittel;
  long       anz_nf;
  double     summe;

  if (!anz_vf) anz_vf = 1;

  summe = 0;
  anz_nf = 0;

  pv_get (hTimer, (void *) &tme_act);
  tme_later = 0;	    

 	while (tme_later < tme_mes)
  {
    val1 = mes_mittel_rel (hMeska, hMeskb,hTimer, security, anz_vf);
    val2 = mes_mittel_rel (hMeska, hMeskb,hTimer, security, anz_vf);
    val3 = mes_mittel_rel (hMeska, hMeskb,hTimer, security, anz_vf);
        
    if (filter)
    {
      summe = summe + auswahl_mittel (val1, val2, val3);
      anz_nf++;
    }
    else
    {
      summe = summe + val1 + val2 + val3;
      anz_nf++; anz_nf++; anz_nf++;
    }

 		pvAsk( hTimer, 0, 0, METH_GETELAPSEDTIME, (void*) &tme_act, sizeof(long), (void*) &tme_later);    	    
        
  } /* end of while */
      
  if (summe > 0)
  {
    mittel = (long) (summe + (anz_nf/2))/ anz_nf;
  }
  else
   {
    mittel = (long) (summe - (anz_nf/2))/ anz_nf;
  }

  return (mittel);

}  /* end of mes_tme_rel */



/* ---------------------------------------------------------------------------
f_title: mes_tme_abs
------------------------------------------------------------------------------
kind   : C_function
purpose: 
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -
output_para: -
--------------------------------------------------------------------------- */

long mes_tme_abs(long hMesk, long anz_vf, long filter, long hTimer, long security, long tme_mes)
{
  long       tme_act;
  long       tme_later=0;
  long       val1;
  long       val2;
  long       val3;
  long       mittel;
  long       anz_nf;
  double     summe;

  if (!anz_vf) anz_vf = 1;

  summe = 0;
  anz_nf = 0;
  tme_later = 0;
  /*pv_get (hTimer, (void *) &tme_act);*/
  pvAsk( hTimer, 0, 0, METH_GET, 0, sizeof(long), (void*) &tme_act);
  while ( tme_later < tme_mes)
  {
    val1 = mes_mittel_abs (hMesk,hTimer,security, anz_vf);
    val2 = mes_mittel_abs (hMesk,hTimer,security, anz_vf);
    val3 = mes_mittel_abs (hMesk,hTimer,security, anz_vf);
        
    if (filter)
    {
      summe = summe + auswahl_mittel (val1, val2, val3);
      anz_nf++;
    }
    else
    {
      summe = summe + val1 + val2 + val3;
      anz_nf++; anz_nf++; anz_nf++;
    }
    pvAsk( hTimer, 0, 0, METH_GETELAPSEDTIME, (void*) &tme_act, sizeof(long), (void*) &tme_later);    	    
  } /* end of while ((tme_act - tme_later) > 0) */
      
  if (summe > 0)
  {
    mittel = (long) ((summe + (anz_nf/2))/ anz_nf);
  }
  else
   {
    mittel = (long) ((summe - (anz_nf/2))/ anz_nf);
  }

  return (mittel);

}  /* end of mes_tme_abs */



/* ---------------------------------------------------------------------------
f_title: mes_mittel_rel
------------------------------------------------------------------------------
kind   : Funktion
purpose: 
Berechnet das absolute Druckgefaelle gemittelt aus gegebener Anzahl Messungen.
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -
output_para: -
--------------------------------------------------------------------------- */

long mes_mittel_rel(long hMeska,long hMeskb, long hTimer, long security, long anz_mes)                    
{
  long       vala;
  long       valb;
  long       mittel;
  long       summe;
  long       dif;
  long       cnt;
	int						SAMPLE_RATE=10; 			 /*	about 7 mys; faster isn't really reasonable because of compatibility to D2 and the M36 has only a sum sample rate	of 100kHz */
	long 					loopStartTime;
	long					loopTime;
	long ecode = 0;
  summe = 0;
  for (cnt = 0; (cnt < anz_mes); cnt++)
  {
#if defined(MEN_D002)
	/* do not longer wait ... */
#else
    ecode = pvAsk( hTimer, 0, security, METH_GET, 0, sizeof(long), (void*) &loopStartTime);										
		if(ecode==0)
		{
#endif
  	  pv_get (hMeska, (void *) &vala);
   		pv_get (hMeskb, (void *) &valb);
  
  	  dif = vala - valb;
   		summe = summe + dif;
#if defined(MEN_D002)
	/* do not longer wait ... */
#else
		}
		if(ecode == 0)
		{
			loopTime=0;
			/* Ensure a exact waiting time via active waiting */
			while((ecode == 0) && (loopTime<SAMPLE_RATE))
			{
		      ecode = pvAsk( hTimer, 0, security, METH_GETELAPSEDTIME, (void*) &loopStartTime, sizeof(long), (void*) &loopTime);
			}
		}										
#endif
  }

  if (summe > 0)
  {
    mittel = (summe + (anz_mes/2)) / anz_mes;
  }
  else
  {
    mittel = (summe - (anz_mes/2)) / anz_mes;
  }
  return (mittel);

}  /* end of mes_mittel_rel */


/* ---------------------------------------------------------------------------
f_title: auswahl_mittel
------------------------------------------------------------------------------
kind   : C_function
purpose: 
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -
output_para: -
--------------------------------------------------------------------------- */

long auswahl_mittel(long val1, long val2, long val3)
{
  long       val;

  if (val1 > val2)
  {
    if (val3 > val1)
    {
      val = val1;
    }
    else
    {
      if (val3 < val2)
      {
        val = val2;
      }
      else
      { 
        val = val3;
      }
    }
  }
  else
  {
    if (val3 > val2)
    {
      val = val2;
    }
    else
    {
      if (val3 < val1)
      {
        val = val1;
      }
      else
      {
        val = val3;
      }
    }
  }
  return (val);

}  /* end of auswahl_mittel */


/* -------------------------------------------------------------------------
    S G N 
   -------------------------------------------------------------------------
    Input      :  x          x-value
    Output     :  sign       +1 (x >= 0)
                             -1 (x <  0)
    Function   :  calculate the sign of a value
   ------------------------------------------------------------------------- */

int sign( double x)
{
  if (x >= 0.0) {
    return 1;
  }
  else {
    return -1;
  }
}



/* ---------------------------------------------------------------------------
f_title: aver_buf_d
------------------------------------------------------------------------------
kind   : C_Function
purpose: 
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -  buf = Quellbufferpointer 
output_para: -
--------------------------------------------------------------------------- */

double aver_buf_d( long *buf, long ind_anf, long ind_end)   
{
  long             ind;
  long             anz;
  double           anz_d;
  double           val_d;
  double           sum_d;
  double           result_d;


  anz = ind_end - ind_anf;
  anz_d = (double) anz;

  sum_d = (double) 0;
  for (ind = ind_anf; ind < ind_end; ind++)
  {
    val_d = (double) buf[ind];
    sum_d = sum_d + val_d;
  }
  if (sum_d > 0)
  {
    result_d = (sum_d + anz_d/2) / anz_d;
  }
  else
  {
    result_d = (sum_d - anz_d/2) / anz_d;
  }
  return result_d;

}  /* end aver_buf_d */

/* ======================================================================== */


/* ---------------------------------------------------------------------------
f_title: aver_buf
------------------------------------------------------------------------------
kind   : C_Function
purpose: 
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: - buf = Quellbufferpointer
output_para: -
--------------------------------------------------------------------------- */

long aver_buf(long *buf, long ind_anf, long ind_end)
{
   return (long) aver_buf_d(buf,ind_anf,ind_end);
}  /* end aver_buf */

/* ======================================================================== */
/* ---------------------------------------------------------------------------
f_title: rampe_linear
------------------------------------------------------------------------------
kind   : C_procedure                                      hans.mayer@besota.de
purpose: 
Stellt eine lineare Stellwert-Rampe.
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -
output_para: -
--------------------------------------------------------------------------- */
long rampe_linear(tPropTlm *propTlm,long hStlk,long security,long val_anf,long val_end,double dauer,long hTimer)
{
  long          val_stl;
  long          tme_act;
  long          delta_time=0;
  double        delta_time_d=0.0;
  double        lambda;
  long          ecode;

  dauer = 1000.0 * dauer; /* dauer in Mykrosec*/
  ecode = pvAsk ( hTimer, 0, security, METH_GET, 0, sizeof(long), (void*) &tme_act );
  if (ecode == 0)
  {
    while (((double)delta_time < dauer) && (ecode == 0)  && (propTlm->breakFlag != 0))
    {
      
      lambda = (double)delta_time / dauer;
      val_stl = (long)( (1.0 - lambda) * (double) val_anf + lambda * (double)val_end);
      ecode = pvAsk ( hStlk, 0, security, METH_SET,(void *)&(val_stl),0,NULL);
#ifndef WIN32
			taskDelay(2); /* wait for 2ms */
#else
			Sleep(2);
#endif
      if (ecode == 0)
      {
        ecode = pvAsk(hTimer,0,security, METH_GETELAPSEDTIME,(void*) &tme_act, sizeof(long), (void*) &delta_time);
      }			
    }
    ecode = pvAsk ( hStlk, 0, security, METH_SET,(void *)&(val_end),0,NULL);
  }
  return ecode;
} /* end of rampe_linear */

/* ======================================================================== */

/* ---------------------------------------------------------------------------
f_title: mes_mit_abs
------------------------------------------------------------------------------
kind   : C_function
purpose: 
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -
output_para: -
--------------------------------------------------------------------------- */

long mes_mit_abs(long hObj,long hTimer, long security, long anz_vf,long filter,long anz_nf)
{
  long           val1;
  long           val2;
  long           val3;
  long           mittel;
  long           anz_ges;
  long           cnt;
  double         summe;

  long           ecode;


  if (!anz_vf || !anz_nf)
  {
    return 0;
  }
  anz_ges = anz_vf * anz_nf;

  summe = 0;
  if (filter)
  {
    if (anz_vf > 1)                 /* groesser Eins bedeutet: Vormittelung */
    {
      for (cnt = 0; (cnt < anz_nf); cnt++)
      {
        val1 = mes_mittel_abs (hObj,hTimer, security, anz_vf);
        val2 = mes_mittel_abs (hObj,hTimer, security, anz_vf);
        val3 = mes_mittel_abs (hObj,hTimer, security, anz_vf);
        
        summe = summe + auswahl_mittel (val1, val2, val3);

      } /* end of for (cnt = 0; (cnt < anz_nf); cnt++) */
      
      if (summe > 0)
      {
        mittel = (long) ((summe + (anz_nf/2))/ anz_nf);
      }
      else
      {
        mittel = (long) ((summe - (anz_nf/2))/ anz_nf);
      }
    }
    else           /* anz_vf == Null oder Eins bedeutet: keine Vormittelung */
    {
      if (anz_nf > 1)              /* groesser Eins bedeutet: Nachmittelung */
      {
        for (cnt = 0; (cnt < anz_nf); cnt++)
        {
          ecode = pvAsk ( hObj, 0, 0, METH_GET, 0, 
                          sizeof(long), (void*) &val1 );
          if (ecode == 0)
          {
            ecode = pvAsk ( hObj, 0, 0, METH_GET, 0, 
                            sizeof(long), (void*) &val2 );
            if (ecode == 0)
            {
              ecode = pvAsk ( hObj, 0, 0, METH_GET, 0, 
                            sizeof(long), (void*) &val3 );
            }
          }
                    
          summe = summe + auswahl_mittel (val1, val2, val3);
  
        } /* end of for (cnt = 0; (cnt < anz_nf); cnt++) */
        
        if (summe > 0)
        {
          mittel = (long)((summe + (anz_nf/2))/ anz_nf);
        }
        else
        {
          mittel = (long)((summe - (anz_nf/2))/ anz_nf);
        }
      }
      else        /* anz_nf == Null oder Eins bedeutet: keine Nachmittelung */
      {
        ecode = pvAsk ( hObj, 0, 0, METH_GET, 0, 
                          sizeof(long), (void*) &val1 );
        if (ecode == 0)
        {
          ecode = pvAsk ( hObj, 0, 0, METH_GET, 0, 
                          sizeof(long), (void*) &val2 );
          if (ecode == 0)
          {
            ecode = pvAsk ( hObj, 0, 0, METH_GET, 0, 
                          sizeof(long), (void*) &val3 );
          }
        }          
        mittel = auswahl_mittel (val1, val2, val3);
      }
    }
  }
  else  /* else of if (filter) */
  {
    mittel = mes_mittel_abs (hObj,hTimer,security, anz_ges);
  }
  return mittel;

} /* end of mes_mit_abs */

/* ======================================================================== */

/* ---------------------------------------------------------------------------
f_title: aver_buf_end
------------------------------------------------------------------------------
kind   : C_Function
purpose: 
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -
output_para: -
--------------------------------------------------------------------------- */

long aver_buf_end (long hBuffer, long security, int ind_anf, int ind_end)   
{
  long          ind;
  long          anz;
  double        anz_d;
  double        val_d;
  long          val_l;
  double        sum_d = 0.0;
  long          result;
  double        result_d;

  anz = ind_end - ind_anf;
  anz_d = (double) anz;

  result = CONST_VAL_DIV_0;

  if (anz)
  {
    sum_d = (double) 0;
    for (ind = ind_anf; ind < ind_end; ind++)
    {
      pvAsk (hBuffer, ind, security, METH_GET, 0, sizeof(long), (void*) &val_l );
      val_d = (double) val_l;
      sum_d = sum_d + val_d;
    }
    if (sum_d > 0.0)
    {
      result_d = sum_d / anz_d + 0.5;
    }
    else
    {
      result_d = sum_d / anz_d - 0.5;
    }
    result = (long) result_d;
  }
  return result;

} /* end of aver_buf_end */

/* ======================================================================== */

/* ---------------------------------------------------------------------------
f_title: ari_double
------------------------------------------------------------------------------
kind   : C_Function
purpose: 
Multiplikation: Operanden int, Ergebnis double
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -
output_para: -
--------------------------------------------------------------------------- */

double ari_double ( int ope1, int ope2, int mode)
{
  double        ope1_d;
  double        ope2_d;
  double        resu_d;

  resu_d = (double) CONST_VAL_DIV_0;

  if (mode == 1)
  {
    ope1_d = (double) ope1;
    ope2_d = (double) ope2;
    resu_d = ope1_d + ope2_d;
  }
  else
  {
    if (mode == 2)
    {
      ope1_d = (double) ope1;
      ope2_d = (double) ope2;
      resu_d = ope1_d - ope2_d;
    }
    else
    {
      if (mode == 3)
      {
        ope1_d = (double) ope1;
        ope2_d = (double) ope2;
        resu_d = ope1_d * ope2_d;
      }
      else
      {
        if (mode == 4) 
        {
          if (ope2)
          {
            ope1_d = (double) ope1;
            ope2_d = (double) ope2;
            resu_d = ope1_d / ope2_d;
          }
        }
      }
    }
  }
  return resu_d;
  
} /* end of ari_double */

/* ---------------------------------------------------------------------------
f_title: ind_anz_tme
------------------------------------------------------------------------------
kind   : C_procedure
purpose: 
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -
output_para: -
--------------------------------------------------------------------------- */

long ind_anz_tme(long ind_anz_ges,long dauer_ges,long tme_tat,long* fehl)              
{
  double        ind_anz_ges_d;  /* Gesamtindexanzahl                   */
  double        dauer_ges_d;    /* Gesamtdauer                         */
  double        tme_tat_d;      /* tatsaechliche Dauer                 */
  double        ind_anz_d;
  long          ind_anz;
  

  ind_anz = val_touch;
 

  if (!*fehl)
  {
    if (dauer_ges)
    {
      ind_anz_ges_d = (double) ind_anz_ges;
      dauer_ges_d   = (double) dauer_ges;
      tme_tat_d     = (double) tme_tat;

      ind_anz_d = tme_tat_d * ind_anz_ges_d / dauer_ges_d;

      ind_anz = (long) ind_anz_d;
    }
    else
    {
      *fehl = ERR_NULL_DIVISION;
    }
  }
  
  
  return ind_anz;

} /* end of ind_anz_tme */


/* ======================================================================== */

/* ---------------------------------------------------------------------------
f_title: aver_buf_anz
------------------------------------------------------------------------------
kind   : C_Function
purpose: 
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -
output_para: -
--------------------------------------------------------------------------- */

long aver_buf_anz(long hBuffer,long security,long ind_anf, long anz)   
{
  long          ind;
  long          ind_end;
  double        anz_d;
  long          val;
  double        val_d;
  double        sum_d;
  long          result;
  double        result_d;

  ind_end = ind_anf + anz;

  anz_d = (double) anz;

  result = CONST_VAL_DIV_0;

  if (anz)
  {
    sum_d = (double) 0;
    for (ind = ind_anf; ind < ind_end; ind++)
    {
      pvAsk ( hBuffer, ind, security, METH_GET, 0, sizeof(long), (void*) &val);
      val_d = (double) val;
      sum_d = sum_d + val_d;
    }
    if (sum_d > 0)
    {
      result_d = (sum_d + anz_d/2) / anz_d;
    }
    else
    {
      result_d = (sum_d - anz_d/2) / anz_d;
    }
    result = (int) result_d;
  }
  return result;

} /* end of aver_buf_anz */

/* ======================================================================== */
/* ---------------------------------------------------------------------------
f_title: mes_mit_rel
------------------------------------------------------------------------------
kind   : C_function
purpose: 
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -
output_para: -
--------------------------------------------------------------------------- */

long mes_mit_rel(long hMeskA, long hMeskB, long hTimer, long security, long anz_vf, long filter, long anz_nf)
{
  /* 
      hMeskA = Messkanal A Detektionskanal         
      hMeskB = Messkanal B Bezugskanal             
      anz_vf = Anzahl Mittelungen vor Filter       
      filter = Filter-Schalter                     
      anz_nf = Anzahl Mittelungen nach Filter      
  */

  long           ecode;
  long           vala1;
  long           vala2;
  long           vala3;
  long           valb1;
  long           valb2;
  long           valb3;
  long           dif1=0;
  long           dif2=0; 
  long           dif3=0; 
  long           mittel;
  long           anz_ges;
  long           cnt;
  double         summe;


  if (!anz_vf || !anz_nf)
  {
    return CONST_VAL_DIV_0;
  }
  anz_ges = anz_vf * anz_nf;

  summe = 0;
  if (filter)
  {
    if (anz_vf > 1)                 /* groesser Eins bedeutet: Vormittelung */
    {
      for (cnt = 0; (cnt < anz_nf); cnt++)
      {
        dif1 = mes_mittel_rel (hMeskA, hMeskB, hTimer, security, anz_vf);
        dif2 = mes_mittel_rel (hMeskA, hMeskB, hTimer, security, anz_vf);
        dif3 = mes_mittel_rel (hMeskA, hMeskB, hTimer, security, anz_vf);
        
        summe = summe + auswahl_mittel (dif1, dif2, dif3);

      } /* end of for (cnt = 0; (cnt < anz_nf); cnt++) */
      
      if (summe > 0)
      {
        mittel = ((long)(summe + (double)(anz_nf/2)))/ anz_nf;
      }
      else
      {
        mittel = ((long)(summe - (double)(anz_nf/2)))/ anz_nf;
      }
    }
    else           /* anz_vf == Null oder Eins bedeutet: keine Vormittelung */
    {
      if (anz_nf > 1)              /* groesser Eins bedeutet: Nachmittelung */
      {
        for (cnt = 0; (cnt < anz_nf); cnt++)
        {
          ecode = pvAsk ( hMeskA, 0, 0, METH_GET, 0, sizeof(long), (void*) &vala1);
          if (ecode == 0)
          {
            ecode = pvAsk ( hMeskB, 0, 0, METH_GET, 0, sizeof(long), (void*) &valb1);
            if (ecode == 0)
            {
              ecode = pvAsk ( hMeskA, 0, 0, METH_GET, 0, sizeof(long), (void*) &vala2);
              if (ecode == 0)
              {
                ecode = pvAsk ( hMeskB, 0, 0, METH_GET, 0, sizeof(long), (void*) &valb2);
                if (ecode == 0)
                {
                  ecode = pvAsk ( hMeskA, 0, 0, METH_GET, 0, sizeof(long), (void*) &vala3);
                  if (ecode == 0)
                  {
                    ecode = pvAsk ( hMeskB, 0, 0, METH_GET, 0, sizeof(long), (void*) &valb3);
                  }
                }
              }
            }
          }
        
          dif1 = vala1 - valb1;
          dif1 = vala2 - valb2;
          dif1 = vala3 - valb3;
          
          summe = summe + auswahl_mittel (dif1, dif2, dif3);         
  
        } /* end of for (cnt = 0; (cnt < anz_nf); cnt++) */
        
        if (summe > 0)
        {
          mittel = ((long)(summe + (double)(anz_nf/2)))/ anz_nf;
        }
        else
        {
          mittel = ((long)(summe - (double)(anz_nf/2)))/ anz_nf;
        }
      }
      else        /* anz_nf == Null oder Eins bedeutet: keine Nachmittelung */
      {
        ecode = pvAsk ( hMeskA, 0, 0, METH_GET, 0, sizeof(long), (void*) &vala1);
        if (ecode == 0)
        {
          ecode = pvAsk ( hMeskB, 0, 0, METH_GET, 0, sizeof(long), (void*) &valb1);
          if (ecode == 0)
          {
            ecode = pvAsk ( hMeskA, 0, 0, METH_GET, 0, sizeof(long), (void*) &vala2);
            if (ecode == 0)
            {
              ecode = pvAsk ( hMeskB, 0, 0, METH_GET, 0, sizeof(long), (void*) &valb2);
              if (ecode == 0)
              {
                ecode = pvAsk ( hMeskA, 0, 0, METH_GET, 0, sizeof(long), (void*) &vala3);
                if (ecode == 0)
                {
                  ecode = pvAsk ( hMeskB, 0, 0, METH_GET, 0, sizeof(long), (void*) &valb3);
                }
              }
            }
          }
        }
        dif1 = vala1 - valb1;
        dif1 = vala2 - valb2;
        dif1 = vala3 - valb3;
          
        mittel = auswahl_mittel (dif1, dif2, dif3);
      }
    }
  }
  else  /* else of if (filter) */
  {
    mittel = mes_mittel_rel (hMeskA, hMeskB, hTimer, security, anz_ges);
  }
  return mittel;

} /* end of mes_mit_rel */

/* ======================================================================== */

/* ---------------------------------------------------------------------------
f_title: prt_alr_ran
------------------------------------------------------------------------------
kind   : C_procedure
purpose: 
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -
output_para: -
--------------------------------------------------------------------------- */

long prt_alr_ran (long* hVar, long* p_valu, long* p_valo, long* p_vals, long* ecode, long* tl_error)  
{
    /* p_valu untere Toleranz                     */
    /* p_valo obere Toleranz                      */
    /* p_vals relativer Range                     */
    /* p_pntr Ptr auf Parameter-Ptr des Fehlerpar.*/
   
  tPvObj    *pObj;

  long           vala;                /* Kernwert            */
  long           valu;                /* Wert unter Toleranz */
  long           valo;                /* Wert obere Toleranz */
  long           vals;                /* relativer Range     */
  long           prtan;
  long           security=0;

  vala = 0;
  valu = 0;
  valo = 0;
  vals = 0;
    
  pObj = getRealObjAddress( *hVar);
  if ( pObj == NULL)
  {
    *ecode = ERR_NO_SUCH_OBJECT;
  }
  else
  {
    if (!*ecode )
    {
      if (*hVar)
      {
        if (pObj->dimension > 2)    
        {                       /* Protokollvariablen-Array aus PRT Elementen */
        /*v_read (p_var, &prtan, 6);*/
          *ecode = pvAsk ( *hVar, 0, security, METH_GET, 0, sizeof(long), (void*) &prtan);
/*         if (prtan == val_prtan)
          {        Stellwert mit Kernwert und mit asymmetrischen Toleranzen */
          /*v_read (p_var, &vala, 0);
            v_read (p_var, &valu, 1);
            v_read (p_var, &valo, 2);
            v_read (p_var, &vals, 5);*/
            *ecode = pvAsk ( *hVar, 0, security, METH_GET, 0, sizeof(long), (void*) &vala);
            if (*ecode == 0)
            {
              *ecode = pvAsk ( *hVar, 1, security, METH_GET, 0, sizeof(long), (void*) &valu);
              if (*ecode == 0)
              {
                *ecode = pvAsk ( *hVar, 2, security, METH_GET, 0, sizeof(long), (void*) &valo);
                if (*ecode == 0)
                {
                  *ecode = pvAsk ( *hVar, 5, security, METH_GET, 0, sizeof(long), (void*) &vals);
                }
              }
/*            } */
          }
          else
          {
            *ecode = ERR_ARRAY_PRT;
          }
        }
        else
        {
          if (pObj->dimension == 2)
          {           /* Stellwert ohne Kernwert und symmetrischen Toleranzen */
          /*v_read (p_var, &valu, 0);
            v_read (p_var, &valo, 1);*/
            *ecode = pvAsk ( *hVar, 0, security, METH_GET, 0, sizeof(long), (void*) &valu);
            if (*ecode == 0)
            {
              *ecode = pvAsk ( *hVar, 1, security, METH_GET, 0, sizeof(long), (void*) &valo);
            }
            vala = (valu + valo) / 2;
            vals = valo - valu;
          }
          else
		      {
    		    if (pObj->dimension == 1)    
		      	{                                                   /* Einzelwert */
              /*v_read (p_var, &vala, 0);*/
              *ecode = pvAsk ( *hVar, 0, security, METH_GET, 0, sizeof(long), (void*) &vala);
			      }
            else
			      {
              *tl_error = ERR_ARRAY_PRT_ALR_ONE;
			      }
		      }
        }
      }
      else
      {
        *tl_error = ERR_PTR_PARA_NULL;
      }
    }
  }
  *p_valu = valu;
  *p_valo = valo;
  *p_vals = vals;
  

  return vala;

} /* end of prt_alr_ran */

/* ---------------------------------------------------------------------------
f_title: prt_alr_one
------------------------------------------------------------------------------
kind   : C_procedure
purpose: 
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -
output_para: -
--------------------------------------------------------------------------- */

long prt_alr_nug(long* hVar, void**p_pntr, long* ecode)  
{
                /* p_pntr Ptr auf Parameter-Ptr des Fehlerpar.*/

  tPvObj    *pObj;

  void*         pntr;                /* Parameter-Ptr des Fehlerparameters  */
  long          vala;                                /* Kernwert            */
  long          valu;                                /* Wert unter Toleranz */
  long          valo;                                /* Wert obere Toleranz */
  long          prtan;
  long           security=0;


  vala = 0;
  pntr = *p_pntr;
  
  pObj = getRealObjAddress( *hVar);
  if ( pObj == NULL)
  {
    *ecode = ERR_NO_SUCH_OBJECT;
  }
  else
  {
    if (!(*ecode))
    {
      if (*hVar)
      {
      /*if ((*(V_OBJEKT*) p_var).feldgroesse == PRT)    */
        if (pObj->dimension > 2)    
        {                       /* Protokollvariablen-Array aus PRT Elementen */
/*        v_read (p_var, &prtan, 6);*/
          *ecode = pvAsk ( *hVar, 0, security, METH_GET, 0, sizeof(long), (void*) &prtan);

/*        if (prtan == val_prtan)
          {}       /* Stellwert mit Kernwert und mit asymmetrischen Toleranzen */
/*            v_read (p_var, &vala, 0);*/
          *ecode = pvAsk ( *hVar, 0, security, METH_GET, 0, sizeof(long), (void*) &vala);

          
/*        else
          {
            *ecode = ERR_ARRAY_PRT;
            pntr = p_var;
          }*/
        }
        else
        {
/*        if ((*(V_OBJEKT*) p_var).feldgroesse == 2)    */
          if (pObj->dimension == 2)
          {           /* Stellwert ohne Kernwert und symmetrischen Toleranzen */
/*          v_read (p_var, &valu, 0);
            v_read (p_var, &valo, 1);*/
            *ecode = pvAsk ( *hVar, 0, security, METH_GET, 0, sizeof(long), (void*) &valu);
            if (*ecode == 0)
            {
              *ecode = pvAsk ( *hVar, 1, security, METH_GET, 0, sizeof(long), (void*) &valo);
            }
            vala = (valu + valo + 1) / 2;
          }
          else
          {
/*          if ((*(V_OBJEKT*) p_var).feldgroesse == 1)    */
            if (pObj->dimension == 1)
            {                                                   /* Einzelwert */
/*             v_read (p_var, &vala, 0);*/
               *ecode = pvAsk ( *hVar, 0, security, METH_GET, 0, sizeof(long), (void*) &vala);
            }
            else
            {
              *ecode = ERR_ARRAY_PRT_ALR_ONE;
              pntr = hVar;
            }
          }
        }
      }
      else
      {
        *ecode = ERR_PTR_PARA_NULL;
      }
    }
  }
  *p_pntr = pntr;

  return vala;

} /* end of prt_alr_nug */

/* ---------------------------------------------------------------------------
f_title: getDirection
------------------------------------------------------------------------------
kind   : C_procedure
purpose: finds out if a Variable is read or write
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: Variable Handle
output_para: direction
             - 0 for input
             - 1 for output
             - 2 for error
--------------------------------------------------------------------------- */

long getDirection(long hVar, long* ecode)  
{
  long direction; /* 0 in / 1 out */
  long hClass;
  tPvObj    *pObj;
  tPvRspGetClassInfo classInfo;

  pObj = getObjAddress( hVar);
	if ( pObj == NULL)
	{
		*ecode = ERR_NO_SUCH_OBJECT;
	}
	else
	{
    hClass = pObj->classHandle;
    *ecode = pvAsk(PV_SERVER, 0, 0, METH_GETCLASSINFO, &hClass, sizeof(classInfo), &classInfo);
    direction = classInfo.direction;
  }
  return direction;
}

/* ---------------------------------------------------------------------------
f_title: getDimension
------------------------------------------------------------------------------
kind   : C_procedure
purpose: returns the dimesion of an variable
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: Variable Handle
output_para: dimension
--------------------------------------------------------------------------- */

long getDimension(long hVar, long* ecode)  
{
  long dimension;

  tPvObj    *pObj;

  pObj = getObjAddress( hVar);
	if ( pObj == NULL)
	{
		*ecode = ERR_NO_SUCH_OBJECT;
	}
	else
	{
    dimension = pObj->dimension;
  }
  return dimension;
}

/* ---------------------------------------------------------------------------
f_title: interpol_index
------------------------------------------------------------------------------
kind   : C_Function
purpose: 
Multiplikation: Operanden int    , Ergebnis double
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -
output_para: -
--------------------------------------------------------------------------- */

double interpol_index(long val_schwelle,long val_alt, long val_neu,long ial_alt, long ial_neu)
{
  double        vs;
  double        va;
  double        vn;
  double        ia;
  double        in;
  double        index_schwelle;

  vs = (double) val_schwelle;
  va = (double) val_alt;
  vn = (double) val_neu;
  ia = (double) ial_alt;
  in = (double) ial_neu;

  if (vn == va)
  {
    index_schwelle = ia;
  }
  else
  {
    index_schwelle = ia + (((vs-va) * (in-ia)) / (vn-va));
  }
  return index_schwelle;
  
} /* interpol_index */


/* ======================================================================== */

/* ---------------------------------------------------------------------------
f_title: tme_ind_anz
------------------------------------------------------------------------------
kind   : C_procedure
purpose: 
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -
output_para: -
--------------------------------------------------------------------------- */

long tme_ind_anz(long ind_anz_ges, long dauer_ges, double ind_anz_d, long* tl_error)
{
  
  /* ind_anz_ges,          Gesamtindexanzahl                  */
  /* dauer_ges,            Gesamtdauer                         */
  /* ind_anz_d,            tatsaechliche Indexanzahl           */
  /* tl_error              Modulfehler */

  double        ind_anz_ges_d;
  double        dauer_ges_d;
  double        tme_tat_d;
  long          tme_tat;

  tme_tat = val_touch;
  
  if (ind_anz_ges)
  {
    ind_anz_ges_d = (double) ind_anz_ges;
    dauer_ges_d = (double) dauer_ges;

    tme_tat_d = ind_anz_d * dauer_ges_d / ind_anz_ges_d;

    tme_tat = (long) tme_tat_d;
  }
  else
  {
    *tl_error = ERR_NULL_DIVISION;
  }
  
  return tme_tat;

} /* end of tme_ind_anz */

/* ======================================================================== */

/* ---------------------------------------------------------------------------
f_title: abs_double
------------------------------------------------------------------------------
kind   : C_Function
purpose: 
Multiplikation: Operanden int, Ergebnis double
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -
output_para: -
--------------------------------------------------------------------------- */

double abs_double(long ope1)       
{
  double        ope1_d;

  if (ope1 < 0)
  {
    ope1_d = - (double) ope1;
  }
  else
  {
    ope1_d = (double) ope1;
  }
  return ope1_d;
  
} /* end of abs_double */

/* ======================================================================== */

/* ---------------------------------------------------------------------------
f_title: stan_buf_anz
------------------------------------------------------------------------------
kind   : C_Function
purpose: 
------------------------------------------------------------------------------
 input_glob: -
output_glob: -

 input_para: -
output_para: -
--------------------------------------------------------------------------- */

long stan_buf_anz(long hBuf, long ind_anf,long anz,long aver,long security, long* ecode)   
{
  /* hBuf               Quellbufferhandle                  */

  long          ind;
  long          ind_end;
  double        anz_d;
  long          val;
  double        val_d;
  double        sum_d;
  double        aver_d;
  long          result;
  double        result_d;
  double        varianz_d;

  ind_end = ind_anf + anz;
  aver_d = (double) aver; 
  anz_d = (double) anz;

  result = CONST_MIN_VALUE;

  if (anz > 1)
  {
    sum_d = (double) 0;
    for (ind = ind_anf; ind < ind_end; ind++)
    {
      *ecode = pvAsk ( hBuf, ind, security, METH_GET, 0, sizeof(long), (void*) &val);
      val_d = (double)val;
      sum_d += ((aver_d - val_d) * (aver_d - val_d));
    }
    if (sum_d < 0)
    {
      result = CONST_MIN_VALUE;
    }
    else
    {
      varianz_d = sqrt (sum_d); 

      result_d = (varianz_d + (anz_d-1)/2) / (anz_d-1);

      result = (int) result_d;
    }
  }
  return result;

} /* end of stan_buf_anz */

/* ======================================================================== */
