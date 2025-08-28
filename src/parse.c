#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "parse.h"

void list_employees(struct dbheader_t *dbhdr, struct employee_t *employees) {}

int add_employee(struct dbheader_t *dbhdr, struct employee_t *employees,
        char *addstring) {}

int read_employees(int fd, struct dbheader_t *dbhdr,
        struct employee_t **employeesOut) {}

int output_file(int fd, struct dbheader_t *dbhdr,
        struct employee_t *employees) {}

int validate_db_header(int fd, struct dbheader_t **headerOut) {
    if(fd < 0) {
        printf("bad fild descriptor received from main");
        return STATUS_ERROR;
    }
    struct dbheader *header = calloc(1, sizeof(struct dbheader_t));
    if(header == -1) {
        printf("calloc failed to create dbheader validator.\n");
        return STATUS_ERROR;
    }
    // TODO : finish function
    read();
}

// returns a status and not a value
int create_db_header(int fd, struct dbheader_t **headerOut) {
    struct dbheader *header = calloc(1, sizeof(struct dbheader_t));
    if(header == -1) {
        printf("calloc failed to create dbheader.\n");
        return STATUS_ERROR;
    }

    header->magic = HEADER_MAGIC;
    header->version = 0x1;
    header->count = 0;
    header->filesize = sizeof(struct dbheader_t);

    *headerOut = header;

    return STATUS_SUCCESS;
}
