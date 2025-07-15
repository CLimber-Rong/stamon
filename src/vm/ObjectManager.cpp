/*
	Name: ObjectManager.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 29/09/23 16:56
	Description: 对象管理器
*/

#pragma once

//备注：这个对象管理器的GC还需要再更改（需要把操作栈当中的对象也作为GCRoots）

#define FREE_OBJECT(obj, type, typeID)\
	if(obj->getType()==typeID) {\
		type* p = (type*)obj;\
		Pool.DeleteObject(p);\
		MemConsumeSize -= sizeof(type);\
	} else
//这个宏用来简便FreeObject方法的开发
//这个宏只能在ObjectManager.cpp当中使用

#include"DataType.hpp"
#include"MemoryPool.hpp"
#include"ArrayList.hpp"
#include"Stack.hpp"
#include"NumberMap.hpp"
#include"ObjectManagerException.cpp"
#include"stmlib.hpp"

namespace stamon::vm {
	typedef datatype::Variable Variable;

	class ObjectScope {		//单个对象作用域
			NumberMap<Variable> scope; //实际上存储的是DataType指针
		public:

			ObjectScope() {}
			ObjectScope(NumberMap<Variable> s) {
				scope = s;
			}

			bool exist(int key) {	//是否存在该标识符
				return scope.containsKey(key);
			}

			Variable* get(int key) {
				//除非你能保证要获取的变量一定存在，否则你最好先用exist函数检查
				return scope.get(key);
			}

			void put(int key, Variable* object) {	//存储一个标识符
				scope.put(key, object);
				return;
			}

			NumberMap<Variable> getScope() {
				return scope;
			}

			ArrayList<datatype::DataType*> getObjects() {
				ArrayList<Variable*> var;
				ArrayList<datatype::DataType*> obj;

				var = getScope().getValList<Variable*>();
				
				for(int i=0,len=var.size(); i<len; i++) {
					obj.add(var.at(i)->data);
				}

				return obj;
			}
			
			void destroyScope() {
				ArrayList<Variable*> var;

				var = getScope().getValList<Variable*>();

				for(int i=0;i<var.size();i++) {
					delete var[i];
				}
			}
	};

	class ObjectManager {
			unsigned long long MemConsumeSize;     //占用内存大小，按字节计
			unsigned long long MemLimit;	//对象占用内存的最大限制，按字节计
			ArrayList<datatype::DataType*> Objects; //用一个列表维护所有对象
			ArrayList<ObjectScope> Scopes;		//当前的作用域用一个列表来表示
			MemoryPool Pool;				//对象内存池
			//最新的作用域在列表末尾，全局作用域在列表开头

			datatype::NullType NullConst;

			/*
			 * 在新建左值变量的时候，会给变量赋null
			 * 此时的null来自于这里
			 * 这个null值不参与gc
			 */

			datatype::IntegerType* InterningIntegerConst[5+1+256] = {NULL};
			//先全部初始化为0
			/*
			 * 虚拟机在运行过程当中通常需要频繁申请一些小整数
			 * 通过把小整数放到栈的方式，可以减小内存分配次数，从而优化性能
			 * 这里我规定小整数的范围是-5~256
			 * 这种机制通常被称为小整数池，我更喜欢叫做整数复用池
			 */


		public:

			ArrayList<datatype::DataType*> OPND; //正在计算中的数据，也是GcRoot
			STMException* ex;
			bool is_gc;

			ObjectManager() {}

			ObjectManager(
			    bool isGC, unsigned long long mem_limit,
				int pool_cache_size, STMException* e
			) : Pool(e, mem_limit, pool_cache_size) {
				//构造函数，mem_limit表示最大内存限制，按字节计
				MemConsumeSize = 0;
				MemLimit = mem_limit;

				//初始化一些常量对象
				NullConst.gc_flag = true;	//初始化NullConst，它不参与gc
				for(int i=0;i<5+1+256;i++) {
					InterningIntegerConst[i] = new datatype::IntegerType(i-5);
					InterningIntegerConst[i]->gc_flag = true;
					//初始化范围是-5~256，它们不参与gc
				}

				is_gc = isGC;
				ex = e;
			}

			datatype::NullType* getNullConst() {
				return &NullConst;
			}

			template<class T>
			bool GCConditions(T* object) {
				/*
				* 用于判断此时是否要进行GC
				* 开发者也可以选择重写这个函数
				* 有些开发者编写的GC条件取决于object
				* 所以我编写了这个参数，但是我不会用到
				* 这里默认当对象占用内存以及GC预留内存大于内存限制时触发GC
				*/
				if(is_gc==false) {
					return false;
				}
				int TotalConsumeSize =
				    MemConsumeSize
				    + sizeof(datatype::DataType*) * Objects.size();
				//总占用内存=对象占用内存+GC预留内存
				if(TotalConsumeSize>=MemLimit) {
					return true;
				}
				return false;
			}



			/*
			 * 开发者应该用MallocObject函数创建对象
			 * 这个函数的用法是MallocObject<对象类型>()
			 *
			 * 举例：
			 * 已知引用了stamon::datatype和stamon::vm
			 * 已知有名为manager的ObjectManager对象、名为s的String对象
			 * 创建一个内容为s的字符串对象，应该这么做
			 * StringType* str;
			 * str = manager.MallocObject<StringType>(s);
			*/

			template<class T, typename...Types>
			T* MallocObject(Types&& ...args) {
				//这个代码比较难懂，涉及到形参模板和右值引用
				return _malloc_object<T>(args...);
			}



			/*
			 * _malloc_object是对象管理器对于分配对象的主体实现部分
			 * 开发者不应该直接调用此接口
			 */

			template<class T, typename...Types>
			T* _malloc_object(Types&& ...args) {

				//这个代码比较难懂，涉及到形参模板和右值引用
				T* result;      //要返回的对象

				result = Pool.NewObject<T>(args...);		//从内存池新建对象

				CATCH {		//如果GC报错就退出函数
					return NULL;
				}

				MemConsumeSize += sizeof(T);   //更新内存占用数

				if(GCConditions<T>(result)) {      //如果满足GC条件
					GC();
					CATCH {		//如果GC报错就退出函数
						return NULL;
					}
				}

				if(MemConsumeSize>MemLimit) {
					//如果GC后内存还是不够，就报错
					THROW(
						exception::objectmanager::MemoryError("_malloc_object()")
					);
					return NULL;
				}

				if(result==NULL) {
					//如果物理内存不足，就报错
					THROW(
						exception
						::objectmanager
						::PhysicalMemoryError("_malloc_object()")
					);
					return NULL;
				}

				/*
				 * 注意：一定要在GC后才能添加到列表，否则刚申请的对象可能会被GC掉
				*/

				result->gc_flag = false;

				Objects.add(result);    //添加对象到列表

				return result;  //返回对象
			}

			/*
			 * 对于对象申请的优化，我利用了模板特化，对整数申请和空值申请进行了特化
			 * 由于编译器要求，我把代码实现放到了末尾
			 */

			Variable* NewVariable(int id) {
				Variable* result = new Variable();
				result->data = &NullConst;
				Scopes.at(Scopes.size()-1).put(id, result);
				return result;
			}

			Variable* NewVariable(int id, datatype::DataType* val) {
				Variable* result = new Variable();
				result->data = val;
				Scopes.at(Scopes.size()-1).put(id, result);
				return result;
			}

			Variable* GetVariable(int id) {
				//从最新的作用域到全局作用域逐个查找
				for(int i=Scopes.size()-1; i>=0; i--) {
					if(Scopes.at(i).exist(id) == true) {
						return Scopes.at(i).get(id);
					}
				}
				THROW(
					exception::objectmanager::IdentifierError("GetVariable()")
				);	//未定义标识符
				return NULL;
			}

			//正常的作用域操作

			void PushScope() {
				ObjectScope scope;
				Scopes.add(scope);
			}

			void PopScope() {
				Scopes[Scopes.size()-1].destroyScope();
				Scopes.erase(Scopes.size()-1);
			}

			/*
			 * 特别的，在初始化类对象时，会将类对象地成员表作为一个作用域进行入栈
			 * 于是，初始化语句中的定义变量、函数、类就会存入这个作用域
			 * 在初始化结束后再弹出该作用域
			 * 这种方法可以优雅地把成员定义存入到类对象中
			 */

			void PushMemberTabScope(ObjectScope s) {
				//将成员表作为作用域入栈
				Scopes.add(s);
			}

			void PopMemberTabScope() {
				//将成员表作用域弹出栈，不destroyScope
				Scopes.erase(Scopes.size()-1);
			}

			ObjectScope getTopScope() {
				return Scopes[Scopes.size()-1];
			}

			void GC() {
				//垃圾回收函数，是整个项目最难的部分之一
				//采用标准的标记清除算法

				//标记非垃圾对象

				//先把操作数标记为非垃圾对象
				ArrayList<datatype::DataType*> opnd_unscanned = OPND;
				for(int i=0,len=OPND.size(); i<len; i++) {
					OPND[i]->gc_flag = true;
				}
				MarkScopeObject(opnd_unscanned);

				//再根据GC Root标记非垃圾对象
				for(int i=0; i<Scopes.size(); i++) {
					//遍历作用域
					ObjectScope scope = Scopes.at(i);
					//获取当前作用域的变量表
					ArrayList<datatype::DataType*> unscanned;
					//未扫描的对象列表
					InitUnscannedScope(scope, unscanned);
					//把作用域里的变量（也就是GCRoots）加载到unscanned里
					MarkScopeObject(unscanned);
					//遍历该作用域的变量涉及到的全部对象，并且标记他们
				}

				//清除垃圾对象
				CleanScopeTrash();
			}

			void InitUnscannedScope(
			    ObjectScope &scope,
			    ArrayList<datatype::DataType*> &unscanned
			) {
				unscanned = scope.getObjects();
				//把scope的GCRoots全部存到unscanned中
				for(int i=0,len=unscanned.size(); i<len; i++) {
					unscanned.at(i)->gc_flag = true; //把GCRoots标记为已搜索
				}
				return;
			}

			void MarkScopeObject(ArrayList<datatype::DataType*>& unscanned) {
				//遍历该作用域的变量涉及到的全部对象，并且标记他们
				while(unscanned.empty()==false) {
					int len = unscanned.size();
					/*从unscanned列表中删除某个对象o*/
					datatype::DataType* o = unscanned.at(len-1);
					unscanned.erase(len-1);
					/*for(在o中引用的每个对象p)*/

					if(o->getType()==datatype::SequenceTypeID) {
						//扫描列表当中的对象
						datatype::SequenceType* list 
												= (datatype::SequenceType*)o;

						for(int i=0,len=list->getVal().size();i<len;i++) {
							auto p = list->getVal()[i]->data;
							if(p->gc_flag==false) {
								/*
								 * 如果这个对象还没有被扫描过
								 * 那么标记，并放到unscanned中
								*/
								p->gc_flag = true;
								unscanned.add(p);
							}
						}

					}

					if(o->getType()==datatype::ObjectTypeID) {
						//扫描类对象引用的对象
						auto obj = (datatype::ObjectType*)o;

						ArrayList<Variable*> referVars = 
											 obj
											 ->getVal()
											 .getValList<Variable*>();
						//获取引用对象的列表

						for(int i=0,len=referVars.size();i<len;i++) {
							auto p = referVars[i]->data;
							if(p->gc_flag==false) {
								/*如果这个对象还没有被扫描过，
								那么标记，并放到unscanned中*/
								p->gc_flag = true;
								unscanned.add(p);
							}
						}

					}

					if(o->getType()==datatype::MethodTypeID) {
						//扫描类方法的容器
						auto func = (datatype::MethodType*)o;
						
						datatype::ObjectType* obj = func->getContainer();
						//获取容器

						if(obj!=NULL) {
							//有容器，容器也要扫描
							if(obj->gc_flag==false) {
								obj->gc_flag = true;
								unscanned.add(obj);
							}
						}
					}
				}
			}

			void CleanScopeTrash() {
				ArrayList<datatype::DataType*> NewObjects;
				//把清理后有用的对象存储在这个列表里

				for(int i=0,len=Objects.size();i<len;i++) {
					if(Objects.at(i)->gc_flag==false) {
						//垃圾对象
						FreeObject(Objects.at(i));	//释放对象
					} else {
						//非垃圾对象
						Objects.at(i)->gc_flag = false;	//把gc_flag设为false
						NewObjects.add(Objects.at(i));
					}
				}

				Objects = NewObjects;
				//更新对象列表
			}

			void FreeObject(datatype::DataType* o) {
				FREE_OBJECT(o, datatype::NullType, datatype::NullTypeID)
				FREE_OBJECT(o, datatype::IntegerType, datatype::IntegerTypeID)
				FREE_OBJECT(o, datatype::FloatType, datatype::FloatTypeID)
				FREE_OBJECT(o, datatype::DoubleType, datatype::DoubleTypeID)
				FREE_OBJECT(o, datatype::StringType, datatype::StringTypeID)
				FREE_OBJECT(o, datatype::SequenceType, datatype::SequenceTypeID)
				FREE_OBJECT(o, datatype::ClassType, datatype::ClassTypeID)
				FREE_OBJECT(o, datatype::MethodType, datatype::MethodTypeID)
				FREE_OBJECT(o, datatype::ObjectType, datatype::ObjectTypeID)
				THROW(exception::objectmanager::ObjectError("FreeObject()"));
			}

			~ObjectManager() {
				//释放所有对象
				for(int i=0,len=Objects.size(); i<len; i++) {
					FreeObject(Objects[i]);
				}
				//释放整数复用池
				for(int i=0;i<5+1+256;i++) {
					FreeObject(InterningIntegerConst[i]);
				}
				//释放内存池的剩余内存
				Pool.ClearAllFreeMem();
			}
	};


	/*
	* 此处运用到了模板特化，代码相对来说需要更长时间去理解
	* 利用模板特化，我们可以在编译期就直接判断代码调用的模板类型，从而进行优化
	* 例如，对于一个template<T>的函数f而言
	* 我们可以特别定义一个template<int>的函数f，然后进行一些更细节的处理
	* 这里我们特别优化了：
		* NullType的申请：
			* 直接返回null常量而不申请对象
		* IntegerType的申请：
			* 对于-5~256的整数，直接返回常量而不申请对象
			* 这种机制通常被称为小整数池，我喜欢叫做整数复用池
	*/

	template<>
	datatype::NullType*
	ObjectManager::MallocObject<datatype::NullType> () {
		//特别优化NullType
		return getNullConst();
	}

	template<>
	datatype::IntegerType*
	ObjectManager::MallocObject<datatype::IntegerType> (int&& val) {
		//利用整数复用池，来特别优化整数分配
		if(-5<=val && val<=256) {
			return InterningIntegerConst[val+5];
		}
		return _malloc_object<datatype::IntegerType>(val);
	}

} //namespace stamon::vm

#undef FREE_OBJECT