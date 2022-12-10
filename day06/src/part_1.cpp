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

	static auto index_of_first_marker(const std::string &data_stream)
	{
		std::set<char> marker;

		for (size_t i = 0; i < data_stream.length(); ++i)
		{
			size_t k = i;
			for (size_t j = 0; j < 4; ++j, ++k)
			{
				auto result = marker.insert(data_stream[ k ]);
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

	auto index_of_first_marker() const
	{
		return index_of_first_marker(_data_stream);
	}

};

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "One Argument needed" << std::endl;
		return 1;
	}

	const std::string file_path = argv[ 1 ];
	std::ifstream in_stream(file_path);

	if (!in_stream.is_open())
		throw std::logic_error(file_path + " is an invalid File Path");

	for (std::string line; std::getline(in_stream, line);)
		std::cout << subroutine::index_of_first_marker(line) << std::endl;

	return 0;
}