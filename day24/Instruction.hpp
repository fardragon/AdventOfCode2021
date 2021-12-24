#ifndef ADVENTOFCODE2021_INSTRUCTION_HPP
#define ADVENTOFCODE2021_INSTRUCTION_HPP


#include <variant>
#include <cstdint>
#include <string>

enum class Op
{
	Inp,
	Add,
	Mul,
	Div,
	Mod,
	Eql
};

enum class Variable
{
	x,
	y,
	z,
	w
};


struct Instruction
{
	Op op;
	Variable firstOperand;
	std::variant<std::monostate, Variable, std::int64_t> secondOperand;
};

Instruction ParseInstruction(const std::string &str);
std::variant<std::monostate, Variable, std::int64_t> StrToOperand(const std::string &str);

#endif //ADVENTOFCODE2021_INSTRUCTION_HPP
