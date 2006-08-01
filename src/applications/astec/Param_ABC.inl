// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Param_ABC.inl $
// $Author: Ape $
// $Modtime: 21/07/04 11:34 $
// $Revision: 3 $
// $Workfile: Param_ABC.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: Param_ABC::Draw
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
inline
void Param_ABC::Draw()
{
}


// -----------------------------------------------------------------------------
// Name: Param_ABC::FillRemotePopupMenu
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
inline
void Param_ABC::FillRemotePopupMenu( QPopupMenu& /*popupMenu*/, const ActionContext& /*context*/ )
{
}


// -----------------------------------------------------------------------------
// Name: Param_ABC::CheckValidity
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
inline
bool Param_ABC::CheckValidity()
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: Param_ABC::IsOptional
// Created: SBO 2005-11-08
// -----------------------------------------------------------------------------
inline
bool Param_ABC::IsOptional() const
{
    return pOptional_;
}
