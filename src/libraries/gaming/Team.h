// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Team_h_
#define __Team_h_

#include "DIN_Types.h"
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Team_ABC.h"

// =============================================================================
/** @class  Team
    @brief  Team
*/
// Created: AGN 2003-12-22
// =============================================================================
class Team : public kernel::EntityImplementation< kernel::Team_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             Team( uint id, DIN::DIN_Input& input, kernel::Controller& controller );
    virtual ~Team();
    //@}

private:
    //! @name Copy / Assignement
    //@{
    Team( const Team& );
    Team& operator=( const Team& );
    //@}

    //! @name Helpers
    //@{
    static QString ReadName( DIN::DIN_Input& input );
    //@}
};

#endif // __Team_h_
