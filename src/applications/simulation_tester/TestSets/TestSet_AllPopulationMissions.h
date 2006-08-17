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

#ifndef __TestSet_AllPopulationMissions_h_
#define __TestSet_AllPopulationMissions_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "TestSet_ABC.h"

namespace TEST {

    class Workspace;

// =============================================================================
// Created: SBO 2005-08-11
// =============================================================================
class TestSet_AllPopulationMissions : public TestSet_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TestSet_AllPopulationMissions( uint nIteration = 1 );
    virtual ~TestSet_AllPopulationMissions();
    //@}

    //! @name Operations
    //@{
    virtual void Load( Workspace& workspace );
    //@}
};

} // end namespace TEST

#endif // __TestSet_AllPopulationMissions_h_
