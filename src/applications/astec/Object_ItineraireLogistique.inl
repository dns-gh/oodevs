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
// Name: Object_ItineraireLogistique::IsEquipped
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
bool Object_ItineraireLogistique::IsEquipped() const
{
    return bEquipped_;
}
    
// -----------------------------------------------------------------------------
// Name: Object_ItineraireLogistique::GetFlow
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint Object_ItineraireLogistique::GetFlow() const
{
    return nFlow_;
}
    
// -----------------------------------------------------------------------------
// Name: Object_ItineraireLogistique::GetWidth
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint Object_ItineraireLogistique::GetWidth() const
{
    return nWidth_;
}
    
// -----------------------------------------------------------------------------
// Name: Object_ItineraireLogistique::GetLength
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint Object_ItineraireLogistique::GetLength() const
{
    return nLength_;
}
    
// -----------------------------------------------------------------------------
// Name: Object_ItineraireLogistique::GetMaxWeight
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint Object_ItineraireLogistique::GetMaxWeight() const
{
    return nMaxWeight_;
}

// -----------------------------------------------------------------------------
// Name: Object_ItineraireLogistique::SetFlow
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void Object_ItineraireLogistique::SetFlow( uint nFlow )
{
    nFlow_ = nFlow;
}
    
// -----------------------------------------------------------------------------
// Name: Object_ItineraireLogistique::SetWidth
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void Object_ItineraireLogistique::SetWidth( uint nWidth )
{
    nWidth_ = nWidth;
}
    
// -----------------------------------------------------------------------------
// Name: Object_ItineraireLogistique::SetLength
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void Object_ItineraireLogistique::SetLength( uint nLength )
{
    nLength_ = nLength;
}
    
// -----------------------------------------------------------------------------
// Name: Object_ItineraireLogistique::SetMaxWeight
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void Object_ItineraireLogistique::SetMaxWeight( uint nMaxWeight )
{
    nMaxWeight_ = nMaxWeight;
}
    
// -----------------------------------------------------------------------------
// Name: Object_ItineraireLogistique::SetEquipped
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void Object_ItineraireLogistique::SetEquipped( bool bEquipped  )
{
    bEquipped_ = bEquipped;
}
