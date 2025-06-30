#pragma once

#include "FileBase.h"
#include "File.h"
#include <memory>


class Directory : public FileBase
{
public:
	explicit Directory(const std::filesystem::path& path);
	Directory(const Directory&) = delete;
	Directory& operator=(const Directory&) = delete;
	Directory(Directory&&) noexcept = default;
	Directory& operator=(Directory&&) = delete;
	std::vector<File> getFiles() const;
	std::vector<Directory> getSubdirs() const;
	std::vector<std::unique_ptr<FileBase>> getEntries() const;
	void rename(const std::string& newName) override;
	void remove() override;
	FileBase::Type type() const override { return FileBase::Type::Directory; }
};

Directory makeDirectory(const std::filesystem::path& dirPath);