// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "MedicalPriorities.h"
#include "ParameterVisitor_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: MedicalPriorities constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
MedicalPriorities::MedicalPriorities( const OrderParameter& parameter )
    : Parameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalPriorities constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
MedicalPriorities::MedicalPriorities( const OrderParameter& parameter, const sword::LogMedicalPriorities& message )
    : Parameter< QString >( parameter )
{
    for( int i = 0; i < message.elem_size(); ++i )
        AddMedicalPriority( E_HumanWound( message.elem().Get(i) ) );
}

// -----------------------------------------------------------------------------
// Name: MedicalPriorities destructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
MedicalPriorities::~MedicalPriorities()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalPriorities::AddMedicalPriority
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void MedicalPriorities::AddMedicalPriority( E_HumanWound value )
{
    priorities_.push_back( value );
    QString current = GetValue();
    if( !current.isEmpty() )
        current += ", ";
    SetValue( current + tools::ToString( value ) );
}

// -----------------------------------------------------------------------------
// Name: MedicalPriorities::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void MedicalPriorities::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() )
    {
        sword::LogMedicalPriorities* list =message.add_value()->mutable_logmedicalpriorities();
        for( unsigned int i = 0; i < priorities_.size(); ++i )
            list->add_elem( sword::EnumHumanWound( priorities_.at( i ) ) );
    }
}
// -----------------------------------------------------------------------------
// Name: MedicalPriorities::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void MedicalPriorities::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() )
    {
        sword::LogMedicalPriorities* list = message.mutable_logmedicalpriorities();
        for( unsigned int i = 0; i < priorities_.size(); ++i )
            list->add_elem( sword::EnumHumanWound( priorities_.at( i ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalPriorities::Serialize
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void MedicalPriorities::Serialize( xml::xostream& xos ) const
{
    Parameter< QString >::Serialize( xos );
    QString value( "" );
    for( T_Priorities::const_iterator it = priorities_.begin(); it != priorities_.end(); ++it )
        value += ( !value.isEmpty() ? ";" : "" ) + QString::number( *it );
    if( !value.isEmpty() )
        xos << xml::attribute( "value", value.toStdString() );
}

// -----------------------------------------------------------------------------
// Name: MedicalPriorities::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool MedicalPriorities::IsSet() const
{
    return !priorities_.empty();
}

// -----------------------------------------------------------------------------
// Name: MedicalPriorities::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string MedicalPriorities::SerializeType() const
{
    return "medicalpriorities";
}

// -----------------------------------------------------------------------------
// Name: MedicalPriorities::Accept
// Created: ABR 2013-06-13
// -----------------------------------------------------------------------------
void MedicalPriorities::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
