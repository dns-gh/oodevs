// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-11-19 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Weapons_Data.cpp $
// $Author: Nld $
// $Modtime: 3/05/05 10:03 $
// $Revision: 13 $
// $Workfile: ADN_Weapons_Data.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Weapons_Data_WeaponInfos.h"
#include "moc_ADN_Weapons_Data_WeaponInfos.cpp"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Categories_Data.h"
#include "ADN_Weapons_Data_PhInfos.h"
#include "ADN_Tr.h"
#include "ADN_WorkspaceElement.h"

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_WeaponInfos::ADN_Weapons_Data_WeaponInfos
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Weapons_Data_WeaponInfos::ADN_Weapons_Data_WeaponInfos()
    : ptrLauncher_      ( ADN_Workspace::GetWorkspace().GetLaunchers().GetData().GetLaunchersInfos(), 0 )
    , ptrAmmunition_    ( (ADN_Resources_Data::T_AmmoCategoryInfo_Vector&)ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( eDotationFamily_Munition ).categories_, 0 )
    , nRoundsPerBurst_  ( 1 )
    , burstDuration_    ( "1s" )
    , nRoundsPerReload_ ( 1 )
    , reloadDuration_   ( "1s" )
    , bDirect_        ( false )
    , phs_            ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Volumes_Data >( eVolumes ).GetVolumesInfos() )
    , bSimulation_    ( false )
    , nFirePosture_   ( eUnitPosture_PostureArret )
    , nTargetPosture_ ( eUnitPosture_PostureArret )
    , nExperience_    ( eUnitExperience_Veteran )
    , nTiredness_     ( eUnitTiredness_Normal )
    , nStress_        ( eUnitStress_Calm )
    , bIndirect_    ( false )
    , rAverageSpeed_( 0 )
    , rMinRange_    ( 0 )
    , rMaxRange_    ( 0 )
{
    BindExistenceTo( &ptrLauncher_ );
    BindExistenceTo( &ptrAmmunition_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_WeaponInfos destructor
// Created: JSR 2013-07-29
// -----------------------------------------------------------------------------
ADN_Weapons_Data_WeaponInfos::~ADN_Weapons_Data_WeaponInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_WeaponInfos::CreateCopy
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
ADN_Weapons_Data_WeaponInfos* ADN_Weapons_Data_WeaponInfos::CreateCopy()
{
    ADN_Weapons_Data_WeaponInfos* pCopy = new ADN_Weapons_Data_WeaponInfos();
    pCopy->ptrLauncher_ = ptrLauncher_.GetData();
    pCopy->ptrAmmunition_ = ptrAmmunition_.GetData();
    pCopy->nRoundsPerBurst_ = nRoundsPerBurst_.GetData();
    pCopy->burstDuration_ = burstDuration_.GetData();
    pCopy->nRoundsPerReload_ = nRoundsPerReload_.GetData();
    pCopy->reloadDuration_ = reloadDuration_.GetData();
    pCopy->bDirect_ = bDirect_.GetData();

    for( uint n = 0; n < phs_.size(); ++n )
    {
        ADN_Weapons_Data_PhSizeInfos& phSizeInfo = * phs_[n];
        ADN_Weapons_Data_PhSizeInfos& phSizeInfoCopy = * pCopy->phs_[n];
        for( auto it = phSizeInfo.vPhs_.begin(); it != phSizeInfo.vPhs_.end(); ++it )
            phSizeInfoCopy.vPhs_.AddItem( (*it)->CreateCopy() );
    }

    pCopy->bSimulation_ = bSimulation_.GetData();
    pCopy->nFirePosture_ = nFirePosture_.GetData();
    pCopy->nTargetPosture_ = nTargetPosture_.GetData();
    pCopy->nExperience_ = nExperience_.GetData();
    pCopy->nTiredness_ = nTiredness_.GetData();
    pCopy->nStress_ = nStress_.GetData();

    pCopy->bIndirect_ = bIndirect_.GetData();
    pCopy->rAverageSpeed_ = rAverageSpeed_.GetData();
    pCopy->rMinRange_ = rMinRange_.GetData();
    pCopy->rMaxRange_ = rMaxRange_.GetData();

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_WeaponInfos::ReadTargetSize
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Weapons_Data_WeaponInfos::ReadTargetSize( xml::xistream& input )
{
    bDirect_ = true;
    std::string target = input.attribute< std::string >( "target" );
    auto itPhSizeInfo = std::find_if( phs_.begin(), phs_.end(), ADN_Weapons_Data_PhSizeInfos::Cmp( target ) );
    if( itPhSizeInfo != phs_.end() )
        (*itPhSizeInfo)->ReadArchive( input );
}

namespace
{
    template< typename T >
    bool Check( const T& data, const char* name )
    {
        if( !data.GetData() )
        {
            ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, tools::translate( "ADN_Weapons_Data_WeaponInfos", "Weapon system" ).toStdString(), eWeapons, -1, tools::translate( "ADN_Weapons_Data_WeaponInfos", name ).arg( data.GetRefName().c_str() ).toStdString() );
            return false;
        }
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_WeaponInfos::ReadArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data_WeaponInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "launcher", ptrLauncher_ )
          >> xml::attribute( "munition", ptrAmmunition_ );

    if( !Check( ptrLauncher_, "Launcher %1" ) || !Check( ptrAmmunition_, "Ammunition %1" ) )
        return;

    connect( &ptrLauncher_.GetData()->strName_, SIGNAL( DataChanged(void*)), this, SLOT(OnNameChanged(void*) ));
    connect( &ptrAmmunition_.GetData()->strName_, SIGNAL( DataChanged(void*)), this, SLOT(OnNameChanged(void*) ));
    OnNameChanged(0);

    input >> xml::start( "burst" )
            >> xml::attribute( "munition", nRoundsPerBurst_ )
            >> xml::attribute( "duration", burstDuration_ )
          >> xml::end
          >> xml::start( "reloading" )
            >> xml::attribute( "munition", nRoundsPerReload_ )
            >> xml::attribute( "duration", reloadDuration_ )
          >> xml::end
          >> xml::optional
          >> xml::start( "direct-fire" )
            >> xml::list( "hit-probabilities", *this, &ADN_Weapons_Data_WeaponInfos::ReadTargetSize )
          >> xml::end
          >> xml::optional
          >> xml::start( "indirect-fire" )
            >> xml::attribute( "average-speed", rAverageSpeed_ )
            >> xml::attribute( "min-range",     rMinRange_ )
            >> xml::attribute( "max-range",     rMaxRange_ )
          >> xml::end;

    bIndirect_ = rMaxRange_ != .0;
    ConnectLauncherAmmunition();
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_WeaponInfos::WriteArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data_WeaponInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "weapon-system" )
        << xml::attribute( "launcher", ptrLauncher_ )
        << xml::attribute( "munition", ptrAmmunition_ )
        << xml::start( "burst" )
        << xml::attribute( "munition", nRoundsPerBurst_ )
        << xml::attribute( "duration", burstDuration_ )
        << xml::end
        << xml::start( "reloading" )
        << xml::attribute( "munition", nRoundsPerReload_ )
        << xml::attribute( "duration", reloadDuration_ )
        << xml::end;

    if( bDirect_.GetData() )
    {
        output << xml::start( "direct-fire" );
        for( auto it = phs_.begin(); it != phs_.end(); ++it )
            (*it)->WriteArchive( output );
        output << xml::end;
    }

    if( bIndirect_.GetData() )
    {
        output << xml::start( "indirect-fire" )
            << xml::attribute( "average-speed", rAverageSpeed_ )
            << xml::attribute( "min-range",     rMinRange_ )
            << xml::attribute( "max-range",     rMaxRange_ )
            << xml::end;
    }

    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_WeaponInfos::CheckDatabaseValidity
// Created: LDC 2014-03-21
// -----------------------------------------------------------------------------
void ADN_Weapons_Data_WeaponInfos::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    if( bIndirect_.GetData() && rMaxRange_.GetData() < rMinRange_.GetData() )
        checker.AddError( eIndirectFireRange, strName_.GetData(), eWeapons );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_WeaponInfos::ConnectLauncherAmmunition
// Created: MMC 2013-03-13
// -----------------------------------------------------------------------------
void ADN_Weapons_Data_WeaponInfos::ConnectLauncherAmmunition()
{
    if( !ptrLauncher_.GetData() || !ptrAmmunition_.GetData() )
        return;
    LauncherOrAmmunitionChanged();
    connect( &ptrLauncher_.GetData()->bDirect_, SIGNAL( DataChanged( void* ) ), this, SLOT( LauncherOrAmmunitionChanged() ) );
    connect( &ptrLauncher_.GetData()->bIndirect_, SIGNAL( DataChanged( void* ) ), this, SLOT( LauncherOrAmmunitionChanged() ) );
    connect( &ptrAmmunition_.GetData()->bDirect_, SIGNAL( DataChanged( void* ) ), this, SLOT( LauncherOrAmmunitionChanged() ) );
    connect( &ptrAmmunition_.GetData()->bIndirect_, SIGNAL( DataChanged( void* ) ), this, SLOT( LauncherOrAmmunitionChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_WeaponInfos::IsValidOnChange
// Created: MMC 2013-03-13
// -----------------------------------------------------------------------------
bool ADN_Weapons_Data_WeaponInfos::IsValidOnChange( bool bDirect, bool bLauncher ) const
{
    if( !ptrLauncher_.GetData() || !ptrAmmunition_.GetData() )
        return false;
    bool bNewLauncherDirect    = ( bLauncher && bDirect )? !ptrLauncher_.GetData()->bDirect_.GetData() : ptrLauncher_.GetData()->bDirect_.GetData();
    bool bNewLauncherIndirect  = ( bLauncher && !bDirect )? !ptrLauncher_.GetData()->bIndirect_.GetData() : ptrLauncher_.GetData()->bIndirect_.GetData();
    bool bNewAmmoDirect        = ( !bLauncher && bDirect )? !ptrAmmunition_.GetData()->bDirect_.GetData() : ptrAmmunition_.GetData()->bDirect_.GetData();
    bool bNewAmmoIndirect      = ( !bLauncher && !bDirect )? !ptrAmmunition_.GetData()->bIndirect_.GetData() : ptrAmmunition_.GetData()->bIndirect_.GetData();
    return ( bNewLauncherDirect && bNewAmmoDirect ) || ( bNewLauncherIndirect && bNewAmmoIndirect );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_WeaponInfos::LauncherOrAmmunitionChanged
// Created: MMC 2013-03-13
// -----------------------------------------------------------------------------
void ADN_Weapons_Data_WeaponInfos::LauncherOrAmmunitionChanged()
{
    if( !ptrLauncher_.GetData() || !ptrAmmunition_.GetData() )
        return;
    bDirect_ = ptrLauncher_.GetData()->bDirect_.GetData() && ptrAmmunition_.GetData()->bDirect_.GetData();
    bIndirect_ = ptrLauncher_.GetData()->bIndirect_.GetData() && ptrAmmunition_.GetData()->bIndirect_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_WeaponInfos::OnNameChanged
// Created: LGY 2013-07-24
// -----------------------------------------------------------------------------
void ADN_Weapons_Data_WeaponInfos::OnNameChanged(void*)
{
    strName_ = ( ptrLauncher_.GetData() ? ptrLauncher_.GetData()->strName_.GetData() : "" )
        + " & "
        + ( ptrAmmunition_.GetData() ? ptrAmmunition_.GetData()->strName_.GetData() : "" );
}
