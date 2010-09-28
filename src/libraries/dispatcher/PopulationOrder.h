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

namespace Common
{
    class MsgCrowdOrder;
}

namespace kernel
{
    class Entity_ABC;
}

namespace dispatcher
{

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
    explicit PopulationOrder( const Common::MsgCrowdOrder& asn );
    virtual ~PopulationOrder();
    //@}

    //! @name Operations
    //@{
    virtual void Send( ClientPublisher_ABC& publisher ) const;
    static void SendNoMission( const kernel::Entity_ABC& entity, ClientPublisher_ABC& publisher );
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
    std::auto_ptr< Common::MsgCrowdOrder > message_;
    //@}
};

}

#endif // __PopulationOrder_h_
