#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys\stat.h>

#include "FileWork.h"
#include "../Strings/_Strings.h"

/*!
* \file
* \brief 
* This file contains functions to file work
*/

//!--------------------
//!Read text from file pointer fp to text 
//!\return Number of lines in text
//!
//!--------------------
int gettext(char *text, size_t size,  FILE *fp)
{
    assert(text != NULL);
    assert(fp != NULL);

    fread(text, sizeof(char), size, fp);

    int numberLines = 1;
    int counter = 0;

    while (text[counter] != '\0')    
    {
        if (text[counter] == '\n')
            numberLines++;

        counter++;
    }
    
    return numberLines;
}

//!--------------------------
//!Function splits text from text to lines and write pointers to start lines in lines array
//!--------------------------
void getlines_from_text(const char **lines, char *text)
{
    assert(lines  != nullptr);
    assert(text   != nullptr);

    int   n                = 0;
    int   line_num         = 0;
    bool  next_is_new_line = false;

    lines[line_num] = &text[n];

    while (text[n] != '\0')
    {
        if (next_is_new_line)
        {
            next_is_new_line = false;
            lines[line_num] = &text[n];
        }

        if (text[n] == '\n')
        {
            text[n] = '\0';
            next_is_new_line = true;
            line_num++;
        }
        n++;
    }
}

//!----------------
//!Puts text to file.
//!
//!Function puts N sympoles and replaces \0 to \n and writes N symbols.
//!\param [in] text Text to put in file
//!\param [in] fp   File pointer to file in which text will be printed
//!\param [in] N   
//!
//!----------------
void put_text_to_file(const char *text, FILE *fp, size_t N)
{    
    assert(text != nullptr);
    assert(fp   != nullptr);

    for(int i = 0; i < N; i++)
    {
        char c = text[i];
        if (c == '\0')
            c = '\n';
        putc(c, fp);
    }
}

//!-----------------------------------------------------
//! Function to measure file size in bytes
//! \param  [in] file_name Name of file whose size you need to know
//! \return File size in bytes
//!
//! ----------------------------------------------------
int get_text_size(const char *file_name)
{
    assert(file_name != nullptr);

    struct stat buff = {};
    stat(file_name, &buff);
    return buff.st_size;
}