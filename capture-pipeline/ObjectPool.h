/*
 * ObjectPool.h
 *
 *  Created on: 3 May 2016
 *      Author: Turan Murat Güvenç
 */

#ifndef OBJECTPOOL_H_
#define OBJECTPOOL_H_

#include <memory>
#include <functional>
#include <vector>
#include <tbb/concurrent_queue.h>
#include <assert.h>
#include <tbb/atomic.h>
#include <tbb/concurrent_hash_map.h>

template <typename T>
class Nop
{
public:
	static void Pull(T* /*t*/)
	{
	}
	static void Push(T* /*t*/)
	{
	}
};

typedef std::shared_ptr<tbb::atomic<size_t>> counter;

static inline counter createCounter()
{
	auto cnt = std::make_shared<tbb::atomic<size_t>>();
	*cnt = 0;
	return cnt;
}

template <typename T, typename Initializer = Nop<T>>
class object_pool
{
public:
	typedef std::function<std::shared_ptr<T>(void)> factory;

	object_pool(std::string name, std::string key, factory _factory, size_t maximumPoolSize = 20000, counter _cnt = createCounter()) :
		factory_(_factory),
		cnt(_cnt),
		maxSize(maximumPoolSize),
		maximumReached(false)
	{
		//PoolRegistry::poolRegistry()->registerPool(name, key, _cnt, maximumPoolSize);
		pool_ = std::make_shared<tbb::concurrent_bounded_queue<std::shared_ptr<T>>>();
		assert(maxSize > 0);
	}

	std::shared_ptr<T> tryCreate(){
		return create(false);
	}
	std::shared_ptr<T> create(){
		return create(true);
	}

	double getRatio()
	{
		return static_cast<double>(*cnt - pool_->size()) / static_cast<double>(maxSize);
	}

	std::shared_ptr<T> create(bool wait)
	{
		std::shared_ptr<T> obj;
		if (!maximumReached)
		{
			if (!pool_->try_pop(obj))
			{
				auto currentCnt = cnt->fetch_and_increment();
				if (currentCnt > maxSize)
				{
					cnt->fetch_and_decrement();
					maximumReached = true;
					//Log.info("POOL IS FULL!!!!!");
					if (!wait)
						return obj;
					pool_->pop(obj);
				}
				else
				{
					obj = factory_();
				}
			}
		}
		else
		{
			if (!wait){
				if (!pool_->try_pop(obj))
					return obj;
			}
			else
				pool_->pop(obj);
		}
		// Automatically collects obj.
		Initializer::Pull(obj.get());
		auto pool = pool_;
		return std::shared_ptr<T>(obj.get(), [pool, obj](T*){
			Initializer::Push(obj.get());
			pool->push(obj);
		});
	}

private:
	std::shared_ptr<tbb::concurrent_bounded_queue<std::shared_ptr<T>>> pool_;
	factory factory_;
	counter cnt;
	size_t maxSize;
	bool maximumReached;
};

template <typename K, typename T, typename HashCompare, typename Initializer = Nop<T>>
class parametric_object_pool
{
public:
	typedef typename std::function<std::shared_ptr<T>(void)> inner_factory;
	typedef typename std::function<inner_factory(K)> factory;

	parametric_object_pool(const std::string& name, factory _factory) :
		m_map(), m_factory(_factory), m_counter(createCounter()), m_name(name)
	{
	}
	std::shared_ptr<T> create(K key)
	{
		return create(key, true);
	}
	std::shared_ptr<T> tryCreate(K key)
	{
		return create(key, false);
	}
	std::shared_ptr<T> create(K key, bool wait)
	{
		std::shared_ptr<object_pool<T, Initializer>> pool;
		typename the_map::accessor ac;

		if (m_map.insert(ac, key))
		{
			pool = std::make_shared<object_pool<T, Initializer>>(m_name, HashCompare::textFor(key), m_factory(key), HashCompare::maxPoolSize(key), createCounter());
			ac->second = pool;
		}
		else
		{
			pool = ac->second;
		}

		ac.release();
		return pool->create(wait);
	}

private:
	typedef typename tbb::concurrent_hash_map<K, std::shared_ptr<object_pool<T, Initializer>>, HashCompare> the_map;
	the_map m_map;
	factory m_factory;
	counter m_counter;
	std::string m_name;
};
#endif /* OBJECTPOOL_H_ */
