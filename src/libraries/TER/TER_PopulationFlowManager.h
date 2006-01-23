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
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_PopulationFlowManager.h $
// $Author: Age $
// $Modtime: 4/03/05 14:45 $
// $Revision: 2 $
// $Workfile: TER_PopulationFlowManager.h $
//
// *****************************************************************************

#ifndef __TER_PopulationFlowManager_h_
#define __TER_PopulationFlowManager_h_

#include "TER_PopulationFlow_ABC.h"
#include "TER_PopulationFlowTraits.h"

#include "pathfind/SpatialContainer.h"

class MT_Rect;

// =============================================================================
// Created: AGE 2005-01-31
// =============================================================================
class TER_PopulationFlowManager
{
public:
    //! @name Types
    //@{
    typedef std::vector< TER_PopulationFlow_ABC* > T_PopulationFlowVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
     TER_PopulationFlowManager( const MT_Rect& extent );
    ~TER_PopulationFlowManager();
    //@}

    //! @name Operations
    //@{
    void GetListWithinCircle    ( const MT_Vector2D& vCenter, MT_Float rRadius, T_PopulationFlowVector& flows ) const;
    void GetListIntersectingLine( const MT_Vector2D& vStart, const MT_Vector2D& vEnd, T_PopulationFlowVector& flows ) const;

    TER_PopulationFlow_ABC::T_Hint UpdatePosition( TER_PopulationFlow_ABC& flow, const TER_PopulationFlow_ABC::T_Hint& hint );
    bool                           Remove        ( TER_PopulationFlow_ABC& flow, const TER_PopulationFlow_ABC::T_Hint& hint );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TER_PopulationFlowManager( const TER_PopulationFlowManager& );            //!< Copy constructor
    TER_PopulationFlowManager& operator=( const TER_PopulationFlowManager& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef pathfind::SpatialContainer< TER_PopulationFlow_ABC*, TER_PopulationFlowTraits, MT_Float > T_PopulationFlows;
    //@}

private:
    //! @name Member data
    //@{
    T_PopulationFlows flows_;
    //@}
};

#endif // __TER_PopulationFlowManager_h_
