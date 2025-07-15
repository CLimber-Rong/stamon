/*
	Name: Compiler.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 22/02/24 12:31
	Description: 编译器头文件
*/

#pragma once

#include "Parser.cpp"

namespace stamon::c {
class Compiler {
public:
	FileMap filemap;
	SyntaxScope global_scope;
	ArrayList<SourceSyntax> *src;
	STMException *ex;
	ArrayList<String> *ErrorMsg;
	ArrayList<String> *WarningMsg;

	Compiler() {
	}

	Compiler(STMException *e, ArrayList<String> *error_msg,
			 ArrayList<String> *warning_msg)
		: filemap(e)
		, global_scope(e) {

		src = new ArrayList<SourceSyntax>();
		ex = e;
		ErrorMsg = error_msg;
		WarningMsg = warning_msg;
		
	}

	void compile(String filename, bool isSupportImport) {

		LineReader reader = filemap.mark(filename);

		CATCH {
			return;
		}

		int lineNo = 1;
		Lexer lexer(ex, filename);

		while (reader.isMore()) {
			String text = reader.getLine();

			CATCH {
				return;
			}

			int index = lexer.getLineTok(lineNo, text);

			CATCH {
				ErrorMsg->add(ex->getError().toString());
				ex->isError = false;
			}

			if(ISWARNING) {
				for (int i = 0, len = ex->Warning.size(); i < len; i++) {
					auto warning = ex->Warning[i];
					WarningMsg->add(warning.toString());
				}
				ex->isWarning = false;
			}

			lineNo++;
		}

		Matcher matcher(lexer, ex);
		Parser *parser = new Parser(
				matcher, ex, global_scope, filename, src, filemap, ErrorMsg,
				isSupportImport);

		ast::AstNode *node = parser->Parse(); // 语法分析

		CATCH {
			ErrorMsg->add(ex->getError().toString());
			ex->isError = false;
		}

		SourceSyntax syntax;
		syntax.program = node;
		syntax.filename = filename;

		src->add(syntax);

		reader.close();
	}
};
} // namespace stamon::c