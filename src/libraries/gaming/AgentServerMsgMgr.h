// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentServerMsgMgr_h_
#define __AgentServerMsgMgr_h_

#include "clients_kernel/Types.h"
#include "game_asn/Asn.h"

#include "DIN/DIN_Engine.h"
#include "DIN/DIN_Link.h"
#include "DIN/DIN_Input.h"
#include "DIN/MessageService/DIN_BufferedMessage.h"
#include "DIN/MessageService/DIN_MessageServiceUserCbk.h"
#include "DIN/ConnectionService/DIN_ConnectionServiceServerUserCbk.h"
#include "DIN/ConnectionService/DIN_ConnectionServiceClientUserCbk.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "Publisher_ABC.h"

#include "boost/thread/mutex.hpp"

namespace kernel
{
    class Agent_ABC;
    class Controllers;
}

class AgentServerController;
class MsgRecorder;
class Model;
class Simulation;
class DIN_InputDeepCopy;
class Profile;

namespace DIN
{
	template < class T > class DIN_MessageServiceUserCbk;
}

//=============================================================================
// Created: NLD 2002-07-12
//=============================================================================
class AgentServerMsgMgr : public kernel::Observer_ABC
                        , public kernel::OptionsObserver_ABC
                        , public Publisher_ABC
{
   
public:
    //! @name Constructor/Destructor
    //@{
             AgentServerMsgMgr( kernel::Controllers& controllers, DIN::DIN_Engine& engine, Simulation& simu, Profile& profile, boost::mutex& mutex ); 
    virtual ~AgentServerMsgMgr();
    //@}

    //! @name Operations
    //@{
    bool IsPaused() const;
    void RegisterMessageRecorder( MsgRecorder& recorder );
    void UnregisterMessageRecorder( MsgRecorder& recorder );

    void Enable( DIN::DIN_Link& session );
    void DoUpdate();
    void Flush();
    void Disconnect();

    virtual void Send( ASN1T_MsgsOutClient& message );
    virtual void SendMagicDestruction( const kernel::Entity_ABC& agent );

    DIN::DIN_BufferedMessage BuildMessage();
    void SendMsgOutClient           ( ASN1OCTET* pMsg, int nMsgLength );
    void SendMsgUnitMagicActionDestroyComposante( const kernel::Agent_ABC& agent );

    void SetModel( Model& model );
    //@}

private:
    //! @name Service callback
    //@{
    bool OnError( DIN::DIN_Link &link, const DIN::DIN_ErrorDescription& info );
    //@}

    //! @name Message callbacks
    //@{
    // Tests
    void OnReceiveMsgProfilingValues( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );

    // Debug
    void OnReceiveMsgUnitVisionCones                       ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgTrace                                 ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgUnitInterVisibility                   ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgObjectInterVisibility                 ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgPopulationConcentrationInterVisibility( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgPopulationFlowInterVisibility         ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgKnowledgeGroup                        ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgDebugDrawPoints                       ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgPopulationCollision                   ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );

    // ASN
    void OnReceiveMsgInClient           ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );

    void _OnReceiveMsgProfilingValues                       ( DIN::DIN_Input& input );
    void _OnReceiveMsgUnitVisionCones                       ( DIN::DIN_Input& input );
    void _OnReceiveMsgTrace                                 ( DIN::DIN_Input& input );
    void _OnReceiveMsgUnitInterVisibility                   ( DIN::DIN_Input& input );
    void _OnReceiveMsgObjectInterVisibility                 ( DIN::DIN_Input& input );
    void _OnReceiveMsgPopulationConcentrationInterVisibility( DIN::DIN_Input& input );
    void _OnReceiveMsgPopulationFlowInterVisibility         ( DIN::DIN_Input& input );
    void _OnReceiveMsgKnowledgeGroup                        ( DIN::DIN_Input& input );
    void _OnReceiveMsgDebugDrawPoints                       ( DIN::DIN_Input& input );
    void _OnReceiveMsgPopulationCollision                   ( DIN::DIN_Input& input );
    void _OnReceiveMsgInClient                              ( DIN::DIN_Input& input );    

    // Control
    void OnReceiveMsgCtrlPauseAck             ( const ASN1T_MsgCtrlPauseAck&  asnMsg );
    void OnReceiveMsgCtrlResumeAck            ( const ASN1T_MsgCtrlResumeAck& asnMsg );
    void OnReceiveMsgCtrlChangeTimeFactorAck  ( const ASN1T_MsgCtrlChangeTimeFactorAck& asnMsg );
    void OnReceiveMsgCtrlInfo                 ( const ASN1T_MsgCtrlInfo& asnMsg );
    void OnReceiveMsgCtrlMeteoGlobalAck       ();
    void OnReceiveMsgCtrlMeteoLocalAck        ();
    void OnReceiveMsgCheckPointSaveBegin      ();
    void OnReceiveMsgCheckPointSaveEnd        ();
    void OnReceiveMsgCheckPointLoadBegin      ();
    void OnReceiveMsgCheckPointLoadEnd        ();
    void OnReceiveMsgCheckPointSetFrequencyAck();
    void OnReceiveMsgCheckPointSaveNowAck     ();
    void OnReceiveMsgCtrlSendCurrentStateBegin();
    void OnReceiveMsgCtrlSendCurrentStateEnd  ();
    void OnReceiveMsgCtrlBeginTick            ( const ASN1T_MsgCtrlBeginTick& asnMsg );
    void OnReceiveMsgCtrlEndTick              ( const ASN1T_MsgCtrlEndTick& asnMsg );
    void OnReceiveMsgAutomateCreation         ( const ASN1T_MsgAutomateCreation& asnMsg );
    void OnReceiveMsgPionCreation             ( const ASN1T_MsgPionCreation& asnMsg );
    void OnReceiveMsgChangeDiplomatie         ( const ASN1T_MsgChangeDiplomatie& asnMsg );
    void OnReceiveMsgAuthenticationResponse   ( const ASN1T_MsgAuthenticationResponse& asnMsg );

    // Profiles
    void OnReceiveMsgProfileCreation             ( const ASN1T_MsgProfileCreation& message );
    void OnReceiveMsgProfileCreationRequestAck   ( const ASN1T_MsgProfileCreationRequestAck& message );
    void OnReceiveMsgProfileDestruction          ( const ASN1T_MsgProfileDestruction& message );
    void OnReceiveMsgProfileDestructionRequestAck( const ASN1T_MsgProfileDestructionRequestAck& message );
    void OnReceiveMsgProfileUpdate               ( const ASN1T_MsgProfileUpdate& message );
    void OnReceiveMsgProfileUpdateRequestAck     ( const ASN1T_MsgProfileUpdateRequestAck& message );

    // Side/Formation/Knowledge Group creation
    void OnReveiveMsgKnowledgeGroupCreation   ( const ASN1T_MsgKnowledgeGroupCreation& asnMsg );
    void OnReveiveMsgSideCreation             ( const ASN1T_MsgSideCreation& asnMsg );
    void OnReveiveMsgFormationCreation        ( const ASN1T_MsgFormationCreation& asnMsg );

    // Attributes
    void OnReceiveMsgUnitAttributes    ( const ASN1T_MsgUnitAttributes& asnMsg );
    void OnReceiveMsgUnitMagicActionAck( const ASN1T_MsgUnitMagicActionAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgUnitPathFind      ( const ASN1T_MsgUnitPathFind& asnMsg );
    void OnReceiveMsgAutomateAttributes( const ASN1T_MsgAutomateAttributes& asnMsg );

    // Log maintenance
    void OnReceiveMsgLogMaintenanceTraitementEquipementCreation   ( const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& asnMsg );
    void OnReceiveMsgLogMaintenanceTraitementEquipementDestruction( const ASN1T_MsgLogMaintenanceTraitementEquipementDestruction& asnMsg );
    void OnReceiveMsgLogMaintenanceTraitementEquipementUpdate     ( const ASN1T_MsgLogMaintenanceTraitementEquipementUpdate& asnMsg );
    void OnReceiveMsgLogMaintenanceEtat                           ( const ASN1T_MsgLogMaintenanceEtat&                  asnMsg );

    // Log sante
    void OnReceiveMsgLogSanteTraitementHumainCreation   ( const ASN1T_MsgLogSanteTraitementHumainCreation& asnMsg );
    void OnReceiveMsgLogSanteTraitementHumainDestruction( const ASN1T_MsgLogSanteTraitementHumainDestruction& asnMsg );
    void OnReceiveMsgLogSanteTraitementHumainUpdate     ( const ASN1T_MsgLogSanteTraitementHumainUpdate& asnMsg );
    void OnReceiveMsgLogSanteEtat                       ( const ASN1T_MsgLogSanteEtat&                  asnMsg );

    // Log supply
    void OnReceiveMsgLogRavitaillementTraitementCreation   ( const ASN1T_MsgLogRavitaillementTraitementCreation& asnMsg );
    void OnReceiveMsgLogRavitaillementTraitementDestruction( const ASN1T_MsgLogRavitaillementTraitementDestruction& asnMsg );
    void OnReceiveMsgLogRavitaillementTraitementUpdate     ( const ASN1T_MsgLogRavitaillementTraitementUpdate& asnMsg );
    void OnReceiveMsgLogRavitaillementEtat                 ( const ASN1T_MsgLogRavitaillementEtat&  asnMsg );
    void OnReceiveMsgLogRavitaillementQuotas               ( const ASN1T_MsgLogRavitaillementQuotas& asnMsg );
    void OnReceiveMsgLogRavitaillementChangeQuotaAck       ( const ASN1T_MsgLogRavitaillementChangeQuotasAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgLogRavitaillementPousserFluxAck       ( const ASN1T_MsgLogRavitaillementPousserFluxAck& asnMsg, unsigned long nCtx );

    // Limas / Limits
    void OnReceiveMsgLimitCreationRequestAck   ( const ASN1T_MsgLimitCreationRequestAck&    asnMsg, unsigned long nCtx );
    void OnReceiveMsgLimitUpdateRequestAck     ( const ASN1T_MsgLimitUpdateRequestAck&      asnMsg, unsigned long nCtx );
    void OnReceiveMsgLimitDestructionRequestAck( const ASN1T_MsgLimitDestructionRequestAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgLimaCreationRequestAck    ( const ASN1T_MsgLimaCreationRequestAck&    asnMsg, unsigned long nCtx );
    void OnReceiveMsgLimaUpdateRequestAck      ( const ASN1T_MsgLimaUpdateRequestAck&      asnMsg, unsigned long nCtx );
    void OnReceiveMsgLimaDestructionRequestAck ( const ASN1T_MsgLimaDestructionRequestAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgLimitCreation      ( const ASN1T_MsgLimitCreation& asnMsg );
    void OnReceiveMsgLimitUpdate        ( const ASN1T_MsgLimitUpdate& asnMsg );
    void OnReceiveMsgLimitDestruction   ( const ASN1T_MsgLimitDestruction& asnMsg );
    void OnReceiveMsgLimaCreation       ( const ASN1T_MsgLimaCreation& asnMsg );
    void OnReceiveMsgLimaUpdate         ( const ASN1T_MsgLimaUpdate& asnMsg );
    void OnReceiveMsgLimaDestruction    ( const ASN1T_MsgLimaDestruction& asnMsg );

    // Object
    void OnReceiveMsgObjectCreation      ( const ASN1T_MsgObjectCreation& asnMsg );
    void OnReceiveMsgObjectUpdate        ( const ASN1T_MsgObjectUpdate& asnMsg );
    void OnReceiveMsgObjectDestruction   ( const ASN1T_MsgObjectDestruction& asnMsg );
    void OnReceiveMsgObjectMagicActionAck( const ASN1T_MsgObjectMagicActionAck& asnMsg, unsigned long nCtx );

    // Knowledge unit
    void OnReceiveMsgUnitKnowledgeCreation   ( const ASN1T_MsgUnitKnowledgeCreation&    asnMsg );
    void OnReceiveMsgUnitKnowledgeUpdate     ( const ASN1T_MsgUnitKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgUnitKnowledgeDestruction( const ASN1T_MsgUnitKnowledgeDestruction& asnMsg );

    // Knowledge object
    void OnReceiveMsgObjectKnowledgeCreation   ( const ASN1T_MsgObjectKnowledgeCreation&    asnMsg );
    void OnReceiveMsgObjectKnowledgeUpdate     ( const ASN1T_MsgObjectKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgObjectKnowledgeDestruction( const ASN1T_MsgObjectKnowledgeDestruction& asnMsg );

    // Knowledge population
    void OnReceiveMsgPopulationKnowledgeCreation                ( const ASN1T_MsgPopulationKnowledgeCreation&                 asnMsg );
    void OnReceiveMsgPopulationKnowledgeUpdate                  ( const ASN1T_MsgPopulationKnowledgeUpdate&                   asnMsg );
    void OnReceiveMsgPopulationKnowledgeDestruction             ( const ASN1T_MsgPopulationKnowledgeDestruction&              asnMsg );
    void OnReceiveMsgPopulationConcentrationKnowledgeCreation   ( const ASN1T_MsgPopulationConcentrationKnowledgeCreation&    asnMsg );
    void OnReceiveMsgPopulationConcentrationKnowledgeUpdate     ( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgPopulationConcentrationKnowledgeDestruction( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& asnMsg );
    void OnReceiveMsgPopulationFlowKnowledgeCreation            ( const ASN1T_MsgPopulationFluxKnowledgeCreation&             asnMsg );
    void OnReceiveMsgPopulationFlowKnowledgeUpdate              ( const ASN1T_MsgPopulationFluxKnowledgeUpdate&               asnMsg );
    void OnReceiveMsgPopulationFlowKnowledgeDestruction         ( const ASN1T_MsgPopulationFluxKnowledgeDestruction&          asnMsg );

    // Orders
    void OnReceiveMsgAutomateOrder          ( const ASN1T_MsgAutomateOrder& asnMsg ); 
    void OnReceiveMsgAutomateOrderAck       ( const ASN1T_MsgAutomateOrderAck& asnMsg, unsigned long nCtx ); 
    void OnReceiveMsgAutomateOrderManagement( const ASN1T_MsgAutomateOrderManagement& asnMsg );
    void OnReceiveMsgPionOrder              ( const ASN1T_MsgPionOrder& asnMsg ); 
    void OnReceiveMsgPionOrderAck           ( const ASN1T_MsgPionOrderAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgPionOrderManagement    ( const ASN1T_MsgPionOrderManagement& asnMsg );
    void OnReceiveMsgFragOrderAck           ( const ASN1T_MsgFragOrderAck& asnMsg, unsigned long nCtx ); 

    // Automate mode
    void OnReceiveMsgSetAutomateModeAck( const ASN1T_MsgSetAutomateModeAck& asnMsg, unsigned long nCtx );
    
    // Hierachy changes
    void OnReceiveMsgChangeAutomateAck          ( const ASN1T_MsgChangeAutomateAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgChangeAutomate             ( const ASN1T_MsgChangeAutomate& asnMsg );
    void OnReceiveMsgChangeDiplomacyAck         ( const ASN1T_MsgChangeDiplomatieAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgChangeGroupeConnaissanceAck( const ASN1T_MsgChangeGroupeConnaissanceAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgChangeLiensLogistiques     ( const ASN1T_MsgChangeLiensLogistiques& asnMsg );
    void OnReceiveMsgChangeLiensLogistiquesAck  ( const ASN1T_MsgChangeLiensLogistiquesAck& asnMsg, unsigned long nCtx );

    // RC
    void OnReceiveMsgCR( const ASN1T_MsgCR& asnMsg );

    // Tirs
    void OnReceiveMsgStartPionFire         ( const ASN1T_MsgStartPionFire&       asnMsg );
    void OnReceiveMsgStopPionFire          ( const ASN1T_MsgStopPionFire&        asnMsg );
    void OnReceiveMsgExplosion             ( const ASN1T_MsgExplosion&           asnMsg );
    void OnReceiveMsgStartFireEffect       ( const ASN1T_MsgStartFireEffect&     asnMsg );
    void OnReceiveMsgStopFireEffect        ( const ASN1T_MsgStopFireEffect&      asnMsg );
    void OnReceiveMsgStartPopulationFire   ( const ASN1T_MsgStartPopulationFire& asnMsg );
    void OnReceiveMsgStopPopulationFire    ( const ASN1T_MsgStopPopulationFire&  asnMsg );
	
	// Population
	void OnMsgPopulationCreation				( const ASN1T_MsgPopulationCreation& asnMsg );
	void OnMsgPopulationUpdate					( const ASN1T_MsgPopulationUpdate& asnMsg ); 
	void OnMsgPopulationConcentrationCreation	( const ASN1T_MsgPopulationConcentrationCreation& asnMsg ); 
	void OnMsgPopulationConcentrationDestruction( const ASN1T_MsgPopulationConcentrationDestruction& asnMsg );
	void OnMsgPopulationConcentrationUpdate		( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg );
	void OnMsgPopulationFluxCreation			( const ASN1T_MsgPopulationFluxCreation& asnMsg );
	void OnMsgPopulationFluxDestruction			( const ASN1T_MsgPopulationFluxDestruction& asnMsg ); 
	void OnMsgPopulationFluxUpdate				( const ASN1T_MsgPopulationFluxUpdate& asnMsg ); 
    void OnReceiveMsgPopulationMagicActionAck   ( const ASN1T_MsgPopulationMagicActionAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgPopulationOrderManagement  ( const ASN1T_MsgPopulationOrderManagement& asnMsg );
    void OnReceiveMsgPopulationOrderAck         ( const ASN1T_MsgPopulationOrderAck& asnMsg, unsigned long nCtx );
    void OnReceiveMsgPopulationOrder            ( const ASN1T_MsgPopulationOrder& asnMsg ); 
    //@}

    //! @name Helpers
    //@{
    void Send( unsigned int id, DIN::DIN_BufferedMessage& message );
    void Send( unsigned int id );
    typedef void ( AgentServerMsgMgr::* T_Callback ) ( DIN::DIN_Input& input );
    void Enqueue( DIN::DIN_Input& input, T_Callback function );

    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    void ToggleVisionCones();

    Model& GetModel() const;
    //@}

    //! @name Copy / Assignment
    //@{
    AgentServerMsgMgr( const AgentServerMsgMgr& );
    AgentServerMsgMgr& operator=( const AgentServerMsgMgr& );
    //@}

    //! @name Types
    //@{
    typedef std::vector< DIN_InputDeepCopy* > T_Inputs;
    typedef T_Inputs::const_iterator        CIT_Inputs;
    //@}
    
private:
    kernel::Controllers& controllers_;
    Model*               model_;
    Simulation&          simulation_;
    Profile&             profile_;
    boost::mutex&        mutex_;
    MsgRecorder*         msgRecorder_;

    DIN::DIN_Link* session_;
    DIN::DIN_MessageServiceUserCbk<AgentServerMsgMgr>*  pMessageService_;

    // ASN
    ASN1OCTET           aASNEncodeBuffer_[100000];
    ASN1OCTET           aASNDecodeBuffer_[100000];

    boost::mutex inputMutex_;
    T_Inputs workingInputs_; // network thread only
    T_Inputs buffer_;        // shared
    T_Inputs pendingInputs_; // main thread only

    bool needsVisionCones_, needsVisionSurfaces_;
};

#endif // __AgentServerMsgMgr_h_

