// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "MaintenancePriorities.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "protocol/Protocol.h"
#pragma warning( push )
#pragma warning( disable : 4702 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: MaintenancePriorities constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
MaintenancePriorities::MaintenancePriorities( const OrderParameter& parameter )
    : Parameter< std::string >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaintenancePriorities constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
MaintenancePriorities::MaintenancePriorities( const OrderParameter& parameter, const tools::Resolver_ABC< EquipmentType >& resolver, const sword::IdList& message )
    : Parameter< std::string >( parameter )
{
    for( int i = 0; i < message.elem_size(); ++i )
        AddPriority( resolver.Get( message.elem(i).id() ) );
}

// -----------------------------------------------------------------------------
// Name: MaintenancePriorities destructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
MaintenancePriorities::~MaintenancePriorities()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaintenancePriorities::AddPriority
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void MaintenancePriorities::AddPriority( const EquipmentType& value )
{
    priorities_.push_back( &value );
    std::string current = GetValue();
    if( !current.empty() )
        current += ", ";
    SetValue( current + value.GetName() );
}

namespace
{
    void SetPriorities( sword::IdList& dst, const MaintenancePriorities::T_Priorities& src )
    {
        for( unsigned int i = 0; i < src.size(); ++i )
            dst.add_elem()->set_id( src.at( i )->GetId() );
    }
}
// -----------------------------------------------------------------------------
// Name: MaintenancePriorities::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void MaintenancePriorities::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
        SetPriorities( *message.add_value()->mutable_logmaintenancepriorities(), priorities_ );
}
// -----------------------------------------------------------------------------
// Name: MaintenancePriorities::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void MaintenancePriorities::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
        SetPriorities( *message.mutable_logmaintenancepriorities(), priorities_ );
}

// -----------------------------------------------------------------------------
// Name: MaintenancePriorities::CommitTo
// Created: AGE 2007-10-24
// -----------------------------------------------------------------------------
void MaintenancePriorities::CommitTo( std::string& content ) const
{
    for( T_Priorities::const_iterator it = priorities_.begin(); it != priorities_.end(); ++it )
    {
        if( !content.empty() )
            content += ",";
        content += boost::lexical_cast< std::string >( (*it)->GetId() );
    }
}

// -----------------------------------------------------------------------------
// Name: MaintenancePriorities::Serialize
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void MaintenancePriorities::Serialize( xml::xostream& xos ) const
{
    Parameter< std::string >::Serialize( xos );
    if( IsSet() )
    {
        QString value( "" );
        for( T_Priorities::const_iterator it = priorities_.begin(); it != priorities_.end(); ++it )
            value += ( !value.isEmpty() ? ";" : "" ) + QString::number( (*it)->GetId() );
        xos << xml::attribute( "value", value.toStdString() );
    }
}

// -----------------------------------------------------------------------------
// Name: MaintenancePriorities::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool MaintenancePriorities::IsSet() const
{
    return !priorities_.empty();
}

// -----------------------------------------------------------------------------
// Name: MaintenancePriorities::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string MaintenancePriorities::SerializeType() const
{
    return "maintenancepriorities";
}

// -----------------------------------------------------------------------------
// Name: MaintenancePriorities::Accept
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
void MaintenancePriorities::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
