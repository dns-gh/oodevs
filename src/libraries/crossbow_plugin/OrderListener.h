// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OrderListener_h_
#define __OrderListener_h_

#include "ESRI.h"
#include "Listener_ABC.h"

namespace kernel
{
    class OrderTypes;
}

namespace dispatcher
{
    class Model;
    class Publisher_ABC;
}

namespace crossbow
{
    class Connector;
    class OrderDispatcher;

// =============================================================================
/** @class  OrderListener
    @brief  OrderListener
*/
// Created: SBO 2007-05-30
// =============================================================================
class OrderListener : public Listener_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             OrderListener( Connector& connector, dispatcher::Publisher_ABC& simulation, const kernel::OrderTypes& types, const dispatcher::Model& model );
    virtual ~OrderListener();
    //@}

    //! @name Operations
    //@{
    void Listen();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderListener( const OrderListener& );            //!< Copy constructor
    OrderListener& operator=( const OrderListener& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ProcessOrder( IRowPtr row ) const;
    void MarkProcessed( IRowPtr row ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< OrderDispatcher > dispatcher_;
    ITablePtr table_;
    IQueryFilterPtr waitingOrdersFilter_;
    ICursorPtr cursor_;
    //@}
};

}

#endif // __OrderListener_h_
