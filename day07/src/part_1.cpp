#include <iostream>
#include <fstream>
#include <utility>
#include <vector>

struct Directory;

struct File
{
	std::string name;
	size_t size;
	[[maybe_unused]] const Directory &parent_directory;
};

struct Directory
{
private:
	std::string _name;
	size_t _size;
	std::vector<Directory> _sub_directories;
	std::vector<File> _files;

public:

	Directory()
	{
		_name = "undefined";
		_size = 0;
		_sub_directories = std::vector<Directory>();
		_files = std::vector<File>();
	}

	Directory(
			std::string directory_name,
			const std::vector<Directory> &sub_directories = std::vector<Directory>(),
			const std::vector<File> &files = std::vector<File>()
	) : _name(std::move(directory_name)), _sub_directories(sub_directories), _files(files)
	{
		if (!sub_directories.empty() && !files.empty())
		{
			_size = calculate_size();
		}
	}

	[[nodiscard]] const auto &sub_directories() const { return _sub_directories; }

	[[nodiscard]] const auto &name() const { return _name; }

	auto add_sub_directory(const Directory &sub_directory)
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
	static size_t _calculate_size_of_files(const Directory &directory)
	{
		size_t size = 0;
		for (const auto &file: directory._files)
			size += file.size;
		return size;
	}

	[[nodiscard]] size_t _calculate_size(const Directory &directory) const
	{
		size_t size = 0;
		if (directory._sub_directories.empty())
			return _calculate_size_of_files(directory);
		for (const auto &sub_directory: directory._sub_directories)
		{
			size += _calculate_size(sub_directory);
		}
		return size;
	}

public:
	[[nodiscard]] size_t calculate_size() const
	{
		size_t size = _calculate_size_of_files(*this);
//		for (const auto &directory: _sub_directories)
//		{
//			size += _calculate_size(directory);
//		}
		return size;
	}

	[[nodiscard]] auto size() const { return _size; }

};

struct Filesystem
{
private:
	Directory _root;
	std::stack<Directory *> _last_cds;

	static inline auto remove_whitespace(std::string &string)
	{
		return string.erase(std::remove_if(string.begin(), string.end(), ::isspace), string.end());
	}

public:
	explicit Filesystem(const std::vector<std::string> &input)
	{
		for (const auto &line: input)
		{
			if (_is_command(line))
				read_command(line);
			else
				read_directory_structure(line);
		}
	}

private:
	static inline bool _is_command(const std::string &line) { return line.find('$') != std::string::npos; }

	static inline bool _is_root_directory(const std::string &command)
	{
		return command.find("cd /") != std::string::npos;
	}

	inline void _add_root_directory()
	{
		_root = Directory("/");
		_last_cds.push(&_root);
	}

	static inline bool _is_directory_up(const std::string &command) { return command.find("..") != std::string::npos; }

	inline void _move_directory_up() { _last_cds.pop(); }

	inline static const auto &_find_directory(const std::string &name, const Directory &directory)
	{
		if (directory.name() == name)
			return directory;

		for (const auto &dir: directory.sub_directories())
		{
			if (dir.name() == name)
				return dir;
		}
		throw std::logic_error(name + " not found in sub_directories");
	}

	inline static auto _get_entry_name(const std::string &entry)
	{
		std::string entry_name = entry.substr(entry.rfind(' '));
		remove_whitespace(entry_name);
		return entry_name;
	}

	inline void _change_directory(const std::string &command)
	{
		if (_is_root_directory(command))
		{
			_add_root_directory();
			return;
		}
		if (_is_directory_up(command))
		{
			_move_directory_up();
			return;
		}

		auto directory_name = _get_entry_name(command);

		auto &directory = const_cast<Directory &>(_find_directory(directory_name, *_last_cds.top()));

		_last_cds.push(&directory);
	}

	inline auto _add_to_last_directory(const Directory &directory)
	{
		auto &last_directory = _last_cds.top();
		last_directory->add_sub_directory(directory);
	}

	inline auto _add_to_last_directory(const File &file)
	{
		auto &last_directory = _last_cds.top();
		last_directory->add_file(file);
	}

public:
	void read_command(const std::string &command)
	{
		if (command.find('$') == std::string::npos)
			throw std::invalid_argument(R"('$' not found in ')" + command + "\'");


		if (command.find("cd") != std::string::npos)
		{
			_change_directory(command);
		}
		else if (command.find("ls") != std::string::npos)
		{
		}
		else
		{
			throw std::invalid_argument("\'" + command + "\' is an invalid command");
		}
	}

	void read_directory_structure(const std::string &entry)
	{
		if (entry.find("dir") != std::string::npos)
		{
			auto name = _get_entry_name(entry);

			_add_to_last_directory({ name });
		}
		else
		{
			auto index_at_space = entry.find(' ');
			if (index_at_space == std::string::npos)
				throw std::logic_error(entry + " must be file, but no space found");

			auto size = std::stoul(entry.substr(0, index_at_space));
			auto name = _get_entry_name(entry);

			_add_to_last_directory({ name, size, *_last_cds.top() });
		}
	}

	[[nodiscard]] static auto get_tmp(const Directory &directory)
	{
		std::vector<Directory> directories;

		for (const auto &sub_directory: directory.sub_directories())
		{
			auto directory_size = sub_directory.calculate_size();
			if (directory_size <= 100'000)
			{
				directories.push_back(directory);
			}
		}

		return directories;
	}

	[[nodiscard]] auto part_1() const
	{
		std::vector<Directory> directories;

		//_root
		// dir
		//	dir
		//	dir
		//		dir
		//		dir
		//	dir
		//	dir
		//		dir

		for (const auto &directory: _root.sub_directories())
		{
			if (!directory.sub_directories().empty())
			{
				auto tmp = get_tmp(directory);
				directories.insert(directories.end(), tmp.begin(), tmp.end());
			}
			auto directory_size = directory.calculate_size();
			if (directory_size <= 100'000)
			{
				directories.push_back(directory);
			}
		}

		return directories;
	}

	[[nodiscard]] auto sum_of_directories_part_1() const
	{
		auto directories_part_1 = part_1();

		size_t size = 0;
		for (const auto &directory: directories_part_1)
			size += directory.size();

		return size;
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

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "One Argument needed" << std::endl;
		return 1;
	}


	auto input = get_input(argv[ 1 ]);

	Filesystem filesystem(input);

	std::cout << filesystem.sum_of_directories_part_1() << std::endl;

	return 0;
}