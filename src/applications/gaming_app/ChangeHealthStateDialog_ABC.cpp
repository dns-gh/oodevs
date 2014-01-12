// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ChangeHealthStateDialog_ABC.h"
#include "moc_ChangeHealthStateDialog_ABC.cpp"
#include "actions/Action_ABC.h"
#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/tools.h"
#include "clients_gui/RichSpinBox.h"

// -----------------------------------------------------------------------------
// Name: ChangeHealthStateDialog_ABC constructor
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
ChangeHealthStateDialog_ABC::ChangeHealthStateDialog_ABC( QWidget* pParent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile, unsigned int numberOfLines )
    : QDialog( pParent, tools::translate( "ChangeHealthStateDialog_ABC", "Change health state" ), true )
    , controllers_ ( controllers )
    , actionsModel_( actionsModel )
    , simulation_  ( simulation )
    , profile_     ( profile )
    , selected_    ( controllers )
{
    // Init dialog
    setCaption( tools::translate( "ChangeHealthStateDialog_ABC", "Change health state" ) );
    resize( 250, 150 );
    // Main layout
    grid_ = new QGridLayout( this, numberOfLines + 4, 2, 5, 5 );
    // Separator
    QFrame* hline = new QFrame( this );
    hline->setFrameStyle( QFrame::HLine | QFrame::Sunken );
    grid_->addMultiCellWidget( hline, numberOfLines, numberOfLines, 0, 1 );
    // Labels
    grid_->addWidget( new QLabel( tools::translate( "InhabitantChangeHealthStateDialog", "New total:" ), this ), numberOfLines + 1, 0 );
    newTotalLabel_ = new QLabel( "0", this );
    grid_->addWidget( newTotalLabel_, numberOfLines + 1, 1 );
    grid_->addWidget( new QLabel( tools::translate( "InhabitantChangeHealthStateDialog", "Original total:" ), this ), numberOfLines + 2, 0 );
    originalTotalLabel_ = new QLabel( "0", this );
    grid_->addWidget( originalTotalLabel_, numberOfLines + 2, 1 );
    //Configurations
    newTotalLabel_->setAlignment( Qt::AlignCenter );
    newTotalLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    originalTotalLabel_->setAlignment( Qt::AlignCenter );
    originalTotalLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    // Buttons
    Q3HBox* buttonLayout = new Q3HBox( this );
    QPushButton* okButton     = new QPushButton( tr( "Ok" )    , buttonLayout );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), buttonLayout );
    grid_->addMultiCellWidget( buttonLayout, numberOfLines + 3, numberOfLines + 3, 0, 1 );
    okButton->setDefault( TRUE );
    // Signals
    connect( okButton    , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );

    selected_ = 0;
    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeHealthStateDialog_ABC destructor
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
ChangeHealthStateDialog_ABC::~ChangeHealthStateDialog_ABC()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ChangeHealthStateDialog_ABC::AddLine
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
void ChangeHealthStateDialog_ABC::AddLine( unsigned int line, const QString& objectName, const QString& label )
{
    grid_->addWidget( new QLabel( label, this ), line, 0 );
    spinboxes_[ line ] = new gui::RichSpinBox( objectName, this, 0, std::numeric_limits< int >::max(), 10 );
    grid_->addWidget( spinboxes_[ line ], line, 1 );
    connect( spinboxes_[ line ], SIGNAL( valueChanged( int ) ), SLOT( OnValuesChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: ChangeHealthStateDialog_ABC::GetSpinboxValue
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
unsigned int ChangeHealthStateDialog_ABC::GetSpinboxValue( unsigned int line ) const
{
    CIT_Spinboxes it = spinboxes_.find( line );
    if( it == spinboxes_.end() )
        throw MASA_EXCEPTION( "ChangeHealthStateDialog_ABC : line not found" );
    return it->second->value();
}

// -----------------------------------------------------------------------------
// Name: ChangeHealthStateDialog_ABC::GetSelected
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& ChangeHealthStateDialog_ABC::GetSelected() const
{
    if( !selected_ )
        throw MASA_EXCEPTION( "ChangeHealthStateDialog_ABC - entity not referenced" );
    return *selected_;
}

// -----------------------------------------------------------------------------
// Name: ChangeHealthStateDialog_ABC::NotifyContextMenu
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
void ChangeHealthStateDialog_ABC::DoNotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    if( profile_.CanDoMagic( entity ) )
    {
        selected_ = &entity;
        kernel::ContextMenu* subMenu = menu.SubMenu( "Order", tools::translate( "Magic orders", "Magic orders" ) );
        subMenu->insertItem( tools::translate( "ChangeHealthStateDialog_ABC", "Change health state" ), this, SLOT( Show() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeHealthStateDialog_ABC::Show
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void ChangeHealthStateDialog_ABC::Show()
{
    if( !selected_ )
        return;
    unsigned int total = 0;
    for( auto it = spinboxes_.begin(); it != spinboxes_.end(); ++it )
    {
        it->second->setValue( GetValue( it->first ) );
        total += it->second->value();
    }
    originalTotalLabel_->setText( locale().toString( total ) );
    OnValuesChanged();
    show();
}

// -----------------------------------------------------------------------------
// Name: ChangeHealthStateDialog_ABC::Validate
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void ChangeHealthStateDialog_ABC::Validate()
{
    if( ! selected_ )
        return;
    accept();
    CreateMagicAction( actionsModel_ );
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ChangeHealthStateDialog_ABC::Reject
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void ChangeHealthStateDialog_ABC::Reject()
{
    reject();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ChangeHealthStateDialog_ABC::closeEvent
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void ChangeHealthStateDialog_ABC::closeEvent( QCloseEvent * /* e */ )
{
    Reject();
}

// -----------------------------------------------------------------------------
// Name: ChangeHealthStateDialog_ABC::OnValuesChanged
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void ChangeHealthStateDialog_ABC::OnValuesChanged()
{
    unsigned int total = 0;
    for( auto it = spinboxes_.begin(); it != spinboxes_.end(); ++it )
        total += it->second->value();
    newTotalLabel_->setText( locale().toString( total ) );
}
