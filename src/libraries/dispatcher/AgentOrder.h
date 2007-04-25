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

struct ASN1T_MsgPionOrder;

namespace dispatcher
{
    class Publisher_ABC;
    class Agent;

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
             AgentOrder( Model& model, Agent& agent, const ASN1T_MsgPionOrder& asn );
    virtual ~AgentOrder();
    //@}

    //! @name Operations
    //@{
           void Send         ( Publisher_ABC& publisher );
    static void SendNoMission( const Agent& agent, Publisher_ABC& publisher );
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
    Agent& agent_;
    //@}
};

}

#endif // __AgentOrder_h_
