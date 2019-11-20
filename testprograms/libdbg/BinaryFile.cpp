#include "BinaryFile.h"

#include <iostream>
#include <sys/mman.h> // For mmap
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>


BinaryFile::BinaryFile(const std::string& inFileName) :
	data(NULL),
	fileName(inFileName),
	end((off_t)0)
{
	// Open the file for reading. Use open() rather than ifstream because
	// mmap needs a real UNIX file descriptor
	int fd = open(fileName.c_str(), O_RDONLY);

	end = lseek(fd, 0, SEEK_END);

	data = mmap(NULL, end, PROT_READ, MAP_SHARED, fd, 0);

	if(data == MAP_FAILED)
	{
		std::cerr << "Error mmap'ing file: " << fileName << "\n";
	}
	close(fd);

}

BinaryFile::~BinaryFile()
{
	munmap(data, end);
}

void* BinaryFile::retrieve(off_t offset, size_t length)
{
	if((offset + length) <= end)
	{
		return reinterpret_cast<unsigned char*>(data) + offset;
	}
	else
	{
		std::cerr << "Requesting offset " << offset << " with length " << length
				  << " from file: " << fileName << " failed.";
		return NULL;
	}
}
