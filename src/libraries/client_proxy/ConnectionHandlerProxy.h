// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ConnectionHandlerProxy_h_
#define __ConnectionHandlerProxy_h_

#include "SwordConnectionHandler_ABC.h"

// =============================================================================
/** @class  ConnectionHandlerProxy
    @brief  ConnectionHandlerProxy
*/
// Created: SEB 2010-10-12
// =============================================================================
class ConnectionHandlerProxy : public SwordConnectionHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ConnectionHandlerProxy();
    virtual ~ConnectionHandlerProxy();
    //@}

    //! @name Proxy operations
    //@{
    void SetHandler( SwordConnectionHandler_ABC* handler );
    //@}

    //! @name Proxyfied operations
    //@{
    virtual void OnConnectionSucceeded( const std::string& endpoint );
    virtual void OnConnectionFailed( const std::string& endpoint, const std::string& reason );
    virtual void OnConnectionError( const std::string& endpoint, const std::string& reason );
    virtual void OnConnectionWarning( const std::string& endpoint, const std::string& reason );
    virtual void OnAuthenticationSucceeded( const std::string& profile );
    virtual void OnAuthenticationFailed( const std::string& profile, const std::string& reason );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ConnectionHandlerProxy( const ConnectionHandlerProxy& );            //!< Copy constructor
    ConnectionHandlerProxy& operator=( const ConnectionHandlerProxy& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    SwordConnectionHandler_ABC* handler_;
    //@}
};

#endif // __ConnectionHandlerProxy_h_
