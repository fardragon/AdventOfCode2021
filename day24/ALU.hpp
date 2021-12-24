
#ifndef ADVENTOFCODE2021_ALU_HPP
#define ADVENTOFCODE2021_ALU_HPP

#include <functional>
#include "Instruction.hpp"



class ALU;
template <> struct std::hash<ALU>;



class ALU
{
public:
	void ApplyInstruction(const Instruction &instruction);
	void ApplyInputInstruction(const Instruction &instruction, std::int64_t input);

	std::strong_ordering operator<=>(const ALU &) const = default;
	[[nodiscard]] bool IsSolved() const;

private:
	std::int64_t &GetVariable(const Variable &var);
	std::int64_t GetValue(const std::variant<std::monostate, Variable, std::int64_t> &operand);

	friend struct std::hash<ALU>;

private:
	std::int64_t x = 0;
	std::int64_t y = 0;
	std::int64_t z = 0;
	std::int64_t w = 0;
};


// custom specialization of std::hash can be injected in namespace std
template<>
struct std::hash<ALU>
{
	std::size_t operator()(const ALU &alu) const noexcept
	{
		const auto h1 = std::hash<std::int64_t>{}(alu.x);
		const auto h2 = std::hash<std::int64_t>{}(alu.y);
		const auto h3 = std::hash<std::int64_t>{}(alu.z);
		const auto h4 = std::hash<std::int64_t>{}(alu.w);

		std::size_t res = 17;
		res = res * 31 + h1;
		res = res * 31 + h2;
		res = res * 31 + h3;
		res = res * 31 + h4;
		return res;
	}
};

#endif //ADVENTOFCODE2021_ALU_HPP
