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

class MIL_Population;
class MIL_PopulationFlow;
class MIL_PopulationAttitude;

// =============================================================================
// Created: NLD 2005-09-28
// =============================================================================
class MIL_PopulationConcentration
{
public:
    //! @name Constructors/Destructor
    //@{
     MIL_PopulationConcentration( MIL_Population& population, MIL_InputArchive& archive );
    ~MIL_PopulationConcentration();
    //@}

    //! @name Operations
    //@{
    bool Update(); // Return false if the concentration must be deleted
    void Clean ();
    //@}

    //! @name Actions
    //@{
    MT_Float PushHumans( MT_Float rNbr );
    MT_Float PullHumans( MT_Float rNbr );
    void     Move      ( const MT_Vector2D& destination );
    //@}

    //! @name Accessors
    //@{
    const MT_Vector2D&            GetPosition() const;
    const MIL_PopulationAttitude& GetAttitude() const;
    //@}

    //! @name Network
    //@{
    void SendCreation    () const;
    void SendFullState   () const;
    void SendChangedState() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MIL_PopulationConcentration( const MIL_PopulationConcentration& );            //!< Copy constructor
    MIL_PopulationConcentration& operator=( const MIL_PopulationConcentration& ); //!< Assignement operator
    //@}

    //! @name Network
    //@{
    bool HasChanged() const;
    //@}

private:
          MIL_Population&         population_;
          uint                    nID_;
          MT_Vector2D             position_;
          MT_Float                rNbrAliveHumans_;
          MT_Float                rNbrDeadHumans_;
    const MIL_PopulationAttitude* pAttitude_;

          MIL_PopulationFlow*     pPullingFlow_;

    // Network
    bool bHumansUpdated_;
    bool bAttitudeUpdated_;

public:
    static MIL_MOSIDManager idManager_;
};

#include "MIL_PopulationConcentration.inl"

#endif // __MIL_PopulationConcentration_h_
