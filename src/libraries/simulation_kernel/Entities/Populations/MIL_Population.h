// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Population_h_
#define __MIL_Population_h_

#include "Entities/MIL_Entity_ABC.h"
#include "Entities/MIL_VisitableEntity_ABC.h"
#include <tools/Set.h>
#include <boost/scoped_ptr.hpp>

namespace sword
{
    class DEC_Logger;
    class CrowdOrder;
    class FragOrder;
    class UnitMagicAction;
    class MissionParameters;
}

class DEC_PopulationDecision;
class DEC_PopulationKnowledge;
class MIL_AffinitiesMap;
class MIL_Agent_ABC;
class MIL_AgentPion;
class MIL_Army_ABC;
class MIL_Color;
class MIL_DictionaryExtensions;
class MIL_KnowledgeGroup;
class MIL_PopulationType;
class MIL_PopulationFlow;
class MIL_PopulationConcentration;
class MIL_PopulationElement_ABC;
class MIL_PopulationOrderManager;
class MIL_PopulationAttitude;
class PHY_Volume;
class PHY_FireResults_Population;
class TER_Localisation;
class MIL_UrbanObject_ABC;
class MissionController_ABC;

// =============================================================================
// Created: NLD 2005-09-28
// =============================================================================
class MIL_Population : public MIL_Entity_ABC
                     , public MIL_VisitableEntity_ABC< MIL_PopulationElement_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Population( const MIL_PopulationType& type, xml::xistream& xis, MIL_Army_ABC& army, MissionController_ABC& controller, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger* logger );
             MIL_Population( const MIL_PopulationType& type, MIL_Army_ABC& army, MissionController_ABC& controller, const MT_Vector2D& point, int number, const std::string& name, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger* logger, unsigned int context = 0 );
    virtual ~MIL_Population();
    //@}

    //! @name Accessors
    //@{
    const MIL_PopulationType& GetType() const;
    const DEC_PopulationDecision& GetDecision() const;
    DEC_PopulationDecision& GetDecision();
    const MIL_PopulationOrderManager& GetOrderManager() const;
    MIL_PopulationOrderManager& GetOrderManager();
    double GetMaxSpeed() const;
    double GetDefaultFlowDensity() const;
    const MIL_PopulationAttitude& GetDefaultAttitude() const;
    const MIL_PopulationAttitude& GetAttitude() const;
    virtual MIL_Army_ABC& GetArmy() const;
    const DEC_PopulationKnowledge& GetKnowledge() const;
    void SetKnowledgeGroup( boost::shared_ptr< MIL_KnowledgeGroup > pKnowledgeGroup );
    boost::shared_ptr< MIL_KnowledgeGroup > GetKnowledgeGroup() const;
    bool IsDead() const;
    bool HasDoneMagicMove() const;
    unsigned int GetAllHumans() const;
    unsigned int GetHealthyHumans() const;
    unsigned int GetWoundedHumans() const;
    unsigned int GetContaminatedHumans() const;
    unsigned int GetDeadHumans() const;
    unsigned int GetAllHumansInConcentration( unsigned int concentrationId );
    void HealWounded();
    double GetArmedIndividuals() const;
    void SetArmedIndividuals( double armedIndividuals );
    double GetNewArmedIndividuals() const;
    void SetNewArmedIndividuals( double armedIndividuals );
    bool HasFlow() const;
    const std::string& GetCriticalIntelligence() const;
    float GetAffinity( unsigned long teamID ) const;
    const tools::Set< MIL_Agent_ABC* >& GetHidden() const;
    virtual bool CanEmitReports() const;

    bool HasReachedBlockBorder          ( const MIL_UrbanObject_ABC* pUrbanKnowledge ) const;
    bool HasReachedDestination          ( const MT_Vector2D& destination ) const;
    bool HasReachedDestinationCompletely( const MT_Vector2D& destination ) const;
    //@}

    //! @name Geometry
    //@{
    bool IsInZone( const TER_Localisation& loc ) const;
    MT_Vector2D GetClosestPoint( const MT_Vector2D& refPos ) const;
    MT_Vector2D GetClosestPoint( const TER_Localisation& loc ) const;
    double GetDistanceTo( const TER_Localisation& loc ) const;
    boost::shared_ptr< MT_Vector2D > GetSecuringPoint( const MIL_Agent_ABC& securingAgent ) const;
    boost::shared_ptr< MT_Vector2D > GetSafetyPosition( const MIL_AgentPion& agent, double rMinDistance ) const;
    boost::shared_ptr< MT_Vector2D > GetBarycenter() const;
    boost::shared_ptr< MT_Vector2D > GetConcentrationPosition( unsigned int concentrationId ) const;
    MT_Vector2D GetFlowHeadPosition();
    //@}

    //! @name Effects on pions
    //@{
    double GetPionMaxSpeed( const MIL_PopulationAttitude& attitude, double rDensity, const PHY_Volume& pionVolume ) const;
    void SetPionMaxSpeed( double rSpeed );
    void ResetPionMaxSpeed();
    double GetPionReloadingTimeFactor( double rDensity ) const;
    //@}

    //! @name Effects from pions
    //@{
    void NotifyAttackedBy( const MIL_Agent_ABC& attacker );
    void Secure( const MIL_AgentPion& securer  );
    void Exterminate( const MIL_AgentPion& exterminator, double rSurface );
    void NotifyChanneled( const TER_Localisation& localisation );
    void NotifyUrbanDestructionStart();
    //@}

    //! @name Operations
    //@{
    void UpdateKnowledges();
    void CleanKnowledges();
    void UpdateDecision( float duration );
    void UpdateCrowdCollisions();
    void UpdateState();
    void Clean();
    void ClearObjectCollisions();

    void AddHidden( MIL_Agent_ABC& agent );
    void RemoveHidden( MIL_Agent_ABC& agent );
    //@}

    //! @name Actions
    //@{
    void Move( const MT_Vector2D& destination );
    void MoveAlong( const std::vector< boost::shared_ptr< MT_Vector2D > >& destination );
    void CancelMove();
    void FireOnPions( double rIntensity, PHY_FireResults_Population& fireResult );
    void FireOnPion( double rIntensity, MIL_Agent_ABC& target, PHY_FireResults_Population& fireResult );
    void Attack( float intensity );
    double GetDangerosity( const MIL_AgentPion& target ) const;
    void SetAttitude( const MIL_PopulationAttitude& attitude );
    void SetBlinded( bool blinded );
    bool IsBlinded() const;
    MIL_PopulationElement_ABC* GetClosestAliveElement( const MIL_Agent_ABC& reference ) const;
    MIL_PopulationConcentration* GetClosestConcentration( const MT_Vector2D& position, int maxDistance ) const;
    void ComputeClosestAliveElement( const MT_Vector2D& position, MIL_PopulationElement_ABC*& pClosestElement, double& rMinDistance ) const;
    double GetUrbanBlockAngriness() const;
    void SetUrbanBlockAngriness( double );
    void ChangeComposition( unsigned int healthy, unsigned int wounded, unsigned int contaminated, unsigned int dead );
    double ComputeUrbanBlocDestruction( MIL_UrbanObject_ABC* pUrbanObjet );
    void SetUrbanDestructionState( bool state );
    bool GetUrbanDestructionState();
    void SetDemonstrationState( bool state );
    bool GetDemonstrationState();
    //@}

    //! @name Types
    //@{
    typedef std::vector< MIL_PopulationConcentration* > T_ConcentrationVector;
    typedef T_ConcentrationVector::iterator            IT_ConcentrationVector;
    typedef T_ConcentrationVector::const_iterator     CIT_ConcentrationVector;
    //@}

    //! @name Tools
    //@{
    MIL_PopulationFlow& CreateFlow( MIL_PopulationConcentration& concentration );
    MIL_PopulationFlow& CreateFlow( const MIL_PopulationFlow& source, const MT_Vector2D& splitPoint );
    MIL_PopulationConcentration& GetConcentration( const MT_Vector2D& position );
    MIL_PopulationConcentration* RetrieveConcentration( unsigned int concentrationId ) const;
    //@}

    //! @name Network
    //@{
    uint32_t OnReceiveOrder( const sword::CrowdOrder& msg );
    void     OnReceiveFragOrder( const sword::FragOrder& msg, const std::function< void( uint32_t ) >& sendAck );
    void SendCreation( unsigned int context = 0 ) const;
    void SendFullState() const;
    void UpdateNetwork();
    void OnReceiveUnitMagicAction( const sword::UnitMagicAction& asnMsg );
    void OnReceiveCrowdMagicActionMoveTo( const sword::MissionParameters& msg );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Visitor
    //@{
    virtual void Apply( MIL_EntityVisitor_ABC< MIL_PopulationElement_ABC >& visitor ) const;
    virtual void Apply( MIL_EntitiesVisitor_ABC& visitor ) const;
    //@}

protected:
    //! @name Constructor
    //@{
    MIL_Population( const MIL_PopulationType& type, MissionController_ABC& controller );
    //@}

private:
    //! @name Magic actions
    //@{
    void OnReceiveMsgDestroyAll();
    void OnReceiveCriticalIntelligence( const sword::MissionParameters& msg );
    void OnReceiveMsgChangeHealthState( const sword::MissionParameters& asn );
    void OnReceiveMsgChangeArmedIndividuals( const sword::MissionParameters& asn );
    void OnReceiveMsgChangeAttitude( const sword::MissionParameters& asn );
    void OnReloadBrain( const sword::MissionParameters& msg );
    void OnChangeBrainDebug( const sword::MissionParameters& msg );
    //@}

    //! @name Helpers
    //@{
    void GetClosestPointAndDistance( const TER_Localisation& loc, MT_Vector2D& closestPoint, double& rMinDistance ) const;
    void UpdateBarycenter();
    bool HasHumansChanged() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< MIL_PopulationConcentration > > T_ConcentrationVector;
    typedef std::vector< boost::shared_ptr< MIL_PopulationFlow > > T_FlowVector;
    //@}

private:
    //! @name Member Data
    //@{
    const MIL_PopulationType*                       pType_;
    MIL_Army_ABC*                                   pArmy_;
    boost::shared_ptr< MIL_KnowledgeGroup >         pKnowledgeGroup_;
    const MIL_PopulationAttitude*                   pDefaultAttitude_;
    double                                          rArmedIndividuals_;
    double                                          rNewArmedIndividuals_;
    double                                          rMale_;
    double                                          rFemale_;
    double                                          rChildren_;
    std::string                                     criticalIntelligence_;
    T_ConcentrationVector                           concentrations_;
    T_FlowVector                                    flows_;
    T_ConcentrationVector                           trashedConcentrations_;
    T_FlowVector                                    trashedFlows_;
    boost::scoped_ptr< DEC_PopulationKnowledge >    pKnowledge_;
    boost::scoped_ptr< MIL_PopulationOrderManager > orderManager_;
    boost::shared_ptr< MT_Vector2D >                vBarycenter_;
    std::auto_ptr< MIL_Color >                      pColor_;
    // Pion effects
    double                                          rOverloadedPionMaxSpeed_;
    bool                                            bPionMaxSpeedOverloaded_;
    bool                                            bBlinded_;
    // Misc
    bool                                            bHasDoneMagicMove_;
    bool                                            criticalIntelligenceChanged_;
    bool                                            armedIndividualsChanged_;
    bool                                            isDamagingUrbanBlock_;
    bool                                            isDemonstrating_;
    boost::scoped_ptr< MIL_AffinitiesMap >          pAffinities_;
    boost::scoped_ptr< MIL_DictionaryExtensions >   pExtensions_;
    double                                          urbanBlockAngriness_;
    tools::Set< MIL_Agent_ABC* >                    hidden_;
    //@}

    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_Population* population, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_Population* population, const unsigned int /*version*/ );
};

BOOST_CLASS_EXPORT_KEY( MIL_Population )

#endif // __MIL_Population_h_
