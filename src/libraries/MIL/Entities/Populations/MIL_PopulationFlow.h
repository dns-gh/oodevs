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

#ifndef __MIL_PopulationFlow_h_
#define __MIL_PopulationFlow_h_

#include "MIL.h"

#include "Tools/MIL_MOSIDManager.h"
#include "Entities/Actions/PHY_MovingEntity_ABC.h"
#include "TER/TER_PopulationFlow_ABC.h"

class MIL_Population;
class MIL_PopulationConcentration;
class MIL_PopulationAttitude;
class DEC_Population_Path;

// =============================================================================
// Created: NLD 2005-09-28
// =============================================================================
class MIL_PopulationFlow : public PHY_MovingEntity_ABC
                         , public TER_PopulationFlow_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     MIL_PopulationFlow( MIL_Population& population, MIL_PopulationConcentration& sourceConcentration );
     MIL_PopulationFlow();
    ~MIL_PopulationFlow();
    //@}

    //! @name Operations
    //@{
    bool Update();
    void Clean ();

    bool IsValid() const; // false = will be deleted
    //@}

    //! @name Actions
    //@{
    void Move( const MT_Vector2D& destination );
    //@}

    //! @name Accessors
    //@{
                  uint                    GetID            () const;
                  MIL_Population&         GetPopulation    () const;
    virtual const MT_Vector2D&            GetPosition      () const;
    virtual const MT_Vector2D&            GetDirection     () const;
    virtual const TER_Localisation&       GetLocation      () const;
            const MIL_PopulationAttitude& GetAttitude      () const;
                  uint                    GetNbrAliveHumans() const;
                  uint                    GetNbrDeadHumans () const;
                  MT_Float                GetSpeed         () const;
                  MT_Float                GetMaxSpeed      ( const PHY_Volume& pionVolume ) const;
    //@}

    //! @name Concentration management
    //@{
    void UnregisterSourceConcentration( MIL_PopulationConcentration& concentration );
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

private:
    //! @name Copy/Assignement
    //@{
    MIL_PopulationFlow( const MIL_PopulationFlow& );            //!< Copy constructor
    MIL_PopulationFlow& operator=( const MIL_PopulationFlow& ); //!< Assignement operator
    //@}

    //! @name 
    //@{
    virtual MT_Float GetMaxSpeed              () const;
    virtual MT_Float GetSpeedWithReinforcement( const TerrainData& environment ) const;
    virtual MT_Float GetSpeedWithReinforcement( const TerrainData& environment, const MIL_Object_ABC& object ) const;
    //@}

    //! @name  Position operations
    //@{
    virtual void       ApplyMove         ( const MT_Vector2D& position, const MT_Vector2D& direction, MT_Float rSpeed, MT_Float rWalkedDistance );
            void       UpdateTailPosition( const MT_Float rWalkedDistance );

    const MT_Vector2D& GetHeadPosition() const;
    const MT_Vector2D& GetTailPosition() const;
          void         SetHeadPosition( const MT_Vector2D& position );
          void         SetTailPosition( const MT_Vector2D& position );
          void         SetDirection   ( const MT_Vector2D& direction );
          void         SetSpeed       ( const MT_Float rSpeed );
          void         UpdateLocation ();
          void         UpdateDensity  ();
    //@}

    //! @name Notifications
    //@{
    virtual void NotifyMovingOnPathPoint   ( const DEC_PathPoint& point );
    virtual void NotifyMovingOnSpecialPoint( const DEC_PathPoint& point );
    virtual void NotifyMovingInsideObject  ( MIL_Object_ABC& object );
    virtual void NotifyMovingOutsideObject ( MIL_Object_ABC& object );
    virtual void NotifyEnvironmentChanged  ();
    virtual void NotifyCurrentPathChanged  ();
    //@}

    //! @name 
    //@{
    virtual bool CanMove              () const;
    virtual bool CanObjectInteractWith( const MIL_Object_ABC& object ) const;
    virtual bool HasResources         ();
    virtual void SendRC               ( const MIL_RC& rc ) const;
    //@}

    //! @name Network
    //@{
    void SendDestruction() const;
    bool HasChanged     () const;
    //@}

private:
          MIL_Population*              pPopulation_;
    const uint                         nID_;

    const MIL_PopulationAttitude*      pAttitude_;
          MIL_PopulationConcentration* pSourceConcentration_;
          MIL_PopulationConcentration* pDestConcentration_;
         
    MT_Vector2D          destination_;
    DEC_Population_Path* pCurrentPath_;
    bool                 bHeadMoveFinished_;

    MT_Vector2D          direction_;
    MT_Float             rSpeed_;
    T_PointList          flowShape_;
    TER_Localisation     location_; // For terrain

    MT_Float             rNbrAliveHumans_;
    MT_Float             rNbrDeadHumans_;
    MT_Float             rDensity_;
          
    // Network
    bool bPathUpdated_;
    bool bFlowShapeUpdated_;
    bool bDirectionUpdated_;
    bool bSpeedUpdated_;
    bool bHumansUpdated_;
    bool bAttitudeUpdated_;     

public:
    static MIL_MOSIDManager idManager_;
};

#include "MIL_PopulationFlow.inl"

#endif // __MIL_PopulationFlow_h_
