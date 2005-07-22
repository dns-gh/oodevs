// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-17 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Types/PawnType.inl $
// $Author: Sbo $
// $Modtime: 13/07/05 15:06 $
// $Revision: 3 $
// $Workfile: PawnType.inl $
//
// *****************************************************************************

namespace TIC
{

// -----------------------------------------------------------------------------
// Name: PawnType::Find
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const PawnType* PawnType::Find( const std::string& strName )
{
    CIT_PawnTypeMap it = pawnTypes_.find( strName );
    return it == pawnTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PawnType::Find
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const PawnType* PawnType::Find( ASN1T_TypePion asnId )
{
    for( CIT_PawnTypeMap it = pawnTypes_.begin(); it != pawnTypes_.end(); ++it )
        if( ( *it ).second->GetId() == asnId )
            return ( *it ).second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PawnType::GetName
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const std::string& PawnType::GetName() const
{
    return strName_;
}
          
// -----------------------------------------------------------------------------
// Name: PawnType::GetId
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
uint PawnType::GetId() const
{
    return nId_;
}

// -----------------------------------------------------------------------------
// Name: PawnType::GetNatureLevel
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const NatureLevel&  PawnType::GetNatureLevel() const
{
    assert( pLevel_ );
    return *pLevel_;
}

// -----------------------------------------------------------------------------
// Name: PawnType::GetNatureCategory
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const NatureCategory&  PawnType::GetNatureCategory() const
{
    assert( pCategory_ );
    return *pCategory_;
}

// -----------------------------------------------------------------------------
// Name: PawnType::GetNatureSpecialization
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const NatureSpecialization&  PawnType::GetNatureSpecialization() const
{
    assert( pSpecialization_ );
    return *pSpecialization_;
}

// -----------------------------------------------------------------------------
// Name: PawnType::GetNatureWeapon
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const NatureWeapon&  PawnType::GetNatureWeapon() const
{
    assert( pWeapon_ );
    return *pWeapon_;
}

// -----------------------------------------------------------------------------
// Name: PawnType::GetNatureMobility
// Created: SBO 2005-06-14
// -----------------------------------------------------------------------------
inline
const NatureMobility&  PawnType::GetNatureMobility() const
{
    assert( pMobility_ );
    return *pMobility_;
}

// -----------------------------------------------------------------------------
// Name: PawnType::GetNatureQualifier
// Created: SBO 2005-06-14
// -----------------------------------------------------------------------------
inline
const NatureQualifier&  PawnType::GetNatureQualifier() const
{
    assert( pQualifier_ );
    return *pQualifier_;
}

// -----------------------------------------------------------------------------
// Name: PawnType::GetNatureCapacity
// Created: SBO 2005-06-20
// -----------------------------------------------------------------------------
inline
const NatureCapacity&  PawnType::GetNatureCapacity() const
{
    assert( pCapacity_ );
    return *pCapacity_;
}

} // end namespace TIC