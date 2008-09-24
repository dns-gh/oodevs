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

namespace dispatcher
{
    class Model;
    class SimulationPublisher_ABC;
}

namespace plugins
{
namespace crossbow
{
    class Database_ABC;
    class Table_ABC;
    class Row_ABC;
    class OrderDispatcher;
    class OrderTypes;

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
             OrderListener( Database_ABC& database, const dispatcher::Model& model, const OrderTypes& types, dispatcher::SimulationPublisher_ABC& publisher );
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
    void Copy( Row_ABC& row );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::SimulationPublisher_ABC& publisher_;
    std::auto_ptr< OrderDispatcher >     dispatcher_;
//    Table_ABC&                           table_;
    Table_ABC&                           validation_;
    long                                 ref_;
    Database_ABC&                        database_;
    //@}
};

}
}

#endif // __OrderListener_h_
