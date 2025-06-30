#include "File.h"

#include <exception>
#include <string>

namespace fs = std::filesystem;

void File::ensureOpen()
{
	if (!fileStream.is_open())
	{
		fileStream.open(path, std::ios::in | std::ios::out);
		if (!fileStream.is_open())
			throw fs::filesystem_error("Failed to open file stream", path, std::make_error_code(std::errc::io_error));
	}
}

File::File(const fs::path& path, bool lazy) : FileBase(path), readPos(0)
{
	if (!fs::exists(path))
		throw fs::filesystem_error("Failed to open file: path does not exist", path,
			std::make_error_code(std::errc::no_such_file_or_directory));
	if (!fs::is_regular_file(path))
		throw fs::filesystem_error("Failed to open file: path is not a file", path,
			std::make_error_code(std::errc::is_a_directory));
	if (!lazy)
		ensureOpen();
}

File::File(File&& other) noexcept : FileBase(std::move(other.path)), fileStream(std::move(other.fileStream)), 
									readPos(other.readPos)
{
	other.readPos = std::fstream::pos_type(0);
	other.path = fs::path();
	other.fileStream.setstate(std::ios::badbit);
}

void File::rename(const std::string& newName)
{
	bool was_opened = fileStream.is_open();
	fileStream.close();
	fs::path newPath = path.parent_path() / (newName + path.extension().string());
	try {
		fs::rename(path, newPath);
	}
	catch (const fs::filesystem_error& e) {
		throw fs::filesystem_error(std::string("Failed to rename file: ") + e.what(), path, newPath, e.code());
	}
	path = std::move(newPath);
	if (was_opened)
	{
		fileStream.open(path, std::ios::in | std::ios::out);
		if (!fileStream.is_open())
			throw fs::filesystem_error("Failed to reopen file after rename", std::make_error_code(std::errc::io_error));
	}
}

void File::remove()
{
	fileStream.close();
	try
	{
		fs::remove(path);
	}
	catch (const fs::filesystem_error& e)
	{
		throw fs::filesystem_error("Failed to remove file", path, e.code());
	}
	path.clear();
}

void File::writeLine(const std::string& line)
{
	ensureOpen();
	fileStream.clear();
	fileStream.seekp(0, std::ios::end);
	fileStream << line << std::endl;
	if (fileStream.fail())
		throw fs::filesystem_error("Failed to write line to file", path, 
			std::make_error_code(std::errc::io_error));
}

bool File::readLine(std::string& out)
{
	ensureOpen();
	fileStream.clear();
	fileStream.seekg(readPos);
	if (!std::getline(fileStream, out))
	{
		if (fileStream.eof())
		{
			readPos = std::fstream::pos_type(0);
			return false;
		}
		throw fs::filesystem_error("Failed to read line from file", path,
			std::make_error_code(std::errc::io_error));
	}
	readPos = fileStream.tellg();
	return true;
}