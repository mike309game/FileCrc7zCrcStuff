//#include <corecrt.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
//#include <process.h>
#include <stdlib.h>
#include <ftw.h>
#include <string>
#include <algorithm>
#include <zconf.h>
#include <zlib.h>
#include <map>

static size_t threshold = 157286400; //150 mb
static std::map<std::string, size_t> map;

int Recurse(const char *fpath, const struct stat64 *sb, int typeFlag) {
    if(typeFlag != FTW_D) { //not dir
		size_t crcResult = 0xffffffff;
		if(sb->st_size < threshold) {
			FILE* fp = fopen64(fpath, "rb");
			if(fp) {
				size_t size;
				fseek(fp, 0, SEEK_END);
				size = ftell(fp);
				rewind(fp);
				char* buffer = (char*)malloc(size);
				if(buffer) {
					fread(buffer, size, 1, fp);
					crcResult = crc32(0L, (Bytef*)buffer, (uInt)size);
					free(buffer);
				} else {
					printf("COULDN'T ALLOC MEM AT %s\n", fpath);
				}
				fclose(fp);
			} else {
				printf("FOPEN FAIL %s\n", fpath);
				std::cout << fpath << std::endl;
			}
		}
		std::string path(fpath);
		std::replace(path.begin(), path.end(), '/', '\\');
		map[path] = crcResult;
    }
    return 0;
}

int main(int argc, char** argv) {
    if(argc < 1) {
        perror("fuck you");
        exit(EXIT_FAILURE);
    } else {
        ftw64(argv[1], Recurse, 8);
		for(auto item : map) {
			printf("%s\t0x%08x\n", item.first.c_str(), item.second);
		}
	}
    //system("pause");
}
