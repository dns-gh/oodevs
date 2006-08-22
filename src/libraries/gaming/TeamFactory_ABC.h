// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamFactory_ABC_h_
#define __TeamFactory_ABC_h_

namespace DIN { class DIN_Input; }

namespace kernel
{
    class Team_ABC;
}

// =============================================================================
/** @class  TeamFactory_ABC
    @brief  TeamFactory_ABC
*/
// Created: AGE 2006-02-15
// =============================================================================
class TeamFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamFactory_ABC() {};
    virtual ~TeamFactory_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual kernel::Team_ABC* CreateTeam( unsigned long id, DIN::DIN_Input& input ) = 0;
    //@}
};

#endif // __TeamFactory_ABC_h_
