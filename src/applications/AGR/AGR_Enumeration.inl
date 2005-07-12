// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-04-26 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_Enumeration.inl $
// $Author: Agn $
// $Modtime: 26/04/04 11:33 $
// $Revision: 1 $
// $Workfile: AGR_Enumeration.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: AGR_Enumeration::GetName
/** @return 
*/
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
INLINE
const std::string& AGR_Enumeration::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: AGR_Enumeration::GetEnumValueSet
/** @return 
*/
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
INLINE
const T_String_Set& AGR_Enumeration::GetEnumValueSet() const
{
    return valueSet_;
}
