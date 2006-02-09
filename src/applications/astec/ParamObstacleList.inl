// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamObstacleList.inl $
// $Author: Ape $
// $Modtime: 25/05/04 15:39 $
// $Revision: 2 $
// $Workfile: ParamObstacleList.inl $
//
// *****************************************************************************

#include "GLTool.h"


// -----------------------------------------------------------------------------
// Name: ParamObstacleList::Copy
/** @param  pSrc 
    @param  pDst 
*/
// Created: APE 2004-05-24
// -----------------------------------------------------------------------------
inline
void ParamObstacleList::Copy( ASN1T_MissionGenObject& src, ASN1T_MissionGenObject& dest )
{
    dest.oid_obstacle_planifie = src.oid_obstacle_planifie;
    dest.preliminaire          = src.preliminaire;
    dest.type_obstacle         = src.type_obstacle;
    dest.urgence               = src.urgence;
    dest.priorite              = src.priorite;

    dest.pos_obstacle.type               = src.pos_obstacle.type;
    dest.pos_obstacle.vecteur_point.n    = src.pos_obstacle.vecteur_point.n;
    dest.pos_obstacle.vecteur_point.elem = new ASN1T_CoordUTM[ src.pos_obstacle.vecteur_point.n ];
    asnUMTCoordPtrList_.push_back( dest.pos_obstacle.vecteur_point.elem );

    for( uint j = 0; j < src.pos_obstacle.vecteur_point.n; ++j )
        dest.pos_obstacle.vecteur_point.elem[j] = 
             src.pos_obstacle.vecteur_point.elem[j];
}


// -----------------------------------------------------------------------------
// Name: ParamObstacleList::Draw
// Created: APE 2004-05-25
// -----------------------------------------------------------------------------
inline
void ParamObstacleList::Draw()
{
    GLTool::Draw( *this );
}
