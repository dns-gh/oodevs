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

class MIL_Population;
class MIL_PopulationConcentration;
class DEC_Population_Path;

// =============================================================================
// Created: NLD 2005-09-28
// =============================================================================
class MIL_PopulationFlow : public PHY_MovingEntity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     MIL_PopulationFlow( const MIL_Population& population, MIL_PopulationConcentration& sourceConcentration );
    ~MIL_PopulationFlow();
    //@}

    //! @name Operations
    //@{
    void Clean();
    //@}

    //! @name Actions
    //@{
    void Move( const MT_Vector2D& destination );
    //@}

    //! @name Accessors
    //@{
                  uint         GetID       () const;
    virtual const MT_Vector2D& GetPosition () const;
    virtual const MT_Vector2D& GetDirection() const;
    //@}

    //! @name Network
    //@{
    void SendCreation () const;
    void SendFullState() const;
    //@}

protected:
    //! @name 
    //@{
    virtual MT_Float GetMaxSpeed              () const;
    virtual MT_Float GetSpeedWithReinforcement( const TerrainData& environment ) const;
    virtual MT_Float GetSpeedWithReinforcement( const TerrainData& environment, const MIL_Object_ABC& object ) const;
    //@}

    //! @name 
    //@{
    virtual void ApplyMove( const MT_Vector2D& position, const MT_Vector2D& direction, MT_Float rSpeed );
    //@}

    //! @name Notifications
    //@{
    virtual void NotifySpecialPoint       ( const DEC_PathPoint& point );
    virtual void NotifyMovingInsideObject ( MIL_Object_ABC& object );
    virtual void NotifyMovingOutsideObject( MIL_Object_ABC& object );
    virtual void NotifyEnvironmentChanged ();
    virtual void NotifyCurrentPathChanged ();
    //@}

    //! @name 
    //@{
    virtual bool CanMove              () const;
    virtual bool CanObjectInteractWith( const MIL_Object_ABC& object ) const;
    virtual bool HasResources         ();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MIL_PopulationFlow( const MIL_PopulationFlow& );            //!< Copy constructor
    MIL_PopulationFlow& operator=( const MIL_PopulationFlow& ); //!< Assignement operator
    //@}

private:
    const MIL_Population&              population_;
    const uint                         nID_;

          MT_Vector2D                  headPosition_;
          MT_Vector2D                  tailPosition_;
          MT_Vector2D                  direction_;

          MT_Vector2D                  destination_;
          DEC_Population_Path*         pCurrentPath_;
          
          MIL_PopulationConcentration* pSourceConcentration_;
          MIL_PopulationConcentration* pDestConcentration_;

public:
    static MIL_MOSIDManager idManager_;
};

#include "MIL_PopulationFlow.inl"

#endif // __MIL_PopulationFlow_h_
