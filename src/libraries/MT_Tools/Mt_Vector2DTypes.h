// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Mt_Vector2DTypes_h_
#define __Mt_Vector2DTypes_h_

#include "MT_Vector2D.h"
#include <vector>
#include <list>
#include <set>

typedef std::list< MT_Vector2D >      T_PointList;
typedef T_PointList::iterator        IT_PointList;
typedef T_PointList::const_iterator CIT_PointList;

typedef std::vector< MT_Vector2D >               T_PointVector;
typedef const T_PointVector                     CT_PointVector;
typedef T_PointVector::iterator                 IT_PointVector;
typedef T_PointVector::const_iterator          CIT_PointVector;
typedef T_PointVector::reverse_iterator        RIT_PointVector;
typedef T_PointVector::const_reverse_iterator CRIT_PointVector;

typedef std::set< MT_Vector2D, TER_DistanceLess > T_PointSet;
typedef T_PointSet::iterator                     IT_PointSet;
typedef T_PointSet::const_iterator              CIT_PointSet;

typedef std::vector< MT_Vector2D* >        T_PointPtrVector;
typedef T_PointPtrVector::const_iterator CIT_PointPtrVector;

extern MT_Vector2D MT_ComputeBarycenter( const T_PointVector& points );

#endif // __Mt_Vector2DTypes_h_
