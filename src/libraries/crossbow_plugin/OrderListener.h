// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_OrderListener_h_
#define __crossbow_OrderListener_h_

#include "Listener_ABC.h"

// protocol
class Publisher_ABC;

namespace dispatcher
{
    class Model_ABC;
    class SimulationPublisher_ABC;
}

namespace plugins
{
namespace crossbow
{
    class Workspace_ABC;
    class WorkingSession_ABC;
    class ActionSerializer_ABC;

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
             OrderListener( Workspace_ABC& workspace, ActionSerializer_ABC& serializer, dispatcher::SimulationPublisher_ABC& publisher, const WorkingSession_ABC& session );
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
    bool SendCreation( const Row_ABC& row );
    void Clean();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Publisher_ABC >      publisher_;
    Workspace_ABC&                      workspace_;
    const WorkingSession_ABC&           session_;
    const ActionSerializer_ABC&         serializer_;
    //@}
};

}
}

#endif // __crossbow_OrderListener_h_
