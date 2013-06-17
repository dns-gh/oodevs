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

#include "ActionWithTarget_ABC.h"

namespace kernel
{
    class FragOrderType;
    class Controller;
}

namespace xml
{
    class xistream;
}

namespace actions
{

// =============================================================================
/** @class  FragOrder
    @brief  FragOrder
*/
// Created: SBO 2007-03-19
// =============================================================================
class FragOrder : public ActionWithTarget_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FragOrder( const kernel::Entity_ABC* entity, const kernel::FragOrderType& fragOrder, kernel::Controller& controller, bool registered );
             FragOrder( xml::xistream& xis, kernel::Controller& controller, const tools::Resolver_ABC< kernel::FragOrderType >& fragOrders, const kernel::Entity_ABC& entity );
    virtual ~FragOrder();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Publish( Publisher_ABC& publisher, int context ) const;
    virtual void Polish();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    bool registered_;
    //@}
};

}

#endif // __ActionFragOrder_h_
