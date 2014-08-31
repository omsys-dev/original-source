// setini.c sets .ini file from command line
// Copyright 2000 Omni Systems, Inc. and Deborah Pembrook. All rights reserved.

#include <stdio.h>
#include <windows.h>

// usage: setini inifile.ini section key value

int main(int argc, char *argv[])
{
	char *section  = NULL;
	char *keyword = NULL;
	char *value = NULL;
	char *path = NULL;
	char *newpath = NULL;
	FILE *ini = NULL;


	//notify user of deleted files or usage
	switch (argc){
		case 5:
			break;
		case 4:
			fprintf(stderr, "Deleting %s.\n", argv[3]);
			break;
		case 3:
			fprintf(stderr, "Deleting all keywords from %s.\n", argv[2]);
			break;
		default:
			fprintf(stderr, "usage: setini inifile.ini section key value\n");
			return 2;
	}

	path = argv[1];
	section = argv[2];
	if (argc > 3)
		keyword = argv[3];
	if (argc > 4)
		value = argv[4];

	// ensure full path is present
	if (strchr(path, '\\') == NULL) {
		newpath = (char *) calloc(strlen(path) + 3, sizeof(char));
		if (path[1] == ':') {
			newpath[0] = path[0];
			newpath[1] = path[1];
			newpath[2] = '.';
			newpath[3] = '\\';
			strcpy(newpath + 4, path + 2);
		}
		else {
			newpath[0] = '.';
			newpath[1] = '\\';
			strcpy(newpath + 2, path);
		}
		path = newpath;
	}

	// ensure file exists
	if ((ini = fopen(path, "r")) == NULL){
		fprintf(stderr, "Error 1: %s does not exist.\n", path);
		if (newpath)		
			free(newpath);
		return 1;
	}
	else 
		fclose(ini);

	WritePrivateProfileString(section, keyword, value, path);
	if (newpath)		
		free(newpath);
	return 0;
}



//end of setini.c
