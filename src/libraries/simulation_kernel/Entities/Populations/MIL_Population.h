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

#include "MIL.h"
#include "Entities/MIL_Entity_ABC.h"
#include "Entities/MIL_VisitableEntity_ABC.h"
#include "Entities/Orders/MIL_PopulationOrderManager.h"
#include <map>

namespace sword
{
    class CrowdOrder;
    class FragOrder;
    class UnitMagicAction;
    class MissionParameters;
}

namespace xml
{
    class xostream;
    class xistream;
}

class DEC_PopulationDecision;
class DEC_PopulationKnowledge;
class MIL_AffinitiesMap;
class MIL_Agent_ABC;
class MIL_AgentPion;
class MIL_Army_ABC;
class MIL_DictionaryExtensions;
class MIL_Formation;
class MIL_PopulationType;
class MIL_PopulationFlow;
class MIL_PopulationConcentration;
class MIL_PopulationElement_ABC;
class MIL_PopulationAttitude;
class PHY_Volume;
class PHY_FireResults_Population;
class TER_Localisation;
class UrbanObjectWrapper;

// =============================================================================
// Created: NLD 2005-09-28
// =============================================================================
class MIL_Population : public MIL_Entity_ABC
                     , public MIL_VisitableEntity_ABC< MIL_PopulationElement_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Population( xml::xistream& xis, const MIL_PopulationType& type, MIL_Army_ABC& army, unsigned int gcPause, unsigned int gcMult );
             MIL_Population( const MIL_PopulationType& type, MIL_Army_ABC& army, const MT_Vector2D& point, int number, const std::string& name, unsigned int gcPause, unsigned int gcMult, unsigned int context = 0 );
    virtual ~MIL_Population();
    //@}

    //! @name Accessors
    //@{
    unsigned int GetID() const;
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
    bool IsDead() const;
    bool HasDoneMagicMove() const;
    unsigned int GetAllHumans() const;
    unsigned int GetHealthyHumans() const;
    unsigned int GetWoundedHumans() const;
    unsigned int GetContaminatedHumans() const;
    unsigned int GetDeadHumans() const;
    unsigned int GetAllHumansInConcentration( unsigned int concentrationId );
    double GetArmedIndividuals() const;
    void SetArmedIndividuals( double armedIndividuals );
    double GetNewArmedIndividuals() const;
    void SetNewArmedIndividuals( double armedIndividuals );
    bool HasFlow() const;
    const std::string& GetCriticalIntelligence() const;
    float GetAffinity( unsigned long teamID ) const;

    bool HasReachedBlockBorder          ( const UrbanObjectWrapper* pUrbanKnowledge ) const;
    bool HasReachedDestination          ( const MT_Vector2D& destination ) const;
    bool HasReachedDestinationCompletely( const MT_Vector2D& destination ) const;
    //@}

    //! @name Geometry
    //@{
    bool IsInZone( const TER_Localisation& loc ) const;
    MT_Vector2D GetClosestPoint( const MT_Vector2D& refPos ) const;
    MT_Vector2D GetClosestPoint( const TER_Localisation& loc ) const;
    double GetDistanceTo( const TER_Localisation& loc ) const;
    MT_Vector2D GetSecuringPoint( const MIL_Agent_ABC& securingAgent ) const;
    MT_Vector2D GetSafetyPosition( const MIL_AgentPion& agent, double rMinDistance ) const;
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
    //@}

    //! @name Operations
    //@{
    void UpdateKnowledges();
    void CleanKnowledges();
    void UpdateDecision( float duration );
    void UpdateState();
    void Clean();
    //@}

    //! @name Actions
    //@{
    void Move( const MT_Vector2D& destination );
    void FireOnPions( double rIntensity, PHY_FireResults_Population& fireResult );
    void FireOnPion( double rIntensity, MIL_Agent_ABC& target, PHY_FireResults_Population& fireResult );
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
    double ComputeUrbanBlocDestruction( UrbanObjectWrapper* pUrbanObjet );
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
    void OnReceiveOrder( const sword::CrowdOrder& msg );
    void OnReceiveFragOrder( const sword::FragOrder& msg );
    void SendCreation( unsigned int context = 0 ) const;
    void SendFullState() const;
    void UpdateNetwork();
    void OnReceiveUnitMagicAction( const sword::UnitMagicAction& asnMsg );
    void OnReceiveCrowdMagicActionMoveTo( const sword::UnitMagicAction& asn );
    void OnReceiveCriticalIntelligence( const sword::UnitMagicAction& msg );
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
    //@}

protected:
    //! @name Constructor
    //@{
    MIL_Population( const MIL_PopulationType& type );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_Population( const MIL_Population& );            //!< Copy constructor
    MIL_Population& operator=( const MIL_Population& ); //!< Assignment operator
    //@}

    //! @name Magic actions
    //@{
    void OnReceiveMsgDestroyAll();
    void OnReceiveMsgChangeHealthState( const sword::UnitMagicAction& asn );
    void OnReceiveMsgChangeArmedIndividuals( const sword::UnitMagicAction& asn );
    void OnReceiveMsgChangeAttitude( const sword::UnitMagicAction& asn );
    void OnReloadBrain( const sword::MissionParameters& msg );
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
    typedef std::vector< MIL_PopulationConcentration* > T_ConcentrationVector;
    typedef T_ConcentrationVector::iterator            IT_ConcentrationVector;
    typedef T_ConcentrationVector::const_iterator     CIT_ConcentrationVector;

    typedef std::vector< MIL_PopulationFlow* > T_FlowVector;
    typedef T_FlowVector::iterator            IT_FlowVector;
    typedef T_FlowVector::const_iterator     CIT_FlowVector;
    //@}

private:
    //! @name Member Data
    //@{
    const MIL_PopulationType*                   pType_;
    const unsigned int                          nID_;
    MIL_Army_ABC*                               pArmy_;
    const MIL_PopulationAttitude*               pDefaultAttitude_;
    double                                      rArmedIndividuals_;
    double                                      rNewArmedIndividuals_;
    double                                      rMale_;
    double                                      rFemale_;
    double                                      rChildren_;
    std::string                                 criticalIntelligence_;
    T_ConcentrationVector                       concentrations_;
    T_FlowVector                                flows_;
    T_ConcentrationVector                       trashedConcentrations_;
    T_FlowVector                                trashedFlows_;
    DEC_PopulationKnowledge*                    pKnowledge_;
    MIL_PopulationOrderManager                  orderManager_;
    boost::shared_ptr< MT_Vector2D >            vBarycenter_;
    // Pion effects
    double                                      rOverloadedPionMaxSpeed_;
    bool                                        bPionMaxSpeedOverloaded_;
    bool                                        bBlinded_;
    // Misc
    bool                                        bHasDoneMagicMove_;
    bool                                        criticalIntelligenceChanged_;
    bool                                        armedIndividualsChanged_;
    std::auto_ptr< MIL_AffinitiesMap >          pAffinities_;
    std::auto_ptr< MIL_DictionaryExtensions >   pExtensions_;
    double                                      urbanBlockAngriness_;
    //@}

    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_Population* population, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_Population* population, const unsigned int /*version*/ );
};

BOOST_CLASS_EXPORT_KEY( MIL_Population )

#endif // __MIL_Population_h_
