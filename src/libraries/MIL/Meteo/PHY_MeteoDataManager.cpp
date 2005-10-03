//*****************************************************************************
// Created: JVT 02-10-21
//*****************************************************************************

#include "MIL_pch.h"

#include "PHY_MeteoDataManager.h"

#include "PHY_Ephemeride.h"
#include "PHY_Meteo.h"
#include "PHY_Precipitation.h"
#include "PHY_Lighting.h"
#include "RawVisionData/PHY_RawVisionData.h"

#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"

#include "Tools/MIL_Tools.h"

//-----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager constructor
// Created: JVT 02-10-21
// Last modified: JVT 04-03-24
//-----------------------------------------------------------------------------
PHY_MeteoDataManager::PHY_MeteoDataManager( MIL_InputArchive& initArchive )
    : pEphemeride_ ( 0 )
    , pGlobalMeteo_( 0 )
    , localMeteos_ ( )
    , pRawData_    ( 0 )        
{
    PHY_Precipitation::Initialize();
    PHY_Lighting     ::Initialize();

    std::string fileName;
    initArchive.ReadField( "Meteo", fileName );

    MIL_InputArchive archive;
    archive.AddWarningStream( std::cout );
    archive.Open( fileName );
    MIL_AgentServer::GetWorkspace().GetConfig().AddFileToCRC( fileName );

    archive.Section( "Meteo" );
    pEphemeride_  = new PHY_Ephemeride( archive );
    InitializeGlobalMeteo( archive );
    pRawData_     = new PHY_RawVisionData( *pGlobalMeteo_, initArchive );
    InitializeLocalMeteos( archive );

    archive.EndSection(); // Meteo
    archive.Close();
}

//-----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager 
// Created: JVT 02-10-21
//-----------------------------------------------------------------------------
PHY_MeteoDataManager::~PHY_MeteoDataManager()
{  
    delete pRawData_;
    assert( localMeteos_.empty() );
    pGlobalMeteo_->DecRef();

    PHY_Lighting     ::Terminate();
    PHY_Precipitation::Terminate();
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::InitializeGlobalMeteo
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::InitializeGlobalMeteo( MIL_InputArchive& archive )
{
    archive.Section( "MeteoGlobale" );
    pGlobalMeteo_ = new PHY_Meteo( archive, *pEphemeride_ );
    pGlobalMeteo_->IncRef();
    archive.EndSection(); // MeteoGlobale
}

//-----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::InitializeLocalMeteos
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
void PHY_MeteoDataManager::InitializeLocalMeteos( MIL_InputArchive& archive )
{
    // Initialisation des météos locales
    if ( !archive.BeginList( "PatchsLocaux", MIL_InputArchive::eNothing ) )
        return;

    assert( pRawData_ );
    while( archive.NextListElement() )
    {
        archive.Section( "PatchLocal" );

        std::string strPos;
        MT_Vector2D vUpLeft;
        MT_Vector2D vDownRight;

        archive.ReadAttribute( "hautGauche", strPos );
        MIL_Tools::ConvertCoordMosToSim( strPos, vUpLeft );

        archive.ReadAttribute( "basDroit", strPos );
        MIL_Tools::ConvertCoordMosToSim( strPos, vDownRight );

        PHY_Meteo* pMeteo = new PHY_Meteo( archive, *pEphemeride_ );
        localMeteos_.push_back( pMeteo );
        pRawData_->RegisterMeteoPatch( vUpLeft, vDownRight, pMeteo );

        archive.EndSection();
    }
    archive.EndList(); // PatchsLocaux
}


// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::OnReceiveMsgGlobalMeteo
// Created: NLD 2003-08-04
// Last modified: JVT 03-08-05
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::OnReceiveMsgGlobalMeteo( const ASN1T_MsgCtrlMeteoGlobale& asnMsg )
{
    assert( pGlobalMeteo_ );
    pGlobalMeteo_->Update( asnMsg.attributs );
    
    NET_ASN_MsgCtrlMeteoGlobaleAck asnReplyMsg;
    asnReplyMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::OnReceiveMsgLocalMeteo
// Created: NLD 2003-08-04
// Last modified: JVT 03-08-05
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::OnReceiveMsgLocalMeteo( const ASN1T_MsgCtrlMeteoLocale& asnMsg )
{
    MT_Vector2D vUpLeft;
    MT_Vector2D vDownRight;

    NET_ASN_Tools::ReadPoint( asnMsg.rect_point_haut_gauche, vUpLeft    );
    NET_ASN_Tools::ReadPoint( asnMsg.rect_point_bas_droite , vDownRight );

    PHY_Meteo* pTmp = asnMsg.meteo.t == T_MsgCtrlMeteoLocale_meteo_globale ? 0 /*reset meteo globale*/ : new PHY_Meteo( *asnMsg.meteo.u.attributs );
    assert( pRawData_ );
    pRawData_->RegisterMeteoPatch( vUpLeft, vDownRight, pTmp );

    NET_ASN_MsgCtrlMeteoLocaleAck asnReplyMsg;
    asnReplyMsg.Send();
}


//=============================================================================
// SPECIAL AMMO EFFECTS
//=============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::RegisterWeatherEffect
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::RegisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory )
{
    assert( pRawData_ );
    pRawData_->RegisterWeatherEffect( surface, ammoCategory );
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::UnregisterWeatherEffect
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::UnregisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory )
{
    assert( pRawData_ );
    pRawData_->UnregisterWeatherEffect( surface, ammoCategory );
}


//-----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::Update
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
void PHY_MeteoDataManager::Update()
{
    // cycle jour / nuit
    assert( pEphemeride_ );
    if( pEphemeride_->UpdateNight() )
    {
        MT_LOG_DEBUG_MSG( MT_FormatString( "Maintenant il fait %s", pEphemeride_->GetLightingBase().GetName().c_str() ) );

        pGlobalMeteo_->Update( *pEphemeride_ );
        for( CIT_MeteoList itMeteo = localMeteos_.begin(); itMeteo != localMeteos_.end(); ++itMeteo )
            (*itMeteo)->Update( *pEphemeride_ );
    }
}

