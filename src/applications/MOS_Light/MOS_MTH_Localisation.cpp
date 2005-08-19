//*****************************************************************************
//
// $Created: FBD 03-01-13 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_Localisation.cpp $
// $Author: Nld $
// $Modtime: 21/01/05 18:21 $
// $Revision: 10 $
// $Workfile: MOS_MTH_Localisation.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_MTH_Localisation.h"

#ifndef MOS_USE_INLINE
#	include "MOS_MTH_Localisation.inl"
#endif

#include "MOS_Attr_Def.h"
#include "MOS_Net_Def.h"

#include "MOS_PointListViewItem.h"
#include "MOS_Agent.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_Mission_ABC.h"
#include "MOS_MTH_Localisation_ListView.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_MTH_Localisation constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_MTH_Localisation::MOS_MTH_Localisation( ASN1T_Localisation& asnLocalisation, QWidget* pParent, const std::string& strName )
    :   QWidget( pParent )
    ,   MOS_MissionTool_ABC()
    ,   pAsnLocalisation_ ( &asnLocalisation )
    ,   vOldPos_         ( 0., 0.)
    ,   pAgent_          ( 0 )
    ,   bMovingLine_     ( false )
    ,   bFirstWidget_    ( false )
	,  pMission_ ( 0 )
	, pWidgetTab_( 0 )
{
    QVBoxLayout* pLocalLayout = new QVBoxLayout( this );

    // Title
    QLabel* pLabel = CreateTitle( this, strName );
    pLocalLayout->addWidget( pLabel );

    // Points
    pListView_ = new MOS_MTH_Localisation_ListView( asnLocalisation, this, strName );
    pLocalLayout->addWidget( pListView_ );
    
    // Type
    pTypeComboBox_ = new QComboBox( this, "test" );   
    pTypeComboBox_->insertItem( "Circle"  , EnumTypeLocalisation::circle );
    pTypeComboBox_->insertItem( "Ellipse" , EnumTypeLocalisation::ellipse  );
    pTypeComboBox_->insertItem( "Ligne"   , EnumTypeLocalisation::line );
    pTypeComboBox_->insertItem( "Polygone", EnumTypeLocalisation::polygon );
    pTypeComboBox_->insertItem( "Point"   , EnumTypeLocalisation::point );
    pTypeComboBox_->insertItem( "Secteur" , EnumTypeLocalisation::sector );
    pTypeComboBox_->setCurrentItem( EnumTypeLocalisation::polygon );
    pLocalLayout->addWidget( pTypeComboBox_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_MTH_Localisation constructor
// Created: NLD 2003-08-12
// -----------------------------------------------------------------------------
MOS_MTH_Localisation::MOS_MTH_Localisation( QWidget* pParent, const std::string& strName )
    :   QWidget( pParent )
    ,   MOS_MissionTool_ABC()
    ,   pAsnLocalisation_ ( 0 )
    ,   vOldPos_         ( 0., 0.)
    ,   pAgent_          ( 0 )
    ,   bMovingLine_     ( false )
    ,   bFirstWidget_    ( false )
	,  pMission_ ( 0 )
	, pWidgetTab_( 0 )
{
    QVBoxLayout* pLocalLayout = new QVBoxLayout( this );

    // Title
    QLabel* pLabel = CreateTitle( this, strName );
    pLocalLayout->addWidget( pLabel );

    // Points
    pListView_ = new MOS_MTH_Localisation_ListView( this, strName );
    pLocalLayout->addWidget( pListView_ );
    
    // Type
    pTypeComboBox_ = new QComboBox( this, "test" );   
    pTypeComboBox_->insertItem( "Circle"  , EnumTypeLocalisation::circle );
    pTypeComboBox_->insertItem( "Ellipse" , EnumTypeLocalisation::ellipse  );
    pTypeComboBox_->insertItem( "Ligne"   , EnumTypeLocalisation::line );
    pTypeComboBox_->insertItem( "Polygone", EnumTypeLocalisation::polygon );
    pTypeComboBox_->insertItem( "Point"   , EnumTypeLocalisation::point );
    pTypeComboBox_->insertItem( "Secteur" , EnumTypeLocalisation::sector );
    pTypeComboBox_->setCurrentItem( EnumTypeLocalisation::polygon );
    pLocalLayout->addWidget( pTypeComboBox_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Localisation constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_MTH_Localisation::~MOS_MTH_Localisation()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Localisation::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_Localisation::Initialize()
{
    pListView_->Initialize();
    if( pAsnLocalisation_ )
        pAsnLocalisation_->vecteur_point.n = 0;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_Localisation::Terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_Localisation::Terminate()
{
    pListView_->Terminate();
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_Localisation::WriteMsg
// Created: NLD 2003-03-04
//-----------------------------------------------------------------------------
void MOS_MTH_Localisation::WriteMsg(  std::string& sParam )
{
    if( pAsnLocalisation_ )
        pAsnLocalisation_->type = (ASN1T_EnumTypeLocalisation)pTypeComboBox_->currentItem();
    pListView_->WriteMsg( sParam );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Localisation::WriteMsg
// Created: NLD 2003-03-04
//-----------------------------------------------------------------------------
void MOS_MTH_Localisation::WriteMsg( ASN1T_Localisation& asnLocalisation, std::string& sParam  )
{
    asnLocalisation.type = (ASN1T_EnumTypeLocalisation)pTypeComboBox_->currentItem();
    sParam += MT_FormatString( "TypeLoca: %d | ", (uint)pTypeComboBox_->currentItem() );

    pListView_->WriteMsg( asnLocalisation, sParam );
}




//-----------------------------------------------------------------------------
// Name: MOS_MTH_Localisation::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_Localisation::FillRandomParameters()
{
    uint nRnd = rand() % 3;
    switch( nRnd )
    {
        case 0 : nRnd = 2/*li32*/;  break;
        case 1 : nRnd = 3/*po32*/;  break;
        case 2 : nRnd = 4/*pt*/;    break;
    }
    SetLocalisationType( (ASN1T_EnumTypeLocalisation)nRnd );
    
    pListView_->FillRandomParameters();
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Localisation::ResaveParameters
// Created: FBD 03-09-08
//-----------------------------------------------------------------------------
void MOS_MTH_Localisation::ResaveParameters( MT_OutputArchive_ABC& archive )
{
    uint nLocaType = GetLocalisationType();

    archive.WriteField( "sName", std::string( "MOS_MTH_Localisation|nLocaType" ) );
    archive.WriteField( "nLocaType", nLocaType );
    pListView_->ResaveParameters( archive );
}



//-----------------------------------------------------------------------------
// Name: MOS_MTH_Localisation::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_Localisation::ReloadParameters( MT_InputArchive_ABC& archive )
{
    uint nLocaType = 0;
// -------------------------------------------------
        std::string sName;
        archive.ReadField( "sName", sName );
        assert( sName == "MOS_MTH_Localisation|nLocaType" );
// -------------------------------------------------
    archive.ReadField( "nLocaType", nLocaType );
    SetLocalisationType( (ASN1T_EnumTypeLocalisation)nLocaType );
    
    pListView_->ReloadParameters( archive );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Localisation::SetAgent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_MTH_Localisation::SetAgent( MOS_Agent* pAgent )
{
    pListView_->SetAgent( pAgent );
}



//-----------------------------------------------------------------------------
// Name: MOS_MTH_Localisation::NotifyAgentHasMoved
// Created: FBD 03-01-23
//-----------------------------------------------------------------------------
void MOS_MTH_Localisation::NotifyAgentHasMoved( MOS_Agent& /*agent*/, const MT_Vector2D& /*vPos*/ )
{

}



//-----------------------------------------------------------------------------
// Name: MOS_MTH_Localisation::SetMission
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_MTH_Localisation::SetMission( MOS_Mission_ABC* pMission )
{
    pMission_ = pMission;
    pListView_->SetMission( pMission );
}   

//-----------------------------------------------------------------------------
// Name: MOS_MTH_Localisation::SetWidgetTab
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
void MOS_MTH_Localisation::SetWidgetTab( QWidget* pWidget )
{
    pWidgetTab_ = pWidget;
    pListView_->SetWidgetTab( pWidget );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Localisation::SetLocalisationType
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
void MOS_MTH_Localisation::SetLocalisationType( ASN1T_EnumTypeLocalisation nType )
{
    pTypeComboBox_->setCurrentItem( nType );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Localisation::GetLocalisationType
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
ASN1T_EnumTypeLocalisation MOS_MTH_Localisation::GetLocalisationType() const
{
    return (ASN1T_EnumTypeLocalisation)(pTypeComboBox_->currentItem());
}


// -----------------------------------------------------------------------------
// Name: MOS_MTH_Localisation::Clear
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
void MOS_MTH_Localisation::Clear()
{
    pListView_->Clear();
}
