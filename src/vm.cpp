#include "vm.h"

#include "ast.h"

VM::VM()
{
  heap["store"] = 0;
}

void push(VM::Stack &stack, const JSON &value)
{
  stack.push(value);
}

// void expressionPush(VM::Stack &stack, std::vector<JSON> expressionTokens)
// {
//   for (JSON i : expressionTokens)
//   {
//     push(stack, i);
//   }
// }

JSON pop(VM::Stack &stack)
{
  JSON top = stack.top();
  stack.pop();
  return top;
}

JSON add(const JSON &a, const JSON &b)
{
  return a.get<double>() + b.get<double>();
}

JSON sub(const JSON &a, const JSON &b)
{
  return a.get<double>() - b.get<double>();
}

JSON times(const JSON &a, const JSON &b)
{
  return a.get<double>() * b.get<double>();
}

JSON divide(const JSON &a, const JSON &b)
{
  return a.get<double>() / b.get<double>();
}

JSON VM::run(AST::Ptr prog)
{
  Stack stack;
  exec(prog, stack);
  auto garbage = pop(stack);
  auto ans = pop(stack);
  if (!stack.empty())
  {
    throw std::range_error("stack not empty");
  }
  return ans;
}

void VM::exec(AST::Ptr prog, VM::Stack &stack)
{
  switch (prog->getType())
  {
  case ASTType::number:
    push(stack, prog->token->getValue());
    break;
  case ASTType::recall:
    push(stack, heap["store"]);
    break;
  case ASTType::store:
  {
    exec(prog->args.at(0), stack);
    heap["store"] = stack.top();
  }
  break;
  case ASTType::add:
  {
    exec(prog->args.at(0), stack);
    exec(prog->args.at(1), stack);
    auto a = pop(stack);
    auto b = pop(stack);
    push(stack, add(a, b));
  }
  break;

  case ASTType::all:
  {
    JSON ans = {};
    for (int i = 0; i < prog->args.size(); i++)
    {
      exec(prog->args.at(i), stack);
      JSON token = pop(stack);
      ans.push_back(token);
      
    }
  
    push(stack, ans);
  }

  case ASTType::sub:
  {
    exec(prog->args.at(0), stack);
    exec(prog->args.at(1), stack);
    auto a = pop(stack);
    auto b = pop(stack);
    push(stack, sub(a, b));
  }
  break;
  case ASTType::times:
  {
    exec(prog->args.at(0), stack);
    exec(prog->args.at(1), stack);
    auto a = pop(stack);
    auto b = pop(stack);
    push(stack, times(a, b));
  }
  break;
  case ASTType::divide:
  {
    exec(prog->args.at(0), stack);
    exec(prog->args.at(1), stack);
    auto a = pop(stack);
    auto b = pop(stack);
    push(stack, divide(a, b));
  }
  break;
  default:
    throw std::range_error(prog->toJSON());
  }
}
