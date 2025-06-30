#include "Directory.h"

#include <exception>
#include <string>

namespace fs = std::filesystem;

Directory::Directory(const std::filesystem::path& path) : FileBase(path)
{
	if (!fs::exists(path))
		throw fs::filesystem_error("Failed to open directory: path does not exist", path, 
			std::make_error_code(std::errc::no_such_file_or_directory));
	if (!fs::is_directory(path))
		throw fs::filesystem_error("Failed to open directory: path is not a directory", path,
			std::make_error_code(std::errc::not_a_directory));
}

Directory makeDirectory(const fs::path& dirPath) {
	if (fs::exists(dirPath) && fs::is_directory(dirPath)) {
		throw fs::filesystem_error("Cannot create directory: path exists but is not a directory", dirPath,
			std::make_error_code(std::errc::file_exists));
		return Directory(dirPath);
	}
	try {
		if (!fs::create_directories(dirPath)) {
			throw fs::filesystem_error("Failed to create directory (unknown reason)", dirPath,
				std::make_error_code(std::errc::io_error));
		}
	}
	catch (const fs::filesystem_error& e) {
		throw fs::filesystem_error(
			std::string("Failed to create directory: ") + e.what(), dirPath, e.code());
	}
	return Directory(dirPath);
}

void Directory::rename(const std::string& newName)
{
	fs::path newPath = path.parent_path() / newName;
	try {
		fs::rename(path, newPath);
	}
	catch (const fs::filesystem_error& e) {
		throw fs::filesystem_error(std::string("Failed to rename directory: ") + e.what(), path, newPath, e.code());
	}
	path = std::move(newPath);
}

void Directory::remove()
{
	try {
		fs::remove_all(path);
	}
	catch (const fs::filesystem_error& e) {
		throw fs::filesystem_error("Failed to remove directory", path, e.code());
	}
	path = fs::path();
}

std::vector<File> Directory::getFiles() const
{
	std::vector<File> files;
	try {
		fs::directory_iterator it(path), end;
		for (it; it != end; ++it)
		{
			try {
				if (it->is_regular_file())
					files.emplace_back(it->path(), true);
			}
			catch (const fs::filesystem_error& e) {
				throw fs::filesystem_error(std::string("Failed to process file '") + it->path().filename().string() + "'", path,
					it->path().filename(), e.code());
			}
		}
	}
	catch (const fs::filesystem_error& e) {
		throw fs::filesystem_error(std::string("Failed to iterate directory '") + path.string() + "' : " + e.what(), 
			path, e.code());
	}
	return files;
}

std::vector<Directory> Directory::getSubdirs() const
{
	std::vector<Directory> subdirs;
	try {
		fs::directory_iterator it(path), end;
		for (it; it != end; ++it)
		{
			try {
				if (it->is_directory())
					subdirs.emplace_back(it->path());
			}
			catch (const fs::filesystem_error& e) {
				throw fs::filesystem_error(std::string("Failed to process subdir '") + it->path().filename().string() + "'", path,
					it->path().filename(), e.code());
			}
		}
	}
	catch (const fs::filesystem_error& e) {
		throw fs::filesystem_error(std::string("Failed to iterate directory '") + path.string() + "' : " + e.what(),
			path, e.code());
	}
	return subdirs;
}

std::vector<std::unique_ptr<FileBase>> Directory::getEntries() const
{
	std::vector<std::unique_ptr<FileBase>> entries;
	try {
		fs::directory_iterator it(path), end;
		for (it; it != end; ++it)
		{
			try {
				if (it->is_directory())
					entries.emplace_back(std::make_unique<Directory>(it->path()));
				else if (it->is_regular_file())
					entries.emplace_back(std::make_unique<File>(it->path(), true));
			}
			catch (const fs::filesystem_error& e) {
				throw fs::filesystem_error(std::string("Failed to process entry '") + it->path().filename().string() + "'", path,
					it->path().filename(), e.code());
			}
		}
	}
	catch (const fs::filesystem_error& e) {
		throw fs::filesystem_error(std::string("Failed to iterate directory '") + path.string() + "' : " + e.what(),
			path, e.code());
	}
	return entries;
}