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

#include "pathfind/SpatialContainer.h"
#include "TER_ObjectTraits.h"

class TER_DynaObject_ABC;
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
    typedef std::vector< TER_DynaObject_ABC* > T_DynaObjectVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             TER_ObjectManager( const MT_Rect& extent );
    virtual ~TER_ObjectManager();
    //@}

    //! @name Operations
    //@{
    void GetListDynaObjectsAt( const MT_Vector2D& vPos, T_DynaObjectVector& dynaObjectsSet ) const;
    void GetListDynaObjectWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, T_DynaObjectVector& dynaObjectsSet ) const;        

    TER_ObjectPositionHint UpdateObjectPosition( TER_DynaObject_ABC& object, const TER_ObjectPositionHint& hint );
    bool RemoveObject( TER_DynaObject_ABC& object, const TER_ObjectPositionHint& hint );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TER_ObjectManager( const TER_ObjectManager& );            //!< Copy constructor
    TER_ObjectManager& operator=( const TER_ObjectManager& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef pathfind::SpatialContainer< TER_DynaObject_ABC*, TER_ObjectTraits, MT_Float > T_Objects;
    //@}

private:
    //! @name Member data
    //@{
    T_Objects objects_;
    //@}
};

#endif // __TER_ObjectManager_h_
