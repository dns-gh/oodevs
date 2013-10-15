// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AfterAction.h"
#include "moc_AfterAction.cpp"
#include "AfterActionFunctionList.h"
#include "AfterActionRequestList.h"
#include "clients_gui/FileDialog.h"
#include "clients_kernel/Tools.h"
#include "gaming/AfterActionModel.h"
#include "tools/ExerciseConfig.h"
#include "icons.h"

// -----------------------------------------------------------------------------
// Name: AfterAction constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterAction::AfterAction( QWidget* parent, kernel::Controllers& controllers, const tools::ExerciseConfig& config, AfterActionModel& model,
                          IndicatorPlotFactory& plotFactory, actions::gui::InterfaceBuilder_ABC& interfaceBuilder )
    : gui::RichDockWidget( controllers, parent, "after-action" )
    , config_( config )
    , model_( model )
{
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable( true );

    QWidget* main = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout( main );
    QHBoxLayout* toolbarLayout = new QHBoxLayout;
    QToolButton* load = new QToolButton;
    load->setAutoRaise( true );
    load->setPixmap( MAKE_PIXMAP( open ) );
    QToolButton* save = new QToolButton;
    save->setAutoRaise( true );
    save->setPixmap( MAKE_PIXMAP( save ) );
    toolbarLayout->addWidget( load );
    toolbarLayout->addWidget( save );
    toolbarLayout->addStretch();

    QTabWidget* functionsTab = new QTabWidget;
    AfterActionFunctionList* list = new AfterActionFunctionList( functionsTab, controllers, model, interfaceBuilder );
    functionsTab->addTab( list, tools::translate( "AfterAction", "Creation" ) );
    AfterActionRequestList* requests = new AfterActionRequestList( functionsTab, controllers, plotFactory );
    functionsTab->addTab( requests, tools::translate( "AfterAction", "Requests" ) );

    setFeatures( QDockWidget::AllDockWidgetFeatures );
    setWindowTitle( tools::translate( "AfterAction", "After action review" ) );
    layout->addLayout( toolbarLayout );
    layout->addWidget( functionsTab );

    scrollArea->setWidget( main );
    setWidget( scrollArea );

    connect( load, SIGNAL( clicked() ), SLOT( OnLoad() ) );
    connect( save, SIGNAL( clicked() ), SLOT( OnSave() ) );
}

// -----------------------------------------------------------------------------
// Name: AfterAction destructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterAction::~AfterAction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AfterAction::OnLoad
// Created: JSR 2013-10-11
// -----------------------------------------------------------------------------
void AfterAction::OnLoad()
{
    tools::Path filename = gui::FileDialog::getOpenFileName( this, tr( "Load request file" ), config_.BuildExerciseChildFile( "" ), tr( "Requests (*.xml)" ) );
    if( filename.IsEmpty() )
        return;
    filename.MakePreferred();
    try
    {
        model_.LoadRequests( config_.GetLoader(), filename );
    }
    catch( const xml::exception& )
    {
        QMessageBox::critical( this, tr( "Error" ), tr( "'%1' is not a valid request file." ).arg( filename.ToUTF8().c_str() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AfterAction::OnSave
// Created: JSR 2013-10-11
// -----------------------------------------------------------------------------
void AfterAction::OnSave()
{
    tools::Path filename = gui::FileDialog::getSaveFileName( this, tr( "Save requests to file" ), config_.BuildExerciseChildFile( "" ), tr( "Requests (*.xml)" ) );
    if( filename.IsEmpty() )
        return;
    filename.MakePreferred();
    if( filename.Extension() != ".xml" )
        filename.ReplaceExtension( ".xml" );
    try
    {
        model_.SaveRequests( filename );
    }
    catch( const xml::exception& )
    {
        QMessageBox::critical( this, tr( "Error" ), tr( "Unable to save requests to file '%1'." ).arg( filename.ToUTF8().c_str() ) );
    }
}

