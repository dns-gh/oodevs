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
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_ObjectManager.h $
// $Author: Age $
// $Modtime: 4/03/05 14:45 $
// $Revision: 2 $
// $Workfile: TER_ObjectManager.h $
//
// *****************************************************************************

#ifndef __TER_ObjectManager_h_
#define __TER_ObjectManager_h_

#include "TER_ObjectTraits.h"
#include "TER_Object_ABC.h"
#include "pathfind/SpatialContainer.h"

class MT_Ellipse;
class TER_Localisation;
class TER_Polygon;
class TER_ObjectPositionHint;
class MT_Rect;

// =============================================================================
/** @class  TER_ObjectManager
    @brief  TER_ObjectManager
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
     TER_ObjectManager( const MT_Rect& extent );
    ~TER_ObjectManager();
    //@}

    //! @name Operations
    //@{
    void GetListAt          ( const MT_Vector2D& vPos, T_ObjectVector& objects ) const;
    void GetListWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, T_ObjectVector& objects ) const;        

    TER_Object_ABC::T_Hint UpdatePosition( TER_Object_ABC& object, const TER_Object_ABC::T_Hint& hint );
    bool                   Remove        ( TER_Object_ABC& object, const TER_Object_ABC::T_Hint& hint );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TER_ObjectManager( const TER_ObjectManager& );            //!< Copy constructor
    TER_ObjectManager& operator=( const TER_ObjectManager& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef pathfind::SpatialContainer< TER_Object_ABC*, TER_ObjectTraits, MT_Float > T_Objects;
    //@}

private:
    //! @name Member data
    //@{
    T_Objects objects_;
    //@}
};

#endif // __TER_ObjectManager_h_
