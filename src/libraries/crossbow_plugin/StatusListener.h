// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StatusListener_h_
#define __StatusListener_h_

#include "Listener_ABC.h"

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

namespace plugins
{
namespace crossbow
{
    class Database_ABC;
    class Table_ABC;
    class Row_ABC;

// =============================================================================
/** @class  StatusListener
    @brief  StatusListener
*/
// Created: JCR 2007-06-13
// =============================================================================
class StatusListener : public Listener_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             StatusListener( Database_ABC& database, dispatcher::SimulationPublisher_ABC& publisher );
    virtual ~StatusListener();
    //@}

    //! @name Operations
    //@{    
    virtual void Listen();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    StatusListener( const StatusListener& );            //!< Copy constructor
    StatusListener& operator=( const StatusListener& ); //!< Assignement operator
    //@}
    
    //! @name Helpers
    //@{
    void ChangeStatus( const std::string& status );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::SimulationPublisher_ABC& publisher_;
//    Table_ABC& propertiesTable_;
    Database_ABC& database_;
    bool paused_;
    //@}
};

}
}

#endif // __StatusListener_h_
