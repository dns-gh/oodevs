//*****************************************************************************
// 
// $Created: AGN 03-04-23 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_AgentKnowledgeList.cpp $
// $Author: Age $
// $Modtime: 23/11/04 10:48 $
// $Revision: 10 $
// $Workfile: MOS_MTH_AgentKnowledgeList.cpp $
// 
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_MTH_AgentKnowledgeList.h"
#include "moc_MOS_MTH_AgentKnowledgeList.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_MTH_AgentKnowledgeList.inl"
#endif

#include "MOS_MTH_AgentKnowledgeListItem.h"
#include "MOS_Agent.h"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_WorldWidget.h"
#include "MOS_AgentManager.h"
#include "MOS_Agent.h"
#include "MOS_Gtia.h"

//-----------------------------------------------------------------------------
// Name: MOS_MTH_AgentKnowledgeList constructor
// Created: AGN 03-04-24
//-----------------------------------------------------------------------------
MOS_MTH_AgentKnowledgeList::MOS_MTH_AgentKnowledgeList( ASN1T_ListKnowledgeAgent& asnListAgent, QWidget* pParent, const std::string& sName )
: QListView( pParent, sName.c_str() )
, MOS_MissionTool_ABC()
, pASNAgentList_( & asnListAgent )
, pASNAgent_( 0 )
, pAgent_( 0 )
{
    asnListAgent.n = 0;

    addColumn( sName.c_str() );

    connect( this, SIGNAL( clicked( QListViewItem* ) ), this, SLOT( OnClick( QListViewItem* ) ) );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_AgentKnowledgeList constructor
// Created: AGN 03-04-24
//-----------------------------------------------------------------------------
MOS_MTH_AgentKnowledgeList::MOS_MTH_AgentKnowledgeList( ASN1T_KnowledgeAgent& asnAgent, QWidget* pParent, const std::string& sName )
: QListView( pParent, sName.c_str() )
, MOS_MissionTool_ABC()
, pASNAgentList_( 0 )
, pASNAgent_( & asnAgent )
, pAgent_( 0 )
{
    addColumn( sName.c_str() );

    connect( this, SIGNAL( clicked( QListViewItem* ) ), this, SLOT( OnClick( QListViewItem* ) ) );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_AgentKnowledgeList destructor
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
MOS_MTH_AgentKnowledgeList::~MOS_MTH_AgentKnowledgeList()
{

}



//-----------------------------------------------------------------------------
// Name: MOS_MTH_AgentKnowledgeList::SetMission
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
void MOS_MTH_AgentKnowledgeList::SetMission( MOS_Mission_ABC* /*pMission*/ )
{

}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_AgentKnowledgeList::SetWidgetTab
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
void MOS_MTH_AgentKnowledgeList::SetWidgetTab( QWidget* /*pWidget*/ )
{

}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_AgentKnowledgeList::SetAgent
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
void MOS_MTH_AgentKnowledgeList::SetAgent( MOS_Agent* pAgent )
{
    if( pAgent_ != 0 )
        pAgent_->GetGtia().NotifyShouldDrawKnowledges( false );

    clear();

    QCheckListItem* pParent = new QCheckListItem( this, "Connaissances", QCheckListItem::Controller );
    pParent->setOpen( true );

    pAgent_ = pAgent;
    const MOS_Gtia::T_AgentKnowledgeMap& knowMap = pAgent->GetGtia().GetAgentKnowledges();
    for( MOS_Gtia::CIT_AgentKnowledgeMap it = knowMap.begin(); it != knowMap.end(); ++it )
    {
        const std::string strTitle = it->second->GetName();
        if( pASNAgent_ != 0 )
            new MOS_MTH_AgentKnowledgeListItem( *it->second, pParent, QCheckListItem::RadioButton, strTitle );
        else
            new MOS_MTH_AgentKnowledgeListItem( *it->second, pParent, QCheckListItem::CheckBox, strTitle );
    }

    pAgent_->GetGtia().NotifyShouldDrawKnowledges( true );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_AgentKnowledgeList::WriteMsg
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
void MOS_MTH_AgentKnowledgeList::WriteMsg( std::string& sParam )
{
    if( pASNAgent_ != 0 )
    {
        QListViewItem* pItem = this->firstChild()->firstChild();
        if ( pItem == 0 )
        {
            sParam += std::string( "No Knowledge |" );
        }

        while( pItem != 0 )
        {
            MOS_MTH_AgentKnowledgeListItem* pKnow = static_cast< MOS_MTH_AgentKnowledgeListItem* >( pItem );
            if( pKnow->isOn() )
            {
                (*pASNAgent_) = pKnow->GetMOSID();
                sParam += MT_FormatString( "KnowId:%d | ", pKnow->GetMOSID() );
                break;
            }
            pItem = pItem->nextSibling();
        }
    }
    else
    {
        assert( pASNAgentList_ != 0 );
        QListViewItem* pItem = this->firstChild()->firstChild();

        T_AgentKnowledgeVector result;
        while( pItem != 0 )
        {
            MOS_MTH_AgentKnowledgeListItem* pKnow = static_cast< MOS_MTH_AgentKnowledgeListItem* >( pItem );
            if( pKnow->isOn() )
                result.push_back( & pKnow->GetRepresented() );
            pItem = pItem->nextSibling();
        }

        pASNAgentList_->n = result.size();
        pASNAgentList_->elem = new ASN1T_OID[ result.size() ];
        sParam += MT_FormatString( "NBKnowledge:%d | ", result.size() );

        for( IT_AgentKnowledgeVector it = result.begin(); it != result.end(); ++it )
        {
            pASNAgentList_->elem[it - result.begin()] = (*it)->GetID();
            sParam += MT_FormatString( "KnowledgeId:%d | ", (*it)->GetID() );

        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_AgentKnowledgeList::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_AgentKnowledgeList::FillRandomParameters()
{
    uint nInfo = 0;
    QListViewItem* pItem = this->firstChild()->firstChild();
    while( pItem != 0 )
    {
        ++nInfo;
        MOS_MTH_AgentKnowledgeListItem* pKnow = static_cast< MOS_MTH_AgentKnowledgeListItem* >( pItem );
        pKnow->setOn( false );
        pItem = pItem->nextSibling();
    }

    if( pASNAgent_ != 0 )
    {
        if ( nInfo != 0 )
        {
            uint nSelected = rand() % nInfo;
            pItem = this->firstChild()->firstChild();
            uint i = 0;
            while( pItem != 0 )
            {
                MOS_MTH_AgentKnowledgeListItem* pKnow = static_cast< MOS_MTH_AgentKnowledgeListItem* >( pItem );
                if ( i == nSelected )
                {
                    pKnow->setOn( true );
                    break;
                }
                else
                    pItem = pItem->nextSibling();
            }
        }
    }
    else
    {
        uint nNbToSelected = rand() % nInfo;

        uint i = 0;
        while ( i < nNbToSelected )
        {
            pItem = this->firstChild()->firstChild();
            while( pItem != 0 )
            {
                MOS_MTH_AgentKnowledgeListItem* pKnow = static_cast< MOS_MTH_AgentKnowledgeListItem* >( pItem );
                if ( !pKnow->isOn() )
                {
                    uint nHazard = rand() % 2;
                    if( nHazard )
                    {
                        pKnow->setOn( true );
                        ++i;
                    }
                }
                pItem = pItem->nextSibling();
            }
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_AgentKnowledgeList::ResaveParameters
// Created: FBD 03-09-08
//-----------------------------------------------------------------------------
void MOS_MTH_AgentKnowledgeList::ResaveParameters( MT_OutputArchive_ABC& archive )
{
    uint nInfo = 0;
    QListViewItem* pItem = this->firstChild()->firstChild();
    while( pItem != 0 )
    {
        ++nInfo;
        pItem = pItem->nextSibling();
    }

    if( pASNAgent_ != 0 )
    {
        if ( nInfo != 0 )
        {
            uint nSelected = 0;
            pItem = this->firstChild()->firstChild();
            uint i = 0;
            while( pItem != 0 )
            {
                MOS_MTH_AgentKnowledgeListItem* pKnow = static_cast< MOS_MTH_AgentKnowledgeListItem* >( pItem );
                if ( pKnow->isOn() )
                {
                    nSelected = i;
                    archive.WriteField( "sName", std::string( "MOS_MTH_AgentKnowledgeList|nSelected" ) );
                    archive.WriteField( "nSelected", nSelected );
                    break;
                }
                else
                    pItem = pItem->nextSibling();
            }
        }
    }
    else
    {
        uint nNbToSelected = 0;
        while( pItem != 0 )
        {
            MOS_MTH_AgentKnowledgeListItem* pKnow = static_cast< MOS_MTH_AgentKnowledgeListItem* >( pItem );
            if ( pKnow->isOn() )
                nNbToSelected++;

            pItem = pItem->nextSibling();
        }
        archive.WriteField( "sName", std::string( "MOS_MTH_AgentKnowledgeList|nNbToSelected" ) );
        archive.WriteField( "nNbToSelected", nNbToSelected );

        uint i = 0;
        pItem = this->firstChild()->firstChild();
        while( pItem != 0 )
        {
            MOS_MTH_AgentKnowledgeListItem* pKnow = static_cast< MOS_MTH_AgentKnowledgeListItem* >( pItem );
            if ( pKnow->isOn() )
            {
                uint nIdOn = i;
                archive.WriteField( "sName", std::string( "MOS_MTH_AgentKnowledgeList|nIdOn" ) );
                archive.WriteField( "nIdOn", nIdOn );
                ++i;
            }
            pItem = pItem->nextSibling();
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_AgentKnowledgeList::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_AgentKnowledgeList::ReloadParameters( MT_InputArchive_ABC& archive )
{
    uint nInfo = 0;
    QListViewItem* pItem = this->firstChild()->firstChild();
    while( pItem != 0 )
    {
        ++nInfo;
        MOS_MTH_AgentKnowledgeListItem* pKnow = static_cast< MOS_MTH_AgentKnowledgeListItem* >( pItem );
        pKnow->setOn( false );
        pItem = pItem->nextSibling();
    }

    if( pASNAgent_ != 0 )
    {
        if ( nInfo != 0 )
        {
            uint nSelected = 0; //rand() % nInfo;
// -------------------------------------------------
        std::string sName;
        archive.ReadField( "sName", sName );
        assert( sName == "MOS_MTH_AgentKnowledgeList|nSelected" );
// -------------------------------------------------
            archive.ReadField( "nSelected", nSelected );

            pItem = this->firstChild()->firstChild();
            uint i = 0;
            while( pItem != 0 )
            {
                MOS_MTH_AgentKnowledgeListItem* pKnow = static_cast< MOS_MTH_AgentKnowledgeListItem* >( pItem );
                if ( i == nSelected )
                {
                    pKnow->setOn( true );
                    break;
                }
                else
                    pItem = pItem->nextSibling();
            }
        }
    }
    else
    {
        uint nNbToSelected = 0; //rand() % nInfo;
// -------------------------------------------------
        std::string sName;
        archive.ReadField( "sName", sName );
        assert( sName == "MOS_MTH_AgentKnowledgeList|nNbToSelected" );
// -------------------------------------------------
        archive.ReadField( "nNbToSelected", nNbToSelected );

        uint i = 0;
        while ( i < nNbToSelected )
        {
// -------------------------------------------------
            std::string sName;
            archive.ReadField( "sName", sName );
            assert( sName == "MOS_MTH_AgentKnowledgeList|nIdOn" );
            uint nIdOn = 0;
            archive.ReadField( "nIdOn", nIdOn );
// -------------------------------------------------

            uint j = 0;
            pItem = this->firstChild()->firstChild();
            while( pItem != 0 )
            {
                MOS_MTH_AgentKnowledgeListItem* pKnow = static_cast< MOS_MTH_AgentKnowledgeListItem* >( pItem );
                if ( j == nIdOn )
                {
                    ++i;
                    pKnow->setOn( true );
                    break;
                }
                ++j;
                pItem = pItem->nextSibling();
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_AgentKnowledgeList::NotifyAgentHasMoved
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
void MOS_MTH_AgentKnowledgeList::NotifyAgentHasMoved( MOS_Agent& /*agent*/, const MT_Vector2D& /*vPos*/ )
{

}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_AgentKnowledgeList::Initialize
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
void MOS_MTH_AgentKnowledgeList::Initialize()
{

}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_AgentKnowledgeList::Terminate
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
void MOS_MTH_AgentKnowledgeList::Terminate()
{
    if( pAgent_ != 0 )
        pAgent_->GetGtia().NotifyShouldDrawKnowledges( false );

    if( pASNAgentList_ && pASNAgentList_->n > 0 )
        delete [] pASNAgentList_->elem; //$$$ TMP
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_AgentKnowledgeList::OnClick
// Created: AGN 03-04-24
//-----------------------------------------------------------------------------
// slot
void MOS_MTH_AgentKnowledgeList::OnClick( QListViewItem* pItem )
{
    QCheckListItem* pTest = static_cast< QCheckListItem* >( pItem );
    if( pTest == 0 || pTest->type() == QCheckListItem::Controller )
        return;

    MOS_MTH_AgentKnowledgeListItem* pCastedItem = static_cast< MOS_MTH_AgentKnowledgeListItem* >( pItem );
    if( pCastedItem == 0 )
        return;

    MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().SetWantedCenter( pCastedItem->GetRepresented().GetPosition() );
}

