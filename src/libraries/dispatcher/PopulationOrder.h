// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationOrder_h_
#define __PopulationOrder_h_

#include "Order_ABC.h"


namespace Common
{
    class MsgUnitOrder;
    class MsgPopulationOrder;
}

namespace dispatcher
{
    class ClientPublisher_ABC;
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
             PopulationOrder( Model_ABC& model, Population& population, const Common::MsgPopulationOrder& asn );
    virtual ~PopulationOrder();
    //@}

    //! @name Operations
    //@{
           void Send         ( ClientPublisher_ABC& publisher );
    static void SendNoMission( const Population& population, ClientPublisher_ABC& publisher );
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
