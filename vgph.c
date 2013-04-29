/*
 * Copyright (c) 2013, Marcus Carlsson <carlsson.marcus@gmail.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of the author nor the names of other
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE 
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <sys/ioctl.h>

struct dataset {
    double value;
    char *name;
};

int main(int argc, char *argv[]) {

    /* Print help */
    if (argc == 1 || !strcmp("-h", argv[1]) || !strcmp("--help", argv[1])) {
        fprintf(stderr, "usage: %s [name:]value [...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct dataset *data;
    int i, length=0;
    double min=DBL_MAX, max=DBL_MIN;

    data = malloc(sizeof(*data) * (argc - 1));
    for (i = 0; i < argc - 1; i++) {
        // Check if ':' is in the argument
        if (strchr(argv[i + 1], ':')) {
            // Split the string and save the first part as the name of the
            // value.
            data[i].name = strtok(argv[i + 1], ":");
            data[i].value = atof(strtok(NULL, ":"));
        } else {
            data[i].name = NULL;
            data[i].value = atof(argv[i + 1]);
        }
        // Find min/max values
        if (data[i].value > max)
            max = data[i].value;
        if (data[i].value < min)
            min = data[i].value;
        // Update longest name string
        if (data[i].name != NULL && strlen(data[i].name) > length)
            length = strlen(data[i].name);
    }

    struct winsize w;
    // Get the size of the terminal into the struct 'w'.
    ioctl(0, TIOCGWINSZ, &w);

    // Remove 2 chars for spacing []
    int c, graph, columns = w.ws_col - 2;
    double difference = max - min;
    if (length > 0)
        columns -= length + 1;

    // Print the graphs
    for (i = 0; i < argc - 1; i++) {
        if (length > 0)
            printf("%*s [", length, data[i].name == NULL ? "" : data[i].name);
        else
            fputs("[", stdout);
        // All graphs are of the same size, set the length to be full instead
        // of 0.
        if (difference == 0)
            graph = columns;
        else
            graph = ((data[i].value - min) / difference) * columns;
        for (c = 0; c < graph; c++)
            fputs("#", stdout);
        puts("]");
    }
    free(data);
    return EXIT_SUCCESS;
}
