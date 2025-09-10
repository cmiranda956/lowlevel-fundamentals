#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "parse.h"

/*****************************************
 * NOTES:
 *
 * Memory endian-ess
 *  - host is readable by the program, and what we program to test against
 *  - network is how it is stored on disk
 *
 ******************************************/

void list_employees(struct dbheader_t *dbhdr, struct employee_t *employees) {
    for(int i = 0; i < dbhdr->count; i++) {
        printf("Employee %d\n", i);
        printf("\tName: %s\n", employees[i].name);
        printf("\tAddress: %s\n", employees[i].address);
        printf("\tHours: %d\n", employees[i].hours);
    }
}

int add_employee(struct dbheader_t *dbhdr, struct employee_t *employees, char *addstring) {
    if(addstring == NULL) {
        printf("missing string argument from user");
        return STATUS_ERROR;
    }
    if(dbhdr == NULL) {
        printf("null dbhdr pointer received from main");
        return STATUS_ERROR;
    }
    if(employees == NULL) {
        printf("null employees pointer received from main");
        return STATUS_ERROR;
    }
    char *name = strtok(addstring, ",");
    char *addr= strtok(NULL, ",");
    char *hours = strtok(NULL, ",");
    if(name == NULL || addr == NULL || hours == NULL) {
        printf("USAGE: Name,Address,Hours\n");
        return STATUS_ERROR;
    }

    strncpy(employees[dbhdr->count - 1].name, name, sizeof(employees[dbhdr->count - 1].name));
    strncpy(employees[dbhdr->count - 1].address, addr, sizeof(employees[dbhdr->count - 1].address));
    employees[dbhdr->count - 1].hours = atoi(hours);

    return STATUS_SUCCESS;
}

int read_employees(int fd, struct dbheader_t *dbhdr, struct employee_t **employeesOut) {
    if(fd < 0) {
        printf("bad FD from the user in read_employees");
        return STATUS_ERROR;
    }

    int count = dbhdr->count;

    struct employee_t *employees = calloc(count, sizeof(struct employee_t));

    if(employees == NULL) {
        printf("read_employees: calloc failed");
        return STATUS_ERROR;
    }
    
    read(fd, employees, (count * sizeof(struct employee_t)));

    for(int i = 0; i < count; i++) {
        employees[i].hours = ntohl(employees[i].hours);
    }

    *employeesOut = employees;

    return STATUS_SUCCESS;
}

int output_file(int fd, struct dbheader_t *dbhdr, struct employee_t *employees) {
    if(fd < 0) {
        printf("func: output_file:: bad file descriptor provided.");
        return STATUS_ERROR;
    }
    int realcount = dbhdr->count;

    dbhdr->version = htons(dbhdr->version);
    dbhdr->count = htons(dbhdr->count);
    dbhdr->magic = htonl(dbhdr->magic);
    dbhdr->filesize = htonl((sizeof(struct dbheader_t) + (sizeof(struct employee_t) * realcount)));

    lseek(fd, 0, SEEK_SET);
    write(fd, dbhdr, sizeof(struct dbheader_t));

    for(int i = 0; i < realcount; i++) {
        employees[i].hours = htonl(employees[i].hours);
        write(fd, &employees[i], sizeof(struct employee_t));
    }

    return STATUS_SUCCESS;
}

int validate_db_header(int fd, struct dbheader_t **headerOut) {
    if(fd < 0) {
        printf("bad file descriptor received from main");
        return STATUS_ERROR;
    }
    struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
    if(header == -1) {
        printf("calloc failed to create dbheader validator.\n");
        return STATUS_ERROR;
    }
    if(read(fd, header, sizeof(struct dbheader_t)) != sizeof(struct dbheader_t)) {
        perror("read");
        free(header);
        return STATUS_ERROR;
    }

    header->version = ntohs(header->version);
    header->count = ntohs(header->count);
    header->magic = ntohl(header->magic);
    header->filesize = ntohl(header->filesize);

    if(header->version != 1) {
        printf("improper header version\n");
        free(header);
        return STATUS_ERROR;
    }
    if(header->magic != HEADER_MAGIC) {
        printf("improper header magic value\n");
        free(header);
        return STATUS_ERROR;
    }

    struct stat dbstat = {0};
    fstat(fd, &dbstat);
    if(header->filesize != dbstat.st_size) {
        printf("corrupted database\n");
        free(header);
        return STATUS_ERROR;
    }
    *headerOut = header;

    return STATUS_SUCCESS;
}

// returns a status and not a value
int create_db_header(struct dbheader_t **headerOut) {
    if(headerOut == NULL) {
        printf("null pointer passed to create_db_header\n");
        return STATUS_ERROR;
    }
    struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
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
