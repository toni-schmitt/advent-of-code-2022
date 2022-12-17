#include <iostream>
#include <vector>
#include <fstream>

struct CPU
{
private:
	size_t _x_register { 1 };
	size_t _cycle { 1 };
	std::vector<std::pair<size_t, size_t>> _crucial_cycle_x { };

	static auto _get_increment_from_instruction(const std::string &instruction)
	{
		auto index_at_space = instruction.find(' ');
		if (index_at_space == std::string::npos)
			throw std::logic_error(instruction + " is an invalid Instruction");

		auto increment_str = instruction.substr(index_at_space);

		return std::stoi(increment_str);
	}

	[[nodiscard]] inline auto _is_crucial_cycle() const
	{
		return _cycle == 20 || _cycle == 60 || _cycle == 100 || _cycle == 140 || _cycle == 180 || _cycle == 220;
	}

	inline auto _remember_x()
	{
		_crucial_cycle_x.emplace_back(_cycle, _x_register);
	}

public:
	auto read_instruction(const std::string &instruction)
	{
		if (instruction == "noop")
		{
			if (_is_crucial_cycle())
				_remember_x();
			_cycle += 1;
		}
		else if (instruction.find("addx") != std::string::npos)
		{
			if (_is_crucial_cycle())
				_remember_x();
			_cycle += 1;
			if (_is_crucial_cycle())
				_remember_x();
			_cycle += 1;
			_x_register += _get_increment_from_instruction(instruction);
		}
		else
		{
			throw std::invalid_argument(instruction + " is an invalid Instruction");
		}
	}

	[[nodiscard]] inline const auto &get_crucial_cycles() const { return _crucial_cycle_x; }
};

#define NAMEOF(variable) ((decltype(&variable))nullptr, #variable)

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[ 0 ] << " [input_file]" << std::endl;
		return 1;
	}


	std::ifstream in_stream(argv[ 1 ]);
	if (!in_stream.is_open())
	{
		std::cerr << argv[ 1 ] << " is an invalid Path" << std::endl;
		return 1;
	}

	auto cpu = CPU();
	for (std::string instruction; std::getline(in_stream, instruction);)
	{
		cpu.read_instruction(instruction);
	}

	size_t sum_of_signal_strengths = 0;
	auto crucial_cycles = cpu.get_crucial_cycles();
	for (const auto &crucial_cycle: crucial_cycles)
	{
		auto cycle_index = crucial_cycle.first;
		auto X = crucial_cycle.second;
		auto signal_strength = cycle_index * X;
		std::cout << NAMEOF(cycle_index) << "(" << cycle_index << ") * " << NAMEOF(X) << "(" << X
				  << ") =\t" << NAMEOF(signal_strength) << "(" << signal_strength << ")"
				  << std::endl;
		sum_of_signal_strengths += signal_strength;
	}

	std::cout << NAMEOF(sum_of_signal_strengths) << " = " << sum_of_signal_strengths << std::endl;

	return 0;
}