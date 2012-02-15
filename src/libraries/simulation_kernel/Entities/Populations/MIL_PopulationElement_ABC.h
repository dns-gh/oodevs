// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_PopulationElement_ABC_h_
#define __MIL_PopulationElement_ABC_h_

#include "MIL.h"
#include "MIL_PopulationHumans.h"
#include <boost/shared_ptr.hpp>
#include <map>

class AttritionCapacity;
class MIL_Agent_ABC;
class MIL_AgentPion;
class MIL_Object_ABC;
class MIL_BurnEffectManipulator;
class MIL_Population;
class MIL_PopulationAttitude;
class MIL_PopulationType;
class MIL_IntoxicationEffect;
class MIL_ContaminationEffect;
class MIL_DecontaminationEffect;
class MT_Circle;
class PHY_FireResults_ABC;
class PHY_FireResults_Population;
class PHY_Volume;
class TER_Localisation;
class TER_PopulationConcentration_ABC;
class TER_PopulationFlow_ABC;
class MIL_NbcAgentType;

// =============================================================================
// Created: NLD 2005-09-28
// =============================================================================
class MIL_PopulationElement_ABC : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::vector< MIL_Agent_ABC* > T_AgentVector;
    typedef std::vector< MIL_Object_ABC* > T_ObjectVector;
    typedef std::vector< TER_PopulationConcentration_ABC* > T_PopulationConcentrationVector;
    typedef std::vector< TER_PopulationFlow_ABC* > T_PopulationFlowVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MIL_PopulationElement_ABC();
             MIL_PopulationElement_ABC( MIL_Population& population, unsigned int nID );
    virtual ~MIL_PopulationElement_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Clean();
    //@}

    //! @name Modifiers
    //@{
    void SetAttitude( const MIL_PopulationAttitude& attitude );
    //@}

    //! @name Humans management
    //@{
    void PushHumans( const MIL_PopulationHumans& humans );
    MIL_PopulationHumans PullHumans( unsigned int nNbr );
    void KillAllHumans();
    void KillHumans( unsigned int humans );
    void WoundHumans( unsigned int humans );
    void ReintegrateUrbanBlock();
    double Exterminate( double rSurface );
    //@}

    //! @name Actions
    //@{
    void FireOnPions( double rIntensity, PHY_FireResults_Population& fireResult );
    void FireOnPion( double rIntensity, MIL_Agent_ABC& target, PHY_FireResults_Population& fireResult );
    double GetDangerosity( const MIL_AgentPion& target ) const;
    void ApplyFire( unsigned int nNbrAmmoFired, PHY_FireResults_ABC& fireResult, bool lethal );
    void ApplyIndirectFire( const MT_Circle& attritionCircle, PHY_FireResults_ABC& fireResult );
    void ApplyExplosion( const AttritionCapacity& capacity, PHY_FireResults_ABC& fireResult );
    void ApplyBurn( const MIL_BurnEffectManipulator& burn );
    void ApplyContamination( const MIL_NbcAgentType& type );
    void ApplyIntoxication( const MIL_NbcAgentType& type );
    void ApplyDecontamination( double rRatioWorkers );
    void NotifyUrbanDestructionStart();
    void NotifyObjectCollision( MIL_Object_ABC& object );
    void ClearObjectCollisions();
    void Attack( MIL_PopulationElement_ABC& element, float intensity );
    //@}

    //! @name Accessors
    //@{
    MIL_Population& GetPopulation() const;
    const MIL_PopulationAttitude& GetAttitude() const;
    unsigned int GetID() const;
    bool HasHumansChanged() const;
    unsigned int GetAllHumans() const;
    unsigned int GetTotalLivingHumans() const;
    unsigned int GetHealthyHumans() const;
    unsigned int GetWoundedHumans() const;
    unsigned int GetContaminatedHumans() const;
    unsigned int GetDeadHumans() const;
    double GetDensity() const;
    bool IsDead() const;
    bool IsInZone( const TER_Localisation& loc ) const;
    const T_AgentVector& GetCollidingAgents() const;
    const T_ObjectVector& GetCollidingObjects() const;
    const T_PopulationFlowVector& GetCollidingPopulationFlows() const;
    const T_PopulationConcentrationVector& GetCollidingPopulationConcentrations() const;

    virtual const TER_Localisation& GetLocation() const = 0;
    virtual boost::shared_ptr< MT_Vector2D > GetSecuringPoint( const MIL_Agent_ABC& securingAgent ) const;
    virtual boost::shared_ptr< MT_Vector2D > GetSafetyPosition( const MIL_AgentPion& agent, double rMinDistance, double rSeed ) const = 0;
    virtual double GetDefaultDensity( const MIL_PopulationType& type ) const = 0;
    virtual bool CanBePerceived() const = 0;
    virtual bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius, std::vector< MT_Vector2D >& shape ) const = 0;
    //@}

    //! @name Pion effects
    //@{
    double GetPionMaxSpeed( const PHY_Volume& pionVolume ) const;
    double GetPionReloadingTimeFactor() const;
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

protected:
    //! @name Tools
    //@{
    void UpdateDensity();
    void UpdateCollisions();
    //@}

    //! @name Network
    //@{
    bool HasAttitudeChanged() const;
    //@}

    //! @name Events
    //@{
    virtual void NotifyCollision( MIL_Agent_ABC& agent ) = 0;
    //@}

private:
    //! @name Helpers
    //@{
    void ApplyLethalDamage( unsigned int nDead, PHY_FireResults_ABC& fireResult );
    void UpdateCollidingAttackingAgents( MIL_Agent_ABC& target );
    void ClearCollidingAttackingAgents();
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< MIL_IntoxicationEffect > T_IntoxicationEffect;
    typedef std::map< unsigned int, T_IntoxicationEffect > T_IntoxicationEffects;

    typedef boost::shared_ptr< MIL_ContaminationEffect > T_ContaminationEffect;
    typedef std::map< unsigned int, T_ContaminationEffect > T_ContaminationEffects;
    //@}

private:
    //! @name Member data
    //@{
    const unsigned int nID_;
    MIL_Population* pPopulation_;
    MIL_PopulationHumans humans_;
    double rDensity_; // Alive humans density
    const MIL_PopulationAttitude* pAttitude_;
    T_AgentVector collidingAgents_;
    T_ObjectVector collidingObjects_;
    T_PopulationConcentrationVector collidingPopulationConcentrations_;
    T_PopulationFlowVector collidingPopulationFlows_;
    // Network
    bool bAttitudeUpdated_;
    bool bHumansUpdated_;
    T_IntoxicationEffects intoxicationEffects_;
    T_ContaminationEffects contaminationEffects_;
    std::auto_ptr< MIL_DecontaminationEffect > pDecontaminationEffect_;
    T_AgentVector collidingAttackingAgents_;
    //@}
};

#endif // __MIL_PopulationElement_ABC_h_
