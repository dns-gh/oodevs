// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UsagesDockWidget.h"
#include "moc_UsagesDockWidget.cpp"

#include "clients_gui/RichSpinBox.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichWidget.h"
#include "clients_gui/RichTableWidget.h"
#include "clients_gui/Usages.h"
#include "clients_kernel/AccommodationType.h"
#include "clients_kernel/AccommodationTypes.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/PhysicalAttribute_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/Usages_ABC.h"
#include "preparation/StaticModel.h"
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget constructor
// Created: LGY 2011-01-13
// -----------------------------------------------------------------------------
UsagesDockWidget::UsagesDockWidget( QWidget* parent, kernel::Controllers& controllers, StaticModel& staticModel )
    : gui::RichDockWidget( controllers, parent, "usagesDockWidget", tr( "Usages" ) )
    , controllers_( controllers )
    , staticModel_( staticModel )
    , isEditing_  ( false )
{
    gui::SubObjectName subObject( this->objectName() );
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    pMotivations_ = new gui::RichWidget< QComboBox >( "motivations" );
    pButton_ = new gui::RichPushButton( "add", tr( "Add" ) );
    buttonLayout->addWidget( pMotivations_ );
    buttonLayout->addWidget( pButton_ );

    pTable_ = new gui::RichTableWidget( "usageDockTable" );
    pTable_->setColumnCount( 2 );
    QStringList labels;
    labels.append( tr( "Usages" ) );
    labels.append( tr( "Proportion(%)" ) );
    pTable_->setHorizontalHeaderLabels( labels );
    pTable_->setAlternatingRowColors( true );
    pTable_->setSelectionBehavior( QAbstractItemView::SelectRows );
    pTable_->horizontalHeader()->setResizeMode( 0, QHeaderView::Stretch );
    pTable_->horizontalHeader()->setResizeMode( 1, QHeaderView::ResizeToContents );

    QWidget* content = new QWidget( this );
    QVBoxLayout* hLayout = new QVBoxLayout( content);
    hLayout->addLayout( buttonLayout );
    hLayout->addWidget( pTable_ );
    setWidget( content );

    connect( pButton_, SIGNAL( clicked() ), this, SLOT( Add() ) );

    Clean();
    controllers_.Update( *this );

}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget destructor
// Created: LGY 2011-01-13
// -----------------------------------------------------------------------------
UsagesDockWidget::~UsagesDockWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget::NotifySelectionChanged
// Created: ABR 2012-05-25
// -----------------------------------------------------------------------------
void UsagesDockWidget::NotifySelectionChanged( const T_Elements& elements )
{
    Clean();
    selectedElements_ = elements;
    if( selectedElements_.size() == 1 && selectedElements_.front() )
    {
        setEnabled( true );
        Load();
        pMotivations_->show();
        pButton_->show();
        pTable_->show();
    }
    else
        setEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget::NotifyUpdated
// Created: ABR 2012-05-25
// -----------------------------------------------------------------------------
void UsagesDockWidget::NotifyUpdated( const kernel::UrbanObject_ABC& element )
{
    if( isEditing_ || element.IsUpdatingTemplate() )
        return;
    if( selectedElements_.size() == 1 && selectedElements_.front() == &element )
    {
        pMotivations_->hide();
        pButton_->hide();
        for( int i = pTable_->rowCount() - 1; i >= 0; --i )
            pTable_->removeRow( i );
        pTable_->hide();
        Load();
        pMotivations_->show();
        pButton_->show();
        pTable_->show();
    }
}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget::NotifyDeleted
// Created: ABR 2012-05-25
// -----------------------------------------------------------------------------
void UsagesDockWidget::NotifyDeleted( const kernel::UrbanObject_ABC& )
{
    Clean();
}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget::AddItem
// Created: LGY 2011-01-14
// -----------------------------------------------------------------------------
void UsagesDockWidget::AddItem( const std::string& name, int value )
{
    int row = pTable_->rowCount();
    pTable_->insertRow( row );
    QTableWidgetItem* pName = new QTableWidgetItem( name.c_str() );
    pName->setFlags( pName->flags() & ~Qt::ItemIsEditable );
    gui::RichSpinBox* pValue = new gui::RichSpinBox( name.c_str(), 0, 0, 100 );
    pValue->setValue( value );
    connect( pValue, SIGNAL( valueChanged( int ) ), this, SLOT( OnItemValueChanged() ) );
    pTable_->setItem( row, 0, pName );
    pTable_->setCellWidget( row, 1, pValue );
    Validate();
}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget::Validate
// Created: LGY 2011-02-17
// -----------------------------------------------------------------------------
void UsagesDockWidget::Validate()
{
    int count = pTable_->rowCount();
    for( int i = 0; i < count; ++i )
    {
        int total = 0;
        for( int j = 0; j < count; ++j )
            if( i != j )
            {
                gui::RichSpinBox* pValue = static_cast< gui::RichSpinBox* >( pTable_->cellWidget( j, 1 ) );
                if( pValue )
                    total += pValue->value();
            }
            gui::RichSpinBox* pValue = static_cast< gui::RichSpinBox* >( pTable_->cellWidget( i, 1 ) );
            if( pValue )
                pValue->setRange( 0, 100 - total );
    }
    if( selectedElements_.size() > 0 )
    {
        kernel::UrbanObject_ABC* object = const_cast< kernel::UrbanObject_ABC* >( selectedElements_.front() );
        if( object && !object->IsUpdatingTemplate() )
            object->UpdateTemplate( staticModel_.objectTypes_ );
    }
}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget::AddMotivation
// Created: LGY 2011-01-14
// -----------------------------------------------------------------------------
void UsagesDockWidget::AddMotivation( const std::string& role, int value )
{
    if( selectedElements_.size() != 1 )
        return;
    kernel::UrbanObject_ABC* urbanObject = const_cast< kernel::UrbanObject_ABC* >( selectedElements_.front() );
    if( !urbanObject )
        return;
    const kernel::PhysicalAttribute_ABC& pPhysical = urbanObject->Get< kernel::PhysicalAttribute_ABC >();
    kernel::Usages_ABC& usagesExtension = pPhysical.GetUsages();
    usagesExtension.Add( role, value );
    controllers_.controller_.Update( *urbanObject );
}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget::RemoveMotivation
// Created: LGY 2011-01-14
// -----------------------------------------------------------------------------
void UsagesDockWidget::RemoveMotivation( const std::string& role )
{
    if( selectedElements_.size() != 1 )
        return;
    kernel::UrbanObject_ABC* urbanObject = const_cast< kernel::UrbanObject_ABC* >( selectedElements_.front() );
    if( !urbanObject )
        return;
    const kernel::PhysicalAttribute_ABC& pPhysical = urbanObject->Get< kernel::PhysicalAttribute_ABC >();
    kernel::Usages_ABC& usagesExtension = pPhysical.GetUsages();
    usagesExtension.Remove( role );
    controllers_.controller_.Update( *urbanObject );
}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget::Load
// Created: LGY 2011-01-14
// -----------------------------------------------------------------------------
void UsagesDockWidget::Load()
{
    if( selectedElements_.size() != 1 )
        return;
    kernel::UrbanObject_ABC* urbanObject = const_cast< kernel::UrbanObject_ABC* >( selectedElements_.front() );
    if( !urbanObject )
        return;
    const kernel::PhysicalAttribute_ABC& pPhysical = urbanObject->Get< kernel::PhysicalAttribute_ABC >();
    const kernel::Usages_ABC& usagesExtension = pPhysical.GetUsages();
    const kernel::T_Usages& usages = usagesExtension.GetUsages();
    BOOST_FOREACH( const kernel::T_Usages::value_type& usage, usages )
        if( usage.first != gui::Usages::defaultStr_ )
            AddItem( usage.first, usage.second );
}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget::Clean
// Created: LGY 2011-01-13
// -----------------------------------------------------------------------------
void UsagesDockWidget::Clean()
{
    pMotivations_->hide();
    pButton_->hide();
    for( int i = pTable_->rowCount() - 1; i >= 0; --i )
        pTable_->removeRow( i );
    pTable_->hide();
}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget::Initialize
// Created: LGY 2011-01-13
// -----------------------------------------------------------------------------
void UsagesDockWidget::Initialize()
{
    pMotivations_->clear();
    tools::Iterator< const kernel::AccommodationType& > it = staticModel_.accommodationTypes_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const std::string& usage = it.NextElement().GetRole();
        if( usage != gui::Usages::defaultStr_ )
            pMotivations_->addItem( usage.c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget::Add
// Created: LGY 2011-01-13
// -----------------------------------------------------------------------------
void UsagesDockWidget::Add()
{
    if( selectedElements_.size() != 1 )
        return;
    const std::string motivation = pMotivations_->currentText().toStdString();
    QList< QTableWidgetItem* > pList = pTable_->findItems( motivation.c_str(), Qt::MatchExactly );
    if( pList.empty() )
    {
        AddItem( motivation, 0 );
        AddMotivation( motivation, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget::OnItemValueChanged
// Created: LGY 2011-01-13
// -----------------------------------------------------------------------------
void UsagesDockWidget::OnItemValueChanged()
{
    isEditing_ = true;
    for( int i = 0; i < pTable_->rowCount(); ++i )
    {
        QTableWidgetItem* pName = pTable_->item( i, 0 );
        gui::RichSpinBox* pValue = static_cast< gui::RichSpinBox* >( pTable_->cellWidget( i, 1 ) );
        if( pName && pValue )
            AddMotivation( pName->text().toStdString(), pValue->value() );
    }
    Validate();
    isEditing_ = false;
}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget::contextMenuEvent
// Created: LGY 2011-01-14
// -----------------------------------------------------------------------------
void UsagesDockWidget::contextMenuEvent( QContextMenuEvent* pEvent )
{
    QList< QTableWidgetItem* > pList = pTable_->selectedItems();
    if( selectedElements_.size() == 1 && !pList.isEmpty() )
    {
        QAction* pRemoveAction = new QAction( tr( "Delete" ), this );
        connect( pRemoveAction, SIGNAL( triggered() ), this, SLOT( Delete() ) );
        QMenu* pContextMenu = new QMenu( this );
        pContextMenu->addAction( pRemoveAction );
        pContextMenu->exec( pEvent->globalPos() );
    }
}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget::Delete
// Created: LGY 2011-01-14
// -----------------------------------------------------------------------------
void UsagesDockWidget::Delete()
{
    QList< QTableWidgetItem* > pList = pTable_->selectedItems();
    std::vector< int > deleteRow;
    BOOST_FOREACH( QTableWidgetItem* pItem, pList )
        if( pItem && pItem->column() == 0 )
        {
            RemoveMotivation( pItem->text().toStdString() );
        }
}
