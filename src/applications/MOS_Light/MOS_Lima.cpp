//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Lima.cpp $
// $Author: Nld $
// $Modtime: 19/01/05 16:57 $
// $Revision: 20 $
// $Workfile: MOS_Lima.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_Lima.h"

#ifndef MOS_USE_INLINE
#   include "MOS_Lima.inl"
#endif

#include "MOS_LimaListView.h"
#include "MOS_LimaListViewItem.h"
#include "MOS_LimaCheckListItem.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_AgentServerMsgMgr.h"
#include "MOS_AgentServerController.h"
#include "MOS_MOSServer.h"
#include "MOS_LineManager.h"
#include "MOS_World.h"
#include "MOS_AgentServerConnectionMgr.h"
#include "MOS_ASN_Tools.h"
#include "MOS_ASN_Messages.h"

using namespace DIN;

MOS_IDManager MOS_Lima::idManager_( 137 );

//-----------------------------------------------------------------------------
// Name: MOS_Lima constructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
MOS_Lima::MOS_Lima( bool bEditable )
    : nID_               ( idManager_.GetFreeIdentifier() )
    , nStateLima_       ( eStateLimaCreated )
    , nStateNetworkLima_( eStateLimaNotRegistered )
    , bEditable_        ( bEditable )
    , nFuncType_        ( eLimaFuncLCA )
    , bCreatedByMOS_     ( true )
{
    SetDefaultName();
    MOS_App::GetApp().GetLineManager().RegisterListViewsForLima( *this );
    MOS_App::GetApp().GetLineManager().RegisterLima( *this );

    
    for ( IT_LimaItemMap itItem = limaItemMap_.begin() ; itItem != limaItemMap_.end() ; ++itItem )
        itItem->second = itItem->first->CreateLima( *this );
}

// -----------------------------------------------------------------------------
// Name: MOS_Lima constructor
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
MOS_Lima::MOS_Lima( const ASN1T_MsgLimaCreation& asnMsg )
    : nID_              ( asnMsg.oid  )
    , nStateLima_       ( eStateLimaOk )
    , nStateNetworkLima_( eStateLimaRegistered )
    , bEditable_        ( false )
    , nFuncType_        ( (E_FuncLimaType)asnMsg.fonction )
    , bCreatedByMOS_    ( false )
{
    SetDefaultName();
    MOS_App::GetApp().GetLineManager().RegisterListViewsForLima( *this );
    MOS_App::GetApp().GetLineManager().RegisterLima( *this );

    
    assert( asnMsg.geometrie.type == EnumTypeLocalisation::line );
    for( uint i = 0; i != asnMsg.geometrie.vecteur_point.n ; ++i )
    {
        MT_Vector2D vTmp;
        MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.geometrie.vecteur_point.elem[i].data, vTmp );
        CreatePoint( vTmp );
    }

    for ( IT_LimaItemMap itItem = limaItemMap_.begin() ; itItem != limaItemMap_.end() ; ++itItem )
        itItem->second = itItem->first->CreateLima( *this );
}

//-----------------------------------------------------------------------------
// Name: MOS_Lima destructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
MOS_Lima::~MOS_Lima()
{
    MOS_App::GetApp().GetLineManager().UnregisterLima( *this );
    DeleteListView();

    if( bCreatedByMOS_ )
        idManager_.ReleaseIdentifier( nID_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_Lima::RegisterListViewItem
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_Lima::RegisterListViewItem( MOS_LimaListView* pListView, QListViewItem* pItem )
{
    IT_LimaItemMap itItem = limaItemMap_.find( pListView );
    assert( itItem != limaItemMap_.end() );
    itItem->second = pItem;
    
}


//-----------------------------------------------------------------------------
// Name: MOS_Lima::UnregisterListviewItem
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_Lima::UnregisterListViewItem( MOS_LimaListView* pListView, QListViewItem* pItem )
{
    IT_LimaItemMap itItem = limaItemMap_.find( pListView );
    assert( itItem != limaItemMap_.end() );
    assert( itItem->second == pItem );
    itItem->second = 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_Lima::SetDefaultName
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_Lima::SetDefaultName()
{
    sName_ = QString( "Lima %1" ).arg(  MOS_App::GetApp().GetLineManager().GetLimaList().size() );
}
//-----------------------------------------------------------------------------
// Name: MOS_Lima::RegisterListView
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
void MOS_Lima::RegisterListView( MOS_LimaListView& listview )
{
    limaItemMap_.insert( std::make_pair( &listview, (QListViewItem*)0 ) );
}


//-----------------------------------------------------------------------------
// Name: MOS_Lima::UnregisterListView
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
void MOS_Lima::UnregisterListView( MOS_LimaListView& listview )
{
    IT_LimaItemMap itItem = limaItemMap_.find( &listview );
    assert( itItem != limaItemMap_.end() );
    limaItemMap_.erase( itItem );
}

//-----------------------------------------------------------------------------
// Name: MOS_Lima::AddPoint
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
const MT_Vector2D& MOS_Lima::CreatePoint( const MT_Vector2D& vPos, MOS_PointListViewItem* pPointAfter )
{
    MT_Vector2D* pPoint = new MT_Vector2D( vPos );
    RegisterPoint( *pPoint );
    for ( IT_LimaItemMap itItem = limaItemMap_.begin() ; itItem != limaItemMap_.end() ; ++itItem  )
    {
        if ( itItem->first->GetType() == eListViewSelector )
        {
            QListViewItem* pItem = itItem->second;
            if( pItem )
            {
                MOS_LimaCheckListItem* pLima = (MOS_LimaCheckListItem*)pItem;
                if ( pLima )
                    pLima->CreatePoint( *pPoint, pPointAfter );
            }
        }
        else
        {
            QListViewItem* pItem = itItem->second;
            if( pItem )
            {
                MOS_LimaListViewItem* pLima = (MOS_LimaListViewItem*)pItem;
                if ( pLima )
                    pLima->CreatePoint( *pPoint, pPointAfter );
            }
            
        }
    }
    return *pPoint;
}


//-----------------------------------------------------------------------------
// Name: MOS_Lima::MOS_LineManager
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
bool MOS_Lima::UpdateLimaToSim()
{
    if ( !MOS_App::GetApp().GetMOSServer().GetController().GetConnectionMgr().IsConnectedToAgentServer() )
        return true;

    uint i;
    switch( nStateLima_ )
    {
        case eStateLimaCreated:
        {
            assert( !pointVector_.empty() );
            MOS_ASN_MsgLimaCreation asnMsg;

            asnMsg.GetAsnMsg().oid                             = nID_;
            asnMsg.GetAsnMsg().fonction                        = (ASN1T_EnumTypeLima)nFuncType_;
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

            SetNetworkState( eStateLimaRegistering );

            delete[] asnMsg.GetAsnMsg().geometrie.vecteur_point.elem;
        }
        break;

        case eStateLimaCoordModified:
        {
            MOS_ASN_MsgLimaUpdate asnMsg;

            asnMsg.GetAsnMsg().oid                             = nID_;
            asnMsg.GetAsnMsg().fonction                        = (ASN1T_EnumTypeLima)nFuncType_;
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

            SetNetworkState( eStateLimaRegistering );

            delete[] asnMsg.GetAsnMsg().geometrie.vecteur_point.elem;


/*
            DIN_BufferedMessage msg = lineMgr.PrebuildMsgLima();
            msg << (MIL_MOSContextID)this; 
            msg << GetID();
            msg << sName_;

            msg << (uint8)pointVector_.size();
            for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
            {
                std::string sMGRS("");
                const MT_Vector2D& vPos = **itPoint;
                MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( vPos, sMGRS );
                msg << ConvertPositionValue( sMGRS );
            }
            lineMgr.SendMsgLimaChangeRequest( msg );
            SetNetworkState( eStateLimaRegistering );
            */
        }
        break;

        case eStateLimaDeleted:
        {
            MOS_ASN_MsgLimaDestruction asnMsg;

            asnMsg.GetAsnMsg() = nID_;
            asnMsg.Send( (MIL_MOSContextID)this );
            SetNetworkState( eStateLimaRegistering );

/*            DIN_BufferedMessage msg = lineMgr.PrebuildMsgLima();
            msg << (MIL_MOSContextID)this; 
            msg << GetID();
            lineMgr.SendMsgLimaDestructionRequest( msg );
            SetNetworkState( eStateLimaRegistering );*/
        }
        break;

        case eStateLimaOk:
        default:
            break;
    }

    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_Lima::DeleteAllPoints
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
void MOS_Lima::DeleteAllPoints()
{
    IT_PointPtrVector itPoint = pointVector_.begin();
    while ( itPoint != pointVector_.end() )
    {
        DeletePoint( **itPoint );
        itPoint = pointVector_.begin();
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_Lima::DelPoint
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_Lima::DeletePoint( MT_Vector2D& vPoint )
{
    IT_PointPtrVector itPoint = std::find( pointVector_.begin() , pointVector_.end() , &vPoint );
    assert( itPoint != pointVector_.end() );

    for ( IT_LimaItemMap itItem = limaItemMap_.begin() ; itItem != limaItemMap_.end() ; ++itItem  )
    {
        if ( itItem->first->GetType() == eListViewSelector )
        {
            MOS_LimaCheckListItem* pLima = static_cast< MOS_LimaCheckListItem* >( itItem->second );
            if ( pLima )
                pLima->DeletePoint( vPoint );
        }
        else
        {
            MOS_LimaListViewItem* pLima = static_cast< MOS_LimaListViewItem* >( itItem->second );
            if ( pLima  )
                pLima->DeletePoint( vPoint );
        }
    }
    
    delete *itPoint;
    pointVector_.erase( itPoint );
}
//-----------------------------------------------------------------------------
// Name: MOS_Lima::InsertPoint
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
const MT_Vector2D& MOS_Lima::InsertPoint( const MT_Vector2D& vPosInsert, const MT_Vector2D& vPos )
{
    IT_PointPtrVector itPoint = std::find( pointVector_.begin() , pointVector_.end(), &vPosInsert );
    assert( itPoint != pointVector_.end() );
    MT_Vector2D* pPoint = new MT_Vector2D( vPos );
    pointVector_.insert( itPoint , pPoint );
    return *pPoint;
}


//-----------------------------------------------------------------------------
// Name: MOS_Lima::SetName
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------
void MOS_Lima::SetName( const std::string sText )
{
    sName_ = sText;
    for ( IT_LimaItemMap itItem = limaItemMap_.begin() ; itItem != limaItemMap_.end() ; ++itItem  )
    {
        if ( itItem->first->GetType() == eListViewSelector )
        {
            MOS_LimaCheckListItem* pLima = static_cast< MOS_LimaCheckListItem* >( itItem->second );
            if ( pLima )
                pLima->SetName( sName_ );
        }
        else
        {
            MOS_LimaListViewItem* pLima = static_cast< MOS_LimaListViewItem* >( itItem->second );
            if ( pLima )
                pLima->SetName( sName_ );
            
        }
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_Lima::SetState
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------

void MOS_Lima::SetState( E_StateLima nStateLima )
{
    if ( nStateLima_ == eStateLimaCreated )
    {
        if ( nStateLima == eStateLimaDeleted )
            delete this;
        else
        if ( nStateLima == eStateLimaOk ) 
            nStateLima_ = nStateLima;
        return;
    }

    nStateLima_ = nStateLima;
    if ( nStateLima_ == eStateLimaDeleted )
        DeleteListView();
    else
    if ( nStateLima_ == eStateLimaCoordModified )
    {
        for ( IT_LimaItemMap itItem = limaItemMap_.begin() ; itItem != limaItemMap_.end() ; ++itItem  )
        {
            if ( itItem->first->GetType() == eListViewSelector )
            {
                MOS_LimaCheckListItem* pLima = static_cast< MOS_LimaCheckListItem* >( itItem->second );
                if ( pLima )
                    pLima->UpdateNamePos();
            }
            else
            {
                MOS_LimaListViewItem* pLima = static_cast< MOS_LimaListViewItem* >( itItem->second );
                if ( pLima )
                    pLima->UpdateNamePos();
            }
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_Lima::GetLimaItem
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
QListViewItem* MOS_Lima::GetItem( MOS_LimaListView& listView )
{
    IT_LimaItemMap itItem = limaItemMap_.find( &listView );
    assert( itItem != limaItemMap_.end() );
    return itItem->second;
}

//-----------------------------------------------------------------------------
// Name: MOS_Lima::ConvertPositionValue
// Created: NLD 2002-08-06
//-----------------------------------------------------------------------------
std::string& MOS_Lima::ConvertPositionValue( std::string& strValue )
{
    strValue.erase( std::find( strValue.begin(), strValue.end(), ' ' ), strValue.end() );
    assert( strValue.size() == 15 );
    return strValue;
}
//-----------------------------------------------------------------------------
// Name: MOS_Lima::DeleteListView
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
void MOS_Lima::DeleteListView()
{
    for ( IT_LimaItemMap itItem = limaItemMap_.begin() ; itItem != limaItemMap_.end() ; ++itItem  )
    {
        if ( itItem->first->GetType() == eListViewSelector )
        {
            MOS_LimaCheckListItem* pLima = static_cast< MOS_LimaCheckListItem* >( itItem->second );
            if ( pLima )
                itItem->first->DeleteLima( *pLima );
        }
        else
        {
            MOS_LimaListViewItem* pLima = static_cast< MOS_LimaListViewItem* >( itItem->second );
            if ( pLima )
                itItem->first->DeleteLima( *pLima );
        }
        itItem->second = 0;
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_Lima::DeleteListView
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
void MOS_Lima::DeleteListView( MOS_LimaListView& listView )
{
    IT_LimaItemMap itItem = limaItemMap_.find( &listView );
    if ( itItem != limaItemMap_.end() )
    {
        if ( itItem->first->GetType() == eListViewSelector )
        {
            MOS_LimaCheckListItem* pLima = static_cast< MOS_LimaCheckListItem* >( itItem->second );
            if ( pLima )
                itItem->first->DeleteLima( *pLima );
        }
        else
        {
            MOS_LimaListViewItem* pLima = static_cast< MOS_LimaListViewItem* >( itItem->second );
            if ( pLima )
                itItem->first->DeleteLima( *pLima );
        }
        itItem->second = 0;
    }
    UnregisterListView( listView );
}


//-----------------------------------------------------------------------------
// Name: MOS_Lima::Read
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
int MOS_Lima::Read( MT_InputArchive_ABC& archive )
{
    int nOldID = 0;
    archive.ReadField( "ID", nOldID );

    int nTmp = 0;
    archive.ReadField( "Type", nTmp );
    printf( "Type : %d\n", nTmp );
    nFuncType_ = (E_FuncLimaType)nTmp;
    for ( IT_LimaItemMap itItem = limaItemMap_.begin() ; itItem != limaItemMap_.end() ; ++itItem  )
    {
        if ( itItem->first->GetType() == eListViewSelector )
        {
//            QListViewItem* pItem = itItem->second;
//            if( pItem )
//            {
//                MOS_LimaCheckListItem* pLima = (MOS_LimaCheckListItem*)pItem;
//                if ( pLima )
//                    pLima-> CreatePoint( *pPoint, pPointAfter );
//            }
        }
        else
        {
            QListViewItem* pItem = itItem->second;
            if( pItem )
            {
                MOS_LimaListViewItem* pLima = (MOS_LimaListViewItem*)pItem;
                if ( pLima )
                    pLima->SetFuncState( nFuncType_ );
            }
            
        }
    }

    
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
// Name: MOS_Lima::Write
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
int MOS_Lima::Write( MT_OutputArchive_ABC& archive )
{
    archive.WriteField( "ID", (int)nID_ );
    archive.WriteField( "Type", (int)nFuncType_ );
    
    uint nPoints = pointVector_.size();
    archive.WriteField( "nPoints", (int)nPoints );
    assert( nPoints != 0 );

    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        const MT_Vector2D& vPos = **itPoint;
        vPos.Write( archive );
    }
    return 0;
}



//-----------------------------------------------------------------------------
// Name: MOS_Lima::GetTypeFunc
// Created: FBD 03-02-28
//-----------------------------------------------------------------------------
E_FuncLimaType MOS_Lima::GetTypeFunc() const
{
    return nFuncType_;
}

//-----------------------------------------------------------------------------
// Name: MOS_Lima::SetTypeFunc
// Created: FBD 03-02-28
//-----------------------------------------------------------------------------
void MOS_Lima::SetTypeFunc( E_FuncLimaType nTypeFunc )
{
    nFuncType_ = nTypeFunc;
}


//-----------------------------------------------------------------------------
// Name: MOS_Lima::SetTypeFunc
// Created: FBD 03-02-28
//-----------------------------------------------------------------------------
/*ASN1T_EnumTypeLigneAlat MOS_Lima::ConvertSimLimaFunctionToMos() const
{
    switch( nFuncType_ )
    {
        case eLimaFuncLD  : return EnumTypeLigneAlat::ld;
        case eLimaFuncLCA : return EnumTypeLigneAlat::lbond;
        case eLimaFuncLC  : return EnumTypeLigneAlat::fscl; 
        case eLimaFuncLI  : return EnumTypeLigneAlat::larr;
        case eLimaFuncLO  : return EnumTypeLigneAlat::lobjint;  
    }
    assert( false );
    return EnumTypeLigneAlat::none;
}*/
