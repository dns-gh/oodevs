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

#include "Listener_ABC.h"

namespace kernel
{
    class OrderTypes;
}

namespace dispatcher
{
    class Model;
    class SimulationPublisher_ABC;
}

namespace crossbow
{
    class Connector;
    class OrderDispatcher;
    class Table_ABC;
    class Row_ABC;

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
             OrderListener( Connector& connector, dispatcher::SimulationPublisher_ABC& publisher, const kernel::OrderTypes& types, const dispatcher::Model& model );
    virtual ~OrderListener();
    //@}

    //! @name Operations
    //@{
    virtual void Listen();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderListener( const OrderListener& );            //!< Copy constructor
    OrderListener& operator=( const OrderListener& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{    
    void MarkProcessed( Row_ABC& row ) const;
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::SimulationPublisher_ABC& publisher_;
    std::auto_ptr< OrderDispatcher > dispatcher_;
    Table_ABC& table_;
    //@}
};

}

#endif // __OrderListener_h_
