// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TER_PopulationConcentration_ABC_h_
#define __TER_PopulationConcentration_ABC_h_

#include "TER_Localisation.h"
#include "TER_PopulationConcentrationTraits.h"

namespace spatialcontainer
{
    template< typename T, typename Traits, typename Coordinate > class SpatialContainerNode;
};

// =============================================================================
// Created: AGE 2005-01-31
// =============================================================================
class TER_PopulationConcentration_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< const TER_PopulationConcentration_ABC* > T_ConstPopulationConcentrationVector;
    typedef std::vector< TER_PopulationConcentration_ABC* >  T_PopulationConcentrationVector;
    typedef T_PopulationConcentrationVector::iterator        IT_PopulationConcentrationVector;
    typedef T_PopulationConcentrationVector::const_iterator  CIT_PopulationConcentrationVector;

    typedef spatialcontainer::SpatialContainerNode< TER_PopulationConcentration_ABC*, TER_PopulationConcentrationTraits, double >* T_Hint;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             TER_PopulationConcentration_ABC();
    virtual ~TER_PopulationConcentration_ABC();
    //@}

    //! @name Operations
    //@{
    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius ) const; // NB : return true if the circle is inside
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
    TER_PopulationConcentration_ABC( const TER_PopulationConcentration_ABC& );            //!< Copy constructor
    TER_PopulationConcentration_ABC& operator=( const TER_PopulationConcentration_ABC& ); //!< Assignment operator
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
// Name: TER_PopulationConcentration_ABC::serialize
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
template< typename Archive >
inline
void TER_PopulationConcentration_ABC::serialize( Archive& /*file*/, const unsigned int )
{
    // NOTHING
}

#endif // __TER_PopulationConcentration_ABC_h_
