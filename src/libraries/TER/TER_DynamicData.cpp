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

#include "TER_PathFinderThread.h"
#include "pathfind/TerrainRetractationHandle.h"

// -----------------------------------------------------------------------------
// Name: TER_DynamicData constructor
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
TER_DynamicData::TER_DynamicData( const T_PointVector& points, const TerrainData& terrainData )
    : handles_    ()
    , nNbrRefs_   ( 0 )
    , points_     ( points )
    , terrainData_( terrainData )
{
    
}

// -----------------------------------------------------------------------------
// Name: TER_DynamicData destructor
// Created: AGE 2005-02-02
// -----------------------------------------------------------------------------
TER_DynamicData::~TER_DynamicData()
{
    assert( handles_.empty() );
    assert( nNbrRefs_ == 0 );
}

// -----------------------------------------------------------------------------
// Name: TER_DynamicData::AddForRegistration
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_DynamicData::AddForRegistration( TER_PathFinderThread& thread )
{
    ++ nNbrRefs_;
    thread.AddDynamicDataToRegister( *this );    
}

// -----------------------------------------------------------------------------
// Name: TER_DynamicData::RegisterDynamicData
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_DynamicData::RegisterDynamicData( TER_PathFinderThread& thread )
{
    bool bOut = handles_.insert( std::make_pair( &thread, &thread.CreateLineTree( points_, terrainData_ ) ) ).second;
    assert( bOut );
    assert( nNbrRefs_ > 0 );
    -- nNbrRefs_;
}

// -----------------------------------------------------------------------------
// Name: TER_DynamicData::AddForUnregistration
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_DynamicData::AddForUnregistration( TER_PathFinderThread& thread )
{
    ++ nNbrRefs_;
    thread.AddDynamicDataToUnregister( *this );    
}

// -----------------------------------------------------------------------------
// Name: TER_DynamicData::UnregisterDynamicData
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_DynamicData::UnregisterDynamicData( TER_PathFinderThread& thread )
{
    IT_HandleMap it = handles_.find( &thread );
    assert( it != handles_.end() );
    delete it->second;
    handles_.erase( it );

    assert( nNbrRefs_ > 0 );
    -- nNbrRefs_;

    if( nNbrRefs_ == 0 && handles_.empty() )
        delete this;
}

