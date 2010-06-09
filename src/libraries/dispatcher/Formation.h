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

#include "Formation_ABC.h"

namespace Common
{
    enum EnumNatureLevel;
    class MsgFormationCreation;
}

namespace dispatcher
{
    class Model_ABC;
    class Team_ABC;

// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: NLD 2006-09-19
// =============================================================================
class Formation : public dispatcher::Formation_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Formation( const Model_ABC& model, const Common::MsgFormationCreation& msg, const tools::Resolver_ABC< kernel::HierarchyLevel_ABC >& levels );
    virtual ~Formation();
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::HierarchyLevel_ABC& GetLevel() const;
    virtual kernel::Formation_ABC* GetParent() const;
    virtual kernel::Team_ABC& GetTeam() const;
    virtual const tools::Resolver< dispatcher::Formation_ABC >& GetFormations() const;
    virtual const tools::Resolver< dispatcher::Automat_ABC >& GetAutomates() const;
    //@}

    //! @name Operations
    //@{
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;

    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;

    virtual void Register( dispatcher::Formation_ABC& formation );
    virtual void Remove( dispatcher::Formation_ABC& formation );
    virtual void Register( dispatcher::Automat_ABC& automat );
    virtual void Remove( dispatcher::Automat_ABC& automat );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Formation( const Formation& );            //!< Copy constructor
    Formation& operator=( const Formation& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    template< typename Superior, typename Entity >
    void MoveChildren( Superior& superior, tools::Resolver< Entity >& entities );
    //@}

private:
    //! @name Member data
    //@{
    const Model_ABC&                            model_;
    const std::string                           name_;
    dispatcher::Team_ABC&                       team_;
    const kernel::HierarchyLevel_ABC&           level_;
    dispatcher::Formation_ABC*                  parent_;
    tools::Resolver< dispatcher::Formation_ABC > formations_;
    tools::Resolver< dispatcher::Automat_ABC >   automats_;
    //@}
};

}

#endif // __Formation_h_
