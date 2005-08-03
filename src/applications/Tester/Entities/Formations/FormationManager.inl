// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Formations/FormationManager.inl $
// $Author: Sbo $
// $Modtime: 20/06/05 16:21 $
// $Revision: 1 $
// $Workfile: FormationManager.inl $
//
// *****************************************************************************

namespace TIC
{

//-----------------------------------------------------------------------------
// Name: FormationManager::GetFormation
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
inline
Formation_ABC& FormationManager::GetFormation( FormationManager::EFormation eFormation )
{
    Formation_ABC* pFormation = formationMap_.find( eFormation )->second;
    assert( pFormation );
    return *pFormation;
}

} // end namespace TIC
