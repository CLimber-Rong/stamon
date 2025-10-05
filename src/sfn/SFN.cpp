/*
	Name: SFN.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 20/02/24 15:39
	Description: Stamon For Native，即本地库的实现
*/

#pragma once

#include "SFNException.cpp"
#include "Stamon.hpp"
#include "StamonConfig.hpp"
#include "BasicIo.hpp"

#define SFN_PARA_LIST \
	stamon::sfn::SFN &sfn, stamon::datatype::Variable *arg, \
			stamon::vm::ObjectManager *manager

// 用这个宏（SFN Parameter List）可以快速声明SFN函数的参数列表

namespace stamon::sfn {
class SFN;
} // namespace stamon::sfn

stamon::String DataType2String(stamon::STMException *ex, stamon::datatype::DataType *dt);

void sfn_input(SFN_PARA_LIST);
void sfn_print(SFN_PARA_LIST);

void sfn_int(SFN_PARA_LIST);
void sfn_str(SFN_PARA_LIST);
void sfn_len(SFN_PARA_LIST);

void sfn_typeof(SFN_PARA_LIST);
void sfn_throw(SFN_PARA_LIST);
void sfn_system(SFN_PARA_LIST);
void sfn_exit(SFN_PARA_LIST);
void sfn_version(SFN_PARA_LIST);

namespace stamon::sfn {
class SFN {
	HashMap<String, void(*)(SFN_PARA_LIST)> sfn_functions;
	// 定义一个函数指针map
public:
	STMException *ex;

	vm::ObjectManager *manager;

	SFN() {
	}

	SFN(STMException *e, vm::ObjectManager *objman) {
		ex = e;

		manager = objman;

		// 在这里将库函数按接口号填入
		sfn_functions.put(String("print"), sfn_print);
		sfn_functions.put(String("int"), sfn_int);
		sfn_functions.put(String("str"), sfn_str);
		sfn_functions.put(String("len"), sfn_len);
		sfn_functions.put(String("input"), sfn_input);
		sfn_functions.put(String("typeof"), sfn_typeof);
		sfn_functions.put(String("throw"), sfn_throw);
		sfn_functions.put(String("system"), sfn_system);
		sfn_functions.put(String("exit"), sfn_exit);
		sfn_functions.put(String("version"), sfn_version);
	}
	void call(String port, datatype::Variable *arg) {
		if (sfn_functions.exist(port) == 0) {
			THROW(exception::sfn::SFNError("call()", "undefined sfn port"));
		} else {
			sfn_functions.get(port)(*this, arg, manager);
		}
	}
};
} // namespace stamon::sfn

stamon::String DataType2String(stamon::STMException *ex, stamon::datatype::DataType *dt) {

	using namespace stamon;

	switch (dt->getType()) {
	case datatype::IntegerTypeID:
		return toString(((datatype::IntegerType *) dt)->getVal());

	case datatype::FloatTypeID:
		return toString(((datatype::FloatType *) dt)->getVal());

	case datatype::DoubleTypeID:
		return toString(((datatype::DoubleType *) dt)->getVal());

	case datatype::StringTypeID:
		return String("\"")
			 + ((datatype::StringType *) dt)->getVal()
			 + String("\"");

	case datatype::NullTypeID:
		return String("null");

	case datatype::SequenceTypeID: {
		String rst("{ ");

		ArrayList<datatype::Variable *> list;

		list = ((datatype::SequenceType *) dt)->getVal();

		for (int i = 0, len = list.size(); i < len; i++) {
			rst = rst + DataType2String(ex, list[i]->data);

			if (i != len - 1) {
				// 如果不是最后一个元素，那么就在元素末尾加逗号
				rst = rst + String(", ");
			}
		}

		rst = rst + String(" }");

		return rst;
	}

	case datatype::ClassTypeID:
		return String("<class>");

	case datatype::MethodTypeID:
		return String("<function>");

	case datatype::ObjectTypeID:
		return String("<object>");

	default: {
		THROW(exception::sfn::SFNError(
				"DataType2String()", "unknown data-type"));
		return String("");
	}
	}
}

void sfn_print(SFN_PARA_LIST) {

	using namespace stamon;

	STMException *ex = sfn.ex;
	datatype::DataType *val = arg->data;
	if (val->getType() != datatype::StringTypeID) {
		THROW(exception::sfn::SFNError(
				"sfn_print()", "invalid type"));
		return;
	}
	platform_print(((datatype::StringType *) val)->getVal());
	return;
}

void sfn_int(SFN_PARA_LIST) {

	using namespace stamon;

	STMException *ex = sfn.ex;
	datatype::DataType *val = arg->data;

	switch (val->getType()) {
	case datatype::IntegerTypeID: {
		return;
	}

	case datatype::FloatTypeID: {
		arg->data = manager->MallocObject<datatype::IntegerType>(
				(int) (((datatype::FloatType *) val)->getVal()));
		break;
	}

	case datatype::DoubleTypeID: {
		arg->data = manager->MallocObject<datatype::IntegerType>(
				(int) (((datatype::DoubleType *) val)->getVal()));
		break;
	}

	case datatype::StringTypeID: {
		String src = ((datatype::StringType *) val)->getVal();
		int dst = 0;

		for (int i = 0; i < src.length(); i++) {
			if ('0' <= src[i] && src[i] <= '9') {
				dst *= 10;
				dst += src[i] - '0';
			} else {
				THROW(exception::sfn::SFNError(
						"sfn_int()", "invalid integer format"));
				return;
			}
		}

		arg->data = manager->MallocObject<datatype::IntegerType>(dst);
		break;
	}

	default: {
		THROW(exception::sfn::SFNError("sfn_int()", "invalid type"));
	}
	}

	return;
}

void sfn_str(SFN_PARA_LIST) {

	using namespace stamon;

	STMException *ex = sfn.ex;
	datatype::DataType *val = arg->data;

	if (arg->data->getType() == datatype::StringTypeID) {
		return;
	}

	arg->data = manager->MallocObject<datatype::StringType>(
			DataType2String(ex, val));

	return;
}

void sfn_len(SFN_PARA_LIST) {

	using namespace stamon;

	STMException *ex = sfn.ex;
	datatype::DataType *val = arg->data;

	if (val->getType() == datatype::SequenceTypeID) {
		arg->data = manager->MallocObject<datatype::IntegerType>(
				((datatype::SequenceType *) val)->getVal().size());

	} else if (val->getType() == datatype::StringTypeID) {
		arg->data = manager->MallocObject<datatype::IntegerType>(
				((datatype::StringType *) val)->getVal().length());

	} else {
		THROW(exception::sfn::SFNError("sfn_int()", "invalid type"));
	}

	return;
}

void sfn_input(SFN_PARA_LIST) {

	using namespace stamon;

	STMException *ex = sfn.ex;
	datatype::DataType *val = arg->data;

	String s = platform_input();

	arg->data = manager->MallocObject<datatype::StringType>(String(s));

	return;
}

#define CHECK_DATA_TYPE_ID(type) \
	if (val->getType() == datatype::type##ID) { \
		rst = String(#type); \
	}

void sfn_typeof(SFN_PARA_LIST) {

	using namespace stamon;

	STMException *ex = sfn.ex;
	datatype::DataType *val = arg->data;
	String rst;

	CHECK_DATA_TYPE_ID(NullType)
	CHECK_DATA_TYPE_ID(IntegerType)
	CHECK_DATA_TYPE_ID(FloatType)
	CHECK_DATA_TYPE_ID(DoubleType)
	CHECK_DATA_TYPE_ID(StringType)
	CHECK_DATA_TYPE_ID(SequenceType)
	CHECK_DATA_TYPE_ID(ClassType)
	CHECK_DATA_TYPE_ID(MethodType)
	CHECK_DATA_TYPE_ID(ObjectType)

	arg->data = manager->MallocObject<datatype::StringType>(rst);

	return;
}

void sfn_throw(SFN_PARA_LIST) {

	using namespace stamon;

	STMException *ex = sfn.ex;
	String err_msg = ((datatype::StringType *) arg->data)->getVal();
	THROW(exception::sfn::SFNError("sfn_throw()", err_msg));
	return;
}

void sfn_system(SFN_PARA_LIST) {

	using namespace stamon;

	STMException *ex = sfn.ex;
	int status = platform_system(
			((datatype::StringType *) arg->data)->getVal());
	arg->data = manager->MallocObject<datatype::IntegerType>(status);
	return;
}

void sfn_exit(SFN_PARA_LIST) {

	using namespace stamon;

	platform_exit(((datatype::IntegerType *) arg->data)->getVal());
}

void sfn_version(SFN_PARA_LIST) {

	using namespace stamon;

	arg->data = manager->MallocObject<datatype::StringType>(
			toString(config::STAMON_VER_X) + String(".")
			+ toString(config::STAMON_VER_Y) + String(".")
			+ toString(config::STAMON_VER_Z));
	return;
}

#undef SFN_PARA_LIST
#undef CHECK_DATA_TYPE_ID