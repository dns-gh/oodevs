// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TER_PopulationFlowManager_h_
#define __TER_PopulationFlowManager_h_

#include "TER_PopulationFlow_ABC.h"
#include "TER_PopulationFlowTraits.h"
#include <pathfind/SpatialContainer.h>

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
    virtual ~TER_PopulationFlowManager();
    //@}

    //! @name Operations
    //@{
    void GetListWithinCircle    ( const MT_Vector2D& vCenter, double rRadius, T_PopulationFlowVector& flows ) const;
    void GetListIntersectingLine( const MT_Vector2D& vStart, const MT_Vector2D& vEnd, T_PopulationFlowVector& flows ) const;
    void GetListWithinLocalisation( const TER_Localisation& localisation, T_PopulationFlowVector& flows ) const;

    TER_PopulationFlow_ABC::T_Hint UpdatePosition( TER_PopulationFlow_ABC& flow, const TER_PopulationFlow_ABC::T_Hint& hint );
    bool                           Remove        ( TER_PopulationFlow_ABC& flow, const TER_PopulationFlow_ABC::T_Hint& hint );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TER_PopulationFlowManager( const TER_PopulationFlowManager& );            //!< Copy constructor
    TER_PopulationFlowManager& operator=( const TER_PopulationFlowManager& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef pathfind::SpatialContainer< TER_PopulationFlow_ABC*, TER_PopulationFlowTraits, double > T_PopulationFlows;
    //@}

private:
    //! @name Member data
    //@{
    T_PopulationFlows flows_;
    //@}
};

#endif // __TER_PopulationFlowManager_h_
