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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Messages/MessageManager.h $
// $Author: Sbo $
// $Modtime: 29/06/05 9:29 $
// $Revision: 13 $
// $Workfile: MessageManager.h $
//
// *****************************************************************************

#ifndef __MessageManager_h_
#define __MessageManager_h_

#include "Types.h"
#include "Messages/ASN_Types.h"
#include "Workspace.h"

namespace DIN
{
	template < class T > class DIN_MessageServiceUserCbk;
 
	class DIN_Input;
    class DIN_BufferedMessage;
	class DIN_Engine;
	class DIN_Link;
    class DIN_ErrorDescription;
}

namespace TEST
{
    
    class NetworkManager;

// =============================================================================
/** @class  MessageManager
    @brief  MessageManager
    @par    Using example
    @code
    MessageManager;
    @endcode
*/
// Created: SBO 2005-05-09
// =============================================================================
class MessageManager
{
	MT_COPYNOTALLOWED( MessageManager );

public:
    //-------------------------------------------------------------------------
    /** @name Messages */
    //-------------------------------------------------------------------------
    //@{
    //! SIM <- MOS
    enum 
    {
        eMsgMosSim                  = 0,
        eMsgMosSimWithContext       = 1,

        eMsgEnableUnitVisionCones   = 1000,
        eMsgDisableUnitVisionCones  = 1001,
        eMsgUnitMagicAction         = 1002
    };

    //! SIM -> MOS
    enum
    {
        eMsgSimMos                  = 0,
        eMsgSimMosWithContext       = 1,

        eMsgUnitVisionCones         = 1004,
        eMsgUnitTrace               = 1005,
        eMsgInit                    = 1007,
        eMsgProfilingValues         = 1008,
        eMsgUnitInterVisibility     = 1009,
        eMsgObjectInterVisibility   = 1010,
        eMsgKnowledgeGroup          = 1013,
        eMsgArmy                    = 1014,
        eMsgDebugDrawPoints         = 1015,

        eMsgTerrainType             = 1016,
    };
    //@}

    //! @name Constructors/Destructor
    //@{
             MessageManager( Workspace& workspace, NetworkManager& networkManager );
    virtual ~MessageManager();
    //@}

    //! @name Service activation
    //@{
    void Enable ( DIN::DIN_Link& link );
    void Disable( DIN::DIN_Link& link );
    //@}

	//! @name Message management
	//@{
	void SendMsgMosSim           ( ASN1T_MsgsMosSim& asnMsg );
	void SendMsgMosSimWithContext( ASN1T_MsgsMosSimWithContext& asnMsg, T_NetContextId nCtx );
	void SendMsgMosSim           ( ASN1OCTET* pMsg, int nMsgLength );
	void SendMsgMosSimWithContext( ASN1OCTET* pMsg, int nMsgLength, T_NetContextId nCtx );

    void SendMsgDebugDrawPoints  ( DIN::DIN_BufferedMessage& msg );
	//@}

	//! @name Message
	//@{
	DIN::DIN_BufferedMessage BuildMessage();
	//@}

    //! @name Accessors
    //@{
	bool IsPaused() const;
    //@}

private:
    //! @name Service callback
    //@{
    bool OnError                              ( DIN::DIN_Link &link, const DIN::DIN_ErrorDescription& info );

	void OnReceiveMsgInit                     ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
	void OnReceiveMsgSimMos                   ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
	void OnReceiveMsgSimMosWithContext        ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );

    // Control
    void OnReceiveMsgCtrlInfo                 ( const ASN1T_MsgCtrlInfo& asnMsg );
    void OnReceiveMsgCtrlBeginTick            ( const ASN1T_MsgCtrlBeginTick& asnMsg );
    void OnReceiveMsgCtrlEndTick              ( const ASN1T_MsgCtrlEndTick& asnMsg );
    void OnReceiveMsgCtrlChangeTimeFactorAck  ( const ASN1T_MsgCtrlChangeTimeFactorAck& asnMsg );
    void OnReceiveMsgCtrlSendCurrentStateBegin();
    void OnReceiveMsgCtrlSendCurrentStateEnd  ();

    //! @name Entities
    //@{
    void OnReceiveMsgKnowledgeGroup           ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgTeam                     ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgAutomatCreation          ( const ASN1T_MsgAutomateCreation& asnMsg );
	void OnReceiveMsgPawnCreation             ( const ASN1T_MsgPionCreation& asnMsg );
    void OnReceiveMsgUnitDotations            ( const ASN1T_MsgUnitDotations& asnMsg );
    void OnReceiveMsgUnitAttributes           ( const ASN1T_MsgUnitAttributes& asnMsg );

    void OnReceiveMsgObjectCreation           ( const ASN1T_MsgObjectCreation& asnMsg );
    void OnReceiveMsgObjectUpdate             ( const ASN1T_MsgObjectUpdate& asnMsg );
    void OnReceiveMsgObjectDestruction        ( const ASN1T_MsgObjectDestruction& asnMsg );
    //@}

    //! @name Knowledge
    //@{
    // unit
    void OnReceiveMsgUnitKnowledgeCreation   ( const ASN1T_MsgUnitKnowledgeCreation&    asnMsg );
    void OnReceiveMsgUnitKnowledgeUpdate     ( const ASN1T_MsgUnitKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgUnitKnowledgeDestruction( const ASN1T_MsgUnitKnowledgeDestruction& asnMsg );

    // object
    void OnReceiveMsgObjectKnowledgeCreation   ( const ASN1T_MsgObjectKnowledgeCreation& asnMsg );
    void OnReceiveMsgObjectKnowledgeUpdate     ( const ASN1T_MsgObjectKnowledgeUpdate& asnMsg );
    void OnReceiveMsgObjectKnowledgeDestruction( const ASN1T_MsgObjectKnowledgeDestruction& asnMsg );
    //@}

    //! @name Limits/Limas
    //@{
    void OnReceiveMsgLimitCreation            ( const ASN1T_MsgLimitCreation&    asnMsg );
    void OnReceiveMsgLimitDestruction         ( const ASN1T_MsgLimitDestruction& asnMsg );
    void OnReceiveMsgLimaCreation             ( const ASN1T_MsgLimaCreation&     asnMsg );
    void OnReceiveMsgLimaDestruction          ( const ASN1T_MsgLimaDestruction&  asnMsg );
    //@}

    // Hierarchy changes
    void OnReceiveMsgChangeAutomat            ( const ASN1T_MsgChangeAutomate& asnMsg );
    void OnReceiveMsgChangeAutomateAck        ( const ASN1T_MsgChangeAutomateAck& asnMsg, T_NetContextId nCtx );
    void OnReceiveMsgChangeTeamRelation       ( const ASN1T_MsgChangeDiplomatie& asnMsg );
    void OnReceiveMsgChangeTeamRelationAck    ( const ASN1T_MsgChangeDiplomatieAck& asnMsg, T_NetContextId nCtx );
    void OnReceiveMsgChangeKnowledgeGroupAck  ( const ASN1T_MsgChangeGroupeConnaissanceAck& asnMsg, T_NetContextId nCtx );

    // pathfind and terrain type
    void OnReceiveMsgPawnPathFind             ( const ASN1T_MsgUnitPathFind& asnMsg            );
    void OnReceiveMsgPawnTerrainType          ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgDebugDrawPoints          ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );

    // communication/orders
    void OnReceiveMsgAutomateOrderAck         ( const ASN1T_MsgAutomateOrderAck& asnMsg, T_NetContextId nCtx );
    void OnReceiveMsgPionOrderAck             ( const ASN1T_MsgPionOrderAck&     asnMsg, T_NetContextId nCtx );
    //@}

private:
    //! @name Member data
    //@{
    DIN::DIN_MessageServiceUserCbk< MessageManager >*  pMessageService_;
    Workspace&                                         workspace_;
	
    // ASN
    ASN1OCTET        aASNEncodeBuffer_[100000];
    ASN1OCTET        aASNDecodeBuffer_[100000];
    //@}
};

} // end namespace TEST

#endif // __MessageManager_h_
