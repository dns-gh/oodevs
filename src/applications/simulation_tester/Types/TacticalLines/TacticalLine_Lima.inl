// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-09 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

namespace TEST {

// -----------------------------------------------------------------------------
// Name: TacticalLine_Lima::GetLineType
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
inline
TacticalLine_ABC::E_LineType TacticalLine_Lima::GetLineType() const
{
    return TacticalLine_ABC::eLima;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_Lima::GetLimaType
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
inline
ASN1T_EnumLimaType TacticalLine_Lima::GetLimaType() const
{
    return eLimaType_;
}

} // end namespace TEST
