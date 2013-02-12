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
#include "Meteo/PHY_MeteoDataManager.h"
#include "MT_Tools/MT_FormatString.h"
#include "Tools/MIL_Tools.h"

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
        AgentFactory_ABC* factory = &sink->factory_;
        const Sink::T_Elements& elements = sink->elements_;
        archive << factory
                << sink->gcPause_
                << sink->gcMult_
                << sink->logEnabled_
                << elements;
    }

    template< typename Archive >
    void load_construct_data( Archive& archive, sword::legacy::Sink* sink, const unsigned int /*version*/ )
    {
        AgentFactory_ABC* factory;
        unsigned int gcPause;
        unsigned int gcMult;
        bool logEnabled;
        Sink::T_Elements elements;
        archive >> factory
                >> gcPause
                >> gcMult
                >> logEnabled
                >> elements;
        ::new( sink )Sink( *factory, gcPause, gcMult, logEnabled );
        sink->elements_ = elements;
    }
}
}

// -----------------------------------------------------------------------------
// Name: Sink constructor
// Created: SLI 2012-01-13
// -----------------------------------------------------------------------------
Sink::Sink( AgentFactory_ABC& factory, unsigned int gcPause, unsigned int gcMult, bool logEnabled )
    : pElevation_( new ElevationGetter() )
    , factory_   ( factory )
    , gcPause_   ( gcPause )
    , gcMult_    ( gcMult )
    , logEnabled_( logEnabled )
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
// Name: Sink::Finalize
// Created: BAX 2012-10-18
// -----------------------------------------------------------------------------
void Sink::Finalize()
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
        if( pion->IsMarkedForDestruction() && !pion->GetRole< DEC_RolePion_Decision >().IsUsedByDIA() )
        {
            it = elements_.erase( it );
            delete pion;
        }
        else
            ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: Sink::ExecutePerceptions
// Created: SLI 2012-09-11
// -----------------------------------------------------------------------------
void Sink::ExecutePerceptions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Sink::ExecuteCommands
// Created: SLI 2012-01-13
// -----------------------------------------------------------------------------
void Sink::ExecuteCommands()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Sink::ApplyEffects
// Created: SLI 2012-01-13
// -----------------------------------------------------------------------------
void Sink::ApplyEffects()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Sink::NotifyEffects
// Created: MCO 2012-10-25
// -----------------------------------------------------------------------------
void Sink::NotifyEffects()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Sink::UpdateModel
// Created: SLI 2012-01-13
// -----------------------------------------------------------------------------
void Sink::UpdateModel( unsigned int /*tick*/, int /*duration*/, const MIL_ObjectManager& /*objects*/, const MIL_EffectManager& /*effects*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Sink::UpdateKnowledges
// Created: SLI 2012-09-25
// -----------------------------------------------------------------------------
void Sink::UpdateKnowledges()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Sink::UpdateUrbanModel
// Created: SLI 2012-09-24
// -----------------------------------------------------------------------------
void Sink::UpdateUrbanModel( const MIL_UrbanCache& /*cache*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Sink::LogProfiling
// Created: MCO 2012-11-12
// -----------------------------------------------------------------------------
void Sink::LogProfiling()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Sink::Configure
// Created: SLI 2012-02-10
// -----------------------------------------------------------------------------
MIL_AgentPion& Sink::Configure( MIL_AgentPion& pion )
{
    pion.RegisterRole( *new sword::legacy::NullRoleAdapter() );
    try
    {
        pion.RegisterRole( *new sword::legacy::RolePion_Decision( pion, gcPause_, gcMult_, logEnabled_ ) );
    }
    catch( const tools::Exception& e )
    {
        MT_LOG_ERROR_MSG( e.CreateLoggerMsg() );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Can't configure legacy sink ( '" << tools::GetExceptionMsg( e ) << "' )" );
    }

    pion.RegisterRole( *new moving::PHY_RoleAction_Moving( pion ) );
    pion.RegisterRole( *new PHY_RolePion_Location( pion ) );
    pion.RegisterRole( *new PHY_RolePion_Perceiver( pion ) );
    pion.RegisterRole( *new PHY_RolePion_Composantes( pion ) );
    pion.RegisterRole( *new transport::PHY_RoleAction_Loading( pion ) ); // $$$$ _RC_ SLI 2012-11-09: must be created after RolePion_Composantes
    tools::Resolver< MIL_AgentPion >::Register( pion.GetID(), pion );
    return pion;
}

// -----------------------------------------------------------------------------
// Name: Sink::Create
// Created: SLI 2012-02-10
// -----------------------------------------------------------------------------
MIL_AgentPion* Sink::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis, RoleExtender_ABC* ext )
{
    if( MIL_AgentPion* pPion = Find( xis.attribute< unsigned long >( "id" ) ) )
        throw MASA_EXCEPTION( MT_FormatString( "A unit with ID '%d' already exists.", pPion->GetID() ) );
    SinkRoleExtender chainExt( ext, boost::bind( &Sink::Configure, boost::ref( *this ), _1 ) );
    MIL_AgentPion& pion = *factory_.Create( type, automate, xis, &chainExt );
    { 
        std::string strPosition;
        xis >> xml::attribute( "position", strPosition );
        MT_Vector2D vPosTmp;
        MIL_Tools::ConvertCoordMosToSim( strPosition, vPosTmp );
        Initialize( pion, vPosTmp );
    }
    pion.ReadOverloading( xis );
    pion.GetRole< PHY_RolePion_Composantes >().ReadOverloading( xis ); // Equipments + Humans
    return &pion;
}

// -----------------------------------------------------------------------------
// Name: Sink::Create
// Created: SLI 2012-02-10
// -----------------------------------------------------------------------------
MIL_AgentPion* Sink::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, RoleExtender_ABC* ext )
{
    SinkRoleExtender chainExt( ext, boost::bind( &Sink::Configure, boost::ref( *this ), _1 ) );
    MIL_AgentPion& pion = *factory_.Create( type, automate, vPosition, &chainExt );
    Initialize( pion, vPosition );
    return &pion;
}

// -----------------------------------------------------------------------------
// Name: Sink::Create
// Created: SLI 2012-02-10
// -----------------------------------------------------------------------------
MIL_AgentPion* Sink::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, const std::string& name, RoleExtender_ABC* ext )
{
    SinkRoleExtender chainExt( ext, boost::bind( &Sink::Configure, boost::ref( *this ), _1 ) );
    MIL_AgentPion& pion =* factory_.Create( type, automate, vPosition, name, &chainExt );
    Initialize( pion, vPosition );
    return &pion;
}

// -----------------------------------------------------------------------------
// Name: Sink::Initialize
// Created: SLG 2010-01-21
// -----------------------------------------------------------------------------
void Sink::Initialize( MIL_AgentPion& pion, const MT_Vector2D& vPosition )
{
    pion.GetRole< PHY_RoleInterface_Location >().Move( vPosition, MT_Vector2D( 0., 1. ), 0. );
    pion.GetRole< PHY_RoleInterface_UrbanLocation >().MagicMove( vPosition );
}

// -----------------------------------------------------------------------------
// Name: Sink::CreateFloodModel
// Created: LGY 2012-08-07
// -----------------------------------------------------------------------------
std::auto_ptr< propagation::FloodModel_ABC > Sink::CreateFloodModel() const
{
    return std::auto_ptr< propagation::FloodModel_ABC >( new propagation::FloodModel( *pElevation_ ) );
}

// -----------------------------------------------------------------------------
// Name: Sink::GetModelCount
// Created: SLI 2012-09-12
// -----------------------------------------------------------------------------
unsigned long Sink::GetModelCount() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Sink::StartCommand
// Created: LGY 2012-09-21
// -----------------------------------------------------------------------------
std::size_t Sink::StartCommand( const std::string& /*type*/, const core::Model& /*parameters*/ )
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Sink::StopCommand
// Created: LGY 2012-09-21
// -----------------------------------------------------------------------------
void Sink::StopCommand( std::size_t /*command*/ )
{
    // NOTHING
}
