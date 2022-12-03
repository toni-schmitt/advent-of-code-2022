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
	shape _ours;
	shape _opponent;

public:
	Round(shape ours, shape opponent) : _ours(ours), _opponent(opponent) { };

	size_t get_result() const
	{
		size_t result = 0;
		switch (_ours)
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


		if (_ours == _opponent)
			return Draw + result;

		if (_ours == Rock && _opponent == Paper)
			return Loose + result;
		if (_ours == Rock && _opponent == Scissors)
			return Win + result;
		if (_ours == Paper && _opponent == Scissors)
			return Loose + result;
		if (_ours == Paper && _opponent == Rock)
			return Win + result;
		if (_ours == Scissors && _opponent == Rock)
			return Loose + result;
		if (_ours == Scissors && _opponent == Paper)
			return Win + result;

		return Draw + result;
	}

	friend std::ostream &operator<<(std::ostream &os, const Round &round)
	{
		return os << ( char ) round._ours << ' ' << ( char ) (round._opponent + 23);
	}


};

auto get_rounds(const std::vector<std::string> &input)
{
	std::vector<Round> rounds;

	for (const auto &line: input)
	{
		auto ours = ( Round::shape ) line[ 2 ];
		auto opponents = ( Round::shape ) line[ 0 ];
		rounds.emplace_back(ours, opponents);
	}

	return rounds;
}

auto get_input(const std::string &file_path)
{
	auto get_valid_input = [](const std::string &invalid_line)
	{
		std::string valid_line = invalid_line;
		std::replace(valid_line.begin(), valid_line.end(), 'X', 'A');
		std::replace(valid_line.begin(), valid_line.end(), 'Y', 'B');
		std::replace(valid_line.begin(), valid_line.end(), 'Z', 'C');
		return valid_line;
	};

	std::ifstream in_stream(file_path);

	if (!in_stream.is_open())
		throw std::logic_error(file_path + " is an invalid Path");

	std::vector<std::string> input;

	for (std::string line; std::getline(in_stream, line);)
	{
		input.push_back(get_valid_input(line));
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
		std::cout << round << std::endl;
		std::cout << "Result: " << result << std::endl;
		score += result;
	}

	// Calculate Score
	std::cout << "Final Score: " << score << std::endl;

	return 0;
}