// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentOrder_h_
#define __AgentOrder_h_

#include "Order_ABC.h"

namespace sword
{
    class UnitOrder;
}

namespace kernel
{
    class Entity_ABC;
}

namespace dispatcher
{

// =============================================================================
/** @class  AgentOrder
    @brief  AgentOrder
*/
// Created: NLD 2007-04-20
// =============================================================================
class AgentOrder : public Order_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentOrder( const sword::UnitOrder& message );
    virtual ~AgentOrder();
    //@}

    //! @name Operations
    //@{
    virtual void Send( ClientPublisher_ABC& publisher ) const;
    static void SendNoMission( const kernel::Entity_ABC& entity, ClientPublisher_ABC& publisher );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentOrder( const AgentOrder& );            //!< Copy constructor
    AgentOrder& operator=( const AgentOrder& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< sword::UnitOrder > message_;
    //@}
};

}

#endif // __AgentOrder_h_
