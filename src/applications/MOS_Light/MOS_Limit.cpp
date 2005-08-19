//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Limit.cpp $
// $Author: Nld $
// $Modtime: 23/04/04 11:23 $
// $Revision: 17 $
// $Workfile: MOS_Limit.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_Limit.h"

#ifndef MOS_USE_INLINE
#   include "MOS_Limit.inl"
#endif

#include "MOS_LimitListView.h"
#include "MOS_LimitListViewItem.h"
#include "MOS_LimitCheckListItem.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_AgentServerMsgMgr.h"
#include "MOS_AgentServerController.h"
#include "MOS_MOSServer.h"
#include "MOS_LineManager.h"
#include "MOS_World.h"

#include "MOS_AgentServerConnectionMgr.h"

#include "MOS_ASN_Messages.h"
#include "MOS_IDManager.h"

using namespace DIN;


MOS_IDManager MOS_Limit::idManager_( 138 );

//-----------------------------------------------------------------------------
// Name: MOS_Limit constructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
MOS_Limit::MOS_Limit( bool bEditable )
    : nID_               ( idManager_.GetFreeIdentifier() )
    , nStateLimit_       ( eStateLimitCreated )
    , nStateNetworkLimit_( eStateLimitNotRegistered )
    , bEditable_         ( bEditable )
    , bCreatedByMOS_     ( true )
    , bLocal_            ( true )
{
    SetDefaultName();
    MOS_App::GetApp().GetLineManager().RegisterListViewsForLimit( *this );
    MOS_App::GetApp().GetLineManager().RegisterLimit( *this );

    for ( IT_LimitItemMap itItem = limitItemMap_.begin() ; itItem != limitItemMap_.end() ; ++itItem )
        itItem->second = itItem->first->CreateLimit( *this );
}


//-----------------------------------------------------------------------------
// Name: MOS_Limit constructor
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
MOS_Limit::MOS_Limit( const ASN1T_MsgLimitCreation& asnMsg )
    : nID_               ( asnMsg.oid )
    , nStateLimit_       ( eStateLimitOk )
    , nStateNetworkLimit_( eStateLimitRegistered )
    , bEditable_         ( false )
    , bCreatedByMOS_     ( false )
    , bLocal_            ( false )
{
    SetDefaultName();
    MOS_App::GetApp().GetLineManager().RegisterListViewsForLimit( *this );
    MOS_App::GetApp().GetLineManager().RegisterLimit( *this );

    assert( asnMsg.geometrie.type == EnumTypeLocalisation::line );
    for( uint i = 0; i != asnMsg.geometrie.vecteur_point.n ; ++i )
    {
        MT_Vector2D vTmp;
        MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.geometrie.vecteur_point.elem[i].data, vTmp );
        CreatePoint( vTmp );
    }

    for ( IT_LimitItemMap itItem = limitItemMap_.begin() ; itItem != limitItemMap_.end() ; ++itItem )
        itItem->second = itItem->first->CreateLimit( *this );
}

//-----------------------------------------------------------------------------
// Name: MOS_Limit destructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
MOS_Limit::~MOS_Limit()
{
    MOS_App::GetApp().GetLineManager().UnregisterLimit( *this );
    DeleteListView();

    if( bCreatedByMOS_ )
        idManager_.ReleaseIdentifier( nID_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_Limit::RegisterListViewItem
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_Limit::RegisterListViewItem( MOS_LimitListView* pListView, QListViewItem* pItem )
{
    IT_LimitItemMap itItem = limitItemMap_.find( pListView );
    assert( itItem != limitItemMap_.end() );
    itItem->second = pItem;
    
}


//-----------------------------------------------------------------------------
// Name: MOS_Limit::UnregisterListviewItem
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_Limit::UnregisterListViewItem( MOS_LimitListView* pListView, QListViewItem* pItem )
{
    IT_LimitItemMap itItem = limitItemMap_.find( pListView );
    assert( itItem != limitItemMap_.end() );
    assert( itItem->second == pItem );
    itItem->second = 0;
}

//-----------------------------------------------------------------------------
// Name: MOS_Limit::SetDefaultName
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_Limit::SetDefaultName()
{
    sName_ = QString( "Limit %1" ).arg(  MOS_App::GetApp().GetLineManager().GetLimitList().size() );
}
//-----------------------------------------------------------------------------
// Name: MOS_Limit::RegisterListView
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
void MOS_Limit::RegisterListView( MOS_LimitListView& listview )
{
    limitItemMap_.insert( std::make_pair( &listview, (QListViewItem*)0 ) );
}


//-----------------------------------------------------------------------------
// Name: MOS_Limit::UnregisterListView
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
void MOS_Limit::UnregisterListView( MOS_LimitListView& listview )
{
    IT_LimitItemMap itItem = limitItemMap_.find( &listview );
    assert( itItem != limitItemMap_.end() );
    limitItemMap_.erase( itItem );
}

//-----------------------------------------------------------------------------
// Name: MOS_Limit::AddPoint
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
const MT_Vector2D& MOS_Limit::CreatePoint( const MT_Vector2D& vPos, MOS_PointListViewItem* pPointAfter )
{
    MT_Vector2D* pPoint = new MT_Vector2D( vPos );
    RegisterPoint( *pPoint );
    for ( IT_LimitItemMap itItem = limitItemMap_.begin() ; itItem != limitItemMap_.end() ; ++itItem  )
    {
        if ( itItem->first->GetType() == eListViewSelector )
        {
            QListViewItem* pItem = itItem->second;
            if( pItem )
            {
                MOS_LimitCheckListItem* pLimit = (MOS_LimitCheckListItem*)pItem;
                if ( pLimit )
                    pLimit->CreatePoint( *pPoint, pPointAfter );
            }
        }
        else
        {
            QListViewItem* pItem = itItem->second;
            if( pItem )
            {
                MOS_LimitListViewItem* pLimit = (MOS_LimitListViewItem*)pItem;
                if ( pLimit )
                    pLimit->CreatePoint( *pPoint, pPointAfter );
            }
            
        }
    }
    return *pPoint;
}


//-----------------------------------------------------------------------------
// Name: MOS_Limit::MOS_LineManager
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
bool MOS_Limit::UpdateLimitToSim()
{
    if ( !MOS_App::GetApp().GetMOSServer().GetController().GetConnectionMgr().IsConnectedToAgentServer() )
        return true;

    uint i;

    switch( nStateLimit_ )
    {
        case eStateLimitCreated:
        {
            assert( !pointVector_.empty() );
        
            MOS_ASN_MsgLimitCreation asnMsg;

            asnMsg.GetAsnMsg().oid                          = nID_;
            asnMsg.GetAsnMsg().level                        = EnumNatureLevel::ooo;
            asnMsg.GetAsnMsg().geometrie.type               = EnumTypeLocalisation::line;
            asnMsg.GetAsnMsg().geometrie.vecteur_point.n    = pointVector_.size();
            asnMsg.GetAsnMsg().geometrie.vecteur_point.elem = new ASN1T_CoordUTM[ pointVector_.size() ];

            i = 0;
            for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
            {
                std::string strMGRS;
                const MT_Vector2D& vPos = **itPoint;
                MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( vPos, strMGRS );
                ConvertPositionValue( strMGRS );
                asnMsg.GetAsnMsg().geometrie.vecteur_point.elem[i] = strMGRS.c_str();
                ++i;
            }

            asnMsg.Send( (MIL_MOSContextID)this );

            SetNetworkState( eStateLimitRegistering );

            delete[] asnMsg.GetAsnMsg().geometrie.vecteur_point.elem;
        }
        break;

        case eStateLimitCoordModified:
        {
            assert( !pointVector_.empty() );

            MOS_ASN_MsgLimitUpdate asnMsg;

            asnMsg.GetAsnMsg().oid                             = nID_;
            asnMsg.GetAsnMsg().geometrie.type               = EnumTypeLocalisation::line;
            asnMsg.GetAsnMsg().geometrie.vecteur_point.n    = pointVector_.size();
            asnMsg.GetAsnMsg().geometrie.vecteur_point.elem = new ASN1T_CoordUTM[ pointVector_.size() ];

            i = 0;
            for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
            {
                std::string strMGRS;
                const MT_Vector2D& vPos = **itPoint;
                MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( vPos, strMGRS );
                ConvertPositionValue( strMGRS );
                asnMsg.GetAsnMsg().geometrie.vecteur_point.elem[i] = strMGRS.c_str();
                ++i;
            }

            asnMsg.Send( (MIL_MOSContextID)this );

            SetNetworkState( eStateLimitRegistering );

            delete[] asnMsg.GetAsnMsg().geometrie.vecteur_point.elem;
        }
        break;

        case eStateLimitDeleted:
        {
            MOS_ASN_MsgLimitDestruction asnMsg;

            asnMsg.GetAsnMsg() = nID_;
            asnMsg.Send( (MIL_MOSContextID)this );
            SetNetworkState( eStateLimitRegistering );
        }
        break;

        case eStateLimitOk:
        default:
            break;
    }

    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_Limit::DeleteAllPoints
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
void MOS_Limit::DeleteAllPoints()
{
    IT_PointPtrVector itPoint = pointVector_.begin();
    while ( itPoint != pointVector_.end() )
    {
        DeletePoint( **itPoint );
        itPoint = pointVector_.begin();
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_Limit::DelPoint
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_Limit::DeletePoint( MT_Vector2D& vPoint )
{
    IT_PointPtrVector itPoint = std::find( pointVector_.begin() , pointVector_.end() , &vPoint );
    assert( itPoint != pointVector_.end() );

    for ( IT_LimitItemMap itItem = limitItemMap_.begin() ; itItem != limitItemMap_.end() ; ++itItem  )
    {
        if ( itItem->first->GetType() == eListViewSelector )
        {
            MOS_LimitCheckListItem* pLimit = static_cast< MOS_LimitCheckListItem* >( itItem->second );
            if ( pLimit )
                pLimit->DeletePoint( vPoint );
        }
        else
        {
            MOS_LimitListViewItem* pLimit = static_cast< MOS_LimitListViewItem* >( itItem->second );
            if ( pLimit )
                pLimit->DeletePoint( vPoint );
        }
    }
    
    delete *itPoint;
    pointVector_.erase( itPoint );
}

//-----------------------------------------------------------------------------
// Name: MOS_Limit::InsertPoint
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
const MT_Vector2D& MOS_Limit::InsertPoint( const MT_Vector2D& vPosInsert, const MT_Vector2D& vPos )
{
    IT_PointPtrVector itPoint = std::find( pointVector_.begin() , pointVector_.end(), &vPosInsert );
    assert( itPoint != pointVector_.end() );
    MT_Vector2D* pPoint = new MT_Vector2D( vPos );
    pointVector_.insert( itPoint , pPoint );
    return *pPoint;
}


//-----------------------------------------------------------------------------
// Name: MOS_Limit::SetName
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------
void MOS_Limit::SetName( const std::string sText )
{
    sName_ = sText;
    for ( IT_LimitItemMap itItem = limitItemMap_.begin() ; itItem != limitItemMap_.end() ; ++itItem  )
    {
        if ( itItem->first->GetType() == eListViewSelector )
        {
            MOS_LimitCheckListItem* pLimit = static_cast< MOS_LimitCheckListItem* >( itItem->second );
            if ( pLimit )
                pLimit->SetName( sName_ );
        }
        else
        {
            MOS_LimitListViewItem* pLimit = static_cast< MOS_LimitListViewItem* >( itItem->second );
            if ( pLimit )
                pLimit->SetName( sName_ );
            
        }
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_Limit::SetState
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------

void MOS_Limit::SetState( E_StateLimit nStateLimit )
{
    if ( nStateLimit_ == eStateLimitCreated  )
    {
        if ( nStateLimit == eStateLimitDeleted )
        {
            delete this;
        }
        else
        if ( nStateLimit == eStateLimitOk ) 
        {
            nStateLimit_ = nStateLimit;
        }
        return;
    }

    nStateLimit_ = nStateLimit;
    if ( nStateLimit_ == eStateLimitDeleted )
        DeleteListView();
    else
    if ( nStateLimit_ == eStateLimitCoordModified )
    {
        for ( IT_LimitItemMap itItem = limitItemMap_.begin() ; itItem != limitItemMap_.end() ; ++itItem  )
        {
            if ( itItem->first->GetType() == eListViewSelector )
            {
                MOS_LimitCheckListItem* pLimit = static_cast< MOS_LimitCheckListItem* >( itItem->second );
                if ( pLimit )
                    pLimit->UpdateNamePos();
            }
            else
            {
                MOS_LimitListViewItem* pLimit = static_cast< MOS_LimitListViewItem* >( itItem->second );
                if ( pLimit )
                    pLimit->UpdateNamePos();
            }
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_Limit::GetLimitItem
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
QListViewItem* MOS_Limit::GetItem( MOS_LimitListView& listView )
{
    IT_LimitItemMap itItem = limitItemMap_.find( &listView );
    assert( itItem != limitItemMap_.end() );
    return itItem->second;
}

//-----------------------------------------------------------------------------
// Name: MOS_Limit::ConvertPositionValue
// Created: NLD 2002-08-06
//-----------------------------------------------------------------------------
std::string& MOS_Limit::ConvertPositionValue( std::string& strValue )
{
    strValue.erase( std::find( strValue.begin(), strValue.end(), ' ' ), strValue.end() );
    assert( strValue.size() == 15 );
    return strValue;
}
//-----------------------------------------------------------------------------
// Name: MOS_Limit::DeleteListView
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
void MOS_Limit::DeleteListView()
{
    for ( IT_LimitItemMap itItem = limitItemMap_.begin() ; itItem != limitItemMap_.end() ; ++itItem  )
    {
        if ( itItem->first->GetType() == eListViewSelector )
        {
            MOS_LimitCheckListItem* pLimit = static_cast< MOS_LimitCheckListItem* >( itItem->second );
            if ( pLimit )
                itItem->first->DeleteLimit( *pLimit );
        }
        else
        {
            MOS_LimitListViewItem* pLimit = static_cast< MOS_LimitListViewItem* >( itItem->second );
            if ( pLimit )
                itItem->first->DeleteLimit( *pLimit );
        }
        itItem->second = 0;
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_Limit::DeleteListView
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
void MOS_Limit::DeleteListView( MOS_LimitListView& listView )
{
    IT_LimitItemMap itItem = limitItemMap_.find( &listView );
    if ( itItem != limitItemMap_.end() )
    {
        if ( itItem->first->GetType() == eListViewSelector )
        {
            MOS_LimitCheckListItem* pLimit = static_cast< MOS_LimitCheckListItem* >( itItem->second );
            if ( pLimit )
                itItem->first->DeleteLimit( *pLimit );
        }
        else
        {
            MOS_LimitListViewItem* pLimit = static_cast< MOS_LimitListViewItem* >( itItem->second );
            if ( pLimit )
                itItem->first->DeleteLimit( *pLimit );
        }
        itItem->second = 0;
    }
    UnregisterListView( listView );
}

//-----------------------------------------------------------------------------
// Name: MOS_Limit::Read
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
int MOS_Limit::Read( MT_InputArchive_ABC& archive )
{
    int nOldID = 0;
    archive.ReadField( "ID", (int)nOldID );
    
    uint nPoints = 0;
    archive.ReadField( "nPoints", nPoints );
    assert( nPoints != 0 );

    for ( uint i = 0 ; i < nPoints ; ++i )
    {
        MT_Vector2D vPos;
        vPos.Read( archive );
        CreatePoint( vPos );
    }
    return 0;

}

//-----------------------------------------------------------------------------
// Name: MOS_Limit::Write
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
int MOS_Limit::Write( MT_OutputArchive_ABC& archive )
{
    archive.WriteField( "ID", (int)nID_ );
    
    uint nPoints = pointVector_.size();
    assert( nPoints != 0 );
    archive.WriteField( "nPoints", (int)nPoints );

    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        const MT_Vector2D& vPos = **itPoint;
        vPos.Write( archive );
    }
    return 0;
}

