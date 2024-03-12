#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

class ExprAST {
public:
  virtual ~ExprAST() = default;
};

class NumberExprAST : public ExprAST {
public:
  explicit NumberExprAST(double val) : val_(val) {}
private:
  double val_;
};

class VariableExprAST : public ExprAST {
public:
  explicit VariableExprAST(std::string name) : name_(std::move(name)) {}
private:
  std::string name_;
};

class BinaryExprAST : public ExprAST {
public:
  BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs)
    : op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}
private:
  char op_;
  std::unique_ptr<ExprAST> lhs_;
  std::unique_ptr<ExprAST> rhs_;
};

class CallExprAST : public ExprAST {
public:
  CallExprAST(std::string callee, std::vector<std::unique_ptr<ExprAST>> args)
    : callee_(std::move(callee)), args_(std::move(args)) {}
private:
  std::string callee_;
  // function args
  std::vector<std::unique_ptr<ExprAST>> args_;
};

class PrototypeAST {
public:
  PrototypeAST(std::string name, std::vector<std::string> args) : name_(std::move(name)), args_(std::move(args)) {}
  auto GetName() const -> const std::string & { return name_; }
private:
  std::string name_;
  std::vector<std::string> args_;
};

class FunctionAST {
public:
  FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<ExprAST> body) 
    : proto_(std::move(proto)), body_(std::move(body)) {}
private:
  std::unique_ptr<PrototypeAST> proto_;
  std::unique_ptr<ExprAST> body_;
};