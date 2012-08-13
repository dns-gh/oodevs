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
#include "Knowledge_RapForLocal.h"
#include "RoleAction_IndirectFiring.h"
#include <wrapper/Command.h>
#include <module_api/Log.h>
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>
#include <boost/bind.hpp>

using namespace sword;
using namespace sword::fire;

DEFINE_HOOK( GetForceRatio, double, ( const SWORD_Model* model, const SWORD_Model* entity ) )
{
    return sword::fire::Knowledge_RapForLocal().GetValue( model, entity ); // $$$$ MCO 2012-05-15: cache this
}
DEFINE_HOOK( GetDangerousEnemies, void, ( const SWORD_Model* model, const SWORD_Model* entity,
    void(*visitor)( const SWORD_Model* knowledge, void* userData ), void* userData ) )
{
    sword::fire::Knowledge_RapForLocal rapfor;
    const Knowledge_RapForLocal::T_KnowledgeAgents& enemies = rapfor.GetDangerousEnemies( model, entity ); // $$$$ MCO 2012-05-15: cache this and/or GetDangerousEnemies()
    std::for_each( enemies.begin(), enemies.end(), boost::bind( visitor, _1, userData ) );
}
DEFINE_HOOK( GetAmmunitionForIndirectFire, const char*, ( const SWORD_Model* model, const SWORD_Model* firer, const char* type, const MT_Vector2D* target ) )
{
    if( ! type )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Invalid null type in GetAmmunitionForIndirectFire" );
        return 0;
    }
    try
    {
        return RoleAction_IndirectFiring().GetAmmunitionForIndirectFire( model, firer, type, target );
    }
    catch( std::exception& e )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, e.what() );
    }
    catch( ... )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Unknown exception during GetAmmunitionForIndirectFire" );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade constructor
// Created: MCO 2012-03-19
// -----------------------------------------------------------------------------
ModuleFacade::ModuleFacade()
{
    wrapper::RegisterCommand< DirectFireCommand >( "direct fire command", *this );
}
