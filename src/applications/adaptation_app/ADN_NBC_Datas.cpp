// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_NBC_Datas.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_DataException.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::NbcIntoxInfos
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
ADN_NBC_Datas::NbcIntoxInfos::NbcIntoxInfos( const std::string& nodeName )
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
    , nodeName_             ( nodeName )
    , bIntoxPresent_        ( false )
    , rNbAlivedHumans_      ( 100.0 )
    , rNbHurtedHumans1_     ( 0.0 )
    , rNbHurtedHumans2_     ( 0.0 )
    , rNbHurtedHumans3_     ( 0.0 )
    , rNbHurtedHumansE_     ( 0.0 )
    , rNbDeadHumans_        ( 0.0 )
    , bContaminationPresent_( false )
{
    rNbAlivedHumans_.SetDataName( "le pourcentage d'humains survivants dans" );
    rNbAlivedHumans_.SetParentNode( *this );
    rNbHurtedHumans1_.SetDataName( "le pourcentage d'humains blessés de niveau U1 dans" );
    rNbHurtedHumans1_.SetParentNode( *this );
    rNbHurtedHumans2_.SetDataName( "le pourcentage d'humains blessés de niveau U2 dans" );
    rNbHurtedHumans2_.SetParentNode( *this );
    rNbHurtedHumans3_.SetDataName( "le pourcentage d'humains blessés de niveau U3 dans" );
    rNbHurtedHumans3_.SetParentNode( *this );
    rNbHurtedHumansE_.SetDataName( "le pourcentage d'humains blessés de niveau UE dans" );
    rNbHurtedHumansE_.SetParentNode( *this );
    rNbDeadHumans_.SetDataName( "le pourcentage d'humains décédés dans" );
    rNbDeadHumans_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::GetNodeName
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
std::string ADN_NBC_Datas::NbcIntoxInfos::GetNodeName()
{
    return nodeName_;
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::GetItemName
// Created: SBO 2006-10-31
// -----------------------------------------------------------------------------
std::string ADN_NBC_Datas::NbcIntoxInfos::GetItemName()
{
    return nodeName_;
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::CopyFrom
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::NbcIntoxInfos::CopyFrom( NbcIntoxInfos& infos )
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
// Name: ADN_NBC_Datas::NbcIntoxInfos::ReadEffect
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::NbcIntoxInfos::ReadEffect( xml::xistream& input )
{
    std::string wound = input.attribute< std::string >( "wound" );
    std::transform( wound.begin(), wound.end(), wound.begin(), std::tolower );
    ADN_Type_Double* pWound =
        wound == "healthy" ? &rNbAlivedHumans_ :
        wound == "u1"        ? &rNbHurtedHumans1_ :
        wound == "u2"        ? &rNbHurtedHumans2_ :
        wound == "u3"        ? &rNbHurtedHumans3_ :
        wound == "ue"        ? &rNbHurtedHumansE_ :
        wound == "dead"      ? &rNbDeadHumans_ :
        0;
    if( pWound )
    {
        *pWound = input.attribute< double >( "percentage" ) * 100.;
        if( pWound->GetData() < 0. || pWound->GetData() > 100. )
            throw ADN_DataException( tools::translate( "NBC_Data", "Invalid data" ).ascii(), tools::translate( "NBC_Data", "NBC - Wound '%1' data < 0 or > 1" ).arg( wound.c_str() ).ascii() );
    }
    else
        throw ADN_DataException( tools::translate( "NBC_Data", "Invalid data" ).ascii(),tools::translate( "NBC_Data", "NBC - Invalid wound type '%1'" ).arg( wound.c_str() ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::ReadArchive
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::NbcIntoxInfos::ReadArchive( xml::xistream& input )
{
    input >>  xml::attribute( "affliction", bIntoxPresent_ );
    if( bIntoxPresent_.GetData() )
    {
        input >> xml::list( "effect", *this, &ADN_NBC_Datas::NbcIntoxInfos::ReadEffect );
        if( rNbAlivedHumans_.GetData() + rNbHurtedHumans1_.GetData() + rNbHurtedHumans2_.GetData() + rNbHurtedHumans3_.GetData() + rNbHurtedHumansE_.GetData() + rNbDeadHumans_.GetData() != 100.0 )
            throw ADN_DataException( tools::translate( "NBC_Data", "Invalid data" ).ascii(), tools::translate( "NBC_Data","NBC - Agent '%1' - Poisoning effect data sum < 100" ).arg( GetParentNode()->GetNodeName().c_str() ).ascii() );
    }
    input >> xml::optional >> xml::attribute( "contamination", bContaminationPresent_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::WriteArchive
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::NbcIntoxInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "effects" )
           << xml::attribute( "type", "liquid" )
           << xml::attribute( "affliction", "false" );
    WriteContent( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::NbcIntoxInfos::WriteContent
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::NbcIntoxInfos::WriteContent( xml::xostream& output )
{
    if( bIntoxPresent_.GetData() )
    {
        if( rNbAlivedHumans_.GetData() + rNbHurtedHumans1_.GetData() + rNbHurtedHumans2_.GetData() + rNbHurtedHumans3_.GetData() + rNbHurtedHumansE_.GetData() + rNbDeadHumans_.GetData() != 100.0 )
            throw ADN_DataException( tools::translate( "NBC_Data","Invalid data" ).ascii(), tools::translate( "NBC_Data", "NBC - Agent '%1' - Poisoning effect data sum < 100" ).arg( GetParentNode()->GetNodeName().c_str() ).ascii() );
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
// Name: ADN_NBC_Datas::NbcGazInfos
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
ADN_NBC_Datas::NbcGazInfos::NbcGazInfos()
    : intoxInfos_  ( "gaz" )
    , lifeTime_    ( "1s" )
    , rSpreadAngle_( 20 )
{
    intoxInfos_.SetParentNode( *this );
    lifeTime_.SetDataName( "la durée de vie d'" );
    lifeTime_.SetParentNode( *this );
    rSpreadAngle_.SetDataName( "l'angle de propagation dû au vent d'" );
    rSpreadAngle_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::GetNodeName
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
std::string ADN_NBC_Datas::NbcGazInfos::GetNodeName()
{
    return GetParentNode()->GetNodeName();
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::GetItemName
// Created: SBO 2006-10-31
// -----------------------------------------------------------------------------
std::string ADN_NBC_Datas::NbcGazInfos::GetItemName()
{
    return "";
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::CopyFrom
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::NbcGazInfos::CopyFrom( NbcGazInfos& infos )
{
    intoxInfos_.CopyFrom( infos.intoxInfos_ );
    lifeTime_ = infos.lifeTime_.GetData();
    rSpreadAngle_ = infos.rSpreadAngle_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::ReadArchive
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::NbcGazInfos::ReadArchive( xml::xistream& input )
{
    intoxInfos_.ReadArchive( input );
    input >> xml::attribute( "life-time", lifeTime_ )
          >> xml::attribute( "propagation", rSpreadAngle_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::WriteArchive
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::NbcGazInfos::WriteArchive( xml::xostream& output )
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
ADN_NBC_Datas::NbcAgentInfos::NbcAgentInfos()
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
    , strName_()
    , nMosId_( ADN_Workspace::GetWorkspace().GetNbc().GetData().GetNextId() )
    , liquidInfos_( "liquide" )
    , category_( "chemical" )
    , bGazPresent_( false )
    , gazInfos_()
{
    strName_.SetDataName( "le nom d'" );
    strName_.SetParentNode( *this );
    liquidInfos_.SetParentNode( *this );
    bGazPresent_.SetParentNode( *this );
    gazInfos_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: NbcAgentInfos::GetNodeName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_NBC_Datas::NbcAgentInfos::GetNodeName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::GetItemName
// Created: SBO 2006-10-31
// -----------------------------------------------------------------------------
std::string ADN_NBC_Datas::NbcAgentInfos::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: NbcAgentInfos::CreateCopy
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
ADN_NBC_Datas::NbcAgentInfos* ADN_NBC_Datas::NbcAgentInfos::CreateCopy()
{
    NbcAgentInfos* pCopy = new NbcAgentInfos();
    pCopy->liquidInfos_.CopyFrom( liquidInfos_ );
    pCopy->bLiquidPresent_ = bLiquidPresent_.GetData();
    pCopy->gazInfos_.CopyFrom( gazInfos_ );
    pCopy->bGazPresent_ = bGazPresent_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::NbcAgentInfos::ReadEffect
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::NbcAgentInfos::ReadEffect( xml::xistream& input )
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
void ADN_NBC_Datas::NbcAgentInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", strName_ )
          >> xml::optional >> xml::attribute( "category", category_ )
          >> xml::list( "effects", *this, &ADN_NBC_Datas::NbcAgentInfos::ReadEffect );
    if ( category_ == "" )
        category_ = "chemical";
}

// -----------------------------------------------------------------------------
// Name: NbcAgentInfos::WriteArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::NbcAgentInfos::WriteArchive( xml::xostream& output )
{
    if( !bLiquidPresent_.GetData() && !bGazPresent_.GetData() )
        return;
    output << xml::start( "agent" )
           << xml::attribute( "name", strName_ )
           << xml::attribute( "category", category_ )
           << xml::attribute( "id", nMosId_ );
    if( bLiquidPresent_.GetData() )
        liquidInfos_.WriteArchive( output );
    if( bGazPresent_.GetData() )
        gazInfos_.WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas constructor
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
ADN_NBC_Datas::ADN_NBC_Datas()
    : ADN_Data_ABC                  ()
    , nNextId_                      ( 1 )
    , rContaminationDistance_       ( 0.f )
    , rContaminationQuantityGiven_  ( 0.f )
    , rWindSpeedLimitForSpreading_  ( 0.f )
    , rNbcSuitMaxSpeedMultiplier_   ( 0.f )
    , rNbcSuitReloadSpeedMultiplier_( 0.f )
{
    vNbcAgent_.SetItemTypeName( "un agent NBC" );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas destructor
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
ADN_NBC_Datas::~ADN_NBC_Datas()
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::FilesNeeded
// Created: APE 2004-12-06
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szNBC_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::Reset
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::Reset()
{
    nNextId_ = 1;
    vNbcAgent_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::ReadAgent
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::ReadAgent( xml::xistream& input )
{
    std::auto_ptr<NbcAgentInfos> spNew( new NbcAgentInfos() );
    spNew->ReadArchive( input );
    vNbcAgent_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::ReadArchive
// Created: AGN 2004-07-02
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::ReadArchive( xml::xistream& input )
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
            >> xml::list( "agent", *this, &ADN_NBC_Datas::ReadAgent )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::WriteArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_NBC_Datas::WriteArchive( xml::xostream& output )
{
    output << xml::start( "nbc" );
    ADN_Tools::AddSchema( output, "NBC" );
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
    for( T_NbcAgentInfos_Vector::iterator itAgent = vNbcAgent_.begin(); itAgent != vNbcAgent_.end(); ++itAgent )
        (*itAgent)->WriteArchive( output );
    output << xml::end;
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::GetNextId
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
int ADN_NBC_Datas::GetNextId()
{
    return nNextId_++;
}
