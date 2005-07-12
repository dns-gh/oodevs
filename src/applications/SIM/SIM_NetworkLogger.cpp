// *****************************************************************************
//
// $Created: NLD 2004-02-11 $
// $Archive: /MVW_v10/Build/SDK/SIM/src/SIM_NetworkLogger.cpp $
// $Author: Nld $
// $Modtime: 8/11/04 10:05 $
// $Revision: 2 $
// $Workfile: SIM_NetworkLogger.cpp $
//
// *****************************************************************************

#include "SIM_pch.h"
#include "SIM_NetworkLogger.h"

#include "NEK/NEK_AddressINET.h"
#include "MT/MT_Thread/MT_CriticalSectionLocker.h"

using namespace NEK;

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger constructor
// Created: NLD 2004-02-11
// -----------------------------------------------------------------------------
SIM_NetworkLogger::SIM_NetworkLogger( uint nPort, uint nLogLevels, uint nLogLayers )
    : MT_Logger_ABC( nLogLevels, nLogLayers )
    , nekEngine_( )
{
    NEK_AddressINET address( nPort );
    pServerSocket_ = &nekEngine_.AddAListeningSocket( *this, address, NEK_Protocols::eTCP );
}

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger destructor
// Created: NLD 2004-02-11
// -----------------------------------------------------------------------------
SIM_NetworkLogger::~SIM_NetworkLogger()
{
    for( IT_SocketSet itSocket = clientSocketSet_.begin(); itSocket != clientSocketSet_.end(); ++itSocket )
        (**itSocket).Close();
}


// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger::LogString
// Created: NLD 2004-02-11
// -----------------------------------------------------------------------------
void SIM_NetworkLogger::LogString( const char* strLayerName, E_LogLevel nLevel, const char* szMsg, const char* strContext, int nCode )
{
    MT_CriticalSectionLocker locker( criticalSection_ );

    if( clientSocketSet_.empty() )
        return;

    std::stringstream strTmp;

    strTmp << "[" << GetTimestampAsString() << "]";

    if( strLayerName != NULL )
        strTmp << " " << strLayerName << " -";

    if( nLevel != eLogLevel_None )
        strTmp << " " << GetLogLevelAsString( nLevel ) << " -";

    strTmp << " " << szMsg;

    if( nCode != -1 )
        strTmp << " (" << nCode << ")";

    if( strContext != NULL )
        strTmp << " [Context: " << strContext << "]";

    strTmp << "\r\n";


    for( IT_SocketSet itSocket = clientSocketSet_.begin(); itSocket != clientSocketSet_.end(); ++itSocket )
    {
        NEK_Socket_ABC& socket = **itSocket;
        socket.GetOutput().Append( (const uint8*)( strTmp.str().c_str() ), strTmp.str().size() );
        socket.NotifyOutputUpdated();
    }
}

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger::Update
// Created: NLD 2004-02-11
// -----------------------------------------------------------------------------
void SIM_NetworkLogger::Update()
{
    nekEngine_.DispatchEvents();
}

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger::OnAccept
// Created: NLD 2004-02-11
// -----------------------------------------------------------------------------
void SIM_NetworkLogger::OnAccept( NEK::NEK_Socket_ABC& acceptedSocket, NEK::NEK_Socket_ABC& /*listeningSocket*/ )
{
    MT_CriticalSectionLocker locker( criticalSection_ );
    acceptedSocket.Accept();
    clientSocketSet_.insert( &acceptedSocket );
}

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger::OnClose
// Created: NLD 2004-02-11
// -----------------------------------------------------------------------------
void SIM_NetworkLogger::OnClose( NEK::NEK_Socket_ABC& socket )
{
    MT_CriticalSectionLocker locker( criticalSection_ );
    clientSocketSet_.erase( &socket );
}

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger::OnTimeout
// Created: NLD 2004-02-11
// -----------------------------------------------------------------------------
void SIM_NetworkLogger::OnTimeout( NEK::NEK_Socket_ABC& socket )
{
    MT_CriticalSectionLocker locker( criticalSection_ );
    clientSocketSet_.erase( &socket );
}

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger::OnError
// Created: NLD 2004-02-11
// -----------------------------------------------------------------------------
void SIM_NetworkLogger::OnError( NEK::NEK_Socket_ABC& socket, const MT_Exception& /*exception*/ )
{
    MT_CriticalSectionLocker locker( criticalSection_ );
    clientSocketSet_.erase( &socket );
}

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger::OnConnected
// Created: NLD 2004-02-11
// -----------------------------------------------------------------------------
void SIM_NetworkLogger::OnConnected( NEK::NEK_Socket_ABC& /*socket*/ )
{
    
}

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger::OnRead
// Created: NLD 2004-02-11
// -----------------------------------------------------------------------------
void SIM_NetworkLogger::OnRead( NEK::NEK_Socket_ABC& /*socket*/, NEK::NEK_Input& /*input*/ )
{
    
}

// -----------------------------------------------------------------------------
// Name: SIM_NetworkLogger::OnWrite
// Created: NLD 2004-02-11
// -----------------------------------------------------------------------------
void SIM_NetworkLogger::OnWrite( NEK::NEK_Socket_ABC& /*socket*/, NEK::NEK_Output& /*output*/ )
{
    
}
