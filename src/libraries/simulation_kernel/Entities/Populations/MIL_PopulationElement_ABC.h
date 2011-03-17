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

class AttritionCapacity;
class MIL_Agent_ABC;
class MIL_AgentPion;
class MIL_BurnEffectManipulator;
class MIL_Injury_ABC;
class MIL_Population;
class MIL_PopulationAttitude;
class MIL_PopulationType;
class MT_Circle;
class PHY_Volume;
class PHY_FireResults_ABC;
class PHY_FireResults_Population;
class TER_Localisation;

// =============================================================================
// Created: NLD 2005-09-28
// =============================================================================
class MIL_PopulationElement_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
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
    double Exterminate( double rSurface );
    //@}

    //! @name Actions
    //@{
    void FireOnPions( double rIntensity, PHY_FireResults_Population& fireResult );
    void FireOnPion( double rIntensity, MIL_Agent_ABC& target, PHY_FireResults_Population& fireResult );
    double GetDangerosity( const MIL_AgentPion& target ) const;
    void ApplyFire( unsigned int nNbrAmmoFired, PHY_FireResults_ABC& fireResult );
    void ApplyIndirectFire( const MT_Circle& attritionCircle, PHY_FireResults_ABC& fireResult );
    void ApplyExplosion( const AttritionCapacity& capacity, PHY_FireResults_ABC& fireResult );
    void ApplyInjury( MIL_Injury_ABC& injury );
    void ApplyBurn( const MIL_BurnEffectManipulator& burn );
    //@}

    //! @name Accessors
    //@{
    MIL_Population& GetPopulation() const;
    const MIL_PopulationAttitude& GetAttitude() const;
    unsigned int GetID() const;
    unsigned int GetAllHumans() const;
    unsigned int GetTotalLivingHumans() const;
    unsigned int GetHealthyHumans() const;
    unsigned int GetWoundedHumans() const;
    unsigned int GetContaminatedHumans() const;
    unsigned int GetDeadHumans() const;
    double GetDensity() const;
    bool IsDead() const;
    bool IsInZone( const TER_Localisation& loc ) const;

    virtual const TER_Localisation& GetLocation() const = 0;
    virtual MT_Vector2D GetSecuringPoint( const MIL_Agent_ABC& securingAgent ) const;
    virtual MT_Vector2D GetSafetyPosition( const MIL_AgentPion& agent, double rMinDistance, double rSeed ) const = 0;
    virtual double GetDefaultDensity( const MIL_PopulationType& type ) const = 0;
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
    void ClearCollisions();
    //@}

    //! @name Network
    //@{
    bool HasAttitudeChanged() const;
    bool HasHumansChanged() const;
    //@}

    //! @name Events
    //@{
    virtual void NotifyCollision( MIL_Agent_ABC& agent ) = 0;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MIL_Agent_ABC* >   T_AgentVector;
    typedef T_AgentVector::const_iterator CIT_AgentVector;
    //@}

private:
    const unsigned int nID_;
    MIL_Population* pPopulation_;
    MIL_PopulationHumans humans_;
    double rDensity_; // Alive humans density
    const MIL_PopulationAttitude* pAttitude_;
    T_AgentVector collidingAgents_;
    // Network
    bool bAttitudeUpdated_;
    bool bHumansUpdated_;
};

#endif // __MIL_PopulationElement_ABC_h_
