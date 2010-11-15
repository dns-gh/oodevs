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

#include "protocol/Protocol.h"

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

// -----------------------------------------------------------------------------
// Name: ParameterList constructor
// Created: MGD 2010-11-09
// -----------------------------------------------------------------------------
ParameterList::ParameterList( const kernel::OrderParameter& parameter, const ::google::protobuf::RepeatedPtrField< ::Common::MsgMissionParameter_Value >& list, const actions::ParameterFactory_ABC& factory, const kernel::Entity_ABC& entity )
    : Parameter< QString >( parameter )
    , parameter_( parameter )
{
    int i = 0;
    for( ::google::protobuf::RepeatedPtrField< ::Common::MsgMissionParameter_Value >::const_iterator it = list.begin(); it != list.end(); ++it, ++i )
        AddParameter( *factory.CreateParameter( OrderParameter( tools::translate( "Parameter", "%1 (item %2)" ).arg( parameter.GetName().c_str() ).arg( i + 1 ).ascii(), "location", false ), *it, entity ) );
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
// Name: ParameterList::Display
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void ParameterList::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Item( tools::translate( "ParameterList", "Parameter" ) ).Display( GetName() );
}

// -----------------------------------------------------------------------------
// Name: ParameterList::Serialize
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void ParameterList::Serialize( xml::xostream& xos ) const
{
    Parameter_ABC::Serialize( xos );
    xos << xml::attribute( "type", parameter_.GetType() );
}

// -----------------------------------------------------------------------------
// Name: ParameterList::CommitTo
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void ParameterList::CommitTo( Common::MsgMissionParameter_Value& message ) const
{
    ::google::protobuf::RepeatedPtrField< ::Common::MsgMissionParameter_Value >* list = message.mutable_list();

    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        Common::MsgMissionParameter_Value* elementValue = list->Add();
        it->second->CommitTo( *elementValue );
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

}
}
