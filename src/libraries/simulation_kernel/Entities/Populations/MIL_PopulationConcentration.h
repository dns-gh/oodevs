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

#include "MIL_PopulationElement_ABC.h"
#include "simulation_terrain/TER_PopulationConcentration_ABC.h"

class MIL_Population;
class MIL_PopulationFlow;
class MIL_PopulationAttitude;
class MIL_RealObject_ABC;
class PHY_Volume;

// =============================================================================
// Created: NLD 2005-09-28
// =============================================================================
class MIL_PopulationConcentration : public MIL_PopulationElement_ABC
                                  , public TER_PopulationConcentration_ABC
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
    virtual void Clean ();

    bool IsValid       () const; // false = will be deleted
    bool CanBePerceived() const;
    //@}

    //! @name Actions
    //@{
    void MagicMove( const MT_Vector2D& destination );
    void Move     ( const MT_Vector2D& destination );
    //@}

    //! @name Flows management
    //@{
    void     RegisterPushingFlow   ( MIL_PopulationFlow& flow );
    void     UnregisterPushingFlow ( MIL_PopulationFlow& flow );

    MT_Float GetPullingFlowsDensity() const;
    void     SetPullingFlowsDensity( const MIL_RealObject_ABC& splittingObject );
    //@}

    //! @name Accessors
    //@{
    const MIL_RealObject_ABC* GetSplittingObject() const;
    const MT_Vector2D&        GetPosition       () const;
          bool                IsNearPosition    ( const MT_Vector2D& position ) const;

    virtual const TER_Localisation& GetLocation      () const;
    virtual       MT_Vector2D       GetSecuringPoint ( const MIL_Agent_ABC& securingAgent ) const;
    virtual       MT_Vector2D       GetSafetyPosition( const MIL_AgentPion& agent, MT_Float rMinDistance, MT_Float rSeed ) const;
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

    //! @name Tools
    //@{
    void NotifyCollision( MIL_Agent_ABC& agent );
    void UpdateLocation ();
    //@}

    //! @name Network
    //@{
    void SendDestruction() const;
    bool HasChanged     () const;
    //@}

private:
    MT_Vector2D             position_;
    TER_Localisation        location_;

    MIL_PopulationFlow*     pPullingFlow_;
    T_FlowSet               pushingFlows_;

    const MIL_RealObject_ABC* pSplittingObject_;
    MT_Float                  rPullingFlowsDensity_;
};

#include "MIL_PopulationConcentration.inl"

#endif // __MIL_PopulationConcentration_h_
