// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleAction_FolkInfluence.h"

#include "MIL_AgentServer.h"
#include "MIL_Folk.h"
#include "folk/ObjectManager_ABC.h"
#include "folk/Object_ABC.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleAction_FolkInfluence, "PHY_RoleAction_FolkInfluence" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_FolkInfluence constructor
// Created: JCR 2007-09-12
// -----------------------------------------------------------------------------
PHY_RoleAction_FolkInfluence::PHY_RoleAction_FolkInfluence()
    : pManager_  ( MIL_AgentServer::GetWorkspace().GetFolkManager().GetObjectManager() )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_FolkInfluence destructor
// Created: JCR 2007-09-12
// -----------------------------------------------------------------------------
PHY_RoleAction_FolkInfluence::~PHY_RoleAction_FolkInfluence()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_FolkInfluence::serialize
// Created: JCR 2007-09-14
// -----------------------------------------------------------------------------
//template< typename Archive > 
//void PHY_RoleAction_FolkInfluence::serialize( Archive&, const uint )
//{
//     // $$$$ JCR 2007-09-14: TODO
//}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_FolkInfluence::Update
// Created: JCR 2007-09-14
// -----------------------------------------------------------------------------
void PHY_RoleAction_FolkInfluence::Update( bool /*bUpdate*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_FolkInfluence::Clean
// Created: JCR 2007-09-14
// -----------------------------------------------------------------------------
void PHY_RoleAction_FolkInfluence::Clean()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_FolkInfluence::InfluenceActivity
// Created: JCR 2007-09-13
// -----------------------------------------------------------------------------
population::Object_ABC* PHY_RoleAction_FolkInfluence::InfluenceActivity( const MT_Vector2D& vPos, const std::string& activity, double influence, int connectivity )
{
    population::Object_ABC*  pObject = 0;
    if ( pManager_ )
    {
        geometry::Point3d   pos( vPos.rX_, vPos.rY_, 0. );        
        pObject = pManager_->Create( pos );
        if ( !pObject )
            return 0;
        if ( connectivity == 0 )
        {        
            population::Object_ABC::T_PositionArea area;
            area.push_back( pos );
            pObject->LoadInfluence( activity, influence, area );
        }
        else
            pObject->LoadInfluence( activity, influence, pos, connectivity );        
    }
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_FolkInfluence::InfluenceActivityInArea
// Created: JCR 2007-09-13
// -----------------------------------------------------------------------------
population::Object_ABC* PHY_RoleAction_FolkInfluence::InfluenceActivityInArea( const MT_Vector2D& vPos, const std::string& activity, double influence, const TER_Localisation& area  )
{
    population::Object_ABC*  pObject = 0;
    if ( pManager_ )
    {
        geometry::Point3d       pos( vPos.rX_, vPos.rY_, 0. );
        pObject = pManager_->Create( pos );
        if ( !pObject )
            return pObject;
        switch ( area.GetType() )
        {
        case TER_Localisation::eCircle:
            {
                const MT_Vector2D& vCenter = area.GetCircleCenter();
                geometry::Point3d  center( vCenter.rX_, vCenter.rY_, 0. );
                pObject->LoadInfluence( activity, influence, center, area.GetCircleRadius() );
                break;
            }
        case TER_Localisation::ePolygon:
            {
                const T_PointVector& points = area.GetPoints();
                population::Object_ABC::T_PositionArea  to;
                for ( T_PointVector::const_iterator it = points.begin(); it != points.end(); ++it )
                    to.push_back( geometry::Point3d( it->rX_, it->rY_, 0. ) );                
                pObject->LoadInfluence( activity, influence, to );
                break;
            }
        }        
    }
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_FolkInfluence::ReleaseInfluence
// Created: JCR 2007-09-13
// -----------------------------------------------------------------------------
void PHY_RoleAction_FolkInfluence::ReleaseInfluence( population::Object_ABC& object )
{
    if ( pManager_ )
        pManager_->Release( object.GetID() );
}
