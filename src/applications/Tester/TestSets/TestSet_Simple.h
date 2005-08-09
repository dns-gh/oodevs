// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-05 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __TestSet_Simple_h_
#define __TestSet_Simple_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "TestSet_ABC.h"

namespace TEST {

// =============================================================================
/** @class  TestSet_Simple
    @brief  TestSet_Simple
    @par    Using example
    @code
    TestSet_Simple;
    @endcode
*/
// Created: SBO 2005-08-05
// =============================================================================
class TestSet_Simple : public TestSet_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TestSet_Simple();
    virtual ~TestSet_Simple();
    //@}

    //! @name Operations
    //@{
    virtual void Load( Scheduler& scheduler );
    //@}
};

} // end namespace TEST

#endif // __TestSet_Simple_h_
