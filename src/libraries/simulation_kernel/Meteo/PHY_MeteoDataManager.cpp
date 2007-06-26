//*****************************************************************************
// Created: JVT 02-10-21
//*****************************************************************************

#include "simulation_kernel_pch.h"

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
PHY_MeteoDataManager::PHY_MeteoDataManager( MIL_Config& config )
    : pEphemeride_ ( 0 )
    , pGlobalMeteo_( 0 )
    , meteos_      ( )
    , pRawData_    ( 0 )        
{
    PHY_Precipitation::Initialize();
    PHY_Lighting     ::Initialize();

    std::string fileName = config.GetWeatherFile();

    MIL_InputArchive archive;
    archive.Open( fileName );
    config.AddFileToCRC( fileName );

    archive.Section( "Meteo" );
    pEphemeride_  = new PHY_Ephemeride( archive );
    InitializeGlobalMeteo( archive );
    pRawData_     = new PHY_RawVisionData( *pGlobalMeteo_, config );
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
    pGlobalMeteo_->DecRef();
    assert( meteos_.size() == 1 );

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
    RegisterMeteo( *pGlobalMeteo_ );
    archive.EndSection(); // MeteoGlobale
}

//-----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::InitializeLocalMeteos
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
void PHY_MeteoDataManager::InitializeLocalMeteos( MIL_InputArchive& archive )
{
    // Initialisation des m�t�os locales
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
        RegisterMeteo( *pMeteo );
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
void PHY_MeteoDataManager::OnReceiveMsgGlobalMeteo( const ASN1T_MsgControlGlobalMeteo& asnMsg )
{
    assert( pGlobalMeteo_ );
    pGlobalMeteo_->Update( asnMsg.attributes );
    
    NET_ASN_MsgControlGlobalMeteoAck asnReplyMsg;
    asnReplyMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::OnReceiveMsgLocalMeteo
// Created: NLD 2003-08-04
// Last modified: JVT 03-08-05
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::OnReceiveMsgLocalMeteo( const ASN1T_MsgControlLocalMeteo& asnMsg )
{
    MT_Vector2D vUpLeft;
    MT_Vector2D vDownRight;

    NET_ASN_Tools::ReadPoint( asnMsg.top_left_coordinate,      vUpLeft    );
    NET_ASN_Tools::ReadPoint( asnMsg.bottom_right_coordinate , vDownRight );

    PHY_Meteo* pTmp = 0;
    if( asnMsg.meteo.t == T_MsgControlLocalMeteo_meteo_attributes )
    {
        pTmp = new PHY_Meteo( *asnMsg.meteo.u.attributes );
        RegisterMeteo( *pTmp );
    }

    assert( pRawData_ );
    pRawData_->RegisterMeteoPatch( vUpLeft, vDownRight, pTmp );

    NET_ASN_MsgControlLocalMeteoAck asnReplyMsg;
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
        for( CIT_MeteoSet it = meteos_.begin(); it != meteos_.end(); ++it )
            (*it)->Update( *pEphemeride_ );
    }
}

