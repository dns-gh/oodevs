// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TER_ObjectManager_h_
#define __TER_ObjectManager_h_

#include "TER_ObjectTraits.h"
#include "TER_Object_ABC.h"
#include <spatialcontainer/SpatialContainer.h>

class MT_Ellipse;
class TER_Localisation;
class TER_Polygon;
class TER_ObjectPositionHint;
class TER_ObjectVisitor_ABC;
class MT_Rect;

// =============================================================================
/** @class  TER_ObjectManager
    @brief  TER Object manager
*/
// Created: AGE 2005-01-31
// =============================================================================
class TER_ObjectManager
{
public:
    //! @name Types
    //@{
    typedef std::vector< TER_Object_ABC* > T_ObjectVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit TER_ObjectManager( const MT_Rect& extent );
    virtual ~TER_ObjectManager();
    //@}

    //! @name Operations
    //@{
    void GetListAt( const MT_Vector2D& vPos, T_ObjectVector& objects ) const;
    void GetListWithinCircle( const MT_Vector2D& vCenter, double rRadius, T_ObjectVector& objects ) const;
    void GetListWithinCircle2( const MT_Vector2D& vCenter, double rRadius, std::vector< const TER_Object_ABC* >& objects ) const;
    void GetListWithinLocalisation( const TER_Localisation& localisation, T_ObjectVector& objects ) const;
    TER_Object_ABC::T_Hint UpdatePosition( TER_Object_ABC& object, const TER_Object_ABC::T_Hint& hint );
    bool Remove( TER_Object_ABC& object, const TER_Object_ABC::T_Hint& hint );
    void Accept( TER_ObjectVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TER_ObjectManager( const TER_ObjectManager& );            //!< Copy constructor
    TER_ObjectManager& operator=( const TER_ObjectManager& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef spatialcontainer::SpatialContainer< TER_Object_ABC*, TER_ObjectTraits, double > T_Objects;
    //@}

private:
    //! @name Member data
    //@{
    T_Objects objects_;
    //@}
};

#endif // __TER_ObjectManager_h_
