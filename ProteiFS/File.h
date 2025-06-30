#pragma once

#include "FileBase.h"
#include <fstream>

class File : public FileBase
{
public:
	explicit File(const std::filesystem::path& path, bool lazy = false);
	File(const File&) = delete;
	File& operator=(const File&) = delete;
	File(File&&) noexcept;
	File& operator=(File&&) = delete;
	void rename(const std::string& newName) override;
	void writeLine(const std::string& line);
	bool readLine(std::string& out);
	void remove() override;
	void resetReadPos() { readPos = std::fstream::pos_type(0); }
	FileBase::Type type() const override { return FileBase::Type::File; }
	~File() { fileStream.close(); }
private:
	std::fstream fileStream;
	std::fstream::pos_type readPos;
	void ensureOpen();
};
