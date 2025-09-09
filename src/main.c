#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[]) {
    printf("Usage: %s <options> :<filename>\n", argv[0]);
    printf("Available options:\n");
    printf("\t -n: Create a new database\n");
    printf("\t -f: Open a database\n");
    printf("\t -?: Print this dialog\n");
    return;
}

int main(int argc, char *argv[]) {
    int option;
    int dbfd = -1;
    char *filepath = NULL;
    bool new_file = false;
    struct dbheader_t *dbheader = {0};
    struct employee_t *employees = {0};

    if(argc < 2) {
        print_usage(argv);
        return STATUS_ERROR;
    }

    while ((option = getopt(argc, argv, "nef:")) != -1) {
        switch (option) {
            case 'n':
                new_file = true;
                break;
            case 'e':
                return 0;
                break;
            case 'a':
                addstring = optarg;
                break;
            case 'f':
                filepath = optarg;
                break;
            case '?':
                print_usage(argv);
                break;
            default:
                return -1;
        }
    }
    if(new_file == true) {
        dbfd = create_db_file(filepath);
        if(dbfd == STATUS_ERROR) {
            printf("Database could not be created.\n");
            return STATUS_ERROR;
        }
        if(create_db_header(&dbheader) != STATUS_SUCCESS) {
            printf("Database header could not be created.\n");
            return STATUS_ERROR;
        }
    } else {
        dbfd = open_db_file(filepath);
        if(dbfd == STATUS_ERROR) {
            printf("Database could not be opened.\n");
            return STATUS_ERROR;
        }
        if(validate_db_header(dbfd, &dbheader) != STATUS_SUCCESS) {
            printf("failed to validate database header\n");
            return STATUS_ERROR;
        }
    }
    if(read_employees(dbfd, dbheader, &employees) != STATUS_SUCCESS) {
        printf("failed to read employees");
        return STATUS_ERROR;
    }
    output_file(dbfd, dbheader, &employee);
}
