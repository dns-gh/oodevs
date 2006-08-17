// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_Convoy_ABC.cpp $
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
#include "Decision/DEC_ModelPion.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Orders/Pion/MIL_PionMissionType.h"
#include "Tools/MIL_Tools.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

      MT_InterpolatedFunction< MT_Float > PHY_Convoy_ABC::formingTime_;
      MT_InterpolatedFunction< MT_Float > PHY_Convoy_ABC::loadingTime_;
      MT_InterpolatedFunction< MT_Float > PHY_Convoy_ABC::unloadingTime_;
const MIL_AgentTypePion*                  PHY_Convoy_ABC::pConvoyAgentType_   = 0;
const MIL_PionMissionType*                PHY_Convoy_ABC::pConvoyMissionType_ = 0;

BOOST_CLASS_EXPORT_GUID( PHY_Convoy_ABC, "PHY_Convoy_ABC" )

// =============================================================================
// 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::Initialize
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing convoys" );

    archive.Section( "Ravitaillement" );
    archive.Section( "Convois" );

    InitializeConvoyUnitType( archive );
    InitializeConvoyMission ( archive );

    InitializeInterpolatedTime( archive, "TempsConstitution", formingTime_   );
    InitializeInterpolatedTime( archive, "TempsChargement"  , loadingTime_   );
    InitializeInterpolatedTime( archive, "TempsDechargement", unloadingTime_ );
   
    archive.EndSection(); // Convois
    archive.EndSection(); // Ravitaillement
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::InitializeConvoyUnitType
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::InitializeConvoyUnitType( MIL_InputArchive& archive )
{
    archive.Section( "TypeUnite" );
    std::string strConvoyAgentType;
    archive.ReadAttribute( "nom", strConvoyAgentType );

    pConvoyAgentType_ = MIL_AgentTypePion::FindPionType( strConvoyAgentType );
    if( !pConvoyAgentType_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown type for convoy", archive.GetContext() );

    archive.EndSection(); // TypeUnite
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::InitializeConvoyMission
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::InitializeConvoyMission( MIL_InputArchive& archive )
{
    archive.Section( "Mission" );
    std::string strMission;
    archive.ReadAttribute( "nom", strMission );

    pConvoyMissionType_ = MIL_PionMissionType::FindPionMissionType( strMission );
    if( !pConvoyMissionType_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Invalid mission name for convoy", archive.GetContext() );

    assert( pConvoyAgentType_ );
    if( !pConvoyAgentType_->GetModel().IsMissionAvailable( *pConvoyMissionType_ ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Convoy type pion cannot receive convoy mission", archive.GetContext() );

    archive.EndSection(); // Mission
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::InitializeInterpolatedTime
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_Convoy_ABC::InitializeInterpolatedTime( MIL_InputArchive& archive, const std::string& strTagName, MT_InterpolatedFunction< MT_Float >& data )
{
    archive.BeginList( strTagName );

    data.AddNewPoint( 0., 0. );
    uint     nMaxNbrCamions         = 0;
    MT_Float rTimeWhenMaxNbrCamions = 0.;
    while( archive.NextListElement() )
    {
        archive.Section( "Temps" );

        uint     nNbrCamions;
        MT_Float rTime;

        archive.ReadAttribute( "nbCamions", nNbrCamions, CheckValueGreater( 0  ) );
        archive.ReadTime     (              rTime      , CheckValueGreater( 0. ) );
        rTime = MIL_Tools::ConvertSecondsToSim( rTime );

        data.AddNewPoint( nNbrCamions, rTime ); 

        if( nNbrCamions >= nMaxNbrCamions )
        {
            rTimeWhenMaxNbrCamions = rTime;
            nMaxNbrCamions = nNbrCamions;
        }
        archive.EndSection(); // Temps
    }   
    data.SetAfterValue( rTimeWhenMaxNbrCamions );

    archive.EndList(); // strTagName
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

// =============================================================================
// CHECKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, PHY_Convoy_ABC::T_ConveyorMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const PHY_Convoy_ABC::T_ConveyorMap& map, const uint )
        {
            file << map.size();
            for ( PHY_Convoy_ABC::CIT_ConveyorMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first;
                file << it->second;
            }   
        }
        
        template< typename Archive >
        void load( Archive& file, PHY_Convoy_ABC::T_ConveyorMap& map, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                PHY_ComposantePion* pComp;
                
                file >> pComp;
                file >> map[ pComp ];
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_Convoy_ABC::serialize( Archive& file, const uint )
{
    file & const_cast< PHY_SupplyConsign_ABC*& >( pConsign_ )
         & conveyors_;
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::GetSupplyingAutomate
// Created: NLD 2006-07-31
// -----------------------------------------------------------------------------
MIL_AutomateLOG& PHY_Convoy_ABC::GetSupplyingAutomate() const
{
    assert( pConsign_ );
    return pConsign_->GetSupplyingAutomate();
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::GetConvoyingAutomate
// Created: NLD 2006-07-31
// -----------------------------------------------------------------------------
MIL_AutomateLOG& PHY_Convoy_ABC::GetConvoyingAutomate() const
{
    assert( pConsign_ );
    return pConsign_->GetConvoyingAutomate();
}
    
// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::GetSuppliedAutomate
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
const MIL_Automate& PHY_Convoy_ABC::GetSuppliedAutomate() const
{
    assert( pConsign_ );
    return pConsign_->GetSuppliedAutomate();
}

// =============================================================================
// OPERATIONS
// =============================================================================

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

            const MT_Float rNbrConvoyed = conveyor.Convoy( *pConsign_, dotationCategory, itMerchandise->second );
            if( rNbrConvoyed > 0. )
                itMerchandise->second -= rNbrConvoyed;            
        }
            
        // Allocate new conveyors
        while( itMerchandise->second > 0. )
        {
            PHY_ComposantePion* pConveyorComp = 0;
            MIL_AgentPion*      pConveyorPion = 0;
            if( !pConsign_->GetConvoyingAutomate().SupplyGetAvailableConvoyTransporter( pConveyorComp, pConveyorPion, dotationCategory ) )
                break; // No more convoys

            PHY_Conveyor* pConveyor = new PHY_Conveyor( *pConveyorComp, *pConveyorPion );
            bool bOut = conveyors_.insert( std::make_pair( pConveyorComp, pConveyor ) ).second;
            assert( bOut );

            const MT_Float rNbrConvoyed = pConveyor->Convoy( *pConsign_, dotationCategory, itMerchandise->second );
            if( rNbrConvoyed > 0. )
                itMerchandise->second -= rNbrConvoyed;            
        }
    }            

    if( conveyors_.empty() )
        return false;

    pConsign_->CancelMerchandiseOverheadReservation();
    return true; 
}

// =============================================================================
// EVENTS
// =============================================================================

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
 
