//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Lima.cpp $
// $Author: Ape $
// $Modtime: 26/01/05 15:47 $
// $Revision: 8 $
// $Workfile: MOS_Lima.cpp $
//
//*****************************************************************************

#include "MOS_Light2_Pch.h"
#include "MOS_Lima.h"

#include "MOS_App.h"
#include "MOS_AgentServerMsgMgr.h"
#include "MOS_AgentServerController.h"
#include "MOS_AgentServerConnectionMgr.h"
#include "MOS_ASN_Messages.h"
#include "MOS_MOSServer.h"
#include "MOS_LineManager.h"
#include "MOS_World.h"
#include "MOS_Tools.h"

MOS_IDManager MOS_Lima::idManager_( 137 );

#ifndef MOS_USE_INLINE
#   include "MOS_Lima.inl"
#endif


//-----------------------------------------------------------------------------
// Name: MOS_Lima constructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
MOS_Lima::MOS_Lima()
    : MOS_TacticalLine_ABC ()
    , nFuncType_        ( eLimaFuncLCA )
{
    nID_ = idManager_.GetFreeIdentifier();

    strName_ = MOS_Tools::ToString( eLimaFuncLCA );
    strName_ += " ";
    strName_ += QString::number( MOS_App::GetApp().GetLineManager().GetLineList().size() );
}


// -----------------------------------------------------------------------------
// Name: MOS_Lima constructor
/** @param  pointList 
*/
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
MOS_Lima::MOS_Lima( T_PointVector pointList, E_FuncLimaType nFuncType )
    : MOS_TacticalLine_ABC ()
    , nFuncType_           ( nFuncType )
{
    pointList_ = pointList;
    nID_ = idManager_.GetFreeIdentifier();

    strName_ = MOS_Tools::ToString( nFuncType );
    strName_ += " ";
    strName_ += QString::number( MOS_App::GetApp().GetLineManager().GetLineList().size() );
}


// -----------------------------------------------------------------------------
// Name: MOS_Lima constructor
// Created: APE 2005-01-26
// -----------------------------------------------------------------------------
MOS_Lima::MOS_Lima( const ASN1T_MsgLimaCreation& asnMsg )
: MOS_TacticalLine_ABC ()
{
    nID_ = asnMsg.oid;
    idManager_.LockIdentifier( nID_ );
    nState_ = eStateOk;
    nNetworkState_ = eNetworkStateRegistered;
    bCreatedByMOS_ = false;

    strName_ = MOS_Tools::ToString( eLimaFuncLCA );
    strName_ += " ";
    strName_ += QString::number( MOS_App::GetApp().GetLineManager().GetLineList().size() );

    assert( asnMsg.geometrie.type == EnumTypeLocalisation::line );
    for( uint i = 0; i != asnMsg.geometrie.vecteur_point.n ; ++i )
    {
        MT_Vector2D vTmp;
        MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.geometrie.vecteur_point.elem[i].data, vTmp );
        pointList_.push_back( vTmp );
    }

    nFuncType_ = (E_FuncLimaType)asnMsg.fonction;
}


//-----------------------------------------------------------------------------
// Name: MOS_Lima destructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
MOS_Lima::~MOS_Lima()
{
//    if( bCreatedByMOS_ )
    idManager_.ReleaseIdentifier( nID_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_Lima::UpdateToSim
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
bool MOS_Lima::UpdateToSim()
{
    if ( !MOS_App::GetApp().GetMOSServer().GetController().GetConnectionMgr().IsConnectedToAgentServer() )
        return false;

    assert( nNetworkState_ != eNetworkStateRegistering );

    uint i;
    switch( nState_ )
    {
        case eStateCreated:
        {
            assert( !pointList_.empty() );
            MOS_ASN_MsgLimaCreation asnMsg;

            asnMsg.GetAsnMsg().oid                             = nID_;
            asnMsg.GetAsnMsg().fonction                        = (ASN1T_EnumTypeLima)nFuncType_;
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
            MOS_ASN_MsgLimaUpdate asnMsg;

            asnMsg.GetAsnMsg().oid                             = nID_;
            asnMsg.GetAsnMsg().fonction                        = (ASN1T_EnumTypeLima)nFuncType_;
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
            MOS_ASN_MsgLimaDestruction asnMsg;

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
// Name: MOS_Lima::Read
/** @param  archive 
*/
// Created: APE 2004-07-26
// -----------------------------------------------------------------------------
void MOS_Lima::Read( MT_InputArchive_ABC& archive )
{
    archive.Section( "Lima" );
    int nFuncType;
    archive.ReadField( "Type", nFuncType );
    nFuncType_ = (E_FuncLimaType)nFuncType;
    MOS_TacticalLine_ABC::Read( archive );
    archive.EndSection();

    strName_ = MOS_Tools::ToString( nFuncType_ );
    strName_ += " ";
    strName_ += QString::number( MOS_App::GetApp().GetLineManager().GetLineList().size() );
}


// -----------------------------------------------------------------------------
// Name: MOS_Lima::Write
/** @param  archive 
*/
// Created: APE 2004-07-26
// -----------------------------------------------------------------------------
void MOS_Lima::Write( MT_OutputArchive_ABC& archive ) const
{
    archive.Section( "Lima" );
    archive.WriteField( "Type", nFuncType_ );
    MOS_TacticalLine_ABC::Write( archive );
    archive.EndSection();
}
