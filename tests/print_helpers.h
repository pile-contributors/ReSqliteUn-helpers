/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
\file         printhelpers.h
\date         February 2017
\author       Nicu Tofan

\brief        Helpers for printing things to console.

*//*

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#ifndef PRINT_HELPERS_HEADER
#define PRINT_HELPERS_HEADER
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <gtest/gtest.h>
#include <resqliteun/resqliteun.h>

#include <stdio.h>
#include <sqlite/sqlite3.h>

//! General switch for table printing.
#define PRINT_TABLES

//! General switch for statement printing.
#define PRINT_STATEMENTS

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
//! Helper structure for printing entire tables
struct PrintAllData {

    int row_number;
    int columns;
    static const char * line;
    static const char * line2;

    //! Constructor.
    PrintAllData();

    //! Call at the end to print the statistics.
    void
    end ();

    //! Called on each record by the callback mechanism.
    int
    print_record (
            int argc,
            char **argv,
            char **colName);

    //! Prints everything in the result.
    void
    print_result (
            sqlite3_stmt *stmt);

}; // struct PrintAllData
/* ========================================================================= */


//! Prints sql statements.
void
printStatement (
        const char * value);

extern "C" {

//! Callback to be used for printing all rows of a table.
int
print_all_callback (
        void *arg,
        int argc,
        char **argv,
        char **colName);
}

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  TESTS    --------------------------------------------------------------- */


/*  TESTS    =============================================================== */
//
//
//
//

/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#endif // PRINT_HELPERS_HEADER
