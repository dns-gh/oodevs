// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_NBC_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "clients_kernel/XmlTranslations.h"

tools::IdManager ADN_NBC_Data::idManager_;

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data::NbcIntoxInfos
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
ADN_NBC_Data::NbcIntoxInfos::NbcIntoxInfos()
    : parentName_           ()
    , bIntoxPresent_        ( false )
    , rNbAlivedHumans_      ( 100.0 )
    , rNbHurtedHumans1_     ( 0.0 )
    , rNbHurtedHumans2_     ( 0.0 )
    , rNbHurtedHumans3_     ( 0.0 )
    , rNbHurtedHumansE_     ( 0.0 )
    , rNbDeadHumans_        ( 0.0 )
    , bContaminationPresent_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data::CopyFrom
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ADN_NBC_Data::NbcIntoxInfos::CopyFrom( NbcIntoxInfos& infos )
{
    bIntoxPresent_ = infos.bIntoxPresent_.GetData();
    rNbAlivedHumans_ = infos.rNbAlivedHumans_.GetData();
    rNbHurtedHumans1_ = infos.rNbHurtedHumans1_.GetData();
    rNbHurtedHumans2_ = infos.rNbHurtedHumans2_.GetData();
    rNbHurtedHumans3_ = infos.rNbHurtedHumans3_.GetData();
    rNbHurtedHumansE_ = infos.rNbHurtedHumansE_.GetData();
    rNbDeadHumans_ = infos.rNbDeadHumans_.GetData();
    bContaminationPresent_ = infos.bContaminationPresent_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data::NbcIntoxInfos::ReadEffect
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_NBC_Data::NbcIntoxInfos::ReadEffect( xml::xistream& input )
{
    std::string wound = input.attribute< std::string >( "wound" );
    std::transform( wound.begin(), wound.end(), wound.begin(), std::tolower );
    ADN_Type_Double* pWound =
        wound == "healthy" ? &rNbAlivedHumans_ :
        wound == "u1"      ? &rNbHurtedHumans1_ :
        wound == "u2"      ? &rNbHurtedHumans2_ :
        wound == "u3"      ? &rNbHurtedHumans3_ :
        wound == "ue"      ? &rNbHurtedHumansE_ :
        wound == "dead"    ? &rNbDeadHumans_ :
        0;
    if( pWound )
    {
        *pWound = input.attribute< double >( "percentage" ) * 100.;
        if( pWound->GetData() < 0. || pWound->GetData() > 100. )
            throw MASA_EXCEPTION( tools::translate( "NBC_Data", "NBC - Wound '%1' data < 0 or > 1" ).arg( wound.c_str() ).toStdString() );
    }
    else
        throw MASA_EXCEPTION(tools::translate( "NBC_Data", "NBC - Invalid wound type '%1'" ).arg( wound.c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data::ReadArchive
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ADN_NBC_Data::NbcIntoxInfos::ReadArchive( xml::xistream& input )
{
    input >>  xml::attribute( "affliction", bIntoxPresent_ );
    if( bIntoxPresent_.GetData() )
        input >> xml::list( "effect", *this, &ADN_NBC_Data::NbcIntoxInfos::ReadEffect );
    input >> xml::optional >> xml::attribute( "contamination", bContaminationPresent_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data::WriteArchive
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ADN_NBC_Data::NbcIntoxInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "effects" )
           << xml::attribute( "type", "liquid" )
           << xml::attribute( "affliction", "false" );
    WriteContent( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data::NbcIntoxInfos::WriteContent
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_NBC_Data::NbcIntoxInfos::WriteContent( xml::xostream& output ) const
{
    if( bIntoxPresent_.GetData() )
    {
        output << xml::attribute( "affliction", "true" )
               << xml::start( "effect" )
                << xml::attribute( "wound", "healthy" )
                << xml::attribute( "percentage", rNbAlivedHumans_.GetData() / 100.0 )
               << xml::end
               << xml::start( "effect" )
                << xml::attribute( "wound", "u1" )
                << xml::attribute( "percentage", rNbHurtedHumans1_.GetData() / 100.0 )
               << xml::end
               << xml::start( "effect" )
                << xml::attribute( "wound", "u2" )
                << xml::attribute( "percentage", rNbHurtedHumans2_.GetData() / 100.0 )
               << xml::end
               << xml::start( "effect" )
                << xml::attribute( "wound", "u3" )
                << xml::attribute( "percentage", rNbHurtedHumans3_.GetData() / 100.0 )
               << xml::end
               << xml::start( "effect" )
                << xml::attribute( "wound", "ue" )
                << xml::attribute( "percentage", rNbHurtedHumansE_.GetData() / 100.0 )
               << xml::end
               << xml::start( "effect" )
                << xml::attribute( "wound", "dead" )
                << xml::attribute( "percentage", rNbDeadHumans_.GetData() / 100.0 )
               << xml::end;
    }
    if( bContaminationPresent_.GetData() )
        output << xml::attribute( "contamination", bContaminationPresent_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data::NbcIntoxInfos::CheckDatabaseValidity
// Created: LDC 2014-03-21
// -----------------------------------------------------------------------------
void ADN_NBC_Data::NbcIntoxInfos::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    if( bIntoxPresent_.GetData() )
    {
        if( rNbAlivedHumans_.GetData() + rNbHurtedHumans1_.GetData() + rNbHurtedHumans2_.GetData() + rNbHurtedHumans3_.GetData() + rNbHurtedHumansE_.GetData() + rNbDeadHumans_.GetData() != 100.0 )
            checker.AddError( eInvalidPoisoningData, parentName_, eNBC );
    }
}
    
// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data::CheckDatabaseValidity
// Created: LDC 2014-03-21
// -----------------------------------------------------------------------------
void ADN_NBC_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    for( auto it = vNbcAgent_.begin(); it != vNbcAgent_.end(); ++it )
    {
        auto agent = (*it );
        if( agent->bLiquidPresent_.GetData() )
            agent->liquidInfos_.CheckDatabaseValidity( checker );
        if( agent->bGazPresent_.GetData() )
            agent->gazInfos_.intoxInfos_.CheckDatabaseValidity( checker );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data::NbcGazInfos
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
ADN_NBC_Data::NbcGazInfos::NbcGazInfos()
    : intoxInfos_  ()
    , lifeTime_    ( "1s" )
    , rSpreadAngle_( 20 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data::CopyFrom
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ADN_NBC_Data::NbcGazInfos::CopyFrom( NbcGazInfos& infos )
{
    intoxInfos_.CopyFrom( infos.intoxInfos_ );
    lifeTime_ = infos.lifeTime_.GetData();
    rSpreadAngle_ = infos.rSpreadAngle_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data::ReadArchive
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ADN_NBC_Data::NbcGazInfos::ReadArchive( xml::xistream& input )
{
    intoxInfos_.ReadArchive( input );
    input >> xml::attribute( "life-time", lifeTime_ )
          >> xml::attribute( "propagation", rSpreadAngle_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data::WriteArchive
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ADN_NBC_Data::NbcGazInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "effects" )
            << xml::attribute( "type", "gaseous" )
            << xml::attribute( "affliction", "false" )
            << xml::attribute( "propagation", rSpreadAngle_ )
            << xml::attribute( "life-time", lifeTime_ );
    intoxInfos_.WriteContent( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: NbcAgentInfos::NbcAgentInfos
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
ADN_NBC_Data::NbcAgentInfos::NbcAgentInfos()
    : nId_( ADN_NBC_Data::idManager_.GetNextId() )
    , liquidInfos_()
    , category_( "chemical" )
    , bGazPresent_( false )
    , bLiquidPresent_( false )
    , gazInfos_()
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eNBC, "nbc" ) );
}

// -----------------------------------------------------------------------------
// Name: NbcAgentInfos::NbcAgentInfos
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
ADN_NBC_Data::NbcAgentInfos::NbcAgentInfos( unsigned int id )
    : nId_( id )
    , liquidInfos_()
    , category_( "chemical" )
    , bGazPresent_( false )
    , bLiquidPresent_( false )
    , gazInfos_()
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eNBC, "nbc" ) );
    ADN_NBC_Data::idManager_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: NbcAgentInfos::CreateCopy
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
ADN_NBC_Data::NbcAgentInfos* ADN_NBC_Data::NbcAgentInfos::CreateCopy()
{
    NbcAgentInfos* pCopy = new NbcAgentInfos();
    pCopy->liquidInfos_.CopyFrom( liquidInfos_ );
    pCopy->bLiquidPresent_ = bLiquidPresent_.GetData();
    pCopy->gazInfos_.CopyFrom( gazInfos_ );
    pCopy->bGazPresent_ = bGazPresent_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data::NbcAgentInfos::ReadEffect
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_NBC_Data::NbcAgentInfos::ReadEffect( xml::xistream& input )
{
    const std::string type = input.attribute< std::string >( "type" );
    if( type == "liquid" )
    {
        bLiquidPresent_ = true;
        liquidInfos_.ReadArchive( input );
    }
    else if( type == "gaseous" )
    {
        bGazPresent_ = true;
        gazInfos_.ReadArchive( input );
    }
}

// -----------------------------------------------------------------------------
// Name: NbcAgentInfos::ReadArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_NBC_Data::NbcAgentInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", *this )
          >> xml::optional >> xml::attribute( "category", category_ );
    liquidInfos_.parentName_ = strName_.GetData();
    gazInfos_.intoxInfos_.parentName_ = strName_.GetData();
    input >> xml::list( "effects", *this, &ADN_NBC_Data::NbcAgentInfos::ReadEffect );
    if( category_ == "" )
        category_ = "chemical";
}

// -----------------------------------------------------------------------------
// Name: NbcAgentInfos::WriteArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_NBC_Data::NbcAgentInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "agent" )
           << xml::attribute( "name", *this )
           << xml::attribute( "category", category_ )
           << xml::attribute( "id", nId_ );
    if( bLiquidPresent_.GetData() )
        liquidInfos_.WriteArchive( output );
    if( bGazPresent_.GetData() )
        gazInfos_.WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data constructor
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
ADN_NBC_Data::ADN_NBC_Data()
    : ADN_Data_ABC                  ( eNBC )
    , rContaminationDistance_       ( 0.f )
    , rContaminationQuantityGiven_  ( 0.f )
    , rWindSpeedLimitForSpreading_  ( 0.f )
    , rNbcSuitMaxSpeedMultiplier_   ( 0.f )
    , rNbcSuitReloadSpeedMultiplier_( 0.f )
{
    vNbcAgent_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data destructor
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
ADN_NBC_Data::~ADN_NBC_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data::FilesNeeded
// Created: APE 2004-12-06
// -----------------------------------------------------------------------------
void ADN_NBC_Data::FilesNeeded( tools::Path::T_Paths& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szNBC_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data::ReadAgent
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_NBC_Data::ReadAgent( xml::xistream& input )
{
    std::auto_ptr< NbcAgentInfos > spNew( new NbcAgentInfos( input.attribute< unsigned int >( "id" ) ) );
    spNew->ReadArchive( input );
    vNbcAgent_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data::ReadArchive
// Created: AGN 2004-07-02
// -----------------------------------------------------------------------------
void ADN_NBC_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "nbc" )
            >> xml::start( "propagation" )
                >> xml::attribute( "wind-speed-limit", rWindSpeedLimitForSpreading_ )
                >> xml::attribute( "contamination-distance", rContaminationDistance_ )
                >> xml::attribute( "contamination-quantity-given", rContaminationQuantityGiven_ )
            >> xml::end
            >> xml::start( "nbc-suit" )
                >> xml::attribute( "max-speed-modifier", rNbcSuitMaxSpeedMultiplier_ )
                >> xml::attribute( "reloading-time-modifier", rNbcSuitReloadSpeedMultiplier_ )
            >> xml::end;
    input >> xml::start( "agents" )
            >> xml::list( "agent", *this, &ADN_NBC_Data::ReadAgent )
          >> xml::end;
    vNbcAgent_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data::WriteArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_NBC_Data::WriteArchive( xml::xostream& output ) const
{
    if( vNbcAgent_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "nbc" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "NBC" );
    output  << xml::start( "propagation" )
                << xml::attribute( "wind-speed-limit", rWindSpeedLimitForSpreading_ )
                << xml::attribute( "contamination-distance", rContaminationDistance_ )
                << xml::attribute( "contamination-quantity-given", rContaminationQuantityGiven_ )
            << xml::end
            << xml::start( "nbc-suit" )
                << xml::attribute( "max-speed-modifier", rNbcSuitMaxSpeedMultiplier_ )
                << xml::attribute( "reloading-time-modifier", rNbcSuitReloadSpeedMultiplier_ )
            << xml::end;
    output << xml::start( "agents" );
    for( auto itAgent = vNbcAgent_.begin(); itAgent != vNbcAgent_.end(); ++itAgent )
        (*itAgent)->WriteArchive( output );
    output << xml::end;
    output << xml::end;
}
