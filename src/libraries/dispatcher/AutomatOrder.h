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

namespace sword
{
    class AutomatOrder;
}

namespace kernel
{
    class Entity_ABC;
}

namespace dispatcher
{
    class Automat_ABC;

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
    explicit AutomatOrder( const sword::AutomatOrder& message );
    virtual ~AutomatOrder();
    //@}

    //! @name Operations
    //@{
    virtual void Send( ClientPublisher_ABC& publisher ) const;
    static void SendNoMission( const kernel::Entity_ABC& automat, ClientPublisher_ABC& publisher );
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
    std::unique_ptr< sword::AutomatOrder > message_;
    //@}
};

}

#endif // __AutomatOrder_h_
