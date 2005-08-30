// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-18 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Export/Pawn.cpp $
// $Author: Sbo $
// $Modtime: 19/07/05 16:33 $
// $Revision: 23 $
// $Workfile: Pawn.cpp $
//
// *****************************************************************************

#include "TIC/Types.h"
#include "Pawn.h"
#include "Platform.h"
#include "TIC/Entities/Pawn.h"
#include "TIC/Entities/Platform.h"

#include "TIC/Entities/Natures/Pawn/NatureCategory.h"
#include "TIC/Entities/Natures/Pawn/NatureLevel.h"
#include "TIC/Entities/Natures/Pawn/NatureSpecialization.h"
#include "TIC/Entities/Natures/Pawn/NatureWeapon.h"
#include "TIC/Entities/Natures/Pawn/NatureMobility.h"
#include "TIC/Entities/Natures/Pawn/NatureQualifier.h"
#include "TIC/Entities/Natures/Pawn/NatureCapacity.h"

using namespace TICExport;


namespace TICExport
{

//-----------------------------------------------------------------------------
// Name: Pawn::Pawn
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
Pawn::Pawn()
: pPawn_ ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: Pawn::Pawn
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
Pawn::Pawn( const TIC::Pawn& pawn )
: pPawn_ ( &pawn )
{
}

//-----------------------------------------------------------------------------
// Name: Pawn::~Pawn
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
Pawn::~Pawn()
{
}

//-----------------------------------------------------------------------------
// Name: Pawn::Pawn
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
unsigned int Pawn::GetId() const
{
    assert( pPawn_ );
    return pPawn_->GetId();
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetNatureName
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
const std::string& Pawn::GetNatureName() const
{
    assert( pPawn_ );
    return pPawn_->GetType();
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetNatureLevel
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
const std::string& Pawn::GetNatureLevel() const
{
    assert( pPawn_ );
    return pPawn_->GetNatureLevel().GetKey();
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetNatureCategory
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
const std::string& Pawn::GetNatureCategory() const
{
    assert( pPawn_ );
    return pPawn_->GetNatureCategory().GetKey();
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetNatureSpecialization
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
const std::string& Pawn::GetNatureSpecialization() const
{
    assert( pPawn_ );
    return pPawn_->GetNatureSpecialization().GetKey();
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetNatureWeapon
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
const std::string& Pawn::GetNatureWeapon() const
{
    assert( pPawn_ );
    return pPawn_->GetNatureWeapon().GetKey();
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetNatureMobility
// Created: SBO 2005-06-14
//-----------------------------------------------------------------------------
const std::string& Pawn::GetNatureMobility() const
{
    assert( pPawn_ );
    return pPawn_->GetNatureMobility().GetKey();
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetNatureQualifier
// Created: SBO 2005-06-14
//-----------------------------------------------------------------------------
const std::string& Pawn::GetNatureQualifier() const
{
    assert( pPawn_ );
    return pPawn_->GetNatureQualifier().GetKey();
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetNatureCapacity
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
const std::string& Pawn::GetNatureCapacity() const
{
    assert( pPawn_ );
    return pPawn_->GetNatureCapacity().GetKey();
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetDirection
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
int Pawn::GetDirection() const
{
    assert( pPawn_ );
    return pPawn_->GetDirection();
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetSpeed
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
unsigned int Pawn::GetSpeed() const
{
    assert( pPawn_ );
    return ( unsigned int )pPawn_->GetSpeed();
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetLatitude
// Created: SBO 2005-06-06
//-----------------------------------------------------------------------------
double Pawn::GetLatitude() const
{
    assert( pPawn_ );
    return pPawn_->GetPosition().GetLatitude();
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetLongitude
// Created: SBO 2005-06-06
//-----------------------------------------------------------------------------
double Pawn::GetLongitude() const
{
    assert( pPawn_ );
    return pPawn_->GetPosition().GetLongitude();
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetHeight
// Created: SBO 2005-06-03
//-----------------------------------------------------------------------------
int Pawn::GetHeight() const
{
    assert( pPawn_ );
    return pPawn_->GetHeight();
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetState
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
Pawn::EState Pawn::GetState() const
{
    assert( pPawn_ );
    return ( Pawn::EState )( pPawn_->GetState() );
}

//-----------------------------------------------------------------------------
// Name: Pawn::IsPc
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
bool Pawn::IsPc() const
{
    assert( pPawn_ );
    return pPawn_->IsPc();
}

//-----------------------------------------------------------------------------
// Name: Pawn::IsAggregated
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
bool Pawn::IsAggregated() const
{
    assert( pPawn_ );
    return pPawn_->IsAggregated();
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetPlatformCount
// Created: SBO 2005-05-27
//-----------------------------------------------------------------------------
unsigned int Pawn::GetPlatformCount() const
{
    assert( pPawn_ );
    return pPawn_->GetPlatformCount();
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetPlatformAt
// Created: SBO 2005-05-27
//-----------------------------------------------------------------------------
const Platform Pawn::GetPlatformAt( unsigned int nIdx ) const
{
    const TIC::Platform* pPlatform = pPawn_->GetPlatformAt( nIdx );
    assert( pPlatform );
    return Platform( *pPlatform );
}

} // namespace TICExport