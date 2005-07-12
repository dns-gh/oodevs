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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_Mission.inl $
// $Author: Ape $
// $Modtime: 14/05/04 17:45 $
// $Revision: 4 $
// $Workfile: AGR_Mission.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: AGR_Mission::GetCompleteName
/** @return 
*/
// Created: AGN 2004-04-23
// -----------------------------------------------------------------------------
INLINE
std::string AGR_Mission::GetCompleteName() const
{
    return strModuleName_ + strMissionName_;
}

// -----------------------------------------------------------------------------
// Name: AGR_Mission::SetLowName
/** @param  strName 
*/
// Created: AGN 2004-04-23
// -----------------------------------------------------------------------------
INLINE
void AGR_Mission::SetLowName( const std::string& strName )
{
    strLowName_ = strName;
}

// -----------------------------------------------------------------------------
// Name: AGR_Mission::GetName
/** @return 
*/
// Created: AGN 2004-04-23
// -----------------------------------------------------------------------------
INLINE
const std::string& AGR_Mission::GetName() const
{
    return strMissionName_;
}

// -----------------------------------------------------------------------------
// Name: AGR_Mission::IsMissionForAutomate
/** @return 
*/
// Created: AGN 2004-04-23
// -----------------------------------------------------------------------------
INLINE
bool AGR_Mission::IsMissionForAutomate() const
{
    return bMissionForAutomata_;
}

// -----------------------------------------------------------------------------
// Name: AGR_Mission::GetLowName
/** @return 
*/
// Created: AGN 2004-04-23
// -----------------------------------------------------------------------------
INLINE
const std::string& AGR_Mission::GetLowName() const
{
    return strLowName_;
}


// -----------------------------------------------------------------------------
// Name: AGR_Mission::NeedsDeleteting
/** @return 
*/
// Created: APE 2004-05-14
// -----------------------------------------------------------------------------
INLINE
bool AGR_Mission::NeedsDeleteting() const
{
    return vMissionMember_.size() >= 1 && nFragOrderType_ == eSequence; 
}
