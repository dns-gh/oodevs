// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Acceptor_h_
#define __Acceptor_h_

#include "asio.h"

namespace tools
{
    class SocketManager;

// =============================================================================
/** @class  Acceptor
    @brief  Acceptor
*/
// Created: AGE 2007-09-06
// =============================================================================
class Acceptor
{
public:
    //! @name Constructors/Destructor
    //@{
             Acceptor( SocketManager& manager, boost::asio::io_service& service, unsigned short port );
    virtual ~Acceptor();
    //@}

    //! @name Operations
    //@{
    void DenyConnections();
    void AllowConnections();
	bool IsAllowingConnections() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Acceptor( const Acceptor& );            //!< Copy constructor
    Acceptor& operator=( const Acceptor& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void OnAccepted( const boost::shared_ptr< boost::asio::ip::tcp::socket >& socket, const boost::system::error_code& error );
    void Listen();
    //@}

private:
    //! @name Member data
    //@{
    SocketManager& manager_;
    boost::asio::io_service& service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    unsigned short port_;
    volatile bool accept_;
    //@}
};

}

#endif // __Acceptor_h_
