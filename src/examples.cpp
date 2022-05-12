#include "examples.h"

Example::Example(const std::string &_input,
		 const std::vector<Token::Ptr> &_tokens,
		 const AST::Ptr &_ast, const JSON &_ans)
  : input(_input), tokens(_tokens), ast(_ast), ans(_ans) {}

Token::Ptr append(std::vector<Token::Ptr> &tokens, Token::Ptr token) {
  tokens.push_back(token);
  return token;
}

// SCANNER_INPUT1="3.14"
// PARSER_RESULT1=
//       3.14
Example::Ptr ex0() {
  int line=0, col=0;
  std::string input = "3.14?5";
  
  std::vector<Token::Ptr> tokens;
  Token::Ptr tk0Pi = append(tokens,Token::number(3.14,line,col)); col += 4;
  Token::Ptr tk8Eoe = append(tokens, Token::eoe(line, col++));
  Token::Ptr tk1Five = append(tokens,Token::number(5,line,col)); col += 1;
  Token::Ptr tk1Eof = append(tokens,Token::eof(line,col));        col += 1;

  AST::Ptr ast = AST::all({AST::number(tk0Pi), AST::number(tk1Five) }) ;
  JSON ans = {3.14, 5.0};
  std::cout << "this is what it should be" << ans << std::endl;

  return Example::Ptr(new Example(input,tokens,ast,ans));
}

// PARSER_RESULT1=
//       *
//      / \
//     S   R
//     |
//     +
//    / \
//   4   5

Example::Ptr ex1() {
  int line=0, col=0;
  std::string input = "(4+5)S*R?3.14";
  
  std::vector<Token::Ptr> tokens;
  Token::Ptr tk0Lparen = append(tokens,Token::lparen(line,col++));
  Token::Ptr tk1Four = append(tokens,Token::number(4,line,col++));
  Token::Ptr tk2Add = append(tokens,Token::add(line,col++));
  Token::Ptr tk3Five = append(tokens,Token::number(5,line,col++));
  Token::Ptr tk4Rparen = append(tokens,Token::rparen(line,col++));
  Token::Ptr tk5Store = append(tokens,Token::keyword("S",line,col++));
  Token::Ptr tk6Times = append(tokens,Token::times(line,col++));
  Token::Ptr tk7Recall = append(tokens,Token::keyword("R",line,col++));
  Token::Ptr tk8Eoe = append(tokens, Token::eoe(line, col++));
    Token::Ptr tk0Pi = append(tokens,Token::number(3.14,line,col)); col += 4;
  Token::Ptr tk9Eof = append(tokens,Token::eof(line,col++));

  AST::Ptr ast = AST::all({
    AST::times(tk6Times, 
	       AST::store(tk5Store,
			  AST::add(tk2Add,
				   AST::number(tk1Four),
				   AST::number(tk3Five))),
	       AST::recall(tk7Recall)), AST::number(tk0Pi) });

  JSON ans = {81, 3.14};  

  return Example::Ptr(new Example(input,tokens,ast,ans));
}

Example::Ptr ex2() {
  int line=0, col=0;
  std::string input = "3S+R";
  
  std::vector<Token::Ptr> tokens;
  Token::Ptr tk0Three = append(tokens,Token::number(3,line,col++));
  Token::Ptr tk1Store = append(tokens,Token::keyword("S",line,col++));
  Token::Ptr tk2Add = append(tokens,Token::add(line,col++));
  Token::Ptr tk3Recall = append(tokens,Token::keyword("R",line,col++));

  Token::Ptr tk8Eoe = append(tokens, Token::eoe(line, col++));
  Token::Ptr tk0Lparen = append(tokens,Token::lparen(line,col++));
  Token::Ptr tk1Four = append(tokens,Token::number(4,line,col++));
  Token::Ptr tk2Add2 = append(tokens,Token::add(line,col++));
  Token::Ptr tk3Five = append(tokens,Token::number(5,line,col++));
  Token::Ptr tk4Rparen = append(tokens,Token::rparen(line,col++));
  Token::Ptr tk5Store = append(tokens,Token::keyword("S",line,col++));
  Token::Ptr tk6Times = append(tokens,Token::times(line,col++));
  Token::Ptr tk7Recall = append(tokens,Token::keyword("R",line,col++));
  Token::Ptr tk4Eof = append(tokens,Token::eof(line,col++));

  AST::Ptr ast = AST::all({AST::add(tk2Add,
	     AST::store(tk1Store,
			  AST::number(tk0Three)),
	     AST::recall(tk3Recall)),
       AST::times(tk6Times, 
	       AST::store(tk5Store,
			  AST::add(tk2Add2,
				   AST::number(tk1Four),
				   AST::number(tk3Five))),
	       AST::recall(tk7Recall))
       });
    

  JSON ans = {6, 81};

  return Example::Ptr(new Example(input,tokens,ast,ans));
}

std::size_t Example::size() { return 3; }
Example::Ptr Example::example(int k) {
  switch(k) {
  case 0: return ex0();
  case 1: return ex1();
  case 2: return ex2();    
  default: return NULL;
  }
}
