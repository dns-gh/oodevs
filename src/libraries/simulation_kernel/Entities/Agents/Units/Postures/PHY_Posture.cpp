// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Postures/PHY_Posture.cpp $
// $Author: Nld $
// $Modtime: 22/11/04 14:16 $
// $Revision: 2 $
// $Workfile: PHY_Posture.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_Posture.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "protocol/Protocol.h"
#include "MT_Tools/MT_Logger.h"
#pragma warning( push, 0 )
#include <google/protobuf/descriptor.h>
#pragma warning( pop )

namespace
{
    std::map< std::string, const PHY_Posture* > postures;
}

const PHY_Posture PHY_Posture::mouvement_        ( "Mouvement"        , eUnitPosture_PostureMouvement        , sword::UnitAttributes::moving                          , PHY_ConsumptionType::moving_       , ePostureCanModifyDetection | ePostureCanModifyPH | ePostureInstantaneous );
const PHY_Posture PHY_Posture::mouvementDiscret_ ( "MouvementDiscret" , eUnitPosture_PostureMouvementDiscret , sword::UnitAttributes::infiltrating                    , PHY_ConsumptionType::moving_       , ePostureCanModifyDetection | ePostureCanModifyPH | ePostureInstantaneous );
const PHY_Posture PHY_Posture::arret_            ( "Arret"            , eUnitPosture_PostureArret            , sword::UnitAttributes::stopping                        , PHY_ConsumptionType::engineStarted_, ePostureCanModifyDetection | ePostureCanModifyPH | ePostureInstantaneous, &posteReflexe_ );
const PHY_Posture PHY_Posture::posteReflexe_     ( "PosteReflexe"     , eUnitPosture_PosturePosteReflexe     , sword::UnitAttributes::short_stopping                  , PHY_ConsumptionType::engineStarted_, ePostureCanModifyDetection | ePostureCanModifyPH                        , &poste_       , &arret_ );
const PHY_Posture PHY_Posture::poste_            ( "Poste"            , eUnitPosture_PosturePoste            , sword::UnitAttributes::parked                          , PHY_ConsumptionType::engineStopped_, ePostureCanModifyDetection | ePostureCanModifyPH                        , &posteAmenage_, &posteReflexe_ );
const PHY_Posture PHY_Posture::posteAmenage_     ( "PosteAmenage"     , eUnitPosture_PosturePosteAmenage     , sword::UnitAttributes::parked_on_self_prepared_area    , PHY_ConsumptionType::engineStopped_, ePostureCanModifyDetection | ePostureCanModifyPH                        , 0             , &poste_ );
const PHY_Posture PHY_Posture::postePrepareGenie_( "PostePrepareGenie", eUnitPosture_PosturePostePrepareGenie, sword::UnitAttributes::parked_on_engineer_prepared_area, PHY_ConsumptionType::engineStopped_, ePostureCanModifyDetection | ePostureCanModifyPH );

// -----------------------------------------------------------------------------
// Name: PHY_Posture::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Posture::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing postures" );
    postures[ mouvement_        .GetName() ] = &mouvement_;
    postures[ mouvementDiscret_ .GetName() ] = &mouvementDiscret_;
    postures[ arret_            .GetName() ] = &arret_;
    postures[ posteReflexe_     .GetName() ] = &posteReflexe_;
    postures[ poste_            .GetName() ] = &poste_;
    postures[ posteAmenage_     .GetName() ] = &posteAmenage_;
    postures[ postePrepareGenie_.GetName() ] = &postePrepareGenie_;
    const auto descriptor = sword::UnitAttributes_Posture_descriptor();
    for( int i = 0; i < descriptor->value_count(); ++i )
        FindPosture( static_cast< sword::UnitAttributes_Posture >( descriptor->value( i )->number() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Posture::Terminate()
{
    postures.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Posture::PHY_Posture( const std::string& strName, E_UnitPosture nType, sword::UnitAttributes_Posture nAsnID, const PHY_ConsumptionType& consumptionMode, unsigned int nFlags, const PHY_Posture* pNextAutoPosture, const PHY_Posture* pPreviousAutoPosture )
    : strName_             ( strName )
    , nType_               ( static_cast< unsigned int >( nType ) )
    , nAsnID_              ( nAsnID )
    , nFlags_              ( nFlags )
    , consumptionMode_     ( consumptionMode )
    , pNextAutoPosture_    ( pNextAutoPosture )
    , pPreviousAutoPosture_( pPreviousAutoPosture )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Posture::~PHY_Posture()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::GetPostureCount
// Created: MCO 2013-08-27
// -----------------------------------------------------------------------------
std::size_t PHY_Posture::GetPostureCount()
{
    return postures.size();
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::CanModifyPH
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
bool PHY_Posture::CanModifyPH() const
{
    return ( nFlags_ & ePostureCanModifyPH ) == ePostureCanModifyPH;
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::CanModifyDetection
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
bool PHY_Posture::CanModifyDetection() const
{
    return ( nFlags_ & ePostureCanModifyDetection ) == ePostureCanModifyDetection;
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::IsInstantaneous
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
bool PHY_Posture::IsInstantaneous() const
{
    return ( nFlags_ & ePostureInstantaneous ) == ePostureInstantaneous;
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::FindPosture
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const PHY_Posture* PHY_Posture::FindPosture( const std::string& strName )
{
    auto it = postures.find( strName );
    return it == postures.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::FindPosture
// Created: MCO 2013-08-27
// -----------------------------------------------------------------------------
const PHY_Posture& PHY_Posture::FindPosture( sword::UnitAttributes_Posture nAsnID )
{
    for( auto it = postures.begin(); it != postures.end(); ++it )
        if( it->second->nAsnID_ == nAsnID )
            return *it->second;
    throw MASA_EXCEPTION( "posture mapping mismatch between internal and protocol" );
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::FindPosture
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_Posture* PHY_Posture::FindPosture( unsigned int nID )
{
    for( auto it = postures.begin(); it != postures.end(); ++it )
        if( it->second->nType_ == nID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const std::string& PHY_Posture::GetName() const
{
    return strName_;
}
// -----------------------------------------------------------------------------
// Name: PHY_Posture::GetID
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
unsigned int PHY_Posture::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
sword::UnitAttributes_Posture PHY_Posture::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::GetNextAutoPosture
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_Posture* PHY_Posture::GetNextAutoPosture() const
{
    return pNextAutoPosture_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::GetPreviousAutoPosture
// Created: SLI 2013-01-24
// -----------------------------------------------------------------------------
const PHY_Posture* PHY_Posture::GetPreviousAutoPosture() const
{
    return pPreviousAutoPosture_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::GetConsumptionMode
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
const PHY_ConsumptionType& PHY_Posture::GetConsumptionMode() const
{
    return consumptionMode_;
}
