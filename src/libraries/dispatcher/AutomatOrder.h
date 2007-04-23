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

struct ASN1T_MsgAutomateOrder;

namespace dispatcher
{
    class Publisher_ABC;
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
             AutomatOrder( Model& model, Automat& automat, const ASN1T_MsgAutomateOrder& asn );
    virtual ~AutomatOrder();
    //@}

    //! @name Operations
    //@{
    void Send( Publisher_ABC& publisher );
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
    Automat&                            automat_;
    ASN1T_EnumAutomateOrderFormation    formation_;
    //@}
};

}

#endif // __AutomatOrder_h_
