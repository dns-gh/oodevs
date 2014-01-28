// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ConsignDialog.h"
#include "moc_ConsignDialog.cpp"
#include "clients_kernel/Tools.h"
#include "clients_gui/RichPushButton.h"
#include "gaming/LogisticsConsign_ABC.h"
#include "actions/ActionsModel.h"

// -----------------------------------------------------------------------------
// Name: ConsignDialog constructor
// Created: LGY 2014-01-21
// -----------------------------------------------------------------------------
ConsignDialog::ConsignDialog( const QString& objectName, QWidget* parent,
                              actions::ActionsModel& actionsModel )
    : gui::RichWidget< QDialog >( objectName, parent )
    , actionsModel_( actionsModel )
{
    QVBoxLayout* mainLayout = new QVBoxLayout();
    gui::RichPushButton* applyButton = new gui::RichPushButton( "automated_resolution_button",
        tools::translate( "ConsignDialog", "Apply the automated resolution" ) );
    connect( applyButton, SIGNAL( clicked() ), SLOT( OnApply() ) );
    mainLayout->addWidget( applyButton );
    setLayout( mainLayout );
}

// -----------------------------------------------------------------------------
// Name: ConsignDialog destructor
// Created: LGY 2014-01-21
// -----------------------------------------------------------------------------
ConsignDialog::~ConsignDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConsignDialog::Show
// Created: LGY 2014-01-21
// -----------------------------------------------------------------------------
void ConsignDialog::Show( const LogisticsConsign_ABC& consign )
{
    id_ = consign.GetId();
    setWindowTitle( tools::translate( "ConsignDialog", "request" ) + " " + QString::number( id_ ) );
    show();
}

// -----------------------------------------------------------------------------
// Name: ConsignDialog::OnApply
// Created: LGY 2014-01-21
// -----------------------------------------------------------------------------
void ConsignDialog::OnApply()
{
    actionsModel_.PublishSelectNewLogisticState( id_ );
    hide();
}
