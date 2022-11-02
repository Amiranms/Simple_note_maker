#pragma once
#include <chrono>
#include <ctime>
#include <string>
#include <vector>

using namespace std::chrono;

class Note
{
private:
	std::string title;
	std::string content;
	system_clock::time_point created;
	system_clock::time_point updated;
	std::vector<std::string> tags;

public:
	Note(
		const std::string& _title,
		const std::string& _content,
		const std::vector<std::string>& _tags
	) : title(_title), content(_content), tags(_tags), created(system_clock::now()), updated(created) {}

	Note(
		const std::string& _title,
		const std::string& _content,
		const std::vector<std::string>& _tags,
		const system_clock::time_point& _created,
		const system_clock::time_point& _updated
	) : title(_title), content(_content), tags(_tags), created(_created), updated(_updated) {}

	std::string getTitle();
	std::string getContent();
	std::vector<std::string> getTags();
	std::chrono::system_clock::rep getCreated();
	std::chrono::system_clock::rep getUpdated();
	std::string getCreatedAsString();
	std::string getUpdatedAsString();

	void setTitle(const std::string&);
	void setContent(const std::string&);
	void setTags(const std::vector<std::string>&);
	void setUpdated(const system_clock::time_point&);
};