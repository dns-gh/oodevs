// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __PopulationFactory_h_
#define __PopulationFactory_h_

#include "PopulationFactory_ABC.h"

// =============================================================================
/** @class  PopulationFactory
    @brief  PopulationFactory
*/
// Created: MGD 2009-10-24
// =============================================================================
class PopulationFactory : public PopulationFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    PopulationFactory();
    virtual ~PopulationFactory();
    //@}

    //! @name Operations
    //@{
    virtual MIL_Population& Create( xml::xistream& xis, MIL_Army& army );
    //@}
};

#endif // __PopulationFactory_h_
