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

#include "Types/PawnType.h"

#include "Entities/Automat.h"
#include "Entities/Types/PawnType.h"

namespace TEST
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
// Name: Pawn::IsPc
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
bool Pawn::IsPc() const
{
	return bIsPc_;
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
const PawnType& Pawn::GetType() const
{
    assert( pType_ );
    return *pType_;
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

} // end namespace TEST
