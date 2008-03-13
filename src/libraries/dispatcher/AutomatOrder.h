// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatOrder_h_
#define __AutomatOrder_h_

#include "Order_ABC.h"
#include "game_asn/Simulation.h"

struct ASN1T_MsgAutomatOrder;

namespace dispatcher
{
    class ClientPublisher_ABC;
    class Automat;

// =============================================================================
/** @class  AutomatOrder
    @brief  AutomatOrder
*/
// Created: NLD 2007-04-20
// =============================================================================
class AutomatOrder : public Order_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomatOrder( Model& model, Automat& automat, const ASN1T_MsgAutomatOrder& asn );
    virtual ~AutomatOrder();
    //@}

    //! @name Operations
    //@{
           void Send         ( ClientPublisher_ABC& publisher );
    static void SendNoMission( const Automat& automat, ClientPublisher_ABC& publisher ); 
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatOrder( const AutomatOrder& );            //!< Copy constructor
    AutomatOrder& operator=( const AutomatOrder& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Automat&                        automat_;
    ASN1T_EnumAutomatOrderFormation formation_;
    //@}
};

}

#endif // __AutomatOrder_h_
