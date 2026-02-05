/*
	Name: TypeCalculator.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 04/01/25 16:46
	Description: 类型计算器
*/

#pragma once

#include"Token.cpp"
#include"DataType.hpp"
#include"ObjectManager.cpp"

//为了方便开发，我编写了一系列简写的宏
#define DTT datatype::DataType
#define INT datatype::IntegerType 
#define FLT datatype::FloatType
#define DBL datatype::DoubleType
#define STR datatype::StringType
#define NUL datatype::NullType
#define SEQ datatype::SequenceType

#define IDINT datatype::IntegerTypeID
#define IDFLT datatype::FloatTypeID
#define IDDBL datatype::DoubleTypeID
#define IDSTR datatype::StringTypeID
#define IDNUL datatype::NullTypeID
#define IDSEQ datatype::SequenceTypeID

#define ISNUM(dt) (\
                    (dt->getType()==IDINT)\
                    ||(dt->getType()==IDFLT)\
                    ||(dt->getType()==IDDBL)\
                  )

#define NEWDT(type) manager->MallocObject<type>

#define BIND_UN(type, name) UnCalcFunc[type] = &TypeCalculator::name;
#define BIND_BIN(type, name) BinCalcFunc[type] = &TypeCalculator::name;

/*
 * 类型计算是个比较复杂的部分，我很难找到一个优雅的算法来统一所有运算符
 * 我将会建立一个由类方法指针所组成的数组，通过查找数组下标，可以快速找到相关的运算函数
 */

namespace stamon::vm {
    class TypeCalculator {

        public:

            datatype::DataType*
            (TypeCalculator::*UnCalcFunc[ast::UnaryTypeCount])
			(datatype::DataType* a);

            datatype::DataType*
            (TypeCalculator::*BinCalcFunc[ast::BinaryTypeCount])
			(datatype::DataType* a, datatype::DataType* b);
            //由类方法所组成的数组

            STMException* ex;
            ObjectManager* manager;

            TypeCalculator() {}

            TypeCalculator(STMException* e, ObjectManager* man) {
                ex = e;
                manager = man;

                //初始化单目运算函数
                BIND_UN(ast::UnaryPositiveType, UnaryPositive);
                BIND_UN(ast::UnaryNegativeType, UnaryNegative);
                BIND_UN(ast::UnaryNotType     , UnaryNot);
                BIND_UN(ast::UnaryInverseType , UnaryInverse);
                
                //初始化双目运算函数
                BIND_BIN(ast::BinaryLogicORType        , BinaryLogicOR);
                BIND_BIN(ast::BinaryLogicANDType       , BinaryLogicAND);
                BIND_BIN(ast::BinaryBitORType          , BinaryBitOR);
                BIND_BIN(ast::BinaryBitXORType         , BinaryBitXOR);
                BIND_BIN(ast::BinaryBitANDType         , BinaryBitAND);
                BIND_BIN(ast::BinaryEqualityType       , BinaryEquality);
                BIND_BIN(ast::BinaryInequalityType     , BinaryInequality);
                BIND_BIN(ast::BinaryBigThanType        , BinaryBigThan);
                BIND_BIN(ast::BinaryLessThanType       , BinaryLessThan);
                BIND_BIN(ast::BinaryBigThanOrEqualType , BinaryBigThanOrEqual);
                BIND_BIN(ast::BinaryLessThanOrEqualType, BinaryLessThanOrEqual);
                BIND_BIN(ast::BinaryLeftShiftType      , BinaryLeftShift);
                BIND_BIN(ast::BinaryRightShiftType     , BinaryRightShift);
                BIND_BIN(ast::BinaryAddType            , BinaryAdd);
                BIND_BIN(ast::BinarySubType            , BinarySub);
                BIND_BIN(ast::BinaryMultType           , BinaryMult);
                BIND_BIN(ast::BinaryDiviType           , BinaryDivi);
                BIND_BIN(ast::BinaryModType            , BinaryMod);

            }

            inline int AssignOperatorToBinaryOperator(int op) {
                //将运算赋值（例如S）
                switch (op)
                {
                case c::TokenAddAss: return ast::BinaryAddType;
				case c::TokenSubAss: return ast::BinarySubType;
				case c::TokenMulAss: return ast::BinaryMultType;
				case c::TokenDivAss: return ast::BinaryDiviType;
				case c::TokenModAss: return ast::BinaryModType;
				case c::TokenAndAss: return ast::BinaryBitANDType;
				case c::TokenXOrAss: return ast::BinaryBitXORType;
				case c::TokenOrAss:  return ast::BinaryBitORType;
				case c::TokenLSHAss: return ast::BinaryLeftShiftType;
				case c::TokenRSHAss: return ast::BinaryRightShiftType;
                
                default: return -1;
                }
            }

            inline bool checkUnaryOperator(int optype) {
                /*
                 * 检查该运算符是否合理，是则返回true，否则返回false
                 */
                return ast::UnaryPositiveType<=optype
                    && optype<=ast::UnaryTypeCount;
            }

            bool checkUnaryOperandType(
                datatype::DataType* a, int optype
            ) {
                /*
                 * 检查运算数的类型是否合理
                 * 合理则返回true，否则返回false
                 * 在调用此函数前，请先调用checkUnaryOperator来确保运算符是合理的
                 */
                switch (optype)
                {
                case ast::UnaryPositiveType:
                case ast::UnaryNegativeType:
                    //正负运算的类型判断相同，因此case被整合到一起
                    return ISNUM(a);
                case ast::UnaryNotType:
                    return true;
                case ast::UnaryInverseType:
                    return a->getType()==IDINT;
                default:
                    return false;
                }
            }

            datatype::DataType* UnaryOperate(
                datatype::DataType* a, int optype
            ) {
                /*
                 * 单目运算，给定需要运算的a，以及运算类型optype，计算后返回结果
                 * optype以ExprAst.cpp的_UnaryOperatorType为准
                 * 调用该函数之前请先分别调用：
                 * checkUnaryOperator和checkUnaryOperandType
                 * 来确保运算符的合理性和运算数类型的合理性
                 */
                return (this->*(UnCalcFunc[optype]))(a);
            }

            inline bool checkBinaryOperator(int optype) {
                /*
                 * 检查该运算符是否合理，是则返回true，否则返回false
                 */
                return ast::BinaryLogicORType<=optype
                    && optype<=ast::BinaryTypeCount;
            }

            bool checkBinaryOperandType(
                datatype::DataType* a, int optype, datatype::DataType* b
            ) {
                /*
                 * 检查运算数的类型是否合理
                 * 合理则返回true，否则返回false
                 * 在调用此函数前，请先调用checkBinaryOperator来确保运算符是合理的
                 */

                //部分运算的类型规则是相同的，因此我把case整合到一起，即跳转到同一代码
                switch(optype) {
                case ast::BinaryLogicORType:
                case ast::BinaryLogicANDType:
                    //任何类型都能参与逻辑运算
                    return true;

                case ast::BinaryBitORType:
                case ast::BinaryBitXORType:
                case ast::BinaryBitANDType:
                case ast::BinaryLeftShiftType:
                case ast::BinaryRightShiftType:
                    //只有整数能参与位运算
                    return a->getType()==IDINT && b->getType()==IDINT;
                
                case ast::BinaryEqualityType:
                case ast::BinaryInequalityType: 
                {   //注意：如果case代码块中要创建变量，则需要用花括号包围代码块
                    //如果同为数字，则可以判等
                    if(ISNUM(a) && ISNUM(b)) {
                        return true;
                    }
                    //如果是非数字且两者非同类，必然无法判等
                    if(a->getType()!=b->getType()) {
                        return false;
                    }
                    //在保障两者同类的情况下，字符串、空值、类、函数都可以判等
                    /*
                     * 我想要解释一下接下来的做法
                     * 显然，用if-else或者switch去逐个判断，是个效率低且冗杂的方法
                     * 因此，我利用了位运算的特性，编写了效率更快且更简洁的代码
                     * 我设立了一个整数变量range：
                     * range的第StringTypeID位、第NullTypeID位等等都被设置为1
                     * 其余位数被设置为0
                     * 如此一来，如果range的第a->getType()位是1
                     * 则a的类型是上述的类型中的一种，即a的类型是允许判等的
                     */
                    constexpr int range = (1<<datatype::StringTypeID)
                                        | (1<<datatype::NullTypeID)
                                        | (1<<datatype::ClassTypeID)
                                        | (1<<datatype::MethodTypeID);
                    return ((1<<a->getType()) & range) != 0;
                }

                case ast::BinaryBigThanType:
                case ast::BinaryLessThanType:
                case ast::BinaryBigThanOrEqualType:
                case ast::BinaryLessThanOrEqualType:
                    //如果是都是数字或者都是字符串，则可以判断
                    return (ISNUM(a) && ISNUM(b))
                         ||(a->getType()==b->getType() && a->getType()==IDSTR);
                
                case ast::BinaryAddType:
                {
                    //数字、字符串和数列都能参与加法运算
                    //为了使代码更简洁，我采用位运算来判断类型，原理同上
                    if(ISNUM(a) && ISNUM(b)) {
                        return true;
                    }
                    if(a->getType()!=b->getType()) {
                        return false;
                    }
                    if(a->getType()==IDSTR || a->getType()==IDSEQ) {
                        return true;
                    } else {
                        return false;
                    }
                }

                case ast::BinarySubType:
                case ast::BinaryMultType:
                case ast::BinaryDiviType:
                case ast::BinaryModType:
                    return ISNUM(a) && ISNUM(b);

                default:
                    return false;
                }
            }

            datatype::DataType* BinaryOperate(
                datatype::DataType* a, int optype, datatype::DataType* b
            ) {
                /*
                 * 双目运算，给定需要运算的a和b，以及运算类型optype，计算后返回结果
                 * optype以ExprAst.cpp的_BinaryOperatorType为准
                 * 调用该函数之前请先分别调用：
                 * checkBinaryOperator和checkBinaryOperandType
                 * 来确保运算符的合理性和运算数类型的合理性
                 */
                return (this->*(BinCalcFunc[optype]))(a, b);
            }

            //以下是一些用于数据类型转换的函数

            inline bool toBool(DTT* dt) {
                switch (dt->getType())
                {
                case IDINT:
                    return ((INT*)dt)->getVal()!=0;
                
                case IDFLT:
                    return ((FLT*)dt)->getVal()!=0.0f;
                
                case IDDBL:
                    return ((DBL*)dt)->getVal()!=0.0;

                case IDNUL:
                    return false;
                
                case IDSTR:
                    return !((STR*)dt)->getVal().empty();
                
                case IDSEQ:
                    return !((SEQ*)dt)->getVal().empty();
                
                case datatype::ClassTypeID:
                    return true;
                
                default:
                    return true;
                }
            }

            /*
             * 接下来我将编写toInt、toFloat和toDouble
             * 三个函数用于互相精度转换
             */

            inline int toInt(DTT* dt) {
                //调用此函数前请先确保dt是数字
                switch (dt->getType())
                {
                case IDINT:
                    return (int) ((INT*)dt)->getVal();
                case IDFLT:
                    return (int) ((FLT*)dt)->getVal();
                case IDDBL:
                    return (int) ((DBL*)dt)->getVal();
                default:
                    return 0;
                }
            }

            inline float toFloat(DTT* dt) {
                //调用此函数前请先确保dt是数字
                switch (dt->getType())
                {
                case IDINT:
                    return (float) ((INT*)dt)->getVal();
                case IDFLT:
                    return (float) ((FLT*)dt)->getVal();
                case IDDBL:
                    return (float) ((DBL*)dt)->getVal();
                default:
                   return 0;
                }
            }

            inline double toDouble(DTT* dt) {
                //调用此函数前请先确保dt是数字
                switch (dt->getType())
                {
                case IDINT:
                    return (double) ((INT*)dt)->getVal();
                case IDFLT:
                    return (double) ((FLT*)dt)->getVal();
                case IDDBL:
                    return (double) ((DBL*)dt)->getVal();
                default:
                    return 0;
                }
            }

        private:

            //以下是用于计算数据的函数

            DTT* UnaryPositive(DTT* a) {
                return a;
            }

            DTT* UnaryNegative(DTT* a) {
                switch (a->getType())
                {
                case IDINT:
                    return NEWDT(INT)(
                        -((INT*)a)->getVal()
                    );
                case IDFLT:
                    return NEWDT(FLT)(
                        -((FLT*)a)->getVal()
                    );
                case IDDBL:
                    return NEWDT(DBL)(
                        -((DBL*)a)->getVal()
                    );
                default:
                    return NULL;
                }
            }

            DTT* UnaryNot(DTT* a) {
                return NEWDT(INT)(!toBool(a));
            }
            
            DTT* UnaryInverse(DTT* a) {
                return NEWDT(INT)(
                    ~( ((INT*)a)->getVal() )
                );
            }

            DTT* BinaryLogicOR(DTT* a, DTT* b) {
                return NEWDT(INT)(toBool(a) || toBool(b));
            }

            DTT* BinaryLogicAND(DTT* a, DTT* b) {
                return NEWDT(INT)(toBool(a) && toBool(b));
            }

            //利用宏来简写运算的实现，下同
            #define DEFINE_BIT_OPERATE(name, op) \
            DTT* name(DTT* a, DTT* b) { \
                return NEWDT(INT)( \
                    (((INT*)a)->getVal()) \
                 op (((INT*)b)->getVal()) \
                ); \
            }

            DEFINE_BIT_OPERATE(BinaryBitOR, |);
            DEFINE_BIT_OPERATE(BinaryBitXOR, ^);
            DEFINE_BIT_OPERATE(BinaryBitAND, &);
            DEFINE_BIT_OPERATE(BinaryLeftShift, <<);
            DEFINE_BIT_OPERATE(BinaryRightShift, >>);
            //请在调用前确保位移数是非负数

            #undef DEFINE_BIT_OPERATE

            //将判断数字精度并计算的代码封装到一个宏
            #define MATH_CALCULATION(op) \
                /*从精度最高的DoubleType开始向下判断*/ \
                if(a->getType()==IDDBL || b->getType()==IDDBL) { \
                    return NEWDT(DBL)(toDouble(a) op toDouble(b)); \
                } \
                if(a->getType()==IDFLT || b->getType()==IDFLT) { \
                    return NEWDT(FLT)(toFloat(a) op toFloat(b)); \
                } \
                if(a->getType()==IDINT || b->getType()==IDINT) { \
                    return NEWDT(INT)(toInt(a) op toInt(b)); \
                }
            
            //将数字比较计算的代码封装到一个宏
            #define LOGIC_CALCULATION(op) \
                if(a->getType()==IDDBL || b->getType()==IDDBL) { \
                    return NEWDT(INT)(toDouble(a) op toDouble(b)); \
                } \
                if(a->getType()==IDFLT || b->getType()==IDFLT) { \
                    return NEWDT(INT)(toFloat(a) op toFloat(b)); \
                } \
                if(a->getType()==IDINT || b->getType()==IDINT) { \
                    return NEWDT(INT)(toInt(a) op toInt(b)); \
                }

            //封装判等和判不等的运算
            #define DEFINE_EQU_OPERATE(name, op) \
                DTT* name(DTT* a, DTT* b) { \
                    switch (a->getType()) \
                    { \
                    /*特判类和函数和空值*/ \
                    \
                    case datatype::ClassTypeID: \
                        return NEWDT(INT)( \
                                    ((datatype::ClassType*)a)->getVal() \
                                    op ((datatype::ClassType*)b)->getVal() \
                        ); \
                    \
                    case datatype::MethodTypeID: \
                        return NEWDT(INT)( \
                                    ((datatype::MethodType*)a)->getVal() \
                                    op ((datatype::MethodType*)b)->getVal() \
                        ); \
                    \
                    case datatype::NullTypeID: \
                        return NEWDT(INT)(true); \
                    \
                    default: \
                        break; \
                    } \
                    \
                    MATH_CALCULATION(op); \
                    \
                    return NULL; \
                }
            
            DEFINE_EQU_OPERATE(BinaryEquality, ==);
            DEFINE_EQU_OPERATE(BinaryInequality, !=);

            #undef DEFINE_EQU_OPERATE

            //封装大小比较的运算
            #define DEFINE_COMPARE_OPERATE(name, op) \
                DTT* name(DTT* a, DTT* b) { \
                    if(a->getType()==IDSTR) { \
                        return NEWDT(INT)( \
                            ((STR*)a)->getVal() \
                            op ((STR*)b)->getVal() \
                        ); \
                    } \
                    \
                    LOGIC_CALCULATION(op); \
                    \
                    return NULL; \
                    \
                }

            DEFINE_COMPARE_OPERATE(BinaryBigThan, >);
            DEFINE_COMPARE_OPERATE(BinaryLessThan, <);
            DEFINE_COMPARE_OPERATE(BinaryBigThanOrEqual, >=);
            DEFINE_COMPARE_OPERATE(BinaryLessThanOrEqual, <=);
            
            #undef DEFINE_COMPARE_OPERATE

            DTT* BinaryAdd(DTT* a, DTT* b) {

                //需要特判字符串和数列的加法
                if(a->getType()==IDSTR) {
                    return NEWDT(STR)(
                        ((STR*)a)->getVal()
                        + ((STR*)b)->getVal()
                    );
                }

                if(a->getType()==IDSEQ) {
                    return NEWDT(SEQ)(
                        ((SEQ*)a)->getVal()
                        + ((SEQ*)b)->getVal()
                    );
                }

                MATH_CALCULATION(+);

                return NULL;
            }

            DTT* BinaryMod(DTT* a, DTT* b) {
                //请在调用BinaryMod之前确保除数不为0

                //由于浮点取模较为复杂，无法用MATH_CALCULATION简写
                //因此我自己动手实现取模运算

                if(a->getType()==IDDBL || b->getType()==IDDBL) {
                    double x = toDouble(a);
                    double y = toDouble(b);
                    return NEWDT(DBL)(x - (long)(x / y) * y);
                }

                if(a->getType()==IDFLT || b->getType()==IDFLT) {
                    float x = toFloat(a);
                    float y = toFloat(b);
                    return NEWDT(FLT)(x - (long)(x / y) * y);
                }

                if(a->getType()==IDINT || b->getType()==IDINT) {
                    return NEWDT(INT)(toInt(a) % toInt(b)); 
                }

                return NULL;
            }

            #define DEFINE_MATH_OPERATE(name, op) \
                DTT* name(DTT* a, DTT* b) { \
                    MATH_CALCULATION(op); \
                    return NULL; \
                }

            DEFINE_MATH_OPERATE(BinarySub, -);
            DEFINE_MATH_OPERATE(BinaryMult, *);
            DEFINE_MATH_OPERATE(BinaryDivi, /);
            //请在调用BinaryDiv之前确保除数不为0

            #undef DEFINE_MATH_OPERATE


            #undef MATH_CALCULATION
            #undef LOGIC_CALCULATION

    };
}

#undef DTT
#undef INT
#undef FLT
#undef DBL
#undef STR
#undef NUL
#undef SEQ

#undef IDINT
#undef IDFLT
#undef IDDBL
#undef IDSTR
#undef IDNUL
#undef IDSEQ

#undef ISNUM

#undef NEWDT

#undef BIND_UN
#undef BIND_BIN