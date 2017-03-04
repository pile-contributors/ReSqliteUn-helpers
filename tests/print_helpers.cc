/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
\file         printhelpers.cc
\date         February 2017
\author       Nicu Tofan

\brief        Helpers for printing stuff.

*//*

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "print_helpers.h"

#include <gtest/gtest.h>
#include <resqliteun/resqliteun.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

const char * PrintAllData::line =
        "\n------------------------------------------------------------"
        "-----------------------------------------------------------\n";

const char * PrintAllData::line2 =
        "============================================================"
        "===========================================================\n\n\n";


/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
void printStatement (const char *value)
{
#  ifdef PRINT_STATEMENTS
    fprintf(stderr, "%s\n", value);
#  endif // PRINT_TABLES
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
PrintAllData::PrintAllData() :
    row_number (0),
    columns(0)
{}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void PrintAllData::end()
{
#   ifdef PRINT_TABLES
    printf(line+1);
    printf("      Total: %i rows, %i columns\n", row_number, columns);
    printf(line2);
#   endif // PRINT_TABLES
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
int PrintAllData::print_record(int argc, char **argv, char **colName)
{
#   ifdef PRINT_TABLES
    int i;
    if (row_number == 0) {
        columns = argc;
        printf(line);
        printf("|  Row  |");
        for (i=0; i<argc; ++i) {
            printf("%20s |", colName[i]);
        }
        printf(line);
    }

    printf("| %5i |", row_number+1);

    for (i=0; i<argc; i++) {
        printf("%20s |", argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    ++row_number;
#   endif // PRINT_TABLES
    return 0;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void PrintAllData::print_result(sqlite3_stmt *stmt)
{
#   ifdef PRINT_TABLES
    if (columns == 0) {
        columns = sqlite3_column_count (stmt);
    }
    int i;
    if (row_number == 0) {
        printf(line);
        printf("|  Row  |");
        for (i=0; i<columns; ++i) {
            printf("%20s |", sqlite3_column_name(stmt, i));
        }
        printf(line);
    }

    // We should only reach this point if a previous
    // sqlite3_step succeded.
    int retval = SQLITE_ROW;
    for (;;) {
        if (retval == SQLITE_ROW) {
            ++row_number;
            printf("| %5i |", row_number);
            for (i=0; i<columns; i++) {
                printf("%20s |", sqlite3_column_text(stmt, i));
            }
            printf("\n");
        } else if (retval == SQLITE_DONE) {
            break;
        } else {
            printf("Some error\n");
            break;
        }
        // fetch a row's status
        retval = sqlite3_step (stmt);
    }

    end ();
#   endif // PRINT_TABLES
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
extern "C" {
    int print_all_callback (void *arg, int argc, char **argv, char **colName)
    {
        PrintAllData * pad = static_cast<PrintAllData *>(arg);
        return pad->print_record(argc, argv, colName);
    }
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//

/* ------------------------------------------------------------------------- */
/* ========================================================================= */
