#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <cctype>

#define TRY_CONTINUE(x) try { (x); } catch (...) { continue; }

class stack_of_crates
{
private:
	std::stack<char> _stack;

public:
	explicit stack_of_crates(const std::string &elements)
	{
		for (const auto &element: elements)
		{
			_stack.push(element);
		}
	}


	bool empty() const { return _stack.empty(); }

	void push(const std::stack<char>::value_type &value) { _stack.push(value); }

	void pop() { _stack.pop(); }

	std::stack<char>::reference top() { return _stack.top(); }
};

struct crane_operator
{
	static void move(stack_of_crates &from, stack_of_crates &to, size_t times = 1)
	{
		std::vector<char> to_middleman;
		for (size_t i = 0; i < times; ++i)
		{
			auto element = from.top();
			to_middleman.push_back(element);
			from.pop();
		}

		for (auto it = to_middleman.rbegin(); it != to_middleman.rend(); ++it)
		{
			to.push(*it);
		}
	}
};

auto get_input_from_file(const std::string &file_path)
{
	std::fstream in_stream(file_path);

	if (!in_stream.is_open())
		throw std::logic_error(file_path + " is an invalid File Path");

	std::vector<std::string> input;

	for (std::string line; std::getline(in_stream, line);)
		input.push_back(line);

	return input;
}

struct from_input
{
private:
	std::vector<std::string> _input;

public:
	explicit from_input(std::vector<std::string> input) : _input(std::move(input)) { }

private:
	auto _get_stack_input()
	{
		std::vector<std::string> stack_input;

		for (const auto &line: _input)
		{
			if (line.empty())
				break;

			stack_input.push_back(line);
		}

		stack_input.erase(stack_input.end() - 1);
		std::reverse(stack_input.begin(), stack_input.end());

		std::vector<std::string> tmp;

		for (size_t column = 0; column < stack_input.front().length(); ++column)
		{
			if (::isspace(stack_input.front()[ column ]) || stack_input.front()[ column ] == '[' ||
				stack_input.front()[ column ] == ']')
				continue;

			std::string element;
			for (const auto &line: stack_input)
			{
				try
				{
					element += line.at(column);
				}
				catch (...)
				{
					continue;
				}
			}
			element.erase(std::remove_if(element.begin(), element.end(), ::isspace), element.end());
			tmp.push_back(element);
		}

		return tmp;
	}

public:

	auto get_stacks()
	{
		std::vector<std::string> stack_input = _get_stack_input();

		for (const auto &stack: stack_input)
			std::cout << stack << std::endl;

		std::vector<stack_of_crates> stacks;

		for (const auto &line: stack_input)
		{
			stack_of_crates stack_of_crates(line);
			stacks.push_back(stack_of_crates);
		}

		return stacks;
	}

	auto get_commands()
	{
		std::vector<std::string> commands;

		for (const auto &line: _input)
		{
			if (line.find("move") == std::string::npos && line.find("from") == std::string::npos &&
				line.find("to") == std::string::npos)
				continue;

			commands.push_back(line);
		}


		return commands;
	}

};

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "One Argument needed" << std::endl;
		return 1;
	}

	std::vector<std::string> commands;
	std::vector<stack_of_crates> stacks;
	{
		from_input from_input(get_input_from_file(argv[ 1 ]));
		stacks = from_input.get_stacks();
		commands = from_input.get_commands();
	}


	for (auto command: commands)
	{
		auto split_command = [](std::string &command) -> auto
		{
			std::vector<std::string> splitted;

			std::string tmp;
			for (const auto &c: command)
			{
				if (isspace(c))
				{
					splitted.push_back(tmp);
					tmp = "";
					continue;
				}

				tmp += c;
			}
			splitted.push_back(tmp);


			return splitted;
		};

		;
		auto command_splitted = split_command(command.erase(command.find("move "), 5)
													 .erase(command.find("from "), 5)
													 .erase(command.find("to "), 3));

		size_t nbr_of_crates_to_be_moved = std::stoi(command_splitted[ 0 ]);
		size_t from_stack_index = std::stoi(command_splitted[ 1 ]) - 1;
		size_t to_stack_index = std::stoi(command_splitted[ 2 ]) - 1;

		std::cout << "move " << nbr_of_crates_to_be_moved << " from " << from_stack_index << " to " << to_stack_index
				  << std::endl;
		if (from_stack_index >= stacks.size())
		{
			std::cerr << command << std::endl;
			std::cerr << "Invalid Index for \'from_stack_index\'" << std::endl;
		}
		if (to_stack_index >= stacks.size())
		{
			std::cerr << command << std::endl;
			std::cerr << "Invalid Index for \'to_stack_index\'" << std::endl;
		}

		crane_operator::move(stacks[ from_stack_index ], stacks[ to_stack_index ], nbr_of_crates_to_be_moved);
	}

	for (auto &stack: stacks)
		std::cout << stack.top();
	std::cout << std::endl;


	return 0;
}

