#include "FileUtil.h"

#if defined(_WIN32)
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

bool ensureDir(const char* path) {
	if (!path || !*path) return false;
	// try create; if exists, that's fine
	int rc = MKDIR(path);
	(void)rc;
	return true;
}

void ensureDataDirs() {
	ensureDir("data");
	ensureDir("data/players");
	ensureDir("data/matches");
}


