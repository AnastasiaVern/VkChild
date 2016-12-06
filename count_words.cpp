#define _CRT_SECURE_NO_WARNINGS
#include <boost/process.hpp>
#include <regex>
#include <iostream>
#include <string>
#include <vector>

namespace bp = ::boost::process;


bp::child start_child(const std::string& path)
{
	std::vector<std::string> args{ path };

	bp::context ctx;
	ctx.stdout_behavior = bp::capture_stream();
	return bp::launch(path, args, ctx);
}
int main(int argc, char *argv[])
{
	try
	{
		if (argc < 2)
		{
			std::cerr << "Please specify a file name" << std::endl;
			return EXIT_FAILURE;
		}

		std::ifstream file(argv[1]);
		if (!file)
		{
			std::cerr << "Cannot open file" << std::endl;
			return EXIT_FAILURE;
		}

		bp::child cs = start_child(argv[1]);
		bp::pistream& is = cs.get_stdout();

		std::string line;
		std::regex finder("\\S+\\s*");
		std::cmatch res;
		size_t counter(0);

		while (std::getline(is, line)) {
			while (std::regex_search(line.c_str(), res, finder))
			{
				line = res.suffix().str();
				++counter;
			}
		}

		std::cout << "Words number= " << counter << std::endl;
		bp::status s = cs.wait();
	}
	catch (boost::filesystem::filesystem_error &ex)
	{
		std::cout << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
}
