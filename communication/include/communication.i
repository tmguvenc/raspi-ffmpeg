/* File : communication.i */
%module CommunicationModule
%include std_vector.i
%include various.i

%{
#include "rvms_client.h"
%}

/* Let's just grab the original header file here */

%include std_string.i
%include cpointer.i
%include "rvms_client.h"