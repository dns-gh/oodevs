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
#include "clients_gui/RichGroupBox.h"
#include "clients_gui/RichWidget.h"
#include "clients_gui/RichPushButton.h"
#include "clients_kernel/Tools.h"
#include <string>

// -----------------------------------------------------------------------------
// Name: FilterDialog constructor
// Created: ABR 2011-06-20
// -----------------------------------------------------------------------------
FilterDialog::FilterDialog( const QString& objectName, QWidget* parent, xml::xistream& xis, const tools::ExerciseConfig& config )
    : QDialog       ( parent, "FilterDialog" )
    , filterManager_( 0 )
{
    gui::SubObjectName subObject( objectName + "FilterDialog" );

    // Filters list box
    list_ = new gui::RichWidget< QListWidget >( "filterList" );
    connect( list_, SIGNAL( currentRowChanged( int ) ), SLOT( OnSelectFilter( int ) ) );

    gui::RichGroupBox* selectFilterBox = new gui::RichGroupBox( "selectFilter", tools::translate( "FilterDialog", "Select filter:" ), this );
    QHBoxLayout* selectFilterBoxLayout = new QHBoxLayout( selectFilterBox );
    selectFilterBoxLayout->addWidget( list_ );
    
    // Descriptions
    description_ = new QLabel();
    description_->setWordWrap( true );

    gui::RichGroupBox* descriptionBox = new gui::RichGroupBox( "description", tools::translate( "FilterDialog", "Description:" ), this );
    descriptionBox->setMinimumHeight( 80 );
    QHBoxLayout* descriptionBoxLayout = new QHBoxLayout( descriptionBox );
    descriptionBoxLayout->addWidget( description_ );
    
    // Parameters
    stack_ = new QStackedWidget( this );
    connect( list_, SIGNAL( currentRowChanged( int ) ), stack_, SLOT( setCurrentIndex( int ) ) );
    
    // Buttons
    Q3HBox* box = new Q3HBox( this, "FilterDialog_ButtonsHBox" );
    box->setSpacing( 5 );
    box->setMaximumHeight( 40 );
    okButton_ = new gui::RichPushButton( "ok", tools::translate( "FilterDialog", "Ok" ), box );
    okButton_->setDefault( true );
    gui::RichPushButton* cancelBtn = new gui::RichPushButton( "cancel", tools::translate( "FilterDialog", "Cancel" ), box );
    connect( okButton_, SIGNAL( clicked() ), SLOT( OnAccept() ) );
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( OnReject() ) );
    
    //  Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout( this, 5, 5, "FilterDialog_MainLayout" );
    mainLayout->addWidget( selectFilterBox, 100 );
    mainLayout->addWidget( descriptionBox, 1 );
    mainLayout->addWidget( stack_, 1 );
    mainLayout->addWidget( box );

    // Manager
    {
        filterManager_.reset( new FilterManager( xis, config, *list_, *stack_, *parent ) );
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
        Filter_ABC& filter = filterManager_->GetFilter( list_->currentRow() );
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
    catch( const std::exception& e )
    {
        setEnabled( true );
        reject();
        QMessageBox::critical( this, tools::translate( "FilterDialog", "Error on filter execution" ), tools::GetExceptionMsg( e ).c_str() );
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
