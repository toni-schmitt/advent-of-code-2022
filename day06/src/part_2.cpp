#include <iostream>
#include <fstream>
#include <utility>
#include <set>

struct subroutine
{
private:
	std::string _data_stream;

public:
	explicit subroutine(std::string data_stream) : _data_stream(std::move(data_stream)) { };

	auto index_of_first_marker() const
	{
		std::set<char> marker;

		for (size_t i = 0; i < _data_stream.length(); ++i)
		{
			size_t k = i;
			for (size_t j = 0; j < 14; ++j, ++k)
			{
				if (k >= _data_stream.length())
					throw std::logic_error("Marker out of bounds");
				auto result = marker.insert(_data_stream[ k ]);
				if (!result.second)
				{
					marker.clear();
					break;
				}
			}
			if (!marker.empty())
				return k;
		}
		throw std::logic_error("Marker not found");
	}

};

auto get_datastream_from_input(const std::string &file_path)
{
	std::ifstream in_stream(file_path);

	if (!in_stream.is_open())
		throw std::logic_error(file_path + " is an invalid File Path");

	std::string data_stream;
	for (std::string line; std::getline(in_stream, line);)
		data_stream.append(line);

	return data_stream;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "One Argument needed" << std::endl;
		return 1;
	}

	auto data_stream = get_datastream_from_input(argv[ 1 ]);

	subroutine subroutine(data_stream);

	std::cout << subroutine.index_of_first_marker() << std::endl;

	return 0;
}