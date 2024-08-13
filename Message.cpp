#include "Message.hpp"

Message::Message(const std::string& color, const std::string& name, const std::string& message)
{
	_color = color;
	_name = name;
	_message = message;
}

void Message::save(std::ostream& ostream) //serialize
{
	boost::archive::text_oarchive oarchive(ostream);
	oarchive << _color;
	oarchive << _name;
	oarchive << _message;

}

void Message::load(std::istream& istream) //deserialize
{
	boost::archive::text_iarchive iarchive(istream);
	iarchive >> _color;
	iarchive >> _name;
	iarchive >> _message;

}

std::string Message::getColor()
{
	return _color;
}

std::string Message::getName()	
{	
	return _name;
}

std::string Message::getMessage()
{
	return _message;
}
