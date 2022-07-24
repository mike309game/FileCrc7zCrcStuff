#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

static map<string, size_t> xap;

int main(int argc, char** argv) {
	ifstream file(argv[1]);
	string line;
	
	string fnameCurrent;
	bool fuckoff = false;
	
	while(getline(file, line)) {
		if(line.starts_with("Path = ")) {
			fnameCurrent = "\\2tbro\\ᗢfuck\\this drive backups\\" + line.substr(7, line.length() - 7);
			replace(fnameCurrent.begin(), fnameCurrent.end(), '/', '\\');
		} else if(line.starts_with("Size = ")) {
			size_t size = stoull(line.substr(7, line.length() - 7));
			if(size >= 157286400) {
				fuckoff = true;
			}
		} else if(line.starts_with("CRC = ")) {
			size_t crc = 0;
			if(line != "CRC = ") {
				crc = stoull(line.substr(6, line.length() - 6), nullptr, 16);
			}
			xap[fnameCurrent] = fuckoff ? 0xffffffff : crc;
			fuckoff = false;
		}
	}
	
	for(auto item : xap) {
		printf("%s\t0x%08x\n", item.first.c_str(), item.second);
	}
	
	return 0;
}