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
// Name: MOS_DynaObject_ItineraireLogistique::IsEquipped
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
bool MOS_DynaObject_ItineraireLogistique::IsEquipped() const
{
    return bEquipped_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ItineraireLogistique::GetFlow
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint MOS_DynaObject_ItineraireLogistique::GetFlow() const
{
    return nFlow_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ItineraireLogistique::GetWidth
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint MOS_DynaObject_ItineraireLogistique::GetWidth() const
{
    return nWidth_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ItineraireLogistique::GetLength
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint MOS_DynaObject_ItineraireLogistique::GetLength() const
{
    return nLength_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ItineraireLogistique::GetMaxWeight
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint MOS_DynaObject_ItineraireLogistique::GetMaxWeight() const
{
    return nMaxWeight_;
}
