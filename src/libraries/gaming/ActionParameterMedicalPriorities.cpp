// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterMedicalPriorities.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterMedicalPriorities constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
ActionParameterMedicalPriorities::ActionParameterMedicalPriorities( const OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterMedicalPriorities constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
ActionParameterMedicalPriorities::ActionParameterMedicalPriorities( const OrderParameter& parameter, const ASN1T_LogMedicalPriorities& asn )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddMedicalPriority( E_HumanWound( asn.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterMedicalPriorities constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
ActionParameterMedicalPriorities::ActionParameterMedicalPriorities( const kernel::OrderParameter& parameter, xml::xistream& xis )
    : ActionParameter< QString >( parameter )
{
    std::string value;
    xis >> attribute( "value", value );
    QStringList list = QStringList::split( ";", value.c_str() );
    for( unsigned int i = 0; i < list.count(); ++i )
        AddMedicalPriority( E_HumanWound( list[i].toUInt() ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterMedicalPriorities destructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
ActionParameterMedicalPriorities::~ActionParameterMedicalPriorities()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterMedicalPriorities::AddMedicalPriority
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ActionParameterMedicalPriorities::AddMedicalPriority( E_HumanWound value )
{
    priorities_.push_back( value );
    QString current = GetValue();
    if( !current.isEmpty() )
        current += ", ";
    SetValue( current + tools::ToString( value ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterMedicalPriorities::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ActionParameterMedicalPriorities::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_logMedicalPriorities;
    ASN1T_LogMedicalPriorities*& list = asn.value.u.logMedicalPriorities = new ASN1T_LogMedicalPriorities();
    list->n = priorities_.size();
    asn.null_value = list->n ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_EnumHumanWound[ list->n ];
    for( unsigned int i = 0; i < priorities_.size(); ++i )
        list->elem[i] = ASN1T_EnumHumanWound( priorities_.at( i ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterMedicalPriorities::Clean
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ActionParameterMedicalPriorities::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.logMedicalPriorities )
        delete[] asn.value.u.logMedicalPriorities->elem;
    delete asn.value.u.logMedicalPriorities;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterMedicalPriorities::Serialize
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ActionParameterMedicalPriorities::Serialize( xml::xostream& xos ) const
{
    ActionParameter< QString >::Serialize( xos );
    QString value;
    for( T_Priorities::const_iterator it = priorities_.begin(); it != priorities_.end(); ++it )
        value += ( !value.isEmpty() ? ";" : "" ) + QString::number( *it );
    xos << attribute( "value", value.ascii() );
}
