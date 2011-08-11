// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::UnitStateDialog */

#include "clients_gui_pch.h"
#include "UnitStateDialog.h"
#include "moc_UnitStateDialog.cpp"
#include "UnitStateTable_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Team_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitStateDialog constructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateDialog::UnitStateDialog( QWidget* parent, kernel::Controllers& controllers )
    : QDialog( parent, "UnitStateDialog_Dialog", false )
    , controllers_( controllers )
    , selected_   ( controllers )
{
    // Init dialog
    setCaption( tr( "Unit state" ) );
    resize( 600, 600 );
    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout( this, 0, 5, "UnitStateDialog_VBoxLayout_MainLayout" );
    mainLayout->setMargin( 5 );
    // Header
    Q3HBox* header = new Q3HBox( this, "UnitStateDialog_HBox_Header" );
    header->setSpacing( 5 );
    selectedEntityLabel_ = new QLabel( header, "UnitStateDialog_Label_SelectedEntity" );
    selectedEntityLabel_->setMinimumHeight( 40 );
    selectedEntityLabel_->setMinimumWidth( 150 );
    selectedEntityLabel_->setAlignment( Qt::AlignCenter );
    selectedEntityLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    // Tabs
    tabWidget_ = new QTabWidget( this, "UnitStateDialog_TabWidget_Tabs" );
    // Buttons
    Q3HBox* buttons = new Q3HBox( this, "UnitStateDialog_HBox_Buttons" );
    buttons->setSpacing( 5 );
    resetButton_             = new QPushButton( tr( "Reset" )   , buttons, "UnitStateDialog_PushButton_Reset" );
    validateButton_          = new QPushButton( tr( "Validate" ), buttons, "UnitStateDialog_PushButton_Validate" );
    QPushButton* closeButton = new QPushButton( tr( "Close" )   , buttons, "UnitStateDialog_PushButton_Cancel" );
    validateButton_->setDefault( true );
    // Layouts
    mainLayout->add( header );
    mainLayout->add( tabWidget_ );
    mainLayout->add( buttons );
    // Signals
    connect( resetButton_   , SIGNAL( clicked() ), SLOT( Reset() ) );
    connect( validateButton_, SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( closeButton    , SIGNAL( clicked() ), SLOT( hide() ) );
    // Register
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog destructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateDialog::~UnitStateDialog()
{
    selected_ = 0;
    controllers_.Unregister( *this );
    tabs_.clear();
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::NotifySelected
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateDialog::NotifySelected( const kernel::Entity_ABC* element )
{
    if( selected_ == element )
        return;
    if( selected_ && selected_->GetTypeName() == kernel::Agent_ABC::typeName_ && isShown() )
        for( unsigned int i = 0; i < tabs_.size(); ++i )
            if( tabs_[ i ]->HasChanged( *selected_.ConstCast() ) )
            {
                int nResult = QMessageBox::information( this, tr( "Sword" ), QString( "You have unsaved modifications on unit %1, do you want to send them ?" ).arg( selected_->GetName() ), QMessageBox::Yes, QMessageBox::No );
                if( nResult == QMessageBox::Yes )
                    Validate();
                break;
            }
    selectedEntityLabel_->setText( ( element ) ? element->GetName() : tr( "---" ) );
    selected_ = const_cast< kernel::Entity_ABC* >( element );
    if( isShown() )
        UnitStateDialog::Reset();
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::Show
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
void UnitStateDialog::Show()
{
    if( !selected_ )
        return;
    UnitStateDialog::Reset();
    show();
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::Validate
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateDialog::Validate() const
{
    if( !selected_ || selected_->GetTypeName() != kernel::Agent_ABC::typeName_ )
        return;
    for( unsigned int i = 0; i < tabs_.size(); ++i )
        if( tabs_[ i ]->HasChanged( *selected_.ConstCast() ) )
            tabs_[ i ]->Commit( *selected_.ConstCast() );
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::Reset
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateDialog::Reset()
{
    // Purge content
    for( unsigned int i = 0; i < tabs_.size(); ++i )
        tabs_[ i ]->Purge();
    // Disable if needed
    bool enable = ( selected_ ) ? selected_->GetTypeName() == kernel::Agent_ABC::typeName_ : false;
    for( unsigned int i = 0; i < tabs_.size(); ++i )
        tabs_[ i ]->setReadOnly( !enable );
    resetButton_->setEnabled( enable );
    validateButton_->setEnabled( enable );
    // Exit if needed
    if( !selected_ )
        return;
    // Load content from selected if needed
    QString typeName = selected_->GetTypeName();
    if( typeName == kernel::Agent_ABC::typeName_ || typeName == kernel::Automat_ABC::typeName_ ||
        typeName == kernel::Formation_ABC::typeName_ || typeName == kernel::Team_ABC::typeName_ )
        for( unsigned int i = 0; i < tabs_.size(); ++i )
            tabs_[ i ]->RecursiveLoad( *selected_.ConstCast() );
}
