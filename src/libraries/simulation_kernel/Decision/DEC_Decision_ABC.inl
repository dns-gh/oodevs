// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Decision_ABC::Callback
// Created: LDC 2009-06-29
// -----------------------------------------------------------------------------
template< typename T >
void DEC_Decision_ABC::Callback( unsigned int actionId, T value )
{
    GetScriptRef( "CallbackAction" )( actionId, value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision_ABC::SetVariable
// Created: LDC 2009-07-31
// -----------------------------------------------------------------------------
template <typename T>
void DEC_Decision_ABC::SetVariable( const std::string& name, T value )
{
    GetScriptRef( name ) = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision_ABC::SetScriptVariable
// Created: LDC 2009-07-31
// -----------------------------------------------------------------------------
template< typename T >
void DEC_Decision_ABC::SetScriptVariable( const T& source, T& dest )
{
    dest = source;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision_ABC::GetVariable
// Created: LDC 2009-07-31
// -----------------------------------------------------------------------------
template< typename T >
T DEC_Decision_ABC::GetVariable( const std::string& name )
{
    T value = T();
    directia::tools::binders::ScriptRef scriptRef = GetScriptRef( name );
    if( !scriptRef )
        scriptRef = value;
    GetScriptRef( "DEC_SetVariable__" ) = boost::function< void( const T& ) >( boost::bind( &DEC_Decision_ABC::SetScriptVariable<T>, _1, boost::ref( value ) ) );
    GetScriptRef( "DEC_SetVariable__" )( GetScriptRef( name ) );
    return value;
}
