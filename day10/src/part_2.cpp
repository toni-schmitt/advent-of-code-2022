#include <iostream>
#include <vector>
#include <fstream>

struct CRT
{
private:

	struct Sprite
	{
		size_t start_position;
		size_t middle_position;
		size_t end_position;
	};

	static constexpr char _dark_pixel = '.';
	static constexpr char _lit_pixel = '#';

	static constexpr size_t _width = 40;
	static constexpr size_t _height = 6;
	char _pixels[_width][_height] = { };
	std::pair<size_t, size_t> _current_position = { 0, 0 };
	Sprite _sprite = { 0, 0, 0 };

	[[nodiscard]] inline auto _is_in_sprite() const
	{
		if (_sprite.start_position > _current_position.first || _sprite.end_position < _current_position.first)
			return false;
		return true;
	}

	inline auto _update_sprite(const size_t &x_register)
	{
		_sprite.start_position = x_register - 1;
		_sprite.middle_position = x_register;
		_sprite.end_position = x_register + 1;
	}

	[[nodiscard]] static inline auto _is_end_of_screen(const size_t &row)
	{
		return row == _width - 1;
	}

	inline auto _update_current_position(const size_t &cycle)
	{
		( void ) cycle;
		if (_is_end_of_screen(_current_position.first))
		{
			_current_position.second += 1;
			_current_position.first = 0;
			return;
		}
		_current_position.first += 1;
	}

	[[nodiscard]] inline auto _get_char_to_draw() const
	{
		if (!_is_in_sprite())
		{
			return _dark_pixel;
		}
		return _lit_pixel;
	}

public:

	auto draw_current_pixel_on_screen() const
	{
		std::cout << _pixels[ _current_position.first ][ _current_position.second ] << std::flush;
		if (_is_end_of_screen(_current_position.first))
			std::cout << std::endl;
	}

	auto draw_pixel(const size_t &cycle, const size_t &x_register)
	{
		// x_register == horizontal position of middle of sprite
		// cycle == position of crt's current position
		_update_sprite(x_register);

		auto char_to_draw = _get_char_to_draw();

		_pixels[ _current_position.first ][ _current_position.second ] = char_to_draw;

		draw_current_pixel_on_screen();
		_update_current_position(cycle);
	}

	auto draw_screen() const
	{
		for (size_t row = 0; row < _height; ++row)
		{
			for (size_t column = 0; column < _width; ++column) // NOLINT(modernize-loop-convert)
			{
				std::cout << _pixels[ column ][ row ];
			}
			std::cout << std::endl;
		}
	}
};

struct CPU
{
private:
	size_t _x_register { 1 };
	size_t _cycle { 1 };
	std::vector<std::pair<size_t, size_t>> _crucial_cycle_x { };
	CRT &_crt;

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

	inline auto _execute()
	{
		_crt.draw_pixel(_cycle, _x_register);
		_cycle += 1;
	}

	inline auto _execute(const size_t &increment)
	{
		_execute();
		_execute();
		_x_register += increment;
	}

public:
	explicit CPU(CRT &crt) : _crt(crt) { }

	auto read_instruction(const std::string &instruction)
	{
		if (instruction == "noop")
		{
			_execute();
		}
		else if (instruction.find("addx") != std::string::npos)
		{
			auto increment = _get_increment_from_instruction(instruction);
			_execute(increment);
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

	auto crt = CRT();
	auto cpu = CPU(crt);
	for (std::string instruction; std::getline(in_stream, instruction);)
	{
		cpu.read_instruction(instruction);
	}

//	size_t sum_of_signal_strengths = 0;
//	auto crucial_cycles = cpu.get_crucial_cycles();
//	for (const auto &crucial_cycle: crucial_cycles)
//	{
//		auto cycle_index = crucial_cycle.first;
//		auto X = crucial_cycle.second;
//		auto signal_strength = cycle_index * X;
//		std::cout << NAMEOF(cycle_index) << "(" << cycle_index << ") * " << NAMEOF(X) << "(" << X
//				  << ") =\t" << NAMEOF(signal_strength) << "(" << signal_strength << ")"
//				  << std::endl;
//		sum_of_signal_strengths += signal_strength;
//	}
//
//	std::cout << NAMEOF(sum_of_signal_strengths) << " = " << sum_of_signal_strengths << std::endl;

//	crt.draw_screen();

	return 0;
}