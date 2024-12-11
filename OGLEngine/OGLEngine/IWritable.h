#pragma once
#include <string>
class IWritable
{
public:
	virtual bool Serialization(const std::string& filename);
	virtual bool Deserialization(const std::string& filename);
private:
};

