#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct Elve
{
	int section_start;
	int section_end;

	friend std::ostream &operator<<(std::ostream &os, const Elve &elve)
	{
		return os << elve.section_start << '-' << elve.section_end;
	}
};


struct ElvePair
{
private:
	std::pair<Elve, Elve> _elve_pair;

public:
	ElvePair(const Elve &elve_1, const Elve &elve_2)
	{
		_elve_pair = std::make_pair(elve_1, elve_2);
	}

private:
	static auto _elve_is_doing_double_work(const Elve &elve_1, const Elve &elve_2)
	{
		return elve_1.section_start >= elve_2.section_start &&
			   elve_1.section_end <= elve_2.section_end;
	}

public:
	auto is_doing_double_work() const
	{
		return _elve_is_doing_double_work(_elve_pair.first, _elve_pair.second) ||
			   _elve_is_doing_double_work(_elve_pair.second, _elve_pair.first);
	}

	friend std::ostream &operator<<(std::ostream &os, const ElvePair &elve_pair)
	{
		return os << elve_pair._elve_pair.first << ',' << elve_pair._elve_pair.second;
	}

};

Elve get_elve_from_line(const std::string &line)
{
	std::string section_start = line.substr(0, line.find('-'));
	std::string section_end = line.substr(line.find('-') + 1);
	return { std::stoi(section_start), std::stoi(section_end) };
}

ElvePair get_elve_pair_from_line(const std::string &line)
{
	std::string line_first_elve = line.substr(0, line.find(','));
	std::string line_second_elve = line.substr(line.find(',') + 1);
	return {
			get_elve_from_line(line_first_elve),
			get_elve_from_line(line_second_elve),
	};
}

auto get_elve_pairs_from_file(const std::string &file_path)
{
	std::ifstream in_stream(file_path);

	if (!in_stream.is_open())
		throw std::logic_error(file_path + " is an invalid File path");

	std::vector<ElvePair> elve_pairs;

	for (std::string line; std::getline(in_stream, line);)
	{
		elve_pairs.push_back(get_elve_pair_from_line(line));
	}

	return elve_pairs;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "One Argument needed" << std::endl;
		return 1;
	}

	auto elve_pairs = get_elve_pairs_from_file(argv[ 1 ]);

	auto sum_of_double_work = 0;
	for (const auto &elve_pair: elve_pairs)
	{
		if (elve_pair.is_doing_double_work())
			sum_of_double_work += 1;
	}

	std::cout << sum_of_double_work << std::endl;

	return 0;
}