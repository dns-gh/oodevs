// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "InitialStateDialog.h"
#include "moc_InitialStateDialog.cpp"
#include "InitialStateTableCrew.h"
#include "InitialStateTableEquipments.h"
#include "InitialStateTableResources.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/tools.h"
#include "preparation/InitialState.h"

// -----------------------------------------------------------------------------
// Name: InitialStateDialog constructor
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
InitialStateDialog::InitialStateDialog( QWidget* parent, kernel::Controllers& controllers, const StaticModel& staticModel )
    : QDialog( parent, "InitialStateDialog_Dialog", true )
    , controllers_( controllers )
    , staticModel_( staticModel )
    , selected_   ( controllers )
{
    // Init dialog
    setCaption( tools::translate( "InitialStateDialog", "Initial state" ) );
    resize( 450, 450 );
    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout( this, 0, 5, "InitialStateDialog_VBoxLayout_MainLayout" );
    mainLayout->setMargin( 5 );
    // Tabs
    QTabWidget* tabWidget = new QTabWidget( this, "InitialStateDialog_TabWidget_Tabs" );
    tabs_.push_back( boost::shared_ptr< InitialStateTableCrew >       ( new InitialStateTableCrew(       tabWidget, "InitialStateDialog_TableCrew" ) ) );
    tabs_.push_back( boost::shared_ptr< InitialStateTableEquipments > ( new InitialStateTableEquipments( tabWidget, "InitialStateDialog_TableEquipments" ) ) );
    tabs_.push_back( boost::shared_ptr< InitialStateTableResources >  ( new InitialStateTableResources(  tabWidget, staticModel_, "InitialStateDialog_TableResources" ) ) );
    tabWidget->addTab( tabs_[ eCrew      ].get(), tools::translate( "InitialStateDialog", "Crew" ) );
    tabWidget->addTab( tabs_[ eEquipment ].get(), tools::translate( "InitialStateDialog", "Equipments" ) );
    tabWidget->addTab( tabs_[ eResources ].get(), tools::translate( "InitialStateDialog", "Resources" ) );
    // Buttons
    Q3HBox* buttons = new Q3HBox( this, "InitialStateDialog_HBox_Buttons" );
    buttons->setSpacing( 5 );
    QPushButton* resetButton  = new QPushButton( tr( "Reset" ) , buttons, "InitialStateDialog_PushButton_Reset" );
    QPushButton* okButton     = new QPushButton( tr( "Ok" )    , buttons, "InitialStateDialog_PushButton_Ok" );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), buttons, "InitialStateDialog_PushButton_Cancel" );
    okButton->setDefault( true );
    // Layouts
    mainLayout->add( tabWidget );
    mainLayout->add( buttons );
    // Signals
    connect( resetButton , SIGNAL( clicked() ), SLOT( Reset() ) );
    connect( okButton    , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    // Register
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InitialStateDialog destructor
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
InitialStateDialog::~InitialStateDialog()
{
    selected_ = 0;
    controllers_.Unregister( *this );
    tabs_.clear();
}

// -----------------------------------------------------------------------------
// Name: InitialStateDialog::Show
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
void InitialStateDialog::Show()
{
    if( !selected_ )
        return;
    const InitialState& extension = selected_->Get< InitialState >();
    for( unsigned int i = 0; i < tabs_.size(); ++i )
        tabs_[ i ]->ReadExtension( extension );
    show();
}

// -----------------------------------------------------------------------------
// Name: InitialStateDialog::Validate
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
void InitialStateDialog::Validate()
{
    if( !selected_ )
        return;
    accept();
    InitialState& extension = selected_.ConstCast()->Get< InitialState >();
    for( unsigned int i = 0; i < tabs_.size(); ++i )
        tabs_[ i ]->WriteExtension( extension );
    controllers_.controller_.Update( *selected_ );
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: InitialStateDialog::Reset
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
void InitialStateDialog::Reset()
{
    if( !selected_ )
        return;
    InitialState& extension = selected_.ConstCast()->Get< InitialState >();
    extension.Reset();
    for( unsigned int i = 0; i < tabs_.size(); ++i )
        tabs_[ i ]->ReadExtension( extension );
    controllers_.controller_.Update( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: InitialStateDialog::NotifyContextMenu
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
void InitialStateDialog::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = const_cast< kernel::Agent_ABC* > ( &entity );
    menu.InsertItem( "Update", tools::translate( "InitialStateDialog", "Change initial state" ), this, SLOT( Show() ) );
}

// -----------------------------------------------------------------------------
// Name: InitialStateDialog::Reject
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
void InitialStateDialog::Reject()
{
    reject();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: InitialStateDialog::closeEvent
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
void InitialStateDialog::closeEvent( QCloseEvent * /*e*/ )
{
    Reject();
}
