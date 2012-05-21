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
#include "clients_kernel/Controllers.h"
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget constructor
// Created: LGY 2011-01-13
// -----------------------------------------------------------------------------
UsagesDockWidget::UsagesDockWidget( QWidget* parent, kernel::Controllers& controllers, StaticModel& staticModel )
    : gui::RichDockWidget( controllers, parent, "usagesDockWidget", tr( "Usages" ), false )
    , controllers_( controllers )
    , staticModel_( staticModel )
    , isEditing_  ( false )
{

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    pMotivations_ = new QComboBox();
    pButton_ = new QPushButton( tr( "Add" ) );
    buttonLayout->addWidget( pMotivations_ );
    buttonLayout->addWidget( pButton_ );

    pTable_ = new QTableWidget();
    pTable_->setColumnCount( 2 );
    QStringList labels;
    labels.append( tr( "Usages" ) );
    labels.append( tr( "Proportion(%)" ) );
    pTable_->setHorizontalHeaderLabels( labels );
    pTable_->setAlternatingRowColors( true );
    pTable_->setSelectionBehavior( QAbstractItemView::SelectRows );

    QWidget* content = new QWidget( this );
    QVBoxLayout* hLayout = new QVBoxLayout( content);
    hLayout->addLayout( buttonLayout );
    hLayout->addWidget( pTable_ );
    setWidget( content );

    connect( pButton_, SIGNAL( clicked() ), this, SLOT( Add() ) );

    Clean();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget destructor
// Created: LGY 2011-01-13
// -----------------------------------------------------------------------------
UsagesDockWidget::~UsagesDockWidget()
{
    controllers_.Unregister( *this );
}

//// -----------------------------------------------------------------------------
//// Name: UsagesDockWidget::SelectionChanged
//// Created: JSR 2012-03-14
//// -----------------------------------------------------------------------------
//void UsagesDockWidget::SelectionChanged()
//{
//    Clean();
//    const T_TerrainObjects& selected = controller_.GetSelected();
//    if( selected.size() == 1 && selected.front() )
//    {
//        setEnabled( true );
//        Load();
//        pMotivations_->show();
//        pButton_->show();
//        pTable_->show();
//    }
//    else
//        setEnabled( false );
//}
//
//// -----------------------------------------------------------------------------
//// Name: UsagesDockWidget::NotifyUpdated
//// Created: LGY 2011-09-26
//// -----------------------------------------------------------------------------
//void UsagesDockWidget::NotifyUpdated( const urban::TerrainObject_ABC& element )
//{
//    if( isEditing_ )
//        return;
//    const T_TerrainObjects& selected = controller_.GetSelected();
//    if( selected.size() == 1 && selected.front() == &element )
//    {
//        pMotivations_->hide();
//        pButton_->hide();
//        for( int i = pTable_->rowCount() - 1; i >= 0; --i )
//            pTable_->removeRow( i );
//        pTable_->hide();
//        Load();
//        pMotivations_->show();
//        pButton_->show();
//        pTable_->show();
//    }
//}
//
//// -----------------------------------------------------------------------------
//// Name: UsagesDockWidget::NotifyDeleted
//// Created: LGY 2011-01-13
//// -----------------------------------------------------------------------------
//void UsagesDockWidget::NotifyDeleted( const urban::TerrainObject_ABC& /*element*/ )
//{
//    Clean();
//}

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
    QSpinBox* pValue = new gui::RichSpinBox( 0, 0, 100 );
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
                QSpinBox* pValue = static_cast< QSpinBox* >( pTable_->cellWidget( j, 1 ) );
                if( pValue )
                    total += pValue->value();
            }
            QSpinBox* pValue = static_cast< QSpinBox* >( pTable_->cellWidget( i, 1 ) );
            if( pValue )
                pValue->setRange( 0, 100 - total );
    }
}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget::AddMotivation
// Created: LGY 2011-01-14
// -----------------------------------------------------------------------------
void UsagesDockWidget::AddMotivation( const std::string& /*motivation*/, int /*value*/ )
{
    //const T_TerrainObjects& selected = controller_.GetSelected();
    //if( selected.size() != 1 )
    //    return;
    //TerrainObject_ABC* object = const_cast< TerrainObject_ABC* >( selected.front() );
    //PhysicalAttribute* pPhysical = object->Retrieve< PhysicalAttribute >();
    //if( !pPhysical )
    //{
    //    pPhysical = new PhysicalAttribute( *object );
    //    pPhysical->CreateMotivations();
    //    object->Attach( *pPhysical );
    //}
    //if( !pPhysical->GetMotivations() )
    //    pPhysical->CreateMotivations();
    //pPhysical->GetMotivations()->Add( motivation, ( value / 100.f ) );
    //controller_.Update( *object );
}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget::RemoveMotivation
// Created: LGY 2011-01-14
// -----------------------------------------------------------------------------
void UsagesDockWidget::RemoveMotivation( const std::string& /*name*/ )
{
    //const T_TerrainObjects& selected = controller_.GetSelected();
    //if( selected.size() != 1 )
    //    return;
    //TerrainObject_ABC* object = const_cast< TerrainObject_ABC* >( selected.front() );
    //PhysicalAttribute* pPhysical = object->Retrieve< PhysicalAttribute >();
    //if( pPhysical && pPhysical->GetMotivations() )
    //    pPhysical->GetMotivations()->Remove( name );
}

//namespace
//{
//    class MotivationsVisitor : public MotivationsVisitor_ABC
//    {
//    public:
//        explicit MotivationsVisitor( std::map< std::string, float >& motivations )
//            : motivations_( motivations )
//        {
//            // NOTHING
//        }
//        virtual void Visit( const std::string& motivation, float proportion )
//        {
//            motivations_[ motivation ] = proportion;
//        }
//        std::map< std::string, float >& motivations_;
//    };
//}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget::Load
// Created: LGY 2011-01-14
// -----------------------------------------------------------------------------
void UsagesDockWidget::Load()
{
    //const T_TerrainObjects& selected = controller_.GetSelected();
    //if( selected.size() != 1 )
    //    return;
    //TerrainObject_ABC* object = const_cast< TerrainObject_ABC* >( selected.front() );
    //T_Motivations motivations;
    //MotivationsVisitor visitor( motivations );
    //object->Accept( visitor );
    //BOOST_FOREACH( const T_Motivations::value_type& motivation, motivations )
    //    AddItem( motivation.first, static_cast< int >( motivation.second * 100 + 0.01 ) );
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
    //pMotivations_->clear();
    //tools::Iterator< const MotivationType& > it = model_.CreateIterator< MotivationType >();
    //while( it.HasMoreElements() )
    //{
    //    const std::string& usage = it.NextElement().GetName();
    //    if( usage != "default" )
    //        pMotivations_->addItem( usage.c_str() );
    //}
}

// -----------------------------------------------------------------------------
// Name: UsagesDockWidget::Add
// Created: LGY 2011-01-13
// -----------------------------------------------------------------------------
void UsagesDockWidget::Add()
{
    //if( controller_.GetSelected().size() != 1 )
    //    return;
    //const std::string motivation = pMotivations_->currentText().toStdString();
    //QList< QTableWidgetItem* > pList = pTable_->findItems( motivation.c_str(), Qt::MatchExactly );
    //if( pList.empty() )
    //{
    //    AddItem( motivation, 0 );
    //    AddMotivation( motivation, 0 );
    //}
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
        QSpinBox* pValue = static_cast< QSpinBox* >( pTable_->cellWidget( i, 1 ) );
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
void UsagesDockWidget::contextMenuEvent( QContextMenuEvent* /*pEvent*/ )
{
    //QList< QTableWidgetItem* > pList = pTable_->selectedItems();
    //if( controller_.GetSelected().size() == 1 && !pList.isEmpty() )
    //{
    //    QAction* pRemoveAction = new QAction( tr( "Delete" ), this );
    //    connect( pRemoveAction, SIGNAL( triggered() ), this, SLOT( Delete() ) );
    //    QMenu* pContextMenu = new QMenu( this );
    //    pContextMenu->addAction( pRemoveAction );
    //    pContextMenu->exec( pEvent->globalPos() );
    //}
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
            deleteRow.push_back( pItem->row() );
        }
        BOOST_FOREACH( int row, deleteRow )
            pTable_->removeRow( row );
}
