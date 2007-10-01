// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AfterActionItem.h"
#include "AfterActionBuilder_ABC.h"
#include "AfterActionParameter.h"
#include "AfterActionFunction.h"
#include <xeumeuleu/xml.h>
#pragma warning (disable : 4127 4511 4512 )
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>

namespace ba = boost::algorithm;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AfterActionItem constructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
AfterActionItem::AfterActionItem( const std::string& type, xml::xistream& xis )
    : parameters_( 0 )
    , type_      ( type )
    , name_      ( xml::attribute< std::string >( xis, "name" ) )
    , output_    ( xml::attribute( xis, "output", std::string() ) )
    , parameterName_( xml::attribute( xis, "parameter-name", std::string() ) )
    , parameterType_( xml::attribute( xis, "parameter-type", std::string() ) )
    , parameter_( 0 )
{
    std::string input1, input2;
    xis >> xml::optional() >> xml::attribute( "input1", input1 )
        >> xml::optional() >> xml::attribute( "input2", input2 );
    if( ! input1.empty() )
        inputs_.push_back( input1 );
    if( ! input2.empty() )
        inputs_.push_back( input2 );
    inputConnections_.resize( inputs_.size() );
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem constructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
AfterActionItem::AfterActionItem( const AfterActionItem& base, AfterActionFunction& parameters )
    : parameters_       ( &parameters )
    , type_             ( base.type_ )
    , name_             ( base.name_ )
    , output_           ( base.output_ )
    , parameterName_    ( base.parameterName_ )
    , parameterType_    ( base.parameterType_ )
    , inputs_           ( base.inputs_ )
    , inputConnections_ ( base.inputConnections_ )
    , outputConnections_( base.outputConnections_ )
    , parameter_        ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem destructor
// Created: AGE 2007-09-19
// -----------------------------------------------------------------------------
AfterActionItem::~AfterActionItem()
{
    for( IT_Connections it = inputConnections_.begin(); it != inputConnections_.end(); ++it )
        if( *it )
            (*it)->Disconnect( this );
    for( IT_OutgoingConnections it = outputConnections_.begin(); it != outputConnections_.end(); ++it )
        it->first->Disconnect( this, it->second );
    if( parameter_ && parameters_ )
        parameters_->Remove( *parameter_ );
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem::Build
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionItem::Build( AfterActionBuilder_ABC& builder ) const
{
    builder.Start( name_ );
    std::for_each( inputs_.begin(), inputs_.end(), boost::bind( &AfterActionBuilder_ABC::AddInput, &builder, _1 ) );
    if( ! output_.empty() )
        builder.AddOutput( output_ );
    if( ! parameterName_.empty() )
        builder.AddParameter( parameterType_, parameterName_ );
    for( CIT_OutgoingConnections it = outputConnections_.begin(); it != outputConnections_.end(); ++it )
        builder.Connect( it->first, it->second );
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem::Connect
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionItem::Connect( xml::xistream& xis, Resolver_ABC< AfterActionItem_ABC, std::string >& items )
{
    const std::string input = xml::attribute( xis, "input", std::string() );
    if( ! input.empty() )
    {
        T_Inputs split;
        ba::split( split, input, ba::is_any_of( "," ) );
        for( unsigned i = 0; i < split.size(); ++i )
        {
            AfterActionItem_ABC* target = & items.Get( split.at( i ) );
            inputConnections_.at( i ) = target;
            // $$$$ AGE 2007-09-27: 
            static_cast< AfterActionItem *>( target )->outputConnections_.push_back( T_OutgoingConnection( this, i ) );
        }
    }
    if( ! parameterName_.empty() && parameters_ )
        parameter_ = &parameters_->kernel::Resolver< AfterActionParameter, std::string >::Get( xml::attribute< std::string >( xis, parameterName_ ) );
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem::Connect
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionItem::Connect( int input, AfterActionItem_ABC& rhs )
{
    if( ! CanConnect( input, &rhs ) )
        throw std::runtime_error( "Can't connect" );
    inputConnections_.at( input ) = &rhs;
    rhs.ConnectOutput( *this, input );
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem::ConnectOutput
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionItem::ConnectOutput( AfterActionItem_ABC& rhs, int input )
{
    outputConnections_.push_back( T_OutgoingConnection( &rhs, input ) );
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem::CanConnect
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
bool AfterActionItem::CanConnect( int input, const AfterActionItem_ABC* rhs /*=0*/) const
{
    return IsFree( input )
        && ( !rhs || rhs->IsCompatible( Resolve( input, rhs ), this ) );
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem::Disconnect
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
void AfterActionItem::Disconnect( AfterActionItem_ABC* item )
{
    for( IT_OutgoingConnections it = outputConnections_.begin(); it!= outputConnections_.end(); )
        if( it->first == item )
        {
            std::swap( *it, outputConnections_.back() );
            outputConnections_.pop_back();
        }
        else
            ++it;
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem::Disconnect
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
void AfterActionItem::Disconnect( AfterActionItem_ABC* item, int input )
{
    if( inputConnections_.at( input ) != item )
        throw std::runtime_error( __FUNCTION__ );
    inputConnections_.at( input ) = 0;
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem::Resolve
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
std::string AfterActionItem::Resolve( const AfterActionItem_ABC* connection ) const
{
    int link = LinkOutput();
    if( link >= 0 )
        return Resolve( link, connection );
    return output_;
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem::Resolve
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
std::string AfterActionItem::Resolve( int input, const AfterActionItem_ABC* ignore /*= 0*/ ) const
{
    const std::string& type = inputs_.at( input );
    if( type != "any" )
        return type;
    const AfterActionItem_ABC* connected = inputConnections_.at( input );
    if( connected && connected != ignore )
        return connected->Resolve( this );
    if( LinkOutput() != input )
        return type;
    return ConnectedOutputType( ignore );
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem::ConnectedOutputType
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
std::string AfterActionItem::ConnectedOutputType( const AfterActionItem_ABC* ignore ) const
{
    for( CIT_OutgoingConnections it = outputConnections_.begin(); it != outputConnections_.end(); ++it )
        if( it->first != ignore )
            return it->first->Resolve( it->second, this );
    return "any";
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem::LinkOutput
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
int AfterActionItem::LinkOutput() const
{
     if( ba::starts_with( output_, "input" ) )
     {
         const std::string input = ba::trim_copy( ba::erase_head_copy( output_, 5 ) );
         return input.empty() ? 0 : boost::lexical_cast< int >( input ) - 1;
     }
     return -1;
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem::IsFree
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
bool AfterActionItem::IsFree( int input ) const
{
    return input >= 0
        && input < inputConnections_.size()
        && inputConnections_[ input ] == 0;
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem::IsCompatible
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
bool AfterActionItem::IsCompatible( const std::string& type, const AfterActionItem_ABC* connection ) const
{
    const std::string mine = Resolve( connection );
    return type == "any" || mine == "any" || mine == type;
}

namespace
{
    std::string GetId( const AfterActionItem_ABC* item )
    {
        int id = reinterpret_cast< int >( item );
        return boost::lexical_cast< std::string >( id );
    }
    template< typename C >
    std::string BuildInput( const C& inputs )
    {
        std::vector< std::string > names;
        std::transform( inputs.begin(), inputs.end(),
                std::back_inserter( names ), boost::bind( &GetId, _1 ) );
        return ba::join( names, "," );
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem::BuildType
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
std::string AfterActionItem::BuildType() const
{
    for( int i = 0; i < inputs_.size(); ++i )
        if( inputs_[i] == "any" )
            return Resolve( i, 0 );
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem::Commit
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
void AfterActionItem::Commit( xml::xostream& xos ) const
{
    xos << xml::start( type_ )
            << xml::attribute( "function", name_ );
    if( type_ != "plot" ) // $$$$ AGE 2007-09-25: 
        xos << xml::attribute( "id", GetId( this ) );
    if( ! inputs_.empty() )
        xos << xml::attribute( "input", BuildInput( inputConnections_ ) );
    const std::string type = BuildType();
    if( ! type.empty() )
        xos << xml::attribute( "type", type );
    if( ! parameterName_.empty() && parameter_ )
        parameter_->Commit( xos, parameterName_ );
    xos << xml::end();
}
