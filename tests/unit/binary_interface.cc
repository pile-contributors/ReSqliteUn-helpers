/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
\file         binary_interface.cc
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

class TestBinaryUsage : public TestUsageMultiple {};

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  TESTS    --------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
TEST_F(TestBinaryUsage, instance) {

    ReSqliteUn * inst_last = ReSqliteUn::instance ();
    ASSERT_FALSE(inst_last == NULL);

    int count = ReSqliteUn::instanceCount ();
    //ASSERT_EQ(db_list_.count(), count);

    for (int i = 0; i < count; ++i) {
        ReSqliteUn * inst = ReSqliteUn::instanceForIndex (i);
        ASSERT_FALSE(inst == NULL);
    }

    for (int i = 0; i < db_list_.count(); ++i) {
        ReSqliteUn * inst = ReSqliteUn::instanceForDatabase (db_list_.at (i));
        ASSERT_FALSE(inst == NULL);
    }

    ReSqliteUn * inst_not = ReSqliteUn::instanceForIndex (count);
    ASSERT_TRUE(inst_not == NULL);
    inst_not = ReSqliteUn::instanceForDatabase ((void*)inst_last);
    ASSERT_TRUE(inst_not == NULL);

}
/* ========================================================================= */



/*  TESTS    =============================================================== */
//
//
//
//

/* ------------------------------------------------------------------------- */
/* ========================================================================= */
