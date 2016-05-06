/*
 * GaborFilter.h
 *
 *  Created on: 6 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef GABORFILTER_H_
#define GABORFILTER_H_

#include "Filter.h"

#include <opencv2/core/core.hpp>

class GaborFilter : public Filter
{
public:
	explicit GaborFilter();
    virtual ~GaborFilter();

    void *operator ()(void* userData) override;
    void finalize(void *userData) override;

protected:
    void computeKernel();

private:

    int m_kernelSize;
    double m_theta;
    double m_psi;
    double m_lambda;
    double m_sigma;

	cv::Mat m_element, m_kernel;
};

#endif /* GABORFILTER_H_ */
