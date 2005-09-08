// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-04-22 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_Workspace.inl $
// $Author: Agn $
// $Modtime: 22/04/04 11:04 $
// $Revision: 1 $
// $Workfile: AGR_Workspace.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: AGR_Workspace::FindTypeRC
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
inline
const AGR_TypeRC* AGR_Workspace::FindTypeRC( const std::string& strType ) const
{
    CIT_TypeRCMap it = typeRCs_.find( strType );
    if( it == typeRCs_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::GetRCs
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
inline
const AGR_Workspace::T_RCVector& AGR_Workspace::GetRCs() const
{
    return rcs_;
}
