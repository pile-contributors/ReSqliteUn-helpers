/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
\file         sql_interface.cc
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
#define _CRT_SECURE_NO_WARNINGS

#include "name_overrides.h"
#include "print_helpers.h"
#include "helper_usage.h"
#include "helper_with_table.h"
#include "helper_multiple_usage.h"

#include <QString>


/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

/*
This is the example given at:
https://sourceforge.net/p/sqlite-undo/code/HEAD/tree/

> SELECT undoable_begin();
> INSERT INTO Test(data) VALUES('Hello');
> INSERT INTO Test(data) VALUES('Goodbye');
> SELECT undoable_end();
UNDO=1
REDO=0
> SELECT * FROM Test;
Hello
Goodbye
> SELECT undo();
UNDO=0
REDO=1
SQL=DELETE FROM test WHERE rowid=1;DELETE FROM test WHERE rowid=2
> SELECT * FROM Test;
> SELECT redo();
UNDO=1
REDO=0
SQL=INSERT INTO test(rowid,data) VALUES(1,'Hello');\
INSERT INTO test(rowid,data) VALUES(2,'Goodbye')
> SELECT * FROM Test;
Hello
Goodbye

*/

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  TESTS    --------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
TEST_F(TestUsage, sqlite_test) {

    sqlite3_stmt *res; int rc;
    rc = sqlite3_prepare_v2 (db, "SELECT SQLITE_VERSION();\n", -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
    }
    ASSERT_EQ(rc, SQLITE_OK);

    rc = sqlite3_step (res);
    ASSERT_EQ(rc, SQLITE_ROW);
    EXPECT_EQ(ReSqliteUn::columnText (res, 0), QString("3.16.2"));

    sqlite3_finalize(res);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST_F(TestWithTable, active_test) {
    EXPECT_EQ(execute_get_int("SELECT " RESQUN_FUN_ACTIVE "();"), 0);
    command_begin ();
    EXPECT_EQ(execute_get_int("SELECT " RESQUN_FUN_ACTIVE "();"), 1);
    command_end ();
    EXPECT_EQ(execute_get_int("SELECT " RESQUN_FUN_ACTIVE "();"), 0);
    command_begin ();
    EXPECT_EQ(execute_get_int("SELECT " RESQUN_FUN_ACTIVE "();"), 1);
    command_end ();
    EXPECT_EQ(execute_get_int("SELECT " RESQUN_FUN_ACTIVE "();"), 0);
    command_begin ();
    EXPECT_EQ(execute_get_int("SELECT " RESQUN_FUN_ACTIVE "();"), 1);
    command_end ();
    EXPECT_EQ(execute_get_int("SELECT " RESQUN_FUN_ACTIVE "();"), 0);

    attach_to_table("Test", 2);

    EXPECT_EQ(execute_get_int("SELECT " RESQUN_FUN_ACTIVE "();"), 0);
    command_begin ();
    EXPECT_EQ(execute_get_int("SELECT " RESQUN_FUN_ACTIVE "();"), 1);
    command_end ();
    EXPECT_EQ(execute_get_int("SELECT " RESQUN_FUN_ACTIVE "();"), 0);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST_F(TestWithTable, undo_redo) {
    execute_r("PRAGMA table_info(" RESQUN_TBL_IDX ")");
    execute_r("PRAGMA table_info(" RESQUN_TBL_TEMP ")");

    attach_to_table("Test", 2);

    EXPECT_EQ(execute_get_count("Test"), 0);

    command_begin ();

    execute ("INSERT INTO Test(data, data1) VALUES('Hello', 'World');\n");
    EXPECT_EQ(execute_get_count("Test"), 1);

    execute ("INSERT INTO Test(data, data1) VALUES('Goodbye', 'Sky');\n");
    EXPECT_EQ(execute_get_count("Test"), 2);

    command_end ();
    EXPECT_EQ(execute_get_count("Test"), 2);

    printAll("Test");
    printAll(RESQUN_TBL_IDX);
    printAll(RESQUN_TBL_TEMP);

    command_undo();
    EXPECT_EQ(execute_get_count("Test"), 0);

    printAll("Test");
    printAll(RESQUN_TBL_TEMP);

    command_redo();
    EXPECT_EQ(execute_get_count("Test"), 2);

    printAll("Test");
    printAll(RESQUN_TBL_TEMP);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST_F(TestWithTable, undo_redo_overwrite) {
    execute_r("PRAGMA table_info(" RESQUN_TBL_IDX ")");
    execute_r("PRAGMA table_info(" RESQUN_TBL_TEMP ")");

    attach_to_table("Test", 2);

    command_begin ();
    execute ("INSERT INTO Test(data, data1) VALUES('Hello', '1');\n");
    execute ("INSERT INTO Test(data, data1) VALUES('Goodbye', '1');\n");
    command_end ();

    command_begin ();
    execute ("INSERT INTO Test(data, data1) VALUES('Hello', '2');\n");
    execute ("INSERT INTO Test(data, data1) VALUES('Goodbye', '2');\n");
    command_end ();

    command_begin ();
    execute ("INSERT INTO Test(data, data1) VALUES('Hello', '3');\n");
    execute ("INSERT INTO Test(data, data1) VALUES('Goodbye', '3');\n");
    command_end ();

    EXPECT_EQ(execute_get_count("Test"), 6);
    EXPECT_EQ(execute_get_count(RESQUN_TBL_IDX), 3);
    EXPECT_EQ(execute_get_count(RESQUN_TBL_TEMP), 6);

    command_undo();

    EXPECT_EQ(execute_get_count("Test"), 4);
    EXPECT_EQ(execute_get_count(RESQUN_TBL_IDX), 3);
    EXPECT_EQ(execute_get_count(RESQUN_TBL_TEMP), 6);

    command_undo();

    EXPECT_EQ(execute_get_count("Test"), 2);
    EXPECT_EQ(execute_get_count(RESQUN_TBL_IDX), 3);
    EXPECT_EQ(execute_get_count(RESQUN_TBL_TEMP), 6);

    command_undo();

    EXPECT_EQ(execute_get_count("Test"), 0);
    EXPECT_EQ(execute_get_count(RESQUN_TBL_IDX), 3);
    EXPECT_EQ(execute_get_count(RESQUN_TBL_TEMP), 6);

    command_redo();

    EXPECT_EQ(execute_get_count("Test"), 2);
    EXPECT_EQ(execute_get_count(RESQUN_TBL_IDX), 3);
    EXPECT_EQ(execute_get_count(RESQUN_TBL_TEMP), 6);

    command_begin ();
    execute ("INSERT INTO Test(data, data1) VALUES('Hello', '4');\n");
    execute ("INSERT INTO Test(data, data1) VALUES('Goodbye', '4');\n");
    command_end ();

    EXPECT_EQ(execute_get_count("Test"), 4);
    EXPECT_EQ(execute_get_count(RESQUN_TBL_IDX), 2);
    EXPECT_EQ(execute_get_count(RESQUN_TBL_TEMP), 4);

    printAll("Test");
    printAll(RESQUN_TBL_IDX);
    printAll(RESQUN_TBL_TEMP);
}
/* ========================================================================= */

/*  TESTS    =============================================================== */
//
//
//
//

#if 0 //  some helpers

execute_r("PRAGMA table_info(Test)");

#endif
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
