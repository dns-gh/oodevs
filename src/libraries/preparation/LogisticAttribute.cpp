// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "LogisticAttribute.h"

#include "clients_gui/LogisticBase.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Object_ABC.h"

#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: LogisticAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute( gui::PropertiesDictionary& dictionary, kernel::Controllers& controllers, const kernel::Object_ABC& object )
    : controllers_( controllers )
    , logisticBase_( 0 )
    , object_      ( object )
    , idToConvert_ ( 0 )
    , pAutomats_   ( 0 )
    , pFormations_ ( 0 )
{
    CreateDictionary( dictionary );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute( xml::xistream& xis, const tools::Resolver_ABC< kernel::Automat_ABC >& automats,
                                      const tools::Resolver_ABC< kernel::Formation_ABC >& formations, gui::PropertiesDictionary& dictionary,
                                      kernel::Controllers& controllers, const kernel::Object_ABC& object )
    : controllers_( controllers )
    , logisticBase_( 0 )
    , object_      ( object )
    , idToConvert_ ( xis.attribute< unsigned long >( "id" ) )
    , pAutomats_   ( &automats )
    , pFormations_ ( &formations )
{
    CreateDictionary( dictionary );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Finalize
// Created: JSR 2011-11-04
// -----------------------------------------------------------------------------
void LogisticAttribute::Finalize()
{
    if( pAutomats_ )
        logisticBase_ = pAutomats_->Find( idToConvert_ );
    if( !logisticBase_ && pFormations_ )
        logisticBase_ = pFormations_->Find( idToConvert_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
LogisticAttribute::~LogisticAttribute()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::SetLogisticBase
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
void LogisticAttribute::SetLogisticBase( const kernel::Entity_ABC& logisticBase )
{
    logisticBase_ = &logisticBase;
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::SetLogisticBase
// Created: MMC 2012-05-30
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* LogisticAttribute::GetLogisticBase() const
{
    return static_cast< const kernel::Entity_ABC* >( logisticBase_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::NotifyDeleted
// Created: SBO 2010-05-20
// -----------------------------------------------------------------------------
void LogisticAttribute::NotifyDeleted( const kernel::Automat_ABC& entity )
{
    if( &entity == logisticBase_ )
        logisticBase_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::NotifyDeleted
// Created: SBO 2010-05-20
// -----------------------------------------------------------------------------
void LogisticAttribute::NotifyDeleted( const kernel::Formation_ABC& entity )
{
    if( &entity == logisticBase_ )
        logisticBase_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void LogisticAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Camp" ) )
                .Display( tools::translate( "Object", "Logistic base:" ), logisticBase_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::HasValidLogisticBase
// Created: JSR 2012-01-09
// -----------------------------------------------------------------------------
bool LogisticAttribute::HasValidLogisticBase() const
{
    if( !logisticBase_ )
        return false;
    return logisticBase_ ? logisticBase_->Get< gui::LogisticBase >().IsBase() : false;
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::SerializeObjectAttributes
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void LogisticAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "logistic-base" )
            << xml::attribute( "id", logisticBase_ ? logisticBase_->GetId() : 0 )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::CreateDictionary
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void LogisticAttribute::CreateDictionary( gui::PropertiesDictionary& dictionary )
{
    dictionary.RegisterExtension( object_, this, tools::translate( "LogisticAttribute", "Info/Camp attributes/Logistic base" ), logisticBase_ );
}
