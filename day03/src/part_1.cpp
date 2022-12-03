

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <set>

struct Rucksack
{
private:
	std::string _compartment_a;
	std::string _compartment_b;
	size_t _inventory_length;
public:
	explicit Rucksack(const std::string &inventory)
	{
		_inventory_length = inventory.length();
		_compartment_a = inventory.substr(0, _inventory_length / 2);
		_compartment_b = inventory.substr(_inventory_length / 2);
	}

private:
	int _get_priority(char item) const
	{
		if (!std::isupper(item) && !std::islower(item))
			throw std::logic_error(std::string(item, 1) + " is an invalid Item");

		if (std::isupper(item))
			return item - 38;
		if (std::islower(item))
			return item - 96;
		return -1;
	}

	auto _get_duplicates() const
	{
		std::set<char> duplicates;
		for (const auto &item_a : _compartment_a)
		{
			if (_compartment_b.find(item_a) != std::string::npos)
				duplicates.insert(item_a);
		}
		return duplicates;
	}

public:
	auto get_duplicate_priorities() const
	{
		auto duplicates = _get_duplicates();

		size_t sum_of_priorities = 0;
		for (const auto &item: duplicates)
			sum_of_priorities += _get_priority(item);

		return sum_of_priorities;
	}

};

auto get_input(const std::string &file_path)
{
	std::ifstream in_stream(file_path);

	if (!in_stream.is_open())
		throw std::logic_error(file_path + " is an invalid File Path");

	std::vector<std::string> input;
	for (std::string line; std::getline(in_stream, line);)
		input.push_back(line);
	return input;
}

auto get_rucksacks(const std::vector<std::string> &input)
{
	std::vector<Rucksack> rucksacks;

	for (const auto &line : input)
		rucksacks.emplace_back(line);

	return rucksacks;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "One Argument needed" << std::endl;
		return 1;
	}

	auto input = get_input(argv[1]);

	auto rucksacks = get_rucksacks(input);

	size_t sum_of_priorities = 0;
	for (const auto &rucksack : rucksacks)
	{
		sum_of_priorities += rucksack.get_duplicate_priorities();
	}
	std::cout << sum_of_priorities << std::endl;
	return 0;
}
