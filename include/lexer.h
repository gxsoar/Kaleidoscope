#pragma once

#include <string>

enum Token {
  tok_eof = -1,

  // commands
  tok_def = -2,
  tok_extern = -3,

  // primary
  tok_identifier = -4,
  tok_number = -5,
};

class Lexer {
public:
  /// gettok Return the next toker from standard input
  static auto GetTok() -> int {
    static int last_char = ' ';
    // Skip any whitespace
    while(std::isspace(last_char) != 0) {
      last_char = getchar();
    }
    if (std::isalpha(last_char) != 0) {
      identifier_str += std::to_string(last_char);
      while(std::isalnum(last_char = getchar()) != 0) {
        identifier_str += std::to_string(last_char);
      }
      if (identifier_str == "def") {
        return Token::tok_def;
      }
      if (identifier_str == "extern") {
        return Token::tok_extern;
      }
      return Token::tok_identifier;
    }
    if ((std::isdigit(last_char) != 0) || last_char == '.') {
      std::string num_str;
      do {
      num_str += std::to_string(last_char);
      last_char = getchar();
      }while ((std::isdigit(last_char) != 0) || last_char == '.');

      num_val = std::strtod(num_str.c_str(), nullptr);
      return Token::tok_number;
    }
    // 处理注释
    if (last_char == '#') {
      do {
        last_char = getchar();
      }while(last_char != EOF && last_char != '\n' && last_char != '\r');
      if (last_char != EOF) {
        return GetTok();
      }
    }
    if (last_char == EOF) {
      return Token::tok_eof;
    }
    int this_char = last_char;
    last_char = getchar();
    return this_char;
  }

  static auto GetNextToken() -> int {
    return cur_tok = Lexer::GetTok();
  }

private:
  static std::string identifier_str;   // Filled in if tok_identifier
  static double num_val; // Filled in tok_number
  static int cur_tok;
};