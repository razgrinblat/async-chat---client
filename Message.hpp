#pragma once

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
class Message
{
private:
	
	std::string _color;
	std::string _name;
	std::string _message;

public:
	Message() = default;
	Message(const std::string& color, const std::string& name, const std::string& message);
	~Message() = default;
	void save(std::ostream& ostream);
	void load(std::istream& istream);
	std::string getColor();
	std::string getName();
	std::string getMessage();
};



