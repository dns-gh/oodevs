// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PerceptionFlyingShell.h"
#include "PerceptionLevel.h"
#include "wrapper/View.h"
#include "wrapper/Hook.h"
#include "wrapper/Effect.h"
#include "wrapper/Event.h"
#include "simulation_kernel/Tools/MIL_Tools.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>
#include <set>

using namespace sword;
using namespace sword::perception;

double PerceptionFlyingShell::rRadius_ = 0;

DECLARE_HOOK( CanFlyingShellBePerceived, bool, ( const SWORD_Model* flyingShell, const SWORD_Model* zone, const MT_Vector2D* source, double radius ) )

// -----------------------------------------------------------------------------
// Name: PerceptionFlyingShell::Initialize
// Created: NLD 2007-02-13
// -----------------------------------------------------------------------------
void PerceptionFlyingShell::Initialize( xml::xistream& xis )
{
    xis >> xml::start( "cobra-radar" )
            >> xml::attribute( "action-range", rRadius_ )
        >> xml::end;
    if( rRadius_ < 0 )
        throw xml::exception( xis.context() + "cobra-radar: action-range < 0" );
}

// -----------------------------------------------------------------------------
// Name: PerceptionFlyingShell constructor
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PerceptionFlyingShell::PerceptionFlyingShell( const wrapper::View& /*model*/, const wrapper::View& entity, PerceptionObserver_ABC& /*observer*/ )
{
    entity[ "perceptions/flying-shell/zones" ].VisitIdentifiedChildren( boost::bind( &PerceptionFlyingShell::AddLocalisation, this, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: PerceptionFlyingShell::~PerceptionFlyingShell
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PerceptionFlyingShell::~PerceptionFlyingShell()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionFlyingShell::AddLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void PerceptionFlyingShell::AddLocalisation( const wrapper::View& perception )
{
    zones_.push_back( perception[ "localization" ] );
}

namespace
{
    template< typename T >
    void Fill( const wrapper::View& flyingShells, T& identifiers )
    {
        for( std::size_t i = 0; i < flyingShells.GetSize(); ++i )
            identifiers.insert( flyingShells.GetElement( i )[ "identifier" ] );
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionFlyingShell::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PerceptionFlyingShell::ExecuteAgents( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& /*surfaces*/, const T_AgentPtrVector& /*perceivableAgents*/ )
{
    const MT_Vector2D source( perceiver[ "movement/position/x" ], perceiver[ "movement/position/y" ] );
    const wrapper::View flyingShells = model[ "flying-shells" ];
    const wrapper::View previousShells = perceiver[ "perceptions/flying-shell/previous" ];
    std::set< std::size_t > lastPerceivedFlyingShells_;
    Fill( previousShells, lastPerceivedFlyingShells_ );
    wrapper::Effect effect( previousShells );
    for( std::size_t i = 0; i < flyingShells.GetSize(); ++i )
    {
        const wrapper::View flyingShell = flyingShells.GetElement( i );
        for( T_ZoneVector::const_iterator itZone = zones_.begin(); itZone != zones_.end(); ++itZone )
        {
            if( GET_HOOK( CanFlyingShellBePerceived )( flyingShell, *itZone, &source, rRadius_ ) )
            {
                const std::size_t identifier = flyingShell[ "identifier" ];
                effect.AddElement() = identifier;
                if( lastPerceivedFlyingShells_.find( identifier ) == lastPerceivedFlyingShells_.end() )
                {
                    wrapper::Event event( "flying shell detection" );
                    event[ "flying-shell" ] = flyingShell;
                    event[ "entity/data" ] = perceiver[ "data" ];
                    event.Post();
                }
            }
        }
    }
    effect.Post();
}
