// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionFactory_h_
#define __ActionFactory_h_

#include "ActionFactory_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Controllers;
    class OrderType;
    class OrderParameter;
}

class Model;
class ActionParameterFactory_ABC;

// =============================================================================
/** @class  ActionFactory
    @brief  ActionFactory
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionFactory : public ActionFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionFactory( kernel::Controllers& controllers, const ActionParameterFactory_ABC& factory
                          , const Model& model, const kernel::Resolver_ABC< kernel::MissionType >& missions );
    virtual ~ActionFactory();
    //@}

    //! @name Operations
    //@{
    virtual Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::MissionType& mission ) const;
    virtual Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::FragOrderType& fragOrder ) const;

    virtual Action_ABC* CreateAction( const ASN1T_MsgUnitOrder& message ) const;
    virtual Action_ABC* CreateAction( const ASN1T_MsgAutomatOrder& message ) const;

    virtual Action_ABC* CreateAction( xml::xistream& xis ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionFactory( const ActionFactory& );            //!< Copy constructor
    ActionFactory& operator=( const ActionFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddParameters  ( Action_ABC& action, const kernel::OrderType& order, const ASN1T_MissionParameters& asn ) const;
    void AddOrderContext( Action_ABC& action, const kernel::OrderType& order, const ASN1T_OrderContext& asn ) const;
    void ReadParameter  ( xml::xistream& xis, Action_ABC& action, kernel::Iterator< const kernel::OrderParameter& >& it, const kernel::Entity_ABC& entity ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const ActionParameterFactory_ABC& factory_;
    const Model& model_;
    const kernel::Resolver_ABC< kernel::MissionType >& missions_;
    //@}
};

#endif // __ActionFactory_h_
