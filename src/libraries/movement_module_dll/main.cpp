// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "movement_module/ModuleFacade.h"
#include <module_api/Module.c>
#include <string>

int SWORD_InitializeModule( void** userData )
{
    try
    {
        *userData = new sword::movement::ModuleFacade();
        return 0;
    }
    catch( std::exception& e )
    {
        SWORD_Log( SWORD_LOG_LEVEL_FATAL, ( "cannot create movement module : " + std::string( e.what() ) ).c_str() );
    }
    catch(...)
    {
        SWORD_Log( SWORD_LOG_LEVEL_FATAL, "cannot create movement module : unknown reason" );
    }
    return 1;
}

void SWORD_DeinitializeModule( void* userData )
{
    delete static_cast< sword::movement::ModuleFacade* >( userData );
}
