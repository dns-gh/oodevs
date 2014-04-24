// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_PathFindRequest.h"
#include "DEC_PathFind_Manager.h"
#include "DEC_Path_ABC.h"
#include "simulation_terrain/TER_PathFinder_ABC.h"

DEC_PathFindRequest::DEC_PathFindRequest( DEC_PathFind_Manager* m, const boost::shared_ptr< DEC_Path_ABC >& p )
    : manager_( m )
    , path_( p )
{
    // NOTHING
}

DEC_PathFindRequest::~DEC_PathFindRequest()
{
    // NOTHING
}

void DEC_PathFindRequest::FindPath( TER_Pathfinder_ABC& pathfind )
{
    profiler_.Start();
    path_->Execute( pathfind );
    manager_->CleanPathAfterComputation( path_, profiler_.Stop() );
}

const boost::shared_ptr< DEC_Path_ABC >& DEC_PathFindRequest::GetPath() const
{
    return path_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFindRequest::IsPathForUnit
// Created: JSR 2013-03-11
// -----------------------------------------------------------------------------
bool DEC_PathFindRequest::IsPathForUnit( MIL_Agent_ABC* pion ) const
{
    return path_.get() && path_->IsPathForUnit( pion );
}
