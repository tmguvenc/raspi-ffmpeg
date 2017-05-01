/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package tr.tmg.murat.guvenc;

public class FrameBuffer {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected FrameBuffer(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(FrameBuffer obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        WebcamModuleJNI.delete_FrameBuffer(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public FrameBuffer() {
    this(WebcamModuleJNI.new_FrameBuffer__SWIG_0(), true);
  }

  public FrameBuffer(long n) {
    this(WebcamModuleJNI.new_FrameBuffer__SWIG_1(n), true);
  }

  public long size() {
    return WebcamModuleJNI.FrameBuffer_size(swigCPtr, this);
  }

  public long capacity() {
    return WebcamModuleJNI.FrameBuffer_capacity(swigCPtr, this);
  }

  public void reserve(long n) {
    WebcamModuleJNI.FrameBuffer_reserve(swigCPtr, this, n);
  }

  public boolean isEmpty() {
    return WebcamModuleJNI.FrameBuffer_isEmpty(swigCPtr, this);
  }

  public void clear() {
    WebcamModuleJNI.FrameBuffer_clear(swigCPtr, this);
  }

  public void add(short x) {
    WebcamModuleJNI.FrameBuffer_add(swigCPtr, this, x);
  }

  public short get(int i) {
    return WebcamModuleJNI.FrameBuffer_get(swigCPtr, this, i);
  }

  public void set(int i, short val) {
    WebcamModuleJNI.FrameBuffer_set(swigCPtr, this, i, val);
  }

}
