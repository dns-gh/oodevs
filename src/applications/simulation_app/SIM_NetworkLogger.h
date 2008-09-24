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

#include "SIM.h"
#include "MT/MT_Logger/MT_Logger_ABC.h"

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
    MT_COPYNOTALLOWED( SIM_NetworkLogger );

public:
    //! @name Constructors/Destructor
    //@{
    explicit SIM_NetworkLogger( uint nPort, uint nLogLevels = eLogLevel_All, uint nLogLayers = eLogLayer_All );
    virtual ~SIM_NetworkLogger();
    //@}

private:
    
    //! @name Log methods 
    //@{
    void LogString        ( const char* strLayerName, E_LogLevel nLevel, const char* szMsg, const char* strContext, int nCode );
    void WaitForClient    ();
    void StartConnection( boost::shared_ptr< boost::asio::ip::tcp::socket > newClientSocket, const boost::system::error_code& error );
    void StopConnection ( boost::shared_ptr< boost::asio::ip::tcp::socket > socket ); 
    void AsyncWrite        ( boost::shared_ptr< boost::asio::ip::tcp::socket > socket, const boost::system::error_code& error ); 
    //@}

private:

    //! @name Types
    //@{
    typedef std::set < boost::shared_ptr< boost::asio::ip::tcp::socket > >  T_SocketSet ; 
    typedef T_SocketSet::iterator                                          IT_SocketSet ; 
    typedef T_SocketSet::const_iterator                                   CIT_SocketSet ; 
    //@}

private:

    //! @name 
    //@{
    T_SocketSet                        sockets_; 
    boost::asio::io_service            io_service_;
    boost::asio::ip::tcp::acceptor    acceptor_ ; 
    std::auto_ptr< boost::mutex    >    criticalSection_;
    std::auto_ptr< boost::thread >    thread_ ; 
    //@}
    
};

#endif // __SIM_NetworkLogger_h_
