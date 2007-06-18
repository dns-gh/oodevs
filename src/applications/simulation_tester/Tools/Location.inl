// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-10 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

namespace TEST {

// -----------------------------------------------------------------------------
// Name: Location::GetType
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
inline
ASN1T_EnumLocationType Location::GetType() const
{
    return eType_;
}
    
// -----------------------------------------------------------------------------
// Name: Location::GetPositionVector
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
inline
const T_PositionVector& Location::GetPositionVector() const
{
    return points_;
}

} // end namespace TEST