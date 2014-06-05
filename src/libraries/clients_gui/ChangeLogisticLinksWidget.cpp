// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ChangeLogisticLinksWidget.h"
#include "moc_ChangeLogisticLinksWidget.cpp"
#include "LogisticHelpers.h"

#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Formation_ABC.h"

using namespace gui;

namespace
{
    QLabel* CreateColor( const QColor& color )
    {
        QPixmap pixmap( 10 , 10 );
        pixmap.fill( color );
        QLabel* label = new QLabel();
        label->setPixmap( pixmap );
        label->setMaximumWidth( 10 );
        return label;
    }
    ValuedComboBox< const kernel::Entity_ABC* >* CreateCombo( const QString& objectName,
                                                              const QString& defaultText )    
    {
        auto combo = new ValuedComboBox< const kernel::Entity_ABC* >( objectName, 0 );
        combo->AddItem( defaultText, 0 );
        return combo;
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksWidget constructor
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
ChangeLogisticLinksWidget::ChangeLogisticLinksWidget( QWidget* parent,
                                                      kernel::Controllers& controllers )
    : QWidget( parent )
    , controllers_( controllers )
{
    nominalCombo_ = CreateCombo( "nominalSuperiorCombo", tr( "None" ) );
    currentCombo_ = CreateCombo( "currentSuperiorCombo", tr( "None" ) );

    QGridLayout* layout = new QGridLayout( this );
    layout->addWidget( CreateColor( Qt::yellow ), 0, 0 );
    layout->addWidget( new QLabel( tr( "Nominal superior" ) ), 0, 1 );
    layout->addWidget( nominalCombo_, 0, 2 );

    currentColor_ = CreateColor( QColor( Qt::yellow ).lighter( 150 ) );
    currentLabel_ = new QLabel( tr( "Current superior" ) );
    layout->addWidget( currentColor_, 1, 0 );
    layout->addWidget( currentLabel_, 1, 1 );
    layout->addWidget( currentCombo_, 1, 2 );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksWidget destructor
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
ChangeLogisticLinksWidget::~ChangeLogisticLinksWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksWidget::SetCurrentVisible
// Created: ABR 2014-06-02
// -----------------------------------------------------------------------------
void ChangeLogisticLinksWidget::SetCurrentVisible( bool visible )
{
    currentColor_->setVisible( visible );
    currentLabel_->setVisible( visible );
    currentCombo_->setVisible( visible );
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksWidget::SetNominalSuperior
// Created: ABR 2014-06-02
// -----------------------------------------------------------------------------
void ChangeLogisticLinksWidget::SetNominalSuperior( const kernel::Entity_ABC* entity )
{
    nominalCombo_->SetCurrentItem( entity );
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksWidget::SetCurrentSuperior
// Created: ABR 2014-06-02
// -----------------------------------------------------------------------------
void ChangeLogisticLinksWidget::SetCurrentSuperior( const kernel::Entity_ABC* entity )
{
    currentCombo_->SetCurrentItem( entity );
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksWidget::GetNominalSuperior
// Created: ABR 2014-06-02
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* ChangeLogisticLinksWidget::GetNominalSuperior() const
{
    return nominalCombo_->GetValue();
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksWidget::GetCurrentSuperior
// Created: ABR 2014-06-02
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* ChangeLogisticLinksWidget::GetCurrentSuperior() const
{
    return currentCombo_->GetValue();
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksWidget::AddItem
// Created: ABR 2014-06-02
// -----------------------------------------------------------------------------
void ChangeLogisticLinksWidget::AddItem( const kernel::Entity_ABC& entity )
{
    if( !logistic_helpers::IsLogisticBase( entity ) )
        return;
    nominalCombo_->AddItem( entity.GetName(), &entity );
    currentCombo_->AddItem( entity.GetName(), &entity );
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksWidget::RemoveItem
// Created: ABR 2014-06-02
// -----------------------------------------------------------------------------
void ChangeLogisticLinksWidget::RemoveItem( const kernel::Entity_ABC& entity )
{
    if( !logistic_helpers::IsLogisticBase( entity ) )
        return;
    nominalCombo_->RemoveItem( &entity );
    currentCombo_->RemoveItem( &entity );
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksWidget::NotifyCreated
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
void ChangeLogisticLinksWidget::NotifyCreated( const kernel::Automat_ABC& automat )
{
    AddItem( automat );
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksWidget::NotifyDeleted
// Created: SBO 2006-06-307
// -----------------------------------------------------------------------------
void ChangeLogisticLinksWidget::NotifyDeleted( const kernel::Automat_ABC& automat )
{
    RemoveItem( automat );
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksWidget::NotifyCreated
// Created: AHC 2010-10-12
// -----------------------------------------------------------------------------
void ChangeLogisticLinksWidget::NotifyCreated( const kernel::Formation_ABC& formation )
{
    AddItem( formation );
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksWidget::NotifyDeleted
// Created: AHC 2010-10-12
// -----------------------------------------------------------------------------
void ChangeLogisticLinksWidget::NotifyDeleted( const kernel::Formation_ABC& formation )
{
    RemoveItem( formation );
}
