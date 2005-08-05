// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-09 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Network/NetworkManager.h $
// $Author: Nld $
// $Modtime: 8/06/05 13:30 $
// $Revision: 5 $
// $Workfile: NetworkManager.h $
//
// *****************************************************************************

#ifndef __NetworkManager_h_
#define __NetworkManager_h_

#include "DIN/DIN_Connector_ABC.h"
#include "DIN/DIN_Engine.h"

namespace DIN
{
    class DIN_Link;
}

namespace TEST
{
    class ConnectionHandler;
    class MessageManager;


enum EConnector
{
	eConnector_SIM_MOS  = (DIN::DIN_Connector_ABC::DIN_ConnectionID)-1,
	eConnector_SIM_LAU  = 1,
	eConnector_MOS_LAU  = 2,
	eConnector_SIM_WMI  = 3,
	eConnector_SIM_TEST = 4
};

enum EConnMagicCookie
{
    eConnMagicTICServerAgentServer   = 1
};

// =============================================================================
/** @class  NetworkManager
    @brief  NetworkManager
    @par    Using example
    @code
    Server;
    @endcode
*/
// Created: SBO 2005-05-09
// =============================================================================
class NetworkManager
{
	MT_COPYNOTALLOWED( NetworkManager );

public:
    //! @name Constructors/Destructor
    //@{
             NetworkManager( const std::string& strHostName, uint nPort );
    virtual ~NetworkManager();
    //@}

    //! @name Operations
    //@{
	void               Connect             ();
    void               Disconnect          ();
    void               Update              ();
    //@}

    //! @name Accessors
    //@{
	DIN::DIN_Engine&   GetDINEngine        ();
    ConnectionHandler& GetConnectionHandler() const;
	MessageManager&    GetMessageMgr       () const;
    DIN::DIN_Link&     GetLink             () const;
    bool               IsConnected         () const;
    //@}

    //! @name Modifiers
    //@{
    void               BindLink            ( DIN::DIN_Link& link );
    void               UnbindLink          ();
    //@}

private:
    //! @name Member data
    //@{
	std::string        strServerHost_;
	uint               nServerPort_;

	DIN::DIN_Engine    dinEngine_;
    DIN::DIN_Link*     pLink_;
    
    ConnectionHandler* pConnectionHandler_;
	MessageManager*    pMessageMgr_;
    //@}
};

}

#include "NetworkManager.inl"

#endif // __NetworkManager_h_
