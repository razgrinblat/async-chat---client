#pragma once

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
class Message
{
private:
	friend class boost::serialization::access;
	std::string _name;
	std::string _message;


	template<class Archive>
	void serialize(Archive& archive);
public:
	Message() = default;
	Message(const std::string& name, const std::string& message);
	~Message() = default;
	void save(std::ostream& ostream);
	void load(std::istream& istream);
	std::string getName();
	std::string getMessage();
};



