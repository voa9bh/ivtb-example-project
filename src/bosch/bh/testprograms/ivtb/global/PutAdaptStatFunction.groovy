package bosch.bh.testprograms.ivtb.global

import bosch.bh.include.platform.teststep.TestFunction
import bosch.bh.spex.sdk.common.PVLong
import bosch.bh.ts3000.procs.StepReturn
import bosch.bh.ts3000.procs.testprogrambuilder.WithTestStepSpec

class PutAdaptStatFunction extends TestFunction implements WithTestStepSpec {

  public static final int CONST_DIV_TOUT = 1
  public static final int CONST_FAC_TOUT = 1

  @Override
  StepReturn apply(StepReturn stepReturn) {
    PutAdaptStatReturn putAdaptStatReturn = new PutAdaptStatReturn("PutAdaptStat")

    PutAdaptStatConfig putAdaptStatConfig = (PutAdaptStatConfig) configData

    int ecode = 0                /* errorcode */
    int tl_error = 0             /* tl errorcode */

    PVLong hTriggerOut           /* Triggerausgang                      <-> p_trag */
    PVLong hStlk                 /* Stellkanal                          <-> p_stlk */
    PVLong hMeskA                /* Messkanal A  zu Stellkanal korresp. <-> p_meska */
    PVLong hMeskB                /* Messkanal B  (Bezugskanal)          <-> p_meskb */

    /* Rückgabeparameter
    int           stpreal;
    int           stlreal;*/
    int mesw

    int relativ               /* Relativum zwischen 2 Sensoren */
    int inver                 /* Differenz-Sollwert-Invertierung     */
    int invers                /* Differenz-Sollwert-Invertierung     */
    int fac_mes
    int div_mes
    int stlr                  /* Stellwert-Regelgroesse              */
    int stlw                  /* Stellwertvorgabe                    */
    int stlz                  /* Stellziel mit Toleranzen angestrebt */
    int stlu                  /* Stellwert unter Toleranz angestrebt */
    int stlo                  /* Stellwert obere Toleranz angestrebt */
    int narrow                /* Prozentsatz der Toleranzeinengung   */
    int kompen                /* Deviationskompensationsprozentsatz  */
    int stl_old               /* letzter tatsaechlich gestellter Wert*/
    int tvor                  /* Ruhezeit fuer Stellwert stlw vorher */
    int tnac                  /* Ruhezeit fuer Stellwert stlw nacher */
    int tgrad                 /* Ruhezeit-Gradient                   */
    double dvor
    double dnac
    int anz_vf
    int filter
    int anz_nf
    int anz_ges
    int stpmin
    int stpmax
    int stl_limit = 0
    int truh                  /* Ruhezeit beide Stellwerte           */
    int abbruch
/*
  int           startTime;
  int           limit_u;
  int           diffTime;
  int           limit_o;
  int           found;
*/
    int stl_val               /* aktueller Stellwert wie ausgegeben  */
    int mesr                  /* Messwert-Regelgroesse               */
    int range
    int stpmax_pos
    int tges
    int flg_trag = 0
    int flg_mes_dif = 1
    int ready
    int trigger_low = 0
    int trigger_high = 1
    int stp
    int pause_time = 1    /* amout of 10 msecs to wait */

    hTriggerOut = putAdaptStatConfig.hTriggerOut as PVLong
    hStlk = putAdaptStatConfig.hStlk as PVLong
    hMeskA = putAdaptStatConfig.hMeskA as PVLong
    hMeskB = putAdaptStatConfig.hMeskB as PVLong

    relativ = putAdaptStatConfig.relativ
    inver = putAdaptStatConfig.inver
    fac_mes = putAdaptStatConfig.fac_mes
    div_mes = putAdaptStatConfig.div_mes

    stlw = putAdaptStatConfig.stlw
    stlz = putAdaptStatConfig.stlz

    narrow = putAdaptStatConfig.narrow
    kompen = putAdaptStatConfig.kompen
    tvor = putAdaptStatConfig.tvor
    tnac = putAdaptStatConfig.tnac
    tgrad = putAdaptStatConfig.tgrad
    anz_vf = putAdaptStatConfig.anz_vf
    filter = putAdaptStatConfig.filter
    anz_nf = putAdaptStatConfig.anz_nf
    stpmin = putAdaptStatConfig.stpmin
    stpmax = putAdaptStatConfig.stpmax
    stl_limit = putAdaptStatConfig.stl_limit

    stpmax_pos = (stpmax < 0) ? 0 : 1

    if (anz_vf < 0) {
      tl_error = ModError.ERR_ANZ_VOR_FILT_NEG
    } else {
      if (!anz_nf) {
        tl_error = ModError.ERR_ANZ_NACH_FILT_NULL
      } else {
        if (anz_nf < 0) {
          tl_error = ModError.ERR_ANZ_NACH_FILT_NEG
        }
      }
    }

    if (!tl_error) {
      if (flg_mes_dif) {
        if (inver && inver != 1) {
          tl_error = ModError.ERR_INVER_ZERO_ONE
        } else {
          if (inver) {
            invers = -1
          } else {
            invers = 1
          }
        }
      } else {
        if (inver) {
          tl_error = ModError.ERR_INVER_NOT_ALLOWED
        } else {
          invers = 1
        }
      }
    }

    if (!tl_error) {
      if (!div_mes) {
        tl_error = ModError.ERR_NULL_DIVISION
      } else {
        tvor = 1000 * tvor
        dvor = (double) tvor * (double) CONST_FAC_TOUT / (double) CONST_DIV_TOUT
        tvor = (int) dvor

        tnac = 1000 * tnac
        dnac = (double) tnac * (double) CONST_FAC_TOUT / (double) CONST_DIV_TOUT
        tnac = (int) dnac

        if (!anz_vf) anz_vf = 1
        if (!anz_nf) anz_nf = 1

        anz_ges = anz_vf * anz_nf

        /* Toleranzeinengung berechnen */
        stlu += ((narrow * (stlz - stlu)) / 100)
        stlo += ((narrow * (stlz - stlo)) / 100)
        stlr = stlz * invers
      }
    }

    stp = 0
    ready = 0
    abbruch = 0

    if (!tl_error) {
      stl_old = mes_mit_abs(hMeskA, propTlm -> hTimer, security, 1, 0, 20)

      while (((stp < stpmin) || ((stp < stpmax) && !ready)) &&
          !abbruch &&
          !tl_error) {
        if (!abbruch) {
          if (!stp) {
            stl_val = stlw
          } else {
            if (flg_mes_dif) {
              mesr = mes_mit_rel(hMeskA, hMeskB, anz_vf, filter, anz_nf)
              mesr -= relativ
            } else {
              mesr = mes_mit_abs(hMeskA, anz_vf, filter, anz_nf)
            }
            mesw = mesr * invers
            mesr = (mesr * fac_mes) / div_mes /* == mesr, da fac_mes und div_mes = 1 */
            stl_val = stl_val + (kompen * (stlr - mesr)) / 100 /* Sollwert um delta(soll-ist) korrigieren */
          }
        }

        if (!tl_error && !abbruch) {
          truh = (stp) ? tnac : tvor

          if (tgrad) {
            tges = truh + (((Math.abs(stl_val - stl_old))) / tgrad) /* [s + (mbar-mbar) / (bar/s) => ms] */
          } else {
            tges = truh
          }
          stl_old = stl_val
          if (stl_val < stl_limit) /* Limit reached? */ {
            if (ecode == 0) {
              hStlk.set(stl_val)
              /* Write new set value */
              if (ecode == 0) {
                hTriggerOut.set(trigger_high)
              }
            }

            waitTime(tges / 1000 as int)

            hTriggerOut.set(trigger_low)

            if (flg_mes_dif) {
              mesr = mes_mit_rel(hMeskA, hMeskB, propTlm.hTimer, security, anz_vf, filter, anz_nf)
              mesr -= relativ
            } else {
              mesr = mes_mit_abs(hMeskA, propTlm.hTimer, security, anz_vf, filter, anz_nf)
            }
            mesw = mesr * invers

            if ((mesw >= stlu) && (mesw <= stlo)) {
              ready = 1
            }
          } else /* Limit reached! */ {
            tl_error = ERR_THRESHOLD_PASSED
          }

        } /* end of if (!TL_error)... */
        stp++
      } /* end of while */
    } /* end of if (!fehl) */

    if (!ready && !tl_error && !abbruch)      /* Beendet wegen max Anz.erreicht */ {
      if (stpmax_pos) {
        tl_error = ERR_ADAPT_LIMIT             /* Toleranzfenster nicht erreicht */
      }
    }

    if (flg_mes_dif) {
      mesr = mes_mit_rel(hMeskA, hMeskB, propTlm.hTimer, security, anz_vf, filter, anz_nf)
      mesr -= relativ;
    } else {
      mesr = mes_mit_abs(hMeskA, propTlm.hTimer, security, anz_vf, filter, anz_nf)
    }

    mesw = mesr * invers

    if (ecode == 0) {
      putAdaptStatReturn.countCyc = stp
      putAdaptStatReturn.pressureSet = stl_old
      putAdaptStatReturn.pressureAct = mesw
    } else {  /* Return a TL_error that  an TS2000 error occured. */
      putAdaptStatReturn << tl_error
    }

    return putAdaptStatReturn
  }
}
