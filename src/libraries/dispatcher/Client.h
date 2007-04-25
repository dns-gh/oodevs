// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Client_h_
#define __Client_h_

#include "Publisher_ABC.h"
#include "game_asn/Asn.h"
#include "tools/Client_ABC.h"

namespace DIN { class DIN_Input; }

namespace dispatcher
{
    class Profile;
    class Model;
    class ProfileManager;
    class SimulationNetworker;
    class LoaderFacade;

// =============================================================================
/** @class  Client
    @brief  Client
*/
// Created: NLD 2006-09-19
// =============================================================================
class Client : public tools::Client_ABC
             , public Publisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Client( const Model& model, ProfileManager& profiles, LoaderFacade& loader, DIN::DIN_MessageService_ABC& messageService, DIN::DIN_Link& link );
             Client( const Model& model, ProfileManager& profiles, SimulationNetworker& simulation, DIN::DIN_MessageService_ABC& messageService, DIN::DIN_Link& link );
    virtual ~Client();
    //@}

    //! @name Messages
    //@{
            void OnReceive( const ASN1T_MsgsClientToSim& asnMsg );
            void OnReceive( const ASN1T_MsgsClientToMiddle& asnMsg );
            void OnReceive( unsigned int nMsgID, DIN::DIN_Input& dinMsg );
    virtual void Send     ( const ASN1T_MsgsMiddleToClient& asnMsg );
            void Send     ( const ASN1T_MsgsMiddleToClient& asnMsg, const DIN::DIN_BufferedMessage& dinMsg );    
    virtual void Send     ( const ASN1T_MsgsSimToClient&    asnMsg );
            void Send     ( const ASN1T_MsgsSimToClient&    asnMsg, const DIN::DIN_BufferedMessage& dinMsg );    
            void Send     ( unsigned int nMsgID, const DIN::DIN_BufferedMessage& dinMsg );
    //@}

    //! @name Tools
    //@{
    static Client& GetClientFromLink( const DIN::DIN_Link& link );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Client( const Client& );            //!< Copy constructor
    Client& operator=( const Client& ); //!< Assignement operator
    //@}

    //! @name Rights management
    //@{
    bool CheckRights( const ASN1T_MsgsClientToSim&    asnMsg ) const;
    bool CheckRights( const ASN1T_MsgsClientToMiddle& asnMsg ) const;
    //@}

    //! @name Messages
    //@{
    void OnReceiveMsgAuthenticationRequest    ( const ASN1T_MsgAuthenticationRequest&     asnMsg );
    //@}

private:
    //! @name Member data
    //@{
    const Model&    model_;
    ProfileManager& profiles_;
    Profile*        pProfile_;

    LoaderFacade*        loader_;
    SimulationNetworker* simulation_;
    //@}   
};

}

#endif // __Client_h_
