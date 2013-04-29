// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "Engine.h"

using namespace timeline::core;

Engine::Engine()
{
    // NOTHING
}

Engine::~Engine()
{
    // NOTHING
}

bool Engine::Execute( const CefString&         /*name*/,
                      CefRefPtr< CefV8Value >  /*object*/,
                      const CefV8ValueList&    /*args*/,
                      CefRefPtr< CefV8Value >& /*retval*/,
                      CefString&               /*exception*/ )
{
    return false;
}

void Engine::Register( CefRefPtr< CefV8Context > context )
{
    auto window = context->GetGlobal();
    auto timeline = CefV8Value::CreateObject( 0 );
    auto enabled = CefV8Value::CreateBool( true );
    timeline->SetValue( "enabled", enabled, V8_PROPERTY_ATTRIBUTE_NONE );
    window->SetValue( "timeline", timeline, V8_PROPERTY_ATTRIBUTE_NONE );
}