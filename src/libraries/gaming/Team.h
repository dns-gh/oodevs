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
#include "clients_kernel/Karma.h"

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

    virtual const kernel::Karma& GetKarma() const;
    virtual void Register( kernel::Formation_ABC& formation );
    virtual void Remove( kernel::Formation_ABC& formation );
    virtual void Register( kernel::Population_ABC& population );
    virtual void Remove( kernel::Population_ABC& population );
    virtual void Register( kernel::Object_ABC& object );
    virtual void Remove( kernel::Object_ABC& object );
    virtual void Register( kernel::KnowledgeGroup_ABC& knGroup );
    virtual void Remove( kernel::KnowledgeGroup_ABC& knGroup );
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;

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
    //! @name Attributes
    //@{
    kernel::Karma karma_;
    //@}

};

#endif // __Team_h_
