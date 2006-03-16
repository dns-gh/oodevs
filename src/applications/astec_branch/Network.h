// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Network_h_
#define __Network_h_

class AgentServerMsgMgr;
class Model;
class Simulation;

namespace DIN
{
    class DIN_Link;
    class DIN_Engine;
    class DIN_ErrorDescription;
    template< typename T > class DIN_ConnectionServiceClientUserCbk;
}

// =============================================================================
/** @class  Network
    @brief  Network
*/
// Created: AGE 2006-02-08
// =============================================================================
class Network
{

public:
    //! @name Constructors/Destructor
    //@{
             Network( Model& model, Simulation& simu );
    virtual ~Network();
    //@}

    //! @name Operations
    //@{
    bool Connect( const std::string& strHostName, unsigned short nPort );
    bool Disconnect();
    bool IsConnected() const;

    void Update();

    AgentServerMsgMgr& GetMessageMgr();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Network( const Network& );            //!< Copy constructor
    Network& operator=( const Network& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void OnConnected     ( DIN::DIN_Link& link );
    void OnNotConnected  ( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    void OnConnectionLost( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    //@}

private:
    //! @name Member data
    //@{
    DIN::DIN_Engine* engine_;
    DIN::DIN_ConnectionServiceClientUserCbk< Network >* pConnService_;

    DIN::DIN_Link* session_;
    AgentServerMsgMgr* manager_;
    //@}
};

#endif // __Network_h_
