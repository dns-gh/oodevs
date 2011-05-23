// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_Convoy_ABC.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:26 $
// $Revision: 8 $
// $Workfile: PHY_Convoy_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_Convoy_ABC.h"
#include "PHY_Conveyor.h"
#include "PHY_SupplyConsign_ABC.h"
#include "Decision/DEC_Model_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Orders/MIL_PionMissionType.h"
#include "Tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>
#include <boost/serialization/map.hpp>

struct PHY_Convoy_ABC::LoadingWrapper
{
    void ReadInterpolatedTime( xml::xistream& xis, MT_InterpolatedFunction< double >& data, std::pair< unsigned int, double >& upperBound )
    {
        PHY_Convoy_ABC::ReadInterpolatedTime( xis, data, upperBound );
    }
    void ReadSpeedModifier( xml::xistream& xis, std::pair< unsigned int, double >& upperBound )
    {
        PHY_Convoy_ABC::ReadSpeedModifier( xis, upperBound );
    }
};

      MT_InterpolatedFunction< double > PHY_Convoy_ABC::formingTime_;
      MT_InterpolatedFunction< double > PHY_Convoy_ABC::loadingTime_;
      MT_InterpolatedFunction< double > PHY_Convoy_ABC::unloadingTime_;
      MT_InterpolatedFunction< double > PHY_Convoy_ABC::coefSpeedModificator_;
const MIL_AgentTypePion*                  PHY_Convoy_ABC::pConvoyAgentType_   = 0;
const MIL_MissionType_ABC*                PHY_Convoy_ABC::pConvoyMissionType_ = 0;

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_Convoy_ABC )

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::Initialize
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing convoys" );
    xis >> xml::start( "supply" )
            >> xml::start( "convoys" );
    InitializeConvoyUnitType( xis );
    InitializeConvoyMission ( xis );
    InitializeInterpolatedTime ( xis, "constitution-times", formingTime_   );
    InitializeInterpolatedTime ( xis, "loading-times"     , loadingTime_   );
    InitializeInterpolatedTime ( xis, "unloading-times"   , unloadingTime_ );
    InitializeSpeedModificators( xis );
    xis     >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::InitializeConvoyUnitType
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::InitializeConvoyUnitType( xml::xistream& xis )
{
    std::string strConvoyAgentType;
    xis >> xml::attribute( "unit-type", strConvoyAgentType );

    pConvoyAgentType_ = MIL_AgentTypePion::Find( strConvoyAgentType );
    if( !pConvoyAgentType_ )
        xis.error( "Unknown type for convoy" );
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::InitializeConvoyMission
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::InitializeConvoyMission( xml::xistream& xis )
{
    std::string strMission;
    xis >> xml::attribute( "mission", strMission );

    pConvoyMissionType_ = MIL_PionMissionType::Find( strMission );
    if( !pConvoyMissionType_ )
        xis.error( "Invalid mission name for convoy" );

    assert( pConvoyAgentType_ );
    if( !pConvoyAgentType_->GetModel().IsMissionAvailable( *pConvoyMissionType_ ) )
        xis.error( "Convoy type pion cannot receive convoy mission" );
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::InitializeInterpolatedTime
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::InitializeInterpolatedTime( xml::xistream& xis, const std::string& strTagName, MT_InterpolatedFunction< double >& data )
{
    data.AddNewPoint( 0., 0. );
    LoadingWrapper loader;

    std::pair< unsigned int, double > upperBound( 0, 0.f );
    xis >> xml::start( strTagName )
            >> xml::list( "unit-time", loader, &LoadingWrapper::ReadInterpolatedTime, data, upperBound )
        >> xml::end;

    data.SetAfterValue( upperBound.second );
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::ReadTime
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::ReadInterpolatedTime( xml::xistream& xis, MT_InterpolatedFunction< double >& data, std::pair< unsigned int, double >& upperBound )
{
    unsigned int     nNbrCamions;
    double rTime;

    xis >> xml::attribute( "truck-count", nNbrCamions );
    if( nNbrCamions <= 0 )
        xis.error( "time: truck-count <= 0" );

    tools::ReadTimeAttribute( xis, "time", rTime );
    if( rTime <= 0 )
        xis.error( "time: time <= 0" );
    rTime = MIL_Tools::ConvertSecondsToSim( rTime );

    data.AddNewPoint( nNbrCamions, rTime );

    if( nNbrCamions >= upperBound.first )
    {
        upperBound.first = nNbrCamions;
        upperBound.second = rTime;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::InitializeSpeedModificators
// Created: NLD 2007-02-05
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::InitializeSpeedModificators( xml::xistream& xis )
{
    coefSpeedModificator_.AddNewPoint( 0., 0. );
    LoadingWrapper loader;

    std::pair< unsigned int, double > upperBound( 0, 0.f );

    xis >> xml::start( "speed-modifiers" )
            >> xml::list( "speed-modifier", loader, &LoadingWrapper::ReadSpeedModifier, upperBound )
        >> xml::end;

    coefSpeedModificator_.SetAfterValue( upperBound.second );
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::ReadSpeedModifier
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::ReadSpeedModifier( xml::xistream& xis, std::pair< unsigned int, double >& upperBound )
{
    unsigned int     nNbrCamions;
    double rValue;

    xis >> xml::attribute( "truck-count", nNbrCamions )
        >> xml::attribute( "value", rValue );

    if( nNbrCamions <= 0 )
        xis.error( "speed-modifier: truck-count <= 0" );
    if( rValue <= 0 )
        xis.error( "speed-modifier: value <= 0" );

    coefSpeedModificator_.AddNewPoint( nNbrCamions, rValue );

    if( nNbrCamions >= upperBound.first )
    {
        upperBound.first = nNbrCamions;
        upperBound.second = rValue;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::Terminate
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::Terminate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC constructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_Convoy_ABC::PHY_Convoy_ABC( PHY_SupplyConsign_ABC& consign )
    : pConsign_ ( &consign )
    , conveyors_()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_Convoy_ABC::PHY_Convoy_ABC()
    : pConsign_ ( 0 )
    , conveyors_()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC destructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_Convoy_ABC::~PHY_Convoy_ABC()
{
    UnlockConvoy();
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::GetSupplyingAutomate
// Created: NLD 2006-07-31
// -----------------------------------------------------------------------------
MIL_AutomateLOG& PHY_Convoy_ABC::GetSupplier() const
{
    assert( pConsign_ );
    return pConsign_->GetSupplier();
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::GetConvoyer
// Created: NLD 2006-07-31
// -----------------------------------------------------------------------------
MIL_AutomateLOG& PHY_Convoy_ABC::GetConvoyer() const
{
    assert( pConsign_ );
    return pConsign_->GetConvoyer();
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::GetSupplied
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
const MIL_Automate& PHY_Convoy_ABC::GetSupplied() const
{
    assert( pConsign_ );
    return pConsign_->GetSupplied();
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::GetStockSupplier
// Created: MGD 2009-02-11
// -----------------------------------------------------------------------------
MIL_Automate& PHY_Convoy_ABC::GetStockSupplier() const
{
    assert( pConsign_ );
    return pConsign_->GetStockSupplier();
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::UnlockConvoy
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::UnlockConvoy()
{
    for( CIT_ConveyorMap it = conveyors_.begin(); it != conveyors_.end(); ++it )
        delete it->second; // Will do the 'StopUsingForLogistic()'
    conveyors_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::ReserveTransporters
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
bool PHY_Convoy_ABC::ReserveTransporters()
{
    assert( pConsign_ );

    T_MerchandiseToConvoyMap merchandise;
    pConsign_->GetMerchandiseToConvoy( merchandise );
    assert( !merchandise.empty() );

    for( IT_MerchandiseToConvoyMap itMerchandise = merchandise.begin(); itMerchandise != merchandise.end(); ++itMerchandise )
    {
        const PHY_DotationCategory& dotationCategory = *itMerchandise->first;

        // Fill the previously used conveyors
        for( CIT_ConveyorMap itConveyor = conveyors_.begin(); itConveyor != conveyors_.end() && itMerchandise->second > 0.; ++itConveyor )
        {
            PHY_Conveyor& conveyor = *itConveyor->second;
            if( conveyor.IsFull() )
                continue;

            const double rNbrConvoyed = conveyor.Convoy( *pConsign_, dotationCategory, itMerchandise->second );
            if( rNbrConvoyed > 0. )
                itMerchandise->second -= rNbrConvoyed;
        }

        // Allocate new conveyors
        while( itMerchandise->second > 0. )
        {
            PHY_ComposantePion* pConveyorComp = 0;
            MIL_AgentPion*      pConveyorPion = 0;
            if( !pConsign_->GetConvoyer().SupplyGetAvailableConvoyTransporter( pConveyorComp, pConveyorPion, dotationCategory ) )
                break; // No more convoys
            PHY_Conveyor* pConveyor = new PHY_Conveyor( *pConveyorComp, *pConveyorPion );
            if( ! conveyors_.insert( std::make_pair( pConveyorComp, pConveyor ) ).second )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
            const double rNbrConvoyed = pConveyor->Convoy( *pConsign_, dotationCategory, itMerchandise->second );
            if( rNbrConvoyed > 0. )
                itMerchandise->second -= rNbrConvoyed;
        }
    }

    if( conveyors_.empty() )
        return false;

    pConsign_->CancelMerchandiseOverheadReservation();
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::EmptyOut
// Created: NLD 2006-07-04
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::EmptyOut()
{
    for( CIT_ConveyorMap it = conveyors_.begin(); it != conveyors_.end(); ++it )
        it->second->EmptyOut();
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::NotifyConveyorDestroyed
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::NotifyConveyorDestroyed( PHY_ComposantePion& composante )
{
    CIT_ConveyorMap itConveyor = conveyors_.find( &composante );
    if( itConveyor == conveyors_.end() )
        return;

    assert( pConsign_ );
    itConveyor->second->NotifyConveyorDestroyed( *pConsign_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::GetFormingTime
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
unsigned int PHY_Convoy_ABC::GetFormingTime() const
{
    return (unsigned int)( formingTime_( static_cast< double >( conveyors_.size() ) ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::GetLoadingTime
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
unsigned int PHY_Convoy_ABC::GetLoadingTime() const
{
    return (unsigned int)( loadingTime_( static_cast< double >( conveyors_.size() ) ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::GetUnloadingTime
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
unsigned int PHY_Convoy_ABC::GetUnloadingTime() const
{
    return (unsigned int)( unloadingTime_( static_cast< double >( conveyors_.size() ) ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::ModifySpeed
// Created: NLD 2007-02-05
// -----------------------------------------------------------------------------
double PHY_Convoy_ABC::ModifySpeed( double rSpeed ) const
{
    if( conveyors_.empty() )
        return rSpeed;
    return rSpeed * coefSpeedModificator_( static_cast< double >( conveyors_.size() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_Convoy_ABC::serialize( Archive& file, const unsigned int )
{
    file & const_cast< PHY_SupplyConsign_ABC*& >( pConsign_ )
         & conveyors_;
}
