//*****************************************************************************
//
// $Created : AML 03-03-26 $
// $Archive : $
// $Author  : $
// $Modtime : $
// $Revision: 15 $
// $Workfile: LAU_ConMgr.cpp $
//
//*****************************************************************************

#include "LAU_pch.h"
#include "LAU_ConMgr.h"


#ifndef USE_INLINE
#   include "LAU_ConMgr.inl"
#endif

using namespace DIN;
using namespace NEK;

#include "LAU.h"
#include "LAU_NetMgr.h"
#include "LAU_MsgMgr.h"
#include "LAU_Workspace.h"



//-----------------------------------------------------------------------------
// Name   : LAU_ConMgr constructor
// Created: AML 03-03-31
//-----------------------------------------------------------------------------
LAU_ConMgr::LAU_ConMgr( LAU_NetMgr& netMgr )
    : LAU_SrvMgr_ABC            (       netMgr         )
{
    DIN_ConnectorHost connectorMos( eConnector_MOS_LAU );
    pConnServiceMos_ = new DIN_ConnectionServiceServerUserCbk< LAU_ConMgr >( 
                                      *this
                                    , netMgr_.GetDIN_Engine()
                                    , connectorMos
                                    , netMgr_.GetConnectionProtocols()
                                    , netMgr.GetMagic()
                                    , "Launching server for MOS client" );

    pConnServiceMos_->SetCbkOnConnectionReceived( & LAU_ConMgr::OnConnectionReceivedMos    );
    pConnServiceMos_->SetCbkOnConnectionFailed  ( & LAU_ConMgr::OnBadConnectionReceived );
    pConnServiceMos_->SetCbkOnConnectionLost    ( & LAU_ConMgr::OnConnectionLost        );

    NEK_AddressINET addressMos( netMgr.GetPortMos() );
    pConnServiceMos_->CreateHost( addressMos, "LAU - Scipio SIM Launcher Server - MOS Part" );



    DIN_ConnectorHost connectorSim( eConnector_SIM_LAU );
    pConnServiceSim_ = new DIN_ConnectionServiceServerUserCbk< LAU_ConMgr >( 
                                      *this
                                    , netMgr_.GetDIN_Engine()
                                    , connectorSim
                                    , netMgr_.GetConnectionProtocols()
                                    , netMgr.GetMagic()
                                    , "Launching server for SIM client" );

    pConnServiceSim_->SetCbkOnConnectionReceived( & LAU_ConMgr::OnConnectionReceivedSim    );
    pConnServiceSim_->SetCbkOnConnectionFailed  ( & LAU_ConMgr::OnBadConnectionReceived );
    pConnServiceSim_->SetCbkOnConnectionLost    ( & LAU_ConMgr::OnConnectionLost        );

    NEK_AddressINET addressSim( netMgr.GetPortSim() );
    pConnServiceSim_->CreateHost( addressSim, "LAU - Scipio SIM Launcher Server - SIM Part" );


}



//-----------------------------------------------------------------------------
// Name   : LAU_ConMgr destructor
// Created: AML 03-03-31
//-----------------------------------------------------------------------------
LAU_ConMgr::~LAU_ConMgr()
{
    delete pConnServiceMos_;
    delete pConnServiceSim_;

    CIT_ID_MOS_PtrMap itMapMos, itMapMosEnd;
    for( itMapMos = mapMos_.begin(), itMapMosEnd = mapMos_.end(); itMapMos != itMapMosEnd; ++itMapMos )
         delete (*itMapMos).second;
    mapMos_.clear();


    CIT_ID_SIM_PtrMap itMapSim, itMapSimEnd;
    for( itMapSim = mapSim_.begin(), itMapSimEnd = mapSim_.end(); itMapSim != itMapSimEnd; ++itMapSim )
        delete (*itMapSim).second;
    mapSim_.clear();
}
    
//-----------------------------------------------------------------------------
// Name   : LAU_ConMgr::Reset
// Created: AML 03-03-31
//-----------------------------------------------------------------------------
void LAU_ConMgr::Reset()
{

}

//=============================================================================
// CONNECTION SERVICE CALLBACKS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: LAU_ConMgr::OnConnectionReceivedMos
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
void LAU_ConMgr::OnConnectionReceivedMos( DIN_Server& /*server*/, DIN_Link& link )
{
    MT_LOG_INFO_MSG( MT_FormatString( "LAU <- MOS - Connection received from %s", link.GetRemoteAddress().GetAddressAsString().c_str() ).c_str() );

    LAU_MOS& mos = CreateMOS( link );    
    netMgr_.GetMsgMgr().EnableMos( mos );
}

//-----------------------------------------------------------------------------
// Name: LAU_ConMgr::OnConnectionReceivedSim
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
void LAU_ConMgr::OnConnectionReceivedSim( DIN_Server& /*server*/, DIN_Link& link )
{
    MT_LOG_INFO_MSG( MT_FormatString( "LAU <- SIM - Connection received from %s", link.GetRemoteAddress().GetAddressAsString().c_str() ).c_str() );

    // $$$$ AML 03-04-16: on a deja crée l'objet LAU_SIM, il faut maintenant attendre sont premier message pour faire la liaison LAU_SIM <-> DIN_LINK
//    LAU_MOS& mos = CreateSim( link );    
    netMgr_.GetMsgMgr().EnableSim( link );
}

//-----------------------------------------------------------------------------
// Name: LAU_ConMgr::OnBadConnectionReceived
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
void LAU_ConMgr::OnBadConnectionReceived( DIN_Server& /*server*/, const NEK_Address_ABC& address, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( MT_FormatString( "LAU <- MOS or SIM - Bad connection received from %s (reason : %s)", address.GetAddressAsString().c_str(), reason.GetInfo().c_str() ).c_str() );
}


//-----------------------------------------------------------------------------
// Name: LAU_ConMgr::OnConnectionLost
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
void LAU_ConMgr::OnConnectionLost( DIN_Server& /*server*/, DIN_Link& link, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( MT_FormatString( "LAU <- MOS or SIM - Connection to %s lost (reason : %s)", link.GetRemoteAddress().GetAddressAsString().c_str(), reason.GetInfo().c_str() ).c_str() );
    
    if( IsLinkToMOS( link ) )
    {
        DestroyMOS( link );
    }
    else if( IsLinkToSIM( link ) )
    {
        DestroySIM( link );
    }
}

//=============================================================================
// TOOLS
//=============================================================================


//-----------------------------------------------------------------------------
// Name   : LAU_ConMgr::CreateMOS
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
LAU_MOS& LAU_ConMgr::CreateMOS( DIN::DIN_Link& link )
{
    assert( !link.GetUserData() );

    LAU_MOS* pMos = new LAU_MOS();
    pMos->Initialize( link );

    bool32 bOut = mapMos_.insert( std::make_pair( link.GetStationID(), pMos ) ).second;
    assert( bOut );
    bOut;

    link.SetUserData( pMos );

    return *pMos;
}



//-----------------------------------------------------------------------------
// Name   : LAU_ConMgr::DestroyMOS
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
void LAU_ConMgr::DestroyMOS( DIN::DIN_Link& link )
{
    assert( link.GetUserData() );
  
    LAU_MOS* pMos = FindLinkedMOS( link );
    if( pMos == NULL )
    {
        assert( 0 );
        return;
    }
    pMos->Terminate();  // ne sert a rien car fait automatiquement lors de la destruction de l'objet!

    int nOut = mapMos_.erase( link.GetStationID() );
    assert( nOut == 1 );
    nOut;

    GetNetMgr().GetWorkspace().MosHasDisconnect( pMos );

    delete pMos;
    
    link.SetUserData( 0 );
}


//-----------------------------------------------------------------------------
// Name   : LAU_ConMgr::FindLinkedMOS
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
LAU_MOS* LAU_ConMgr::FindLinkedMOS( DIN::DIN_Link& link )
{
    IT_ID_MOS_PtrMap itFind =  mapMos_.find( link.GetStationID() );
    if( itFind == mapMos_.end() )
        return NULL;
    return (*itFind).second;
}


//-----------------------------------------------------------------------------
// Name   : LAU_ConMgr::AssociateSIM_WithLink
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
LAU_SIM& LAU_ConMgr::AssociateSIM_WithLink( LAU_SIM& sim, DIN::DIN_Link& link )
{
    assert( !link.GetUserData() );

    sim.Initialize( link );
    sim.SetHasBeenInitialised();

    bool32 bOut = mapSim_.insert( std::make_pair( link.GetStationID(), &sim ) ).second;
    assert( bOut );

    link.SetUserData( &sim );

    return sim;
}



//-----------------------------------------------------------------------------
// Name   : LAU_ConMgr::DestroySIM
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
void LAU_ConMgr::DestroySIM( DIN::DIN_Link& link )
{
#ifdef _DEBUG
    bool32 bAlreadyDeletedDebug = ( link.GetUserData() == 0 );
    bAlreadyDeletedDebug;
#endif // _DEBUG

    // ne rien faire, juste deleter de la map et c'est tout!
  
    LAU_SIM* pSim = FindLinkedSIM( link );
    if( pSim != NULL )
    {
        //pSim->Terminate(); // fait automatiquement lors de la destruction de l'objet!
        pSim->SessionIsNoMoreValid(); // AML 03-06-11: on met pSession_ a 0
    }

    int nOut = mapSim_.erase( link.GetStationID() );
    assert( nOut == 1 );
    nOut;
//    delete pSim;  // not owned!
    
    link.SetUserData( 0 ); // souvent deja fait...
}

//-----------------------------------------------------------------------------
// Name   : LAU_ConMgr::FindLinkedSIM
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
LAU_SIM* LAU_ConMgr::FindLinkedSIM( DIN::DIN_Link& link )
{
    IT_ID_SIM_PtrMap itFind =  mapSim_.find( link.GetStationID() );
    if( itFind == mapSim_.end() )
    {
        return NULL;
    }

    return (*itFind).second;
}