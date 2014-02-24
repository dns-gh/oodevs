// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "DisasterPrototype.h"
#include "preparation/DisasterAttribute.h"
#include "preparation/WeatherModel.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/RichCheckBox.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DisasterPrototype constructor
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
DisasterPrototype::DisasterPrototype( QWidget* parent, const tools::GeneralConfig& config, kernel::Object_ABC*& creation,
                                      Controllers& controllers )
    : DisasterPrototype_ABC( parent, config )
    , controllers_( controllers )
    , creation_   ( creation )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DisasterPrototype destructor
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
DisasterPrototype::~DisasterPrototype()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: DisasterPrototype::NotifyUpdated
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
void DisasterPrototype::NotifyUpdated( const WeatherModel& model )
{
    date_->setDateTime( model.GetDate() );
}

// -----------------------------------------------------------------------------
// Name: DisasterPrototype::Commit
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
void DisasterPrototype::Commit( const kernel::Team_ABC& )
{
    if( creation_ )
    {
        gui::PropertiesDictionary& dictionary = creation_->Get< gui::PropertiesDictionary >();
        QDateTime date;
        if( checkbox_->isChecked() )
            date = date_->dateTime();
        creation_->Attach( *new DisasterAttribute( dictionary, tools::Path::FromUnicode( propagationFiles_->currentText().toStdWString() ), date, *creation_ ) );
    }
}
