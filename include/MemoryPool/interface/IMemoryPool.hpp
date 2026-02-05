/*
	Name: IMemoryPool.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 03/10/25 16:37
	Description: 内存池接口
*/

#pragma once

#include "Exception.hpp"
#include "StamonLib.hpp"

namespace stamon::interface {

template<class Impl> class IMemoryPool : public Impl {
	// 内存池，请用智能指针托管该类对象，或手动创建并释放
public:
	IMemoryPool(STMException *e, int mem_limit, int pool_cache_size)
		: Impl(e, mem_limit, pool_cache_size) {
        //mem_limit为内存极限，pool_cache_size为内存池缓存大小
	}
	template<typename T, typename... Types> T *NewObject(Types &&...args) {
        //向内存池申请一个对象，用法：NewObject<申请类型>(构造参数...)
		return Impl::template NewObject<T>(forward<Types>(args)...);
	}
	void clearAllFreeMem() {
        //清除内存池的所有空闲内存，以此减小内存占用
		return Impl::clearAllFreeMem();
	}
	template<typename T> void DeleteObject(T *object) {
        //删除内存池上的一个对象
		return Impl::DeleteObject(object);
	}
};

} // namespace stamon