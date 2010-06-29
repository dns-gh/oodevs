// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_TerrainAnalysis.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Entities/Orders/MIL_PionOrderManager.h"

#include "simulation_terrain/TER_PathFindManager.h"


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_TerrainAnalysis constructor
// Created: MGD 2010-04-20
// -----------------------------------------------------------------------------
PHY_RolePion_TerrainAnalysis::PHY_RolePion_TerrainAnalysis( MIL_Agent_ABC& pion )
    : pion_( pion )
    , lastPos_()
{

}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_TerrainAnalysis destructor
// Created: MGD 2010-04-20
// -----------------------------------------------------------------------------
PHY_RolePion_TerrainAnalysis::~PHY_RolePion_TerrainAnalysis()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_TerrainAnalysis::GetCrossroads
// Created: MGD 2010-04-20
// -----------------------------------------------------------------------------
void PHY_RolePion_TerrainAnalysis::GetCrossroads( std::vector< boost::shared_ptr< MT_Vector2D > >& points ) const
{
    for( std::map< MT_Vector2D, boost::shared_ptr< MT_Vector2D > >::const_iterator it = crossroadsBuffer_.begin(); it != crossroadsBuffer_.end(); it++ )
    {
        points.push_back( it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_TerrainAnalysis::Update
// Created: MGD 2010-04-20
// -----------------------------------------------------------------------------
void PHY_RolePion_TerrainAnalysis::NotifyHasMove( const MT_Vector2D& newPos )
{
    double range = std::max( pion_.GetRole< PHY_RoleInterface_Perceiver >().GetMaxAgentPerceptionDistance(), 3000. );
    if( lastPos_.SquareDistance( newPos ) > range*0.5 * range*0.5 )
    {
        //Remove old points out of range
        for( std::map< MT_Vector2D, boost::shared_ptr< MT_Vector2D > >::iterator it = crossroadsBuffer_.begin(); it != crossroadsBuffer_.end(); )
        {
            if( it->second->SquareDistance( newPos ) < range * range )
                it = crossroadsBuffer_.erase( it );
            else
                it++;
        }

        //Add new points
        std::vector< boost::shared_ptr< MT_Vector2D > > temp = TER_PathFindManager::GetPathFindManager().FindCrossroadsWithinCircle( newPos, range );
        for( std::vector< boost::shared_ptr< MT_Vector2D > >::const_iterator it = temp.begin(); it != temp.end(); it++ )
        {
            crossroadsBuffer_.insert( std::pair< MT_Vector2D, boost::shared_ptr< MT_Vector2D > >( **it, *it ) );
        }


        //Remove outside fuseau
        const MIL_Fuseau& fuseau = pion_.GetOrderManager().GetFuseau();
        if( !fuseau.IsNull() )
        {
            for( std::map< MT_Vector2D, boost::shared_ptr< MT_Vector2D > >::iterator it = crossroadsBuffer_.begin(); it != crossroadsBuffer_.end(); )
            {

                if( fuseau.IsInside( it->first ) )
                    it = crossroadsBuffer_.erase( it );
                else
                    it++;
            }
        }

        lastPos_ = newPos;
    }
}
