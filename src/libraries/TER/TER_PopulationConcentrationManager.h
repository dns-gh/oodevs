// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-01-31 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_PopulationConcentrationManager.h $
// $Author: Age $
// $Modtime: 4/03/05 14:45 $
// $Revision: 2 $
// $Workfile: TER_PopulationConcentrationManager.h $
//
// *****************************************************************************

#ifndef __TER_PopulationConcentrationManager_h_
#define __TER_PopulationConcentrationManager_h_

#include "TER_PopulationConcentration_ABC.h"
#include "TER_PopulationConcentrationTraits.h"

#include "pathfind/SpatialContainer.h"

class MT_Rect;

// =============================================================================
// Created: AGE 2005-01-31
// =============================================================================
class TER_PopulationConcentrationManager
{
public:
    //! @name Types
    //@{
    typedef std::vector< TER_PopulationConcentration_ABC* > T_PopulationConcentrationVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
     TER_PopulationConcentrationManager( const MT_Rect& extent );
    ~TER_PopulationConcentrationManager();
    //@}

    //! @name Operations
    //@{
    void GetListWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, T_PopulationConcentrationVector& concentrations ) const;

    TER_PopulationConcentration_ABC::T_Hint UpdatePosition( TER_PopulationConcentration_ABC& concentration, const TER_PopulationConcentration_ABC::T_Hint& hint );
    bool                                    Remove        ( TER_PopulationConcentration_ABC& concentration, const TER_PopulationConcentration_ABC::T_Hint& hint );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TER_PopulationConcentrationManager( const TER_PopulationConcentrationManager& );            //!< Copy constructor
    TER_PopulationConcentrationManager& operator=( const TER_PopulationConcentrationManager& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef pathfind::SpatialContainer< TER_PopulationConcentration_ABC*, TER_PopulationConcentrationTraits, MT_Float > T_PopulationConcentrations;
    //@}

private:
    //! @name Member data
    //@{
    T_PopulationConcentrations concentrations_;
    //@}
};

#endif // __TER_PopulationConcentrationManager_h_
