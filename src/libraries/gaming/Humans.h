// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Humans_h_
#define __Humans_h_

#include "clients_kernel/Types.h"

// =============================================================================
/** @class  Humans
    @brief  Humans
*/
// Created: AGE 2006-03-10
// =============================================================================
class Humans
{

public:
    //! @name Constructors/Destructor
    //@{
             Humans();
             Humans( kernel::E_TroopHealthState state );
    virtual ~Humans();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Humans( const Humans& );            //!< Copy constructor
    Humans& operator=( const Humans& ); //!< Assignment operator
    //@}

public:
    //! @name Member data
    //@{
    kernel::E_TroopHealthState state_;
    unsigned officers_;
    unsigned subOfficers_;
    unsigned troopers_;
    //@}
};

#endif // __Humans_h_
