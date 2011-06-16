// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TER_PopulationConcentrationManager_h_
#define __TER_PopulationConcentrationManager_h_

#include "TER_PopulationConcentration_ABC.h"
#include "TER_PopulationConcentrationTraits.h"

#include <spatialcontainer/SpatialContainer.h>

class MT_Rect;
class TER_PopulationConcentrationVisitor_ABC;

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
    virtual ~TER_PopulationConcentrationManager();
    //@}

    //! @name Operations
    //@{
    void GetListWithinCircle    ( const MT_Vector2D& vCenter, double rRadius, T_PopulationConcentrationVector& concentrations ) const;
    void GetListIntersectingLine( const MT_Vector2D& vStart, const MT_Vector2D& vEnd, T_PopulationConcentrationVector& concentrations ) const;
    void GetListWithinLocalisation( const TER_Localisation& localisation, T_PopulationConcentrationVector& concentrations ) const;

    TER_PopulationConcentration_ABC::T_Hint UpdatePosition( TER_PopulationConcentration_ABC& concentration, const TER_PopulationConcentration_ABC::T_Hint& hint );
    bool                                    Remove        ( TER_PopulationConcentration_ABC& concentration, const TER_PopulationConcentration_ABC::T_Hint& hint );
    void Accept( TER_PopulationConcentrationVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TER_PopulationConcentrationManager( const TER_PopulationConcentrationManager& );            //!< Copy constructor
    TER_PopulationConcentrationManager& operator=( const TER_PopulationConcentrationManager& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef spatialcontainer::SpatialContainer< TER_PopulationConcentration_ABC*, TER_PopulationConcentrationTraits, double > T_PopulationConcentrations;
    //@}

private:
    //! @name Member data
    //@{
    T_PopulationConcentrations concentrations_;
    //@}
};

#endif // __TER_PopulationConcentrationManager_h_
