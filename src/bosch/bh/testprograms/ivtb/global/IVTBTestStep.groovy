package bosch.bh.testprograms.ivtb.global

import bosch.bh.include.platform.teststep.TestFunction
import bosch.bh.spex.sdk.common.PVLong
import bosch.bh.ts3000.pvserver.GroovyPVObject

abstract class IVTBTestStep extends TestFunction {
  void set(PVLong... objects) {
    Closure[] setClosures = new Closure[objects.length]
    for (int i = 0; i < objects.length; i++) {
      GroovyPVObject obj = objects[i]
      setClosures[i] = { Object value ->
        obj.set(1)
      }
    }
    parallel(setClosures)
  }

  void reset(PVLong... objects) {
    Closure[] resetClosures = new Closure[objects.length]
    for (int i = 0; i < objects.length; i++) {
      GroovyPVObject obj = objects[i]
      resetClosures[i] = { Object value ->
        obj.set(0)
      }
    }
    parallel(resetClosures)
  }
}
