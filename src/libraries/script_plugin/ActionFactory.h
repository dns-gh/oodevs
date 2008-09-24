// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionFactory_h_
#define __ActionFactory_h_

#include "actions/ActionFactory_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

namespace actions
{
    class ParameterFactory_ABC;
}

namespace kernel
{
    class Controller;
    class OrderParameter;
    class MissionType;
    class FragOrderType;
}

namespace dispatcher
{
    class Model;
}

namespace plugins
{
namespace script
{

// =============================================================================
/** @class  ActionFactory
    @brief  ActionFactory
*/
// Created: AGE 2008-07-16
// =============================================================================
class ActionFactory : public actions::ActionFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionFactory( kernel::Controller& controller, const dispatcher::Model& model, const actions::ParameterFactory_ABC& factory );
    virtual ~ActionFactory();
    //@}

    //! @name Operations
    //@{
    virtual actions::Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::MissionType& mission ) const;
    virtual actions::Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::FragOrderType& fragOrder ) const;
    virtual actions::Action_ABC* CreateAction( xml::xistream& xis ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionFactory( const ActionFactory& );            //!< Copy constructor
    ActionFactory& operator=( const ActionFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    actions::Action_ABC* CreateMission( xml::xistream& xis ) const;
    actions::Action_ABC* CreateFragOrder( xml::xistream& xis ) const;
    const kernel::Entity_ABC& ResolveEntity( const unsigned long id ) const;
    void ReadParameter( xml::xistream& xis, actions::Action_ABC& action, kernel::Iterator< const kernel::OrderParameter& >& it, const kernel::Entity_ABC& entity ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const dispatcher::Model& model_;
    const kernel::Resolver_ABC< kernel::MissionType >& missions_;
    const kernel::Resolver_ABC< kernel::FragOrderType >& fragOrders_;
    const actions::ParameterFactory_ABC& factory_;
    //@}
};

}
}

#endif // __ActionFactory_h_
