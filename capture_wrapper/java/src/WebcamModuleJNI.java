/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package tr.tmg.murat.guvenc;

public class WebcamModuleJNI {
  public final static native long new_FrameBuffer__SWIG_0();
  public final static native long new_FrameBuffer__SWIG_1(long jarg1);
  public final static native long FrameBuffer_size(long jarg1, FrameBuffer jarg1_);
  public final static native long FrameBuffer_capacity(long jarg1, FrameBuffer jarg1_);
  public final static native void FrameBuffer_reserve(long jarg1, FrameBuffer jarg1_, long jarg2);
  public final static native boolean FrameBuffer_isEmpty(long jarg1, FrameBuffer jarg1_);
  public final static native void FrameBuffer_clear(long jarg1, FrameBuffer jarg1_);
  public final static native void FrameBuffer_add(long jarg1, FrameBuffer jarg1_, short jarg2);
  public final static native short FrameBuffer_get(long jarg1, FrameBuffer jarg1_, int jarg2);
  public final static native void FrameBuffer_set(long jarg1, FrameBuffer jarg1_, int jarg2, short jarg3);
  public final static native void delete_FrameBuffer(long jarg1);
  public final static native long new_Webcam(String jarg1, int jarg2, int jarg3, int jarg4, int jarg5);
  public final static native void delete_Webcam(long jarg1);
  public final static native long Webcam_getFrame(long jarg1, Webcam jarg1_);
  public final static native long new_Decode(int jarg1, int jarg2);
  public final static native void delete_Decode(long jarg1);
  public final static native void Decode_decode(long jarg1, Decode jarg1_, long jarg2, FrameBuffer jarg2_, byte[] jarg3);
}