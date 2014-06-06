//*****************************************************************************
//
// $Created: JDY 03-07-11 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Launchers_Data.cpp $
// $Author: Ape $
// $Modtime: 22/04/05 15:54 $
// $Revision: 13 $
// $Workfile: ADN_Launchers_Data.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Launchers_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "clients_kernel/XmlTranslations.h"
#include "ENT/ENT_Tr.h"

//-----------------------------------------------------------------------------
// Name: LauncherInfos::LauncherInfos
// Created: JDY 03-07-15
//-----------------------------------------------------------------------------
ADN_Launchers_Data::LauncherInfos::LauncherInfos()
    : tabModifs_( eNbrUnitPosture, eNbrUnitPosture )
    , bDirect_( false )
    , bIndirect_( false )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eLaunchers, "launchers" ) );
    for( int i = 0; i < eNbrUnitPosture; ++i )
        for( int j = 0; j < eNbrUnitPosture; ++j )
            tabModifs_.Get( i, j ) = 0.0;
}

//-----------------------------------------------------------------------------
// Name: LauncherInfos::LauncherInfos
// Created: JDY 03-07-15
//-----------------------------------------------------------------------------
ADN_Launchers_Data::LauncherInfos::~LauncherInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Launchers_Data::LauncherInfos::ReadPosture
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Launchers_Data::LauncherInfos::ReadPosture( xml::xistream& input )
{
    bDirect_ = true;
    const std::string posture = input.attribute< std::string >( "posture" );
    input >> xml::list( "ph-modifier", *this, &ADN_Launchers_Data::LauncherInfos::ReadPh, posture );
}

// -----------------------------------------------------------------------------
// Name: ADN_Launchers_Data::LauncherInfos::ReadPh
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Launchers_Data::LauncherInfos::ReadPh( xml::xistream& input, const std::string& posture )
{
    const std::string targetPosture = input.attribute< std::string >( "target-posture" );
    int fire = -1;
    int target = -1;
    for( int iPosture = 0; iPosture < eNbrUnitPosture; ++iPosture )
    {
        const std::string current = ADN_Tools::ComputePostureScriptName( static_cast< E_UnitPosture >( iPosture ) );
        if( posture == current )
            fire = iPosture;
        if( targetPosture == current )
            target = iPosture;
    }
    tabModifs_.Get( fire, target ) = input.attribute< double >( "value" ) * 100;
}

// -----------------------------------------------------------------------------
// Name: ADN_Launchers_Data::ReadArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Launchers_Data::LauncherInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", *this )
          >> xml::optional >> xml::attribute( "indirect-fire", bIndirect_ )
          >> xml::list( "ph-modifiers", *this, &ADN_Launchers_Data::LauncherInfos::ReadPosture );
}

// -----------------------------------------------------------------------------
// Name: LauncherInfos::WriteArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Launchers_Data::LauncherInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "launcher" )
           << xml::attribute( "name", *this );
    if( bDirect_.GetData() == true )
        for( int iPostureTireur=0; iPostureTireur< eNbrUnitPosture;++iPostureTireur)
        {
            output << xml::start( "ph-modifiers" )
                    << xml::attribute( "posture", ADN_Tools::ComputePostureScriptName((E_UnitPosture)iPostureTireur) );
            for( int iPostureCible=0; iPostureCible< eNbrUnitPosture;++iPostureCible)
            {
                output << xml::start( "ph-modifier" )
                        << xml::attribute( "target-posture", ADN_Tools::ComputePostureScriptName((E_UnitPosture)iPostureCible) )
                        << xml::attribute( "value", std::abs( tabModifs_.Get(iPostureTireur,iPostureCible).GetData() / 100.0 ) )
                       << xml::end;
            }
            output << xml::end;
        }
    if( bIndirect_.GetData() == true )
        output << xml::attribute( "indirect-fire", bIndirect_ );

    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: LauncherInfos::CreateCopy
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Launchers_Data::LauncherInfos* ADN_Launchers_Data::LauncherInfos::CreateCopy()
{
    LauncherInfos* pCopy = new LauncherInfos();
    for( uint i = 0; i < eNbrUnitPosture; ++i )
        for( uint j = 0; j < eNbrUnitPosture; ++j )
            pCopy->tabModifs_.Get( i, j ) = tabModifs_.Get( i, j ).GetData();
    pCopy->bDirect_ = this->bDirect_.GetData();
    pCopy->bIndirect_ = this->bIndirect_.GetData();
    return pCopy;
}

//-----------------------------------------------------------------------------
// Name: ADN_Launchers_Data constructor
// Created: JDY 03-07-11
//-----------------------------------------------------------------------------
ADN_Launchers_Data::ADN_Launchers_Data()
    : ADN_Data_ABC( eLaunchers )
{
    vLaunchers_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
}

//-----------------------------------------------------------------------------
// Name: ADN_Launchers_Data destructor
// Created: JDY 03-07-11
//-----------------------------------------------------------------------------
ADN_Launchers_Data::~ADN_Launchers_Data()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Launchers_Data::FilesNeeded
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
void ADN_Launchers_Data::FilesNeeded( tools::Path::T_Paths& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szLaunchers_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Launchers_Data::ReadLauncher
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Launchers_Data::ReadLauncher( xml::xistream& input )
{
    std::unique_ptr<LauncherInfos> spNew( new LauncherInfos() );
    spNew->ReadArchive( input );
    vLaunchers_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Launchers_Data::ReadArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Launchers_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "launchers" )
            >> xml::list( "launcher", *this, &ADN_Launchers_Data::ReadLauncher )
          >> xml::end;
    vLaunchers_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_Launchers_Data::WriteArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Launchers_Data::WriteArchive( xml::xostream& output ) const
{
    if( vLaunchers_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "launchers" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Launchers" );
    for( auto it = vLaunchers_.begin(); it != vLaunchers_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}
