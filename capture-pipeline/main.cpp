/*
 * main.cpp
 *
 *  Created on: 26 Nis 2016
 *      Author: Turan Murat Güvenç
 */
#include "CaptureFilter.h"
#include "DecodeFilter.h"
#include "OutputFilter.h"
#include "GrayscaleFilter.h"
#include "ObjectPool.h"

int main()
{
	CaptureFilter captureFilter("/dev/video0");
	DecodeFilter decodeFilter;
	GrayscaleFilter grayscaleFilter;
	OutputFilter outputFilter;

	tbb::pipeline pipeline;

	pipeline.add_filter(captureFilter);
	pipeline.add_filter(decodeFilter);
	pipeline.add_filter(grayscaleFilter);
	pipeline.add_filter(outputFilter);

	pipeline.run(100);

	return 0;
}
