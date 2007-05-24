// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ObstacleType.h"

const MIL_ObstacleType MIL_ObstacleType::reserved_( "reserved", MIL_ObstacleType::eReserved, EnumTypeObstacle::de_manoeuvre );
const MIL_ObstacleType MIL_ObstacleType::initial_ ( "initial" , MIL_ObstacleType::eInitial, EnumTypeObstacle::preliminaire );

// -----------------------------------------------------------------------------
// Name: MIL_ObstacleType constructor
// Created: NLD 2007-05-22
// -----------------------------------------------------------------------------
MIL_ObstacleType::MIL_ObstacleType( const std::string& strName, E_Type nType, ASN1T_EnumTypeObstacle nAsnID )
    : nType_  ( nType )
    , nAsnID_ ( nAsnID )
    , strName_( strName )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObstacleType destructor
// Created: NLD 2007-05-22
// -----------------------------------------------------------------------------
MIL_ObstacleType::~MIL_ObstacleType()
{
    // NOTHING
}
