/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
\file         basic.cc
\date         February 2017
\author       Nicu Tofan

\brief        Basic tests

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

#include <gtest/gtest.h>
#include <resqliteun/resqliteun.h>

#include <sqlite/sqlite3.h>
#include <stdio.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

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
    TestUsage () :
        db (NULL)
    {}

    //! Destructor.
    virtual ~TestUsage ()
    {}

    //! Prepares the environment.
    void SetUp() {
        for (;;) {
            int rc = sqlite3_open(":memory:", &db);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "Cannot open database: %s\n",
                        sqlite3_errmsg(db));
            }
            ASSERT_EQ(rc, SQLITE_OK);

            break;
        }
    }

    //! Terminates the environment.
    void TearDown(){
        sqlite3_close (db);
    }

    /*  FUNCTIONS    ======================================================= */
    //
    //
    //
    //

}; // clss TestUsage

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  TESTS    --------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
TEST_F(TestUsage, sqlite_test) {
    sqlite3_stmt *res; int rc;
    rc = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
    }
    ASSERT_EQ(rc, SQLITE_OK);

    rc = sqlite3_step (res);
    if (rc == SQLITE_ROW) {
        printf("%s\n", sqlite3_column_text(res, 0));
    }
    ASSERT_EQ(rc, SQLITE_ROW);

    sqlite3_finalize(res);
}
/* ========================================================================= */

/*  TESTS    =============================================================== */
//
//
//
//

/* ------------------------------------------------------------------------- */
/* ========================================================================= */
