// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-11 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __TestSet_PawnMissions_h_
#define __TestSet_PawnMissions_h_

#include "TestSet_ABC.h"

namespace TEST {

    class Workspace;

// =============================================================================
/** @class  TestSet_PawnMissions
    @brief  TestSet_PawnMissions
    @par    Using example
    @code
    TestSet_PawnMissions;
    @endcode
*/
// Created: SBO 2005-08-11
// =============================================================================
class TestSet_PawnMissions : public TestSet_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TestSet_PawnMissions();
    virtual ~TestSet_PawnMissions();
    //@}

    //! @name Operations
    //@{
    virtual void Load( Workspace& workspace );
    //@}
};

} // end namespace TEST

#endif // __TestSet_PawnMissions_h_
