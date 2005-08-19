// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_WeatherPanel.cpp $
// $Author: Ape $
// $Modtime: 23/09/04 17:33 $
// $Revision: 5 $
// $Workfile: MOS_WeatherPanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_WeatherPanel.h"
#include "moc_MOS_WeatherPanel.cpp"

#include "MOS_App.h"
#include "MOS_Tools.h"
#include "MOS_Meteo_ABC.h"
#include "MOS_Meteo_Local.h"
#include "MOS_Meteo_Manager.h"
#include "MOS_ShapeEditorMapEventFilter.h"
#include "MOS_World.h"

#ifndef MOS_USE_INLINE
#   include "MOS_WeatherPanel.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_WeatherPanel constructor
/** @param  pParent 
*/
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
MOS_WeatherPanel::MOS_WeatherPanel( QWidget* pParent )
    : QVBox         ( pParent )
    , pLineEditor_  ( new MOS_ShapeEditorMapEventFilter( this ) )
{
    this->setSpacing( 5 );

    pWeatherListView_ = new QListView( this );
    pWeatherListView_->addColumn( tr( "Météo" ) );
    pWeatherListView_->setResizeMode( QListView::LastColumn );

    QGroupBox* pGroupBox = new QGroupBox( 2, Qt::Horizontal, tr( "Météo sélectionnée" ), this );

    new QLabel( tr( "Temps:" ), pGroupBox );
    pWeatherTypeCombo_ = new MT_ValuedComboBox<MOS_Meteo_ABC::E_WeatherType>( pGroupBox );
    for( int n = 0; n < (int)MOS_Meteo_ABC::eWeatherNbr; ++n )
    {
        MOS_Meteo_ABC::E_WeatherType nValue = (MOS_Meteo_ABC::E_WeatherType)n;
        pWeatherTypeCombo_->AddItem( MOS_Tools::ToString( nValue ), nValue );
    }

    new QLabel( tr( "Température:" ), pGroupBox );
    pTemperatureSpinBox_ = new QSpinBox( pGroupBox );
    pTemperatureSpinBox_->setValue( 21 );

    new QLabel( tr( "Vitesse du vent:" ), pGroupBox );
    pWindSpeedSpinBox_ = new QSpinBox( 0, 250, 1, pGroupBox );

    new QLabel( tr( "Direction:" ), pGroupBox );
    pWindDirectionDial_ = new QDial( 0, 359, 1, 0, pGroupBox );
    pWindDirectionDial_->setWrapping( true );
    pWindDirectionDial_->setMaximumSize( 50, 50 );
    
    new QLabel( tr( "Couverture nuageuse (sol):" ), pGroupBox );
    pCloudCoverFloorSpinBox_ = new QSpinBox( pGroupBox );

    new QLabel( tr( "Couverture nuageuse (plafond):" ), pGroupBox );
    pCloudCoverCeilingSpinBox_ = new QSpinBox( pGroupBox );

    new QLabel( tr( "Couverture nuageuse (densité):" ), pGroupBox );
    pCloudDensitySpinBox_ = new QSpinBox( pGroupBox );

    QHBox* pBox = new QHBox( pGroupBox );
    new QLabel( tr( "Position:" ), pBox );
    pLocationButton_ = new QPushButton( "Fixer", pBox );
    pLocationValue1_ = new QLabel( "---", pGroupBox );
    pLocationValue1_->setAlignment( Qt::AlignCenter );
    pLocationValue1_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    pLocationValue1_->setMinimumWidth( 100 );
    pGroupBox->addSpace( 0 );
    pLocationValue2_ = new QLabel( "---", pGroupBox );
    pLocationValue2_->setAlignment( Qt::AlignCenter );
    pLocationValue2_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    pLocationValue2_->setMinimumWidth( 100 );

    pOkButton_ = new QPushButton( tr( "Valider" ), pGroupBox );
    pCancelButton_ = new QPushButton( tr( "Annuler" ), pGroupBox );

    pPopupMenu_ = new QPopupMenu( this );
    pPopupMenu_->insertItem( tr( "Effacer" ), this, SLOT( OnDelete() ), 0, 1 );
    pPopupMenu_->insertItem( tr( "Créer météo locale" ), this, SLOT( OnCreate() ), 0, 2 );

    connect( pWeatherListView_, SIGNAL( selectionChanged( QListViewItem* ) ),                         this, SLOT( OnSelectionChanged( QListViewItem* ) ) );
    connect( pWeatherListView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );
    connect( pLocationButton_,  SIGNAL( clicked() ), this, SLOT( SetLocation() ) );
    connect( pLineEditor_,      SIGNAL( Done() ), this, SLOT( LocationDone() ) );
    connect( pOkButton_,     SIGNAL( clicked() ), this, SLOT( OnOk() ) );
    connect( pCancelButton_, SIGNAL( clicked() ), this, SLOT( OnCancel() ) );

    connect( pWeatherTypeCombo_,         SIGNAL( activated( int ) ),    this, SLOT( OnDataChanged() ) );
    connect( pTemperatureSpinBox_,       SIGNAL( valueChanged( int ) ), this, SLOT( OnDataChanged() ) );
    connect( pWindSpeedSpinBox_,         SIGNAL( valueChanged( int ) ), this, SLOT( OnDataChanged() ) );
    connect( pWindDirectionDial_,        SIGNAL( valueChanged( int ) ), this, SLOT( OnDataChanged() ) );
    connect( pCloudCoverFloorSpinBox_,   SIGNAL( valueChanged( int ) ), this, SLOT( OnDataChanged() ) );
    connect( pCloudCoverCeilingSpinBox_, SIGNAL( valueChanged( int ) ), this, SLOT( OnDataChanged() ) );
    connect( pCloudDensitySpinBox_,      SIGNAL( valueChanged( int ) ), this, SLOT( OnDataChanged() ) );

    this->Init();
}


// -----------------------------------------------------------------------------
// Name: MOS_WeatherPanel destructor
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
MOS_WeatherPanel::~MOS_WeatherPanel()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_WeatherPanel::Init
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
void MOS_WeatherPanel::Init()
{
    MOS_Meteo_Manager& manager = MOS_App::GetApp().GetWeatherManager();
    QListViewItem* pGlobalItem = new MT_ValuedListViewItem< MOS_Meteo_ABC*, eGlobal>( & manager.GetGlobalWeather(), pWeatherListView_, tr( "Météo globale" ) );

    MOS_Meteo_Manager::T_Meteo_Vector& localWeatherList = manager.GetLocalWeatherList();
    for( MOS_Meteo_Manager::IT_Meteo_Vector it = localWeatherList.begin(); it != localWeatherList.end(); ++it )
        new MT_ValuedListViewItem< MOS_Meteo_Local*, eLocal>( *it, pWeatherListView_, tr( "Météo locale" ) );

    pWeatherListView_->setSelected( pGlobalItem, true );
}


// -----------------------------------------------------------------------------
// Name: MOS_WeatherPanel::OnSelectionChanged
/** @param  pItem 
*/
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
void MOS_WeatherPanel::OnSelectionChanged( QListViewItem* pItem )
{
    pLocationButton_->setEnabled( false );
    pLocationValue1_->setText( "---" );
    pLocationValue2_->setText( "---" );
    pOkButton_->setEnabled( false );
    pCancelButton_->setEnabled( false );
    pLineEditor_->Pop();

    // Normally, something should always be selected, but who knows...
    if( pItem == 0 )
        return;

    MOS_Meteo_ABC* pWeather = ((MT_ValuedListViewItem< MOS_Meteo_ABC*, eGlobal>*)pItem)->GetValue();

    pWeatherTypeCombo_->setCurrentItem( (int)pWeather->GetWeatherType() );
    pTemperatureSpinBox_->setValue( pWeather->GetTemperature() );
    pWindSpeedSpinBox_->setValue( pWeather->GetWindSpeed() );
    int nWindDir = pWeather->GetWindDirection();
    nWindDir += (nWindDir > 180 ) ? -180 : 180;
    pWindDirectionDial_->setValue( nWindDir );
    pCloudCoverFloorSpinBox_->setValue( pWeather->GetCloudCoverFloor() );
    pCloudCoverCeilingSpinBox_->setValue( pWeather->GetCloudCoverCeiling() );
    pCloudDensitySpinBox_->setValue( pWeather->GetCloudDensity() );

    if( pItem->rtti() == eLocal )
    {
        MOS_Meteo_Local* pLocalWeather = ((MT_ValuedListViewItem< MOS_Meteo_Local*, eLocal>*)pItem)->GetValue();
        pLocationButton_->setEnabled( true );

        point1_ = pLocalWeather->GetUpperLeftCorner();
        point2_ = pLocalWeather->GetLowerRightCorner();
        
        std::string strPos;
        MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( point1_, strPos );
        pLocationValue1_->setText( strPos.c_str() );
        MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( point2_, strPos );
        pLocationValue2_->setText( strPos.c_str() );
    }

    // Set it again because of the signals emited when we changed the values.
    pOkButton_->setEnabled( false );
    pCancelButton_->setEnabled( false );
}


// -----------------------------------------------------------------------------
// Name: MOS_WeatherPanel::OnRequestPopup
/** @param  pItem 
    @param  pos 
*/
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
void MOS_WeatherPanel::OnRequestPopup( QListViewItem* pItem, const QPoint& pos )
{
    pPopupMenu_->setItemEnabled( 1, pItem != 0 && pItem->rtti() != eGlobal );
    pPopupMenu_->popup( pos );
}


// -----------------------------------------------------------------------------
// Name: MOS_WeatherPanel::OnDelete
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
void MOS_WeatherPanel::OnDelete()
{
    QListViewItem* pItem = pWeatherListView_->selectedItem();
    assert( pItem != 0 &&  pItem->rtti() != eGlobal );

    // Avoid staying in position edition mode if we delete a
    // weather (it could be the one we're editing).
    pLineEditor_->Pop();

    MT_ValuedListViewItem< MOS_Meteo_Local*, eLocal>* pCastItem = (MT_ValuedListViewItem< MOS_Meteo_Local*, eLocal>*)pItem;
    pCastItem->GetValue()->SendDeleteMsg();
    MOS_App::GetApp().GetWeatherManager().DeleteLocalWeather( *(pCastItem->GetValue()) );
    delete pCastItem;

    pWeatherListView_->setSelected( pWeatherListView_->firstChild(), true );
}


// -----------------------------------------------------------------------------
// Name: MOS_WeatherPanel::OnCreate
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
void MOS_WeatherPanel::OnCreate()
{
    MOS_Meteo_Local& meteo = MOS_App::GetApp().GetWeatherManager().CreateLocalWeather();
    QListViewItem* pItem = new MT_ValuedListViewItem< MOS_Meteo_Local*, eLocal>( &meteo, pWeatherListView_, tr( "Météo locale" ) );
    meteo.SendUpdateMsg();
    pWeatherListView_->setSelected( pItem, true );
}


// -----------------------------------------------------------------------------
// Name: MOS_WeatherPanel::SetLocation
// Created: APE 2004-06-03
// -----------------------------------------------------------------------------
void MOS_WeatherPanel::SetLocation()
{
    pLineEditor_->PrepareNewLine( MOS_ShapeEditorMapEventFilter::eRectangle );
    pLineEditor_->Push();
}


// -----------------------------------------------------------------------------
// Name: MOS_WeatherPanel::LocationDone
// Created: APE 2004-06-03
// -----------------------------------------------------------------------------
void MOS_WeatherPanel::LocationDone()
{
    T_PointVector& pointList = pLineEditor_->GetPointList();
    if( pointList.empty() )
        return;

    point1_ = pointList[0];
    point2_ = pointList[1];
    
    std::string strPos;
    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( point1_, strPos );
    pLocationValue1_->setText( strPos.c_str() );
    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( point2_, strPos );
    pLocationValue2_->setText( strPos.c_str() );

    pOkButton_->setEnabled( true );
    pCancelButton_->setEnabled( true );
}


// -----------------------------------------------------------------------------
// Name: MOS_WeatherPanel::OnDataChanged
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
void MOS_WeatherPanel::OnDataChanged()
{
    pOkButton_->setEnabled( true );
    pCancelButton_->setEnabled( true );
}


// -----------------------------------------------------------------------------
// Name: MOS_WeatherPanel::OnOk
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
void MOS_WeatherPanel::OnOk()
{
    QListViewItem* pItem = pWeatherListView_->selectedItem();
    if( pItem == 0 )
        return;

    MOS_Meteo_ABC* pWeather = ((MT_ValuedListViewItem< MOS_Meteo_ABC*, eGlobal>*)pItem)->GetValue();
    pWeather->SetWeatherType( pWeatherTypeCombo_->GetValue() );
    pWeather->SetTemperature( pTemperatureSpinBox_->value() );
    pWeather->SetWindSpeed( pWindSpeedSpinBox_->value() );
    int nWindDir = pWindDirectionDial_->value();
    nWindDir += (nWindDir > 180 ) ? -180 : 180;
    pWeather->SetWindDirection( nWindDir );
    pWeather->SetCloudCoverFloor( pCloudCoverFloorSpinBox_->value() );
    pWeather->SetCloudCoverCeiling( pCloudCoverCeilingSpinBox_->value() );
    pWeather->SetCloudDensity( pCloudDensitySpinBox_->value() );

    if( pItem->rtti() == eLocal )
    {
        MOS_Meteo_Local* pLocalWeather = ((MT_ValuedListViewItem< MOS_Meteo_Local*, eLocal>*)pItem)->GetValue();
        pLocalWeather->SetCorners( point1_, point2_ );
    }

    pWeather->SendUpdateMsg();
    pOkButton_->setEnabled( false );
    pCancelButton_->setEnabled( false );
}


// -----------------------------------------------------------------------------
// Name: MOS_WeatherPanel::OnCancel
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
void MOS_WeatherPanel::OnCancel()
{
    this->OnSelectionChanged( pWeatherListView_->selectedItem() );
}


