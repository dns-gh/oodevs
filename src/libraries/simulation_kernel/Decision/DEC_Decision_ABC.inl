// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: template< typename T > void DEC_Decision_ABC::Callback
// Created: LDC 2009-06-29
// -----------------------------------------------------------------------------
template< typename T >
void DEC_Decision_ABC::Callback( unsigned int actionId, T value )
{
    directia::ScriptRef function = GetBrain().GetScriptFunction( "CallbackAction" );
    function( actionId, value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision_ABC::SetVariable
// Created: LDC 2009-07-31
// -----------------------------------------------------------------------------
template <typename T>
void DEC_Decision_ABC::SetVariable( const std::string& name, T value )
{
    GetBrain().RegisterObject( name, value );
}


// -----------------------------------------------------------------------------
// Name: template< typename T > void DEC_Decision_ABC::SetVariable
// Created: LDC 2009-07-31
// -----------------------------------------------------------------------------
template< typename T >
void DEC_Decision_ABC::SetScriptVariable( const T& source, T& dest )
{
    dest = source;
}

// -----------------------------------------------------------------------------
// Name: template< typename T > T DEC_Decision_ABC::GetVariable
// Created: LDC 2009-07-31
// -----------------------------------------------------------------------------
template< typename T >
T DEC_Decision_ABC::GetVariable( const std::string& name )
{
    T value;
    directia::ScriptRef scriptRef = GetBrain().GetScriptVariable( name );
    if( !scriptRef )
        GetBrain().RegisterObject( name, value );
    GetBrain().RegisterFunction( "DEC_SetVariable__", boost::function< void( const T& ) >( boost::bind( &DEC_Decision_ABC::SetScriptVariable<T>, _1, boost::ref( value ) ) ) );
    GetBrain().GetScriptFunction( "DEC_SetVariable__" )( GetBrain().GetScriptVariable( name ) );
    return value;
}

// -----------------------------------------------------------------------------
// Name: template< typename T > T DEC_Decision_ABC::GetScalarVariable
// Created: LDC 2009-07-31
// -----------------------------------------------------------------------------
template< typename T >
T DEC_Decision_ABC::GetScalarVariable( const std::string& name )
{
    T value = 0;
    directia::ScriptRef scriptRef = GetBrain().GetScriptVariable( name );
    if( !scriptRef )
        GetBrain().RegisterObject( name, value );
    GetBrain().RegisterFunction( "DEC_SetVariable__", boost::function< void( const T& ) >( boost::bind( &DEC_Decision_ABC::SetScriptVariable<T>, _1, boost::ref( value ) ) ) );
    GetBrain().GetScriptFunction( "DEC_SetVariable__" )( GetBrain().GetScriptVariable( name ) );
    return value;
}
