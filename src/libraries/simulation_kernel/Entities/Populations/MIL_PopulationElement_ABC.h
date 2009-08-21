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

#include "simulation_terrain/TER_PopulationConcentration_ABC.h"
#include "MT_Tools/MT_Random.h"

class MIL_Population;
class MIL_PopulationFlow;
class MIL_PopulationAttitude;
class MIL_Agent_ABC;
class MIL_AgentPion;
class AttritionCapacity;
class PHY_Volume;
class PHY_FireResults_Population;
class PHY_FireResults_ABC;

// =============================================================================
// Created: NLD 2005-09-28
// =============================================================================
class MIL_PopulationElement_ABC
{
public:
    //! @name Types
    //@{
    typedef std::pair< MT_Float /*alive humans*/, MT_Float /*dead humans*/ > T_Humans;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
     MIL_PopulationElement_ABC( MIL_Population& population, uint nID );
    ~MIL_PopulationElement_ABC();
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
    void     PushHumans   ( const T_Humans& humans );
    T_Humans PullHumans   ( MT_Float rNbr );
    void     KillAllHumans();
    MT_Float Exterminate  ( MT_Float rSurface );
    uint     Kill         ( uint count );
    uint     Resurrect    ( uint count );
    //@}

    //! @name Actions
    //@{
    void     FireOnPions      ( MT_Float rIntensity, PHY_FireResults_Population& fireResult );
    void     FireOnPion       ( MT_Float rIntensity, MIL_Agent_ABC& target, PHY_FireResults_Population& fireResult );
    MT_Float GetDangerosity   ( const MIL_AgentPion& target ) const;
    void     ApplyFire        (       uint                nNbrAmmoFired  , PHY_FireResults_ABC& fireResult );
    void     ApplyIndirectFire( const MT_Circle&          attritionCircle, PHY_FireResults_ABC& fireResult );
    void     ApplyExplosion   ( const AttritionCapacity& capacity, PHY_FireResults_ABC& fireResult );
    //@}

    //! @name Accessors
    //@{
          MIL_Population&         GetPopulation    () const;
    const MIL_PopulationAttitude& GetAttitude      () const;
          uint                    GetID            () const;
          MT_Float                GetNbrAliveHumans() const;
          MT_Float                GetNbrDeadHumans () const;
          MT_Float                GetNbrHumans     () const;
          MT_Float                GetDensity       () const;
          bool                    IsDead           () const;
          bool                    IsInZone         ( const TER_Localisation& loc ) const;

    virtual const TER_Localisation& GetLocation      () const = 0;
    virtual       MT_Vector2D       GetSecuringPoint ( const MIL_Agent_ABC& securingAgent ) const;
    virtual       MT_Vector2D       GetSafetyPosition( const MIL_AgentPion& agent, MT_Float rMinDistance, MT_Float rSeed ) const = 0;
    //@}

    //! @name Pion effects
    //@{
    MT_Float GetPionMaxSpeed           ( const PHY_Volume& pionVolume ) const;
    MT_Float GetPionReloadingTimeFactor() const;
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

protected:
    //! @name Copy/Assignement
    //@{
    MIL_PopulationElement_ABC( const MIL_PopulationElement_ABC& );            //!< Copy constructor
    MIL_PopulationElement_ABC& operator=( const MIL_PopulationElement_ABC& ); //!< Assignement operator
    //@}

    //! @name Tools
    //@{
    void UpdateDensity   ();
    void UpdateCollisions();
    void ClearCollisions ();
    //@}
 
    //! @name Network
    //@{
    bool HasAttitudeChanged() const;
    bool HasHumansChanged  () const;
    //@}

    //! @name Events
    //@{
    virtual void NotifyCollision( MIL_Agent_ABC& agent ) = 0;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MIL_Agent_ABC* >   T_AgentVector;
    typedef T_AgentVector::const_iterator   CIT_AgentVector;
    //@}

private:
    const uint                    nID_;
          MIL_Population*         pPopulation_;
          MT_Float                rNbrAliveHumans_;
          MT_Float                rNbrDeadHumans_;
          MT_Float                rDensity_; // Alive humans density
    const MIL_PopulationAttitude* pAttitude_;

          T_AgentVector           collidingAgents_;

          // Network
          bool                    bAttitudeUpdated_;   
          bool                    bHumansUpdated_;

private:
    static MT_Random randomGenerator_;
};

#endif // __MIL_PopulationElement_ABC_h_
