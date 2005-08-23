// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
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

//-----------------------------------------------------------------------------
// Name: ConcreteEntity::GetState
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
int32 ConcreteEntity::GetState() const
{
    switch( eOpState_ )
    {
    case EnumEtatOperationnel::detruit_totalement:
        return 0;
    case EnumEtatOperationnel::detruit_tactiquement:
        return 50;
    case EnumEtatOperationnel::operationnel:
        return 100;
    default:
        assert( false );
    }
    return 0;
}

} // end namespace TIC
