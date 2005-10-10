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
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_PopulationConcentration_ABC.h $
// $Author: Jvt $
// $Modtime: 5/04/05 18:36 $
// $Revision: 3 $
// $Workfile: TER_PopulationConcentration_ABC.h $
//
// *****************************************************************************

#ifndef __TER_PopulationConcentration_ABC_h_
#define __TER_PopulationConcentration_ABC_h_

#include "TER_Localisation.h"
#include "TER_PopulationConcentrationTraits.h"

namespace pathfind
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
    typedef std::vector< TER_PopulationConcentration_ABC* >  T_PopulationConcentrationVector;
    typedef T_PopulationConcentrationVector::iterator        IT_PopulationConcentrationVector;
    typedef T_PopulationConcentrationVector::const_iterator  CIT_PopulationConcentrationVector;

    typedef pathfind::SpatialContainerNode< TER_PopulationConcentration_ABC*, TER_PopulationConcentrationTraits, MT_Float >* T_Hint;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             TER_PopulationConcentration_ABC();
    virtual ~TER_PopulationConcentration_ABC();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( boost::archive::binary_iarchive&, const uint );
    void save( boost::archive::binary_oarchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
//    bool IsInside             ( const MT_Vector2D& vPos ) const;
//    bool Intersect2D          ( const MT_Line& orientedLine, T_PointSet& collisions ) const;
    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, MT_Float rRadius ) const; // NB : return true if the circle is inside
    //@}


    //! @name Operations
    //@{
    virtual const TER_Localisation& GetLocation() const = 0;

    void UpdatePatch    ();
    void RemoveFromPatch();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TER_PopulationConcentration_ABC( const TER_PopulationConcentration_ABC& );            //!< Copy constructor
    TER_PopulationConcentration_ABC& operator=( const TER_PopulationConcentration_ABC& ); //!< Assignement operator
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

#endif // __TER_PopulationConcentration_ABC_h_
