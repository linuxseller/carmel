#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#define CHECKFD(fd, filename) { \
    if(fd==-1){ \
        printf("[ERROR] opening %s\n", filename); \
        printf("[ERROR] %s\n", strerror(errno)); \
        exit(1); \
    } \
}
#define WRITEERR(fd, str) { \
    if(write(fd, str, strlen(str))==-1){ \
        printf("[ERROR] %s\n", strerror(errno)); \
        exit(1); \
    } \
}

char *maintemplate = "#include <stdio.h>\n#include <stdlib.h>\n#include <errno.h>\n#include <string.h>\n\nint main(int argc, char **argv){\n    (void) argc;\n    (void) argv;\n    printf(\"Hello World!\\n\");\n    return 0;\n}\n";

char *maketemplatef = "CFLAGS=-Wall -Wextra -Werror -pedantic\n\
CLIBS=-L. -I.\n\
CC= %s\n\
OUTFILE=%s\n\
all: compile\n\
compile: \n\
\tmkdir -p build\n\
\t$(CC) $(CFLAGS) src/main.c -o build/$(OUTFILE) $(CLIBS)\n\
run: compile\n\
\t./$(OUTFILE)\n\
clean:\n\
\trm -v build/*\n";

struct {
    char *name;
    char *text;
    
} CLFile;

char *argshift(int *argc, char ***argv){
    *argc-=1;
    *argv+=1;
    return *(*argv-1);
}

void usage(char *program){
    printf("Usage: %s init [OPTIONS] directory\n", program);
    printf("\t-h prints this help\n");
    printf("\t-c provide compiler (cc default)\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv){
    int dirfd;
    char *compiler = "cc";
    char *prj_dir = NULL;
    char *program = argshift(&argc, &argv);
    if(argc==0){
        usage(program); 
    }
    char *init = argshift(&argc, &argv);
    if(strcmp("init", init)!=0){
        usage(program); 
    }
    while(argc>0){
        char *command = argshift(&argc, &argv);
        if(strcmp(command, "-c")==0){
            compiler = argshift(&argc, &argv);
        } else if(strcmp(command, "-h")==0){
            usage(program);
        } else if (argc!=1){
            printf("[WARN] Unknown command %s\n", command);
            argv-=1;
        }
    };
    prj_dir = argshift(&argc, &argv);

    printf("[INFO] Compiler: %10s, Dir: %10s\n", compiler, (prj_dir==NULL)?"./":prj_dir);
    printf("[INFO] ---------------------------\n");

    if(prj_dir==NULL){
        dirfd = AT_FDCWD;
    } else {
        dirfd = open(prj_dir, O_RDONLY);
        if(dirfd==-1){
            mkdirat(AT_FDCWD, prj_dir, 0755);
        }
    }
    mkdirat(dirfd, "src", 0755);
    printf("[INFO] Created %s\n", "src/");
    mkdirat(dirfd, "src/libs", 0755);
    printf("[INFO] Created %s\n", "src/libs/");
    mkdirat(dirfd, "build", 0755);
    printf("[INFO] Created %s\n", "build/");

    char *file_name;
    file_name = "src/main.c";
    int mainfile = openat(dirfd, file_name, O_CREAT|O_RDWR, S_IRWXU|S_IRGRP);
    CHECKFD(mainfile, file_name);
    printf("[INFO] Created %s\n", file_name);

    file_name = "Makefile";
    int makefile = openat(dirfd, file_name, O_CREAT|O_RDWR, S_IRWXU|S_IRGRP);
    CHECKFD(mainfile, file_name);
    printf("[INFO] Created %s\n", file_name);


    WRITEERR(mainfile, maintemplate); 
    printf("[INFO] Wrote main.c\n");
    char *makefiletext=malloc((strlen(maketemplatef)+strlen(prj_dir)+strlen(compiler))*sizeof(uint8_t));
    sprintf(makefiletext, maketemplatef, compiler, prj_dir);
    WRITEERR(makefile, makefiletext);
    printf("[INFO] Wrote Makefile\n");

    close(makefile);
    close(mainfile);
    return 0;
}
