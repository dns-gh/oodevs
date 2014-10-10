// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PresetDialog.h"
#include "moc_PresetDialog.cpp"
#include "Gradient.h"
#include "GradientPreferences.h"
#include "RichPushButton.h"
#include "RichWidget.h"
#include "SignalAdapter.h"
#include "SubObjectName.h"
#include "Tools.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PresetDialog constructor
// Created: LGY 2010-09-24
// -----------------------------------------------------------------------------
PresetDialog::PresetDialog( QWidget* parent,
                            kernel::OptionsController& options,
                            const std::shared_ptr< Gradient >& gradient,
                            GradientPreferences& preferences )
    : QDialog( parent )
{
    if( !gradient )
    {
        accept();
        return;
    }
    SubObjectName subObject( "rename-preset-dialog" );

    setFixedSize( 300, 80 );
    setCaption( tr( "Rename preset" ) );
    setModal( true );

    auto lineEdit = new RichWidget< QLineEdit >( "editline" );
    lineEdit->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    auto okButton = new RichPushButton( "ok", tr( "Ok" ) );
    okButton->setDefault( true );
    auto cancelButton = new RichPushButton( "cancel", tr( "Cancel" ) );

    auto buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch( 1 );
    buttonLayout->addWidget( okButton );
    buttonLayout->addWidget( cancelButton );

    QFrame* hline = new QFrame( this );
    hline->setFrameStyle( QFrame::HLine | QFrame::Sunken );

    auto layout = new QVBoxLayout( this );
    layout->addWidget( tools::AddLabeledWidget( tr( "New name:" ), lineEdit, 0, 0 ) );
    layout->addWidget( hline );
    layout->addLayout( buttonLayout );

    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    gui::connect( okButton, SIGNAL( clicked() ), [=,&options](){
        QString newName = lineEdit->text();
        if( newName.isEmpty() )
            QMessageBox::warning( this, tr( "Warning" ), tr( "Preset name is empty.") );
        else if( preferences.GetByName( newName ) )
            QMessageBox::warning( this, tr( "Warning" ), tr( "Preset name already exists." ) );
        else
        {
            const auto oldName = gradient->GetName();
            gradient->SetName( newName );
            options.Remove( "Elevation/Gradients/" + oldName.toStdString() );
            options.Change( "Elevation/Gradients/" + newName.toStdString(), gradient->GetValues() );
            options.Change( "Elevation/Gradient", newName );
            accept();
        }
    } );
}

// -----------------------------------------------------------------------------
// Name: PresetDialog destructor
// Created: LGY 2010-09-24
// -----------------------------------------------------------------------------
PresetDialog::~PresetDialog()
{
    // NOTHING
}
