// ProteiFS.cpp: определяет точку входа для приложения.
//

#include "ProteiFS.h"
#include "File.h"
#include "Directory.h"

using namespace std;

int main()
{
	string line;
	Directory dir(R"(D:\Test)");
	auto entries = dir.getEntries();
	for (const auto& entry : entries)
		cout << entry->getName() << " : " << static_cast<int>(entry->type()) << endl;
	return 0;
}
