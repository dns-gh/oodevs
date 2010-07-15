// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ExportWidget.h"
#include "moc_ExportWidget.cpp"
#include "clients_gui/tools.h"
#include <qlistview.h>
#include <qprogressbar.h>

// -----------------------------------------------------------------------------
// Name: ExportWidget constructor
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
ExportWidget::ExportWidget( ScenarioEditPage& page, QWidget* parent, const tools::GeneralConfig& config )
    : QGroupBox( 1, Qt::Vertical, parent )
    , config_( config )
    , page_( page )
{
    setFrameShape( QFrame::NoFrame );
    setMargin( 5 );
    setBackgroundOrigin( QWidget::WindowOrigin );
    QGroupBox* group = new QGroupBox( 2, Qt::Horizontal, tools::translate( "ExportWidget", "Create a package" ), this );
    group->setBackgroundOrigin( QWidget::WindowOrigin );
    {
        QLabel* label = new QLabel(  tools::translate( "ExportWidget", "Exercise to package:" ), group );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        list_ = new QListBox( group );
        list_->setBackgroundOrigin( QWidget::WindowOrigin );
    }
    {
        QLabel* label = new QLabel(  tools::translate( "ExportWidget", "Package description:" ), group );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        description_ = new QTextEdit( group );
        description_->setBackgroundOrigin( QWidget::WindowOrigin );
        description_->setMaximumHeight( 30 );
        description_->setReadOnly( false );
    }
    {
        QLabel* label = new QLabel(  tools::translate( "ExportWidget", "Package content:" ), group );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        content_ = new QListView( group );
        content_->setBackgroundOrigin( QWidget::WindowOrigin );
        content_->addColumn( "exercise features" );
        content_->adjustSize();
    }

    progress_ = new QProgressBar( this );
    progress_->hide();
}

// -----------------------------------------------------------------------------
// Name: ExportWidget destructor
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
ExportWidget::~ExportWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::ExportPackage
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
void ExportWidget::ExportPackage()
{

}
