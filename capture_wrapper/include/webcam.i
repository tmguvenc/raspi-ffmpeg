/* File : capture.i */
%module WebcamModule
%include std_vector.i
%include various.i
%template(FrameBuffer) std::vector<unsigned char>;

%{
#include "webcam.h"
#include "decode.h"
%}

/* Let's just grab the original header file here */

%include std_string.i
%include cpointer.i
%include "webcam.h"
%include "decode.h"