// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/PHY_UnitType.cpp $
// $Author: Nld $
// $Modtime: 8/03/05 15:37 $
// $Revision: 9 $
// $Workfile: PHY_UnitType.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_UnitType.h"
#include "Postures/PHY_Posture.h"
#include "Composantes/PHY_ComposanteTypePion.h"
#include "Composantes/PHY_ComposantePion.h"
#include "Humans/PHY_HumanRank.h"
#include "Tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::sComposanteTypeData::sComposanteTypeData
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
PHY_UnitType::sComposanteTypeData::sComposanteTypeData()
    : nNbr_     ( 0 )
    , bMajor_   ( false )
    , bLoadable_( false )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType constructor
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
PHY_UnitType::PHY_UnitType( MIL_InputArchive& archive )
    : dotationCapacitiesTC1_          ( "ContenanceTC1", archive )    
    , stockCapacities_                ( "Stocks"       , archive )
    , postureTimes_                   ( PHY_Posture::GetPostures().size(), 0 )
    , rCoupDeSondeLength_             ( 0. )
    , rCoefDecontaminationPerTimeStep_( 0. )
    , bCanFly_                        ( false )
    , bIsAutonomous_                  ( false )
{
    InitializeComposantes         ( archive );
    InitializeCommanderRepartition( archive );
    InitializePostureTimes        ( archive );
    InitializeCoupDeSonde         ( archive );
    InitializeNBC                 ( archive );

    if( archive.Section( "PeutVoler", MIL_InputArchive::eNothing ) )
    {
        bCanFly_ = true;
        archive.EndSection(); // PeutVoler
    }

    if( archive.Section( "EstAutonome", MIL_InputArchive::eNothing ) ) 
    {
        bIsAutonomous_ = true;
        archive.EndSection(); // EstAutonome
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType destructor
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
PHY_UnitType::~PHY_UnitType()
{

}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InitializeNBC
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_UnitType::InitializeNBC( MIL_InputArchive& archive )
{
    float rTmp;
    archive.ReadTimeField( "DelaiDecontaminationNBC", rTmp, CheckValueGreater( 0. ) );
    rCoefDecontaminationPerTimeStep_ = 1. / MIL_Tools::ConvertSecondsToSim( rTmp );
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InitializeComposantes
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
void PHY_UnitType::InitializeComposantes( MIL_InputArchive& archive )
{
    archive.BeginList( "Equipements" );

    while( archive.NextListElement() )
    {
        archive.Section( "Equipement" );

        std::string strComposanteType;
        archive.ReadAttribute( "nom", strComposanteType );

        const PHY_ComposanteTypePion* pComposanteType = PHY_ComposanteTypePion::FindComposanteType( strComposanteType );
        if( !pComposanteType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown composante type", archive.GetContext() );

        if( composanteTypes_.find( pComposanteType ) != composanteTypes_.end() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Composante type already exist", archive.GetContext() );

        sComposanteTypeData& compData = composanteTypes_[ pComposanteType ];
        compData.bMajor_ = false;
        archive.ReadAttribute( "majeur", compData.bMajor_, MIL_InputArchive::eNothing );
        
        compData.bLoadable_ = false;
        archive.ReadAttribute( "embarquable", compData.bLoadable_, MIL_InputArchive::eNothing );

        archive.Read( compData.nNbr_ );
        archive.EndSection(); // Equipement
    }

    archive.EndList(); // Equipements
}


// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InitializeCommanderRepartition
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
void PHY_UnitType::InitializeCommanderRepartition( MIL_InputArchive& archive )
{
    if( !archive.Section( "RepartitionDuCommandement", MIL_InputArchive::eNothing ) )
        return;

    const PHY_HumanRank::T_HumanRankMap& ranks = PHY_HumanRank::GetHumanRanks();
    for( PHY_HumanRank::CIT_HumanRankMap it = ranks.begin(); it != ranks.end(); ++it )
    {
        const PHY_HumanRank& rank = *it->second;
        if( !rank.IsCommander() )
            continue;

        if( commandersRepartition_.find( &rank ) != commandersRepartition_.end() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );        

        uint nValue = 0;
        if( !archive.ReadField( rank.GetName(), nValue, MIL_InputArchive::eNothing ) )
            continue;

        commandersRepartition_[ &rank ] = nValue;
    }

    archive.EndSection(); // RepartitionDuCommandement
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InitializePostureTimes
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
void PHY_UnitType::InitializePostureTimes( MIL_InputArchive& archive )
{
    archive.Section( "TempsMiseEnPosture" );

    const PHY_Posture::T_PostureMap& postures = PHY_Posture::GetPostures();
    for( PHY_Posture::CIT_PostureMap it = postures.begin(); it != postures.end(); ++it )
    {
        const PHY_Posture& posture = *it->second;
        if ( posture.IsInstantaneous() )
        {
            // Pour forcer la 'non-déclaration' dans le xml
            if ( archive.Section( posture.GetName(), MIL_InputArchive::eNothing ) )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Posture is instantaneous and has no time", archive.GetContext() );
            continue;
        }
        
        archive.Section( posture.GetName() );

        assert( postureTimes_.size() > posture.GetID() );
        MT_Float rTime;
        archive.ReadTime( rTime );

        postureTimes_[ posture.GetID() ] = (uint)MIL_Tools::ConvertSecondsToSim( rTime );
    
        archive.EndSection(); // Posture
    }
    archive.EndSection(); // TempsMiseEnPosture
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InitializeCoupDeSonde
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
void PHY_UnitType::InitializeCoupDeSonde( MIL_InputArchive& archive )
{
    if( !archive.Section( "CoupDeSonde", MIL_InputArchive::eNothing ) )
    {
        rCoupDeSondeLength_ = 0.;
        rCoupDeSondeWidth_  = 0.;
        return;
    }

    archive.ReadField( "Largeur" , rCoupDeSondeWidth_ );
    archive.ReadField( "Longueur", rCoupDeSondeLength_ );

    if ( rCoupDeSondeLength_ < rCoupDeSondeWidth_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Length (%f) should be greater than width (%f)", rCoupDeSondeLength_, rCoupDeSondeWidth_), archive.GetContext() );

    archive.EndSection(); // CoupDeSonde

    rCoupDeSondeLength_ = MIL_Tools::ConvertMeterToSim( rCoupDeSondeLength_ );
    rCoupDeSondeWidth_  = MIL_Tools::ConvertMeterToSim( rCoupDeSondeWidth_  );
}


// -----------------------------------------------------------------------------
// Name: PHY_UnitType::GetPostureTime
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MT_Float PHY_UnitType::GetPostureTime( const PHY_Posture& posture ) const
{
    assert( postureTimes_.size() > posture.GetID() );
    return postureTimes_[ posture.GetID() ];
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_UnitType::InstanciateComposantes
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
void PHY_UnitType::InstanciateComposantes( PHY_RolePion_Composantes& role ) const
{
    for( CIT_ComposanteTypeMap itComposanteType = composanteTypes_.begin(); itComposanteType != composanteTypes_.end(); ++itComposanteType )
    {
        const PHY_ComposanteTypePion& compType = *itComposanteType->first;
        const sComposanteTypeData&    compData =  itComposanteType->second;

        for( uint i = 0; i < compData.nNbr_; ++i )
            compType.InstanciateComposante( role, compData.bMajor_, compData.bLoadable_ );
    }
}