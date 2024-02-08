/*
	Name:  Preprocessor.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 05/02/24 17:49
	Description: 代码预处理器
*/

#ifndef PREPROCESSOR_CPP
#define PREPROCESSOR_CPP

#include"stmlib.hpp"
#include"Exception.hpp"
#include"ArrayList.hpp"
#include"FileMap.hpp"
#include"String.hpp"
#include"LineReader.hpp"

#include"Ast.hpp"
#include"Parser.cpp"
#include"ParsingQueue.cpp"


#define CE \
	CATCH {\
		ErrorMsg.add(\
		             String((char*)"Error: ")\
		             + String(ex->getError())\
		            );\
		return project;\
	}
//如果执行代码中出现异常，直接返回

namespace stamon {
	namespace c {

		class SourceSyntax {	//用来存储一个文件的文件名和程序
			public:
				ast::AstNode* program;
				String filename;
		};

		class Preprocessor {
				
			public:
				STMException* ex = NULL;

				ArrayList<String> ErrorMsg;

				ParsingQueue* list;

				Preprocessor(const Preprocessor& prr) {
					ex = prr.ex;
					list = prr.list;
				}

				Preprocessor(STMException* e) {
					ex = e;
					list = new ParsingQueue(ex);
				}

				ArrayList<SourceSyntax> ParseSource(
				    String origin, bool isSupportImport
				) {
					//以origin为源码的起点，开始分析

					SyntaxScope global_scope(ex);
					global_scope.isWall = true;

					list->AddSource(origin);

					ArrayList<SourceSyntax> project;

					CE

					while(list->file_cache.empty()==false) {
						//只要还有文件尚未分析
						String filename = list->file_cache[0];
						list->file_cache.erase(0); //弹出第一个文件进行分析

						LineReader reader = list->map.mark(filename);
						CE

						//进行词法分析
						int lineNo = 1;
						Lexer lexer(ex);

						while(reader.isMore()) {
							String text = reader.getLine();
							CE

							int index = lexer.getLineTok(
							                lineNo, text
							            );

							CATCH {
								THROW_S(
								    String((char*)"Error: at \"")
								    + filename
								    + String((char*)"\": ")
								    + toString(lineNo)
								    + String((char*)":")
								    + toString(index)
								    + String((char*)" : ")
								    + ex->getError()
								)
								ErrorMsg.add(ex->getError());

								ex->isError = false;
							}
						}

						Matcher matcher(lexer, ex);
						Parser* parser = new Parser(
						    matcher, ex, global_scope,
							filename, list, isSupportImport
						);

						ast::AstNode* node = parser->Parse();	//语法分析

						CATCH {
							THROW_S(
							    String((char*)"Syntax Error: at \"")
							    + filename
							    + String((char*)"\": ")
							    + toString(parser->ParsingLineNo)
							    + String((char*)": ")
							    + ex->getError()
							)
							ErrorMsg.add(ex->getError());

							ex->isError = false;
						}

						SourceSyntax syntax;
						syntax.program = node;
						syntax.filename = filename;

						project.add(syntax);
						//把该文件分析出的代码存入program
					}

					return project;

				}
		};
	}
}

#undef CE

#endif