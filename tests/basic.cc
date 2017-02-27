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
#define _CRT_SECURE_NO_WARNINGS

// These are the names as defined by the
// https://sourceforge.net/p/sqlite-undo/code/HEAD/tree/
#if 0
#   define RESQUN_FUN_TABLE    "undoable_table"
#   define RESQUN_FUN_ACTIVE   "undoable_active"
#   define RESQUN_FUN_BEGIN    "undoable_begin"
#   define RESQUN_FUN_END      "undoable_end"
#   define RESQUN_FUN_REDO     "redo"
#   define RESQUN_FUN_UNDO     "undo"
#   define RESQUN_TBL_TEMP     "_sqlite_undo"
#endif

#include <gtest/gtest.h>
#include <resqliteun/resqliteun.h>

#include <sqlite/sqlite3.h>
#include <stdio.h>

#include <QString>

char buffloc[512];
#define locbuf &buffloc[0]

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
    PrintAllData() :
        row_number (0),
        columns(0)
    {}

    //! Call at the end to print the statistics.
    void end () {
        printf(line+1);
        printf("      Total: %i rows, %i columns\n", row_number, columns);
        printf(line2);
    }

    //! Called on each record by the callback mechanism.
    int print_record (int argc, char **argv, char **colName)
    {
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
        return 0;
    }

    //! Prints everything in the result.
    void print_result (sqlite3_stmt *stmt)
    {
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
    }
}; // struct PrintAllData

const char * PrintAllData::line =
        "\n------------------------------------------------------------"
        "-----------------------------------------------------------\n";

const char * PrintAllData::line2 =
        "============================================================"
        "===========================================================\n\n\n";

extern "C" {
    int print_all_callback (void *arg, int argc, char **argv, char **colName)
    {
        PrintAllData * pad = static_cast<PrintAllData *>(arg);
        return pad->print_record(argc, argv, colName);
    }
}
/* ========================================================================= */


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

    //! Terminates the environment.
    void TearDown(){
        sqlite3_close (db);
    }

    //! Prepare a statement.
    sqlite3_stmt * prepare (const char * statem = NULL) {
        if (statem == NULL) {
            statem = locbuf;
        }
        sqlite3_stmt *res;
        int rc;
        rc = sqlite3_prepare_v2 (db, statem, -1, &res, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        }
        EXPECT_TRUE(rc == SQLITE_OK);
        EXPECT_TRUE(res != NULL);
        return res;
    }

    //! Execute a command.
    void execute (const char * statem = NULL) {
        if (statem == NULL) {
            statem = locbuf;
        }
        sqlite3_stmt *res = prepare (statem);
        fprintf(stderr, "%s\n", statem);

        int rc = sqlite3_step (res);
        ASSERT_EQ(rc, SQLITE_DONE);

        rc = sqlite3_finalize (res);
        ASSERT_EQ(rc, SQLITE_OK);
    }

    //! Execute a command.
    void execute_r (const char * statem = NULL) {
        if (statem == NULL) {
            statem = locbuf;
        }
        sqlite3_stmt *res = prepare (statem);
        fprintf(stderr, "%s\n", statem);

        int rc = sqlite3_step (res);
        if (rc == SQLITE_ERROR) {
            fprintf(stderr, "Error in execute_r: %s\n", sqlite3_errmsg (db));
        }
        ASSERT_EQ(rc, SQLITE_ROW);

        PrintAllData pad;
        pad.print_result (res);

        rc = sqlite3_finalize (res);
        ASSERT_EQ(rc, SQLITE_OK);
    }

    //! Prints all the rows in a table.
    void printAll (const char * table) {
        PrintAllData pad;
        sprintf(locbuf, "SELECT * FROM %s;\n", table);
        fprintf(stderr, "%s\n", locbuf);

        char *errmsg;
        /*int rc = */sqlite3_exec(
                    db, locbuf, print_all_callback,
                    static_cast<void*>(&pad), &errmsg);
        pad.end ();
        if (errmsg != NULL) {
            fprintf(stderr, "Error in sqlite3_exec: %s\n", errmsg);
            sqlite3_free(errmsg);
        }
    }

    /*  FUNCTIONS    ======================================================= */
    //
    //
    //
    //

}; // class TestUsage

//! Test that creates an in-memory database and a table.
class TestWithTable : public TestUsage {
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

    /*  DATA    ============================================================ */
    //
    //
    //
    //
    /*  FUNCTIONS    ------------------------------------------------------- */

public:

    //! Prepares the environment.
    void SetUp () {
        for (;;) {
            TestUsage::SetUp ();
            create_table ("Test");
            break;
        }
    }

    //! Creates a table with two columns and a given name.
    void create_table (const char * name) {
        sprintf(locbuf, "CREATE TABLE %s ("
                        "  id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "  data TEXT, "
                        "  data1 TEXT);\n", name);
        execute (locbuf);
    }

    //! Attach the undo-redo mechanism to a table
    void attach_to_table (const char * name, int mode) {
        sprintf(locbuf, "SELECT %s('%s', %d);\n",
                RESQUN_FUN_TABLE, name, mode);
        execute_r (locbuf);
    }

    //!
    void active_to_table () {
        sprintf(locbuf, "SELECT %s();\n",
                RESQUN_FUN_ACTIVE);
        execute_r (locbuf);
    }

    //!
    void command_begin () {
        sprintf(locbuf, "SELECT %s();\n",
                RESQUN_FUN_BEGIN);
        execute_r (locbuf);
    }

    //!
    void command_end () {
        sprintf(locbuf, "SELECT %s();\n",
                RESQUN_FUN_END);
        execute_r (locbuf);
    }

    //!
    void command_undo () {
        sprintf(locbuf, "SELECT %s();\n",
                RESQUN_FUN_UNDO);
        execute_r (locbuf);
    }

    //!
    void command_redo () {
        sprintf(locbuf, "SELECT %s();\n",
                RESQUN_FUN_REDO);
        execute_r (locbuf);
    }


    /*  FUNCTIONS    ======================================================= */
    //
    //
    //
    //

}; // class TestUsage

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
TEST_F(TestWithTable, sqlite_test) {
    attach_to_table("Test", 2);
    command_begin ();
    execute ("INSERT INTO Test(data, data1) VALUES('Hello', 'World');\n");
    execute ("INSERT INTO Test(data, data1) VALUES('Goodbye', 'Sky');\n");
    command_end ();

    execute_r("PRAGMA table_info(Test)");

    printAll("Test");
    printAll(RESQUN_TBL_TEMP);

    command_undo();

    printAll("Test");
    printAll(RESQUN_TBL_TEMP);

    command_redo();

    printAll("Test");
    printAll(RESQUN_TBL_TEMP);


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
}
/* ========================================================================= */

/*  TESTS    =============================================================== */
//
//
//
//

/* ------------------------------------------------------------------------- */
/* ========================================================================= */
