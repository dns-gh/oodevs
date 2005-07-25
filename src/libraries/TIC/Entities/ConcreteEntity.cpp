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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/ConcreteEntity.cpp $
// $Author: Sbo $
// $Modtime: 19/07/05 16:31 $
// $Revision: 5 $
// $Workfile: ConcreteEntity.cpp $
//
// *****************************************************************************

#include "TIC_Pch.h"
#include "Entities/ConcreteEntity.h"
#include "Tools/Position.h"

using namespace TIC;

//-----------------------------------------------------------------------------
// Name: ConcreteEntity::ConcreteEntity
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
ConcreteEntity::ConcreteEntity()
    : position_   ()
    , nDirection_ ( 0 )
    , nHeight_    ( 0 )
    , rSpeed_     ( 0. )
    , nOpState_   ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: ConcreteEntity::~ConcreteEntity
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
ConcreteEntity::~ConcreteEntity()
{
}