// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Fixture_h_
#define __Fixture_h_

#include <directia/Brain.h>

// =============================================================================
/** @class  Fixture
    @brief  Fixture
*/
// Created: SLI 2010-07-12
// =============================================================================
class Fixture
{
public:
    //! @name Constructors/Destructor
    //@{
             Fixture() {}
    virtual ~Fixture() {}
    //@}

    //! @name Member data
    //@{
    directia::Brain brain;
    //@}
};

#endif // __Fixture_h_
