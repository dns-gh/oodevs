//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Limit.cpp $
// $Author: Ape $
// $Modtime: 26/01/05 16:03 $
// $Revision: 8 $
// $Workfile: Limit.cpp $
//
//*****************************************************************************

#include "astec_pch.h"
#include "Limit.h"
#include "AgentServerMsgMgr.h"
#include "ASN_Messages.h"
#include "World.h"

IDManager Limit::idManager_( 138 );

//-----------------------------------------------------------------------------
// Name: Limit constructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
Limit::Limit()
    : TacticalLine_ABC()
{
    nID_ = idManager_.GetFreeIdentifier();
    strName_ = QString( "Limit %1" ).arg( nID_ & 0x3FFFFF );
}


// -----------------------------------------------------------------------------
// Name: Limit constructor
/** @param  pointList 
*/
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
Limit::Limit( T_PointVector pointList )
    : TacticalLine_ABC()
{
    pointList_ = pointList;
    nID_ = idManager_.GetFreeIdentifier();
    strName_ = QString( "Limit %1" ).arg( nID_ & 0x3FFFFF );
}


//-----------------------------------------------------------------------------
// Name: Limit constructor
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
Limit::Limit( const ASN1T_MsgLimitCreation& asnMsg )
    : TacticalLine_ABC()
{
    nID_ = asnMsg.oid;
    idManager_.LockIdentifier( nID_ );
    nState_ = eStateOk;
    nNetworkState_ = eNetworkStateRegistered;
    bCreatedBy = false;
    nLevel_        = (E_NatureLevel) asnMsg.level;

    strName_ = QString( "Limit %1" ).arg( nID_ & 0x3FFFFF );

    assert( asnMsg.geometrie.type == EnumTypeLocalisation::line );
    for( uint i = 0; i != asnMsg.geometrie.vecteur_point.n ; ++i )
    {
        MT_Vector2D vTmp;
        App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.geometrie.vecteur_point.elem[i].data, vTmp );
        pointList_.push_back( vTmp );
    }
}


//-----------------------------------------------------------------------------
// Name: Limit destructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
Limit::~Limit()
{
//    if( bCreatedBy )
    idManager_.ReleaseIdentifier( nID_ );
}


//-----------------------------------------------------------------------------
// Name: Limit::UpdateToSim
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
bool Limit::UpdateToSim()
{
    if ( !App::GetApp().GetNetwork().IsConnected() )
        return false;

    uint i;

    switch( nState_ )
    {
        case eStateCreated:
        {
            assert( !pointList_.empty() );
        
            ASN_MsgLimitCreation asnMsg;

            asnMsg.GetAsnMsg().oid                          = nID_;
            asnMsg.GetAsnMsg().level                        = (ASN1T_EnumNatureLevel) nLevel_;
            asnMsg.GetAsnMsg().geometrie.type               = EnumTypeLocalisation::line;
            asnMsg.GetAsnMsg().geometrie.vecteur_point.n    = pointList_.size();
            asnMsg.GetAsnMsg().geometrie.vecteur_point.elem = new ASN1T_CoordUTM[ pointList_.size() ];

            i = 0;
            for ( CIT_PointVector itPoint = pointList_.begin() ; itPoint != pointList_.end() ; ++itPoint )
            {
                std::string strMGRS;
                const MT_Vector2D& vPos = *itPoint;
                App::GetApp().GetWorld().SimToMosMgrsCoord( vPos, strMGRS );
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

            ASN_MsgLimitUpdate asnMsg;

            asnMsg.GetAsnMsg().oid                          = nID_;
            asnMsg.GetAsnMsg().level                        = (ASN1T_EnumNatureLevel) nLevel_;
            asnMsg.GetAsnMsg().geometrie.type               = EnumTypeLocalisation::line;
            asnMsg.GetAsnMsg().geometrie.vecteur_point.n    = pointList_.size();
            asnMsg.GetAsnMsg().geometrie.vecteur_point.elem = new ASN1T_CoordUTM[ pointList_.size() ];

            i = 0;
            for ( CIT_PointVector itPoint = pointList_.begin() ; itPoint != pointList_.end() ; ++itPoint )
            {
                std::string strMGRS;
                const MT_Vector2D& vPos = *itPoint;
                App::GetApp().GetWorld().SimToMosMgrsCoord( vPos, strMGRS );
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
            ASN_MsgLimitDestruction asnMsg;

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
// Name: Limit::Read
/** @param  archive 
*/
// Created: APE 2004-07-26
// -----------------------------------------------------------------------------
void Limit::Read( MT_InputArchive_ABC& archive )
{
    archive.Section( "Limit" );
    TacticalLine_ABC::Read( archive );
    archive.EndSection();
}


// -----------------------------------------------------------------------------
// Name: Limit::Write
/** @param  archive 
*/
// Created: APE 2004-07-26
// -----------------------------------------------------------------------------
void Limit::Write( MT_OutputArchive_ABC& archive ) const
{
    archive.Section( "Limit" );
    TacticalLine_ABC::Write( archive );
    archive.EndSection();
}
