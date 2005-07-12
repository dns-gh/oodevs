//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_GDH.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 9 $
// $Workfile: MOS_MTH_GDH.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_MTH_GDH.h"
#include "moc_MOS_MTH_GDH.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_MTH_GDH.inl"
#endif


#include "MOS_Agent.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_AgentManager.h"
#include "MOS_LifeBar.h"


#include <qlcdnumber.h>
#include <qdatetimeedit.h> 


using namespace DIN;


//-----------------------------------------------------------------------------
// Name: MOS_MTH_GDH constructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_MTH_GDH::MOS_MTH_GDH( ASN1T_GDH& asnGDH, QWidget* pParent, const std::string& sName )
    : QFrame             ( pParent )
    , MOS_MissionTool_ABC( ) 
    , asnGDH_            ( asnGDH )
    , sName_             ( sName )
{
//    setMargin( 50 );
//    setLineWidth( 2 );
//    setFrameStyle( QFrame::Sunken | QFrame::Box );

    QHBoxLayout* pMainLayout = new QHBoxLayout( this );
//    pMainLayout->setMargin( 5 );
//    pMainLayout->setSpacing( 5 );

    // Title
    QLabel* pLabel = CreateTitle( this, sName );
    pMainLayout->addWidget( pLabel );
   
    pDateTimeEdit_ = new QDateTimeEdit( QDateTime::currentDateTime(), this );
    pMainLayout->addWidget( pDateTimeEdit_ );

    pDontUseGdhCheckBox_ = new QCheckBox( "Optionnel", this );
    pMainLayout->addWidget( pDontUseGdhCheckBox_ );
    connect( pDontUseGdhCheckBox_, SIGNAL( toggled( bool ) ), SLOT( SlotUseGDHToogled( bool ) ) );
    pDontUseGdhCheckBox_->setChecked( false );

    setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum ) );
}



//-----------------------------------------------------------------------------
// Name: MOS_MTH_GDH destructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_MTH_GDH::~MOS_MTH_GDH()
{
    
}


//=============================================================================
// INITIALIZATION
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_MTH_GDH::Initialize
// Created: NLD 2002-08-23
//-----------------------------------------------------------------------------
void MOS_MTH_GDH::Initialize()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_GDH::Terminate
// Created: NLD 2002-08-23
//-----------------------------------------------------------------------------
void MOS_MTH_GDH::Terminate()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_GDH::WriteMsg
// Created: FBD 03-01-21
//-----------------------------------------------------------------------------
void MOS_MTH_GDH::WriteMsg( std::string& sParam )
{
    sParam += sName_;
    sParam += " at ";

    if( pDontUseGdhCheckBox_->isChecked() )
    {
        sParam += "0 | ";
        asnGDH_.qualificatif = EnumGDH_Qualificatif::at;
        asnGDH_.datation     = 0;
    }
    else
    {
        sParam += pDateTimeEdit_->dateTime().toString();
        sParam += " | ";

        static QDateTime baseDateTime( QDate( 1901, 1, 1 ) );

        asnGDH_.qualificatif = EnumGDH_Qualificatif::at;
        asnGDH_.datation     = baseDateTime.secsTo( pDateTimeEdit_->dateTime() );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_GDH::NotifyAgentHasMoved
// Created: NLD 2003-03-26
//-----------------------------------------------------------------------------
void MOS_MTH_GDH::NotifyAgentHasMoved( MOS_Agent& /*agent*/, const MT_Vector2D& /*vPos*/ )
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_GDH::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_GDH::FillRandomParameters()
{
    pDontUseGdhCheckBox_->setChecked( true );
//    nValueWidget_ = rand() % nValueMaxWidget_;
//    nValueWidget_ +=  nValueMinWidget_;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_GDH::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_GDH::ReloadParameters( MT_InputArchive_ABC& archive )
{
//    nValueWidget_ = 0; //rand() % nValueMaxWidget_;
// -------------------------------------------------
//        std::string sName;
//        archive.ReadField( "sName", sName );
//        assert( sName == "MOS_MTH_GDH|nValueWidget" );
// -------------------------------------------------
//    archive.ReadField( "nValueWidget", nValueWidget_ );
//    nValueWidget_ +=  nValueMinWidget_;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_GDH::ResaveParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_GDH::ResaveParameters( MT_OutputArchive_ABC& archive )
{
//    archive.WriteField( "sName", std::string( "MOS_MTH_GDH|nValueWidget" ) );
//    archive.WriteField( "nValueWidget", nValueWidget_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_MTH_GDH::SetWidgetTab
// Created: NLD 2004-01-16
// -----------------------------------------------------------------------------
void MOS_MTH_GDH::SetWidgetTab( QWidget* /*pWidget*/ )
{

}

// -----------------------------------------------------------------------------
// Name: MOS_MTH_GDH::SlotUseGDHToogled
// Created: NLD 2004-01-19
// -----------------------------------------------------------------------------
void MOS_MTH_GDH::SlotUseGDHToogled( bool bToggled )
{
    pDateTimeEdit_->setEnabled( !bToggled );
}
