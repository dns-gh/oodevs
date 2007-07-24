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
    class Publisher_ABC;
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
             StatusListener( Connector& connector, dispatcher::Publisher_ABC& simulation );
    virtual ~StatusListener();
    //@}

    //! @name Operations
    //@{
    // void Send( const ASN1T_MsgsSimToClient& msg );
    void Listen();
    //@}

private:
    enum E_Status { eResumed, ePaused };

private:
    //! @name Copy/Assignement
    //@{
    StatusListener( const StatusListener& );            //!< Copy constructor
    StatusListener& operator=( const StatusListener& ); //!< Assignement operator
    //@}
    
    //! @name Helpers
    //@{
    void ProcessStatus( const std::string& status );
    void Pause();
    void Resume();
    //@}

private:
    //! @name ESRI
    //@{
    IRowPtr spRow_;
    long    lFieldValue_;
    //@}

    //! @name Member data
    //@{
    E_Status status_;
    dispatcher::Publisher_ABC& publisher_;
    //@}
};

}

#endif // __StatusListener_h_
