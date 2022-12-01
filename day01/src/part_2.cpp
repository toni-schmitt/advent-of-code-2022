#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <memory>
#include <vector>

struct Elve
{
	size_t id;
	size_t calories;

	Elve(size_t id, size_t calories) : id(id), calories(calories) { };
};

static std::vector<std::shared_ptr<Elve>> sort_elves(std::vector<std::shared_ptr<Elve>> elves)
{
	auto compare_elve = [](const std::shared_ptr<Elve> &elve_1, const std::shared_ptr<Elve> &elve_2)
	{
		return elve_1->calories > elve_2->calories;
	};

	std::sort(elves.begin(), elves.end(), compare_elve);
	return elves;
}

std::vector<std::string> get_input(const std::string &file_name)
{
	std::ifstream in_stream(file_name);

	if (!in_stream.is_open())
		throw std::logic_error(file_name + " is an invalid file Path");

	in_stream.unsetf(std::ios_base::skipws);

	std::vector<std::string> input;

	for (std::string line; std::getline(in_stream, line);)
		input.push_back(line);

	return input;
}

static std::vector<std::shared_ptr<Elve>> get_elves(const std::vector<std::string> &input)
{
	std::vector<std::shared_ptr<Elve>> elves;

	for (size_t i = 0, calories = 0, id = 1; i <= input.size(); ++i)
	{
		if (i == input.size() || input[ i ].empty())
		{
			elves.push_back(std::make_shared<Elve>(id, calories));
			calories = 0;
			id += 1;
			continue;
		}

		calories += std::stoi(input[ i ]);
	}

	return elves;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "One Argument needed" << std::endl;
		return 1;
	}

	auto input = get_input(argv[ 1 ]);

	auto elves = get_elves(input);

	elves = sort_elves(elves);

	size_t calories_of_top_three_elves = 0;

	for (size_t i = 0; i < elves.size(); ++i)
	{
		const auto &elve = elves[i];
		std::cout << "ID:\t" << elve->id << "\t" << "Calories:\t" << elve->calories << std::endl;
		if (i < 3)
			calories_of_top_three_elves += elve->calories;
	}

	std::cout << calories_of_top_three_elves << std::endl;

	return 0;
}