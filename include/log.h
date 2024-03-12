#pragma once

#include "AST.h"
#include <cstdio>
#include <memory>

auto LogError(const char *str) -> std::unique_ptr<ExprAST> {
  fprintf(stderr, "LogError: %s\n", str);
  return nullptr;
}

auto LogErrorP(const char *str) -> std::unique_ptr<PrototypeAST> {
  LogError(str);
  return nullptr;
}