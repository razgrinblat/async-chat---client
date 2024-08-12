#include "Message.hpp"


template<class Archive>
void Message::serialize(Archive& archive)
{
	archive& _name;
	archive& _message;
}

Message::Message(const std::string& name, const std::string& message)
{
	_name = name;
	_message = message;
}

void Message::save(std::ostream& ostream) //serialize
{
	boost::archive::text_oarchive oarchive(ostream);
	oarchive << _name;
	oarchive << _message;

}

void Message::load(std::istream& istream) //deserialize
{
	boost::archive::text_iarchive iarchive(istream);
	iarchive >> _name;
	iarchive >> _message;

}

std::string Message::getName()
{
	return _name;
}

std::string Message::getMessage()
{
	return _message;
}
