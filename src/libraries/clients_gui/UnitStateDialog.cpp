// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UnitStateDialog.h"
#include "moc_UnitStateDialog.cpp"
#include "RichPushButton.h"
#include "RichWidget.h"
#include "SubObjectName.h"
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
    // Init
    SubObjectName subObject( "UnitStateDialog" );
    setCaption( tr( "State" ) );
    resize( 900, 600 );

    // Header
    Q3HBox* targetBox = new Q3HBox( this, "UnitStateDialog_HBox_Header" );
    targetBox->setSpacing( 5 );
    selectedEntityLabel_ = new QLabel( targetBox, "UnitStateDialog_Label_SelectedEntity" );
    selectedEntityLabel_->setMinimumHeight( 40 );
    selectedEntityLabel_->setMinimumWidth( 150 );
    selectedEntityLabel_->setAlignment( Qt::AlignCenter );
    selectedEntityLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    // Tabs
    tabWidget_ = new RichWidget< QTabWidget >( "tabs", this );
    // Buttons
    resetButton_ = new RichPushButton( "reset", tr( "Reset" ) );
    validateButton_ = new RichPushButton( "validate", tr( "Validate" ) );
    validateButton_->setDefault( true );
    RichPushButton* closeButton = new RichPushButton( "close", tr( "Close" ) );

    // Layouts
    headerLayout_ = new QHBoxLayout( );
    headerLayout_->setSpacing( 5 );
    headerLayout_->addWidget( targetBox, 1 );

    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch( 1 );
    buttonsLayout->addWidget( validateButton_ );
    buttonsLayout->addWidget( resetButton_ );
    buttonsLayout->addWidget( closeButton );

    QVBoxLayout* mainLayout = new QVBoxLayout( this, 0, 5, "UnitStateDialog_VBoxLayout_MainLayout" );
    mainLayout->setMargin( 5 );
    mainLayout->addLayout( headerLayout_ );
    mainLayout->addWidget( tabWidget_, 1 );
    mainLayout->addLayout( buttonsLayout );

    // Connections
    connect( resetButton_   , SIGNAL( clicked() ), SLOT( Reset() ) );
    connect( validateButton_, SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( closeButton    , SIGNAL( clicked() ), SLOT( hide() ) );

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
// Name: UnitStateDialog::IsReadOnly
// Created: ABR 2011-08-12
// -----------------------------------------------------------------------------
bool UnitStateDialog::IsReadOnly() const
{
    for( unsigned int i = 0; i < tabs_.size(); ++i )
        if( !tabs_[ i ]->IsReadOnly() )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: UnitStateDialog::NotifySelected
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateDialog::NotifySelected( const kernel::Entity_ABC* element )
{
    if( selected_ == element )
        return;
    if( !IsReadOnly() && selected_ && isShown() )
        for( unsigned int i = 0; i < tabs_.size(); ++i )
            if( tabs_[ i ]->HasChanged( *selected_.ConstCast() ) )
            {
                int nResult = QMessageBox::information( this, tr( "Sword" ), tr( "You have unsaved modifications on unit %1 on the %2 tab, do you want to validate ?" ).arg( selected_->GetName() ).arg( tabWidget_->tabText( i ) ), QMessageBox::Yes, QMessageBox::No );
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
    if( !selected_ )
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
    bool readOnly = IsReadOnly();
    bool activate = !readOnly;
    for( unsigned int i = 0; i < tabs_.size(); ++i )
    {
        bool tabReadOnly = selected_? tabs_[ i ]->IsReadOnlyForType( selected_->GetTypeName() ) : true;
        tabs_[ i ]->SetReadOnly( readOnly | tabReadOnly );
        activate |= !tabReadOnly;
    }
    resetButton_->setEnabled( activate );
    validateButton_->setEnabled( activate );
    // Exit if needed
    if( !selected_ )
        return;
    // Load content from selected if needed
    const std::string& typeName = selected_->GetTypeName();
    if( typeName == kernel::Agent_ABC::typeName_ || typeName == kernel::Automat_ABC::typeName_ ||
        typeName == kernel::Formation_ABC::typeName_ || typeName == kernel::Team_ABC::typeName_ )
        for( unsigned int i = 0; i < tabs_.size(); ++i )
            tabs_[ i ]->RecursiveLoad( *selected_.ConstCast() );
}
