#include <cstdio>
#include "lexer.h"
#include "parser.h"

auto main() -> int {

  Parser::binop_precedence = {{'<', 10}, {'+', 20}, {'-', 20}, {'*', 40}};

  fprintf(stderr, "ready> ");
  Lexer::GetNextToken();

  Parser::MainLoop();

  return 0;
}