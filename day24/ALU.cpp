//
// Created by fardragon on 12/24/21.
//

#include "ALU.hpp"
#include <stdexcept>

void ALU::ApplyInstruction(const Instruction &instruction)
{
	if (instruction.op == Op::Inp)
	{
		throw std::runtime_error("Invalid operation");
	}

	auto &targetVariable = GetVariable(instruction.firstOperand);
	auto value = GetValue(instruction.secondOperand);

	switch (instruction.op)
	{
		case Op::Add:
			targetVariable += value;
			break;
		case Op::Mul:
			targetVariable *= value;
			break;
		case Op::Div:
			targetVariable /= value;
			break;
		case Op::Mod:
			targetVariable %= value;
			break;
		case Op::Eql:
			targetVariable = (targetVariable == value) ? 1 : 0;
			break;
		default:
			throw std::runtime_error("Unknown operation");
	}

}

std::int64_t &ALU::GetVariable(const Variable &var)
{
	switch (var)
	{
		case Variable::x:
			return x;
		case Variable::y:
			return y;
		case Variable::z:
			return z;
		case Variable::w:
			return w;
		default:
			throw std::runtime_error("Unknown variable");
	}
}

std::int64_t ALU::GetValue(const std::variant<std::monostate, Variable, std::int64_t> &operand)
{
	if (std::holds_alternative<Variable>(operand))
	{
		return GetVariable(std::get<Variable>(operand));
	}
	else if (std::holds_alternative<std::int64_t>(operand))
	{
		return std::get<std::int64_t>(operand);
	}
	else
	{
		throw std::runtime_error("Unknown operand");
	}
}

void ALU::ApplyInputInstruction(const Instruction &instruction, std::int64_t input)
{
	if (instruction.op != Op::Inp)
	{
		throw std::runtime_error("Invalid operation");
	}

	auto &targetVariable = GetVariable(instruction.firstOperand);
	targetVariable = input;
}

bool ALU::IsSolved() const
{
	return z == 0;
}
