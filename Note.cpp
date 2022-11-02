#include "Note.h"

std::string Note::getTitle()
{
	return title;
}

std::string Note::getContent()
{
	return content;
}

std::vector<std::string> Note::getTags()
{
	return tags;
}

std::chrono::system_clock::rep Note::getCreated()
{
	return created.time_since_epoch().count();
}

std::chrono::system_clock::rep Note::getUpdated()
{
	return updated.time_since_epoch().count();
}

std::string Note::getCreatedAsString()
{
	std::time_t crt = system_clock::to_time_t(created);
	return ctime(&crt);
}

std::string Note::getUpdatedAsString()
{
	std::time_t upd = system_clock::to_time_t(updated);
	return ctime(&upd);
}

void Note::setTitle(const std::string& new_title)
{
	title = new_title;
}

void Note::setContent(const std::string& new_content)
{
	content = new_content;
}

void Note::setTags(const std::vector<std::string>& new_tags)
{
	tags = new_tags;
}

void Note::setUpdated(const system_clock::time_point& new_updated)
{
	updated = new_updated;
}