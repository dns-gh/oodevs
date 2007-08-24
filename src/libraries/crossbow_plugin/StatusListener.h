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

#include "ESRI.h"
#include "Listener_ABC.h"

namespace dispatcher
{
    class Model;
    class SimulationPublisher_ABC;
}

namespace crossbow
{
    class Connector;

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
             StatusListener( Connector& connector, dispatcher::SimulationPublisher_ABC& publisher );
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

    //! @name Types
    //@{
    enum E_Status { eResumed, ePaused }; // $$$$ SBO 2007-08-24: a bool should be enough
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::SimulationPublisher_ABC& publisher_;
    bool paused_;
    IRowPtr spRow_;
    long lFieldValue_;
    //@}
};

}

#endif // __StatusListener_h_
