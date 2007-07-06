// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionFragOrder_h_
#define __ActionFragOrder_h_

#include "Action_ABC.h"

namespace kernel
{
    class FragOrderType;
    class Controller;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  ActionFragOrder
    @brief  ActionFragOrder
*/
// Created: SBO 2007-03-19
// =============================================================================
class ActionFragOrder : public Action_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionFragOrder( const kernel::Entity_ABC& entity, const kernel::FragOrderType& fragOrder, kernel::Controller& controller, bool registered );
             ActionFragOrder( xml::xistream& xis, kernel::Controller& controller, const kernel::Resolver_ABC< kernel::FragOrderType >& fragOrders, const kernel::Entity_ABC& entity );
    virtual ~ActionFragOrder();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Publish( Publisher_ABC& publisher ) const;
    virtual void Polish();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionFragOrder( const ActionFragOrder& );            //!< Copy constructor
    ActionFragOrder& operator=( const ActionFragOrder& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    bool registered_;
    //@}
};

#endif // __ActionFragOrder_h_
