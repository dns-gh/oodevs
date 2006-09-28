// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ClientsNetworker_h_
#define __ClientsNetworker_h_

#include "Networker_ABC.h"

#include "DIN/MessageService/DIN_MessageServiceUserCbk.h"
#include "DIN/ConnectionService/DIN_ConnectionServiceServerUserCbk.h"

struct ASN1T_MsgsInClient;

namespace dispatcher 
{

class Client;
class MessageManager;

// =============================================================================
/** @class  ClientsNetworker
    @brief  ClientsNetworker
*/
// Created: NLD 2006-09-19
// =============================================================================
class ClientsNetworker : public Networker_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     ClientsNetworker( Dispatcher& dispatcher, unsigned short nPort );
    ~ClientsNetworker();
    //@}

    //! @name Main
    //@{
    void Dispatch( const ASN1T_MsgsInClient& asnMsg );
    void Dispatch( unsigned int nMsgID, DIN::DIN_BufferedMessage& dinMsg );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ClientsNetworker( const ClientsNetworker& );            //!< Copy constructor
    ClientsNetworker& operator=( const ClientsNetworker& ); //!< Assignement operator
    //@}

    //! @name Connection callbacks
    //@{
    void OnConnectionReceived   ( DIN::DIN_Server& server, DIN::DIN_Link& link );
    void OnConnectionLost       ( DIN::DIN_Server& server, DIN::DIN_Link& link                , const DIN::DIN_ErrorDescription& reason );
    void OnBadConnectionReceived( DIN::DIN_Server& server, const NEK::NEK_Address_ABC& address, const DIN::DIN_ErrorDescription& reason );    
    //@}

    //! @name Messages callbacks
    //@{
    void OnReceiveMsgOutClient             ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    bool OnErrorReceivingMessage           ( DIN::DIN_Link &link, const DIN::DIN_ErrorDescription& info );

    //$$$ TMP DIN
    void OnReceiveMsgEnableUnitVisionCones ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgDisableUnitVisionCones( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgEnableProfiling       ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgDisableProfiling      ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgUnitMagicAction       ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgDebugDrawPoints       ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    //@}

    //! @name Tools
    //@{
    template< typename T >
    void Dispatch( const T& asnMsg, uint nDinID, const DIN::DIN_BufferedMessage& dinMsg );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< Client* >         T_ClientSet;
    typedef T_ClientSet::const_iterator CIT_ClientSet;
    //@}

private:
    DIN::DIN_ConnectionServiceServerUserCbk< ClientsNetworker > connectionService_;
    DIN::DIN_MessageServiceUserCbk         < ClientsNetworker > messageService_;
    DIN::DIN_Server*                                            pServer_;
    T_ClientSet                                                 clients_;
};

}

#endif // __ClientsNetworker_h_
