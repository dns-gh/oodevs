// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-28 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __MIL_PopulationElement_ABC_h_
#define __MIL_PopulationElement_ABC_h_

#include "MIL.h"

#include "TER/TER_PopulationConcentration_ABC.h"
#include "MT_Tools/MT_Random.h"

class MIL_Population;
class MIL_PopulationFlow;
class MIL_PopulationAttitude;
class MIL_Agent_ABC;
class MIL_AgentPion;
class PHY_Volume;
class PHY_FireResults_Population;

// =============================================================================
// Created: NLD 2005-09-28
// =============================================================================
class MIL_PopulationElement_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     MIL_PopulationElement_ABC( MIL_Population& population, uint nID );
     MIL_PopulationElement_ABC();
    ~MIL_PopulationElement_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Clean();
    //@}

    //! @name Modifiers
    //@{
    void     SetAttitude( const MIL_PopulationAttitude& attitude );
    MT_Float PushHumans ( MT_Float rNbr );
    MT_Float PullHumans ( MT_Float rNbr );
    //@}

    //! @name Actions
    //@{
    void     FireOnPions   ( MT_Float rIntensity, PHY_FireResults_Population& fireResult );
    void     FireOnPion    ( MT_Float rIntensity, MIL_Agent_ABC& target, PHY_FireResults_Population& fireResult );
    MT_Float GetDangerosity( const MIL_AgentPion& target ) const;
    //@}

    //! @name Accessors
    //@{
          MIL_Population&         GetPopulation    () const;
    const MIL_PopulationAttitude& GetAttitude      () const;
          uint                    GetID            () const;
          MT_Float                GetNbrAliveHumans() const;
          MT_Float                GetNbrDeadHumans () const;
          MT_Float                GetDensity       () const;

    virtual const TER_Localisation& GetLocation () const = 0;
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

    void SetNbrAliveHumans( MT_Float rNbr );
    void SetNbrDeadHumans ( MT_Float rNbr );
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
          MT_Float                rDensity_;
    const MIL_PopulationAttitude* pAttitude_;

          T_AgentVector           collidingAgents_;

          // Network
          bool                    bAttitudeUpdated_;   
          bool                    bHumansUpdated_;

private:
    static MT_Random randomGenerator_;
};

#include "MIL_PopulationElement_ABC.inl"

#endif // __MIL_PopulationElement_ABC_h_
