/*
	Name: ObjectType.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 17/08/23 11:49
	Description:
    * 在STVM中，新建的类对象也是一个值，它对应的数据类型就是ObjectType
    * 这里就写上了ObjectType的定义
*/

#pragma once

#include"DataType.hpp"
#include"HashMap.hpp"

namespace stamon::datatype {
	class ObjectType : public DataType {
			HashMap<int, Variable*> vals;  
			//STVM内部以数字来代替标识符
		public:
			ObjectType(HashMap<int, Variable*> value)
				: DataType(ObjectTypeID), vals(HashMap<int, Variable*>()) {
				vals = value;
			}
			virtual HashMap<int, Variable*> getVal() const {
				return vals;
			}
			virtual ~ObjectType() {
				ArrayList<Variable*> vars = vals.getValList();
				for(int i=0;i<vars.size();i++) {
					delete vars[i];
				}
			}
	};

} //namespace stamon::datatype