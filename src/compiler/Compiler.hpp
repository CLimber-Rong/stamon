/*
	Name: Compiler.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 22/02/24 12:31
	Description: 编译器头文件
*/

#pragma once

#include "BufferStream.cpp"
#include "Parser.cpp"
#include "HashMap.hpp"

namespace stamon::c {

ArrayList<SourceSyntax> *ParseTargetProject(log::Exception *e,
		ArrayList<String> *error_msg, ArrayList<String> *warning_msg,
		String filename, bool is_support_import, ArrayList<SourceSyntax> *src,
		HashMap<String, bool> filemap, SyntaxScope global_scope) {

	log::Exception *ex = e;

	filemap.put(filename, true);

	action::BufferInStream stream(ex, filename);

	CATCH {
		error_msg->add(ex->getError().toString());
		return NULL;
	}

	ArrayList<String> lines = stream.readLines();

	int lineNo = 1;
	Lexer lexer(ex, filename);

	for (int i = 0; i < lines.size(); i++) {
		String text = lines[i];

		int index = lexer.getLineTok(lineNo, text);

		CATCH {
			error_msg->add(ex->getError().toString());
			ex->isError = false;
		}

		if (ISWARNING) {
			for (int i = 0, len = ex->Warning.size(); i < len; i++) {
				auto warning = ex->Warning[i];
				warning_msg->add(warning.toString());
			}
			ex->isWarning = false;
		}

		lineNo++;
	}

	Matcher matcher(lexer, ex);
	Parser *parser = new Parser(matcher, ex, global_scope, filename, src,
			filemap, error_msg, warning_msg, is_support_import);

	ast::AstNode *node = parser->Parse(); // 语法分析

	CATCH {
		error_msg->add(ex->getError().toString());
		ex->isError = false;
	}

	SourceSyntax syntax;
	syntax.program = node;
	syntax.filename = filename;

	src->add(syntax);

	return src;
}

ast::AstNode *MergeAST(ArrayList<SourceSyntax> *syntax_list) {
	ArrayList<ast::AstNode *> *program = new ArrayList<ast::AstNode *>();

	for (int i = 0, len = syntax_list->size(); i < len; i++) {
		// 获取该文件的AST根节点
		ast::AstNode *node = syntax_list->at(i).program;

		// 放入总节点中
		(*program) += *(node->Children());

		// node只是暂存了ast，需要被删除
		// 为了防止删除时将子节点也删除，因此需要先清空字节点列表
		node->Children()->clear();
		delete node;
	}

	ast::AstNode *node = new ast::AstProgram(program);

	// 初始化node
	node->filename = String();
	node->lineNo = -1;

	return node;
}

} // namespace stamon::c