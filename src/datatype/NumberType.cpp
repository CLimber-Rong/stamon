/*
	Name: NumberType.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 14/08/23 10:09
	Description: 数字类型的定义，包括整数和浮点数
*/

#pragma once

#include"DataType.hpp"

namespace stamon::datatype {
	class IntegerType : public DataType {
			int val;
		public:
			IntegerType(int value) : DataType(IntegerTypeID) {
				val = value;
			}
			virtual int getVal() const {
				return val;
			}
			virtual ~IntegerType() = default;
	};

	class FloatType : public DataType {
			float val;
		public:
			FloatType(float value) : DataType(FloatTypeID) {
				val = value;
			}
			virtual float getVal() {
				return val;
			}
			virtual ~FloatType() = default;
	};

	class DoubleType : public DataType {
			double val;
		public:
			DoubleType(double value) : DataType(DoubleTypeID) {
				val = value;
			}
			virtual double getVal() {
				return val;
			}
			virtual ~DoubleType() = default;
	};
} //namespace stamon::datatype