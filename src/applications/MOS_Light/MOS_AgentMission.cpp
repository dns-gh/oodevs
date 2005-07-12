//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentMission.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 18 $
// $Workfile: MOS_AgentMission.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_AgentMission.h"
#include "moc_MOS_AgentMission.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_AgentMission.inl"
#endif


#include <QButtonGroup.h>
#include <QWidget.h>
#include <QCheckBox.h>
#include <QWidgetStack.h>

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_WorldWidget.h"
#include "MOS_Mainwindow.h"
#include "MOS_AgentListView.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_AgentMission constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_AgentMission::MOS_AgentMission( ASN1T_ListAgent& asnListAgent, QWidget* pParent, const std::string& strName, int nNbMaxAgent )
    : QWidget            ( pParent )
    , MOS_MissionTool_ABC()
    , pASNListAgent_     ( &asnListAgent )
    , pASNAgent_         ( 0 )
    , pMission_          ( 0 )
    , pWidgetTab_        ( 0 )
    , nNbMaxAgent_       ( nNbMaxAgent )
    , pAgentPresent_     ( 0 )
    , sName_             ( strName )
{
    QVBoxLayout* pLocalLayout = new QVBoxLayout( this );

    QLabel* pTitle = CreateTitle( this, strName );
    pLocalLayout->addWidget( pTitle );

    pAgentListView_ = new MOS_AgentListView( this, eListViewMission );
    pAgentListView_->setMargin( 5 );
    pAgentListView_->setLineWidth( 2 );
    pAgentListView_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pLocalLayout->addWidget( pAgentListView_ );
   
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentMission constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_AgentMission::MOS_AgentMission( ASN1T_Agent& asnAgent, QWidget* pParent, const std::string& strName, unsigned* pAgentPresent )
    : QWidget            ( pParent )
    , MOS_MissionTool_ABC()
    , pASNListAgent_     ( 0 )
    , pASNAgent_         ( &asnAgent )
    , pMission_          ( 0 )
    , pWidgetTab_        ( 0 )
    , nNbMaxAgent_       ( 1 )
    , pAgentPresent_     ( pAgentPresent )
    , sName_             ( strName )

{
    QVBoxLayout* pLocalLayout = new QVBoxLayout( this );

    QLabel* pTitle = CreateTitle( this, strName );
    pLocalLayout->addWidget( pTitle );

    pAgentListView_ = new MOS_AgentListView( this, eListViewMission );
    pAgentListView_->setMargin( 5 );
    pAgentListView_->setLineWidth( 2 );
    pAgentListView_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pLocalLayout->addWidget( pAgentListView_ );
   
}  

//-----------------------------------------------------------------------------
// Name: MOS_AgentMission destructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_AgentMission::~MOS_AgentMission()
{
    
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentMission::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_AgentMission::Initialize()
{
    if( pASNListAgent_ )
        pASNListAgent_->n = 0;
    assert( pAgentListView_ );
    pAgentListView_->Initialize();

}

//-----------------------------------------------------------------------------
// Name: MOS_AgentMission::terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_AgentMission::Terminate()
{
    if( pASNListAgent_ && pASNListAgent_->n > 0 )
        delete [] pASNListAgent_->elem; //$$$ TMP
    assert( pAgentListView_ );
    pAgentListView_->Terminate();
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentMission::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_AgentMission::IsActive( bool /*bMove*/ )
{
    QWidgetStack* pStack = (QWidgetStack*)parent();
    if ( pStack->visibleWidget() == this )
        return true;
    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentMission::NotifyDeleteAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_AgentMission::NotifyDeleteAgent( MOS_Agent& agent )
{
    assert( pAgentListView_ );
    pAgentListView_->NotifyDeleteAgent( agent );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentMission::NotifyAgentHasMoved
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
void MOS_AgentMission::NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos )
{
    assert( pAgentListView_ );
    pAgentListView_->NotifyAgentHasMoved( agent, vPos );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentMission::SetMission
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_AgentMission::SetMission( MOS_Mission_ABC* pMission )
{
    pMission_ = pMission;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentMission::SetWidgetTab
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
void MOS_AgentMission::SetWidgetTab( QWidget* pWidget )
{
    pWidgetTab_ = pWidget;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentMission::WriteMsg
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_AgentMission::WriteMsg( std::string& sParam )
{
    sParam += sName_;
    sParam += " : ";

    assert( pAgentListView_ );
    if( pASNListAgent_ )
    {
        pAgentListView_->WriteMsg( *pASNListAgent_, sParam );
        assert( !pASNAgent_ );
    }
    else
    {
        assert( pASNAgent_ );
        if( pAgentPresent_ != 0 )
        {
            if( pAgentListView_->IsEmpty() )
            {
                (*pAgentPresent_) = 0;
                sParam += std::string( "Pas d'agent | " );
                return;
            }
            else
            {
                (*pAgentPresent_) = 1;
                sParam += std::string( "Agent present | " );
            }
        }

        pAgentListView_->WriteMsg( *pASNAgent_, sParam );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentMission::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_AgentMission::FillRandomParameters()
{
    pAgentListView_->FillRandomParameters();

}

//-----------------------------------------------------------------------------
// Name: MOS_AgentMission::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_AgentMission::ReloadParameters( MT_InputArchive_ABC& archive )
{
    pAgentListView_->ReloadParameters( archive );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentMission::ResaveParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_AgentMission::ResaveParameters( MT_OutputArchive_ABC& archive )
{
    pAgentListView_->ResaveParameters( archive );

}

//-----------------------------------------------------------------------------
// Name: MOS_AgentMission::SetAgent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_AgentMission::SetAgent( MOS_Agent* pAgent )
{
    assert( pAgentListView_ );
    pAgentListView_->SetAgent( pAgent );
}
