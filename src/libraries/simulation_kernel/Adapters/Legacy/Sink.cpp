// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "Sink.h"
#include "Adapters/SinkRoleExtender.h"
#include "propagation/FloodModel.h"
#include "propagation/ElevationGetter_ABC.h"
#include "RolePion_Decision.h"
#include "NullRoleAdapter.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "MissionController_ABC.h"
#include "Tools/MIL_Color.h"
#include "Tools/MIL_IDManager.h"
#include "MIL_AgentServer.h"
#include "CheckPoints/SerializationTools.h"
#include "MT_Tools/MT_FormatString.h"
#include "simulation_terrain/TER_World.h"
#include <boost/functional/factory.hpp>

using namespace sword::legacy;

BOOST_CLASS_EXPORT_IMPLEMENT( sword::legacy::Sink )

namespace
{
    class ElevationGetter : public propagation::ElevationGetter_ABC
    {
    public:
        ElevationGetter()
        {
            // NOTHING
        }
        virtual ~ElevationGetter()
        {
            // NOTHING
        }
        virtual short GetElevationAt( const geometry::Point2f& point ) const
        {
            return static_cast< short >( MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetAltitude( point.X(), point.Y(), true ) );
        }
    };
}

namespace sword
{
namespace legacy
{
    template< typename Archive >
    void save_construct_data( Archive& archive, const sword::legacy::Sink* sink, const unsigned int /*version*/ )
    {
        const Sink::T_Elements& elements = sink->elements_;
        bool logEnabled = sink->decLogger_.get() != 0;
        const MIL_IDManager* const idManager = &sink->idManager_;
        const MissionController_ABC* const missionController = &sink->missionController_;
        archive << idManager
                << missionController
                << sink->gcPause_
                << sink->gcMult_
                << logEnabled
                << elements;
    }

    template< typename Archive >
    void load_construct_data( Archive& archive, sword::legacy::Sink* sink, const unsigned int /*version*/ )
    {
        unsigned int gcPause;
        unsigned int gcMult;
        bool logEnabled;
        Sink::T_Elements elements;
        MIL_IDManager* idManager;
        MissionController_ABC* missionController;
        archive >> idManager
                >> missionController
                >> gcPause
                >> gcMult
                >> logEnabled
                >> elements;
        ::new( sink )Sink( *idManager, *missionController, gcPause, gcMult,
                logEnabled, archive.GetWorld() );
        sink->elements_ = elements;
    }
}
}

// -----------------------------------------------------------------------------
// Name: Sink constructor
// Created: SLI 2012-01-13
// -----------------------------------------------------------------------------
Sink::Sink( MIL_IDManager& idManager, MissionController_ABC& missionController,
    unsigned int gcPause, unsigned int gcMult, bool logEnabled,
    const boost::shared_ptr< const TER_World >& world )
    : pElevation_( new ElevationGetter() )
    , gcPause_   ( gcPause )
    , gcMult_    ( gcMult )
    , decLogger_ ( logEnabled ? new DEC_Logger( "Pion" ) : 0 )
    , world_     ( world )
    , idManager_ ( idManager )
    , missionController_( missionController )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Sink destructor
// Created: SLI 2012-01-13
// -----------------------------------------------------------------------------
Sink::~Sink()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Sink::Clean
// Created: JSR 2013-01-30
// -----------------------------------------------------------------------------
void Sink::Clean()
{
    for( auto it = elements_.begin(); it != elements_.end(); )
    {
        MIL_AgentPion* pion = it->second;
        assert( pion );
        pion->Clean();
        if( pion->CanBeDeleted() )
        {
            it = elements_.erase( it );
            delete pion;
        }
        else
            ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: Sink::CreateRoles
// Created: AHC 2013-02-22
// -----------------------------------------------------------------------------
void Sink::CreateRoles( sword::SinkRoleExtender& ext )
{
    ext.AddFactory< sword::legacy::NullRoleAdapter >( boost::bind( boost::factory< sword::legacy::NullRoleAdapter* >() ) );
    ext.AddFactory< sword::legacy::RolePion_Decision >( boost::bind( boost::factory< sword::legacy::RolePion_Decision* >() ,_1, gcPause_, gcMult_, decLogger_.get() ) );
    ext.AddFactory< moving::PHY_RoleAction_Moving >( boost::bind( boost::factory< moving::PHY_RoleAction_Moving* >() ,_1 ) );
    ext.AddFactory< PHY_RolePion_Location >( boost::bind( boost::factory< PHY_RolePion_Location* >() ,_1 ) );
    ext.AddFactory< PHY_RolePion_Perceiver >( boost::bind( boost::factory< PHY_RolePion_Perceiver* >() ,_1 ) );
    ext.AddFactory< PHY_RolePion_Composantes >( boost::bind( boost::factory< PHY_RolePion_Composantes* >() ,_1 ) );
    ext.AddFactory< transport::PHY_RoleAction_Loading >( boost::bind( boost::factory< transport::PHY_RoleAction_Loading* >() ,_1 ) ); // $$$$ _RC_ SLI 2012-11-09: must be created after RolePion_Composantes
}

// -----------------------------------------------------------------------------
// Name: Sink::Create
// Created: SLI 2012-02-10
// -----------------------------------------------------------------------------
MIL_AgentPion* Sink::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis, sword::RoleExtender_ABC* ext )
{
    if( MIL_AgentPion* pPion = Find( xis.attribute< unsigned long >( "id" ) ) )
        throw MASA_EXCEPTION( MT_FormatString( "A unit with ID '%d' already exists.", pPion->GetID() ) );
    SinkRoleExtender chainExt( ext );
    CreateRoles( chainExt );
    MIL_AgentPion* pPion = type.InstanciatePion( missionController_, automate, xis );
    type.RegisterRoles( *pPion, &chainExt );
    tools::Resolver< MIL_AgentPion >::Register( pPion->GetID(), *pPion );
    { 
        MT_Vector2D vPos;
        if( xis.has_attribute( "position" ) )
        {
            std::string strPosition;
            xis >> xml::attribute( "position", strPosition );
            world_->MosToSimMgrsCoord( strPosition, vPos );
        }
        else
        {
            xis >> xml::attribute( "x", vPos.rX_ );
            xis >> xml::attribute( "y", vPos.rY_ );
        }
        pPion->CallRole( &PHY_RoleInterface_Location::Move, vPos, MT_Vector2D( 0., 1. ), 0. );
        pPion->CallRole( &PHY_RoleInterface_UrbanLocation::MagicMove, vPos );
    }
    pPion->ReadOverloading( xis );
    return pPion;
}

// -----------------------------------------------------------------------------
// Name: Sink::Create
// Created: SLI 2012-02-10
// -----------------------------------------------------------------------------
MIL_AgentPion* Sink::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, const std::string& name, sword::RoleExtender_ABC* ext )
{
    xml::xobufferstream x;
    x << xml::start( "unit" )
        << xml::attribute( "id", "0" )
        << xml::attribute( "name" , name )
        // Coordinates are not serialized to MGRS to avoid the loss of precision
        // induced by projecting from and to. It was noticeable in gosword.
        << xml::attribute( "x", vPosition.rX_ )
        << xml::attribute( "y", vPosition.rY_ );
    automate.GetColor().WriteODB( x );
    x >> xml::start( "unit" );
    return Create( type, automate, x, ext );
}

// -----------------------------------------------------------------------------
// Name: Sink::CreateFloodModel
// Created: LGY 2012-08-07
// -----------------------------------------------------------------------------
std::unique_ptr< propagation::FloodModel_ABC > Sink::CreateFloodModel() const
{
    return std::unique_ptr< propagation::FloodModel_ABC >( new propagation::FloodModel( *pElevation_ ) );
}

