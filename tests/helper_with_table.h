/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
\file         helper_with_table.h
\date         February 2017
\author       Nicu Tofan

\brief        Base clss for tests.

*//*

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#ifndef HELPER_WITH_TABLE_HEADER
#define HELPER_WITH_TABLE_HEADER
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "helper_usage.h"

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
    void SetUp ();

    //! Creates a table with two columns and a given name.
    void create_table (const char * name) {
        sprintf (locbuf (), "CREATE TABLE %s ("
                        "  id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "  data TEXT, "
                        "  data1 TEXT);\n", name);
        execute (locbuf ());
    }

    //! Attach the undo-redo mechanism to a table
    void attach_to_table (const char * name, int mode) {
        sprintf(locbuf (), "SELECT %s('%s', %d);\n",
                RESQUN_FUN_TABLE, name, mode);
        execute_r (locbuf ());
    }

    //!
    void active_to_table () {
        sprintf(locbuf (), "SELECT %s();\n",
                RESQUN_FUN_ACTIVE);
        execute_r (locbuf ());
    }

    //!
    void command_begin () {
        sprintf(locbuf (), "SELECT %s();\n",
                RESQUN_FUN_BEGIN);
        execute_r (locbuf ());
    }

    //!
    void command_end () {
        sprintf(locbuf (), "SELECT %s();\n",
                RESQUN_FUN_END);
        execute_r (locbuf ());
    }

    //!
    void command_undo () {
        sprintf(locbuf (), "SELECT %s();\n",
                RESQUN_FUN_UNDO);
        execute_r (locbuf ());
    }

    //!
    void command_redo () {
        sprintf(locbuf (), "SELECT %s();\n",
                RESQUN_FUN_REDO);
        execute_r (locbuf ());
    }


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
#endif // HELPER_WITH_TABLE_HEADER
