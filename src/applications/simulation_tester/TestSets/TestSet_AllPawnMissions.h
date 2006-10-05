// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
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

#ifndef __TestSet_AllPawnMissions_h_
#define __TestSet_AllPawnMissions_h_

#include "TestSet_ABC.h"

namespace TEST {

    class Workspace;

// =============================================================================
/** @class  TestSet_AllPawnMissions
    @brief  TestSet_AllPawnMissions
    @par    Using example
    @code
    TestSet_AllPawnMissions;
    @endcode
*/
// Created: SBO 2005-08-11
// =============================================================================
class TestSet_AllPawnMissions : public TestSet_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TestSet_AllPawnMissions( uint nIteration = 1 );
    virtual ~TestSet_AllPawnMissions();
    //@}

    //! @name Operations
    //@{
    virtual void Load( Workspace& workspace );
    //@}
};

} // end namespace TEST

#endif // __TestSet_AllPawnMissions_h_
