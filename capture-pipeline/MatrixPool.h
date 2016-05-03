/*
 * MatrixPool.h
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef MATRIXPOOL_H_
#define MATRIXPOOL_H_

#include <opencv2/opencv.hpp>
#include "ObjectPool.h"

struct MatPoolKey
{
	MatPoolKey():
		rows(0), cols(0), type(0)
	{
	}

	MatPoolKey(int _rows, int _cols, int _type) :
			rows(_rows), cols(_cols), type(_type)
	{

	}
	static size_t hash(const MatPoolKey& x)
	{
		auto h1 = std::hash<int>()(x.rows * x.cols * 117);
		auto h2 = std::hash<int>()(x.type);

		return (h1 / 14) + h2;
	}

	static bool equal(const MatPoolKey& x, const MatPoolKey& y)
	{
		return x.rows == y.rows && x.cols == y.cols && x.type == y.type;
	}
	static std::string textFor(const MatPoolKey& key)
	{
		std::stringstream ss;
		ss << key.rows << " x " << key.rows;
		return ss.str();
	}
	static size_t maxPoolSize(const MatPoolKey& /*key*/)
	{
		return 450;
	}
	int rows;
	int cols;
	int type;
};

class EmptyMatCleaner
{
public:
	static void Pull(cv::Mat* /*mat*/)
	{
	}
	static void Push(cv::Mat* /*mat*/)
	{
	}
};

struct VoidPoolKeyHash
{
	static size_t hash(const size_t& x)
	{
		return x;
	}

	static bool equal(const size_t& x, const size_t& y)
	{
		return  x == y;
	}
	static void Pull(void* /*t*/)
	{
	}
	static void Push(void* /*t*/)
	{
	}
	static std::string textFor(size_t key)
	{
		std::stringstream ss;

		ss << (double)key / (1024 * 1024) << " MB";

		return ss.str();
	}
	static size_t maxPoolSize(size_t key){
		auto kb = key >> 10;
		auto mb = kb >> 10;
		if (mb)
			return 450;
		if (kb > 250)
			return 4500;
		return 45000;
	}
};

class MatCleaner
{
public:
	static void Pull(cv::Mat* mat)
	{
		assert(mat->rows != 0);
		assert(mat->cols != 0);
		assert(mat->data != nullptr);
#ifdef USE_IPP
		ippsSet_8u(0, (Ipp8u*)mat->data, mat->rows * (int)mat->step1());
#else
		memset(mat->data, 0, mat->rows * (int)mat->step1());
#endif
	}
	static void Push(cv::Mat* mat)
	{
		assert(mat->rows != 0);
		assert(mat->cols != 0);
		assert(mat->data != nullptr);
	}
};

class MatrixPool {
public:
	std::shared_ptr<cv::Mat> create(const MatPoolKey& key, bool wait);
	std::shared_ptr<cv::Mat> create(const MatPoolKey& key);
	std::shared_ptr<cv::Mat> tryCreate(const MatPoolKey& key);
};

class ObjectPool{
public:
	typedef parametric_object_pool<size_t, void, VoidPoolKeyHash, VoidPoolKeyHash> void_pool_type;
	typedef parametric_object_pool<MatPoolKey, cv::Mat, MatPoolKey, MatCleaner> mat_pool_type;

	static void_pool_type *pool_void();
	static MatrixPool *pool_mat();

private:
};

#endif /* MATRIXPOOL_H_ */
