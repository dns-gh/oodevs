//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimitMission.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 18 $
// $Workfile: MOS_LimitMission.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_LimitMission.h"

#include "moc_MOS_LimitMission.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_LimitMission.inl"
#endif


#include <QButtonGroup.h>
#include <QWidget.h>
#include <QCheckBox.h>

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_WorldWidget.h"
#include "MOS_Mainwindow.h"
#include "MOS_LimitListView.h"
#include "MOS_Mission_ABC.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_LimitMission constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_LimitMission::MOS_LimitMission( ASN1T_OID& asnLeftLimitID, ASN1T_OID& asnRightLimitID, QWidget* pParent )
    : QFrame             ( pParent )
    , MOS_MissionTool_ABC()
    , asnLeftLimitID_    ( asnLeftLimitID )
    , asnRightLimitID_   ( asnRightLimitID )
    , pMission_          ( 0 )
{
    setMargin( 80 );
    setLineWidth( 2 );
    setFrameStyle( QFrame::Sunken | QFrame::Box );

    QVBoxLayout* pLocalLayout = new QVBoxLayout( this );

    // Title
    QLabel* pLabel = CreateTitle( this, "Limits" );
    pLocalLayout->addWidget( pLabel );

    // Listview
    pLimitListView_ = new MOS_LimitListView( this, eListViewMission );
    pLimitListView_->setMargin( 5 );
    pLimitListView_->setLineWidth( 2 );
    pLimitListView_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pLocalLayout->addWidget( pLimitListView_ );

    // CheckBox
    QHBoxLayout* pCheckBoxLayout = new QHBoxLayout( pLocalLayout );
    pCheckBoxLayout->setMargin( 5 );
    QCheckBox* pCheckBoxAll = new QCheckBox( "Show All", this );
    pCheckBoxAll->setChecked( true );
    pCheckBoxLayout->addWidget( pCheckBoxAll );
    connect( pCheckBoxAll, SIGNAL( toggled( bool ) ), pLimitListView_, SLOT( SlotSetShowAll( bool ) ) );

    // Button
    QPushButton* pSelectButton = new QPushButton( "Add Limit", this );
    pLocalLayout->addWidget( pSelectButton );
    connect( pSelectButton, SIGNAL( clicked() ), pLimitListView_, SLOT( EnterLimitSelector() ) );
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitMission destructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_LimitMission::~MOS_LimitMission()
{
    
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitMission::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimitMission::Initialize()
{
    assert( pLimitListView_ );
    pLimitListView_->Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitMission::terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimitMission::Terminate()
{
    assert( pLimitListView_ );
    pLimitListView_->Terminate();
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitMission::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_LimitMission::IsActive( bool bMove )
{
     if ( pMission_ )
         return pMission_->IsActive( bMove );
     
     return false;
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitMission::WriteMsg
// Created: NLD 03-03-01
//-----------------------------------------------------------------------------
void MOS_LimitMission::WriteMsg( std::string& sParam )
{
    pLimitListView_->WriteMsg( asnLeftLimitID_, asnRightLimitID_, sParam );
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitMission::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_LimitMission::FillRandomParameters()
{
    pLimitListView_->FillRandomParameters();

}

//-----------------------------------------------------------------------------
// Name: MOS_LimitMission::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_LimitMission::ReloadParameters( MT_InputArchive_ABC& archive )
{
    pLimitListView_->ReloadParameters( archive );

}

//-----------------------------------------------------------------------------
// Name: MOS_LimitMission::ResaveParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_LimitMission::ResaveParameters( MT_OutputArchive_ABC& archive )
{
    pLimitListView_->ResaveParameters( archive );

}
//-----------------------------------------------------------------------------
// Name: MOS_LimaMission::SetAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimitMission::SetAgent( MOS_Agent* pAgent )
{
    pLimitListView_->SetAgent( pAgent );
}
//-----------------------------------------------------------------------------
// Name: MOS_LimitMission::SetMission
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_LimitMission::SetMission( MOS_Mission_ABC* pMission )
{
    pMission_ = pMission;
}

