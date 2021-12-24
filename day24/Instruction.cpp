//
// Created by fardragon on 12/24/21.
//

#include "Instruction.hpp"
#include "shared.hpp"
#include <stdexcept>

Instruction ParseInstruction(const std::string &str)
{
	const auto elems = SplitStringWhitespace(str);
	const auto &instruction = elems[0];

	if (instruction == "inp")
	{
		if (elems.size() != 2)
		{
			throw std::runtime_error("Failed to parse input");
		}

		auto operand = StrToOperand(elems[1]);
		return {Op::Inp, std::get<Variable>(operand), std::monostate()};
	}
	else
	{
		if (elems.size() != 3)
		{
			throw std::runtime_error("Failed to parse input");
		}
		Instruction instr;
		instr.firstOperand = std::get<Variable>(StrToOperand(elems[1]));
		instr.secondOperand = StrToOperand(elems[2]);

		if (instruction == "add")
		{
			instr.op = Op::Add;
		}
		else if (instruction == "mul")
		{
			instr.op = Op::Mul;
		}
		else if (instruction == "div")
		{
			instr.op = Op::Div;
		}
		else if (instruction == "mod")
		{
			instr.op = Op::Mod;
		}
		else if (instruction == "eql")
		{
			instr.op = Op::Eql;
		}
		else
		{
			throw std::runtime_error("Unknown instruction");
		}
		return instr;
	}
}

std::variant<std::monostate, Variable, std::int64_t> StrToOperand(const std::string &str)
{
	switch (str.front())
	{
		case 'x':
			return Variable::x;
		case 'y':
			return Variable::y;
		case 'z':
			return Variable::z;
		case 'w':
			return Variable::w;
		default:
			return StrToInteger<std::int64_t>(str);
	}
}