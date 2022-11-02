#include "Application.h"

const std::unordered_map<std::string, int> Application::command = {
	{ "help", 0 },
	{ "add", 1 },
	{ "del", 2 },
	{ "get", 3 },
	{ "upd", 4 },
	{ "filter", 5 },
	{ "quit", 6 }
};

Application& Application::app()
{
	static Application instance;
	return instance;
}

void Application::trim(std::string& str)
{
	str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
		return !std::isspace(ch);
	}));

	str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), str.end());
}

void Application::open()
{
	std::cout << "APP  : 'NoteMaker' app launched..." << std::endl;

	std::ifstream fin;
	fin.open("storage.txt");

	if (fin.is_open())
	{
		int n = 0;
		std::string input;

		if (fin.peek() != EOF)
		{
			fin >> n;
		}

		for (int i = 0; i < n; i++)
		{
			std::getline(fin, input);

			std::string title;
			std::string content;
			std::string tag;

			std::getline(fin, title);
			std::getline(fin, content);
			std::getline(fin, input);

			std::vector<std::string> tags;
			std::stringstream input_stringstream(input);

			while (std::getline(input_stringstream, tag, ','))
			{
				trim(tag);

				if (!tag.empty())
				{
					tags.push_back(tag);
				}
			}

			std::chrono::system_clock::rep time;

			fin >> time;
			std::chrono::system_clock::time_point created{ std::chrono::system_clock::duration{time} };

			fin >> time;
			std::chrono::system_clock::time_point updated{ std::chrono::system_clock::duration{time} };

			collection.emplace_back(title, content, tags, created, updated);
		}

		fin.close();
		std::cout << "TIP  : To get a command list, enter \"help\"..." << std::endl;
	}
	else
	{
		std::cout << "ERROR: Can not read notes from the storage file..." << std::endl;
	}
}

void Application::close()
{
	std::ofstream fout;
	fout.open("storage.txt");

	if (fout.is_open())
	{
		fout << collection.size() << std::endl;

		for (auto &note : collection)
		{
			fout << note.getTitle() << std::endl;
			fout << note.getContent() << std::endl;

			std::vector<std::string> tags = note.getTags();
			for (int i = 0; i < (int)tags.size(); i++)
			{
				fout << tags[i] << ", " + (i == (int)tags.size() - 1);
			}
			fout << std::endl;

			fout << note.getCreated() << std::endl;
			fout << note.getUpdated() << std::endl;
		}

		fout.close();
	}
	else
	{
		std::cout << "ERROR: Can not write notes to the storage file..." << std::endl;
	}

	std::cout << "APP  : 'NoteMaker' app closed..." << std::endl;
}

int Application::checkRequest(const std::string _req)
{
	std::getline(std::cin, req);
	trim(req);

	auto it = command.find(_req);
	return (it != command.end() ? it->second : -1);
}

void Application::printCommandList()
{
	if (!req.empty())
	{
		std::cout << "WARN : Extra input found..." << std::endl;
	}

	std::cout << "APP  : Command list:" << std::endl;
	std::cout << "\tadd            - create a new note" << std::endl;
	std::cout << "\tdel all        - delete all notes" << std::endl;
	std::cout << "\tdel last       - delete last note" << std::endl;
	std::cout << "\tdel note_title - delete note with title 'note_title' if exists" << std::endl;
	std::cout << "\tget all        - get list of titles of all notes" << std::endl;
	std::cout << "\tget last       - get last note" << std::endl;
	std::cout << "\tget note_title - get note with the title 'note_title' if exists" << std::endl;
	std::cout << "\tupd last       - update last note" << std::endl;
	std::cout << "\tupd note_title - update note with the title 'note_title' if exists" << std::endl;
	std::cout << "\tfilter         - filter notes by tags" << std::endl;
	std::cout << "\tquit           - close the application" << std::endl;
}

void Application::parseRequestAdd()
{
	if (!req.empty())
	{
		std::cout << "WARN : Extra input found..." << std::endl;
	}

	std::cout << "APP  : Creating a new note..." << std::endl;
	std::cout << "TIP  : Title and Content have to be non-empty..." << std::endl;
	std::cout << "TIP  : Use a comma to list tags (i.e. \"school, important_info, another_tag...\")" << std::endl;

	std::string title;
	std::string content;
	std::string input;
	std::string tag;

	do
	{
		std::cout << "       Title > " << std::flush;
		std::getline(std::cin, title);
	} while (title.empty());
	
	do
	{
		std::cout << "     Content > " << std::flush;
		std::getline(std::cin, content);
	} while (content.empty());

	std::cout << "        Tags > " << std::flush;
	std::getline(std::cin, input);

	std::vector<std::string> tags;
	std::stringstream input_stringstream(input);

	while (std::getline(input_stringstream, tag, ','))
	{
		trim(tag);

		if (!tag.empty())
		{
			tags.push_back(tag);
		}
	}

	collection.emplace_back(title, content, tags);
}

void Application::parseRequestDel()
{
	if (req.empty()) 
	{
		std::cout << "ERROR: Incomplete input..." << std::endl;
		return;
	}

	if (req == "all")
	{
		if (collection.empty())
		{
			std::cout << "APP  : No notes have been created yet..." << std::endl;
		}
		else
		{
			collection.clear();
			std::cout << "APP  : All notes deleted..." << std::endl;
		}
		return;
	}

	if (req == "last")
	{
		if (collection.empty())
		{
			std::cout << "APP  : No notes have been created yet..." << std::endl;
		} 
		else
		{
			std::cout << "APP  : Last note has been deleted..." << std::endl;
			collection.pop_back();
		}
	}
	else
	{
		auto it = collection.begin();

		for (; it != collection.end(); it++)
		{
			if (req == it->getTitle())
			{
				break;
			}
		}

		if (it == collection.end())
		{
			std::cout << "APP  : Note \"" << req << "\" not found..." << std::endl;
		}
		else
		{
			std::cout << "APP  : Note \"" << it->getTitle() << "\" has been deleted..." << std::endl;
			collection.erase(it);
		}
	}
}

void Application::parseRequestGet()
{
	if (req.empty())
	{
		std::cout << "ERROR: Incomplete input..." << std::endl;
		return;
	}

	if (req == "all")
	{
		if (collection.empty())
		{
			std::cout << "APP  : No notes have been created yet..." << std::endl;
		}
		else
		{
			std::cout << "APP  : All stored notes:" << std::endl;
			for (int i = 0; i < (int)collection.size(); i++)
			{
				std::cout << std::setw(4) << i + 1 << " - " << collection[i].getTitle() << std::endl;
			}
		}
		return;
	}

	auto found = collection.begin();

	if (req == "last") {
		if (collection.empty())
		{
			std::cout << "APP  : No notes have been created yet..." << std::endl;
			return;
		}
		else
		{
			std::cout << "APP  : Last created note:" << std::endl;
			found += collection.size() - 1;
		}
	}
	else
	{
		for (; found != collection.end(); found++)
		{
			if (req == found->getTitle())
			{
				break;
			}
		}
	}

	if (found == collection.end())
	{
		std::cout << "APP  : Note \"" << req << "\" not found..." << std::endl;
	}
	else
	{
		std::cout << "       Title: " << found->getTitle() << std::endl;
		std::cout << "     Content: " << found->getContent() << std::endl;
		std::cout << "        Tags: ";

		auto tags = found->getTags();
		for (int i = 0; i < (int)tags.size(); i++)
		{
			std::cout << '"' << tags[i] << '"' << ", " + (i == (int)tags.size() - 1);
		}

		std::cout << std::endl;
		std::cout << "     Created: " << found->getCreatedAsString() << std::flush;
		std::cout << "     Updated: " << found->getUpdatedAsString() << std::flush;
	}
}

void Application::parseRequestUpd()
{
	if (req.empty())
	{
		std::cout << "ERROR: Incomplete input..." << std::endl;
		return;
	}

	auto found = collection.begin();

	if (req == "last") {
		if (collection.empty())
		{
			std::cout << "APP  : No notes have been created yet..." << std::endl;
			return;
		}
		else
		{
			found += collection.size() - 1;
		}
	}
	else
	{
		for (; found != collection.end(); found++)
		{
			if (req == found->getTitle())
			{
				break;
			}
		}
	}

	if (found == collection.end())
	{
		std::cout << "APP  : Note \"" << req << "\" not found..." << std::endl;
	}
	else
	{
		std::cout << "APP  : Selected note..." << std::endl;
		std::cout << "       Title: " << found->getTitle() << std::endl;
		std::cout << "     Content: " << found->getContent() << std::endl;
		std::cout << "        Tags: ";

		auto tags = found->getTags();
		for (int i = 0; i < (int)tags.size(); i++)
		{
			std::cout << '"' << tags[i] << '"' << ", " + (i == (int)tags.size() - 1);
		}

		std::cout << std::endl;
		std::cout << "APP  : Fill in the fields to update the note..." << std::endl;
		std::cout << "TIP  : Leave the Title and Content fields blank to leave them unchanged..." << std::endl;
		std::cout << "TIP  : Enter '-' in the Tags field to leave it unchanged..." << std::endl;

		std::string title;
		std::string content;
		std::string input;
		std::string tag;

		std::cout << "   New Title > " << std::flush;
		std::getline(std::cin, title);

		std::cout << " New Content > " << std::flush;
		std::getline(std::cin, content);

		std::cout << "    New Tags > " << std::flush;
		std::getline(std::cin, input);

		tags.clear();
		std::stringstream input_stringstream(input);

		while (std::getline(input_stringstream, tag, ','))
		{
			trim(tag);

			if (!tag.empty())
			{
				tags.push_back(tag);
			}
		}

		if (!title.empty())
		{
			found->setTitle(title);
		}
		
		if (!content.empty())
		{
			found->setContent(content);
		}

		if (!tags.empty() && tags.back() != "-")
		{
			found->setTags(tags);
		}

		found->setUpdated(system_clock::now());
	}
}

void Application::parseRequestFilter()
{
	if (!req.empty())
	{
		std::cout << "WARN : Extra input found..." << std::endl;
	}

	std::cout << "TIP  : Enter a comma-separated list of tags or filter notes without tags..." << std::endl;
	std::cout << "        Tags > " << std::flush;

	std::string input;
	std::string tag;

	std::getline(std::cin, input);

	std::stringstream input_stringstream(input);
	std::vector<bool> filter(collection.size());

	std::vector<std::string> tags;
	while (std::getline(input_stringstream, tag, ','))
	{
		trim(tag);

		if (!tag.empty())
		{
			tags.push_back(tag);
		}
	}

	for (int i = 0; i < (int)collection.size(); i++)
	{
		std::vector<std::string> note_tags = collection[i].getTags();

		for (int j = 0; j < (int)tags.size(); j++)
		{
			if (filter[i] || j == 0)
			{
				filter[i] = std::find(note_tags.begin(), note_tags.end(), tag) != note_tags.end();
			}
		}
	}

	int last = 1;
	bool found = false;
	for (int i = 0; i < (int)filter.size(); i++)
	{
		if (filter[i])
		{
			found = true;

			std::cout << std::endl;
			std::cout << std::setw(4) << last++ << "   Title: " << collection[i].getTitle() << std::endl;
			std::cout << "     Content: " << collection[i].getContent() << std::endl;
			std::cout << "        Tags: ";

			auto tags = collection[i].getTags();
			for (int i = 0; i < (int)tags.size(); i++)
			{
				std::cout << '"' << tags[i] << '"' << ", " + (i == (int)tags.size() - 1);
			}

			std::cout << std::endl;
			std::cout << "     Created: " << collection[i].getCreatedAsString() << std::flush;
			std::cout << "     Updated: " << collection[i].getUpdatedAsString() << std::flush;
		}
	}

	if (!found)
	{
		std::cout << "APP  : No notes with these tags found..." << std::endl;
	}
}

void Application::run()
{
	isRunning = true;

	while (isRunning)
	{
		std::cout << "> " << std::flush;
		std::cin >> req;

		switch (checkRequest(req))
		{
		case 0:
			printCommandList();
			break;
		case 1:
			parseRequestAdd();
			break;
		case 2:
			parseRequestDel();
			break;
		case 3:
			parseRequestGet();
			break;
		case 4:
			parseRequestUpd();
			break;
		case 5:
			parseRequestFilter();
			break;
		case 6:
			isRunning = false;
			break;
		default:
			std::cout << "WARN : Command not found..." << std::endl;
		}
	}
}