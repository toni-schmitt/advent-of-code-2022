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


static std::shared_ptr<Elve> get_elve_with_most_calories(std::vector<std::shared_ptr<Elve>> elves)
{
	auto compare_elve = [](const std::shared_ptr<Elve> &elve_1, const std::shared_ptr<Elve> &elve_2)
	{
		return elve_1->calories > elve_2->calories;
	};

	std::vector<std::shared_ptr<Elve>> elves_sorted(elves.begin(), elves.end());

	std::sort(elves_sorted.begin(), elves_sorted.end(), compare_elve);

	return elves_sorted[ 0 ];
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

	for (size_t i = 0, calories = 0, id = 1; i < input.size(); ++i)
	{
		if (input[ i ].empty())
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

	auto elve_with_most_calories = get_elve_with_most_calories(elves);

	std::cout << "ID:\t" << elve_with_most_calories->id << "\t" << "Calories:\t" << elve_with_most_calories->calories
			  << std::endl;
	return 0;
}