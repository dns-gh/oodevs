//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ItineraireLogistique.inl $
// $Author: Nld $
// $Modtime: 27/04/05 12:08 $
// $Revision: 3 $
// $Workfile: MIL_ItineraireLogistique.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_ItineraireLogistique::Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
{
    return *new MIL_ItineraireLogistique( type, nID, army );
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::IsEquipped
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
inline
bool MIL_ItineraireLogistique::IsEquipped() const
{
    return bEquipped_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::Equip
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
inline
void MIL_ItineraireLogistique::Equip()
{
    if( bEquipped_ )
        return;

    bEquipped_ = true;
    NotifyAttributeUpdated( eAttrUpdate_SpecificAttributes );
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::GetWeightSupported
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
inline
MT_Float MIL_ItineraireLogistique::GetWeightSupported() const
{
    return rWeightSupported_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::GetWidth
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
inline
MT_Float MIL_ItineraireLogistique::GetWidth() const
{
    return rWidth_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::GetLength
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
inline
MT_Float MIL_ItineraireLogistique::GetLength() const
{
    return rLength_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::GetFlow
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
inline
MT_Float MIL_ItineraireLogistique::GetFlow() const
{
    return rFlow_;
}
