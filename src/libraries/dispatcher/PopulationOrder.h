// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationOrder_h_
#define __PopulationOrder_h_

#include "Order_ABC.h"

struct ASN1T_MsgPionOrder;

namespace dispatcher
{
    class Publisher_ABC;
    class Population;

// =============================================================================
/** @class  PopulationOrder
    @brief  PopulationOrder
*/
// Created: NLD 2007-04-20
// =============================================================================
class PopulationOrder : public Order_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationOrder( Model& model, Population& population, const ASN1T_MsgPopulationOrder& asn );
    virtual ~PopulationOrder();
    //@}

    //! @name Operations
    //@{
    void Send( Publisher_ABC& publisher );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationOrder( const PopulationOrder& );            //!< Copy constructor
    PopulationOrder& operator=( const PopulationOrder& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Population& population_;
    //@}
};

}

#endif // __PopulationOrder_h_
