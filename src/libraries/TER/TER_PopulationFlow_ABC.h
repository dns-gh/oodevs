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
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_PopulationFlow_ABC.h $
// $Author: Jvt $
// $Modtime: 5/04/05 18:36 $
// $Revision: 3 $
// $Workfile: TER_PopulationFlow_ABC.h $
//
// *****************************************************************************

#ifndef __TER_PopulationFlow_ABC_h_
#define __TER_PopulationFlow_ABC_h_

#include "TER_Localisation.h"
#include "TER_PopulationFlowTraits.h"

namespace pathfind
{
    template< typename T, typename Traits, typename Coordinate > class SpatialContainerNode;
};


// =============================================================================
// Created: AGE 2005-01-31
// =============================================================================
class TER_PopulationFlow_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< TER_PopulationFlow_ABC* >  T_PopulationFlowVector;
    typedef T_PopulationFlowVector::iterator        IT_PopulationFlowVector;
    typedef T_PopulationFlowVector::const_iterator  CIT_PopulationFlowVector;

    typedef pathfind::SpatialContainerNode< TER_PopulationFlow_ABC*, TER_PopulationFlowTraits, MT_Float >* T_Hint;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             TER_PopulationFlow_ABC();
    virtual ~TER_PopulationFlow_ABC();
    //@}

    //! @name Operations
    //@{
//    bool IsInside             ( const MT_Vector2D& vPos ) const;
//    bool Intersect2D          ( const MT_Line& orientedLine, T_PointSet& collisions ) const;
    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, MT_Float rRadius ) const; // NB : return true if the circle is inside
    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, MT_Float rRadius, T_PointVector& shape ) const; // NB : return true if the circle is inside
    bool Intersect2DWithLine  ( const MT_Vector2D& vStart, const MT_Vector2D& vEnd ) const;
    //@}


    //! @name Operations
    //@{
    virtual const TER_Localisation& GetLocation() const = 0;

    void UpdatePatch    ();
    void RemoveFromPatch();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive& file, const uint );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TER_PopulationFlow_ABC( const TER_PopulationFlow_ABC& );            //!< Copy constructor
    TER_PopulationFlow_ABC& operator=( const TER_PopulationFlow_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void InsertInPatch();
    //@}

private:
    //! @name Member data
    //@{
    T_Hint hint_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlow_ABC::serialize
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
template< typename Archive >
inline
void TER_PopulationFlow_ABC::serialize( Archive& /*file*/, const uint )
{
    // NOTHING
}

#endif // __TER_PopulationFlow_ABC_h_
