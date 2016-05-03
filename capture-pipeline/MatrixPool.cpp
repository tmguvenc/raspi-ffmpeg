/*
 * MatrixPool.cpp
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#include "MatrixPool.h"

ObjectPool::mat_pool_type* _pool_mat = new ObjectPool::mat_pool_type("mat",[](const MatPoolKey& key){
	ObjectPool::mat_pool_type::inner_factory f = [key](){
		#ifdef _DEBUG
				size_t esz = CV_ELEM_SIZE(key.type & CV_MAT_TYPE_MASK);
				void* data = flalloc(cv::alignSize(esz*key.rows * key.cols, 32));
				cv::Mat* mat = new cv::Mat(key.rows, key.cols, key.type, data, key.cols * esz);
				return std::shared_ptr<cv::Mat>(mat);
		#else
				return std::make_shared<cv::Mat>(key.rows, key.cols, key.type);
		#endif
	};
	return f;
});

ObjectPool::void_pool_type* _pool_void = new ObjectPool::void_pool_type("void",[](size_t key){
	ObjectPool::void_pool_type::inner_factory f = [key](){
		return std::shared_ptr<void>(malloc(key), [](void* ptr){free(ptr); });
	};
	return f;
});

ObjectPool::void_pool_type *ObjectPool::pool_void(){
	return _pool_void;
}

std::shared_ptr<cv::Mat> MatrixPool::create(const MatPoolKey& key){
	return create(key, true);
}

std::shared_ptr<cv::Mat> MatrixPool::tryCreate(const MatPoolKey& key){
	return create(key, false);
}

std::shared_ptr<cv::Mat> MatrixPool::create(const MatPoolKey& key, bool wait){

	auto esz = CV_ELEM_SIZE(key.type & CV_MAT_TYPE_MASK);
	auto size = cv::alignSize(esz*key.rows * key.cols, 65536);
	auto vp = _pool_void->create(size, wait);

	if (!vp){
		return std::shared_ptr<cv::Mat>();
	}

	auto mat = new cv::Mat(key.rows, key.cols, key.type, vp.get(), key.cols * esz);

	return std::shared_ptr<cv::Mat>(mat, [vp, size](cv::Mat* mat1){
		//ProvideoFastFunctions::set(*mat1, 0);
		*mat1 = cv::Mat::zeros(mat1->rows, mat1->cols, mat1->type());
		delete mat1;
	});
}

MatrixPool* matrixPool = new MatrixPool();

MatrixPool *ObjectPool::pool_mat()
{
	return matrixPool;
}
