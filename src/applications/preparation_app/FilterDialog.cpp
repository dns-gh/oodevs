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

// -----------------------------------------------------------------------------
// Name: FilterDialog constructor
// Created: ABR 2011-06-20
// -----------------------------------------------------------------------------
FilterDialog::FilterDialog( QWidget* parent, xml::xistream& xis, const tools::ExerciseConfig& config, Model& model )
    : QDialog       ( parent, "FilterDialog" )
    , filterManager_( 0 )
{
    QVBoxLayout* mainLayout = new QVBoxLayout( this, 5, 5, "FilterDialog_MainLayout" );
    // Filters list box
    {
        Q3GroupBox* box = new Q3GroupBox( 1, Qt::Horizontal, tools::translate( "FilterDialog", "Select filter:" ), this, "FilterDialog_FiltersGroupBox" );
        list_ = new Q3ListBox( box, "FilterDialog_FiltersListBox" );
        connect( list_, SIGNAL( highlighted( int ) ), SLOT( OnSelectFilter( int ) ) );
        connect( list_, SIGNAL( returnPressed( Q3ListBoxItem* ) ), SLOT( OnAccept() ) );
        mainLayout->addWidget( box );
    }
    // Descriptions
    {
        Q3GroupBox* box = new Q3GroupBox( 1, Qt::Horizontal, tools::translate( "FilterDialog", "Description:" ), this, "FilterDialog_DescriptionGroupBox" );
        box->setMinimumHeight( 80 );
        description_ = new QLabel( box, "FilterDialog_DescriptionLabel" );
        mainLayout->addWidget( box );
    }
    // Parameters
    {
        stack_ = new Q3WidgetStack( this, "FilterDialog_ParametersStack" );
        connect( list_, SIGNAL( highlighted( int ) ), stack_, SLOT( raiseWidget( int ) ) );
        mainLayout->addWidget( stack_ );
    }
    // Buttons
    {
        Q3HBox* box = new Q3HBox( this, "FilterDialog_ButtonsHBox" );
        box->setSpacing( 5 );
        box->setMaximumHeight( 40 );
        okButton_ = new QPushButton( tools::translate( "FilterDialog", "Ok" ), box, "FilterDialog_OkButton" );
        okButton_->setDefault( true );
        okButton_->setAutoDefault( true );
        QPushButton* cancelBtn = new QPushButton( tools::translate( "FilterDialog", "Cancel" ), box, "FilterDialog_CancelButton" );
        connect( okButton_, SIGNAL( clicked() ), SLOT( OnAccept() ) );
        connect( cancelBtn, SIGNAL( clicked() ), SLOT( OnReject() ) );
        mainLayout->addWidget( box );
    }
    // Manager
    {
        filterManager_.reset( new FilterManager( xis, config, *list_, *stack_, *parent, model ) );
    }
    // Dialog setting
    {
        setCaption( filterManager_->GetName().c_str() );
        setMinimumSize( 500, 500 );
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
    if( !okButton_->isEnabled() )
        return;
    try
    {
        Filter_ABC& filter = filterManager_->GetFilter( list_->currentItem() );
        assert( filter.IsValid() );
        if( filter.NeedToReloadExercise() && static_cast< MainWindow* >( parent() )->CheckSaving() == QMessageBox::Cancel )
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
        setEnabled( true );
        reject();
        QMessageBox::critical( this, tools::translate( "FilterDialog", "Error on filter execution" ), e.what() );
    }
    catch( ... )
    {
        setEnabled( true );
        reject();
        QMessageBox::critical( this, tools::translate( "FilterDialog", "Error on filter execution" ), "Unexpected exception" );
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
// Name: FilterDialog::showEvent
// Created: ABR 2012-05-30
// -----------------------------------------------------------------------------
void FilterDialog::showEvent( QShowEvent * e )
{
    filterManager_->Update();
    QWidget::showEvent( e );
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
// Name: FilterDialog::GetName
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
const QString FilterDialog::GetName() const
{
    return filterManager_->GetName().c_str();
}

// -----------------------------------------------------------------------------
// Name: FilterDialog::GetKeySequence
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
const QKeySequence FilterDialog::GetKeySequence() const
{
    const std::string& name = filterManager_->GetId();
    QKeySequence keys = 0;
    if( name == "import" )
        keys = Qt::CTRL + Qt::Key_I;
    else if( name == "export" )
        keys = Qt::CTRL + Qt::Key_E;
    return keys;
}
