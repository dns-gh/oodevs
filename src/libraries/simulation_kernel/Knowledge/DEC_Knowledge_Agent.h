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

#include "DEC_Knowledge_ABC.h"
#include "DEC_Knowledge_AgentDataDetection.h"
#include "DEC_Knowledge_AgentDataRecognition.h"
#include "DEC_Knowledge_AgentDataIdentification.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Tools/MIL_IDManager.h"
#include "Tristate.h"

namespace sword
{
    class UnitKnowledgeUpdate;
}

class MIL_Agent_ABC;
class MIL_Object_ABC;
class MIL_Army_ABC;
class MIL_KnowledgeGroup;
class MIL_Automate;
class PHY_PerceptionLevel;
class PHY_Posture;
class PHY_SensorTypeAgent;
class DEC_Knowledge_AgentPerception;
class PHY_NatureAtlas;
class UrbanObjectWrapper;

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
    static double rMaxDangerosityDegradationByRelevance_;
    static double rMaxDangerosityDegradationByOpState_;
    static double rMaxDangerosityDegradationByNeutralizedState_;

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_Agent( const MIL_KnowledgeGroup& knowledgeGroup, MIL_Agent_ABC& agentKnown, double relevance = 0.);
             DEC_Knowledge_Agent();
    virtual ~DEC_Knowledge_Agent();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Prepare();
    void Update( const DEC_Knowledge_AgentPerception& perception, int currentTimeStep );
    void Update( const DEC_Knowledge_Agent& knowledge, int currentTimeStep );
    void Extrapolate();
    bool Clean() const;
    void Invalidate();
    bool IsValid() const;
    void CopyFrom( const DEC_Knowledge_Agent& agent );
    void HackPerceptionLevel( const PHY_PerceptionLevel* pPerceptionLevel );
    //@}

    //! @name Relevance management
    //@{
    void UpdateRelevance( int currentTimeStep );
    double GetRelevance() const;
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork();
    void SendStateToNewClient();
    //@}

    //! @name Accessors
    //@{
    bool IsRefugee() const;
    bool IsMilitia() const;
    bool IsTerrorist() const;
    bool IsSurrendered() const;
    bool IsPrisoner() const;
    bool IsRefugeeManaged() const;
    bool IsDead() const;
    bool IsInCrowd() const;
    unsigned int GetID() const;
    double GetAltitude() const;
    const MT_Vector2D& GetPosition() const;
    const MT_Vector2D& GetDirection() const;
    double GetSpeed() const;
    double GetOperationalState() const;
    double GetMajorOperationalState() const;
    virtual  MIL_Agent_ABC& GetAgentKnown() const;
    const PHY_NatureAtlas& GetNatureAtlas() const;
    const MIL_Army_ABC* GetArmy() const;
    const PHY_PerceptionLevel& GetMaxPerceptionLevel() const;
    const PHY_PerceptionLevel& GetCurrentPerceptionLevel() const;
    const PHY_PerceptionLevel& GetCurrentPerceptionLevel( const MIL_Agent_ABC& pion ) const;
    const DEC_Knowledge_AgentComposante* GetMajorComposante() const;
    bool IsHuman() const; // $$$$ NLD 2007-04-19: A CHIER
    E_Tristate IsAnEnemy( const MIL_Army_ABC& army ) const;
    E_Tristate IsAFriend( const MIL_Army_ABC& army ) const;
    bool IsInUrbanBlock( const MIL_Object_ABC& urban ) const;
    const std::string& GetCriticalIntelligence() const;
    bool IsPerceptionDistanceHacked() const;
    //@}

    //! @name Perception
    //@{
    const PHY_Volume* GetSignificantVolume( const PHY_SensorTypeAgent& sensorType ) const;
    const PHY_Posture& GetLastPosture() const;
    const PHY_Posture& GetCurrentPosture() const;
    double GetPostureCompletionPercentage() const;
    double GetPopulationDensity() const;
    //@}

    //! @name Decisional operations
    //@{
    void SetCriticalIntelligenceFromAgentKnown();
    double GetDangerosity( const DEC_Knowledge_Agent& target ) const;
    double GetDangerosity( const MIL_Agent_ABC& target ) const;
    double GetMaxRangeToFireOn( const MIL_Agent_ABC& target, double rWantedPH ) const;
    double GetMaterialComposantesAttritionLevel( UrbanObjectWrapper* pUrbanBlock ) const;
    void Lock();
    void Unlock();
    void KillOfficers();
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< const MIL_Automate*, const PHY_PerceptionLevel* > T_PerceptionAutomateSourceMap;
    typedef T_PerceptionAutomateSourceMap::iterator                    IT_PerceptionAutomateSourceMap;
    typedef T_PerceptionAutomateSourceMap::const_iterator             CIT_PerceptionAutomateSourceMap;

    typedef std::map< const MIL_Agent_ABC*, const PHY_PerceptionLevel* > T_PerceptionAgentSourceMap;
    typedef T_PerceptionAgentSourceMap::iterator                        IT_PerceptionAgentSourceMap;
    typedef T_PerceptionAgentSourceMap::const_iterator                 CIT_PerceptionAgentSourceMap;
    //@}

private:
    //! @name Internal updaters
    //@{
    void UpdatePerceptionSources( const DEC_Knowledge_AgentPerception& perception );
    void UpdateCurrentPerceptionLevel( const PHY_PerceptionLevel& perceptionLevel );
    void UpdateMaxPerceptionLevel( const PHY_PerceptionLevel& perceptionLevel );
    //@}

    //! @name Internal network senders
    //@{
    void WriteMsgPerceptionSources( sword::UnitKnowledgeUpdate& asnMsg ) const;
    void SendMsgCreation() const;
    void SendMsgDestruction() const;
    void SendChangedState();
    void SendFullState();
    //@}

    //! @name Tools
    //@{
    void DegradeDangerosity( double& rDangerosity ) const;
    void ChangeRelevance( double rNewRelevance );
    //@}

private:
    //! @name Member data
    //@{
    const MIL_KnowledgeGroup* pKnowledgeGroup_;
    MIL_Agent_ABC* pAgentKnown_;
    const unsigned int nID_;
    DEC_Knowledge_AgentDataDetection dataDetection_;
    DEC_Knowledge_AgentDataRecognition dataRecognition_;
    DEC_Knowledge_AgentDataIdentification dataIdentification_;
    std::string criticalIntelligence_;
    // Internal attributes
    unsigned int nTimeLastUpdate_;
    const PHY_PerceptionLevel* pCurrentPerceptionLevel_;
    const PHY_PerceptionLevel* pPreviousPerceptionLevel_;
    const PHY_PerceptionLevel* pMaxPerceptionLevel_; // Niveau maximal de perception auquel l'unité a été vue
    T_PerceptionAutomateSourceMap perceptionLevelPerAutomateMap_;
    T_PerceptionAutomateSourceMap previousPerceptionLevelPerAutomateMap_;
    T_PerceptionAgentSourceMap perceptionLevelPerAgentMap_;
    double rRelevance_;
    int nTimeExtrapolationEnd_;
    bool bLocked_;
    bool bValid_;
    // Network
    bool bCreatedOnNetwork_;
    bool bRelevanceUpdated_;
    bool bCurrentPerceptionLevelUpdated_;
    bool bMaxPerceptionLevelUpdated_;
    bool bCriticalIntelligenceUpdated_;
    bool bPerceptionDistanceHacked_;
    double rLastRelevanceSent_;
    static MIL_IDManager idManager_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_Agent )

#endif // __DEC_Knowledge_Agent_h_
