/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
\file         helperusage.h
\date         February 2017
\author       Nicu Tofan

\brief        Base clss for tests.

*//*

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#ifndef HELPER_USAGE_HEADER
#define HELPER_USAGE_HEADER
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

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

//! Test that creates an in-memory database.
class TestUsage : public ::testing::Test {
    //
    //
    //
    //
    /*  DEFINITIONS    ----------------------------------------------------- */

    /*  DEFINITIONS    ===================================================== */
    //
    //
    //
    //
    /*  DATA    ------------------------------------------------------------ */

public:
    sqlite3 *db;

    /*  DATA    ============================================================ */
    //
    //
    //
    //
    /*  FUNCTIONS    ------------------------------------------------------- */

public:

    //! Constructor
    TestUsage ();

    //! Destructor.
    virtual ~TestUsage ();

    //! Prepares the environment.
    void
    SetUp ();

    //! Terminates the environment.
    void
    TearDown();

    //! Prepare a statement.
    sqlite3_stmt *
    prepare (
            const char * statem = NULL);

    //! Execute a command.
    void
    execute (
            const char * statem = NULL);

    //! Execute a command.
    void
    execute_r (
            const char * statem = NULL);

    //! Execute a command that returns an integer.
    int
    execute_get_int (
            const char * statem = NULL);

    //! Get the number of records in a table.
    int
    execute_get_count (
            const char * table);

    //! Execute a command that returns a string.
    QString
    execute_get_str (
            const char * statem = NULL);

    //! Prints all the rows in a table.
    void
    printAll (
            const char * table);

    //! A 512 bytes buffer to use all around.
    static char *
    locbuf ();

    /*  FUNCTIONS    ======================================================= */
    //
    //
    //
    //

}; // class TestUsage

/*  CLASS    =============================================================== */
//
//
//
//

/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#endif // HELPER_USAGE_HEADER
