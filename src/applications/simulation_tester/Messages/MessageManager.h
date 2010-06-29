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
// Created: SBO 2005-05-09
// =============================================================================
class MessageManager
{
    MT_COPYNOTALLOWED( MessageManager );

public:
    //! @name DIN Messages
    //@{
    static enum
    {
        eMsgSimToClient       = 0,
        eMsgClientToSim       = 1,

        eMsgSimToMiddle       = 2,
        eMsgMiddleToSim       = 3,

        eMsgClientToMiddle    = 4,
        eMsgMiddleToClient    = 5
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
    void SendMsgInSim          ( MsgsClientToSim& asnMsg );
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
    void OnReceiveMsgOutSim                   ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );

    // Control
    void OnReceiveMsgControlInformation          ( const MsgControlInformation& asnMsg );
    void OnReceiveMsgControlBeginTick            ( const MsgControlBeginTick& asnMsg );
    void OnReceiveMsgControlEndTick              ( const MsgControlEndTick& asnMsg );
    void OnReceiveMsgControlChangeTimeFactorAck  ( const MsgControlChangeTimeFactorAck& asnMsg );
    void OnReceiveMsgControlSendCurrentStateBegin();
    void OnReceiveMsgControlSendCurrentStateEnd  ();

    //! @name Entities
    //@{
    void OnReceiveMsgAutomatCreation          ( const MsgAutomatCreation& asnMsg );
    void OnReceiveMsgPawnCreation             ( const MsgUnitCreation& asnMsg );
    void OnReceiveMsgUnitAttributes           ( const MsgUnitAttributes& asnMsg );

    void OnReceiveMsgObjectCreation           ( const MsgObjectCreation& asnMsg );
    void OnReceiveMsgObjectUpdate             ( const MsgObjectUpdate& asnMsg );
    void OnReceiveMsgObjectDestruction        ( const MsgObjectDestruction& asnMsg );

    void OnReceiveMsgPopulationCreation       ( const MsgPopulationCreation&    asnMsg );
    void OnReceiveMsgPopulationUpdate         ( const MsgPopulationUpdate&      asnMsg );

    void OnReceiveMsgPopulationConcentrationCreation   ( const MsgPopulationConcentrationCreation&    asnMsg );
    void OnReceiveMsgPopulationConcentrationUpdate     ( const MsgPopulationConcentrationUpdate&      asnMsg );
    void OnReceiveMsgPopulationConcentrationDestruction( const MsgPopulationConcentrationDestruction& asnMsg );

    void OnReceiveMsgPopulationFlowCreation            ( const MsgPopulationFlowCreation&             asnMsg );
    void OnReceiveMsgPopulationFlowUpdate              ( const MsgPopulationFlowUpdate&               asnMsg );
    void OnReceiveMsgPopulationFlowDestruction         ( const MsgPopulationFlowDestruction&          asnMsg );
    //@}

    //! @name Knowledge
    //@{
    // unit
    void OnReceiveMsgUnitKnowledgeCreation   ( const MsgUnitKnowledgeCreation&    asnMsg );
    void OnReceiveMsgUnitKnowledgeUpdate     ( const MsgUnitKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgUnitKnowledgeDestruction( const MsgUnitKnowledgeDestruction& asnMsg );

    // population
    void OnReceiveMsgPopulationKnowledgeCreation   ( const MsgPopulationKnowledgeCreation&    asnMsg );
    void OnReceiveMsgPopulationKnowledgeUpdate     ( const MsgPopulationKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgPopulationKnowledgeDestruction( const MsgPopulationKnowledgeDestruction& asnMsg );

    // object
    void OnReceiveMsgObjectKnowledgeCreation   ( const MsgObjectKnowledgeCreation& asnMsg );
    void OnReceiveMsgObjectKnowledgeUpdate     ( const MsgObjectKnowledgeUpdate& asnMsg );
    void OnReceiveMsgObjectKnowledgeDestruction( const MsgObjectKnowledgeDestruction& asnMsg );
    //@}

    //! @name Limits/Limas
    //@{
    void OnReceiveMsgLimitCreation            ( const MsgLimitCreation&    asnMsg );
    void OnReceiveMsgLimitDestruction         ( const MsgLimitDestruction& asnMsg );
    void OnReceiveMsgLimaCreation             ( const MsgLimaCreation&     asnMsg );
    void OnReceiveMsgLimaDestruction          ( const MsgLimaDestruction&  asnMsg );
    //@}

    // automat mode
    void OnReceiveMsgSetAutomatModeAck        ( const MsgSetAutomatModeAck& asnMsg, unsigned int nCtx );

    // Hierarchy changes
    void OnReceiveMsgChangeAutomat            ( const MsgChangeAutomate& asnMsg );
    void OnReceiveMsgChangeAutomateAck        ( const MsgChangeAutomateAck& asnMsg, unsigned int nCtx );
    void OnReceiveMsgChangeTeamRelation       ( const MsgChangeDiplomacy& asnMsg );
    void OnReceiveMsgChangeTeamRelationAck    ( const MsgChangeDiplomacyAck& asnMsg, unsigned int nCtx );
    void OnReceiveMsgChangeKnowledgeGroupAck  ( const MsgChangeGroupeConnaissanceAck& asnMsg, unsigned int nCtx );

    // pathfind and terrain type
    void OnReceiveMsgPawnPathFind             ( const MsgUnitPathFind& asnMsg            );

    // communication/orders
    void OnReceiveMsgAutomatOrderAck         ( const MsgAutomatOrderAck& asnMsg, unsigned int nCtx );
    void OnReceiveMsgUnitOrderAck             ( const MsgUnitOrderAck&     asnMsg, unsigned int nCtx );
    //@}

private:
    //! @name Member data
    //@{
    DIN::DIN_MessageServiceUserCbk< MessageManager >   messageService_;
    Workspace&                                         workspace_;

    bool bIsInitialized_; //$$$ ugly bDummy to handle MOS_Light connection after start and avoid re-lauching missions

    // ASN
    unsigned char    aASNEncodeBuffer_[100000];
    unsigned char    aASNDecodeBuffer_[100000];
    //@}
};

} // end namespace TEST

#endif // __MessageManager_h_
