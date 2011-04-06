// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
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

    typedef pathfind::SpatialContainerNode< TER_PopulationFlow_ABC*, TER_PopulationFlowTraits, double >* T_Hint;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             TER_PopulationFlow_ABC();
    virtual ~TER_PopulationFlow_ABC();
    //@}

    //! @name Operations
    //@{
    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius ) const; // NB : return true if the circle is inside
    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius, T_PointVector& shape ) const; // NB : return true if the circle is inside
    bool Intersect2DWithLine  ( const MT_Vector2D& vStart, const MT_Vector2D& vEnd ) const;
    bool IsIntersecting       ( const TER_Localisation& localisation ) const;
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
    void serialize( Archive& file, const unsigned int );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TER_PopulationFlow_ABC( const TER_PopulationFlow_ABC& );            //!< Copy constructor
    TER_PopulationFlow_ABC& operator=( const TER_PopulationFlow_ABC& ); //!< Assignment operator
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
void TER_PopulationFlow_ABC::serialize( Archive& /*file*/, const unsigned int )
{
    // NOTHING
}

#endif // __TER_PopulationFlow_ABC_h_
