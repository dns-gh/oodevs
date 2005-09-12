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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Report_ABC.inl $
// $Author: Ape $
// $Modtime: 5/08/04 17:47 $
// $Revision: 2 $
// $Workfile: MOS_Report_ABC.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_Report_ABC::GetPos
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
MOS_INLINE
const MT_Vector2D& MOS_Report_ABC::GetPos() const
{
    return vPos_;
}
    

// -----------------------------------------------------------------------------
// Name: MOS_Report_ABC::GetTitle
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
MOS_INLINE
const std::string& MOS_Report_ABC::GetTitle() const
{
    return strTitle_;
}
    

// -----------------------------------------------------------------------------
// Name: MOS_Report_ABC::GetTime
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
MOS_INLINE
int MOS_Report_ABC::GetTime() const
{
    return nTime_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Report_ABC::IsNew
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_Report_ABC::IsNew() const
{
    return bNew_;
}
    

// -----------------------------------------------------------------------------
// Name: MOS_Report_ABC::SetNew
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_Report_ABC::SetNew( bool bNew )
{
    bNew_ = bNew;
}

// -----------------------------------------------------------------------------
// Name: MOS_Report_ABC::GetType
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_Report_ABC::E_Type MOS_Report_ABC::GetType() const
{
    return eType_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Report_ABC::IsRCType
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_Report_ABC::IsRCType() const
{
    return eType_ == eRC || eType_ == eMessage || eType_ == eEvent || eType_ == eWarning;
}