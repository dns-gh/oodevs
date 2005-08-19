//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimaMission.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 18 $
// $Workfile: MOS_LimaMission.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_LimaMission.h"
#include "moc_MOS_LimaMission.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_LimaMission.inl"
#endif


#include <QButtonGroup.h>
#include <QWidget.h>
#include <QCheckBox.h>

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_WorldWidget.h"
#include "MOS_Mainwindow.h"
#include "MOS_LimaListView.h"
#include "MOS_Mission_ABC.h"
using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_LimaMission constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_LimaMission::MOS_LimaMission( ASN1T_ListOID& asnLimas, QWidget* pParent )
    : QFrame             ( pParent )
    , MOS_MissionTool_ABC()
    , asnLimas_          ( asnLimas ) 
{    
    setMargin( 80 );
    setLineWidth( 2 );
    setFrameStyle( QFrame::Sunken | QFrame::Box );

    QVBoxLayout* pLocalLayout = new QVBoxLayout( this );

    // Title
    QLabel* pLabel = CreateTitle( this, "Limas" );
    pLocalLayout->addWidget( pLabel );

    // Listview
    pLimaListView_ = new MOS_LimaListView( this, eListViewMission );
    pLimaListView_->setMargin( 5 );
    pLimaListView_->setLineWidth( 2 );
    pLimaListView_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pLocalLayout->addWidget( pLimaListView_ );

    // CheckBox
    QHBoxLayout* pCheckBoxLayout = new QHBoxLayout( pLocalLayout );
    pCheckBoxLayout->setMargin( 5 );
    QCheckBox* pCheckBoxAll = new QCheckBox( "Show All", this );
    pCheckBoxAll->setChecked( true );
    pCheckBoxLayout->addWidget( pCheckBoxAll );
    connect( pCheckBoxAll, SIGNAL( toggled( bool ) ), pLimaListView_, SLOT( SlotSetShowAll( bool ) ) );

    // Button
    QPushButton* pSelectButton = new QPushButton( "Add Lima", this );
    pLocalLayout->addWidget( pSelectButton );
    connect( pSelectButton, SIGNAL( clicked() ), pLimaListView_, SLOT( EnterLimaSelector() ) );
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaMission destructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_LimaMission::~MOS_LimaMission()
{
    
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaMission::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimaMission::Initialize()
{
    asnLimas_.n = 0;
    assert( pLimaListView_ );
    pLimaListView_->Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaMission::terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimaMission::Terminate()
{
    if( asnLimas_.n > 0 )
        delete [] asnLimas_.elem; //$$$ TMP

    assert( pLimaListView_ );
    pLimaListView_->Terminate();
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaMission::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_LimaMission::IsActive( bool bMove )
{
    if ( pMission_ )
        return pMission_->IsActive( bMove );
    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaMission::WriteMsg
// Created: NLD 2003-03-01
//-----------------------------------------------------------------------------
void MOS_LimaMission::WriteMsg( std::string& sParam )
{
    pLimaListView_->WriteMsg( asnLimas_, sParam );
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaMission::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_LimaMission::FillRandomParameters()
{
    pLimaListView_->FillRandomParameters();

}

//-----------------------------------------------------------------------------
// Name: MOS_LimaMission::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_LimaMission::ReloadParameters( MT_InputArchive_ABC& archive )
{
    pLimaListView_->ReloadParameters( archive );

}

//-----------------------------------------------------------------------------
// Name: MOS_LimaMission::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_LimaMission::ResaveParameters( MT_OutputArchive_ABC& archive )
{
    pLimaListView_->ResaveParameters( archive );

}

//-----------------------------------------------------------------------------
// Name: MOS_LimaMission::SetAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimaMission::SetAgent( MOS_Agent* pAgent )
{
    pLimaListView_->SetAgent( pAgent );
}
//-----------------------------------------------------------------------------
// Name: MOS_LimaMission::SetMission
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_LimaMission::SetMission( MOS_Mission_ABC* pMission )
{
    pMission_ = pMission;
}

