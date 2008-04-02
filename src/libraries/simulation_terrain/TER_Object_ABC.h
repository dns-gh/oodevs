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
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_Object_ABC.h $
// $Author: Jvt $
// $Modtime: 5/04/05 18:36 $
// $Revision: 3 $
// $Workfile: TER_Object_ABC.h $
//
// *****************************************************************************

#ifndef __TER_Object_ABC_h_
#define __TER_Object_ABC_h_

#include "TER_Localisation.h"

#include "TER_ObjectTraits.h"

namespace pathfind
{
    template< typename T, typename Traits, typename Coordinate > class SpatialContainerNode;
};


// =============================================================================
// Created: AGE 2005-01-31
// =============================================================================
class TER_Object_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< TER_Object_ABC* >   T_ObjectVector;
    typedef T_ObjectVector::iterator        IT_ObjectVector;
    typedef T_ObjectVector::const_iterator CIT_ObjectVector;

    typedef pathfind::SpatialContainerNode< TER_Object_ABC*, TER_ObjectTraits, MT_Float >* T_Hint;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             TER_Object_ABC();
    virtual ~TER_Object_ABC();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    template<class Archive>
    void load( Archive&, const uint );
    template<class Archive>
    void save( Archive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    void Initialize( const TER_Localisation& localisation );
    void Terminate();

    bool IsInside                   ( const MT_Vector2D& vPos ) const;
    bool Intersect2D                ( const MT_Line& orientedLine, T_PointSet& collisions ) const;
    bool Intersect2DWithCircle      ( const MT_Vector2D& vCircleCenter, MT_Float rRadius ) const; // NB : return true if the circle is inside
    bool Intersect2DWithLocalisation( const TER_Localisation& localisation ) const;
    //@}

    //! @name Accessors
    //@{
    const TER_Localisation& GetLocalisation() const;
    //@}

    //! @name Modifiers
    //@{
    void UpdateLocalisation( const TER_Localisation& newLocalisation );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TER_Object_ABC( const TER_Object_ABC& );            //!< Copy constructor
    TER_Object_ABC& operator=( const TER_Object_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void InsertInWorld();
    void RemoveFromWorld();
    //@}

private:
    //! @name Member data
    //@{
    TER_Localisation location_;
    T_Hint           hint_;
    //@}
};

#include "TER_Object_ABC.inl"

#endif // __TER_Object_ABC_h_
