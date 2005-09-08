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
// Name: MOS_Object_SiteFranchissement::GetWidth
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint MOS_Object_SiteFranchissement::GetWidth() const
{
    return nWidth_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_SiteFranchissement::GetDepth
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint MOS_Object_SiteFranchissement::GetDepth() const
{
    return nDepth_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_SiteFranchissement::GetSpeed
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint MOS_Object_SiteFranchissement::GetSpeed() const
{
    return nSpeed_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_SiteFranchissement::MustConstruct
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
bool MOS_Object_SiteFranchissement::MustConstruct() const
{
    return bConstruct_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_SiteFranchissement::SetWidth
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void MOS_Object_SiteFranchissement::SetWidth( uint nWidth )
{
    nWidth_ = nWidth;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Object_SiteFranchissement::SetDepth
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void MOS_Object_SiteFranchissement::SetDepth( uint nDepth )
{
    nDepth_ = nDepth;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Object_SiteFranchissement::SetSpeed
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void MOS_Object_SiteFranchissement::SetSpeed( uint nSpeed )
{
    nSpeed_ = nSpeed;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Object_SiteFranchissement::SetConstruct
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void MOS_Object_SiteFranchissement::SetConstruct( bool bConstruct )
{
    bConstruct_ = bConstruct;
}