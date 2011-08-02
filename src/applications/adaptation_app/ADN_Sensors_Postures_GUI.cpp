// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Sensors_Postures_GUI.h"
#include "moc_ADN_Sensors_Postures_GUI.cpp"
#include "ADN_App.h"
#include "ADN_CommonGfx.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Workspace.h"
#include "ENT/ENT_Tr.h"

typedef ADN_Sensors_Data::SensorInfos                SensorInfos;
typedef ADN_Sensors_Data::ModificatorPostureInfos    ModificatorPostureInfos;

//-----------------------------------------------------------------------------
// Internal Table connector for ADN_Sensors_Postures_GUI
//-----------------------------------------------------------------------------
class ADN_CT_Sensors_Postures : public ADN_Connector_Table_ABC
{

public:
    explicit ADN_CT_Sensors_Postures( ADN_Sensors_Postures_GUI& tab )
        : ADN_Connector_Table_ABC( tab, false )
    {}

    void AddSubItems( int i, void* pObj )
    {
        assert( pObj != 0 );
        ADN_TableItem_String* pItemString = new ADN_TableItem_String( &tab_, pObj );
        ADN_TableItem_Double* pItemDouble = new ADN_TableItem_Double( &tab_, pObj );

        // Add a new row & set the new values.
        tab_.setItem( i, 0, pItemString );
        tab_.setItem( i, 1, pItemDouble );

        // Setup the row header item.
        pItemString->setEnabled( false );
        pItemString->setText( ENT_Tr::ConvertFromUnitPosture( static_cast<ModificatorPostureInfos*>(pObj)->eType_, ENT_Tr_ABC::eToTr ).c_str() );

        // Setup the value item.
        pItemDouble->GetValidator().setRange( 0, 1, 2 );
        pItemDouble->GetConnector().Connect( &static_cast<ModificatorPostureInfos*>(pObj)->rCoeff_ );
    }
};

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Postures_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Sensors_Postures_GUI::ADN_Sensors_Postures_GUI( const QString& strColCaption, QWidget* pParent )
    : ADN_Table2( pParent, "ADN_Sensors_Postures_GUI" )
{
    // Setup the table properties.
    setSelectionMode( Q3Table::SingleRow );
    setSorting( true );
    setShowGrid( false );
    setLeftMargin( 0 );
    setMinimumHeight( int( ( eNbrUnitPosture + 1.2 ) * 20 ) );

    // Setup the columns and headers
    setNumCols( 2 );
    setNumRows( 0 );
    setColumnStretchable( 0, true );
    setColumnStretchable( 1, true );

    verticalHeader()->hide();
    horizontalHeader()->setLabel( 0, strColCaption );
    horizontalHeader()->setLabel( 1, tr( "Modifiers" ) );

    // Create the connector.
    pConnector_ = new ADN_CT_Sensors_Postures( *this );

    connect( this, SIGNAL( currentChanged( int, int ) ), SLOT( OnCurrentChanged() ) );
    connect( this, SIGNAL( selectionChanged() ), SLOT( OnCurrentChanged() ) );

}


//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Postures_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Sensors_Postures_GUI::~ADN_Sensors_Postures_GUI()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Postures_GUI::OnCurrentChanged
// Created: HBD 2010-04-29
// -----------------------------------------------------------------------------
void ADN_Sensors_Postures_GUI::OnCurrentChanged()
{
    if( ModificatorPostureInfos* data = static_cast< ModificatorPostureInfos* >( GetCurrentData() ) )
        emit PostureChanged( data->GetItemName(), data->rCoeff_.GetData() );
}
