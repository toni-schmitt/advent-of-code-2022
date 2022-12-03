

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
	Rucksack() { };

	explicit Rucksack(const std::string &inventory)
	{
		_inventory_length = inventory.length();
		_compartment_a = inventory.substr(0, _inventory_length / 2);
		_compartment_b = inventory.substr(_inventory_length / 2);
	}

public:
	auto get_duplicates() const
	{
		std::set<char> duplicates;
		for (const auto &item_a: _compartment_a)
		{
			if (_compartment_b.find(item_a) != std::string::npos)
				duplicates.insert(item_a);
		}
		return duplicates;
	}

	auto get_duplicates_as_string() const
	{
		auto duplicates = get_duplicates();

		std::string duplicates_as_string;
		for (const auto &duplicate: duplicates)
		{
			duplicates_as_string.push_back(duplicate);
		}
		return duplicates_as_string;
	}

};

struct ElveGroup
{
private:
	std::string _rucksack_inventories[3];

public:
	explicit ElveGroup(const std::string &inventory_1, const std::string &inventory_2, const std::string &inventory_3)
	{
		_rucksack_inventories[ 0 ] = inventory_1;
		_rucksack_inventories[ 1 ] = inventory_2;
		_rucksack_inventories[ 2 ] = inventory_3;
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

	auto _get_group_item() const
	{
		for (const auto &item : _rucksack_inventories[0])
		{
			if (_rucksack_inventories[1].find(item) != std::string::npos && _rucksack_inventories[2].find(item) != std::string::npos)
				return item;
		}
		throw std::logic_error("No Group Item Found");
	}

public:
	auto get_group_priority() const
	{
		return _get_priority(_get_group_item());
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

auto get_elve_groups(const std::vector<std::string> &input)
{
	std::vector<ElveGroup> elve_groups;

	for (size_t i = 0; i + 2 < input.size(); i += 3)
	{
		elve_groups.emplace_back(input[ i ], input[ i + 1 ], input[ i + 2 ]);
	}

	return elve_groups;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "One Argument needed" << std::endl;
		return 1;
	}

	auto input = get_input(argv[ 1 ]);

	auto elve_groups = get_elve_groups(input);

	size_t sum_of_group_priorities = 0;
	for (const auto &elve_group: elve_groups)
	{
		sum_of_group_priorities += elve_group.get_group_priority();
	}
	std::cout << sum_of_group_priorities << std::endl;
	return 0;
}
