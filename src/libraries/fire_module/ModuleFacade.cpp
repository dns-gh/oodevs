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
#include <wrapper/Command.h>
#include <module_api/Log.h>
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

using namespace sword;
using namespace sword::fire;

namespace
{
    ModuleFacade* module = 0;
}

#define TRY try {
#define CATCH( hook ) \
    } \
    catch( std::exception& e ) \
    { \
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, e.what() ); \
    } \
    catch( ... ) \
    { \
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Unknown exception during " #hook ); \
    }

DEFINE_HOOK( GetForceRatio, double, ( const SWORD_Model* model, const SWORD_Model* entity ) )
{
    TRY
        assert( module );
        return module->GetForceRatio( model, entity );
    CATCH( GetForceRatio )
    return 0;
}
DEFINE_HOOK( GetDangerousEnemies, void, ( const SWORD_Model* model, const SWORD_Model* entity,
                                          void(*visitor)( const SWORD_Model* knowledge, void* userData ), void* userData ) )
{
    TRY
        assert( module );
        module->GetDangerousEnemies( model, entity, visitor, userData );
    CATCH( GetDangerousEnemies )
}
DEFINE_HOOK( GetAmmunitionForIndirectFire, const char*, ( const SWORD_Model* model, const SWORD_Model* firer, const char* type, const MT_Vector2D* target ) )
{
    if( ! type )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Invalid null type in GetAmmunitionForIndirectFire" );
        return 0;
    }
    TRY
        assert( module );
        return RoleAction_IndirectFiring().GetAmmunitionForIndirectFire( *module, model, firer, type, target );
    CATCH( GetAmmunitionForIndirectFire )
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade constructor
// Created: MCO 2012-03-19
// -----------------------------------------------------------------------------
ModuleFacade::ModuleFacade()
{
    module = this;
    wrapper::RegisterCommand< DirectFireCommand >( "direct fire", *this );
    wrapper::RegisterCommand< DirectFireCommandPopulation >( "direct fire population", *this );
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::GetForceRatio
// Created: SLI 2012-10-17
// -----------------------------------------------------------------------------
double ModuleFacade::GetForceRatio( const wrapper::View& model, const wrapper::View& entity )
{
    sword::fire::Knowledge_RapForLocal rapfor;
    rapfor.Update( model, entity ); // $$$$ MCO 2012-05-15: cache this
    return rapfor.GetValue();
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade::GetDangerousEnemies
// Created: SLI 2012-10-17
// -----------------------------------------------------------------------------
void ModuleFacade::GetDangerousEnemies( const wrapper::View& model, const wrapper::View& entity,
                                        void(*visitor)( const SWORD_Model* knowledge, void* userData ), void* userData )
{
    sword::fire::Knowledge_RapForLocal rapfor;
    rapfor.Update( model, entity ); // $$$$ MCO 2012-05-15: cache this
    const Knowledge_RapForLocal::T_KnowledgeAgents& enemies = rapfor.GetDangerousEnemies();
    std::for_each( enemies.begin(), enemies.end(), boost::bind( visitor, _1, userData ) );
}
