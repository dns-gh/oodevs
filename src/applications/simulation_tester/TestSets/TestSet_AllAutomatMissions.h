// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-17 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __TestSet_AllAutomatMissions_h_
#define __TestSet_AllAutomatMissions_h_

#include "TestSet_ABC.h"

namespace TEST {

    class Workspace;

// =============================================================================
/** @class  TestSet_AllAutomatMissions
    @brief  TestSet_AllAutomatMissions
    @par    Using example
    @code
    TestSet_AllAutomatMissions;
    @endcode
*/
// Created: SBO 2005-08-17
// =============================================================================
class TestSet_AllAutomatMissions : public TestSet_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TestSet_AllAutomatMissions( uint nIteration = 1 );
    virtual ~TestSet_AllAutomatMissions();
    //@}

    //! @name Operations
    //@{
    virtual void Load( Workspace& workspace );
    //@}
};

} // end namespace TEST

#endif // __TestSet_AllAutomatMissions_h_
