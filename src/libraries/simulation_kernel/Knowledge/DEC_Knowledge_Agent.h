// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_Agent.h $
// $Author: Nld $
// $Modtime: 21/04/05 19:01 $
// $Revision: 16 $
// $Workfile: DEC_Knowledge_Agent.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_Agent_h_
#define __DEC_Knowledge_Agent_h_

#include "MIL.h"
#include "DEC_Knowledge_ABC.h"
#include "DEC_Knowledge_AgentDataDetection.h"
#include "DEC_Knowledge_AgentDataRecognition.h"
#include "DEC_Knowledge_AgentDataIdentification.h"
#include "game_asn/Simulation.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"

class MIL_AgentPion;
class MIL_Agent_ABC;
class MIL_KnowledgeGroup;
class MIL_Automate;
class MIL_Army_ABC;
class PHY_PerceptionLevel;
class PHY_Posture;
class PHY_SensorTypeAgent;
class DEC_Knowledge_AgentPerception;
class PHY_NatureAtlas;

// =============================================================================
/** @class  DEC_Knowledge_Agent
    @brief  This class represents the knowledge of an agent 
*/
// Created: NLD 2004-03-11
// Modified: JVT 2004-12-09
// =============================================================================
class DEC_Knowledge_Agent : public DEC_Knowledge_ABC
{

public:
    // Parameters - $$$ Changer - ne devrait pas être public
    static MT_Float rMaxDangerosityDegradationByRelevance_;
    static MT_Float rMaxDangerosityDegradationByOpState_;
    static MT_Float rMaxDangerosityDegradationByNeutralizedState_;

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_Agent( const MIL_KnowledgeGroup& knowledgeGroup, MIL_Agent_ABC& agentKnown );
             DEC_Knowledge_Agent();
    virtual ~DEC_Knowledge_Agent();
    //@}
    
    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    void Prepare    ();
    void Update     ( const DEC_Knowledge_AgentPerception& perception );
    void Update     ( const DEC_Knowledge_Agent& knowledge );
    void Extrapolate();
    bool Clean      () const;
    void Invalidate ();
    bool IsValid    () const;
    //@}

    //! @name Relevance management
    //@{
    void     UpdateRelevance();
    MT_Float GetRelevance   () const;
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork     ();
    void SendStateToNewClient();
    //@}

    //! @name Accessors
    //@{
          bool                           IsRefugee                () const;
          bool                           IsMilitia                () const;
          bool                           IsTerrorist              () const;
          bool                           IsSurrendered            () const;
          bool                           IsPrisoner               () const;
          bool                           IsRefugeeManaged         () const;
          bool                           IsDead                   () const;
          uint                           GetID                    () const;
          MT_Float                       GetAltitude              () const;
    const MT_Vector2D&                   GetPosition              () const;
    const MT_Vector2D&                   GetDirection             () const;
          MT_Float                       GetSpeed                 () const;
          MT_Float                       GetOperationalState      () const;
          MT_Float                       GetMajorOperationalState () const;
          MIL_Agent_ABC&                 GetAgentKnown            () const;
    const PHY_NatureAtlas&               GetNatureAtlas           () const;
    const MIL_Army_ABC*                  GetArmy                  () const;
    const PHY_PerceptionLevel&           GetMaxPerceptionLevel    () const;
    const PHY_PerceptionLevel&           GetCurrentPerceptionLevel() const;
    const DEC_Knowledge_AgentComposante* GetMajorComposante       () const;
          bool                           IsHuman                  () const; // $$$$ NLD 2007-04-19: A CHIER
    E_Tristate                           IsAnEnemy                ( const MIL_Army_ABC& army ) const;
    E_Tristate                           IsAFriend                ( const MIL_Army_ABC& army ) const;
    //@}

    //! @name Perception
    //@{
    const PHY_Volume*  GetSignificantVolume          ( const PHY_SensorTypeAgent& sensorType ) const;
    const PHY_Posture& GetLastPosture                () const;
    const PHY_Posture& GetCurrentPosture             () const;
    MT_Float           GetPostureCompletionPercentage() const;
    //@}

    //! @name Decisional operations
    //@{
    MT_Float GetDangerosity     ( const DEC_Knowledge_Agent& target ) const;
    MT_Float GetDangerosity     ( const MIL_AgentPion& target ) const;
    MT_Float GetMaxRangeToFireOn( const MIL_AgentPion& target, MT_Float rWantedPH ) const;
    void     Lock               ();
    void     Unlock             ();
    void     KillOfficers       ();
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< const MIL_Automate*, const PHY_PerceptionLevel* > T_PerceptionSourceMap;
    typedef T_PerceptionSourceMap::iterator                             IT_PerceptionSourceMap;
    typedef T_PerceptionSourceMap::const_iterator                       CIT_PerceptionSourceMap;
    //@}

private:
    //! @name Internal updaters
    //@{
    void UpdatePerceptionSources     ( const DEC_Knowledge_AgentPerception& perception );
    void UpdateCurrentPerceptionLevel( const PHY_PerceptionLevel& perceptionLevel );
    void UpdateMaxPerceptionLevel    ( const PHY_PerceptionLevel& perceptionLevel );
    //@}

    //! @name Internal network senders
    //@{
    void WriteMsgPerceptionSources( ASN1T_MsgUnitKnowledgeUpdate& asnMsg ) const;

    void SendMsgCreation   () const;
    void SendMsgDestruction() const;
    void SendChangedState  ();
    void SendFullState     ();
    //@}

    //! @name Tools
    //@{
    void DegradeDangerosity( MT_Float& rDangerosity ) const;
    void ChangeRelevance   ( MT_Float rNewRelevance );
    //@}

private:
    const MIL_KnowledgeGroup* pKnowledgeGroup_;
          MIL_Agent_ABC*      pAgentKnown_;
    const uint                nID_;
         
    DEC_Knowledge_AgentDataDetection      dataDetection_;
    DEC_Knowledge_AgentDataRecognition    dataRecognition_;
    DEC_Knowledge_AgentDataIdentification dataIdentification_;

    // Internal attributes
          uint                  nTimeLastUpdate_;
    const PHY_PerceptionLevel*  pCurrentPerceptionLevel_;
    const PHY_PerceptionLevel*  pPreviousPerceptionLevel_;
    const PHY_PerceptionLevel*  pMaxPerceptionLevel_;        // Niveau maximal de perception auquel l'unité a été vue
          T_PerceptionSourceMap perceptionLevelPerAutomateMap_;
          T_PerceptionSourceMap previousPerceptionLevelPerAutomateMap_;
          MT_Float              rRelevance_;
          uint                  nTimeExtrapolationEnd_;
          bool                  bLocked_;
          bool                  bValid_;

    // Network
    bool     bCreatedOnNetwork_;
    bool     bRelevanceUpdated_;
    bool     bCurrentPerceptionLevelUpdated_;
    bool     bMaxPerceptionLevelUpdated_;
    MT_Float rLastRelevanceSent_;
};

#endif // __DEC_Knowledge_Agent_h_
