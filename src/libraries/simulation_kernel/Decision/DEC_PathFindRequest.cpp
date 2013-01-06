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

DEC_PathFindRequest::DEC_PathFindRequest( DEC_PathFind_Manager* m,
        const boost::shared_ptr< DEC_Path_ABC > p )
    : manager_( m )
    , path_( p )
{
}

DEC_PathFindRequest::~DEC_PathFindRequest()
{
}

void DEC_PathFindRequest::FindPath( TER_Pathfinder_ABC& pathfind )
{
    path_->Execute( pathfind );
}

void DEC_PathFindRequest::CleanAfterComputation()
{
    manager_->CleanPathAfterComputation( path_ );
}

const boost::shared_ptr< DEC_Path_ABC >& DEC_PathFindRequest::GetPath() const
{
    return path_;
}
