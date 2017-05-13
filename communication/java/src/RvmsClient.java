/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package tr.tmg.murat.guvenc;

public class RvmsClient {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected RvmsClient(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(RvmsClient obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        CommunicationModuleJNI.delete_RvmsClient(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public RvmsClient(String ip, int port) {
    this(CommunicationModuleJNI.new_RvmsClient(ip, port), true);
  }

  public void start(byte[] BYTE) {
    CommunicationModuleJNI.RvmsClient_start(swigCPtr, this, BYTE);
  }

  public void stop() {
    CommunicationModuleJNI.RvmsClient_stop(swigCPtr, this);
  }

  public static void initialize() {
    CommunicationModuleJNI.RvmsClient_initialize();
  }

  public void readSensor() {
    CommunicationModuleJNI.RvmsClient_readSensor(swigCPtr, this);
  }

  public void moveRight() {
    CommunicationModuleJNI.RvmsClient_moveRight(swigCPtr, this);
  }

  public void moveUp() {
    CommunicationModuleJNI.RvmsClient_moveUp(swigCPtr, this);
  }

  public void moveDown() {
    CommunicationModuleJNI.RvmsClient_moveDown(swigCPtr, this);
  }

  public void moveLeft() {
    CommunicationModuleJNI.RvmsClient_moveLeft(swigCPtr, this);
  }

}
