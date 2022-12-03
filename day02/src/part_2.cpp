#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct Round
{
	enum shape
	{
		Rock = 'A',
		Paper = 'B',
		Scissors = 'C',
	};
	enum result
	{
		Loose = 0,
		Draw = 3,
		Win = 6,
	};
private:
	shape _opponent;
	result _expected_result;

	auto _get_our_shape() const
	{
		switch (_expected_result)
		{
			case Loose:
				if (_opponent == Rock)
					return Scissors;
				return ( shape ) (_opponent - 1);
			case Draw:
				return _opponent;
			case Win:
				if (_opponent == Scissors)
					return Rock;
				return (shape)(_opponent + 1);
		}
		return _opponent;
	}

public:
	Round(shape opponent, char expected_result) : _opponent(opponent)
	{
		switch (expected_result)
		{
			case 'X':
				_expected_result = Loose;
				break;
			case 'Y':
				_expected_result = Draw;
				break;
			case 'Z':
				_expected_result = Win;
				break;
			default:
				throw std::logic_error(std::string(expected_result, 1) + " is an invalid expected Result");
		}
	};

	size_t get_result() const
	{
		auto ours = _get_our_shape();

		size_t result = 0;
		switch (ours)
		{
			case Rock:
				result += 1;
				break;
			case Paper:
				result += 2;
				break;
			case Scissors:
				result += 3;
				break;
		}


		if (ours == _opponent)
			return Draw + result;

		if (ours == Rock && _opponent == Paper)
			return Loose + result;
		if (ours == Rock && _opponent == Scissors)
			return Win + result;
		if (ours == Paper && _opponent == Scissors)
			return Loose + result;
		if (ours == Paper && _opponent == Rock)
			return Win + result;
		if (ours == Scissors && _opponent == Rock)
			return Loose + result;
		if (ours == Scissors && _opponent == Paper)
			return Win + result;

		return Draw + result;
	}


};

auto get_rounds(const std::vector<std::string> &input)
{
	std::vector<Round> rounds;

	for (const auto &line: input)
	{
		auto opponents = ( Round::shape ) line[ 0 ];
		rounds.emplace_back(opponents, line[2]);
	}

	return rounds;
}

auto get_input(const std::string &file_path)
{
	std::ifstream in_stream(file_path);

	if (!in_stream.is_open())
		throw std::logic_error(file_path + " is an invalid Path");

	std::vector<std::string> input;

	for (std::string line; std::getline(in_stream, line);)
	{
		input.push_back(line);
	}

	return input;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "One Argument needed" << std::endl;
		return 1;
	}

	auto input = get_input(argv[ 1 ]);

	// Calculate Rounds
	size_t score = size_t();

	auto rounds = get_rounds(input);
	for (const auto &round: rounds)
	{
		auto result = round.get_result();
		std::cout << "Result: " << result << std::endl;
		score += result;
	}

	// Calculate Score
	std::cout << "Final Score: " << score << std::endl;

	return 0;
}