// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterMaintenancePriorities.h"
#include "clients_kernel/EquipmentType.h"
#include "xeumeuleu/xml.h"
#pragma warning (push)
#pragma warning (disable : 4127 4511 4512 )
#include <boost/lexical_cast.hpp>
#pragma warning (pop)

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterMaintenancePriorities constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
ActionParameterMaintenancePriorities::ActionParameterMaintenancePriorities( const OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterMaintenancePriorities constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
ActionParameterMaintenancePriorities::ActionParameterMaintenancePriorities( const OrderParameter& parameter, const Resolver_ABC< EquipmentType >& resolver, const ASN1T_LogMaintenancePriorities& asn )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddPriority( resolver.Get( asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterMaintenancePriorities constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
ActionParameterMaintenancePriorities::ActionParameterMaintenancePriorities( const OrderParameter& parameter, const Resolver_ABC< EquipmentType >& resolver, xml::xistream& xis )
    : ActionParameter< QString >( parameter )
{
    std::string value;
    xis >> attribute( "value", value );
    QStringList list = QStringList::split( ";", value.c_str() );
    for( unsigned int i = 0; i < list.count(); ++i )
        AddPriority( resolver.Get( list[i].toUInt() ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterMaintenancePriorities destructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
ActionParameterMaintenancePriorities::~ActionParameterMaintenancePriorities()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ActionParameterMaintenancePriorities::AddPriority
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ActionParameterMaintenancePriorities::AddPriority( const EquipmentType& value )
{
    priorities_.push_back( &value );
    QString current = GetValue();
    if( !current.isEmpty() )
        current += ", ";
    SetValue( current + value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterMaintenancePriorities::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ActionParameterMaintenancePriorities::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_logMaintenancePriorities;
    ASN1T_LogMaintenancePriorities*& list = asn.value.u.logMaintenancePriorities = new ASN1T_LogMaintenancePriorities();
    list->n = priorities_.size();
    asn.null_value = list->n ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_EquipmentType[ list->n ];
    for( unsigned int i = 0; i < priorities_.size(); ++i )
        list->elem[i] = priorities_.at( i )->GetId();
}

// -----------------------------------------------------------------------------
// Name: ActionParameterMaintenancePriorities::CommitTo
// Created: AGE 2007-10-24
// -----------------------------------------------------------------------------
void ActionParameterMaintenancePriorities::CommitTo( std::string& content ) const
{
    for( T_Priorities::const_iterator it = priorities_.begin(); it != priorities_.end(); ++it )
    {
        if( !content.empty() )
            content += ",";
        content += boost::lexical_cast< std::string >( (*it)->GetId() );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterMaintenancePriorities::Clean
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ActionParameterMaintenancePriorities::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.logMaintenancePriorities )
        delete[] asn.value.u.logMaintenancePriorities->elem;
    delete asn.value.u.logMaintenancePriorities;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterMaintenancePriorities::Serialize
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ActionParameterMaintenancePriorities::Serialize( xml::xostream& xos ) const
{
    ActionParameter< QString >::Serialize( xos );
    QString value( "" );
    for( T_Priorities::const_iterator it = priorities_.begin(); it != priorities_.end(); ++it )
        value += ( !value.isEmpty() ? ";" : "" ) + QString::number( (*it)->GetId() );
    xos << attribute( "value", value.ascii() );
}
