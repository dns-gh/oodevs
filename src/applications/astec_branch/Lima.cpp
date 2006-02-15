//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Lima.cpp $
// $Author: Ape $
// $Modtime: 26/01/05 15:47 $
// $Revision: 8 $
// $Workfile: Lima.cpp $
//
//*****************************************************************************

#include "astec_pch.h"
#include "Lima.h"
#include "AgentServerMsgMgr.h"
#include "ASN_Messages.h"
#include "World.h"
#include "Tools.h"

IDManager Lima::idManager_( 137 );

//-----------------------------------------------------------------------------
// Name: Lima constructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
Lima::Lima()
    : TacticalLine_ABC ()
    , nFuncType_        ( eLimaFuncLCA )
{
    nID_ = idManager_.GetFreeIdentifier();

    strName_ = Tools::ToString( eLimaFuncLCA );
    strName_ += " ";
    strName_ += QString::number( nID_ & 0x3FFFFF );
}


// -----------------------------------------------------------------------------
// Name: Lima constructor
/** @param  pointList 
*/
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
Lima::Lima( T_PointVector pointList, E_FuncLimaType nFuncType )
    : TacticalLine_ABC ()
    , nFuncType_           ( nFuncType )
{
    pointList_ = pointList;
    nID_ = idManager_.GetFreeIdentifier();

    strName_ = Tools::ToString( nFuncType );
    strName_ += " ";
    strName_ += QString::number( nID_ & 0x3FFFFF );
}


// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: APE 2005-01-26
// -----------------------------------------------------------------------------
Lima::Lima( const ASN1T_MsgLimaCreation& asnMsg )
: TacticalLine_ABC ()
{
    nID_ = asnMsg.oid;
    idManager_.LockIdentifier( nID_ );
    nState_ = eStateOk;
    nNetworkState_ = eNetworkStateRegistered;
    bCreatedBy = false;

    strName_ = Tools::ToString( eLimaFuncLCA );
    strName_ += " ";
    strName_ += QString::number( nID_ & 0x3FFFFF );

    assert( asnMsg.geometrie.type == EnumTypeLocalisation::line );
    for( uint i = 0; i != asnMsg.geometrie.vecteur_point.n ; ++i )
    {
        MT_Vector2D vTmp;
        App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.geometrie.vecteur_point.elem[i].data, vTmp );
        pointList_.push_back( vTmp );
    }

    nFuncType_ = (E_FuncLimaType)asnMsg.fonction;
}


//-----------------------------------------------------------------------------
// Name: Lima destructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
Lima::~Lima()
{
//    if( bCreatedBy )
    idManager_.ReleaseIdentifier( nID_ );
}


//-----------------------------------------------------------------------------
// Name: Lima::UpdateToSim
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
bool Lima::UpdateToSim()
{
    if ( !App::GetApp().GetNetwork().IsConnected() )
        return false;

    assert( nNetworkState_ != eNetworkStateRegistering );

    uint i;
    switch( nState_ )
    {
        case eStateCreated:
        {
            assert( !pointList_.empty() );
            ASN_MsgLimaCreation asnMsg;

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
            ASN_MsgLimaUpdate asnMsg;

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
            ASN_MsgLimaDestruction asnMsg;

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
// Name: Lima::Read
/** @param  archive 
*/
// Created: APE 2004-07-26
// -----------------------------------------------------------------------------
void Lima::Read( MT_InputArchive_ABC& archive )
{
    archive.Section( "Lima" );
    int nFuncType;
    archive.ReadField( "Type", nFuncType );
    nFuncType_ = (E_FuncLimaType)nFuncType;
    TacticalLine_ABC::Read( archive );
    archive.EndSection();

    strName_ = Tools::ToString( nFuncType_ );
    strName_ += " ";
    strName_ += QString::number( nID_ & 0x3FFFFF );
}


// -----------------------------------------------------------------------------
// Name: Lima::Write
/** @param  archive 
*/
// Created: APE 2004-07-26
// -----------------------------------------------------------------------------
void Lima::Write( MT_OutputArchive_ABC& archive ) const
{
    archive.Section( "Lima" );
    archive.WriteField( "Type", nFuncType_ );
    TacticalLine_ABC::Write( archive );
    archive.EndSection();
}
