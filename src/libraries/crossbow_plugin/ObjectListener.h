// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_ObjectListener_h_
#define __crossbow_ObjectListener_h_

#include "Listener_ABC.h"

// protocol
class Publisher_ABC;

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
    class Database_ABC;
    class Row_ABC;
    class Table_ABC;
    class WorkingSession_ABC;
    class ActionSerializer_ABC;

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
             ObjectListener( Workspace_ABC& workspace, ActionSerializer_ABC& serializer, dispatcher::SimulationPublisher_ABC& publisher, const WorkingSession_ABC& session, dispatcher::Logger_ABC& logger );
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
    void Clean();
    bool SendCreation( const Row_ABC& row );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Publisher_ABC > publisher_;
    Workspace_ABC& workspace_;
    ActionSerializer_ABC& serializer_;
    const WorkingSession_ABC& session_;
    dispatcher::Logger_ABC& logger_;
    //@}
};

}
}

#endif // __ObjectListener_h_
