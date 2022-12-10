#include <iostream>
#include <fstream>
#include <utility>
#include <vector>

struct Dir;

struct File
{
	std::string name;
	size_t size;
	const Dir &parent_directory;
};

struct Dir
{
private:
	const std::string _name;
	size_t _size;
	const Dir &_parent;
	std::vector<Dir> _sub_directories;
	std::vector<File> _files;

public:
	explicit Dir(std::string directory_name, const Dir &parent_directory) : _name(std::move(directory_name)),
																			_parent(parent_directory)
	{
		( void ) _parent;
		_size = calculate_size();
	}

	auto add_sub_directory(const Dir &sub_directory)
	{
		_sub_directories.push_back(sub_directory);
		_size = calculate_size();
	}

	auto add_file(const File &file)
	{
		_files.push_back(file);
		_size = calculate_size();
	}

private:
	static size_t _calculate_size_of_files(const Dir &directory)
	{
		size_t size = 0;
		for (const auto &file: directory._files)
			size += file.size;
		return size;
	}

	size_t _caluclate_size(const Dir &directory) const
	{
		size_t size = 0;
		if (directory._sub_directories.empty())
			return _calculate_size_of_files(directory);
		for (const auto &sub_directory: directory._sub_directories)
		{
			size += _caluclate_size(sub_directory);
		}
		return size;
	}

public:
	size_t calculate_size() const
	{
		size_t size = 0;
		for (const auto &directory: _sub_directories)
		{
			size += _caluclate_size(directory);
		}
		return size;
	}

	auto size() const { return _size; }

};

struct Filesystem
{
private:
	std::vector<Dir> _directories;
	std::stack<Dir *> _last_cds;

	static inline auto remove_whitespace(std::string &string)
	{
		return string.erase(std::remove_if(string.begin(), string.end(), ::isspace), string.end());
	}

public:
	explicit Filesystem(const std::vector<std::string> &input)
	{
		for (const auto &line: input)
		{
			if (is_command(line))
				read_command(line);
			else
				read_directory_structure(line);
		}
	}

	static inline bool is_command(const std::string &line) { return line.find('$') != std::string::npos; }

	void read_command(const std::string &command)
	{
		if (command.find('$') == std::string::npos)
			throw std::invalid_argument(R"('$' not found in ')" + command + "\'");


		if (command.find("cd") != std::string::npos)
		{
			if (command.find("..") != std::string::npos)
			{
				_last_cds.pop();
				return;
			}
			auto directory_name = command.substr(command.rfind(' '));
			remove_whitespace(directory_name);
			_last_cds.push(&_directories.emplace_back(directory_name, _directories.back()));
		}
		else if (command.find("ls") != std::string::npos)
		{
		}
		else throw std::invalid_argument("\'" + command + "\' is an invalid command");
	}

	void read_directory_structure(const std::string &entry)
	{
		if (entry.find("dir") != std::string::npos)
		{
			auto name = entry.substr(entry.rfind(' '));
			remove_whitespace(name);
			Dir sub_directory(name, *_last_cds.top());
			_last_cds.top()->add_sub_directory(sub_directory);
		}
		else
		{
			// must be file

			auto index_at_space = entry.find(' ');
			if (index_at_space == std::string::npos)
				throw std::logic_error(entry + " must be file, but no space found");

			auto size = std::stoul(entry.substr(0, index_at_space));
			auto name = entry.substr(index_at_space);
			remove_whitespace(name);
			_last_cds.top()->add_file({ name, size, *_last_cds.top() });
		}
	}

	void print_directory_structure() const
	{

	}
};

auto get_input(const std::string &file_path)
{
	std::ifstream in_stream(file_path);

	if (!in_stream.is_open())
		throw std::logic_error(file_path + " is an invalid Path");

	std::vector<std::string> input;

	for (std::string line; std::getline(in_stream, line);)
		input.push_back(line);

	return input;
}
//
//auto get_filesystem(const std::vector<std::string> &input)
//{
//	Dir root_directory("/", root_directory);
//
//	for (const auto &line: input)
//	{
//		if (line.find('$') != std::string::npos)
//		{
//			// Command
//			if (line.find("cd") != std::string::npos)
//			{
//				auto directory_name = line.substr(line.find(' '));
//			}
//			else if (line.find("ls") != std::string::npos)
//			{
//			}
//			else
//			{
//				throw std::logic_error(line + " is an invalid command");
//			}
//		}
//		else
//		{
//			if (line.find("dir") != std::string::npos)
//			{
//				// directory
//			}
//			else
//			{
//				auto index_at_space = line.find(' ');
//				if (index_at_space == std::string::npos)
//					throw std::logic_error(line + " must be file, but no space found");
//
//				auto size = line.substr(0, index_at_space);
//				auto name = line.substr(index_at_space);
//				Dir tmp;
//				File file = {
//						name,
//						std::stoul(size),
//						tmp // TODO: pls change me!!!!!
//				};
//			}
//		}
//	}
//
//	return filesystem;
//}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "One Argument needed" << std::endl;
		return 1;
	}


	auto input = get_input(argv[ 1 ]);

	Filesystem filesystem(input);

	return 0;
}