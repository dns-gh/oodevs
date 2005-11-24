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

#include "Types/Entities/PawnType.h"
#include "Automat.h"

namespace TEST
{

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
const Automat* Pawn::GetAutomat() const
{
    assert( pAutomat_ ); // Pawn should have an automat
    return pAutomat_;
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetDirection
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
T_Direction Pawn::GetDirection() const
{
	return nDirection_;
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetSpeed
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
T_Speed Pawn::GetSpeed() const
{
	return rSpeed_;
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetPosition
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
const Position& Pawn::GetPosition() const
{
	return position_;
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetHeight
// Created: SBO 2005-06-02
//-----------------------------------------------------------------------------
inline
T_Height Pawn::GetHeight() const
{
	return nHeight_;
}

//-----------------------------------------------------------------------------
// Name: Pawn::GetState
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
int Pawn::GetState() const
{
	return nOpState_;
}

} // end namespace TEST
