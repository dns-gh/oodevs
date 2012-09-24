// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ChangeAutomatTypeDialog.h"
#include "moc_ChangeAutomatTypeDialog.cpp"
#include "ModelBuilder.h"
#include "clients_gui/UnitTreeView.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"

// -----------------------------------------------------------------------------
// Name: ChangeAutomatTypeDialog constructor
// Created: JSR 2012-06-29
// -----------------------------------------------------------------------------
ChangeAutomatTypeDialog::ChangeAutomatTypeDialog( QWidget* parent, kernel::Controllers& controllers, const kernel::AgentTypes& list, gui::ModelObserver_ABC& builder, kernel::Entity_ABC& entity, const std::string& typeName )
    : QDialog( parent )
    , controllers_( controllers )
    , builder_    ( builder )
    , entity_     ( entity )
{
    setModal( true );
    setCaption( tr( "Replace by a new automat" ) );

    QLabel* label1 = new QLabel( tr( "Select new automat type for ") + entity_.GetName() );
    QLabel* label2 = new QLabel( tr( "Current type: ") + typeName.c_str() );
    list_ = new gui::UnitTreeView( controllers, list, this, true );
    list_->SetSorting( "" );
    list_->setFixedSize( 300, 600 );
    okBtn_ = new QPushButton( tr( "Ok" ) );
    okBtn_->setEnabled( false );
    okBtn_->setDefault( true );
    QPushButton* cancelBtn = new QPushButton( tr( "Cancel" ) );

    connect( list_->selectionModel() , SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), SLOT( SelectionChanged() ) );
    connect( okBtn_, SIGNAL( clicked() ), SLOT( OnOk() ) );
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( reject() ) );

    QGridLayout* layout = new QGridLayout( this );
    layout->setSpacing( 10 );
    layout->addWidget( label1, 0, 0, 1, 2 );
    layout->addWidget( label2, 1, 0, 1, 2 );
    layout->addWidget( list_, 2, 0, 1, 2 );
    layout->setRowStretch( 2, 1 );
    layout->addWidget( okBtn_, 3, 0 );
    layout->addWidget( cancelBtn, 3, 1 );

    exec();
}

// -----------------------------------------------------------------------------
// Name: ChangeAutomatTypeDialog destructor
// Created: JSR 2012-06-29
// -----------------------------------------------------------------------------
ChangeAutomatTypeDialog::~ChangeAutomatTypeDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ChangeAutomatTypeDialog::SelectionChanged
// Created: JSR 2012-06-29
// -----------------------------------------------------------------------------
void ChangeAutomatTypeDialog::SelectionChanged()
{
    okBtn_->setEnabled( list_->GetSelected< kernel::AutomatType >() != 0 );
}

// -----------------------------------------------------------------------------
// Name: ChangeAutomatTypeDialog::OnOk
// Created: JSR 2012-06-29
// -----------------------------------------------------------------------------
void ChangeAutomatTypeDialog::OnOk()
{
    if( const kernel::AutomatType* type = list_->GetSelected< kernel::AutomatType >() )
        if( kernel::Automat_ABC* result = builder_.ReplaceAutomat( entity_, *type ) )
            result->Select( controllers_.actions_ );
    accept();
}
