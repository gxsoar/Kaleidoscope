#include <cctype>
#include <cstdio>
#include <cstdlib>
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

static std::string IdentifierStr;   // Filled in if tok_identifier
static double NumVal; // Filled in tok_number
static int curTok;

/// gettok Return the next toker from standard input
static int getTok() {
  static int last_char = ' ';
  // Skip any whitespace
  while(std::isspace(last_char)) {
    last_char = getchar();
  }
  if (std::isalpha(last_char)) {
    IdentifierStr += last_char;
    while(std::isalnum(last_char = getchar())) {
      IdentifierStr += last_char;
    }
    if (IdentifierStr == "def") {
      return Token::tok_def;
    }
    if (IdentifierStr == "extern") {
      return Token::tok_extern;
    }
    return Token::tok_identifier;
  }
  if (std::isdigit(last_char) || last_char == '.') {
    std::string num_str;
    do {
    num_str += last_char;
    last_char = getchar();
    }while (std::isdigit(last_char) || last_char == '.');

    NumVal = std::strtod(num_str.c_str(), 0);
    return Token::tok_number;
  }
  // 处理注释
  if (last_char == '#') {
    do {
      last_char = getchar();
    }while(last_char != EOF && last_char != '\n' && last_char != '\r');
    if (last_char != EOF) {
      return getTok();
    }
  }
  if (last_char == EOF) {
    return Token::tok_eof;
  }
  int this_char = last_char;
  last_char = getchar();
  return this_char;
}


static int getNextToken() {
  return curTok = getTok();
}