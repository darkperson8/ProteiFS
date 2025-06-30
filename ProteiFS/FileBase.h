#pragma once

#include <filesystem>

class FileBase
{
public:
	enum class Type : char {None, File, Directory};
	FileBase(const std::filesystem::path& path) : path(path) {}
	FileBase(const FileBase&) = delete;
	FileBase& operator=(const FileBase&) = delete;
	FileBase(FileBase&&) noexcept = default;
	FileBase& operator=(FileBase&&) = delete;
	virtual Type type() const = 0;
	std::filesystem::path getPath() const { return path; }
	std::string getName() const { return path.filename().string(); }
	virtual void rename(const std::string& newName) = 0;
	virtual void remove() = 0;
	virtual ~FileBase() = default;
protected:
	std::filesystem::path path;
};
