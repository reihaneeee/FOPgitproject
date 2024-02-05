#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
char test_push[100];

char cwd[256];
char root[256];
char duplicate_argv[10][100];
char user_name_mode[5];
char user_name[256];
char user_email_mode[5];
char user_email[256];
char user_alias_command[1000];
char legal_commands[50][20];

int is_dir(char * path)
{
	struct stat stats;
    stat(path, &stats);
    if (S_ISDIR(stats.st_mode))
        return 1;
    return 0;
}

int is_file(char * Path)
{
	FILE * fp = fopen(Path, "r");
	if (!fp) {
		return 0;
	}
	fclose(fp);
	return 1;
}

void save_configuration()
{
	FILE * fp = fopen(".neogit\\Configuration_files\\local_configuration", "w");
	if (!fp)
	{
		printf("can not open configuration file\n");
		return;
	}
	fprintf(fp, "%s\n", user_name_mode);
	fprintf(fp, "%s\n", user_name);
	fprintf(fp, "%s\n", user_email_mode);
	fprintf(fp, "%s\n", user_email);
	fclose(fp);
}

void init(int argc)
{
	if(argc > 2)
	{
		printf("wrong or extra parameters");
		return;
	}
  	char TempDir[256];
  	char TempAdd[256];
   	strcpy(TempDir, cwd);
	strcat(TempDir, "\\.neogit");
	if (is_dir(TempDir))
	{
		printf("an initialized git found in current directory\n");
	   	return;	
	}
	
  	strcpy(TempDir, cwd);
  	int Pos = strlen(TempDir) - 1;
  	while(Pos >= 0)
  	{
  		if (TempDir[Pos] != '\\')
		{
			TempDir[Pos] = '\0';
			Pos--;
		}
		else
		{
			strcpy(TempAdd, TempDir);
			strcat(TempAdd, ".neogit");
			if (is_dir(TempAdd))
			{
				TempDir[Pos] = '\0';
				printf("an initialized git found in %s\n", TempDir);
		       	return;	
			}
			Pos--;
		}
	}
	
	char TempCommand[1000];
	system("echo OFF");
	
	strcpy(TempCommand, "md ");
	strcpy(TempDir, cwd);
	strcat(TempDir, "\\.neogit");
	strcat(TempCommand, TempDir);
	system(TempCommand);
	
	strcpy(TempCommand, "md ");
	strcpy(TempDir, cwd);
	strcat(TempDir, "\\.neogit\\Commit_files");	
	strcat(TempCommand, TempDir);
	system(TempCommand);
	
	strcpy(TempCommand, "md ");
	strcpy(TempDir, cwd);
	strcat(TempDir, "\\.neogit\\Configuration_files");	
	strcat(TempCommand, TempDir);
	system(TempCommand);

	FILE * fp;
	strcpy(TempDir, cwd);
	strcat(TempDir, "\\.neogit\\Configuration_files");
	
	strcpy(TempAdd, TempDir);
	strcat(TempAdd, "\\commits");
	fp = fopen(TempAdd, "w");
	fclose(fp);
	
	strcpy(TempAdd, TempDir);
	strcat(TempAdd, "\\local_alias");
	fp = fopen(TempAdd, "w");
	fclose(fp);
	
	strcpy(TempAdd, TempDir);
	strcat(TempAdd, "\\local_configuration");
	fp = fopen(TempAdd, "w");
	fclose(fp);
	
	strcpy(TempAdd, TempDir);
	strcat(TempAdd, "\\staged_files");
	fp = fopen(TempAdd, "w");
	fclose(fp);
	
	strcpy(TempAdd, TempDir);
	strcat(TempAdd, "\\last_add");
	fp = fopen(TempAdd, "w");
	fprintf(fp, "%s", "NoData");
	fclose(fp);
	
	strcpy(TempAdd, TempDir);
	strcat(TempAdd, "\\last_reset");
	fp = fopen(TempAdd, "w");
	fprintf(fp, "%s", "NoData");
	fclose(fp);
	
	system("attrib .neogit +h");
	system("echo ON");
  	printf("Initialized neogit in %s\n", cwd);
  	save_configuration();
}

void config_alias(int argc, char * argv[], int global)
{
	if ((argc + global) < 4 + global)
	{
		printf("need more parameters\n");
		return;
	}
	FILE * fp;
	char aliasString[100];
	int alias_argc;
	char alias_argv[100][100];
	char Temp[1000];
	int i, Pos = 0;
	for(i = 6; duplicate_argv[2 + global][i]; i++)
	{
		aliasString[Pos++] = duplicate_argv[2 + global][i];
	}
	aliasString[Pos] = '\0';
	char Command[1000];
	strcpy(Command, "ParseCommand ");
	strcat(Command, argv[3 + global]);
	alias_argc = system(Command);
	fp = fopen("c:\\git\\Temp", "r");
	if (!fp)
	{
		printf("there is an error to open temporary arg file\n");
		return;
	}
	Pos = 0;
	fscanf(fp, "%[^\n]s", alias_argv[Pos++]);
	while(!feof(fp))
	{
		fscanf(fp, "\n%[^\n]s", alias_argv[Pos++]);
	}
	fclose(fp);
	remove("c:\\git\\Temp");

	if (strcmp(alias_argv[1], "neogit"))
	{
		printf("alias command should start with neogit\n");
		return;
	}
	int Sw = 0;
	for(i = 0; i < 12; i++)
	{
		if (!strcmp(alias_argv[2], legal_commands[i]))
		{
			Sw = 1;
		}
	}
	if (!Sw)
	{
		printf("command is not in legal commands\n");
		return;
	}
	if (global == 1)
	{
		fp = fopen("c:\\git\\global_alias", "a");
		if (!fp)
		{
			printf("error in finding global alias file\n");
			return;
		}
		fprintf(fp, "%s\n", aliasString);
		fprintf(fp, "%s\n", duplicate_argv[4]);
		fclose(fp);
	}
	else
	{
		strcpy(Temp, root);
		strcat(Temp, "\\.neogit\\Configuration_files\\local_alias");
		fp = fopen(Temp, "a");
		if (!fp)
		{
			printf("error in finding local alias file\n");
			return;
		}
		fprintf(fp, "%s\n", aliasString);
		fprintf(fp, "%s\n", duplicate_argv[3]);
		fclose(fp);
	}			
}

void config_user_name_email(int global)
{
	FILE * fp;
	char Temp[256];
	if (global)
	{
		fp = fopen("C:\\git\\global_configuration", "w");
		if (!fp)
		{
			printf("error in finding global configuration file\n");
			return;
		}	
		fprintf(fp, "%s\n", user_name);
		fprintf(fp, "%s\n", user_email);
		printf("Global Configuration Changed\n");
		fclose(fp);
	}
	strcpy(Temp, root);
	strcat(Temp, "\\.neogit\\Configuration_files");
	if (is_dir(Temp))
	{
		strcat(Temp, "\\local_configuration");
		fp = fopen(Temp, "w");
		if (!fp)
		{
			printf("error in finding local configuration file\n");
			return;
		}	
		fprintf(fp, "%s\n", user_name_mode);
		fprintf(fp, "%s\n", user_name);
		fprintf(fp, "%s\n", user_email_mode);
		fprintf(fp, "%s\n", user_email);
		printf("Local Configuration Changed\n");
		fclose(fp);
	}	
}

void config(int argc, char * argv[])
{
	int global = 0;
	if(argc > 5)
	{
		printf("wrong or extra parameters");
		return;
	}
	if (!strcmp(argv[2], "-global"))
	{
		global = 1;
	}	
	if (strstr(argv[2 + global], "alias."))
	{
		config_alias(argc, argv, global);
		return;
	}
	
	if (!strcmp(argv[2+global], "user.name"))
	{
		strcpy(user_name, duplicate_argv[3+global]);
		if (global)
			strcpy(user_name_mode, "G");
		else
			strcpy(user_name_mode, "L");
	}
	if (!strcmp(argv[2+global], "user.email"))
	{
		strcpy(user_email, duplicate_argv[3+global]);
		if (global)
			strcpy(user_email_mode, "G");
		else
			strcpy(user_email_mode, "L");
	}
	if (strcmp(argv[2+global], "user.name") && strcmp(argv[2+global], "user.email") && !strstr(argv[2 + global], "alias."))
	{
	   	printf("parameters are wrong\n");
		return;
	}
	config_user_name_email(global);
}

int in_Stage_mode(char * Path, char Staged[][256], int StagedCounter, int * Pos)
{
	int i;
	if (StagedCounter == 0) return 0;
	for (i = 0; i < StagedCounter; i++)
		if (!strcmp(Path, Staged[i]))
		{
			*Pos = i;
			return 1;
		}
	return 0;
}

int in_modified_mode(char * Path)
{
	return 1;
}

void file_staging(char * Path, char Staged[][256], int StagedCounter, int indir)
{
	char WFileAdd[256];
	strcpy(WFileAdd, root);
	strcat(WFileAdd, "\\.neogit\\Configuration_files\\staged_files");
	FILE * fp = fopen(WFileAdd, "a");
	if (!fp) {
		printf("stage file not found\n");
		return;
	}
	int Pos;
	if (!indir)
	{
		if (!in_Stage_mode(Path, Staged, StagedCounter, &Pos))
		{
			fprintf(fp, "%s\n", Path);
		}
	}
	if (indir)
	{
		if (!in_Stage_mode(Path, Staged, StagedCounter, &Pos) && in_modified_mode(Path))
		{
			fprintf(fp, "%s\n", Path);
		}		
	}
	fclose(fp);
}

void dir_staging(char * Path, char staged_files[][256], int staged_file_counter)
{
	struct dirent *entry;
	char Temp[256];
	DIR *dir = opendir(Path);
	if (dir == NULL) {
    	printf("directory not accessible %s\n", Path);
    	return;
	}
	while ((entry = readdir(dir)) != NULL)
	{
    	if (!strcmp((*entry).d_name, ".") || !strcmp((*entry).d_name, "..")) 
		{
      		continue;
    	}
    	strcpy(Temp, Path);
    	strcat(Temp, "\\");
    	strcat(Temp, (*entry).d_name);
    	if (is_file(Temp))
		{
			file_staging(Temp, staged_files, staged_file_counter, 1);
		}
		if (is_dir(Temp))
		{
			dir_staging(Temp, staged_files, staged_file_counter);
		}
	}
	closedir(dir);
}

void star_file_staging(char* Path, char staged_files[][256], int staged_file_counter)
{
	char Temp[1000];
	strcpy(Temp, "dir ");
	strcat(Temp, Path);
	strcat(Temp, " /b > c:\\git\\temp_file");
	system(Temp);
	
	FILE * fp = fopen("c:\\git\\temp_file", "r");
	if (!fp)
	{
		printf("there is an error\n");
		return;
	}
	
	char file_name[100];
	fscanf(fp, "%[^\n]s", file_name);
	while(!feof(fp))
	{
		strcpy(Temp, cwd);
		strcat(Temp, "\\");
		strcat(Temp, file_name);	
		if (is_file(Temp))
		{
			file_staging(Temp, staged_files, staged_file_counter, 0);
		}
		if (is_dir(Temp))
		{
			dir_staging(Temp, staged_files, staged_file_counter);
		}	
		fscanf(fp, "\n%[^\n]s", file_name);	
	}
	fclose(fp);
	remove("c:\\git\\temp_file");
	return;
}

void multi_file_staging(int argc, char *argv[], char staged_files[][256], int staged_file_counter)
{
	int i;
	char TempAdd[256];
	for (i = 3; i < argc; i++)
	{
		strcpy(TempAdd, cwd);
		strcat(TempAdd, "\\");
		strcat(TempAdd, argv[i]);
		if (!is_file(TempAdd) && !is_dir(TempAdd))
			printf("%s file or folder not accessible\n", TempAdd);
	
		if (is_file(TempAdd))
			file_staging(TempAdd, staged_files, staged_file_counter, 0);
		
		if (is_dir(TempAdd))
			dir_staging(TempAdd, staged_files, staged_file_counter);
	}
}

void read_staged_files(char staged_files[][256], int * staged_files_counter)
{
	char TempAdd[256];
	strcpy(TempAdd, root);
	strcat(TempAdd, "\\.neogit\\Configuration_files\\staged_files");
	FILE * fp = fopen(TempAdd, "r");
	if (!fp) {
		printf("staged file not found\n");
		return;
	}
	
	fscanf(fp, "%[^\n]s", staged_files[*staged_files_counter]);
	while(!feof(fp))
	{
		(*staged_files_counter)++;
		fscanf(fp, "\n%[^\n]s", staged_files[*staged_files_counter]);
	}
	fclose(fp);
	return;
}

long int load_last_commited_time()
{
	char TempAdd[256];
	time_t Time;
	strcpy(TempAdd, root);
	strcat(TempAdd, "\\.neogit\\Configuration_files\\commits");
	FILE * fp = fopen(TempAdd, "r");
	if (!fp) {
		printf("commits file not found\n");
		return 0;
	}
	fscanf(fp, "%[^\n]s", &Time);
	return 0;
}

void fill_last_add(int argc, char * argv[])
{
	char Temp[1000];
	strcpy(Temp, root);
	strcat(Temp, "\\.neogit\\Configuration_files\\last_add");
	FILE * fp = fopen(Temp, "w");
	if (!fp) {
		printf("there is an error\n");
		return;
	}
	strcpy(Temp, argv[2]);
	int i;
	for(i = 3; i < argc; i++)
	{
		strcat(Temp, " ");
		strcat(Temp, argv[i]);
	}
	fprintf(fp, "%s\n", Temp);
	fclose(fp);
}

void add_redo()
{
	char Temp[1000];
	char reset_command[1000];
	char command[1000];
	strcpy(Temp, root);
	strcat(Temp, "\\.neogit\\Configuration_files\\last_reset");
	FILE * fp = fopen(Temp, "r");
	if (!fp) {
		printf("there is an error\n");
		return;
	}
	fgets(reset_command, 1000, fp);
	fclose(fp);
	if (!strcmp(reset_command, "NoData"))
	{
		printf("No Reset Accure, Can\'t redo add\n");
		return;
	}
	strcpy(command, "neogit add ");
	strcat(command, reset_command);
	system(command);
	strcpy(Temp, root);
	strcat(Temp, "\\.neogit\\Configuration_files\\last_add");
	fp = fopen(Temp, "w");
	if (!fp) {
		printf("there is an error\n");
		return;
	}
	fprintf(fp, "%s", reset_command);
	fclose(fp);
	strcpy(Temp, root);
	strcat(Temp, "\\.neogit\\Configuration_files\\last_reset");
	fp = fopen(Temp, "w");
	if (!fp) {
		printf("there is an error\n");
		return;
	}
	fprintf(fp, "%s", "NoData");
	fclose(fp);
	return;
}

void add(int argc, char * argv[])
{
	char staged_files[100][256];
	char TempAdd[256];
	int staged_file_counter = 0;
	time_t commit_time;

	if (argc < 3)
	{
		printf("few parameters\n");
		return;
	}
	read_staged_files(staged_files, &staged_file_counter);
	commit_time = load_last_commited_time();
	
	if (!strcmp(argv[2], "-redo"))
	{
		add_redo();
		return;
	}
	
	if (!strcmp(argv[2], "-f"))
	{
		multi_file_staging(argc, argv, staged_files, staged_file_counter);
		fill_last_add(argc, argv);
		return;
	}
	
	strcpy(TempAdd, cwd);
	strcat(TempAdd, "\\");
	strcat(TempAdd, duplicate_argv[2]);
	
	if (strstr(duplicate_argv[2], "*"))
	{
		star_file_staging(TempAdd, staged_files, staged_file_counter);
		fill_last_add(argc, argv);
		return;
	}
		
	if (!is_file(TempAdd) && !is_dir(TempAdd))
	{
		printf("%s file or folder not found...\n", TempAdd);
		return;
	}
	if (is_file(TempAdd))
	{
		file_staging(TempAdd, staged_files, staged_file_counter, 0);
	}
	if (is_dir(TempAdd))
	{
		dir_staging(TempAdd, staged_files, staged_file_counter);
	}
	fill_last_add(argc, argv);
	printf("file or directory added\n");
	return;
}

void file_reseting(char * Path, char staged_files[][256], int staged_file_Counter, char Reseted[])
{
	int Pos;
	if (in_Stage_mode(Path, staged_files, staged_file_Counter, &Pos))
	{
		Reseted[Pos] = '1';
	}
}

void dir_reseting(char * Path, char staged_files[][256], int staged_file_Counter, char Reseted[])
{
	DIR *dir = opendir(Path);
	if (dir == NULL) {
    	printf("cannot open directory %s\n", Path);
    	return;
	}
	struct dirent *entry;
	char Temp[256];
	while ((entry = readdir(dir)) != NULL)
	{
    	if (!strcmp((*entry).d_name, ".") || !strcmp((*entry).d_name, "..")) 
		{
      		continue;
    	}
    	strcpy(Temp, Path);
    	strcat(Temp, "\\");
    	strcat(Temp, (*entry).d_name);
    	if (is_file(Temp))
		{
			file_reseting(Temp, staged_files, staged_file_Counter, Reseted);
		}
		if (is_dir(Temp))
		{
			dir_reseting(Temp, staged_files, staged_file_Counter, Reseted);
		}
	}
}

void star_file_reseting(char* Path, char staged_files[][256], int staged_file_counter, char Reseted[])
{
	char Temp[1000];
	strcpy(Temp, "dir ");
	strcat(Temp, Path);
	strcat(Temp, " /b > c:\\git\\temp_file");
	system(Temp);
	
	FILE * fp = fopen("c:\\git\\temp_file", "r");
	if (!fp)
	{
		printf("there is an error\n");
		return;
	}
	
	char file_name[100];
	fscanf(fp, "%[^\n]s", file_name);
	while(!feof(fp))
	{
		strcpy(Temp, cwd);
		strcat(Temp, "\\");
		strcat(Temp, file_name);	
		if (is_file(Temp))
		{
			file_reseting(Temp, staged_files, staged_file_counter, Reseted);
		}
		if (is_dir(Temp))
		{
			dir_reseting(Temp, staged_files, staged_file_counter, Reseted);
		}	
		fscanf(fp, "\n%[^\n]s", file_name);	
	}
	fclose(fp);
	remove("c:\\git\\temp_file");
	return;
}


void multi_file_reseting(int argc, char *argv[], char Staged[][256], int StagedCounter, char Reseted[])
{
	int i;
	char WFileAdd[256];
	for (i = 3; i < argc; i++)
	{
		strcpy(WFileAdd, cwd);
		strcat(WFileAdd, "\\");
		strcat(WFileAdd, argv[i]);
		if (!is_file(WFileAdd) && !is_dir(WFileAdd))
			printf("%s file or folder not found...\n", WFileAdd);
	
		if (is_file(WFileAdd))
			file_reseting(WFileAdd, Staged, StagedCounter, Reseted);
		
		if (is_dir(WFileAdd))
			dir_reseting(WFileAdd, Staged, StagedCounter, Reseted);
	}
}

void save_reseted_files(char staged_files[][256], int staged_file_counter, char reseted[])
{
	char Temp[256];
	int i;
	strcpy(Temp, root);
	strcat(Temp, "\\.neogit\\Configuration_files\\staged_files");
	FILE * fp = fopen(Temp, "w");
	for(i = 0; i < staged_file_counter; i++)
	{
		if (reseted[i] == '0')
			fprintf(fp, "%s\n", staged_files[i]);
	}
	fclose(fp);	
}

void fill_last_reset(int argc, char * argv[])
{
	char Temp[1000];
	strcpy(Temp, root);
	strcat(Temp, "\\.neogit\\Configuration_files\\last_reset");
	FILE * fp = fopen(Temp, "w");
	if (!fp) {
		printf("there is an error\n");
		return;
	}
	strcpy(Temp, argv[2]);
	int i;
	for(i = 3; i < argc; i++)
	{
		strcat(Temp, " ");
		strcat(Temp, argv[i]);
	}
	fprintf(fp, "%s\n", Temp);
	fclose(fp);
}

void reset_undo()
{
	char Temp[1000];
	char add_command[1000];
	char command[1000];
	strcpy(Temp, root);
	strcat(Temp, "\\.neogit\\Configuration_files\\last_add");
	FILE * fp = fopen(Temp, "r");
	if (!fp) {
		printf("there is an error\n");
		return;
	}
	fgets(add_command, 1000, fp);
	fclose(fp);
	if (!strcmp(add_command, "NoData"))
	{
		printf("No Add Accure, Can\'t undo reset\n");
		return;
	}
	strcpy(command, "neogit reset ");
	strcat(command, add_command);
	system(command);
	strcpy(Temp, root);
	strcat(Temp, "\\.neogit\\Configuration_files\\last_reset");
	fp = fopen(Temp, "w");
	if (!fp) {
		printf("there is an error\n");
		return;
	}
	fprintf(fp, "%s", add_command);
	fclose(fp);
	strcpy(Temp, root);
	strcat(Temp, "\\.neogit\\Configuration_files\\last_add");
	fp = fopen(Temp, "w");
	if (!fp) {
		printf("there is an error\n");
		return;
	}
	fprintf(fp, "%s", "NoData");
	fclose(fp);
	return;
}

void reset(int argc, char * argv[])
{
	char Temp[256];
	char staged_files[100][256];
	char reseted[100];
	int staged_file_counter = 0;
	FILE * fp;
	
	read_staged_files(staged_files, &staged_file_counter);
	
	int i;
	for(i = 0; i < staged_file_counter; i++)
		reseted[i] = '0';
		
	if (!strcmp(argv[2], "-undo"))
	{
		reset_undo();
		return;
	}	
		
	if (!strcmp(argv[2], "-f"))
	{
		multi_file_reseting(argc, argv, staged_files, staged_file_counter, reseted);
		save_reseted_files(staged_files, staged_file_counter, reseted);
		fill_last_reset(argc, argv);
		return;
	}
	
	strcpy(Temp, cwd);
	strcat(Temp, "\\");
	strcat(Temp, argv[2]);
	
	if (strstr(duplicate_argv[2], "*"))
	{
		star_file_reseting(Temp, staged_files, staged_file_counter, reseted);
		save_reseted_files(staged_files, staged_file_counter, reseted);
		fill_last_reset(argc, argv);
		return;
	}
	
	if (!is_file(Temp) && !is_dir(Temp))
	{
		printf("%s file or folder not found\n", Temp);
		return;
	}
	if (is_file(Temp))
	{
		file_reseting(Temp, staged_files, staged_file_counter, reseted);
	}
		
	if (is_dir(Temp))
	{
		dir_reseting(Temp, staged_files, staged_file_counter, reseted);
	}
	save_reseted_files(staged_files, staged_file_counter, reseted);
	fill_last_reset(argc, argv);
	printf("file or directory reseted\n");
	return;
}

void load_alias(char aliasNames[100][100], char aliasCommands[100][1000], int * aliasCounter)
{
	FILE * fp = fopen("c:\\git\\global_alias", "r");
	if (!fp)
	{
		printf("global alias file not found\n");
		return;
	}
	fscanf(fp, "%[^\n]s", aliasNames[*aliasCounter]);
	while(!feof(fp))
	{
		fscanf(fp, "\n%[^\n]s", aliasCommands[*aliasCounter]);
		(*aliasCounter)++;
		fscanf(fp, "\n%[^\n]s", aliasNames[*aliasCounter]);
	}
	fclose(fp);
	
	char WFileAdd[1000];
	strcpy(WFileAdd, root);
	strcat(WFileAdd, "\\.neogit\\Configuration_files\\local_alias");
	fp = fopen(WFileAdd, "r");
	if (!fp)
	{
		printf("local alias file not found\n");
		return;
	}
	fscanf(fp, "%[^\n]s", aliasNames[*aliasCounter]);
	while(!feof(fp))
	{
		fscanf(fp, "\n%[^\n]s", aliasCommands[*aliasCounter]);
		(*aliasCounter)++;
		fscanf(fp, "\n%[^\n]s", aliasNames[*aliasCounter]);
	}
	fclose(fp);
}

int check_alias(int argc, char * argv[], char aliasNames[100][100], char aliasCommands[100][1000], int aliasCounter)
{
	int i;
	for(i = 0; i < aliasCounter; i++)
	{
		if (!strcmp(duplicate_argv[1], aliasNames[i]))
		{
			strcpy(user_alias_command, aliasCommands[i]);
			return i;
		}
	}
	return -1;
}

int is_alias(int argc, char * argv[])
{
	char aliasNames[100][100];
	char aliasCommands[100][1000];
	int aliasCounter = 0;
	load_alias(aliasNames, aliasCommands, &aliasCounter);
	if (check_alias(argc, argv, aliasNames, aliasCommands, aliasCounter) != -1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void Parser(int argc, char * argv[])
{
	if (!strcmp(argv[1], "init"))
	{
		init(argc);
		return;
	}
	if (!strcmp(argv[1], "config"))
	{
		config(argc, argv);
		return;
	}
	if (!strcmp(root, "Root Not Found"))
	{
		printf("no git initialized\n");
		return;
	}
	if (!strcmp(argv[1], "add"))
	{
		add(argc, argv);
		return;
	}
	if (!strcmp(argv[1], "reset"))
	{
		reset(argc, argv);
		return;
	}
	if (is_alias(argc, argv))
	{
		system(user_alias_command);
		return;
	}
	printf("Command Not Found\n");
	return;
}

void global_configuration()
{
	FILE * fp = fopen("c:\\git\\global_configuration", "r");
	if (!fp)
	{
		printf("can not open global configuration file\n");
		return;
	}
	strcpy(user_name_mode, "G");
	fscanf(fp, "%[^\n]s", user_name);
	strcpy(user_email_mode, "G");
	fscanf(fp, "\n%[^\n]s", user_email);
	fclose(fp);
}

void local_configuration()
{
	char Temp[256];
  	char TempDir[256];
 	strcpy(TempDir, root);
	strcat(TempDir, "\\.neogit");
	if (is_dir(TempDir))
	{
		strcat(TempDir, "\\Configuration_files\\local_configuration");
		FILE * fp = fopen(TempDir, "r");
		if (!fp)
		{
			printf("can not open local configuration file\n");
			return;
		}
		fscanf(fp, "%[^\n]s", user_name_mode);
		if (!strcmp(user_name_mode,"L"))
		{
			fscanf(fp, "\n%[^\n]s", user_name);
		}
		else
		{
			fscanf(fp, "\n%[^\n]s", Temp);
		}
		fscanf(fp, "\n%[^\n]s", user_email_mode);
		if (!strcmp(user_email_mode,"L"))
		{
			fscanf(fp, "\n%[^\n]s", user_email);
		}
		else
		{
			fscanf(fp, "\n%[^\n]s", Temp);
		}
		fclose(fp);
	}
}

void lower_case(int argc, char *argv[])
{
	int i,j;
	for(i = 0; i < argc; i++)
	{
		strcpy(duplicate_argv[i], argv[i]);
	}
	for(i = 1; i < argc; i++)
	{
		for(j = 0; j < strlen(argv[i]); j++)
		{
			if (argv[i][j] >= 'A' && argv[i][j] <= 'Z')
				argv[i][j] = tolower(argv[i][j]);
		}
	}
}

void finding_git_root()
{
	if (getcwd(cwd, sizeof(cwd)) == NULL) {
    	printf("can not get current working directory\n");
    	return;
  	}
	char TempDir[256];
  	char TempAdd[256];
   	strcpy(TempDir, cwd);
	strcat(TempDir, "\\.neogit");
	if (is_dir(TempDir))
	{
		strcpy(root, cwd);
	   	return;
	}
	
  	strcpy(TempDir, cwd);
  	int Pos = strlen(TempDir) - 1;
  	while(Pos >= 0)
  	{
  		if (TempDir[Pos] != '\\')
		{
			TempDir[Pos] = '\0';
			Pos--;
		}
		else
		{
			strcpy(TempAdd, TempDir);
			strcat(TempAdd, ".neogit");
			if (is_dir(TempAdd))
			{
				TempDir[Pos] = '\0';
				strcpy(root, TempDir);
		       	return;	
			}
			Pos--;
		}
	}
	strcpy(root, "Root Not Found");
	return;
}

void load_legal_commands()
{
	FILE * fp = fopen("c:\\git\\legal_commands", "r");
	int Pos = 0;
	fscanf(fp, "%s", legal_commands[Pos++]);
	while(!feof(fp))
	{
		fscanf(fp, "%s", legal_commands[Pos++]);
	}	
	fclose(fp);
}

void main(int argc, char * argv[])
{
	load_legal_commands();
	global_configuration();
	finding_git_root();
	local_configuration();
	lower_case(argc, argv);
	Parser(argc, argv);
}
