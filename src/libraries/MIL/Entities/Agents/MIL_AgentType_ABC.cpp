// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/MIL_AgentType_ABC.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 17:50 $
// $Revision: 8 $
// $Workfile: MIL_AgentType_ABC.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "MIL_AgentType_ABC.h"

#include "Entities/Agents/Units/Categories/PHY_NatureLevel.h"
#include "Entities/Agents/Units/Categories/PHY_NatureWeapon.h"
#include "Entities/Agents/Units/Categories/PHY_NatureSpecialization.h"
#include "Entities/Agents/Units/Categories/PHY_NatureQualifier.h"
#include "Entities/Agents/Units/Categories/PHY_NatureCategory.h"
#include "Entities/Agents/Units/Categories/PHY_NatureMobility.h"
#include "Entities/Agents/Units/Categories/PHY_NatureAtlas.h"
#include "Entities/Agents/Units/Categories/PHY_MissionCapacity.h"


// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC constructor
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
MIL_AgentType_ABC::MIL_AgentType_ABC( const std::string& strName, MIL_InputArchive& archive )
    : nID_                  ( 0 )
    , strName_              ( strName )
    , pNatureLevel_         ( 0 )
    , pNatureWeapon_        ( 0 )
    , pNatureSpecialization_( 0 )
    , pNatureQualifier_     ( 0 )
    , pNatureCategory_      ( 0 )
    , pNatureMobility_      ( 0 )
    , pNatureAtlas_         ( 0 )
    , pCapaciteMission_     ( 0 )
{
    archive.ReadField( "MosID", nID_ );
    InitializeNature( archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC destructor
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
MIL_AgentType_ABC::~MIL_AgentType_ABC()
{

}


//-----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC::InitializeNature
// Created: NLD 2003-05-12
// Modified: JVT 2004-12-09
//-----------------------------------------------------------------------------
void MIL_AgentType_ABC::InitializeNature( MIL_InputArchive& archive )
{
    std::string strBuf;

    archive.Section( "Nature" );

    // Niveau
    archive.Section( "Niveau" );
    archive.ReadAttribute( "type", strBuf );
    pNatureLevel_ = PHY_NatureLevel::Find( strBuf );
    if ( !pNatureLevel_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown nature level type", archive.GetContext() );
    archive.EndSection(); // Niveau

    // Arme
    archive.Section( "Arme" );
    archive.ReadAttribute( "type", strBuf );
    pNatureWeapon_ = PHY_NatureWeapon::Find( strBuf );
    if ( !pNatureWeapon_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown nature weapon type", archive.GetContext() );
    archive.EndSection(); // Arme

    // Spécialisation
    archive.Section( "Specialisation" );
    archive.ReadAttribute( "type", strBuf );
    pNatureSpecialization_ = PHY_NatureSpecialization::Find( strBuf );
    if ( !pNatureSpecialization_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown specialization type", archive.GetContext() );
    archive.EndSection(); // Specialisation

    // Qualificatif
    archive.Section( "Qualificatif" );
    archive.ReadAttribute( "type", strBuf );
    pNatureQualifier_ = PHY_NatureQualifier::Find( strBuf );
    if ( !pNatureQualifier_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown qualificatif type", archive.GetContext() );
    archive.EndSection(); // Qualificatif

    // Catégorie
    archive.Section( "Categorie" );
    archive.ReadAttribute( "type", strBuf );
    pNatureCategory_ = PHY_NatureCategory::Find( strBuf );
    if ( !pNatureCategory_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown category type", archive.GetContext() );
    archive.EndSection(); // Categorie

    // Type mobilité
    archive.Section( "TypeMobilite" );
    archive.ReadAttribute( "type", strBuf );
    pNatureMobility_ = PHY_NatureMobility::Find( strBuf );
    if ( !pNatureMobility_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown mobility type", archive.GetContext() );
    archive.EndSection(); // TypeMobilite

    // Qualificatif Atlas
    archive.Section( "QualificatifAtlas" );
    archive.ReadAttribute( "type", strBuf );
    pNatureAtlas_ = PHY_NatureAtlas::Find( strBuf );
    if ( !pNatureAtlas_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown atlas qualificatif", archive.GetContext() );
    archive.EndSection(); // QualificatifAtlas

    // Capacite Mission
    archive.Section( "CapaciteMission" );
    archive.ReadAttribute( "type", strBuf );
    pCapaciteMission_ = PHY_MissionCapacity::Find( strBuf );
    if ( !pCapaciteMission_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown mission capacity type", archive.GetContext() );
    archive.EndSection(); // CapaciteMission

    archive.EndSection(); // Nature
}
