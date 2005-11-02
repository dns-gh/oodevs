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

#ifndef __MIL_PopulationConcentration_h_
#define __MIL_PopulationConcentration_h_

#include "MIL.h"

#include "Tools/MIL_MOSIDManager.h"
#include "TER/TER_PopulationConcentration_ABC.h"

class MIL_Population;
class MIL_PopulationFlow;
class MIL_PopulationAttitude;
class PHY_Volume;

// =============================================================================
// Created: NLD 2005-09-28
// =============================================================================
class MIL_PopulationConcentration : public TER_PopulationConcentration_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     MIL_PopulationConcentration( MIL_Population& population, MIL_InputArchive& archive );
     MIL_PopulationConcentration( MIL_Population& population, const MT_Vector2D& position );
     MIL_PopulationConcentration();
    ~MIL_PopulationConcentration();
    //@}

    //! @name Operations
    //@{
    bool Update(); // Return false if the concentration must be deleted
    void Clean ();

    bool IsValid() const; // false = will be deleted
    //@}

    //! @name Actions
    //@{
    MT_Float PushHumans ( MT_Float rNbr );
    MT_Float PullHumans ( MT_Float rNbr );
    void     Move       ( const MT_Vector2D& destination );
    void     SetAttitude( const MIL_PopulationAttitude& attitude );
    //@}

    //! @name Flows management
    //@{
    void RegisterPushingFlow  ( MIL_PopulationFlow& flow );
    void UnregisterPushingFlow( MIL_PopulationFlow& flow );
    //@}

    //! @name Accessors
    //@{
          MIL_Population&         GetPopulation    () const;
    const MT_Vector2D&            GetPosition      () const;
    const MIL_PopulationAttitude& GetAttitude      () const;
          uint                    GetID            () const;
          uint                    GetNbrAliveHumans() const;
          uint                    GetNbrDeadHumans () const;
          MT_Float                GetDensity       () const;
          MT_Float                GetPionMaxSpeed  ( const PHY_Volume& pionVolume ) const;
          bool                    IsNearPosition   ( const MT_Vector2D& position ) const;

    virtual const TER_Localisation& GetLocation () const;
    //@}

    //! @name Network
    //@{
    void SendCreation    () const;
    void SendFullState   () const;
    void SendChangedState() const;
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

public:
    //! @name Types
    //@{
    typedef std::set< MIL_PopulationFlow* > T_FlowSet;
    typedef T_FlowSet::const_iterator       CIT_FlowSet;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MIL_PopulationConcentration( const MIL_PopulationConcentration& );            //!< Copy constructor
    MIL_PopulationConcentration& operator=( const MIL_PopulationConcentration& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void UpdateLocation();
    void UpdateDensity ();
    //@}

    //! @name Network
    //@{
    void SendDestruction() const;
    bool HasChanged     () const;
    //@}

private:
          MIL_Population*         pPopulation_;
          uint                    nID_;
          MT_Vector2D             position_;
          TER_Localisation        location_;
          MT_Float                rNbrAliveHumans_;
          MT_Float                rNbrDeadHumans_;
          MT_Float                rDensity_;
    const MIL_PopulationAttitude* pAttitude_;

          MIL_PopulationFlow*     pPullingFlow_;
          T_FlowSet               pushingFlows_;

    // Network
    bool bHumansUpdated_;
    bool bAttitudeUpdated_;

public:
    static MIL_MOSIDManager idManager_;
};

#include "MIL_PopulationConcentration.inl"

#endif // __MIL_PopulationConcentration_h_
