// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectListener_h_
#define __ObjectListener_h_

#include "Listener_ABC.h"

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

namespace crossbow
{
    class Database_ABC;
    class Row_ABC;
    class Table_ABC;

// =============================================================================
/** @class  ObjectListener
    @brief  ObjectListener
*/
// Created: SBO 2007-09-23
// =============================================================================
class ObjectListener : public Listener_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectListener( Database_ABC& database, dispatcher::SimulationPublisher_ABC& publisher );
    virtual ~ObjectListener();
    //@}

    //! @name Operations
    //@{
    virtual void Listen();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectListener( const ObjectListener& );            //!< Copy constructor
    ObjectListener& operator=( const ObjectListener& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SendCreation( const Row_ABC& row );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::SimulationPublisher_ABC& publisher_;
    Table_ABC& table_;
    Database_ABC& database_;
    //@}
};

}

#endif // __ObjectListener_h_
