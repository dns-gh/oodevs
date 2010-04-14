//*****************************************************************************
// Created: JVT 02-10-21
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MeteoDataManager.h"
#include "PHY_Ephemeride.h"
#include "meteo/PHY_GlobalMeteo.h"
#include "meteo/PHY_LocalMeteo.h"
#include "meteo/PHY_Lighting.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "protocol/SimulationSenders.h"
#include "RawVisionData/PHY_RawVisionData.h"
#include "Tools/MIL_Tools.h"
#include <xeumeuleu/xml.h>

MIL_IDManager PHY_MeteoDataManager::idManager_;

//-----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager constructor
// Created: JVT 02-10-21
// Last modified: JVT 04-03-24
//-----------------------------------------------------------------------------
PHY_MeteoDataManager::PHY_MeteoDataManager( MIL_Config& config )
    : pEphemeride_ ( 0 )
    , pGlobalMeteo_( 0 )
    , meteos_      ()
    , pRawData_    ( 0 )
{
    weather::PHY_Precipitation::Initialize();
    weather::PHY_Lighting     ::Initialize();

    std::string fileName = config.GetWeatherFile();

    xml::xifstream xisWeather( fileName );
    config.AddFileToCRC( fileName );

    xisWeather >> xml::start( "weather" );
    pEphemeride_  = new PHY_Ephemeride( xisWeather );
    InitializeGlobalMeteo( xisWeather );
    pRawData_     = new PHY_RawVisionData( *pGlobalMeteo_, config );
    InitializeLocalMeteos( xisWeather );

    xisWeather >> xml::end();
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

    weather::PHY_Lighting     ::Terminate();
    weather::PHY_Precipitation::Terminate();
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::InitializeGlobalMeteo
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::InitializeGlobalMeteo( xml::xistream& xis )
{
    xis >> xml::start( "theater" );
    pGlobalMeteo_ = new PHY_GlobalMeteo( idManager_.GetFreeId(), xis, pEphemeride_->GetLightingBase(), MIL_Tools::ConvertSpeedMosToSim( 1. ) );
    pGlobalMeteo_->IncRef();
    pGlobalMeteo_->SetListener( this );
    RegisterMeteo( *pGlobalMeteo_ );
    xis >> xml::end();
}

//-----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::InitializeLocalMeteos
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
void PHY_MeteoDataManager::InitializeLocalMeteos( xml::xistream& xis )
{
    xis >> xml::optional() >> xml::start( "local-weather" )
                          >> xml::list( "local", *this, &PHY_MeteoDataManager::ReadPatchLocal )
                      >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::ReadPatchLocal
// Created: ABL 2007-07-27
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::ReadPatchLocal( xml::xistream& xis )
{
    weather::PHY_Meteo* pMeteo = new PHY_LocalMeteo( idManager_.GetFreeId(), xis, pEphemeride_->GetLightingBase(), MIL_Tools::ConvertSpeedMosToSim( 1. ) );
    RegisterMeteo( *pMeteo );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::OnReceiveMsgGlobalMeteo
// Created: NLD 2003-08-04
// Last modified: JVT 03-08-05
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::OnReceiveMsgMeteo( const MsgsClientToSim::MsgControlMeteo& asnMsg )
{
    if( asnMsg.parametres().elem_size() == 7 )
    {
        assert( pGlobalMeteo_ );
        pGlobalMeteo_->Update( asnMsg.parametres() );
        client::ControlGlobalMeteoAck asnReplyMsg;
        asnReplyMsg.Send( NET_Publisher_ABC::Publisher() );
    }
    else if( asnMsg.parametres().elem_size() > 7 )
    {
        weather::PHY_Meteo* meteo = new PHY_LocalMeteo( idManager_.GetFreeId(), asnMsg.parametres(), this );
        RegisterMeteo( *meteo );
        client::ControlLocalMeteoAck replyMsg;
        replyMsg.Send( NET_Publisher_ABC::Publisher() );
    }
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
    //TODO HBD: Envoi d'un message pour la creation d'un weatherEffect.

}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::UnregisterWeatherEffect
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::UnregisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory )
{
    assert( pRawData_ );
    pRawData_->UnregisterWeatherEffect( surface, ammoCategory );
    //TODO HBD: Envoi d'un message pour la destruction d'un weatherEffect.
}

//-----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::Update
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
void PHY_MeteoDataManager::Update( unsigned int date )
{
    // cycle jour / nuit
    assert( pEphemeride_ );
    if( pEphemeride_->UpdateNight( date ) )
    {
        MT_LOG_DEBUG_MSG( MT_FormatString( "Maintenant il fait %s", pEphemeride_->GetLightingBase().GetName().c_str() ) );
        for( CIT_MeteoSet it = meteos_.begin(); it != meteos_.end(); ++it )
            (*it)->Update( pEphemeride_->GetLightingBase() );
    }   
    for( CIT_MeteoSet it = meteos_.begin(); it != meteos_.end(); ++it )
        (*it)->UpdateMeteoPatch( date, *pRawData_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::SendStateToNewClient
// Created: HBD 2010-03-26
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::SendStateToNewClient()
{
    for( IT_MeteoSet it = meteos_.begin(); it != meteos_.end(); ++it )
      (*it)->SendCreation();
}
