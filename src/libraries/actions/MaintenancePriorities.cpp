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
#include <xeumeuleu/xml.h>
#pragma warning (push)
#pragma warning (disable : 4127 4511 4512 )
#include <boost/lexical_cast.hpp>
#pragma warning (pop)

using namespace kernel;
using namespace xml;
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
MaintenancePriorities::MaintenancePriorities( const OrderParameter& parameter, const Resolver_ABC< EquipmentType >& resolver, const ASN1T_LogMaintenancePriorities& asn )
    : Parameter< std::string >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddPriority( resolver.Get( asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: MaintenancePriorities constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
MaintenancePriorities::MaintenancePriorities( const OrderParameter& parameter, const Resolver_ABC< EquipmentType >& resolver, xml::xistream& xis )
    : Parameter< std::string >( parameter )
{
    std::string value;
    xis >> attribute( "value", value );
    QStringList list = QStringList::split( ";", value.c_str() );
    for( unsigned int i = 0; i < list.count(); ++i )
        AddPriority( resolver.Get( list[i].toUInt() ) );
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
void MaintenancePriorities::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_logMaintenancePriorities;
    ASN1T_LogMaintenancePriorities*& list = asn.value.u.logMaintenancePriorities = new ASN1T_LogMaintenancePriorities();
    list->n = priorities_.size();
    if( IsSet() )
    {
        list->elem = new ASN1T_EquipmentType[ list->n ];
        for( unsigned int i = 0; i < priorities_.size(); ++i )
            list->elem[i] = priorities_.at( i )->GetId();
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
// Name: MaintenancePriorities::Clean
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void MaintenancePriorities::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.logMaintenancePriorities )
        delete[] asn.value.u.logMaintenancePriorities->elem;
    delete asn.value.u.logMaintenancePriorities;
}

// -----------------------------------------------------------------------------
// Name: MaintenancePriorities::Serialize
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void MaintenancePriorities::Serialize( xml::xostream& xos ) const
{
    Parameter< std::string >::Serialize( xos );
    QString value( "" );
    for( T_Priorities::const_iterator it = priorities_.begin(); it != priorities_.end(); ++it )
        value += ( !value.isEmpty() ? ";" : "" ) + QString::number( (*it)->GetId() );
    xos << attribute( "value", value.ascii() );
}

// -----------------------------------------------------------------------------
// Name: MaintenancePriorities::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool MaintenancePriorities::IsSet() const
{
    return !priorities_.empty();
}
