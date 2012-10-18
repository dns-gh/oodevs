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
#include "flood/FloodModel.h"
#include "flood/ElevationGetter_ABC.h"
#include "RolePion_Decision.h"
#include "NullRoleAdapter.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "simulation_kernel/Meteo/PHY_MeteoDataManager.h"
#include "MT_Tools/MT_ScipioException.h"
#include "Tools/MIL_Tools.h"

using namespace sword::legacy;

BOOST_CLASS_EXPORT_IMPLEMENT( sword::legacy::Sink )

namespace
{
    class ElevationGetter : public flood::ElevationGetter_ABC
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
            return static_cast< short >( MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetAltitude( point.X(), point.Y() ) );
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
                << elements;
    }

    template< typename Archive >
    void load_construct_data( Archive& archive, sword::legacy::Sink* sink, const unsigned int /*version*/ )
    {
        AgentFactory_ABC* factory;
        unsigned int gcPause;
        unsigned int gcMult;
        Sink::T_Elements elements;
        archive >> factory
                >> gcPause
                >> gcMult
                >> elements;
        ::new( sink )Sink( *factory, gcPause, gcMult );
        sink->elements_ = elements;
    }
}
}

// -----------------------------------------------------------------------------
// Name: Sink constructor
// Created: SLI 2012-01-13
// -----------------------------------------------------------------------------
Sink::Sink( AgentFactory_ABC& factory, unsigned int gcPause, unsigned int gcMult )
    : pElevation_( new ElevationGetter() )
    , factory_   ( factory )
    , gcPause_   ( gcPause )
    , gcMult_    ( gcMult )
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
// Name: Sink::Configure
// Created: SLI 2012-02-10
// -----------------------------------------------------------------------------
MIL_AgentPion& Sink::Configure( MIL_AgentPion& pion )
{
    pion.RegisterRole( *new sword::legacy::NullRoleAdapter() );
    try
    {
        pion.RegisterRole( *new sword::legacy::RolePion_Decision( pion, gcPause_, gcMult_ ) );
    }
    catch( MT_ScipioException& e )
    {
        e.SendToLogger();
    }
    pion.RegisterRole( *new moving::PHY_RoleAction_Moving( pion ) );
    pion.RegisterRole( *new PHY_RolePion_Location( pion ) );
    pion.RegisterRole( *new PHY_RolePion_Perceiver( pion ) );
    pion.RegisterRole( *new PHY_RolePion_Composantes( pion ) );
    tools::Resolver< MIL_AgentPion >::Register( pion.GetID(), pion );
    return pion;
}

// -----------------------------------------------------------------------------
// Name: Sink::Create
// Created: SLI 2012-02-10
// -----------------------------------------------------------------------------
MIL_AgentPion* Sink::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis )
{
    MIL_AgentPion& pion = Configure( *factory_.Create( type, automate, xis ) );
    {
        std::string strPosition;
        xis >> xml::attribute( "position", strPosition );
        MT_Vector2D vPosTmp;
        MIL_Tools::ConvertCoordMosToSim( strPosition, vPosTmp );
        Initialize( pion, vPosTmp );
    }
    pion.GetRole< PHY_RolePion_Composantes >().ReadOverloading( xis ); // Equipments + Humans
    return &pion;
}

// -----------------------------------------------------------------------------
// Name: Sink::Create
// Created: SLI 2012-02-10
// -----------------------------------------------------------------------------
MIL_AgentPion* Sink::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition )
{
    MIL_AgentPion& pion = Configure( *factory_.Create( type, automate, vPosition ) );
    Initialize( pion, vPosition );
    return &pion;
}

// -----------------------------------------------------------------------------
// Name: Sink::Create
// Created: SLI 2012-02-10
// -----------------------------------------------------------------------------
MIL_AgentPion* Sink::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, const std::string& name )
{
    MIL_AgentPion& pion = Configure( *factory_.Create( type, automate, vPosition, name ) );
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
std::auto_ptr< flood::FloodModel_ABC > Sink::CreateFloodModel() const
{
    return std::auto_ptr< flood::FloodModel_ABC >( new flood::FloodModel( *pElevation_ ) );
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
