#pragma once

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "AST.h"
#include "lexer.h"
#include "log.h"

class Parser {
public:
  // numberexpr ::= number
  static auto ParseNumberExpr() -> std::unique_ptr<ExprAST> {
    auto result = std::make_unique<NumberExprAST>(Lexer::num_val);
    Lexer::GetNextToken();
    return std::move(result);
  }

  static auto ParseExpression() -> std::unique_ptr<ExprAST> {
    auto lhs = ParsePrimary();
    if (lhs == nullptr) {
      return nullptr;
    }
    return ParseBinOpRhs(0, std::move(lhs));
  }
  // 处理括号表达式 ()
  // TODO(gaoxiang): 
  static auto ParseParenExpr() -> std::unique_ptr<ExprAST> {
    Lexer::GetNextToken();  // 跳过第一个()
    auto v = ParseExpression();
    if (v == nullptr) {
      return nullptr;
    }
    if (Lexer::cur_tok != ')') {
      return LogError("expected ')'");
    }
    Lexer::GetNextToken();  // 跳过)
    return v;
  }

  // 处理函数调用
  static auto ParseIdentifierExpr() -> std::unique_ptr<ExprAST> {
    std::string id_name = Lexer::identifier_str;
    Lexer::GetNextToken();
    if (Lexer::cur_tok != '(') {
      return std::make_unique<VariableExprAST>(id_name);
    }
    // 处理函数调用的内容
    Lexer::GetNextToken(); // 跳过 (
    // 用来存储函数参数
    std::vector<std::unique_ptr<ExprAST>> args;
    if (Lexer::cur_tok != ')') {
      while(true) {
        if (auto arg = ParseExpression()) {
          args.push_back(std::move(arg));
        } else {
          return nullptr;
        }
        if (Lexer::cur_tok == ')') {
          break;
        }
        if (Lexer::cur_tok != ',') {
          return LogError("Expected ')' or ',' in argument list");
        }
        Lexer::GetNextToken();
      }
    }
    Lexer::GetNextToken();  // 处理 )
    return std::make_unique<CallExprAST>(id_name, std::move(args));
  }

  static auto ParsePrimary() -> std::unique_ptr<ExprAST> {
    switch (Lexer::cur_tok) {
      default:
        return LogError("unknown token when expecting an expression");
      case Token::tok_identifier:
        return ParseIdentifierExpr();
      case Token::tok_number:
        return ParseNumberExpr();
      case '(' :
        return ParseParenExpr();
    }
  }

  static auto GetTokPrecedence() -> int {
    if (isascii(Lexer::cur_tok) == 0) {
      return -1;
    }
    int tok_prec = binop_precedence[Lexer::cur_tok];
    if (tok_prec <= 0) {
      return -1;
    }
    return tok_prec;
  }

  static auto ParseBinOpRhs(int expr_prec, std::unique_ptr<ExprAST> lhs) -> std::unique_ptr<ExprAST> {
    while(true) {
      int tok_prec = GetTokPrecedence();
      if (tok_prec < expr_prec) {
        return lhs;
      }
      int bin_op = Lexer::cur_tok;
      Lexer::GetNextToken();  // eat binop
      auto rhs = ParsePrimary();
      if (rhs == nullptr) {
        return nullptr;
      }
      int next_prec = GetTokPrecedence();
      if (tok_prec < next_prec) {
        rhs = ParseBinOpRhs(tok_prec + 1, std::move(rhs));
        if (rhs == nullptr) {
          return nullptr;
        }
      }
      lhs = std::make_unique<BinaryExprAST>(bin_op, std::move(lhs), std::move(rhs));
    }
  }

  static auto ParsePrototype() -> std::unique_ptr<PrototypeAST> {
    if (Lexer::cur_tok != tok_identifier) {
      return LogErrorP("Expected function name in prototype");
    }
    std::string fn_name = Lexer::identifier_str;
    Lexer::GetNextToken();

    if (Lexer::cur_tok != '(') {
      return LogErrorP("Expected '(' in prototype");
    }

    std::vector<std::string> arg_names;
    while(Lexer::GetNextToken() == tok_identifier) {
      arg_names.push_back(Lexer::identifier_str);
    }
    if (Lexer::cur_tok != ')') {
      return LogErrorP("Expected ')' in prototype");
    }
    Lexer::GetNextToken();
    return std::make_unique<PrototypeAST>(fn_name, std::move(arg_names));
  }

  static auto ParseDefinition() -> std::unique_ptr<FunctionAST> {
    Lexer::GetNextToken();
    auto proto = ParsePrototype();
    if (proto == nullptr) {
      return nullptr;
    }
    if (auto e = ParseExpression()) {
      return std::make_unique<FunctionAST>(std::move(proto), std::move(e));
    }
    return nullptr;
  }

  static auto ParseExtern() -> std::unique_ptr<PrototypeAST> {
    Lexer::GetNextToken();
    return ParsePrototype();
  }

  static auto ParseTopLevelExpr() -> std::unique_ptr<FunctionAST> {
    if (auto e = ParseExpression()) {
      auto proto = std::make_unique<PrototypeAST>("", std::vector<std::string>());
      return std::make_unique<FunctionAST>(std::move(proto), std::move(e));
    }
    return nullptr;
  }

  static void HandleDefinition() {
    if (ParseDefinition()) {
      fprintf(stderr, "Parsed a function definition.\n");
    } else {
      Lexer::GetNextToken();
    }
  }

  static void HandleExtern() {
    if (ParseExtern()) {
      fprintf(stderr, "Parsed an extern\n");
    } else {
      Lexer::GetNextToken();
    }
  }

  static void HandleTopLevelExpr() {
    if (ParseTopLevelExpr()) {
      fprintf(stderr, "Parsed a top-level expr\n");
    } else {
      Lexer::GetNextToken();
    }
  }

  static void MainLoop() {
    while(true) {
      fprintf(stderr, "ready> ");
      switch (Lexer::cur_tok) {
        case Token::tok_eof:
          return;
        case ';':
          Lexer::GetNextToken();
          break;
        case Token::tok_def:
          HandleDefinition();
          break;
        case Token::tok_extern:
          HandleExtern();
          break;
        default:
          HandleTopLevelExpr();
          break;
      }
    }
  }
public:
  // 定义运算符优先级表
  // static std::map<char, int> binop_precedence = {{'<', 10}, {'+', 20}, {'-', 20}, {'*', 40}};
  static std::map<char, int> binop_precedence;
};