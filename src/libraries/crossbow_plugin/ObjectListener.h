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

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

namespace plugins
{
namespace crossbow
{
    class Workspace_ABC;
    class Database_ABC;
    class Row_ABC;
    class Table_ABC;
    class WorkingSession;

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
             ObjectListener( Workspace_ABC& workspace, dispatcher::SimulationPublisher_ABC& publisher, const WorkingSession& session );
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
    void SendCreation( const Row_ABC& row );
    void ListenObjectCreationRequest( Database_ABC& database, const std::string& tablename, const std::string& query );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::SimulationPublisher_ABC& publisher_;
    Workspace_ABC& workspace_;
    const WorkingSession& session_;
    //@}
};

}
}

#endif // __ObjectListener_h_
