// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __SwordProxy_h_
#define __SwordProxy_h_

#include "SwordMessagePublisher_ABC.h"
#include <string>

class SwordConnectionHandler_ABC;
class SwordMessageHandler_ABC;

// =============================================================================
/** @class  SwordProxy
    @brief  SwordProxy
*/
// Created: SEB 2010-10-12
// =============================================================================
class SwordProxy : public SwordMessagePublisher_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SwordProxy( const std::string& host, unsigned short port, const std::string& profile );
    virtual ~SwordProxy();
    //@}

    //! @name Network operations
    //@{
    void Connect( SwordConnectionHandler_ABC* handler );
    void Disconnect();
    void Update();
    //@}

    //! @name Message callbacks
    //@{
    void RegisterMessageHandler( SwordMessageHandler_ABC* handler );
    void UnregisterMessageHandler( SwordMessageHandler_ABC* handler );
    //@}

    //! @name Message senders
    //@{
    virtual void Send( const MsgsClientToSim::MsgClientToSim& message ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SwordProxy( const SwordProxy& );            //!< Copy constructor
    SwordProxy& operator=( const SwordProxy& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    class SwordProxyImplementation;
    //@}

private:
    //! @name Member data
    //@{
    SwordProxyImplementation* pimpl_;
    //@}
};

#endif // __SwordProxy_h_
