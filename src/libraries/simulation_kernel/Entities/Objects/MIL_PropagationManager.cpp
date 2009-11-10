// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PropagationManager.h"
#include "Tools/MIL_Tools.h"
#include <xeumeuleu/xml.h>

// BOOST_CLASS_EXPORT_GUID( MIL_PropagationManager, "MIL_PropagationManager" )

// -----------------------------------------------------------------------------
// Name: MIL_PropagationManager constructor
// Created: RFT 28/04/2008
// Modified: none
// -----------------------------------------------------------------------------
MIL_PropagationManager::MIL_PropagationManager()
{
    // NOTHING
}
       
// -----------------------------------------------------------------------------
// Name: MIL_PropagationManager destructor
// Created: RFT 28/04/2008
// Modified: RFT 30/04/2008
// -----------------------------------------------------------------------------
MIL_PropagationManager::~MIL_PropagationManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PropagationManager::RegisterDynamicFire( MIL_DynamicFire* pDynamicFire, const MT_Vector2D vOrigin )
// Created: RFT 30/04/2008
// Modified: RFT 05/05/2008
// -----------------------------------------------------------------------------
void MIL_PropagationManager::Flag( const MT_Vector2D& vOrigin , unsigned int length , unsigned int width )
{
    //On récupère les coordonnées de l'origine du feu
    int coord_x = (int) (vOrigin.rX_);
    int coord_y = (int) (vOrigin.rY_);

    coord_x = coord_x - coord_x % length;
    coord_y = coord_y - coord_y % width;
    
    coordinatesSet_.insert( T_Coordinate( coord_x, coord_y ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_PropagationManager::UnregisterDynamicFire( MIL_DynamicFire& dynamicFire, const MT_Vector2D vOrigin )
// Created: RFT 05/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_PropagationManager::RemoveFlag( const MT_Vector2D& vOrigin , unsigned int length , unsigned int width )
{
    //On récupère les coordonnées de l'origine du feu
    int coord_x = (int) (vOrigin.rX_);
    int coord_y = (int) (vOrigin.rY_);

    coord_x = coord_x - coord_x % length;
    coord_y = coord_y - coord_y % width;

    T_CoordinatesSet::iterator iter = coordinatesSet_.find( T_Coordinate( coord_x, coord_y ) );
    
    if ( iter != coordinatesSet_.end() ) 
        coordinatesSet_.erase( iter );
}

// -----------------------------------------------------------------------------
// Name: MIL_PropagationManager::DynamicFireIsAlreadyExistingHere( const TER_Localisation& localisation)
// Created: RFT 05/05/2008
// Modified: none
// -----------------------------------------------------------------------------
bool MIL_PropagationManager::IsFlagged( const TER_Localisation& localisation , unsigned int length , unsigned int width ) const
{
    //On récupère les coordonnées de la localisation ( qui sont celles d'un point)
    int coord_x = (int) ( localisation.ComputeBarycenter().rX_ );
    int coord_y = (int) ( localisation.ComputeBarycenter().rY_ );
    
    //Puis on on lui enlève son reste de la division par 5, car le maillage sera de 5 par 5 pour faire des cases de feu de 25m²
    coord_x = coord_x - coord_x % length;
    coord_y = coord_y - coord_y % width;

    return coordinatesSet_.find( T_Coordinate( coord_x, coord_y ) ) != coordinatesSet_.end();
}