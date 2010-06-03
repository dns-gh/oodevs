// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Formation_h_
#define __Formation_h_


#include "SimpleEntity.h"
#include "clients_kernel/Formation_ABC.h"

namespace kernel
{
    class HierarchyLevel_ABC;
    class ModelVisitor_ABC;
    class Team_ABC;
}
//using namespace MsgsSimToClient;
namespace Common
{
    enum EnumNatureLevel;
    class MsgFormationCreation;
}

namespace dispatcher
{
    class Model_ABC;
    class ClientPublisher_ABC;
    class Team_ABC;

// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: NLD 2006-09-19
// =============================================================================
class Formation : public SimpleEntity< kernel::Formation_ABC > 
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
    virtual const tools::Resolver< kernel::Formation_ABC >& GetFormations() const;
    virtual const tools::Resolver< kernel::Automat_ABC >& GetAutomates() const;
    //@}

    //! @name Operations
    //@{
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;

    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;

    virtual void Register( kernel::Formation_ABC& formation );
    virtual void Remove( kernel::Formation_ABC& formation );
    virtual void Register( kernel::Automat_ABC& automat );
    virtual void Remove( kernel::Automat_ABC& automat );
    virtual void NotifyParentDestroyed();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Formation( const Formation& );            //!< Copy constructor
    Formation& operator=( const Formation& ); //!< Assignement operator
    //@}

public:
    //! @name Helpers
    //@{
    void NotifyDestructionToChildFormations();
    //@}

private:
    //! @name Member data
    //@{
    const Model_ABC&                            model_;
    const std::string                           name_;
    kernel::Team_ABC&                           team_;
    const kernel::HierarchyLevel_ABC&           level_;
    kernel::Formation_ABC*                      parent_;
    tools::Resolver< kernel::Formation_ABC >    childFormations_;
    tools::Resolver< kernel::Automat_ABC >      automats_;
    //@}
};

}

#endif // __Formation_h_
