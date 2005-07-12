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

#include "NEK/NEK_Module_ABC.h"
#include "NEK/NEK_Engine.h"

#include "MT/MT_Logger/MT_Logger_ABC.h"

// =============================================================================
// Created: NLD 2004-02-11
// =============================================================================
class SIM_NetworkLogger : public MT_Logger_ABC
                        , public NEK::NEK_Module_ABC
{
    MT_COPYNOTALLOWED( SIM_NetworkLogger );

public:
    //! @name Constructors/Destructor
    //@{
               SIM_NetworkLogger( uint nPort, uint nLogLevels = eLogLevel_All, uint nLogLayers = eLogLayer_All );
    /*final*/ ~SIM_NetworkLogger();
    //@}

    //! @name Main
    //@{
    void Update();        
    //@}

private:
    //! @name Log methods 
    //@{
    /*final*/ void LogString( const char* strLayerName, E_LogLevel nLevel, const char* szMsg, const char* strContext, int nCode );
    //@}

    //! @name Events
    //@{
    /*final*/ void  OnAccept   ( NEK::NEK_Socket_ABC& acceptedSocket, NEK::NEK_Socket_ABC& listeningSocket );
    /*final*/ void  OnConnected( NEK::NEK_Socket_ABC& socket );
    /*final*/ void  OnRead     ( NEK::NEK_Socket_ABC& socket, NEK::NEK_Input& input );
    /*final*/ void  OnWrite    ( NEK::NEK_Socket_ABC& socket, NEK::NEK_Output& output );
    /*final*/ void  OnClose    ( NEK::NEK_Socket_ABC& socket );
    /*final*/ void  OnTimeout  ( NEK::NEK_Socket_ABC& socket );
    /*final*/ void  OnError    ( NEK::NEK_Socket_ABC& socket, const MT_Exception& exception );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< NEK::NEK_Socket_ABC* >  T_SocketSet;
    typedef T_SocketSet::iterator             IT_SocketSet;
    typedef T_SocketSet::const_iterator       CIT_SocketSet;
    //@}

private:
    NEK::NEK_Engine         nekEngine_;
    NEK::NEK_Socket_ABC*    pServerSocket_;  
    T_SocketSet             clientSocketSet_;
    MT_CriticalSection      criticalSection_;
};

#ifdef MT_USE_INLINE
#   include "SIM_NetworkLogger.inl"
#endif

#endif // __SIM_NetworkLogger_h_
