// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DisplayExtractor.h"
#include "moc_DisplayExtractor.cpp"
#include "InternalLinks.h"
#include "Tools.h"
#include "clients_kernel\Entity_ABC.h"
#include "clients_kernel\Agent_ABC.h"
#include "clients_kernel\Automat_ABC.h"
#include "clients_kernel\Formation_ABC.h"
#include "clients_kernel\Team_ABC.h"
#include "clients_kernel\Population_ABC.h"
#include "clients_kernel\Inhabitant_ABC.h"
#include "clients_kernel\KnowledgeGroup_ABC.h"
#include "clients_kernel\UrbanObject_ABC.h"
#include "clients_kernel\AgentKnowledge_ABC.h"
#include "clients_kernel\PopulationKnowledge_ABC.h"
#include "clients_kernel\ObjectKnowledge_ABC.h"
#include "clients_kernel\EquipmentType.h"
#include "clients_kernel\Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DisplayExtractor constructor
// Created: JSR 2012-10-18
// -----------------------------------------------------------------------------
DisplayExtractor::DisplayExtractor( QObject* parent )
    : QObject( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisplayExtractor destructor
// Created: JSR 2012-10-18
// -----------------------------------------------------------------------------
DisplayExtractor::~DisplayExtractor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisplayExtractor::NotifyLinkClicked
// Created: JSR 2012-10-18
// -----------------------------------------------------------------------------
void DisplayExtractor::NotifyLinkClicked( const QString& url )
{
    emit LinkClicked( url );
}

// -----------------------------------------------------------------------------
// Name: DisplayExtractor::GetDisplayName
// Created: JSR 2012-10-18
// -----------------------------------------------------------------------------
QString DisplayExtractor::GetDisplayName( const QString& element ) const
{
    return element;
}

// -----------------------------------------------------------------------------
// Name: DisplayExtractor::GetDisplayName
// Created: JSR 2012-10-18
// -----------------------------------------------------------------------------
QString DisplayExtractor::GetDisplayName( const std::string& element ) const
{
    return element.c_str();
}

// -----------------------------------------------------------------------------
// Name: DisplayExtractor::GetDisplayName
// Created: JSR 2012-10-18
// -----------------------------------------------------------------------------
QString DisplayExtractor::GetDisplayName( const unsigned long& element ) const
{
    static QLocale locale;
    return locale.toString( static_cast< const unsigned int >( element) );
}

// -----------------------------------------------------------------------------
// Name: DisplayExtractor::GetDisplayName
// Created: JSR 2012-10-24
// -----------------------------------------------------------------------------
QString DisplayExtractor::GetDisplayName( const QDateTime& element ) const
{
    return element.toString();
}

// -----------------------------------------------------------------------------
// Name: DisplayExtractor::GetDisplayName
// Created: JSR 2012-10-18
// -----------------------------------------------------------------------------
QString DisplayExtractor::GetDisplayName( const bool& element ) const
{
    static const QString yes = tools::translate( "Yes/No", "Yes" ) ;
    static const QString no = tools::translate( "Yes/No", "No" ) ;
    return element ? yes : no;
}

// -----------------------------------------------------------------------------
// Name: DisplayExtractor::GetDisplayName
// Created: JSR 2012-10-24
// -----------------------------------------------------------------------------
QString DisplayExtractor::GetDisplayName( const E_HumanWound& element ) const
{
    return tools::ToString( element );
}

#define LOCALE_TO_STRING( T ) \
QString DisplayExtractor::GetDisplayName( const T& element ) const \
{ \
    static QLocale locale; \
    return locale.toString( element); \
}

#define GET_ENTITY_NAME( T ) \
QString DisplayExtractor::GetDisplayName( const T& element ) const \
{ \
    return element.GetName(); \
}

#define GET_ENTITY_LINK( T ) \
QString DisplayExtractor::GetDisplayName( const T& element ) const \
{ \
    return InternalLinks::CreateLink( element, element.GetName() ) ; \
}

#define GET_STRING_NAME( T ) \
QString DisplayExtractor::GetDisplayName( const T& element ) const \
{ \
    return element.GetName().c_str(); \
}

LOCALE_TO_STRING( int )
LOCALE_TO_STRING( unsigned int )
LOCALE_TO_STRING( float )
LOCALE_TO_STRING( double )
LOCALE_TO_STRING( long )

GET_ENTITY_NAME( kernel::Entity_ABC )
GET_ENTITY_NAME( kernel::Team_ABC )
GET_ENTITY_NAME( kernel::KnowledgeGroup_ABC )
GET_ENTITY_NAME( kernel::AgentKnowledge_ABC )
GET_ENTITY_NAME( kernel::PopulationKnowledge_ABC )
GET_ENTITY_NAME( kernel::ObjectKnowledge_ABC )

GET_ENTITY_LINK( kernel::Agent_ABC )
GET_ENTITY_LINK( kernel::Automat_ABC )
GET_ENTITY_LINK( kernel::Formation_ABC )
GET_ENTITY_LINK( kernel::Population_ABC )
GET_ENTITY_LINK( kernel::Inhabitant_ABC )
GET_ENTITY_LINK( kernel::UrbanObject_ABC )

GET_STRING_NAME( kernel::EquipmentType )
