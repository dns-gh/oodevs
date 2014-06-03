// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TaskerWidget.h"
#include "moc_TaskerWidget.cpp"
#include "EntitySymbols.h"
#include "ImageWrapper.h"
#include "RichGroupBox.h"
#include "RichLabel.h"
#include "RichPushButton.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "tools/GeneralConfig.h"
#include <boost/assign/list_of.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TaskerWidget constructor
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
TaskerWidget::TaskerWidget( kernel::Controllers& controllers,
                            const gui::EntitySymbols& symbols,
                            const QString& title,
                            bool showActivate /* = true */,
                            bool showClear /* = true */,
                            QWidget* parent /* = 0 */ )
    : QWidget( parent )
    , controllers_( controllers )
    , symbols_( symbols )
    , tasker_( 0 )
    , activateButton_( 0 )
    , clearButton_( 0 )
{
    setMinimumHeight( 64 );
    nameLabel_ = new gui::RichLabel( "event-target-label", "---" );
    symbolLabel_ = new gui::RichLabel( "event-target-symbol-label" );
    groupBox_ = new gui::RichGroupBox( "event-target-groupbox", title );
    connect( groupBox_, SIGNAL( clicked( bool ) ), SIGNAL( Clicked( bool ) ) );

    QWidget* symbolWidget = new QWidget();
    QHBoxLayout* symbolLayout = new QHBoxLayout( symbolWidget );
    symbolLayout->setMargin( 0 );
    symbolLayout->addWidget( symbolLabel_ );
    symbolLayout->addWidget( nameLabel_ );

    QHBoxLayout* layout = new QHBoxLayout( groupBox_ );
    layout->setContentsMargins( 5, 0, 5, 5 );
    layout->addWidget( symbolWidget, 10, Qt::AlignCenter );

    activateButton_ = new gui::RichPushButton( "activateTargetButton", gui::Icon( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/center_time.png" ) ), "" );
    activateButton_->setToolTip( tr( "Select" ) );
    connect( activateButton_, SIGNAL( clicked() ), this, SLOT( OnActivateClicked() ) );
    layout->addWidget( activateButton_, 1, Qt::AlignRight );
    activateButton_->setVisible( showActivate );

    clearButton_ = new gui::RichPushButton( "removeTargetButton", qApp->style()->standardIcon( QStyle::SP_DialogCloseButton ), "" );
    clearButton_->setToolTip( tr( "Remove" ) );
    connect( clearButton_, SIGNAL( clicked() ), this, SIGNAL( ClearClicked() ) );
    layout->addWidget( clearButton_, 1, Qt::AlignRight );
    clearButton_->setVisible( showClear );

    QHBoxLayout* mainLayout = new QHBoxLayout( this );
    mainLayout->setMargin( 0 );
    mainLayout->addWidget( groupBox_ );

    controllers_.controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TaskerWidget destructor
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
TaskerWidget::~TaskerWidget()
{
    controllers_.controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TaskerWidget::EnableStaticWarning
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
void TaskerWidget::EnableStaticWarning( bool warn, const QColor& color /* = QColor() */ )
{
    groupBox_->EnableStaticWarning( warn, color );
    nameLabel_->EnableStaticWarning( warn, color );
}

// -----------------------------------------------------------------------------
// Name: TaskerWidget::BlockSignals
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
void TaskerWidget::BlockSignals( bool blocked )
{
    activateButton_->blockSignals( blocked );
    clearButton_->blockSignals( blocked );
}

// -----------------------------------------------------------------------------
// Name: TaskerWidget::SetTasker
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
void TaskerWidget::SetTasker( const kernel::Entity_ABC* entity )
{
    if( tasker_ == entity )
        return;
    tasker_ = entity;
    bool hasTasker = tasker_ != 0;
    nameLabel_->setText( hasTasker ? tasker_->GetName() : "---" );
    activateButton_->setEnabled( hasTasker );
    clearButton_->setEnabled( hasTasker );
    QPixmap pixmap;
    if( hasTasker )
        if( auto symbol = tasker_->Retrieve< kernel::TacticalHierarchies >() )
        {
            pixmap = symbols_.GetSymbol( *tasker_, symbol->GetSymbol(), symbol->GetLevel(),
                                         QSize( 64, 64 ), gui::EntitySymbols::eColorWithModifier );
            pixmap = pixmap.scaled( QSize( 48, 48 ), Qt::KeepAspectRatio, Qt::SmoothTransformation );
        }
    symbolLabel_->setPixmap( pixmap );
    emit TaskerChanged( tasker_ );
}

// -----------------------------------------------------------------------------
// Name: TaskerWidget::GetTasker
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* TaskerWidget::GetTasker() const
{
    return tasker_;
}

// -----------------------------------------------------------------------------
// Name: TaskerWidget::OnActivateClicked
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
void TaskerWidget::OnActivateClicked()
{
    if( !tasker_ )
        throw MASA_EXCEPTION( "Can't activate an unset target" );
    tasker_->Select( controllers_.actions_ );
    tasker_->MultipleSelect( controllers_.actions_, boost::assign::list_of( tasker_ ) );
    tasker_->Activate( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: TaskerWidget::OnClearClicked
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
void TaskerWidget::OnClearClicked()
{
    SetTasker( 0 );
}

// -----------------------------------------------------------------------------
// Name: TaskerWidget::OnClearClicked
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
void TaskerWidget::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    if( tasker_ && tasker_ == &entity )
        SetTasker( 0 );
}

// -----------------------------------------------------------------------------
// Name: TaskerWidget::setCheckable
// Created: ABR 2014-05-28
// -----------------------------------------------------------------------------
void TaskerWidget::setCheckable( bool checkable )
{
    groupBox_->setCheckable( checkable );
}

// -----------------------------------------------------------------------------
// Name: TaskerWidget::isCheckable
// Created: ABR 2014-05-28
// -----------------------------------------------------------------------------
bool TaskerWidget::isCheckable() const
{
    return groupBox_->isCheckable( );
}

// -----------------------------------------------------------------------------
// Name: TaskerWidget::setChecked
// Created: ABR 2014-05-28
// -----------------------------------------------------------------------------
void TaskerWidget::setChecked( bool checked )
{
    groupBox_->setChecked( checked );
}

// -----------------------------------------------------------------------------
// Name: TaskerWidget::isChecked
// Created: ABR 2014-05-28
// -----------------------------------------------------------------------------
bool TaskerWidget::isChecked() const
{
    return groupBox_->isChecked();
}
