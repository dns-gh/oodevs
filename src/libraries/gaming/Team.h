// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Team_h_
#define __Team_h_

#include "game_asn/Simulation.h"
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
             Team( const ASN1T_MsgTeamCreation& asnMsg, kernel::Controller& controller );
    virtual ~Team();
    //@}

    //! @name Operations
    //@{
    virtual bool IsFriend () const;
    virtual bool IsEnemy  () const;
    virtual bool IsNeutral() const;
    //@}

private:
    //! @name Copy / Assignement
    //@{
    Team( const Team& );
    Team& operator=( const Team& );
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_EnumDiplomacy karma_;
    //@}
};

#endif // __Team_h_
