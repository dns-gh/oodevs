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
// Name: MOS_Object_ItineraireLogistique::IsEquipped
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
bool MOS_Object_ItineraireLogistique::IsEquipped() const
{
    return bEquipped_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Object_ItineraireLogistique::GetFlow
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint MOS_Object_ItineraireLogistique::GetFlow() const
{
    return nFlow_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Object_ItineraireLogistique::GetWidth
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint MOS_Object_ItineraireLogistique::GetWidth() const
{
    return nWidth_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Object_ItineraireLogistique::GetLength
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint MOS_Object_ItineraireLogistique::GetLength() const
{
    return nLength_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Object_ItineraireLogistique::GetMaxWeight
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
inline
uint MOS_Object_ItineraireLogistique::GetMaxWeight() const
{
    return nMaxWeight_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_ItineraireLogistique::SetFlow
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void MOS_Object_ItineraireLogistique::SetFlow( uint nFlow )
{
    nFlow_ = nFlow;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Object_ItineraireLogistique::SetWidth
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void MOS_Object_ItineraireLogistique::SetWidth( uint nWidth )
{
    nWidth_ = nWidth;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Object_ItineraireLogistique::SetLength
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void MOS_Object_ItineraireLogistique::SetLength( uint nLength )
{
    nLength_ = nLength;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Object_ItineraireLogistique::SetMaxWeight
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void MOS_Object_ItineraireLogistique::SetMaxWeight( uint nMaxWeight )
{
    nMaxWeight_ = nMaxWeight;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Object_ItineraireLogistique::SetEquipped
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
void MOS_Object_ItineraireLogistique::SetEquipped( bool bEquipped  )
{
    bEquipped_ = bEquipped;
}
