// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-08-04 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Report_ABC.inl $
// $Author: Ape $
// $Modtime: 5/08/04 17:47 $
// $Revision: 2 $
// $Workfile: Report_ABC.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: Report_ABC::GetPos
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& Report_ABC::GetPos() const
{
    return vPos_;
}
    

// -----------------------------------------------------------------------------
// Name: Report_ABC::GetTitle
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
inline
const std::string& Report_ABC::GetTitle() const
{
    return strTitle_;
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::GetStrippedTitle
// Created: HME 2005-11-29
// -----------------------------------------------------------------------------
inline
const std::string Report_ABC::GetStrippedTitle() const
{
    std::string strTitle = strTitle_;
    uint pos = strTitle_.find( " - " );
    if( pos >= 0 && pos < strTitle.size() )
        strTitle.resize( pos );
    return strTitle;
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::IsInteresting()
// Created: HME 2006-01-13
// -----------------------------------------------------------------------------
inline
bool Report_ABC::IsInteresting() const
{
    
    if( strTitle_ != GetStrippedTitle()
        || strTitle_ == "Ennemi détecté"
        || strTitle_ == "Ennemi reconnu"
        || strTitle_ == "Ennemi identifié" )
        return false;
    return true;
}


// -----------------------------------------------------------------------------
// Name: Report_ABC::GetTime
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
inline
int Report_ABC::GetTime() const
{
    return nTime_;
}


// -----------------------------------------------------------------------------
// Name: Report_ABC::IsNew
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
inline
bool Report_ABC::IsNew() const
{
    return bNew_;
}
    

// -----------------------------------------------------------------------------
// Name: Report_ABC::SetNew
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
inline
void Report_ABC::SetNew( bool bNew )
{
    bNew_ = bNew;
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::GetType
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
inline
Report_ABC::E_Type Report_ABC::GetType() const
{
    return eType_;
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::IsRCType
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
inline
bool Report_ABC::IsRCType() const
{
    return eType_ == eRC || eType_ == eMessage || eType_ == eEvent || eType_ == eWarning;
}