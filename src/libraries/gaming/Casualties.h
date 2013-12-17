// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Casualties_h_
#define __Casualties_h_

#include "ENT/ENT_Enums.h"

// =============================================================================
/** @class  Casualties
    @brief  Casualties
*/
// Created: AGE 2006-03-10
// =============================================================================
class Casualties
{
public:
    //! @name Constructors/Destructor
    //@{
             Casualties();
    explicit Casualties( E_HumanWound wound );
    virtual ~Casualties();
    //@}

public:
    //! @name Member data
    //@{
    E_HumanWound wound_;
    int officers_;
    int subOfficers_;
    int troopers_;
    //@}
};

#endif // __Casualties_h_
