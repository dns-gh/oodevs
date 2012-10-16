// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Composantes_Speeds_GUI.h"
#include "moc_ADN_Composantes_Speeds_GUI.cpp"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Composantes_Data.h"

typedef ADN_Composantes_Data::SpeedInfos SpeedInfos;
typedef ADN_Composantes_Data::ComposanteInfos ComposanteInfos;

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with ADN_Composantes_Speeds_GUI
//-----------------------------------------------------------------------------
class ADN_CT_Composantes_Speeds
    :public ADN_Connector_Table_ABC
{
public:
    ADN_CT_Composantes_Speeds( ADN_Composantes_Speeds_GUI& tab )
        : ADN_Connector_Table_ABC( tab, false )
    {
        // NOTHING
    }

    void AddSubItems( int i, void *obj )
    {
        assert( obj );
        ADN_TableItem_String* pItemString = new ADN_TableItem_String( &tab_, obj );
        ADN_TableItem_Double* pItemSpeed = new ADN_TableItem_Double( &tab_, obj );
        ADN_TableItem_IntPercentage* pItemConstruction = new ADN_TableItem_IntPercentage( &tab_, obj );

        tab_.setItem( i, 0, pItemString );
        tab_.setItem( i, 1, pItemSpeed );
        tab_.setItem( i, 2, pItemConstruction );

        SpeedInfos* infos = static_cast< SpeedInfos* >( obj );
        pItemString->setEnabled( false );
        pItemString->setText( ADN_Tr::ConvertFromLocation( infos->nTypeTerrain_, ENT_Tr_ABC::eToTr ).c_str() );

        pItemSpeed->GetValidator().setBottom( 0 );

        pItemSpeed->GetConnector().Connect( &infos->rSpeed_ );
        pItemConstruction->GetConnector().Connect( &infos->nConstruction_ );
        tab_.AdjustColumns( 10 );
    }

private:
    ADN_CT_Composantes_Speeds& operator=( const ADN_CT_Composantes_Speeds& );
};

//-----------------------------------------------------------------------------
// Name: ADN_Composantes_Speeds_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Composantes_Speeds_GUI::ADN_Composantes_Speeds_GUI( QLineEdit* maxSpeed, QWidget* parent )
    : ADN_Table2( parent, "ADN_Composantes_Speeds_GUI" )
    , maxSpeed_( maxSpeed )
    , popupIsDisplayed_( false )
{
    setSorting( true );
    setSelectionMode( Q3Table::Single );
    setShowGrid( false );
    setLeftMargin( 0 );
    verticalHeader()->hide();
    setNumCols( 3 );
    setNumRows( 0 );
    horizontalHeader()->setLabel( 0, tr( "Ground type" ) );
    horizontalHeader()->setLabel( 1, tr( "Speed (km/h)" ) );
    horizontalHeader()->setLabel( 2, tr( "Construction (%)" ) );
    pConnector_ = new ADN_CT_Composantes_Speeds( *this );
    connect( maxSpeed, SIGNAL( editingFinished() ), SLOT( OnMaxSpeedFinishedEditing() ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_Composantes_Speeds_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Composantes_Speeds_GUI::~ADN_Composantes_Speeds_GUI()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Speeds_GUI::UpdateSpeedsValidator
// Created: ABR 2012-04-17
// -----------------------------------------------------------------------------
bool ADN_Composantes_Speeds_GUI::UpdateSpeedsValidator( double maxSpeed )
{
    bool needReBound = false;
    for( int row = 0; row < numRows(); ++row )
    {
        ADN_TableItem_Double* item = static_cast< ADN_TableItem_Double* >( this->item( row, 1 ) );
        item->GetValidator().setTop( maxSpeed );
        bool ok = false;
        double value = locale().toDouble( item->text(), &ok );
        if( !needReBound && ok && value > maxSpeed )
            needReBound = true;
    }
    return needReBound;
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Speeds_GUI::OnItemSelected
// Created: ABR 2012-04-17
// -----------------------------------------------------------------------------
void ADN_Composantes_Speeds_GUI::OnItemSelected( void* pData )
{
    if( pData == 0 )
        return;
    ComposanteInfos* infos = static_cast< ComposanteInfos* >( pData );
    oldMaxSpeed_ = infos->rMaxSpeed_.GetData();
    UpdateSpeedsValidator( oldMaxSpeed_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Speeds_GUI::OnMaxSpeedFinishedEditing
// Created: ABR 2012-04-16
// -----------------------------------------------------------------------------
void ADN_Composantes_Speeds_GUI::OnMaxSpeedFinishedEditing()
{
    if( popupIsDisplayed_ )
        return;
    bool ok = false;
    double maxSpeed = locale().toDouble( maxSpeed_->text(), &ok );
    if( ok && UpdateSpeedsValidator( maxSpeed ) )
    {
        popupIsDisplayed_ = true;
        int nResult = QMessageBox::warning( this,
            tr( "Warning" ),
            tr( "At least one speed is above the new max speed.\nClick \"Ok\" to bound it to the new max speed value." ),
            QMessageBox::Ok     | QMessageBox::Default,
            QMessageBox::Cancel | QMessageBox::Escape );
        popupIsDisplayed_ = false;

        if( nResult == QMessageBox::Ok )
        {
            for( int row = 0; row < numRows(); ++row )
            {
                ADN_TableItem_Double* item = static_cast< ADN_TableItem_Double* >( this->item( row, 1 ) );
                SpeedInfos* infos = static_cast< SpeedInfos* >( item->GetData() );
                if( infos->rSpeed_.GetData() > maxSpeed )
                    infos->rSpeed_ = maxSpeed;
            }
        }
        else
        {
            maxSpeed_->setText( locale().toString( oldMaxSpeed_ ) );
            maxSpeed = oldMaxSpeed_;
        }
    }
    oldMaxSpeed_ = maxSpeed;
}
