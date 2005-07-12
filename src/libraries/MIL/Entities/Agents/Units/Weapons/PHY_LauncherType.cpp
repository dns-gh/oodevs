// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Weapons/PHY_LauncherType.cpp $
// $Author: Age $
// $Modtime: 12/04/05 15:40 $
// $Revision: 4 $
// $Workfile: PHY_LauncherType.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_LauncherType.h"

#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Roles/Posture/PHY_RolePion_Posture.h"

// =============================================================================
// MANAGER
// =============================================================================

PHY_LauncherType::T_LauncherTypeMap PHY_LauncherType::launcherTypes_;

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::Initialize
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void PHY_LauncherType::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing launcher types" );

    archive.BeginList( "Lanceurs" );

    while( archive.NextListElement() )
    {
        archive.Section( "Lanceur" );

        std::string strLauncherName;
        archive.ReadAttribute( "nom", strLauncherName );

        const PHY_LauncherType*& pLauncherType = launcherTypes_[ strLauncherName ];
        if ( pLauncherType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Launcher type already registered", archive.GetContext() );

        pLauncherType = new PHY_LauncherType( strLauncherName, archive );

        archive.EndSection();
    }
    archive.EndList(); // Lanceurs
}

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::Terminate
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void PHY_LauncherType::Terminate()
{
    for( CIT_LauncherTypeMap it = launcherTypes_.begin(); it != launcherTypes_.end(); ++it )
        delete it->second;
    launcherTypes_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_LauncherType::PHY_LauncherType( const std::string& strName, MIL_InputArchive& archive )
    : strName_       ( strName )
    , bDirectFire_   ( false )
    , bIndirectFire_ ( false )
    , phModificators_( PHY_Posture::GetPostures().size() )
{
    InitializeForDirectFire  ( archive );
    InitializeForIndirectFire( archive );
}

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_LauncherType::~PHY_LauncherType()
{
    phModificators_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::InitializeForIndirectFire
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_LauncherType::InitializeForIndirectFire( MIL_InputArchive& archive )
{
    if ( !archive.Section( "Indirect", MIL_InputArchive::eNothing ) )
        return;
    bIndirectFire_ = true;
    archive.EndSection(); // Indirect
}

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::InitializeForDirectFire
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_LauncherType::InitializeForDirectFire( MIL_InputArchive& archive )
{
    if( !archive.Section( "Direct", MIL_InputArchive::eNothing ) )
        return;

    bDirectFire_ = true;

    archive.Section( "ModificateursPH" );

    const PHY_Posture::T_PostureMap& postures = PHY_Posture::GetPostures();
    for ( PHY_Posture::CIT_PostureMap itPostureSource = postures.begin(); itPostureSource != postures.end(); ++itPostureSource )
    {
        const PHY_Posture& postureSource = *itPostureSource->second;
        if( !postureSource.CanModifyPH() )
            continue;

        std::stringstream strSectionSource;
        strSectionSource << "TireurEn" << postureSource.GetName();
        archive.Section( strSectionSource.str() );

        for ( PHY_Posture::CIT_PostureMap itPostureTarget = postures.begin(); itPostureTarget != postures.end(); ++itPostureTarget )
        {
            const PHY_Posture& postureTarget = *itPostureTarget->second;
            if( !postureTarget.CanModifyPH() )
                continue;

            std::stringstream strSectionTarget;
            strSectionTarget << "CibleEn" << postureTarget.GetName();
            archive.Section( strSectionTarget.str() );

            MT_Float rModificatorValue;

            archive.Read( rModificatorValue, CheckValueBound( 0., 1. ) );

            RegisterPHModificator( postureSource, postureTarget, rModificatorValue );

            archive.EndSection(); // CibleEnXXX
        }
        archive.EndSection(); // TireurEnXXX
    }

    archive.EndSection(); // ModificateursPH
    
    archive.EndSection(); // Direct
}

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::RegisterPHModificator
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_LauncherType::RegisterPHModificator( const PHY_Posture& postureSource, const PHY_Posture& postureTarget, MT_Float rModificatorValue )
{
    assert( rModificatorValue >= 0. && rModificatorValue <= 1. );
    assert( phModificators_.size() > postureSource.GetID() );

    T_PhModificatorValueVector& phModificatorValues = phModificators_[ postureSource.GetID() ];

    if( phModificatorValues.size() <= postureTarget.GetID() )
        phModificatorValues.resize( PHY_Posture::GetPostures().size() );

    assert( phModificatorValues.size() > postureTarget.GetID() );

    phModificatorValues[ postureTarget.GetID() ] = rModificatorValue;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::GetPHModificator
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
MT_Float PHY_LauncherType::GetPHModificator( const PHY_RolePion_Posture& firerPosture, const PHY_RoleInterface_Posture& targetPosture ) const
{
    const uint     nFirerCurrentPosture      = firerPosture.GetCurrentPosture              ().GetID();
    const uint     nFirerLastPosture         = firerPosture.GetLastPosture                 ().GetID();
    const MT_Float rFirerPostureCompletion   = firerPosture.GetPostureCompletionPercentage ();
    const uint     nTargetCurrentPosture     = targetPosture.GetCurrentPosture             ().GetID();
    const uint     nTargetLastPosture        = targetPosture.GetLastPosture                ().GetID();
    const MT_Float rTargetPostureCompletion  = targetPosture.GetPostureCompletionPercentage();
      
    const T_PhModificatorValueVector& firerCurrentModificator = phModificators_[ nFirerCurrentPosture ];
    const T_PhModificatorValueVector& firerLastModificator    = phModificators_[ nFirerLastPosture ];
    const MT_Float rInverseFirerPostureCompletion             = 1.f - rFirerPostureCompletion;

    return           rTargetPostureCompletion   * ( rFirerPostureCompletion * firerCurrentModificator[ nTargetCurrentPosture ] + rInverseFirerPostureCompletion * firerLastModificator[ nTargetCurrentPosture ] )
           + ( 1.f - rTargetPostureCompletion ) * ( rFirerPostureCompletion * firerCurrentModificator[ nTargetLastPosture ]    + rInverseFirerPostureCompletion * firerLastModificator[ nTargetLastPosture ] );
}

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::GetPHModificator
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
MT_Float PHY_LauncherType::GetPHModificator( const PHY_Posture& firerPosture, const PHY_Posture& targetPosture ) const
{
    return phModificators_[ firerPosture.GetID() ][ targetPosture.GetID() ];
}
