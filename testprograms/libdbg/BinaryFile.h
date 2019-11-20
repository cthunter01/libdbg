#ifndef BINARYFILE_H_
#define BINARYFILE_H_

#include <stddef.h> // Defines NULL
#include <string>
#include <sys/types.h>


class BinaryFile
{
public:
	explicit BinaryFile(const std::string& fileName);
	virtual ~BinaryFile();

	//virtual void load(const std::string& fileName);
	void* retrieve(off_t offset, size_t length);
private:

	//BinaryFile() : data(NULL), fileName(""), end((off_t)0) { } // No public default constructor
	void* data;
	std::string fileName;
	off_t end;

};

#endif /* BINARYFILE_H_ */
