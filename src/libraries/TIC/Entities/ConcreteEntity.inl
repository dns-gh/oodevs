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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/ConcreteEntity.inl $
// $Author: Sbo $
// $Modtime: 19/07/05 16:31 $
// $Revision: 6 $
// $Workfile: ConcreteEntity.inl $
//
// *****************************************************************************

namespace TIC {

//-----------------------------------------------------------------------------
// Name: ConcreteEntity::GetDirection
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
T_Direction ConcreteEntity::GetDirection() const
{
	return nDirection_;
}

//-----------------------------------------------------------------------------
// Name: ConcreteEntity::GetSpeed
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
T_Speed ConcreteEntity::GetSpeed() const
{
	return rSpeed_;
}

//-----------------------------------------------------------------------------
// Name: ConcreteEntity::GetPosition
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
const Position& ConcreteEntity::GetPosition() const
{
	return position_;
}

//-----------------------------------------------------------------------------
// Name: ConcreteEntity::GetHeight
// Created: SBO 2005-06-02
//-----------------------------------------------------------------------------
inline
T_Height ConcreteEntity::GetHeight() const
{
	return nHeight_;
}

} // end namespace TIC
