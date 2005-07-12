//*****************************************************************************
// 
// $Created: AGN 03-04-23 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_ObjectKnowledgeList.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 9 $
// $Workfile: MOS_MTH_ObjectKnowledgeList.cpp $
// 
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_MTH_ObjectKnowledgeList.h"
#include "moc_MOS_MTH_ObjectKnowledgeList.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_MTH_ObjectKnowledgeList.inl"
#endif

#include "MOS_Agent.h"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_WorldWidget.h"
#include "MOS_Agent.h"
#include "MOS_ObjectKnowledge.h"
#include "MOS_MTH_ObjectKnowledgeListItem.h"
#include "MOS_Gtia.h"
#include "MOS_Team.h"

//-----------------------------------------------------------------------------
// Name: MOS_MTH_ObjectKnowledgeList constructor
// Created: AGN 03-04-24
//-----------------------------------------------------------------------------
MOS_MTH_ObjectKnowledgeList::MOS_MTH_ObjectKnowledgeList( ASN1T_ListKnowledgeObject& asnKnowledgeObjectList, QWidget* pParent, const std::string& sName )
: QListView( pParent, sName.c_str() )
, MOS_MissionTool_ABC()
, pASNKnowledgeObjectList_( & asnKnowledgeObjectList )
, pASNKnowledgeObject_( 0 )
, pAgent_( 0 )
{
    asnKnowledgeObjectList.n = 0;
    
    addColumn( sName.c_str() );
    connect( this, SIGNAL( clicked( QListViewItem* ) ), this, SLOT( OnClick( QListViewItem* ) ) );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ObjectKnowledgeList constructor
// Created: AGN 03-04-24
//-----------------------------------------------------------------------------
MOS_MTH_ObjectKnowledgeList::MOS_MTH_ObjectKnowledgeList( ASN1T_KnowledgeObject& asnKnowledgeObject, QWidget* pParent, const std::string& sName )
: QListView( pParent, sName.c_str() )
, MOS_MissionTool_ABC()
, pASNKnowledgeObjectList_( 0 )
, pASNKnowledgeObject_( & asnKnowledgeObject )
, pAgent_( 0 )
{
    addColumn( sName.c_str() );
    connect( this, SIGNAL( clicked( QListViewItem* ) ), this, SLOT( OnClick( QListViewItem* ) ) );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ObjectKnowledgeList destructor
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
MOS_MTH_ObjectKnowledgeList::~MOS_MTH_ObjectKnowledgeList()
{

}



//-----------------------------------------------------------------------------
// Name: MOS_MTH_ObjectKnowledgeList::SetMission
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
void MOS_MTH_ObjectKnowledgeList::SetMission( MOS_Mission_ABC* /*pMission*/ )
{

}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ObjectKnowledgeList::SetWidgetTab
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
void MOS_MTH_ObjectKnowledgeList::SetWidgetTab( QWidget* /*pWidget*/ )
{

}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ObjectKnowledgeList::SetAgent
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
void MOS_MTH_ObjectKnowledgeList::SetAgent( MOS_Agent* pAgent )
{
    if( pAgent_ != 0 )
        pAgent_->GetTeam().NotifyShouldDrawKnowledges( false );

    clear();

    QCheckListItem* pParent = new QCheckListItem( this, "Connaissances", QCheckListItem::Controller );
    pParent->setOpen( true );

    pAgent_ = pAgent;
    const MOS_Agent::T_ObjectKnowledgeMap& knowMap = pAgent->GetTeam().GetObjectKnowledges();
    for( MOS_Agent::CIT_ObjectKnowledgeMap it = knowMap.begin(); it != knowMap.end(); ++it )
    {
        MOS_ObjectKnowledge& objectKnowledge = *it->second;

        std::string strTitle = "Object " + itostring( objectKnowledge.GetID() );

        if( pASNKnowledgeObject_ != 0 )
            new MOS_MTH_ObjectKnowledgeListItem( *it->second, pParent, QCheckListItem::RadioButton, strTitle );
        else
            new MOS_MTH_ObjectKnowledgeListItem( *it->second, pParent, QCheckListItem::CheckBox, strTitle );
    }

    pAgent_->GetTeam().NotifyShouldDrawKnowledges( true );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ObjectKnowledgeList::WriteMsg
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
void MOS_MTH_ObjectKnowledgeList::WriteMsg( std::string& sParam )
{
    if( pASNKnowledgeObject_ != 0 )
    {
        QListViewItem* pItem = this->firstChild()->firstChild();
        while( pItem != 0 )
        {
            MOS_MTH_ObjectKnowledgeListItem* pKnow = static_cast< MOS_MTH_ObjectKnowledgeListItem* >( pItem );
            if( pKnow->isOn() )
            {
                (*pASNKnowledgeObject_) = pKnow->GetMOSID();
                sParam += MT_FormatString( "KnowId:%d | ", pKnow->GetMOSID() );
                break;
            }
            pItem = pItem->nextSibling();
        }
    }
    else
    {
        assert( pASNKnowledgeObjectList_ != 0 );
        QListViewItem* pItem = this->firstChild()->firstChild();
        T_ObjectKnowledgeVector result;

        while( pItem != 0 )
        {
            MOS_MTH_ObjectKnowledgeListItem* pKnow = static_cast< MOS_MTH_ObjectKnowledgeListItem* >( pItem );
            if( pKnow->isOn() )
                result.push_back( & pKnow->GetRepresented() );
            pItem = pItem->nextSibling();
        }

        sParam += MT_FormatString( "NbKnow:%d | ", result.size() );

        pASNKnowledgeObjectList_->n = result.size();
        pASNKnowledgeObjectList_->elem = new ASN1T_OID[ result.size() ];

        for( IT_ObjectKnowledgeVector it = result.begin(); it != result.end(); ++it )
        {
            pASNKnowledgeObjectList_->elem[it - result.begin()] = (*it)->GetID();
            sParam += MT_FormatString( "KnowId:%d | ", (*it)->GetID() );
        }
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_ObjectKnowledgeList::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_ObjectKnowledgeList::FillRandomParameters()
{
    uint nInfo = 0;
    QListViewItem* pItem = this->firstChild()->firstChild();
    while( pItem != 0 )
    {
        ++nInfo;
        MOS_MTH_ObjectKnowledgeListItem* pKnow = static_cast< MOS_MTH_ObjectKnowledgeListItem* >( pItem );
        pKnow->setOn( false );
        pItem = pItem->nextSibling();
    }

    if( pASNKnowledgeObject_ != 0 )
    {
        if ( nInfo != 0 )
        {
            uint nSelected = rand() % nInfo;
            pItem = this->firstChild()->firstChild();
            uint i = 0;
            while( pItem != 0 )
            {
                MOS_MTH_ObjectKnowledgeListItem* pKnow = static_cast< MOS_MTH_ObjectKnowledgeListItem* >( pItem );
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
                MOS_MTH_ObjectKnowledgeListItem* pKnow = static_cast< MOS_MTH_ObjectKnowledgeListItem* >( pItem );
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
// Name: MOS_MTH_ObjectKnowledgeList::ResaveParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_ObjectKnowledgeList::ResaveParameters( MT_OutputArchive_ABC& archive )
{
    uint nInfo = 0;
    QListViewItem* pItem = this->firstChild()->firstChild();
    while( pItem != 0 )
    {
        ++nInfo;
        pItem = pItem->nextSibling();
    }

    if( pASNKnowledgeObject_ != 0 )
    {
        if ( nInfo != 0 )
        {
            uint nSelected = 0;
            pItem = this->firstChild()->firstChild();
            uint i = 0;
            while( pItem != 0 )
            {
                MOS_MTH_ObjectKnowledgeListItem* pKnow = static_cast< MOS_MTH_ObjectKnowledgeListItem* >( pItem );
                if ( pKnow->isOn() )
                {
                    nSelected = i;
                    archive.WriteField( "sName", std::string( "MOS_MTH_ObjectKnowledgeList|nSelected" ) );
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
            MOS_MTH_ObjectKnowledgeListItem* pKnow = static_cast< MOS_MTH_ObjectKnowledgeListItem* >( pItem );
            if ( pKnow->isOn() )
                nNbToSelected++;

            pItem = pItem->nextSibling();
        }
        archive.WriteField( "sName", std::string( "MOS_MTH_ObjectKnowledgeList|nNbToSelected" ) );
        archive.WriteField( "nNbToSelected", nNbToSelected );

        uint i = 0;
        pItem = this->firstChild()->firstChild();
        while( pItem != 0 )
        {
            MOS_MTH_ObjectKnowledgeListItem* pKnow = static_cast< MOS_MTH_ObjectKnowledgeListItem* >( pItem );
            if ( pKnow->isOn() )
            {
                uint nIdOn = i;
                archive.WriteField( "sName", std::string( "MOS_MTH_ObjectKnowledgeList|nIdOn" ) );
                archive.WriteField( "nIdOn", nIdOn );
                ++i;
            }
            pItem = pItem->nextSibling();
        }
    }

}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_ObjectKnowledgeList::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_ObjectKnowledgeList::ReloadParameters( MT_InputArchive_ABC& archive )
{
    uint nInfo = 0;
    QListViewItem* pItem = this->firstChild()->firstChild();
    while( pItem != 0 )
    {
        ++nInfo;
        MOS_MTH_ObjectKnowledgeListItem* pKnow = static_cast< MOS_MTH_ObjectKnowledgeListItem* >( pItem );
        pKnow->setOn( false );
        pItem = pItem->nextSibling();
    }

    if( pASNKnowledgeObject_ != 0 )
    {
        if ( nInfo != 0 )
        {
            uint nSelected = 0; //rand() % nInfo;
// -------------------------------------------------
        std::string sName;
        archive.ReadField( "sName", sName );
        assert( sName == "MOS_MTH_ObjectKnowledgeList|nSelected" );
// -------------------------------------------------
            archive.ReadField( "nSelected", nSelected );

            pItem = this->firstChild()->firstChild();
            uint i = 0;
            while( pItem != 0 )
            {
                MOS_MTH_ObjectKnowledgeListItem* pKnow = static_cast< MOS_MTH_ObjectKnowledgeListItem* >( pItem );
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
        assert( sName == "MOS_MTH_ObjectKnowledgeList|nNbToSelected" );
// -------------------------------------------------
        archive.ReadField( "nNbToSelected", nNbToSelected );

        uint i = 0;
        while ( i < nNbToSelected )
        {
// -------------------------------------------------
            std::string sName;
            archive.ReadField( "sName", sName );
            assert( sName == "MOS_MTH_ObjectKnowledgeList|nIdOn" );
            uint nIdOn = 0;
            archive.ReadField( "nIdOn", nIdOn );
// -------------------------------------------------

            uint j = 0;
            pItem = this->firstChild()->firstChild();
            while( pItem != 0 )
            {
                MOS_MTH_ObjectKnowledgeListItem* pKnow = static_cast< MOS_MTH_ObjectKnowledgeListItem* >( pItem );
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
// Name: MOS_MTH_ObjectKnowledgeList::NotifyAgentHasMoved
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
void MOS_MTH_ObjectKnowledgeList::NotifyAgentHasMoved( MOS_Agent& /*agent*/, const MT_Vector2D& /*vPos*/ )
{

}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ObjectKnowledgeList::Initialize
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
void MOS_MTH_ObjectKnowledgeList::Initialize()
{

}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ObjectKnowledgeList::Terminate
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
void MOS_MTH_ObjectKnowledgeList::Terminate()
{
    if( pAgent_ != 0 )
        pAgent_->GetTeam().NotifyShouldDrawKnowledges( false );
    if( pASNKnowledgeObjectList_ && pASNKnowledgeObjectList_->n > 0 )
        delete [] pASNKnowledgeObjectList_->elem; //$$$ TMP
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ObjectKnowledgeList::OnClick
// Created: AGN 03-04-24
//-----------------------------------------------------------------------------
// slot
void MOS_MTH_ObjectKnowledgeList::OnClick( QListViewItem* pItem )
{
    QCheckListItem* pTest = static_cast< QCheckListItem* >( pItem );
    if( pTest == 0 || pTest->type() == QCheckListItem::Controller )
        return;

    MOS_MTH_ObjectKnowledgeListItem* pCastedItem = static_cast< MOS_MTH_ObjectKnowledgeListItem* >( pItem );
    if( pCastedItem == 0 )
        return;

    MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().SetWantedCenter( pCastedItem->GetRepresented().GetCenter() );
}

