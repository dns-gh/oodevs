// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-02-01 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_DynamicData.cpp $
// $Author: Age $
// $Modtime: 24/03/05 15:39 $
// $Revision: 3 $
// $Workfile: TER_DynamicData.cpp $
//
// *****************************************************************************

#include "ter_pch.h"
#include "TER_DynamicData.h"
#include "pathfind/TerrainRetractationHandle.h"
#pragma warning ( disable : 4275 )
#include "boost/thread/mutex.hpp"

// -----------------------------------------------------------------------------
// Name: TER_DynamicData constructor
// Created: AGE 2005-02-02
// -----------------------------------------------------------------------------
TER_DynamicData::TER_DynamicData()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_DynamicData constructor
// Created: AGE 2005-03-03
// -----------------------------------------------------------------------------
TER_DynamicData::TER_DynamicData( const TER_DynamicData& rhs )
    : handles_( rhs.handles_ )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_DynamicData destructor
// Created: AGE 2005-02-02
// -----------------------------------------------------------------------------
TER_DynamicData::~TER_DynamicData()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_DynamicData::Duplicate
// Created: AGE 2005-02-02
// -----------------------------------------------------------------------------
TER_DynamicData& TER_DynamicData::Duplicate()
{
    return *new TER_DynamicData( *this );
}

// -----------------------------------------------------------------------------
// Name: TER_DynamicData::AddRetractationHandle
// Created: AGE 2005-02-02
// -----------------------------------------------------------------------------
void TER_DynamicData::AddRetractationHandle( TerrainRetractationHandle& handle )
{
    handles_.push_back( T_Handle( &handle ) );
}
