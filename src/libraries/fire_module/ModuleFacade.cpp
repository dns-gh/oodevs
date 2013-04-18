// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ModuleFacade.h"
#include "DirectFireCommand.h"
#include "DirectFireCommandPopulation.h"
#include "Knowledge_RapForLocal.h"
#include "RoleAction_IndirectFiring.h"
#include <tools/Exception.h>
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>
#include <map>

using namespace sword;
using namespace sword::fire;

namespace
{
    ModuleFacade* module = 0;
}

DEFINE_HOOK( GetForceRatio, 2, double, ( const SWORD_Model* model, const SWORD_Model* entity ) )
{
    assert( module );
    return module->GetForceRatio( model, entity );
}
DEFINE_HOOK( GetDangerousEnemies, 4, void, ( const SWORD_Model* model, const SWORD_Model* entity,
                                          void(*visitor)( const SWORD_Model* knowledge, void* userData ), void* userData ) )
{
    assert( module );
    module->GetDangerousEnemies( model, entity, visitor, userData );
}
DEFINE_HOOK_WITH_DEFAULT_RESULT( GetAmmunitionForIndirectFire, 4, int, ( const SWORD_Model* model, const SWORD_Model* firer, const char* type, const MT_Vector2D* target ), -1 )
{
    if( !type )
        throw MASA_EXCEPTION( "Invalid null type" );
    assert( module );
    return RoleAction_IndirectFiring().GetAmmunitionForIndirectFire( *module, model, firer, type, target );
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade constructor
// Created: MCO 2012-03-19
// -----------------------------------------------------------------------------
ModuleFacade::ModuleFacade( const wrapper::View& model )
    : Module( model )
{
    module = this;
    wrapper::RegisterCommand< DirectFireCommand >( "direct fire", *this );
    wrapper::RegisterCommand< DirectFireCommandPopulation >( "direct fire population", *this );
}

namespace
{
    std::map< std::size_t, boost::shared_ptr< Knowledge_RapForLocal > > rapforCache;

    const sword::fire::Knowledge_RapForLocal& GetCache( const wrapper::View& model, const wrapper::View& entity )
    {
        boost::shared_ptr< Knowledge_RapForLocal >& rapfor = rapforCache[ entity[ "identifier" ] ];
        if( !rapfor )
            rapfor.reset( new Knowledge_RapForLocal( model, entity ) );
        return *rapfor;
    }
}

DEFINE_HOOK( Tick, 0, void, () )
{
    rapforCache.clear();
    if( GET_PREVIOUS_HOOK( Tick ) )
        GET_PREVIOUS_HOOK( Tick )();
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::GetForceRatio
// Created: SLI 2012-10-17
// -----------------------------------------------------------------------------
double ModuleFacade::GetForceRatio( const wrapper::View& model, const wrapper::View& entity )
{
    return GetCache( model, entity ).GetValue();
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::GetDangerousEnemies
// Created: SLI 2012-10-17
// -----------------------------------------------------------------------------
void ModuleFacade::GetDangerousEnemies( const wrapper::View& model, const wrapper::View& entity,
                                        void(*visitor)( const SWORD_Model* knowledge, void* userData ), void* userData )
{
    const sword::fire::Knowledge_RapForLocal& rapfor = GetCache( model, entity );
    const Knowledge_RapForLocal::T_KnowledgeAgents& enemies = rapfor.GetDangerousEnemies();
    std::for_each( enemies.begin(), enemies.end(), boost::bind( visitor, _1, userData ) );
}