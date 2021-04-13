#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#include <vector>
#include <unordered_map>
#include "serialization/Field.h"

//TODO ��� ������������� �� ����������� �������� ���������� ����������� ��������� ��� ����� �������
class Message {
public:
    using byte_t = char;
	enum Endianness : char {
		MSG_BIG_ENDIAN,
        MSG_LITTLE_ENDIAN
	};
	Message(Endianness endianness = MSG_LITTLE_ENDIAN);
	~Message();
	void addField(Field field);
	void addField(std::string field_name, size_t num_bytes);
	std::vector<Field>::iterator get(std::string field_name);
	std::vector<Field>::const_iterator get(std::string field_name) const;
	std::vector<Field>::iterator begin();
	std::vector<Field>::iterator end();
	std::vector<Field>::const_iterator cbegin() const;
	std::vector<Field>::const_iterator cend() const;
	std::vector<byte_t> toBinary() const;
	void fromBinary(const std::vector<char> &bytes, size_t start = 0);
	size_t size();
private:
	std::vector<Field> fields;
	std::unordered_map<std::string, size_t> index;
	Endianness _endianness;
    size_t size_message;
	
};
#endif