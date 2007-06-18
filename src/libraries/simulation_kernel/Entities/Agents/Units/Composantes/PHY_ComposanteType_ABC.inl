// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Composantes/PHY_ComposanteType_ABC.inl $
// $Author: Jvt $
// $Modtime: 14/04/05 17:00 $
// $Revision: 2 $
// $Workfile: PHY_ComposanteType_ABC.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteType_ABC::GetMosID
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
inline
ASN1T_EquipmentType PHY_ComposanteType_ABC::GetMosID() const
{
    return nMosID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteType_ABC::GetProtection
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
inline
const PHY_Protection& PHY_ComposanteType_ABC::GetProtection() const
{
    assert( pProtection_ );
    return *pProtection_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteType_ABC::GetVolume
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
inline
const PHY_Volume& PHY_ComposanteType_ABC::GetVolume() const
{
    assert( pVolume_ );
    return *pVolume_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteType_ABC::GetName
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
inline
const std::string& PHY_ComposanteType_ABC::GetName() const
{
    return strName_;
}
