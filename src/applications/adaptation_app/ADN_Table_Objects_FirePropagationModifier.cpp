// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Table_Objects_FirePropagationModifier.h"
#include "ADN_Objects_Data.h"

typedef ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::ModifierByFireClass ModifierByFireClass;

//-----------------------------------------------------------------------------
// Name: ADN_Table_Objects_FirePropagationModifier constructor
// Created: BCI 2010-12-02
//-----------------------------------------------------------------------------
ADN_Table_Objects_FirePropagationModifier::ADN_Table_Objects_FirePropagationModifier( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 3 );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Fire model" )
                      << tr( "Ignition threshold" )
                      << tr( "Max combustion energy" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    delegate_.AddSpinBoxOnColumn( 1, 0, INT_MAX );
    delegate_.AddSpinBoxOnColumn( 2, 0, INT_MAX );
    setMinimumHeight( 120 );
}

//-----------------------------------------------------------------------------
// Name: ADN_Table_Objects_FirePropagationModifier destructor
// Created: BCI 2010-12-02
//-----------------------------------------------------------------------------
ADN_Table_Objects_FirePropagationModifier::~ADN_Table_Objects_FirePropagationModifier()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_FireEffectRoe_GUI::AddRow
// Created: MMC 2012-11-06
// -----------------------------------------------------------------------------
void ADN_Table_Objects_FirePropagationModifier::AddRow( int row, void* data )
{
    ModifierByFireClass* pModifier = static_cast< ModifierByFireClass* >( data );
    if( !pModifier || !pModifier->GetCrossedElement() || !pModifier->GetCrossedElement()->isSurface_.GetData())
        return;
    AddItem( row, 0, data, &pModifier->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pModifier->ignitionThreshold_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    AddItem( row, 2, data, &pModifier->maxCombustionEnergy_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
}
