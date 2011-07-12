// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __plugins_crossbow_StatusListener_h_
#define __plugins_crossbow_StatusListener_h_

#include "Listener_ABC.h"

namespace dispatcher
{
    class SimulationPublisher_ABC;
    class Logger_ABC;
}

namespace plugins
{
namespace crossbow
{
    class Workspace_ABC;
    class Table_ABC;
    class Row_ABC;
    class WorkingSession_ABC;

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
             StatusListener( Workspace_ABC& database, dispatcher::SimulationPublisher_ABC& publisher, const WorkingSession_ABC& session, dispatcher::Logger_ABC& logger );
    virtual ~StatusListener();
    //@}

    //! @name Operations
    //@{
    virtual void Listen();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StatusListener( const StatusListener& );            //!< Copy constructor
    StatusListener& operator=( const StatusListener& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Clean( dispatcher::Logger_ABC& logger );
    void ListenStatusUpdate( Table_ABC& table );
    void ChangeStatus( const std::string& status );
    void ListenTimefactorUpdate( Table_ABC& table );
    void ChangeTimeFactor( int speed );
    void MarkProcessed( Row_ABC& row ) const;
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::SimulationPublisher_ABC& publisher_;
    Workspace_ABC& workspace_;
    const WorkingSession_ABC& session_;
    bool paused_;
    dispatcher::Logger_ABC& logger_;
    //@}
};

}
}

#endif // __plugins_crossbow_StatusListener_h_
