// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "PlanificationModePanel.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/tools.h"
#include "tools/GeneralConfig.h"

namespace
{
    QPixmap MakePixmap( const tools::Path& name )
    {
        return gui::Pixmap( tools::GeneralConfig::BuildResourceChildFile( tools::Path( "images/gaming" ) / name + ".png" ) );
    }
}

// -----------------------------------------------------------------------------
// }: PlanificationModePanel constructor
// Created: LGY 2012-06-21
// -----------------------------------------------------------------------------
PlanificationModePanel::PlanificationModePanel( QMainWindow* parent, kernel::Controllers& controllers )
    : gui::RichDockWidget( controllers, parent, "planification", tools::translate( "PlanificationModePanel", "Planification mode" ) )
    , controllers_( controllers )
{
    QWidget* main = new QWidget( this );
    QHBoxLayout* mainLayout = new QHBoxLayout( main );
    mainLayout->setAlignment( Qt::AlignCenter );
    QLabel* label = new QLabel();
    label->setPixmap( MakePixmap( "warning" ) );
    mainLayout->addWidget( label );
    setWidget( main );

    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PlanificationModePanel destructor
// Created: LGY 2012-06-21
// -----------------------------------------------------------------------------
PlanificationModePanel::~PlanificationModePanel()
{
    controllers_.Unregister( *this );
}
