#pragma once

#include "AST.h"
#include <cstdio>
#include <memory>

std::unique_ptr<ExprAST> logError(const char *str) {
  fprintf(stderr, "LogError: %s\n", str);
  return nullptr;
}

std::unique_ptr<PrototypeAST> logErrorP(const char *str) {
  logError(str);
  return nullptr;
}