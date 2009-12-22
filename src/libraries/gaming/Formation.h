// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Formation_h_
#define __Formation_h_

#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/EntityImplementation.h"
#include "tools/Resolver_ABC.h"
#include "game_asn/Simulation.h"

// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: AGE 2006-10-19
// =============================================================================
class Formation : public kernel::EntityImplementation< kernel::Formation_ABC > 
{

public:
    //! @name Constructors/Destructor
    //@{
             Formation( const ASN1T_MsgFormationCreation& message, kernel::Controller& controller, const tools::Resolver_ABC< kernel::HierarchyLevel_ABC >& resolver );
    virtual ~Formation();
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::HierarchyLevel_ABC& GetLevel() const;
    virtual kernel::Formation_ABC* GetParent() const;
    virtual kernel::Team_ABC& GetTeam() const;
    virtual const tools::Resolver< kernel::Formation_ABC >& GetFormations() const;
    virtual const tools::Resolver< kernel::Automat_ABC >& GetAutomates() const;
    //@}

    //! @name Operations
    //@{
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    virtual void Register( kernel::Formation_ABC& formation );
    virtual void Remove( kernel::Formation_ABC& formation );
    virtual void Register( kernel::Automat_ABC& automat );
    virtual void Remove( kernel::Automat_ABC& automat );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Formation( const Formation& );            //!< Copy constructor
    Formation& operator=( const Formation& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::HierarchyLevel_ABC& level_;
    //@}
};

#endif // __Formation_h_
