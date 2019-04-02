/*
 * File:   StdStream.hpp
 * Author: metalhead33
 *
 * Created on 2017. február 23., 17:46
 */

#ifndef STDSTREAM_HPP
#define STDSTREAM_HPP
#include "FIO.hpp"
#include <cstdio>
#include <string>
class StdStream : public Abstract::FIO {
public:
	StdStream();
	StdStream(const std::string& newpath, bool ro = false);
	~StdStream();
	virtual int64_t read(void* data, int64_t size);
	virtual int64_t seek(int64_t position);
	virtual int64_t tell();
	virtual int64_t size();
	virtual int64_t write(const void* data, int64_t size);
	virtual char getc();
	std::string GetPath();
	void close();
	bool IsActive();
	bool open(const std::string& newpath, bool ro = false);

	static Abstract::sFIO createReader(const std::string& newpath);
	static Abstract::sFIO createWriter(const std::string& newpath);

	virtual Abstract::byteBuffer loadIntoBuffer();
	virtual std::string stringize();
private:
	std::string path;
	FILE* chandle;
	bool active;
};
#endif /* STDSTREAM_HPP */
