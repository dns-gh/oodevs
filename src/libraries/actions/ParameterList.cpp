// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "ParameterList.h"
#include "Bool.h"
#include "Identifier.h"
#include "Numeric.h"
#include "Quantity.h"
#include "String.h"
#include "ParameterFactory_ABC.h"

#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"

#include <boost/lexical_cast.hpp>

using namespace kernel;

namespace actions {
    namespace parameters {

// -----------------------------------------------------------------------------
// Name: ParameterList constructor
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
ParameterList::ParameterList( const kernel::OrderParameter& parameter )
    : Parameter< QString >( parameter )
    , parameter_( parameter )
{
    // NOTHING
}

namespace
{
    void RegisterStructure( ParameterList& paramList,
                            const kernel::OrderParameter& parameter,
                            const ::google::protobuf::RepeatedPtrField< ::sword::MissionParameter_Value >& list,
                            const actions::ParameterFactory_ABC& factory,
                            const kernel::Entity_ABC* entity )
    {
        if( !parameter.IsStructure() )
            throw MASA_EXCEPTION( tools::translate( "ParameterList", "Parameter '%1' should be a structure" )
                                    .arg( QString::fromStdString( parameter.GetName() ) ).toStdString() );
        if( static_cast< int >( parameter.Count() ) != list.size() )
            throw MASA_EXCEPTION( tools::translate( "ParameterList", "Expecting '%1' parameters, got '%2' on structure '%3'" )
                                    .arg( parameter.Count() ).arg( list.size() ).arg( QString::fromStdString( parameter.GetName() ) ).toStdString() );
        for( unsigned int i = 0; i < parameter.Count(); ++i )
        {
            std::unique_ptr< Parameter_ABC > param( factory.CreateParameter( parameter.Get( i ), list.Get( i ), entity ) );
            if( param )
                paramList.AddParameter( *param );
            param.release();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ParameterList constructor
// Created: MGD 2010-11-09
// -----------------------------------------------------------------------------
ParameterList::ParameterList( const kernel::OrderParameter& parameter,
                              const ::google::protobuf::RepeatedPtrField< ::sword::MissionParameter_Value >& list,
                              const actions::ParameterFactory_ABC& factory,
                              const kernel::Entity_ABC* entity )
    : Parameter< QString >( parameter )
    , parameter_( parameter )
{
    // Structure
    if( parameter.IsStructure() )
        RegisterStructure( *this, parameter, list, factory, entity );
    // Union
    else if( parameter.IsUnion() )
    {
        if( list.size() < 1 )
            throw MASA_EXCEPTION( tools::translate( "ParameterList", "Expecting at least one parameter on union '%1'" )
                                    .arg( QString::fromStdString( parameter.GetName() ) ).toStdString() );
        auto& idParameter = list.Get( 0 );
        if( !idParameter.has_identifier() )
            throw MASA_EXCEPTION( tools::translate( "ParameterList", "Expecting an identifier as first parameter on union '%1'" )
                                    .arg( QString::fromStdString( parameter.GetName() ) ).toStdString() );
        auto id = idParameter.identifier();
        auto subParam = parameter.Find( id );
        if( !subParam )
            throw MASA_EXCEPTION( tools::translate( "ParameterList", "No parameter found for id '%1' on union '%2'" )
                                    .arg( id ).arg( QString::fromStdString( parameter.GetName() ) ).toStdString() );
        RegisterStructure( *this, *subParam, list, factory, entity );
        SetName( QString::fromStdString( subParam->GetName() ) );
    }
    // List
    else
    {
        const auto& subParam = !parameter.IsStructure() && parameter.Count() == 1 ? parameter.Get( 0 ) : parameter;
        for( int i = 0; i < list.size(); ++i )
        {
            std::unique_ptr< Parameter_ABC > param( factory.CreateParameter( subParam, list.Get( i ), entity ) );
            if( param )
            {
                if( param->GetName().toStdString() == subParam.GetName() )
                    param->SetName( tools::translate( "Parameter", "%1 (item %2)" ).arg( param->GetName() ).arg( i + 1 ) );
                AddParameter( *param );
            }
            param.release();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ParameterList destructor
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
ParameterList::~ParameterList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParameterList::GetType
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
std::string ParameterList::GetType() const
{
    return parameter_.GetType();
}

// -----------------------------------------------------------------------------
// Name: ParameterList::GetDisplayName
// Created: JSR 2012-10-17
// -----------------------------------------------------------------------------
QString ParameterList::GetDisplayName( kernel::DisplayExtractor_ABC& /*extractor*/ ) const
{
    if( IsSet() )
        return tools::translate( "ParameterList", "List" );
    return tools::translate( "ParameterList", "Not set" );
}

// -----------------------------------------------------------------------------
// Name: ParameterList::Display
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void ParameterList::Display( kernel::Displayer_ABC& displayer ) const
{
    if( IsSet() )
        displayer.Item( tools::translate( "Action", "Parameter" ) ).Display( GetName() )
                 .Item( tools::translate( "Parameter", "Value" ) ).Display( tools::translate( "ParameterList", "List" ) );
    else
        displayer.Item( tools::translate( "Action", "Parameter" ) ).Display( GetName() )
                 .Item( tools::translate( "Parameter", "Value" ) ).Display( tools::translate( "ParameterList", "Not set" ) );
}

// -----------------------------------------------------------------------------
// Name: LocationList::DisplayTooltip
// Created: MGD 2010-11-18
// -----------------------------------------------------------------------------
void ParameterList::DisplayTooltip( const gui::Viewport_ABC& viewport, const gui::GlTools_ABC& tools ) const
{
    Parameter< QString >::DisplayTooltip( viewport, tools );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        it->second->DisplayTooltip( viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: ParameterList::CommitTo
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void ParameterList::CommitTo( sword::MissionParameter_Value& message ) const
{
    ::google::protobuf::RepeatedPtrField< ::sword::MissionParameter_Value >* list = message.mutable_list();

    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        sword::MissionParameter_Value* elementValue = list->Add();
        it->second->CommitTo( *elementValue );
    }
}

// -----------------------------------------------------------------------------
// Name: ParameterList::CommitTo
// Created: ABR 2011-02-15
// -----------------------------------------------------------------------------
void ParameterList::CommitTo( std::string& content ) const
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        if( it != elements_.begin() )
            content += ',';
        it->second->CommitTo( content );
    }
}

// -----------------------------------------------------------------------------
// Name: ParameterList::AddList
// Created: JSR 2010-04-22
// -----------------------------------------------------------------------------
ParameterList& ParameterList::AddList( const std::string& name )
{
    ParameterList& ret = *new ParameterList( OrderParameter( name, "list", false ) );
    AddParameter( ret );
    return ret;
}

// -----------------------------------------------------------------------------
// Name: ParameterList::AddBool
// Created: JSR 2010-04-22
// -----------------------------------------------------------------------------
void ParameterList::AddBool( const std::string& name, bool value )
{
    AddParameter( *new Bool( OrderParameter( name, "bool", false ), value ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterList::AddIdentifier
// Created: JSR 2010-04-22
// -----------------------------------------------------------------------------
void ParameterList::AddIdentifier( const std::string& name, unsigned int id )
{
    AddParameter( *new Identifier( OrderParameter( name, "identifier", false ), id ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterList::AddNumeric
// Created: JSR 2010-04-22
// -----------------------------------------------------------------------------
void ParameterList::AddNumeric( const std::string& name, float value )
{
    AddParameter( *new Numeric( OrderParameter( name, "numeric", false ), value ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterList::AddQuantity
// Created: JSR 2010-04-22
// -----------------------------------------------------------------------------
void ParameterList::AddQuantity( const std::string& name, int value )
{
    AddParameter( *new Quantity( OrderParameter( name, "quantity", false ), value ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterList::AddString
// Created: JSR 2010-09-10
// -----------------------------------------------------------------------------
void ParameterList::AddString( const std::string& name, const std::string& value )
{
    AddParameter( *new String( OrderParameter( name, "string", false ), value ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterList::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string ParameterList::SerializeType() const
{
    return GetType();
}

// -----------------------------------------------------------------------------
// Name: ParameterList::IsSet
// Created: ABR 2011-11-23
// -----------------------------------------------------------------------------
bool ParameterList::IsSet() const
{
    return !elements_.empty();
}

}
}
