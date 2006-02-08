// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_Network_h_
#define __MOS_Network_h_

class MOS_AgentServerMsgMgr;

namespace DIN
{
    class DIN_Link;
    class DIN_Engine;
    class DIN_ErrorDescription;
    template< typename T > class DIN_ConnectionServiceClientUserCbk;
}

// =============================================================================
/** @class  MOS_Network
    @brief  MOS_Network
*/
// Created: AGE 2006-02-08
// =============================================================================
class MOS_Network
{

public:
    //! @name Constructors/Destructor
    //@{
             MOS_Network();
    virtual ~MOS_Network();
    //@}

    //! @name Operations
    //@{
    bool Connect( const std::string& strHostName, uint16 nPort );
    bool Disconnect();
    bool IsConnected() const;

    void Update();

    MOS_AgentServerMsgMgr& GetMessageMgr();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MOS_Network( const MOS_Network& );            //!< Copy constructor
    MOS_Network& operator=( const MOS_Network& ); //!< Assignement operator
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
    DIN::DIN_ConnectionServiceClientUserCbk< MOS_Network >* pConnService_;

    DIN::DIN_Link* session_;
    MOS_AgentServerMsgMgr* manager_;
    //@}
};

#endif // __MOS_Network_h_
