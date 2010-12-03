// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_LocalFireClass_Data.h"
#include "ADN_DataException.h"
#include "ADN_Project_Data.h"
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInjuryInfos::LocalFireClassInjuryInfos
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
ADN_LocalFireClass_Data::LocalFireClassInjuryInfos::LocalFireClassInjuryInfos( const std::string& nodeName )
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
    , nodeName_      ( nodeName )
    , nNbHurtHumans1_( 0 )
    , nNbHurtHumans2_( 0 )
    , nNbHurtHumans3_( 0 )
    , nNbHurtHumansE_( 0 )
    , nNbDeadHumans_ ( 0 )
{
    nNbHurtHumans1_.SetDataName( "le pourcentage d'humains bless�s de niveau U1 dans" );
    nNbHurtHumans1_.SetParentNode( *this );
    nNbHurtHumans2_.SetDataName( "le pourcentage d'humains bless�s de niveau U2 dans" );
    nNbHurtHumans2_.SetParentNode( *this );
    nNbHurtHumans3_.SetDataName( "le pourcentage d'humains bless�s de niveau U3 dans" );
    nNbHurtHumans3_.SetParentNode( *this );
    nNbHurtHumansE_.SetDataName( "le pourcentage d'humains bless�s de niveau UE dans" );
    nNbHurtHumansE_.SetParentNode( *this );
    nNbDeadHumans_.SetDataName( "le pourcentage d'humains d�c�d�s dans" );
    nNbDeadHumans_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInjuryInfos::GetNodeName
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
std::string ADN_LocalFireClass_Data::LocalFireClassInjuryInfos::GetNodeName()
{
    return nodeName_;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInjuryInfos::GetItemName
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
std::string ADN_LocalFireClass_Data::LocalFireClassInjuryInfos::GetItemName()
{
    return nodeName_;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::CopyFrom
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::LocalFireClassInjuryInfos::CopyFrom( LocalFireClassInjuryInfos& infos )
{
    nNbHurtHumans1_ = infos.nNbHurtHumans1_.GetData();
    nNbHurtHumans2_ = infos.nNbHurtHumans2_.GetData();
    nNbHurtHumans3_ = infos.nNbHurtHumans3_.GetData();
    nNbHurtHumansE_ = infos.nNbHurtHumansE_.GetData();
    nNbDeadHumans_ = infos.nNbDeadHumans_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::ReadEffect
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::LocalFireClassInjuryInfos::ReadInjury( xml::xistream& input )
{
    std::string wound = input.attribute< std::string >( "type" );
    std::transform( wound.begin(), wound.end(), wound.begin(), std::tolower );
    ADN_Type_Int* pWound =
        wound == "u1"      ? &nNbHurtHumans1_ :
        wound == "u2"      ? &nNbHurtHumans2_ :
        wound == "u3"      ? &nNbHurtHumans3_ :
        wound == "ue"      ? &nNbHurtHumansE_ :
        wound == "dead"    ? &nNbDeadHumans_ :
        0;
    if( pWound )
    {
        *pWound = static_cast< int >( input.attribute< double >( "percentage" ) * 100. );
        if( pWound->GetData() < 0 || pWound->GetData() > 100 )
            throw ADN_DataException( tools::translate( "ADN_LocalFireClass_Data", "Invalid data" ).ascii(), tools::translate( "ADN_LocalFireClass_Data", "Local fire - Wound '%1' data < 0 or > 1" ).arg( wound.c_str() ).ascii() );
    }
    else
        throw ADN_DataException( tools::translate( "ADN_LocalFireClass_Data", "Invalid data" ).ascii(),tools::translate( "ADN_LocalFireClass_Data", "Local fire - Invalid wound type '%1'" ).arg( wound.c_str() ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::ReadArchive
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::LocalFireClassInjuryInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::list( "injury", *this, &ADN_LocalFireClass_Data::LocalFireClassInjuryInfos::ReadInjury );
    if( nNbHurtHumans1_.GetData() + nNbHurtHumans2_.GetData() + nNbHurtHumans3_.GetData() + nNbHurtHumansE_.GetData() + nNbDeadHumans_.GetData() > 100 )
        throw ADN_DataException( tools::translate( "ADN_LocalFireClass_Data", "Invalid data" ).ascii(), tools::translate( "ADN_LocalFireClass_Data", "Local fire - Fire '%1' - Injuries data sum > 100" ).arg( GetParentNode()->GetNodeName().c_str() ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::WriteArchive
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::LocalFireClassInjuryInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "injuries" );
    if( nNbHurtHumans1_.GetData() + nNbHurtHumans2_.GetData() + nNbHurtHumans3_.GetData() + nNbHurtHumansE_.GetData() + nNbDeadHumans_.GetData() > 100 )
        throw ADN_DataException( tools::translate( "ADN_LocalFireClass_Data", "Invalid data" ).ascii(), tools::translate( "ADN_LocalFireClass_Data", "Local fire - Fire '%1' - Injuries data sum > 100" ).arg( GetParentNode()->GetNodeName().c_str() ).ascii() );
    output  << xml::start( "injury" )
                << xml::attribute( "type", "u1" )
                << xml::attribute( "percentage", nNbHurtHumans1_.GetData() / 100. )
            << xml::end
            << xml::start( "injury" )
                << xml::attribute( "type", "u2" )
                << xml::attribute( "percentage", nNbHurtHumans2_.GetData() / 100. )
            << xml::end
            << xml::start( "injury" )
                << xml::attribute( "type", "u3" )
                << xml::attribute( "percentage", nNbHurtHumans3_.GetData() / 100. )
            << xml::end
            << xml::start( "injury" )
                << xml::attribute( "type", "ue" )
                << xml::attribute( "percentage", nNbHurtHumansE_.GetData() / 100. )
            << xml::end
            << xml::start( "injury" )
                << xml::attribute( "type", "dead" )
                << xml::attribute( "percentage", nNbDeadHumans_.GetData() / 100. )
            << xml::end
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInfos::LocalFireClassInfos
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_LocalFireClass_Data::LocalFireClassInfos::LocalFireClassInfos()
    : agents_          ( ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_AgentExtincteur ).categories_ )
    , initialHeat_     ( 0 )
    , maxHeat_         ( 0 )
    , increaseRate_    ( 0 )
    , decreaseRate_    ( 0 )
    , injuryInfos_     ( "injuries" )
    , modifUrbanBlocks_( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos() )
{
    agents_.SetParentNode( *this );
    initialHeat_.SetParentNode( *this );
    maxHeat_.SetParentNode( *this );
    increaseRate_.SetParentNode( *this );
    decreaseRate_.SetParentNode( *this );
    injuryInfos_.SetParentNode( *this );
    weatherEffects_.SetParentNode( *this );
    for( int i= 0 ; i< eNbrSensorWeatherModifiers ; ++i)
    {
        ADN_WeatherFireEffects* pEffect = new ADN_WeatherFireEffects((E_SensorWeatherModifiers)i);
        weatherEffects_.AddItem( pEffect );
    }

}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInfos::~LocalFireClassInfos
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_LocalFireClass_Data::LocalFireClassInfos::~LocalFireClassInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInfos::GetNodeName
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
std::string ADN_LocalFireClass_Data::LocalFireClassInfos::GetNodeName()
{
    std::string strResult( "de la classe d'incendie " );
    return strResult + strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInfos::GetItemName
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
std::string ADN_LocalFireClass_Data::LocalFireClassInfos::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInfos::CreateCopy
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_LocalFireClass_Data::LocalFireClassInfos* ADN_LocalFireClass_Data::LocalFireClassInfos::CreateCopy()
{
    LocalFireClassInfos* pCopy = new LocalFireClassInfos();
    pCopy->injuryInfos_.CopyFrom( injuryInfos_ );
    pCopy->strName_ = strName_.GetData();
    pCopy->initialHeat_ = initialHeat_.GetData();
    pCopy->maxHeat_ = maxHeat_.GetData();
    pCopy->increaseRate_ = increaseRate_.GetData();
    pCopy->decreaseRate_ = decreaseRate_.GetData();
    for( uint n = 0; n < agents_.size(); ++n )
        pCopy->agents_[ n ]->CopyFrom( *agents_[ n ] );
    
    for( uint i=0; i<weatherEffects_.size(); ++i )
        pCopy->weatherEffects_[i]->CopyFrom( *weatherEffects_[i] );

    return pCopy;
}
    
// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInfos::ReadArchive
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::LocalFireClassInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "initial-heat", initialHeat_ )
          >> xml::attribute( "max-heat", maxHeat_ )
          >> xml::attribute( "increase-rate", increaseRate_ )
          >> xml::attribute( "decrease-rate", decreaseRate_ )
          >> xml::start( "extinguisher-agents" )
              >> xml::list( "extinguisher-agent", *this, &ADN_LocalFireClass_Data::LocalFireClassInfos::ReadAgent )
          >> xml::end
          >> xml::start( "weather-effects" )
              >> xml::list( "weather-effect", *this, &ADN_LocalFireClass_Data::LocalFireClassInfos::ReadWeatherEffect )
          >> xml::end
          >> xml::start( "injuries" );
    injuryInfos_.ReadArchive( input );
    input >> xml::end
          >> xml::start( "urban-modifiers" )
            >> xml::list( "urban-modifier", *this, &ADN_LocalFireClass_Data::LocalFireClassInfos::ReadUrbanModifer )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInfos::ReadAgent
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::LocalFireClassInfos::ReadAgent( xml::xistream& input )
{
    std::string agent = input.attribute< std::string >( "agent" );
    IT_ExtinguisherAgentInfos_Vector itAgent = std::find_if( agents_.begin(), agents_.end(), ADN_ExtinguisherAgentInfos::Cmp( agent ));
    if( itAgent == agents_.end() )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Fire class - Invalid extinguisher agent '%1'" ).arg( agent.c_str() ).ascii() );
    ( *itAgent )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInfos::ReadWeatherEffect
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::LocalFireClassInfos::ReadWeatherEffect( xml::xistream& input )
{
    std::string weatherStr = input.attribute< std::string >( "weather" );
    E_SensorWeatherModifiers weather = ADN_Tr::ConvertToSensorWeatherModifiers( weatherStr );
    weatherEffects_.at( weather )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInfos::ReadUrbanModifer
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::LocalFireClassInfos::ReadUrbanModifer( xml::xistream& input )
{
    std::string material = input.attribute< std::string >( "material-type" );
    helpers::IT_UrbanAttritionInfos_Vector it = std::find_if( modifUrbanBlocks_.begin(), modifUrbanBlocks_.end(), helpers::ADN_UrbanAttritionInfos::Cmp( material ) );
    if( it == modifUrbanBlocks_.end() )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Fire class - Invalid urban Material type '%1'" ).arg( material.c_str() ).ascii() );
    (*it)->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInfos::WriteArchive
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::LocalFireClassInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "local-fire-class" )
               << xml::attribute( "name", strName_ )
               << xml::attribute( "initial-heat", initialHeat_ )
               << xml::attribute( "max-heat", maxHeat_ )
               << xml::attribute( "increase-rate", increaseRate_ )
               << xml::attribute( "decrease-rate", decreaseRate_ )
               << xml::start( "extinguisher-agents" );
    for( IT_ExtinguisherAgentInfos_Vector itAgent = agents_.begin(); itAgent != agents_.end(); ++itAgent )
        ( *itAgent )->WriteArchive( output );
    output     << xml::end
               << xml::start( "weather-effects" );
    for( IT_WeatherFireEffects_Vector itWeather = weatherEffects_.begin(); itWeather != weatherEffects_.end(); ++itWeather )
        ( *itWeather )->WriteArchive( output );
    output     << xml::end;
    injuryInfos_.WriteArchive( output );
    output     << xml::start( "urban-modifiers" );
    for( helpers::IT_UrbanAttritionInfos_Vector itUrbanAttrition = modifUrbanBlocks_.begin(); itUrbanAttrition != modifUrbanBlocks_.end(); ++itUrbanAttrition )
        ( *itUrbanAttrition )->WriteArchive( output );
    output     << xml::end;
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data constructor
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_LocalFireClass_Data::ADN_LocalFireClass_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data destructor
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_LocalFireClass_Data::~ADN_LocalFireClass_Data()
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::FilesNeeded
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::FilesNeeded( T_StringList& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szLocalFireClasses_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::Reset
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::Reset()
{
    localFireClasses_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::ReadArchive
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "local-fire-classes" )
            >> xml::list( "local-fire-class", *this, &ADN_LocalFireClass_Data::ReadLocalFireClass )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::ReadLocalFireClass
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::ReadLocalFireClass( xml::xistream& input )
{
    std::auto_ptr< LocalFireClassInfos > spNew( new LocalFireClassInfos() );
    spNew->ReadArchive( input );
    localFireClasses_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::WriteArchive
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "local-fire-classes" );
    ADN_Tools::AddSchema( output, "LocalFireClasses" );
    for( IT_LocalFireClassInfosVector it = localFireClasses_.begin(); it != localFireClasses_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::GetLocalFireClassesInfos
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
ADN_LocalFireClass_Data::T_LocalFireClassInfosVector& ADN_LocalFireClass_Data::GetLocalFireClassesInfos()
{
    return localFireClasses_;
}
