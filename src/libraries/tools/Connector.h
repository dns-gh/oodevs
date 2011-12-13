// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Connector_h_
#define __Connector_h_

#include "asio.h"

namespace tools
{
    class SocketManager;
    class SocketEventCallback_ABC;

// =============================================================================
/** @class  Connector
    @brief  Connector
*/
// Created: AGE 2007-09-06
// =============================================================================
class Connector
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Connector( boost::asio::io_service& service, SocketManager& manager, SocketEventCallback_ABC& callback );
    virtual ~Connector();
    //@}

    //! @name Operations
    //@{
    void Connect( const std::string& endpoint );
    void Close();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Connector( const Connector& );            //!< Copy constructor
    Connector& operator=( const Connector& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void OnResolve( const std::string& endpoint, const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator it );
    void DoConnect( const boost::asio::ip::tcp::endpoint& endpoint );
    void OnConnect( const boost::shared_ptr< boost::asio::ip::tcp::socket >& socket, const boost::asio::ip::tcp::endpoint& endpoint, const boost::system::error_code& error );
    //@}

private:
    //! @name Member data
    //@{
    boost::asio::io_service& service_;
    SocketManager& manager_;
    SocketEventCallback_ABC& callback_;
    boost::asio::ip::tcp::resolver resolver_;
    bool closed_;
    //@}
};

}

#endif // __Connector_h_
