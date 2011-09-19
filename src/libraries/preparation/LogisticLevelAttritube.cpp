// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "LogisticLevelAttritube.h"
#include "LogisticBaseStates.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_gui/Tools.h"
#include <xeumeuleu/xml.hpp>
#include <QtGui/qmessagebox.h>

// -----------------------------------------------------------------------------
// Name: LogisticLevelAttritube constructor
// Created: LGY 2011-07-20
// -----------------------------------------------------------------------------
LogisticLevelAttritube::LogisticLevelAttritube( kernel::Controller& controller, const kernel::Entity_ABC& entity, bool active, kernel::PropertiesDictionary& dictionary )
    : controller_   ( controller )
    , entity_       ( entity )
    , logisticLevel_( active ? &kernel::LogisticLevel::logistic_base_ : &kernel::LogisticLevel::none_ ) // Logistic brain is enabled by default for type "tc2"
{
    CreateDictionary( dictionary, active );
}

// -----------------------------------------------------------------------------
// Name: LogisticLevelAttritube constructor
// Created: LGY 2011-07-20
// -----------------------------------------------------------------------------
LogisticLevelAttritube::LogisticLevelAttritube( kernel::Controller& controller, const kernel::Entity_ABC& entity, kernel::PropertiesDictionary& dictionary )
    : controller_   ( controller )
    , entity_       ( entity )
    , logisticLevel_( &kernel::LogisticLevel::none_ )
{
    CreateDictionary( dictionary, true );
}

// -----------------------------------------------------------------------------
// Name: LogisticLevelAttritube constructor
// Created: LGY 2011-07-20
// -----------------------------------------------------------------------------
LogisticLevelAttritube::LogisticLevelAttritube( kernel::Controller& controller, xml::xistream& xis, const kernel::Entity_ABC& entity, bool active, kernel::PropertiesDictionary& dictionary )
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
// Name: LogisticLevelAttritube::CreateDictionary
// Created: LGY 2011-07-20
// -----------------------------------------------------------------------------
void LogisticLevelAttritube::CreateDictionary( kernel::PropertiesDictionary& dictionary, bool active )
{
    if( active )
        dictionary.Register( entity_, tools::translate( "LogisticLevelAttritube", "Info/LogisticLevel" ), logisticLevel_, *this, &LogisticLevelAttritube::SetLogisticLevel );
}

// -----------------------------------------------------------------------------
// Name: LogisticLevelAttritube destructor
// Created: LGY 2011-07-20
// -----------------------------------------------------------------------------
LogisticLevelAttritube::~LogisticLevelAttritube()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticLevelAttritube::SetLogisticLevel
// Created: LGY 2011-07-20
// -----------------------------------------------------------------------------
void LogisticLevelAttritube::SetLogisticLevel( const kernel::EntityLogisticLevel& logisticLevel )
{
    if( (*logisticLevel) == kernel::LogisticLevel::none_ )
    {
        const LogisticHierarchiesBase* logHierarchy = entity_.Retrieve< LogisticHierarchiesBase >();
        if( logHierarchy )
        {
            tools::Iterator< const kernel::Entity_ABC& > children = logHierarchy->CreateSubordinateIterator();
            if( children.HasMoreElements() )
            {
                int result = QMessageBox::question( 0, tools::translate( "LogisticLevelAttritube", "SWORD" )
                                                     , tools::translate( "LogisticLevelAttritube", "By disabling the logistic function on this formation, all the logistic subordinates superiors will be reset. Do you want to proceed ?" )                                                    , QMessageBox::Yes, QMessageBox::Cancel );
                if( result == QMessageBox::Cancel )
                    return;

                while( children.HasMoreElements() )
                {
                    const kernel::Entity_ABC& entity = children.NextElement();
                    LogisticBaseStates* logEntityHierarchy = const_cast< LogisticBaseStates* >( dynamic_cast< const LogisticBaseStates* >( entity.Retrieve< LogisticHierarchiesBase >() ) );
                    if( logEntityHierarchy )
                        logEntityHierarchy->SetSuperior( kernel::LogisticBaseSuperior() );
                }
            }
        }
    }
    logisticLevel_ = logisticLevel;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticLevelAttritube::SerializeAttributes
// Created: LGY 2011-07-20
// -----------------------------------------------------------------------------
void LogisticLevelAttritube::SerializeAttributes( xml::xostream& xos ) const
{
    if( *logisticLevel_ != kernel::LogisticLevel::none_ )
        xos << xml::attribute( "logistic-level", logisticLevel_->GetName() );
}

// -----------------------------------------------------------------------------
// Name: LogisticLevelAttritube::GetLogisticLevel
// Created: LGY 2011-07-20
// -----------------------------------------------------------------------------
const kernel::LogisticLevel& LogisticLevelAttritube::GetLogisticLevel() const
{
    return *logisticLevel_;
}

// -----------------------------------------------------------------------------
// Name: LogisticLevelAttritube::GetEntity
// Created: ABR 2011-09-15
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& LogisticLevelAttritube::GetEntity() const
{
    return entity_;
}
