//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Limit.cpp $
// $Author: Ape $
// $Modtime: 26/01/05 16:03 $
// $Revision: 8 $
// $Workfile: MOS_Limit.cpp $
//
//*****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_Limit.h"

#include "MOS_App.h"
#include "MOS_AgentServerMsgMgr.h"
#include "MOS_AgentServerController.h"
#include "MOS_AgentServerConnectionMgr.h"
#include "MOS_ASN_Messages.h"
#include "MOS_MOSServer.h"
#include "MOS_LineManager.h"
#include "MOS_World.h"

MOS_IDManager MOS_Limit::idManager_( 138 );

//-----------------------------------------------------------------------------
// Name: MOS_Limit constructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
MOS_Limit::MOS_Limit()
    : MOS_TacticalLine_ABC()
{
    nID_ = idManager_.GetFreeIdentifier();
    strName_ = QString( "Limit %1" ).arg(  MOS_App::GetApp().GetLineManager().GetLineList().size() );
}


// -----------------------------------------------------------------------------
// Name: MOS_Limit constructor
/** @param  pointList 
*/
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
MOS_Limit::MOS_Limit( T_PointVector pointList )
    : MOS_TacticalLine_ABC()
{
    pointList_ = pointList;
    nID_ = idManager_.GetFreeIdentifier();
    strName_ = QString( "Limit %1" ).arg(  MOS_App::GetApp().GetLineManager().GetLineList().size() );
}


//-----------------------------------------------------------------------------
// Name: MOS_Limit constructor
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
MOS_Limit::MOS_Limit( const ASN1T_MsgLimitCreation& asnMsg )
    : MOS_TacticalLine_ABC()
{
    nID_ = asnMsg.oid;
    idManager_.LockIdentifier( nID_ );
    nState_ = eStateOk;
    nNetworkState_ = eNetworkStateRegistered;
    bCreatedByMOS_ = false;

    strName_ = QString( "Limit %1" ).arg(  MOS_App::GetApp().GetLineManager().GetLineList().size() );

    assert( asnMsg.geometrie.type == EnumTypeLocalisation::line );
    for( uint i = 0; i != asnMsg.geometrie.vecteur_point.n ; ++i )
    {
        MT_Vector2D vTmp;
        MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.geometrie.vecteur_point.elem[i].data, vTmp );
        pointList_.push_back( vTmp );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_Limit destructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
MOS_Limit::~MOS_Limit()
{
//    if( bCreatedByMOS_ )
    idManager_.ReleaseIdentifier( nID_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_Limit::UpdateToSim
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
bool MOS_Limit::UpdateToSim()
{
    if ( !MOS_App::GetApp().GetMOSServer().GetController().GetConnectionMgr().IsConnectedToAgentServer() )
        return false;

    uint i;

    switch( nState_ )
    {
        case eStateCreated:
        {
            assert( !pointList_.empty() );
        
            MOS_ASN_MsgLimitCreation asnMsg;

            asnMsg.GetAsnMsg().oid                          = nID_;
            asnMsg.GetAsnMsg().level                        = EnumNatureLevel::ooo;
            asnMsg.GetAsnMsg().geometrie.type               = EnumTypeLocalisation::line;
            asnMsg.GetAsnMsg().geometrie.vecteur_point.n    = pointList_.size();
            asnMsg.GetAsnMsg().geometrie.vecteur_point.elem = new ASN1T_CoordUTM[ pointList_.size() ];

            i = 0;
            for ( CIT_PointVector itPoint = pointList_.begin() ; itPoint != pointList_.end() ; ++itPoint )
            {
                std::string strMGRS;
                const MT_Vector2D& vPos = *itPoint;
                MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( vPos, strMGRS );
                asnMsg.GetAsnMsg().geometrie.vecteur_point.elem[i] = strMGRS.c_str();
                ++i;
            }

            asnMsg.Send( (MIL_MOSContextID)this );

            nNetworkState_ = eNetworkStateRegistering;

            delete[] asnMsg.GetAsnMsg().geometrie.vecteur_point.elem;
        }
        break;

        case eStateModified:
        {
            assert( !pointList_.empty() );

            MOS_ASN_MsgLimitUpdate asnMsg;

            asnMsg.GetAsnMsg().oid                          = nID_;
            asnMsg.GetAsnMsg().level                        = EnumNatureLevel::ooo;
            asnMsg.GetAsnMsg().geometrie.type               = EnumTypeLocalisation::line;
            asnMsg.GetAsnMsg().geometrie.vecteur_point.n    = pointList_.size();
            asnMsg.GetAsnMsg().geometrie.vecteur_point.elem = new ASN1T_CoordUTM[ pointList_.size() ];

            i = 0;
            for ( CIT_PointVector itPoint = pointList_.begin() ; itPoint != pointList_.end() ; ++itPoint )
            {
                std::string strMGRS;
                const MT_Vector2D& vPos = *itPoint;
                MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( vPos, strMGRS );
                asnMsg.GetAsnMsg().geometrie.vecteur_point.elem[i] = strMGRS.c_str();
                ++i;
            }

            asnMsg.Send( (MIL_MOSContextID)this );

            nNetworkState_ = eNetworkStateRegistering;

            delete[] asnMsg.GetAsnMsg().geometrie.vecteur_point.elem;
        }
        break;

        case eStateDeleted:
        {
            MOS_ASN_MsgLimitDestruction asnMsg;

            asnMsg.GetAsnMsg() = nID_;
            asnMsg.Send( (MIL_MOSContextID)this );
            nNetworkState_ = eNetworkStateRegistering;

            std::stringstream strMsg;
            strMsg << "Demande destruction " << strName_;
            MT_LOG_INFO( strMsg.str().c_str(), eSent, 0 );
        }
        break;

        case eStateOk:
        default:
            break;
    }

    return true;
}


// -----------------------------------------------------------------------------
// Name: MOS_Limit::Read
/** @param  archive 
*/
// Created: APE 2004-07-26
// -----------------------------------------------------------------------------
void MOS_Limit::Read( MT_InputArchive_ABC& archive )
{
    archive.Section( "Limit" );
    MOS_TacticalLine_ABC::Read( archive );
    archive.EndSection();
}


// -----------------------------------------------------------------------------
// Name: MOS_Limit::Write
/** @param  archive 
*/
// Created: APE 2004-07-26
// -----------------------------------------------------------------------------
void MOS_Limit::Write( MT_OutputArchive_ABC& archive ) const
{
    archive.Section( "Limit" );
    MOS_TacticalLine_ABC::Write( archive );
    archive.EndSection();
}
