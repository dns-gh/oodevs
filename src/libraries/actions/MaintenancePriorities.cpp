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
MaintenancePriorities::MaintenancePriorities( const OrderParameter& parameter, const tools::Resolver_ABC< EquipmentType >& resolver, const sword::LogMaintenancePriorities& message )
    : Parameter< std::string >( parameter )
{
    for( int i = 0; i < message.elem_size(); ++i )
        AddPriority( resolver.Get( message.elem(i).id() ) );
}

// -----------------------------------------------------------------------------
// Name: MaintenancePriorities constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
MaintenancePriorities::MaintenancePriorities( const OrderParameter& parameter, const tools::Resolver_ABC< EquipmentType >& resolver, xml::xistream& xis )
    : Parameter< std::string >( parameter )
{
    if( xis.has_attribute( "value" ) )
    {
        std::string value;
        xis >> xml::attribute( "value", value );
        QStringList list = QStringList::split( ";", value.c_str() );
        for( int i = 0; i < list.count(); ++i )
            AddPriority( resolver.Get( list[i].toUInt() ) );
    }
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

// -----------------------------------------------------------------------------
// Name: MaintenancePriorities::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void MaintenancePriorities::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
    {
        sword::LogMaintenancePriorities* list =message.mutable_value()->Add()->mutable_logmaintenancepriorities();
        for( unsigned int i = 0; i < priorities_.size(); ++i )
            list->add_elem()->set_id( priorities_.at( i )->GetId() );
    }
}
// -----------------------------------------------------------------------------
// Name: MaintenancePriorities::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void MaintenancePriorities::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
    {
        sword::LogMaintenancePriorities* list = message.mutable_logmaintenancepriorities();
        for( unsigned int i = 0; i < priorities_.size(); ++i )
            list->add_elem()->set_id( priorities_.at( i )->GetId() );
    }
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
        xos << xml::attribute( "value", value.ascii() );
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
