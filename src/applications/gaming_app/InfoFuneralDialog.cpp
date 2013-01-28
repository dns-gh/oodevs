// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoFuneralDialog.h"
#include "LogisticConsignsWidget.h"
#include "clients_kernel/Tools.h"
#include "gaming/LogFuneralConsign.h"

// -----------------------------------------------------------------------------
// Name: InfoFuneralDialog constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
InfoFuneralDialog::InfoFuneralDialog( QWidget* parent, kernel::Controllers& controllers, kernel::DisplayExtractor_ABC& extractor )
    : InfoDialog_Base( parent, tools::translate( "InfoFuneralDialog", "Funeral system" ) )
    , controllers_( controllers )
{
    controllers_.Register( *this );
    QTabWidget* tabs = new QTabWidget( RootWidget() );
    tabs->addTab( new LogisticConsignsWidget< LogFuneralConsign, LogFuneralConsigns >( tabs, controllers, extractor ), tools::translate( "InfoFuneralDialog", "Consigns" ) );
    setMinimumWidth( 400 );
}

// -----------------------------------------------------------------------------
// Name: InfoFuneralDialog destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
InfoFuneralDialog::~InfoFuneralDialog()
{
    controllers_.Unregister( *this );
}

namespace
{
    struct FuneralRevelant
    {
        FuneralRevelant() {}
        bool operator()( const kernel::Entity_ABC& element )
        {
            const LogFuneralConsigns* consigns = element.Retrieve< LogFuneralConsigns >();
            return ( consigns && consigns->IsRelevant() );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: InfoFuneralDialog::ShouldDisplay
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
bool InfoFuneralDialog::ShouldDisplay( const kernel::Entity_ABC& element ) const
{
    FuneralRevelant funeralRevelant;
    return logistic_helpers::CheckEntityAndSubordinatesUpToBaseLog( element, funeralRevelant );
}

// -----------------------------------------------------------------------------
// Name: InfoDialog::NotifySelected
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void InfoFuneralDialog::NotifySelected( const kernel::Entity_ABC* element )
{
    if( element )
        SetEnabled( element && ShouldDisplay( *element ) );
}
