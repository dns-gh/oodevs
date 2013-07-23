// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_FireClass_Data.h"
#include "ADN_Project_Data.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"
#include "clients_kernel/XmlTranslations.h"

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireInjuryInfos::FireInjuryInfos
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
ADN_FireClass_Data::FireInjuryInfos::FireInjuryInfos()
    : parentName_    ()
    , nNbHurtHumans1_( 0 )
    , nNbHurtHumans2_( 0 )
    , nNbHurtHumans3_( 0 )
    , nNbHurtHumansE_( 0 )
    , nNbDeadHumans_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireInjuryInfos::CopyFrom
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::FireInjuryInfos::CopyFrom( FireInjuryInfos& infos )
{
    nNbHurtHumans1_ = infos.nNbHurtHumans1_.GetData();
    nNbHurtHumans2_ = infos.nNbHurtHumans2_.GetData();
    nNbHurtHumans3_ = infos.nNbHurtHumans3_.GetData();
    nNbHurtHumansE_ = infos.nNbHurtHumansE_.GetData();
    nNbDeadHumans_ = infos.nNbDeadHumans_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireInjuryInfos::FireInjuryInfos::ReadEffect
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::FireInjuryInfos::ReadInjury( xml::xistream& input )
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
            throw MASA_EXCEPTION( tools::translate( "ADN_FireClass_Data", "Fire - Wound '%1' data < 0 or > 1" ).arg( wound.c_str() ).toStdString() );
    }
    else
        throw MASA_EXCEPTION(tools::translate( "ADN_FireClass_Data", "Fire - Invalid wound type '%1'" ).arg( wound.c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireInjuryInfos::ReadArchive
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::FireInjuryInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::list( "injury", *this, &ADN_FireClass_Data::FireInjuryInfos::ReadInjury );
    if( nNbHurtHumans1_.GetData() + nNbHurtHumans2_.GetData() + nNbHurtHumans3_.GetData() + nNbHurtHumansE_.GetData() + nNbDeadHumans_.GetData() > 100 )
        throw MASA_EXCEPTION( tools::translate( "ADN_FireClass_Data", "Fire '%1' - Injuries data sum > 100" ).arg( parentName_.c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireInjuryInfos::WriteArchive
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::FireInjuryInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "injuries" );
    if( nNbHurtHumans1_.GetData() + nNbHurtHumans2_.GetData() + nNbHurtHumans3_.GetData() + nNbHurtHumansE_.GetData() + nNbDeadHumans_.GetData() > 100 )
        throw MASA_EXCEPTION( tools::translate( "ADN_FireClass_Data", "Fire '%1' - Injuries data sum > 100" ).arg( parentName_.c_str() ).toStdString() );
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
// Name: ADN_FireClass_Data::FireSurfaceInfos::FireSurfaceInfos
// Created: JSR 2010-12-03
// -----------------------------------------------------------------------------
ADN_FireClass_Data::FireSurfaceInfos::FireSurfaceInfos( E_Location location )
    : ADN_RefWithName( ENT_Tr::ConvertFromLocation( location ) )
    , ignitionThreshold_  ( 0 )
    , maxCombustionEnergy_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireSurfaceInfos::GetItemName
// Created: JSR 2010-12-03
// -----------------------------------------------------------------------------
std::string ADN_FireClass_Data::FireSurfaceInfos::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireSurfaceInfos::CopyFrom
// Created: JSR 2010-12-03
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::FireSurfaceInfos::CopyFrom( FireSurfaceInfos& infos )
{
    strName_ = infos.strName_.GetData();
    ignitionThreshold_ = infos.ignitionThreshold_.GetData();
    maxCombustionEnergy_ = infos.maxCombustionEnergy_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireSurfaceInfos::ReadArchive
// Created: JSR 2010-12-03
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::FireSurfaceInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "ignition-threshold", ignitionThreshold_ )
          >> xml::attribute( "max-combustion-energy", maxCombustionEnergy_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireSurfaceInfos::WriteArchive
// Created: JSR 2010-12-03
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::FireSurfaceInfos::WriteArchive( xml::xostream& output )
{
    output  << xml::start( "surface" )
        << xml::attribute( "type", strName_ )
                << xml::attribute( "ignition-threshold", ignitionThreshold_ )
                << xml::attribute( "max-combustion-energy", maxCombustionEnergy_ )
            << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireClassInfos::FireClassInfos
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_FireClass_Data::FireClassInfos::FireClassInfos()
    : agents_          ( ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( eDotationFamily_AgentExtincteur ).categories_ )
    , initialHeat_     ( 0 )
    , maxHeat_         ( 0 )
    , increaseRate_    ( 0 )
    , decreaseRate_    ( 0 )
    , injuryInfos_     ()
    , modifUrbanBlocks_( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos() )
    , isSurface_       ( false )
    , surfaceInfos_    ( "surfaces" )
{
    for( int i= 0 ; i< eNbrSensorWeatherModifiers ; ++i)
    {
        ADN_WeatherFireEffects* pEffect = new ADN_WeatherFireEffects((E_SensorWeatherModifiers)i);
        weatherEffects_.AddItem( pEffect );
    }

     for( int i = 0; i < eNbrLocation; ++i )
         surfaceInfos_.AddItem( new FireSurfaceInfos( E_Location( i ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireClassInfos::~FireClassInfos
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_FireClass_Data::FireClassInfos::~FireClassInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireClassInfos::CreateCopy
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_FireClass_Data::FireClassInfos* ADN_FireClass_Data::FireClassInfos::CreateCopy()
{
    FireClassInfos* pCopy = new FireClassInfos();
    pCopy->injuryInfos_.CopyFrom( injuryInfos_ );
    for( size_t i = 0; i < modifUrbanBlocks_.size(); ++i )
        pCopy->modifUrbanBlocks_[i]->CopyFrom( *modifUrbanBlocks_[i] );
    pCopy->strName_ = strName_.GetData();
    pCopy->initialHeat_ = initialHeat_.GetData();
    pCopy->maxHeat_ = maxHeat_.GetData();
    pCopy->increaseRate_ = increaseRate_.GetData();
    pCopy->decreaseRate_ = decreaseRate_.GetData();
    pCopy->isSurface_ = isSurface_.GetData();
    for( size_t i = 0; i < agents_.size(); ++i )
        pCopy->agents_[i]->CopyFrom( *agents_[i] );

    for( size_t i = 0; i < weatherEffects_.size(); ++i )
        pCopy->weatherEffects_[i]->CopyFrom( *weatherEffects_[i] );

    for( size_t i = 0; i < surfaceInfos_.size(); ++i )
        pCopy->surfaceInfos_[i]->CopyFrom( *surfaceInfos_[i] );
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireClassInfos::ReadArchive
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::FireClassInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "initial-heat", initialHeat_ )
          >> xml::attribute( "max-heat", maxHeat_ )
          >> xml::attribute( "increase-rate", increaseRate_ )
          >> xml::attribute( "decrease-rate", decreaseRate_ )
          >> xml::start( "extinguisher-agents" )
              >> xml::list( "extinguisher-agent", *this, &ADN_FireClass_Data::FireClassInfos::ReadAgent )
          >> xml::end
          >> xml::start( "weather-effects" )
              >> xml::list( "weather-effect", *this, &ADN_FireClass_Data::FireClassInfos::ReadWeatherEffect )
          >> xml::end
          >> xml::start( "injuries" );
    injuryInfos_.parentName_ = strName_.GetData();
    injuryInfos_.ReadArchive( input );
    input >> xml::end
          >> xml::start( "urban-modifiers" )
            >> xml::list( "urban-modifier", *this, &ADN_FireClass_Data::FireClassInfos::ReadUrbanModifer )
          >> xml::end;
    isSurface_ = input.has_child( "surfaces" );
    if( isSurface_.GetData() )
    {
        input >> xml::start( "surfaces" )
                  >> xml::list( "surface", *this, &ADN_FireClass_Data::FireClassInfos::ReadSurface )
              >> xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireClassInfos::ReadAgent
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::FireClassInfos::ReadAgent( xml::xistream& input )
{
    std::string agent = input.attribute< std::string >( "agent" );
    auto itAgent = std::find_if( agents_.begin(), agents_.end(), ADN_ExtinguisherAgentInfos::Cmp( agent ) );
    if( itAgent == agents_.end() )
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, strName_.GetData(), eFireClasses, -1, tools::translate( "ADN_FireClass_Data", "Extinguisher agents" ).toStdString() );
    else
        ( *itAgent )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireClassInfos::ReadWeatherEffect
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::FireClassInfos::ReadWeatherEffect( xml::xistream& input )
{
    std::string weatherStr = input.attribute< std::string >( "weather" );
    E_SensorWeatherModifiers weather = ADN_Tr::ConvertToSensorWeatherModifiers( weatherStr );
    weatherEffects_.at( weather )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireClassInfos::ReadUrbanModifer
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::FireClassInfos::ReadUrbanModifer( xml::xistream& input )
{
    std::string material = input.attribute< std::string >( "material-type" );
    auto it = std::find_if( modifUrbanBlocks_.begin(), modifUrbanBlocks_.end(), helpers::ADN_UrbanAttritionInfos::Cmp( material ) );
    if( it == modifUrbanBlocks_.end() )
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, strName_.GetData(), eFireClasses, -1, tools::translate( "ADN_FireClass_Data", "Urban attritions" ).toStdString() );
    else
        ( *it )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireClassInfos::ReadSurface
// Created: JSR 2010-12-03
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::FireClassInfos::ReadSurface( xml::xistream& input )
{
    std::string type = input.attribute< std::string >( "type" );
    /*E_Location location = ENT_Tr::ConvertToLocation( type );
    if( location == static_cast< E_Location >( -1 ) )
        throw MASA_EXCEPTION( tr( "Fire - Invalid location type '%1'" ).arg( type.c_str() ).toStdString() );*/
    auto it = std::find_if( surfaceInfos_.begin(), surfaceInfos_.end(), FireSurfaceInfos::Cmp( type ) );
    if( it == surfaceInfos_.end() )
        throw MASA_EXCEPTION( tr( "Fire - Location type not found '%1'" ).arg( type.c_str() ).toStdString() );
    ( *it )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireClassInfos::WriteArchive
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::FireClassInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "fire" )
               << xml::attribute( "name", strName_ )
               << xml::attribute( "initial-heat", initialHeat_ )
               << xml::attribute( "max-heat", maxHeat_ )
               << xml::attribute( "increase-rate", increaseRate_ )
               << xml::attribute( "decrease-rate", decreaseRate_ )
               << xml::start( "extinguisher-agents" );
    for( auto itAgent = agents_.begin(); itAgent != agents_.end(); ++itAgent )
        ( *itAgent )->WriteArchive( output );
    output     << xml::end
               << xml::start( "weather-effects" );
    for( auto itWeather = weatherEffects_.begin(); itWeather != weatherEffects_.end(); ++itWeather )
        ( *itWeather )->WriteArchive( output );
    output     << xml::end;
    injuryInfos_.WriteArchive( output );
    output     << xml::start( "urban-modifiers" );
    for( auto itUrbanAttrition = modifUrbanBlocks_.begin(); itUrbanAttrition != modifUrbanBlocks_.end(); ++itUrbanAttrition )
        ( *itUrbanAttrition )->WriteArchive( output );
    output     << xml::end;
    if( isSurface_.GetData() )
    {
        output << xml::start( "surfaces" );
        for( auto itSurface = surfaceInfos_.begin(); itSurface != surfaceInfos_.end(); ++itSurface )
            ( *itSurface )->WriteArchive( output );
        output << xml::end;
    }
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data constructor
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_FireClass_Data::ADN_FireClass_Data()
    : ADN_Data_ABC( eFireClasses )
    , cellSize_( 10 )
{
    fireClasses_.AddUniquenessChecker( eError, duplicateName_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data destructor
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_FireClass_Data::~ADN_FireClass_Data()
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FilesNeeded
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::FilesNeeded( tools::Path::T_Paths& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szFireClasses_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::Reset
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::Reset()
{
    fireClasses_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::ReadArchive
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "fires" )
            >> xml::start( "cell-size" )
                >> xml::attribute( "value", cellSize_ )
            >> xml::end
            >> xml::list( "fire", *this, &ADN_FireClass_Data::ReadFireClass )
          >> xml::end;
    fireClasses_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::ReadFireClass
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::ReadFireClass( xml::xistream& input )
{
    std::string strName = input.attribute< std::string >( "name" );
    std::auto_ptr< FireClassInfos > spNew( new FireClassInfos() );
    spNew->ReadArchive( input );
    spNew->strName_.SetTranslation( strName, translations_->GetTranslation( "fires", strName ) );
    fireClasses_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::WriteArchive
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::WriteArchive( xml::xostream& output )
{
    if( fireClasses_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "fires" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Fires" );
    output << xml::start( "cell-size" )
                << xml::attribute( "value", cellSize_ )
            << xml::end;
    for( auto it = fireClasses_.begin(); it != fireClasses_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::GetCellSize
// Created: JSR 2010-12-03
// -----------------------------------------------------------------------------
ADN_Type_Int& ADN_FireClass_Data::GetCellSize()
{
    return cellSize_;
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::GetFireClassesInfos
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
ADN_FireClass_Data::T_FireClassInfosVector& ADN_FireClass_Data::GetFireClassesInfos()
{
    return fireClasses_;
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::GetFireThatUse
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
QStringList ADN_FireClass_Data::GetFireThatUse( ADN_Resources_Data::CategoryInfo& infos )
{
    QStringList result;
    for( auto it = fireClasses_.begin(); it != fireClasses_.end(); ++it )
        for( auto itAgent = ( *it )->agents_.begin(); itAgent != ( *it )->agents_.end(); ++itAgent )
        {
            ADN_Resources_Data::CategoryInfo* catInfos = ( *itAgent )->GetCrossedElement();
            if( catInfos && catInfos->strName_.GetData() == infos.strName_.GetData() )
                result << ( *it )->strName_.GetData().c_str();
        }
    return result;
}
