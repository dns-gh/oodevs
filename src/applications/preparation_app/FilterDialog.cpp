// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FilterDialog.h"
#include "moc_FilterDialog.cpp"

#include "Filter_ABC.h"
#include "FilterManager.h"
#include "Menu.h"
#include "MainWindow.h"
#include "clients_kernel/Tools.h"
#include <string>

const int FilterDialog::menuIndex_ = 5;

// -----------------------------------------------------------------------------
// Name: FilterDialog constructor
// Created: ABR 2011-06-20
// -----------------------------------------------------------------------------
FilterDialog::FilterDialog( QWidget* parent, xml::xistream& xis, const tools::ExerciseConfig& config )
    : QDialog       ( parent, "FilterDialog" )
    , filterManager_( 0 )
{
    QVBoxLayout* mainLayout = new QVBoxLayout( this, 5, 5, "FilterDialog_MainLayout" );
    // Filters list box
    {
        QGroupBox* box = new QGroupBox( 1, Qt::Horizontal, tools::translate( "FilterDialog", "Select filter:" ), this, "FilterDialog_FiltersGroupBox" );
        list_ = new QListBox( box, "FilterDialog_FiltersListBox" );
        connect( list_, SIGNAL( highlighted( int ) ), SLOT( OnSelectFilter( int ) ) );
        mainLayout->addWidget( box );
    }
    // Descriptions
    {
        QGroupBox* box = new QGroupBox( 1, Qt::Horizontal, tools::translate( "FilterDialog", "Description:" ), this, "FilterDialog_DescriptionGroupBox" );
        box->setMinimumHeight( 80 );
        description_ = new QLabel( box, "FilterDialog_DescriptionLabel" );
        mainLayout->addWidget( box );
    }
    // Parameters
    {
        stack_ = new QWidgetStack( this, "FilterDialog_ParametersStack" );
        connect( list_, SIGNAL( highlighted( int ) ), stack_, SLOT( raiseWidget( int ) ) );
        mainLayout->addWidget( stack_ );
    }
    // Buttons
    {
        QHBox* box = new QHBox( this, "FilterDialog_ButtonsHBox" );
        box->setMargin( 5 );
        box->setMaximumHeight( 40 );
        okButton_ = new QPushButton( tools::translate( "FilterDialog", "Ok" ), box, "FilterDialog_OkButton" );
        okButton_->setDefault( true );
        QButton* cancelBtn = new QPushButton( tools::translate( "FilterDialog", "Cancel" ), box, "FilterDialog_CancelButton" );
        connect( okButton_, SIGNAL( clicked() ), SLOT( OnAccept() ) );
        connect( cancelBtn, SIGNAL( clicked() ), SLOT( OnReject() ) );
        mainLayout->addWidget( box );
    }
    // Manager
    {
        filterManager_.reset( new FilterManager( xis, config, *list_, *stack_ ) );
    }
    // Dialog setting
    {
        setCaption( filterManager_->GetName().c_str() );
        setMinimumSize( 400, 400 );
        connect( this, SIGNAL( reloadExercise() ), parent, SLOT( ReloadExercise() ) );
        hide();
    }
}

// -----------------------------------------------------------------------------
// Name: FilterDialog destructor
// Created: ABR 2011-06-20
// -----------------------------------------------------------------------------
FilterDialog::~FilterDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterDialog::sizeHint
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
QSize FilterDialog::sizeHint() const
{
    return QSize( 400, 400 );
}

// -----------------------------------------------------------------------------
// Name: FilterDialog::OnSelectFilter
// Created: ABR 2011-06-20
// -----------------------------------------------------------------------------
void FilterDialog::OnSelectFilter( int index )
{
    Filter_ABC& filter = filterManager_->GetFilter( index );
    description_->setText( filter.GetDescription().c_str() );
    disconnect( okButton_, SLOT( setEnabled( bool ) ) );
    connect( &filter, SIGNAL( statusChanged( bool ) ), okButton_, SLOT( setEnabled( bool ) ) );
    okButton_->setEnabled( filter.IsValid() );
}

// -----------------------------------------------------------------------------
// Name: FilterDialog::OnAccept
// Created: ABR 2011-06-20
// -----------------------------------------------------------------------------
void FilterDialog::OnAccept()
{
    try
    {
        Filter_ABC& filter = filterManager_->GetFilter( list_->currentItem() );
        assert( filter.IsValid() );
        if( filter.NeedToReloadExercise() && !static_cast< MainWindow* >( parent() )->CheckSaving() )
            return;
        setEnabled( false );
        filter.Execute();
        setEnabled( true );
        accept();
        if( filter.NeedToReloadExercise() )
            emit reloadExercise();
    }
    catch( std::exception& e )
    {
        QMessageBox::critical( this, tools::translate( "FilterDialog", "Error on filter execution" ), e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: FilterDialog::OnReject
// Created: ABR 2011-06-20
// -----------------------------------------------------------------------------
void FilterDialog::OnReject()
{
    reject();
}

// -----------------------------------------------------------------------------
// Name: FilterDialog::closeEvent
// Created: ABR 2011-06-24
// -----------------------------------------------------------------------------
void FilterDialog::closeEvent( QCloseEvent * e )
{
    if( isEnabled() )
        QWidget::closeEvent( e );
}

// -----------------------------------------------------------------------------
// Name: FilterDialog::AddFilter
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
void FilterDialog::AddFilter( Filter_ABC& filter )
{
    assert( filterManager_.get() && list_ && stack_ );
    filterManager_->AddFilter( filter, *list_, *stack_ );
}

// -----------------------------------------------------------------------------
// Name: FilterDialog::InsertMenuEntry
// Created: ABR 2011-06-24
// -----------------------------------------------------------------------------
void FilterDialog::InsertMenuEntry( Menu& menu )
{
    assert( filterManager_.get() );
    const std::string& name = filterManager_->GetId();
    QKeySequence keys = 0;
    if( name == "import" )
        keys = CTRL + Key_I;
    else if( name == "export" )
        keys = CTRL + Key_E;
    menu.InsertFileMenuEntry( filterManager_->GetName().c_str(), this, SLOT( exec() ), keys, menuIndex_ );
}
