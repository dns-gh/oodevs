// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Pawn.inl $
// $Author: Sbo $
// $Modtime: 21/07/05 17:05 $
// $Revision: 24 $
// $Workfile: Pawn.inl $
//
// *****************************************************************************

#include "Natures/Pawn/NatureCategory.h"
#include "Natures/Pawn/NatureLevel.h"
#include "Natures/Pawn/NatureSpecialization.h"
#include "Natures/Pawn/NatureWeapon.h"
#include "Natures/Pawn/NatureMobility.h"
#include "Natures/Pawn/NatureQualifier.h"
#include "Types/PawnType.h"
#include "Platform.h"

#include "Entities/Automat.h"

namespace TIC
{

//-----------------------------------------------------------------------------
// Name: Pawn::Find
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
Pawn* Pawn::Find( T_EntityId nId )
{
	CIT_PawnMap it;
	it = pawns_.find( nId );
	if ( it != pawns_.end() )
		return ( *it ).second;
	return 0;
}

//-----------------------------------------------------------------------------
// Name: Pawn::Register
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
void Pawn::Register( Pawn& pawn )
{
    bool bOut = pawns_.insert( std::make_pair( pawn.GetId(), &pawn ) ).second;
    assert( bOut );
}

//-----------------------------------------------------------------------------
// Name: Pawn::Unregister
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
void Pawn::Unregister( Pawn& pawn )
{
    int nOut = pawns_.erase( pawn.GetId() );
    assert( nOut == 1 );
}

//-----------------------------------------------------------------------------
// Name: Pawn::AddDisaggregation
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
inline
void Pawn::AddDisaggregation( T_EntityId nId )
{
    bool bOut = disaggregPawns_.insert( nId ).second;
    assert( bOut );
}

//-----------------------------------------------------------------------------
// Name: Pawn::MustBeDisaggregated
// Created: SBO 2005-05-24
//-----------------------------------------------------------------------------
inline
bool Pawn::MustBeDisaggregated() const
{
    return ( bConfigDisaggreg_ || pAutomat_->MustBeDisaggregated() ); // disaggreg if config-file specified
}

//-----------------------------------------------------------------------------
// Name: Pawn::IsPc
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
bool Pawn::IsPc() const
{
	return bIsPc_;
}

//-----------------------------------------------------------------------------
// Name: Pawn::IsAggregated
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
bool Pawn::IsAggregated() const
{
	return bIsAggregated_;
}

//-----------------------------------------------------------------------------
// Name: Pawn::IsLoaded
// Created: SBO 2005-07-21
//-----------------------------------------------------------------------------
inline
bool Pawn::IsLoaded() const
{
    return bIsLoaded_;
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetId
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
T_EntityId Pawn::GetId() const
{
	return nId_;
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetName
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
const std::string& Pawn::GetName() const
{
	return strName_;
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetType
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const std::string& Pawn::GetType() const
{
    assert( pType_ );
    return pType_->GetName();
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetState
// Created: SBO 2005-09-14
// -----------------------------------------------------------------------------
inline
Pawn::EState Pawn::GetState() const
{
    return eState_;
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetNatureLevel
// Created: SBO 2005-05-13
// -----------------------------------------------------------------------------
inline
const NatureLevel& Pawn::GetNatureLevel() const
{
    assert( pType_ );
    return pType_->GetNatureLevel();
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetNatureWeapon
// Created: SBO 2005-05-13
// -----------------------------------------------------------------------------
inline
const NatureWeapon& Pawn::GetNatureWeapon() const
{
    assert( pType_ );
    return pType_->GetNatureWeapon();
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetNatureSpecialization
// Created: SBO 2005-05-13
// -----------------------------------------------------------------------------
inline
const NatureSpecialization& Pawn::GetNatureSpecialization() const
{
    assert( pType_ );
    return pType_->GetNatureSpecialization();
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetNatureCategory
// Created: SBO 2005-05-13
// -----------------------------------------------------------------------------
inline
const NatureCategory& Pawn::GetNatureCategory() const
{
    assert( pType_ );
    return pType_->GetNatureCategory();
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetNatureMobility
// Created: SBO 2005-06-14
// -----------------------------------------------------------------------------
inline
const NatureMobility& Pawn::GetNatureMobility() const
{
    assert( pType_ );
    return pType_->GetNatureMobility();
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetNatureQualifier
// Created: SBO 2005-06-14
// -----------------------------------------------------------------------------
inline
const NatureQualifier& Pawn::GetNatureQualifier() const
{
    assert( pType_ );
    return pType_->GetNatureQualifier();
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetNatureCapacity
// Created: SBO 2005-06-20
// -----------------------------------------------------------------------------
inline
const NatureCapacity& Pawn::GetNatureCapacity() const
{
    assert( pType_ );
    return pType_->GetNatureCapacity();
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetAutomat
// Created: SBO 2005-05-25
// -----------------------------------------------------------------------------
inline
const Automat& Pawn::GetAutomat() const
{
    assert( pAutomat_ );
    return *pAutomat_;
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetPlatformCount
// Created: SBO 2005-05-27
//-----------------------------------------------------------------------------
inline
uint Pawn::GetPlatformCount() const
{
    if( bIsAggregated_ )
        return 0;

    uint nCount = 0;
    for( CIT_PlatformVector it = childPlatforms_.begin(); it != childPlatforms_.end(); ++it )
        if( ( *it )->MustBeDisaggregated() )
            nCount++;
    return nCount;
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetPlatformAt
// Created: SBO 2005-05-27
//-----------------------------------------------------------------------------
inline
const Platform* Pawn::GetPlatformAt( uint nIdx ) const
{
    if( bIsAggregated_ )
        return 0;

    uint nCount = 0;
    for( CIT_PlatformVector it = childPlatforms_.begin(); it != childPlatforms_.end(); ++it )
        if( ( *it )->MustBeDisaggregated() )
            if( ++nCount > nIdx )
                break;
    return ( *it );
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetPlatformAt
// Created: SBO 2005-05-27
//-----------------------------------------------------------------------------
inline
const Path& Pawn::GetPath() const
{
    return path_;
}

} // end namespace TIC
