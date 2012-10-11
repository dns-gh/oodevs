// *****************************************************************************
//
// $Created: NLD 2004-02-11 $
// $Archive: /MVW_v10/Build/SDK/SIM/src/SIM_NetworkLogger.h $
// $Author: Nld $
// $Modtime: 8/11/04 10:04 $
// $Revision: 2 $
// $Workfile: SIM_NetworkLogger.h $
//
// *****************************************************************************

#ifndef __SIM_NetworkLogger_h_
#define __SIM_NetworkLogger_h_

#include "MT_Tools/MT_Logger.h"
#include <boost/asio.hpp>

namespace boost
{
    class thread;
    class mutex;
}

// =============================================================================
// Created: NLD 2004-02-11
// =============================================================================
class SIM_NetworkLogger : public MT_Logger_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit SIM_NetworkLogger( unsigned short nPort, int nLogLevels = eLogLevel_All );
    virtual ~SIM_NetworkLogger();
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< boost::asio::ip::tcp::socket > T_Socket;

    typedef std::set< T_Socket >        T_Sockets;
    typedef T_Sockets::const_iterator CIT_Sockets;
    //@}

private:
    //! @name Operations
    //@{
    virtual void WriteString( const std::string& s );
    //@}

    //! @name Helpers
    //@{
    void Accept();
    void OnAccept( T_Socket socket, const boost::system::error_code& error );
    void OnWrite( T_Socket socket, const boost::system::error_code& error );
    //@}

private:
    //! @name Member data
    //@{
    T_Sockets                      sockets_;
    boost::asio::io_service        service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::auto_ptr< boost::mutex >  mutex_;
    std::auto_ptr< boost::thread > thread_;
    //@}

};

#endif // __SIM_NetworkLogger_h_
