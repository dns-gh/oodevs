// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "LogisticLevelAttribute.h"
#include "LogisticBaseStates.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>
#include <QtGui/qmessagebox.h>

// -----------------------------------------------------------------------------
// Name: LogisticLevelAttribute constructor
// Created: LGY 2011-07-20
// -----------------------------------------------------------------------------
LogisticLevelAttribute::LogisticLevelAttribute( kernel::Controller& controller, const kernel::Entity_ABC& entity, bool active, kernel::PropertiesDictionary& dictionary )
    : controller_   ( controller )
    , entity_       ( entity )
    , logisticLevel_( active ? &kernel::LogisticLevel::logistic_base_ : &kernel::LogisticLevel::none_ ) // Logistic brain is enabled by default for type "tc2"
{
    CreateDictionary( dictionary, active );
}

// -----------------------------------------------------------------------------
// Name: LogisticLevelAttribute constructor
// Created: LGY 2011-07-20
// -----------------------------------------------------------------------------
LogisticLevelAttribute::LogisticLevelAttribute( kernel::Controller& controller, const kernel::Entity_ABC& entity, kernel::PropertiesDictionary& dictionary )
    : controller_   ( controller )
    , entity_       ( entity )
    , logisticLevel_( &kernel::LogisticLevel::none_ )
{
    CreateDictionary( dictionary, true );
}

// -----------------------------------------------------------------------------
// Name: LogisticLevelAttribute constructor
// Created: LGY 2011-07-20
// -----------------------------------------------------------------------------
LogisticLevelAttribute::LogisticLevelAttribute( kernel::Controller& controller, xml::xistream& xis, const kernel::Entity_ABC& entity, bool active, kernel::PropertiesDictionary& dictionary )
    : controller_   ( controller )
    , entity_       ( entity )
    , logisticLevel_( &kernel::LogisticLevel::none_ )
{
    std::string logLevelName = "none";
    xis >> xml::optional >> xml::attribute( "logistic-level", logLevelName );
    logisticLevel_ = const_cast< kernel::LogisticLevel* >( &kernel::LogisticLevel::Resolve( logLevelName ) );
    CreateDictionary( dictionary, active );
}

// -----------------------------------------------------------------------------
// Name: LogisticLevelAttribute::CreateDictionary
// Created: LGY 2011-07-20
// -----------------------------------------------------------------------------
void LogisticLevelAttribute::CreateDictionary( kernel::PropertiesDictionary& dictionary, bool active )
{
    if( active )
        dictionary.Register( entity_, tools::translate( "LogisticLevelAttribute", "Info/LogisticLevel" ), logisticLevel_, *this, &LogisticLevelAttribute::SetLogisticLevel );
}

// -----------------------------------------------------------------------------
// Name: LogisticLevelAttribute destructor
// Created: LGY 2011-07-20
// -----------------------------------------------------------------------------
LogisticLevelAttribute::~LogisticLevelAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticLevelAttribute::SetLogisticLevel
// Created: LGY 2011-07-20
// -----------------------------------------------------------------------------
void LogisticLevelAttribute::SetLogisticLevel( const kernel::EntityLogisticLevel& logisticLevel )
{
    if( (*logisticLevel) == kernel::LogisticLevel::none_ )
    {
        const LogisticHierarchiesBase* logHierarchy = entity_.Retrieve< LogisticHierarchiesBase >();
        if( logHierarchy )
        {
            tools::Iterator< const kernel::Entity_ABC& > children = logHierarchy->CreateSubordinateIterator();
            if( children.HasMoreElements() )
            {
                int result = QMessageBox::question( 0, tools::translate( "LogisticLevelAttribute", "SWORD" )
                                                     , tools::translate( "LogisticLevelAttribute", "By disabling the logistic function on this formation, all the logistic subordinates superiors will be reset. Do you want to proceed ?" )                                                    , QMessageBox::Yes, QMessageBox::Cancel );
                if( result == QMessageBox::Cancel )
                    return;

                while( children.HasMoreElements() )
                {
                    const kernel::Entity_ABC& entity = children.NextElement();
                    if( LogisticHierarchiesBase* logEntityHierarchy = const_cast< kernel::Entity_ABC& >( entity ).Retrieve< LogisticHierarchiesBase >() )
                        logEntityHierarchy->SetLogisticSuperior( kernel::LogisticBaseSuperior() );
                }
            }
        }
    }
    logisticLevel_ = logisticLevel;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticLevelAttribute::SerializeAttributes
// Created: LGY 2011-07-20
// -----------------------------------------------------------------------------
void LogisticLevelAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    if( *logisticLevel_ != kernel::LogisticLevel::none_ )
        xos << xml::attribute( "logistic-level", logisticLevel_->GetName() );
}

// -----------------------------------------------------------------------------
// Name: LogisticLevelAttribute::GetLogisticLevel
// Created: LGY 2011-07-20
// -----------------------------------------------------------------------------
const kernel::LogisticLevel& LogisticLevelAttribute::GetLogisticLevel() const
{
    return *logisticLevel_;
}

// -----------------------------------------------------------------------------
// Name: LogisticLevelAttribute::GetEntity
// Created: ABR 2011-09-15
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& LogisticLevelAttribute::GetEntity() const
{
    return entity_;
}
