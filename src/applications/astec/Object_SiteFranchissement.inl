// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-09-02 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: Object_SiteFranchissement::GetWidth
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint Object_SiteFranchissement::GetWidth() const
{
    return nWidth_;
}

// -----------------------------------------------------------------------------
// Name: Object_SiteFranchissement::GetDepth
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint Object_SiteFranchissement::GetDepth() const
{
    return nDepth_;
}

// -----------------------------------------------------------------------------
// Name: Object_SiteFranchissement::GetSpeed
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint Object_SiteFranchissement::GetSpeed() const
{
    return nSpeed_;
}

// -----------------------------------------------------------------------------
// Name: Object_SiteFranchissement::MustConstruct
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
bool Object_SiteFranchissement::MustConstruct() const
{
    return bConstruct_;
}

// -----------------------------------------------------------------------------
// Name: Object_SiteFranchissement::SetWidth
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void Object_SiteFranchissement::SetWidth( uint nWidth )
{
    nWidth_ = nWidth;
}
    
// -----------------------------------------------------------------------------
// Name: Object_SiteFranchissement::SetDepth
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void Object_SiteFranchissement::SetDepth( uint nDepth )
{
    nDepth_ = nDepth;
}
    
// -----------------------------------------------------------------------------
// Name: Object_SiteFranchissement::SetSpeed
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void Object_SiteFranchissement::SetSpeed( uint nSpeed )
{
    nSpeed_ = nSpeed;
}
    
// -----------------------------------------------------------------------------
// Name: Object_SiteFranchissement::SetConstruct
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void Object_SiteFranchissement::SetConstruct( bool bConstruct )
{
    bConstruct_ = bConstruct;
}