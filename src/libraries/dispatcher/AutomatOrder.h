// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatOrder_h_
#define __AutomatOrder_h_

#include "Order_ABC.h"


namespace Common
{
    class MsgAutomatOrder;
}

namespace dispatcher
{
     class Automat;
     class ClientPublisher_ABC;

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
             AutomatOrder( Model_ABC& model, Automat& automat, const Common::MsgAutomatOrder& asn );
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
    //@}
};

}

#endif // __AutomatOrder_h_
