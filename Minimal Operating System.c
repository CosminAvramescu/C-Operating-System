#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir
{
	char *name;
	struct Dir *parent;
	struct File *head_children_files;
	struct Dir *head_children_dirs;
	struct Dir *next;
} Dir;

typedef struct File
{
	char *name;
	struct Dir *parent;
	struct File *next;
} File;

void stop(Dir *target);

void touch(Dir *parent, char *name)
{
	File *p = NULL;

	//daca exista fisiere in directorul curent
	if (parent->head_children_files)
	{
		p = parent->head_children_files;
		//daca exista deja numele noului fisier in primul director
		if (strcmp(p->name, name) == 0)
		{
			printf("File already exists\n");
			return;
		}
		//se parcurg si celelalte fisiere pentru verificarea numelui
		while (p->next)
		{
			if (strcmp(p->name, name) == 0)
			{
				printf("File already exists\n");
				return;
			}
			p = p->next;
		}
	}

	//se creeaza noul fisier, se aloca memorie, se initializeaza pointerii
	File *newFile = malloc(sizeof(File));
	newFile->next = NULL;
	newFile->parent = parent;
	newFile->name = malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(newFile->name, name);

	//se refac legaturile din lista dupa adaugarea noului fisier
	if (parent->head_children_files)
		p->next = newFile;
	else
		parent->head_children_files = newFile;
}

void mkdir(Dir *parent, char *name)
{
	Dir *pd = NULL;

	//daca exista directoare in directorul curent
	if (parent->head_children_dirs)
	{
		pd = parent->head_children_dirs;
		//daca exista deja numele noului director in primul director
		if (strcmp(pd->name, name) == 0)
		{
			printf("Directory already exists\n");
			return;
		}
		//se parcurg si celelalte fisiere pentru verificarea numelui
		while (pd->next)
		{
			if (strcmp(pd->name, name) == 0)
			{
				printf("Directory already exists\n");
				return;
			}
			pd = pd->next;
		}
	}

	//se creeaza noul director, se aloca memorie, se initializeaza pointerii
	Dir *newDir = malloc(sizeof(Dir));
	newDir->next = NULL;
	newDir->parent = parent;
	newDir->head_children_dirs = NULL;
	newDir->head_children_files = NULL;
	newDir->name = malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(newDir->name, name);

	//se refac legaturile din lista dupa adaugarea noului director
	if (parent->head_children_dirs)
		pd->next = newDir;
	else
		parent->head_children_dirs = newDir;
}

void ls(Dir *parent)
{
	//se parcurg toate directoarele
	Dir *pd = parent->head_children_dirs;
	while (pd)
	{
		printf("%s\n", pd->name);
		pd = pd->next;
	}
	//se parcurg toate fisierele
	File *pf = parent->head_children_files;
	while (pf)
	{
		printf("%s\n", pf->name);
		pf = pf->next;
	}
}

void rm(Dir *parent, char *name)
{
	int gasit = 0;
	File *p = parent->head_children_files, *q = p;

	while (p)
	{
		//daca se gaseste fisierul ce trebuie sters
		if (strcmp(p->name, name) == 0)
		{
			gasit = 1;
			break;
		}
		q = p;
		p = p->next;
	}

	if (gasit)
	{
		//daca fisierul sters e head-ul listei
		if (p == parent->head_children_files)
		{
			//se refac legaturile si se elibereaza memoria
			parent->head_children_files = p->next;
			free(p->name);
			free(p);
		}
		else
		{
			q->next = p->next;
			free(p->name);
			free(p);
		}
	}
	else
		printf("Could not find the file\n");
}

void rmdir(Dir *parent, char *name)
{
	int gasit = 0;
	Dir *p = parent->head_children_dirs, *q = p;

	while (p)
	{
		//daca se gaseste directorul ce trebuie sters
		if (strcmp(p->name, name) == 0)
		{
			gasit = 1;
			break;
		}
		q = p;
		p = p->next;
	}

	if (gasit)
	{
		if(p->head_children_dirs)
			stop(p);
		else
		{
			File *f = p->head_children_files, *del = NULL;
			//se sterg toate fisierele din directorul curent
			while (f)
			{
				del = f;
				f = f->next;
				free(del->name);
				free(del);
			}
			//daca directorul ce trebuie sters este head-ul listei
			//se elibereaza memoria si se refac legaturile listei
			if (p == parent->head_children_dirs)
			{
				parent->head_children_dirs = p->next;
				free(p->name);
				free(p);
			}
			else
			{
				q->next = p->next;
				free(p->name);
				free(p);
			}
		}
	}
	else
		printf("Could not find the dir\n");
}

void cd(Dir **target, char *name)
{
	Dir *initial = *target;
	int gasit = 0;

	//daca se merge spre directorul parinte
	if (strcmp(name, "..") == 0)
	{
		if ((*target)->parent != NULL)
			(*target) = (*target)->parent;
		return;
	}

	//daca exista directoare in directorul curent, se asaza targetul pe primul din lista
	if ((*target)->head_children_dirs)
		(*target) = (*target)->head_children_dirs;
	else
		return;

	//cat timp nu s-a terminat lista de directoare
	while ((*target))
	{
		//daca s-a gasit directorul in care dorim sa ne mutam
		if (strcmp(name, (*target)->name) == 0)
		{
			gasit = 1;
			break;
		}
		(*target) = (*target)->next;
	}

	//daca nu s-a gasit directorul dorit, target revine la pozitia initiala
	if (!gasit)
	{
		printf("No directories found!\n");
		(*target) = initial;
	}
}

char *pwd(Dir *target)
{
	Dir *p = target;
	static char s[MAX_INPUT_LINE_SIZE + 1];

	if (target == NULL)
		return "";

	//implementare recursiva pentru a afisa calea
	pwd(p->parent);
	//se concateneaza " \n"aratorul "/"
	strcat(s, "/");
	//se concateneaza numele fisierului
	return strcat(s, p->name);
}

void stop(Dir *target)
{
	//conditia de oprire
	if (!target)
		return;

	Dir *d = target, *del_dir = NULL;
	while (d)
	{
		//pentru fiecare director
		del_dir = d;
		//se sterge de la adancimea maxima inspre adancimea minima
		//pentru a nu strica legaturile dupa stergeri
		stop(d->head_children_dirs);
		File *f = d->head_children_files, *del_file = NULL;
		//se sterge tot ceea ce contine directorul
		while (f)
		{
			del_file = f;
			f = f->next;
			free(del_file->name);
			free(del_file);
		}
		//se sterge directorul curent si se trece la urmatorul director
		d = d->next;
		free(del_dir->name);
		free(del_dir);
	}
}

void tree(Dir *target, int level)
{
	int nr;

	//conditia de oprire
	if (!target)
		return;

	Dir *d = target;
	while (d)
	{
		//afisarea cu spatii
		nr = level;
		while (nr)
		{
			printf("    ");
			nr--;
		}
		printf("%s\n", d->name);
		//se afiseaza intai directoarele
		//apoi se apeleaza functia recursiv si se incrementeaza nivelul
		tree(d->head_children_dirs, ++level);
		File *f = d->head_children_files;
		//apoi se afiseaza tot ce contine acel director
		while (f)
		{
			//afisare cu spatii
			nr = level;
			while (nr)
			{
				printf("    ");
				nr--;
			}
			printf("%s\n", f->name);
			f = f->next;
		}
		//se trece la urmatorul director si se revine la nivelul acestuia
		d = d->next;
		--level;
	}
}

void mv(Dir *parent, char *oldname, char *newname)
{
	Dir *d = parent->head_children_dirs, *precedent = NULL, *dir_address = NULL;
	File *f = parent->head_children_files, *file_address = NULL;
	int gasit = 0;

	//se cauta daca exista directorul ce trebuie modificat
	//se cauta daca noul nume nu exista deja
	while (d)
	{
		if (strcmp(d->name, oldname) == 0)
		{
			dir_address = d;
			gasit = 1;
		}
		if (strcmp(d->name, newname) == 0)
		{
			printf("File/Director already exists\n");
			return;
		}
		//se retine directorul precedent, pentru a reface legaturile dupa stergere
		//dupa ce se gaseste directorul, loop-ul continua pentru a verifica newname
		//deci precedent trebuie modificat doar cand gasit==0
		if (!gasit)
			precedent = d;
		d = d->next;
	}

	//la fel, se cauta si in fisiere incompatibilitatile
	while (f)
	{
		if (strcmp(f->name, oldname) == 0)
		{
			file_address = f;
			gasit = 1;
		}
		if (strcmp(f->name, newname) == 0)
		{
			printf("File/Director already exists\n");
			return;
		}
		f = f->next;
	}
	if (!gasit)
	{
		printf("File/Director not found\n");
		return;
	}
	else
	{
		if (dir_address)
		{
			//se creeaza noul director, care preia toate valorile pointerilor vechiului director
			Dir *newDir = malloc(sizeof(Dir)), *p = dir_address;
			newDir->name = malloc(sizeof(char) * (strlen(newname) + 1));
			newDir->head_children_dirs = dir_address->head_children_dirs;
			newDir->head_children_files = dir_address->head_children_files;
			newDir->parent = dir_address->parent;
			newDir->next = NULL;
			strcpy(newDir->name, newname);
			while (p->next)
				p = p->next;
			p->next = newDir;
			//se refac legaturile inainte de stergere
			if (precedent == NULL)
				parent->head_children_dirs = dir_address->next;
			else
				precedent->next = dir_address->next;
			free(dir_address->name);
			free(dir_address);
		}
		//se sterge fisierul
		if (file_address)
		{
			rm(parent, oldname);
			touch(parent, newname);
		}
	}
}

int main()
{
	//se initializeaza directorul "home"
	Dir *parent = calloc(1, sizeof(Dir));
	parent->head_children_dirs = NULL;
	parent->head_children_files = NULL;
	parent->next = parent->parent = NULL;
	parent->name = malloc(sizeof(char) * (sizeof(strlen("home") + 1)));
	strcpy(parent->name, "home");

	//in s se va citi cate o linie din stdin
	char *s = calloc(MAX_INPUT_LINE_SIZE + 1, sizeof(char));
	char *words = NULL, *cuv = NULL, *new_name = NULL;
	do
	{
		//se face citirea
		fgets(s, MAX_INPUT_LINE_SIZE + 1, stdin);
		//se extrage primul token
		words = strtok(s, " \n");
		cuv = malloc(sizeof(char) * (strlen(words) + 1));
		strcpy(cuv, words);
		//se implementeaza fiecare comanda printr-un switch, mai elegant
		//fiecare case trateaza cazul in care comanda a fost scrisa corect
		//in caz contrar doar se elibereaza memoria, fie prin ultima intructiune din case, fie prin default
		switch (s[0])
		{
		case 's':
			if (strcmp(cuv, "stop") == 0)
			{
				//se revine la director "home"
				while (parent->parent)
					parent = parent->parent;
				stop(parent);
			}
			free(cuv);
			break;
		case 'l':
			if (strcmp(cuv, "ls") == 0)
				ls(parent);
			free(cuv);
			break;
		case 't':
			if (strcmp(cuv, "tree") == 0)
			{
				tree(parent->head_children_dirs, 0);
				File *f = parent->head_children_files;
				while (f)
				{
					printf("%s\n", f->name);
					f = f->next;
				}
			}
			if (strcmp(cuv, "touch") == 0)
			{
				free(cuv);
				//se face transferul de la handlerul de tokeni (words), la un string alocat dinamic
				words = strtok(0, " \n");
				cuv = malloc(sizeof(char) * (strlen(words) + 1));
				strcpy(cuv, words);
				touch(parent, cuv);
			}
			free(cuv);
			break;
		case 'm':
			if (strcmp(cuv, "mkdir") == 0)
			{
				free(cuv);
				words = strtok(0, " \n");
				cuv = malloc(sizeof(char) * (strlen(words) + 1));
				strcpy(cuv, words);
				mkdir(parent, cuv);
			}
			if (strcmp(cuv, "mv") == 0)
			{
				free(cuv);
				words = strtok(0, " \n");
				cuv = malloc(sizeof(char) * (strlen(words) + 1));
				strcpy(cuv, words);
				words = strtok(0, " \n");
				new_name = malloc(sizeof(char) * (strlen(words) + 1));
				strcpy(new_name, words);
				mv(parent, cuv, new_name);
				free(new_name);
			}
			free(cuv);
			break;
		case 'r':
			if (strcmp(cuv, "rmdir") == 0)
			{
				free(cuv);
				words = strtok(0, " \n");
				cuv = malloc(sizeof(char) * (strlen(words) + 1));
				strcpy(cuv, words);
				rmdir(parent, cuv);
			}
			if (strcmp(cuv, "rm") == 0)
			{
				free(cuv);
				words = strtok(0, " \n");
				cuv = malloc(sizeof(char) * (strlen(words) + 1));
				strcpy(cuv, words);
				rm(parent, cuv);
			}
			free(cuv);
			break;
		case 'p':
			if (strcmp(cuv, "pwd") == 0)
				printf("%s\n", pwd(parent));
			free(cuv);
			break;
		case 'c':
			if (strcmp(cuv, "cd") == 0)
			{
				free(cuv);
				words = strtok(0, " \n");
				cuv = malloc(sizeof(char) * (strlen(words) + 1));
				strcpy(cuv, words);
				cd(&parent, cuv);
			}
			free(cuv);
			break;
		default:
			free(cuv);
		}

	} while (strcmp(s, "stop"));
	free(s);

	return 0;
}
