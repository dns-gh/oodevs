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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Network/ConnectionHandler.h $
// $Author: Sbo $
// $Modtime: 24/05/05 10:12 $
// $Revision: 3 $
// $Workfile: ConnectionHandler.h $
//
// *****************************************************************************

#ifndef __ConnectionHandler_h_
#define __ConnectionHandler_h_

namespace TEST
{
    
    class NetworkManager;

// =============================================================================
/** @class  ConnectionHandler
    @brief  ConnectionHandler
    @par    Using example
    @code
    ConnectionHandler;
    @endcode
*/
// Created: SBO 2005-05-09
// =============================================================================
class ConnectionHandler
{
	MT_COPYNOTALLOWED( ConnectionHandler );

public:
    //! @name Constructors/Destructor
    //@{
             ConnectionHandler( NetworkManager& networkManager );
    virtual ~ConnectionHandler();
    //@}

    //! @name Operations
    //@{
	bool ConnectToServer     ( const std::string& strHostName, uint nPort );
    bool DisconnectFromServer();
    //@}

private:
    //! @name Connection service callbacks
    //@{
    void OnConnected       ( DIN::DIN_Link& link );
    void OnConnectionFailed( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    void OnConnectionLost  ( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    //@}

private:
    //! @name Member data
    //@{
	DIN::DIN_ConnectionServiceClientUserCbk< ConnectionHandler >  connectionService_;
    NetworkManager&                                               networkManager_;
	//@}
};

} // end namespace TEST

#endif // __ConnectionHandler_h_
