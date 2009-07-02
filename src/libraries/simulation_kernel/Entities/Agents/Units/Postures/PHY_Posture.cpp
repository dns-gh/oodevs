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

PHY_Posture::T_PostureMap PHY_Posture::postures_;

const PHY_Posture PHY_Posture::mouvement_        ( "Mouvement"        , ePostureMouvement        , EnumUnitPosture::posture_mouvement          , PHY_ConsumptionType::moving_       , ePostureCanModifyDetection | ePostureCanModifyPH | ePostureInstantaneous );
const PHY_Posture PHY_Posture::mouvementDiscret_ ( "MouvementDiscret" , ePostureMouvementDiscret , EnumUnitPosture::posture_mouvement_discret  , PHY_ConsumptionType::moving_       , ePostureCanModifyDetection | ePostureCanModifyPH | ePostureInstantaneous );
const PHY_Posture PHY_Posture::arret_            ( "Arret"            , ePostureArret            , EnumUnitPosture::posture_arret              , PHY_ConsumptionType::engineStarted_, ePostureCanModifyDetection | ePostureCanModifyPH | ePostureInstantaneous, &posteReflexe_ );
const PHY_Posture PHY_Posture::posteReflexe_     ( "PosteReflexe"     , ePosturePosteReflexe     , EnumUnitPosture::posture_poste_reflexe      , PHY_ConsumptionType::engineStarted_, ePostureCanModifyDetection | ePostureCanModifyPH, &poste_ );
const PHY_Posture PHY_Posture::poste_            ( "Poste"            , ePosturePoste            , EnumUnitPosture::posture_poste              , PHY_ConsumptionType::engineStopped_, ePostureCanModifyDetection | ePostureCanModifyPH, &posteAmenage_ );
const PHY_Posture PHY_Posture::posteAmenage_     ( "PosteAmenage"     , ePosturePosteAmenage     , EnumUnitPosture::posture_poste_amenage      , PHY_ConsumptionType::engineStopped_, ePostureCanModifyDetection | ePostureCanModifyPH );
const PHY_Posture PHY_Posture::postePrepareGenie_( "PostePrepareGenie", ePosturePostePrepareGenie, EnumUnitPosture::posture_poste_prepare_genie, PHY_ConsumptionType::engineStopped_, ePostureCanModifyDetection | ePostureCanModifyPH );

// -----------------------------------------------------------------------------
// Name: PHY_Posture::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Posture::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing postures" );
    postures_[ mouvement_        .GetName() ] = &mouvement_;
    postures_[ mouvementDiscret_ .GetName() ] = &mouvementDiscret_;
    postures_[ arret_            .GetName() ] = &arret_;
    postures_[ posteReflexe_     .GetName() ] = &posteReflexe_;
    postures_[ poste_            .GetName() ] = &poste_;   
    postures_[ posteAmenage_     .GetName() ] = &posteAmenage_;
    postures_[ postePrepareGenie_.GetName() ] = &postePrepareGenie_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Posture::Terminate()
{
    postures_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Posture::PHY_Posture( const std::string& strName, E_PostureType nType, ASN1T_EnumUnitPosture nAsnID, const PHY_ConsumptionType& consumptionMode, uint nFlags, const PHY_Posture* pNextAutoPosture )
    : strName_         ( strName )
    , nType_           ( nType   )
    , nAsnID_          ( nAsnID  )
    , nFlags_          ( nFlags  )
    , consumptionMode_ ( consumptionMode  )
    , pNextAutoPosture_( pNextAutoPosture )
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
// Name: PHY_Posture::GetPostures
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const PHY_Posture::T_PostureMap& PHY_Posture::GetPostures()
{
    return postures_;
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
    CIT_PostureMap it = postures_.find( strName );
    return it == postures_.end() ? 0: it->second;
}


// -----------------------------------------------------------------------------
// Name: PHY_Posture::FindPosture
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_Posture* PHY_Posture::FindPosture( uint nID )
{
    CIT_PostureMap it = std::find_if( postures_.begin(), postures_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nID ), std::compose1( std::mem_fun( &PHY_Posture::GetID ), std::select2nd< T_PostureMap::value_type >() ) ) );
    return it == postures_.end() ? 0: it->second;
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
uint PHY_Posture::GetID() const
{
    return (uint)nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Posture::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
ASN1T_EnumUnitPosture PHY_Posture::GetAsnID() const
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
// Name: PHY_Posture::GetConsumptionMode
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
const PHY_ConsumptionType& PHY_Posture::GetConsumptionMode() const
{
    return consumptionMode_;
}
