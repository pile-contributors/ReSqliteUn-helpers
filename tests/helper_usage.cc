/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
\file         helper_usage.cc
\date         February 2017
\author       Nicu Tofan

\brief        Base clss for tests.

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
#define _CRT_SECURE_NO_WARNINGS

#include "helper_usage.h"
#include "print_helpers.h"

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! A 512 bytes buffer to use all around.
char buffloc[512];

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
TestUsage::TestUsage () :
    db (NULL)
{}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TestUsage::~TestUsage ()
{}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void TestUsage::SetUp ()
{
    for (;;) {
        sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);

        ReSqliteUn::autoregister ();

        int rc = sqlite3_open(":memory:", &db);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Cannot open database: %s\n",
                    sqlite3_errmsg(db));
        }
        ASSERT_EQ(rc, SQLITE_OK);

        break;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
sqlite3_stmt *TestUsage::prepare (const char *statem)
{
    if (statem == NULL) {
        statem = locbuf ();
    }
    sqlite3_stmt *stmt;
    int rc;
    rc = sqlite3_prepare_v2 (db, statem, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
    }
    EXPECT_TRUE(rc == SQLITE_OK);
    EXPECT_TRUE(stmt != NULL);
    return stmt;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void TestUsage::execute (const char *statem)
{
    if (statem == NULL) {
        statem = locbuf ();
    }
    sqlite3_stmt *stmt = prepare (statem);
    printStatement (statem);

    int rc = sqlite3_step (stmt);
    ASSERT_EQ(rc, SQLITE_DONE);

    rc = sqlite3_finalize (stmt);
    ASSERT_EQ(rc, SQLITE_OK);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void TestUsage::execute_r (const char *statem)
{
    if (statem == NULL) {
        statem = locbuf ();
    }
    sqlite3_stmt *stmt = prepare (statem);
    printStatement (statem);

    int rc = sqlite3_step (stmt);
    if (rc == SQLITE_ERROR) {
        fprintf(stderr, "Error in execute_r: %s\n", sqlite3_errmsg (db));
    }
    ASSERT_EQ(rc, SQLITE_ROW);

    PrintAllData pad;
    pad.print_result (stmt);

    rc = sqlite3_finalize (stmt);
    ASSERT_EQ(rc, SQLITE_OK);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
int TestUsage::execute_get_int (const char *statem)
{
    if (statem == NULL) {
        statem = locbuf ();
    }
    sqlite3_stmt *stmt = prepare (statem);
    printStatement (statem);

    int rc = sqlite3_step (stmt);
    if (rc == SQLITE_ERROR) {
        fprintf(stderr, "Error in execute_get_int: %s\n", sqlite3_errmsg (db));
    }
    EXPECT_EQ(rc, SQLITE_ROW);

    int result = sqlite3_column_int(stmt, 0);
    rc = sqlite3_step (stmt);
    EXPECT_EQ(rc, SQLITE_DONE);

    rc = sqlite3_finalize (stmt);
    EXPECT_EQ(rc, SQLITE_OK);

    return result;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
int TestUsage::execute_get_count (const char *table)
{
    sprintf(locbuf (), "SELECT COUNT(*) FROM %s;\n", table);
    printStatement (locbuf ());

    return execute_get_int ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QString TestUsage::execute_get_str (const char *statem)
{
    if (statem == NULL) {
        statem = locbuf ();
    }
    sqlite3_stmt *stmt = prepare (statem);
    printStatement (statem);

    int rc = sqlite3_step (stmt);
    if (rc == SQLITE_ERROR) {
        fprintf(stderr, "Error in execute_get_str: %s\n", sqlite3_errmsg (db));
    }
    EXPECT_EQ(rc, SQLITE_ROW);

    QString result = ReSqliteUn::columnText (stmt, 0);
    rc = sqlite3_step (stmt);
    EXPECT_EQ(rc, SQLITE_DONE);

    rc = sqlite3_finalize (stmt);
    EXPECT_EQ(rc, SQLITE_OK);

    return result;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void TestUsage::printAll (const char *table)
{
    PrintAllData pad;
    sprintf(locbuf (), "SELECT * FROM %s;\n", table);
    printStatement (locbuf ());

    char *errmsg;
    /*int rc = */sqlite3_exec(
                db, locbuf (), print_all_callback,
                static_cast<void*>(&pad), &errmsg);
    pad.end ();
    if (errmsg != NULL) {
        fprintf(stderr, "Error in sqlite3_exec: %s\n", errmsg);
        sqlite3_free(errmsg);
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
char * TestUsage::locbuf ()
{
    return &buffloc[0];
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void TestUsage::TearDown()
{
    sqlite3_close (db);
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//

/* ------------------------------------------------------------------------- */
/* ========================================================================= */
