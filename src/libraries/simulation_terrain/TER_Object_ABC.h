// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TER_Object_ABC_h_
#define __TER_Object_ABC_h_

#include "TER_Localisation.h"
#include "TER_ObjectTraits.h"

namespace spatialcontainer
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
    typedef std::vector< TER_Object_ABC* > T_ObjectVector;
    typedef spatialcontainer::SpatialContainerNode< TER_Object_ABC*, TER_ObjectTraits, double >* T_Hint;
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
    void load( Archive&, const unsigned int );
    template<class Archive>
    void save( Archive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Initialize( const TER_Localisation& localisation, double pointSize = 250. );
    void Terminate();

    virtual bool IsInside( const MT_Vector2D& vPos ) const;
    virtual bool IsOnBorder( const MT_Vector2D& vPos ) const;
    bool Intersect2D( const MT_Line& orientedLine, T_PointSet& collisions ) const;
    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius ) const; // NB : return true if the circle is inside
    bool Intersect2DWithLocalisation( const TER_Localisation& localisation ) const;
    bool IsContainedByLocalisation( const TER_Localisation& localisation ) const;
    //@}

    //! @name Accessors
    //@{
    const TER_Localisation& GetLocalisation() const;
    TER_Localisation  GetLocalisation();
    virtual bool IsUniversal() const;
    double GetPrecision() const;
    //@}

    //! @name Modifiers
    //@{
    virtual void UpdateLocalisation( const TER_Localisation& newLocalisation );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TER_Object_ABC( const TER_Object_ABC& );            //!< Copy constructor
    TER_Object_ABC& operator=( const TER_Object_ABC& ); //!< Assignment operator
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
    T_Hint hint_;
    static const double epsilon_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
template<class Archive>
void TER_Object_ABC::load( Archive& file, const unsigned int )
{
    file >> location_;
    InsertInWorld();
}

// -----------------------------------------------------------------------------
// Name: TER_Object_ABC::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
template<class Archive>
void TER_Object_ABC::save( Archive& file, const unsigned int ) const
{
    file << location_;
}

#endif // __TER_Object_ABC_h_
