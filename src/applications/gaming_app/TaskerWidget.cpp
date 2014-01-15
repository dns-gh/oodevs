// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TaskerWidget.h"
#include "moc_TaskerWidget.cpp"
#include "clients_gui/EntitySymbols.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/RichGroupBox.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/RichPushButton.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "tools/GeneralConfig.h"
#include <boost/assign/list_of.hpp>

// -----------------------------------------------------------------------------
// Name: TaskerWidget constructor
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
TaskerWidget::TaskerWidget( kernel::Controllers& controllers,
                            const gui::EntitySymbols& symbols,
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
    nameLabel_ = new gui::RichLabel( "event-order-target-label", "---" );
    symbolLabel_ = new gui::RichLabel( "event-order-target-symbol-label" );
    groupBox_ = new gui::RichGroupBox( "event-order-target-groupbox", tr( "Recipient" ) );

    QWidget* symbolWidget = new QWidget();
    QHBoxLayout* symbolLayout = new QHBoxLayout( symbolWidget );
    symbolLayout->setMargin( 0 );
    symbolLayout->addWidget( symbolLabel_ );
    symbolLayout->addWidget( nameLabel_ );

    QHBoxLayout* layout = new QHBoxLayout( groupBox_ );
    layout->setContentsMargins( 5, 0, 5, 5 );
    layout->addWidget( symbolWidget, 10, Qt::AlignCenter );

    if( showActivate )
    {
        activateButton_ = new gui::RichPushButton( "activateTargetButton", gui::Icon( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/center_time.png" ) ), "" );
        activateButton_->setToolTip( tr( "Select" ) );
        connect( activateButton_, SIGNAL( clicked() ), this, SLOT( OnActivateClicked() ) );
        layout->addWidget( activateButton_, 1, Qt::AlignRight );
    }

    if( showClear )
    {
        clearButton_ = new gui::RichPushButton( "removeTargetButton", qApp->style()->standardIcon( QStyle::SP_DialogCloseButton ), "" );
        clearButton_->setToolTip( tr( "Remove" ) );
        connect( clearButton_, SIGNAL( clicked() ), this, SIGNAL( ClearClicked() ) );
        layout->addWidget( clearButton_, 1, Qt::AlignRight );
    }

    QHBoxLayout* mainLayout = new QHBoxLayout( this );
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
void TaskerWidget::SetTasker( kernel::Entity_ABC* entity )
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
