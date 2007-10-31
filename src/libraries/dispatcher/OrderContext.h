// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OrderContext_h_
#define __OrderContext_h_

#include "ModelsContainer.h"
#include "Localisation.h"

struct ASN1T_OrderContext;

namespace dispatcher
{
    class LimaOrder;
    class IntelligenceOrder;

// =============================================================================
/** @class  OrderContext
    @brief  OrderContext
*/
// Created: NLD 2007-04-20
// =============================================================================
class OrderContext
{

public:
    //! @name Constructors/Destructor
    //@{
             OrderContext();
             OrderContext( Model& model, const ASN1T_OrderContext& context );
    virtual ~OrderContext();
    //@}

    //! @name Operations
    //@{
           void Send     ( ASN1T_OrderContext& asn ) const;
    static void AsnDelete( ASN1T_OrderContext& asn );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderContext( const OrderContext& );            //!< Copy constructor
    OrderContext& operator=( const OrderContext& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int                          dangerDirection_;
    ModelsContainer< LimaOrder >          limaOrders_;
    ModelsContainer< IntelligenceOrder >  intelligenceOrders_;
    Localisation                          rightLimit_;
    Localisation                          leftLimit_;
    bool                                  bRightLimitPresent_;
    bool                                  bLeftLimitPresent_;
    //@}
};

}

#endif // __OrderContext_h_
