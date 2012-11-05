// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Categories_AttritionEffect_Table.h"
#include "ADN_Equipement_GUI.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_Categories_AttritionEffect_Table::ADN_Categories_AttritionEffect_Table
// Created: ABR 2012-10-18
// -----------------------------------------------------------------------------
ADN_Categories_AttritionEffect_Table::ADN_Categories_AttritionEffect_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /*= 0*/ )
    : ADN_Table3( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 3 );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "State" )
                      << tr( "Wounded %" )
                      << tr( "Dead %" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    setMaximumHeight( 300 );

    delegate_.AddColorOnColumn( 1, 0., 100. );
    delegate_.AddColorOnColumn( 2, 0., 100. );
    delegate_.AddSpinBoxOnColumn( 1, 0, 100 );
    delegate_.AddSpinBoxOnColumn( 2, 0, 100 );
    std::vector< int > linkedList;
    linkedList.push_back( 1 );
    linkedList.push_back( 2 );
    delegate_.SetColumnsSumRestriction( linkedList, gui::CommonDelegate::eLTE, 100 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_AttritionEffect_Table::~ADN_Categories_AttritionEffect_Table
// Created: ABR 2012-10-18
// -----------------------------------------------------------------------------
ADN_Categories_AttritionEffect_Table::~ADN_Categories_AttritionEffect_Table()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_AttritionEffect_Table::AddRow
// Created: ABR 2012-10-19
// -----------------------------------------------------------------------------
void ADN_Categories_AttritionEffect_Table::AddRow( int row, void* data )
{
    helpers::AttritionEffectOnHuman* pAttrition = static_cast< helpers::AttritionEffectOnHuman* >( data );
    if( !pAttrition )
        return;

    AddItem( row, 0, data, ADN_Tr::ConvertFromEquipmentState_ADN( pAttrition->nEquipmentState_.GetData(), ENT_Tr_ABC::eToTr ).c_str() );
    AddItem( row, 1, data, &pAttrition->nInjuredPercentage_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    AddItem( row, 2, data, &pAttrition->nDeadPercentage_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_AttritionEffect_Table::dataChanged
// Created: ABR 2012-10-23
// -----------------------------------------------------------------------------
void ADN_Categories_AttritionEffect_Table::dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
{
    ADN_Table3::dataChanged( topLeft, bottomRight );
    if( topLeft == bottomRight )
        ADN_Workspace::GetWorkspace().GetEquipements().GetGui().UpdateGraph();
}
