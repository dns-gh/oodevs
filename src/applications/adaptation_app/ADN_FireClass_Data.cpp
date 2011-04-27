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
#include "ADN_DataException.h"
#include "ADN_Project_Data.h"
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireInjuryInfos::FireInjuryInfos
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
ADN_FireClass_Data::FireInjuryInfos::FireInjuryInfos( const std::string& nodeName )
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
    , nodeName_      ( nodeName )
    , nNbHurtHumans1_( 0 )
    , nNbHurtHumans2_( 0 )
    , nNbHurtHumans3_( 0 )
    , nNbHurtHumansE_( 0 )
    , nNbDeadHumans_ ( 0 )
{
    nNbHurtHumans1_.SetDataName( "le pourcentage d'humains blessés de niveau U1 dans" );
    nNbHurtHumans1_.SetParentNode( *this );
    nNbHurtHumans2_.SetDataName( "le pourcentage d'humains blessés de niveau U2 dans" );
    nNbHurtHumans2_.SetParentNode( *this );
    nNbHurtHumans3_.SetDataName( "le pourcentage d'humains blessés de niveau U3 dans" );
    nNbHurtHumans3_.SetParentNode( *this );
    nNbHurtHumansE_.SetDataName( "le pourcentage d'humains blessés de niveau UE dans" );
    nNbHurtHumansE_.SetParentNode( *this );
    nNbDeadHumans_.SetDataName( "le pourcentage d'humains décédés dans" );
    nNbDeadHumans_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireInjuryInfos::GetNodeName
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
std::string ADN_FireClass_Data::FireInjuryInfos::GetNodeName()
{
    return nodeName_;
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireInjuryInfos::GetItemName
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
std::string ADN_FireClass_Data::FireInjuryInfos::GetItemName()
{
    return nodeName_;
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
            throw ADN_DataException( tools::translate( "ADN_FireClass_Data", "Invalid data" ).ascii(), tools::translate( "ADN_FireClass_Data", "Fire - Wound '%1' data < 0 or > 1" ).arg( wound.c_str() ).ascii() );
    }
    else
        throw ADN_DataException( tools::translate( "ADN_FireClass_Data", "Invalid data" ).ascii(),tools::translate( "ADN_FireClass_Data", "Fire - Invalid wound type '%1'" ).arg( wound.c_str() ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireInjuryInfos::ReadArchive
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::FireInjuryInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::list( "injury", *this, &ADN_FireClass_Data::FireInjuryInfos::ReadInjury );
    if( nNbHurtHumans1_.GetData() + nNbHurtHumans2_.GetData() + nNbHurtHumans3_.GetData() + nNbHurtHumansE_.GetData() + nNbDeadHumans_.GetData() > 100 )
        throw ADN_DataException( tools::translate( "ADN_FireClass_Data", "Invalid data" ).ascii(), tools::translate( "ADN_FireClass_Data", "Fire '%1' - Injuries data sum > 100" ).arg( GetParentNode()->GetNodeName().c_str() ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireInjuryInfos::WriteArchive
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::FireInjuryInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "injuries" );
    if( nNbHurtHumans1_.GetData() + nNbHurtHumans2_.GetData() + nNbHurtHumans3_.GetData() + nNbHurtHumansE_.GetData() + nNbDeadHumans_.GetData() > 100 )
        throw ADN_DataException( tools::translate( "ADN_FireClass_Data", "Invalid data" ).ascii(), tools::translate( "ADN_FireClass_Data", "Fire '%1' - Injuries data sum > 100" ).arg( GetParentNode()->GetNodeName().c_str() ).ascii() );
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
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
    , strName_            ( ADN_Tr::ConvertFromLocation( location ) )
    , ignitionThreshold_  ( 0 )
    , maxCombustionEnergy_( 0 )
{
    ignitionThreshold_.SetDataName( "le seuil d'ignition dans" );
    ignitionThreshold_.SetParentNode( *this );
    maxCombustionEnergy_.SetDataName( "l'énergie de combustion maximale dans" );
    maxCombustionEnergy_.SetParentNode( *this );
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
    : agents_          ( ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_AgentExtincteur ).categories_ )
    , initialHeat_     ( 0 )
    , maxHeat_         ( 0 )
    , increaseRate_    ( 0 )
    , decreaseRate_    ( 0 )
    , injuryInfos_     ( "injuries" )
    , modifUrbanBlocks_( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos() )
    , isSurface_       ( false )
    , surfaceInfos_    ( "surfaces" )
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

    isSurface_.SetParentNode( *this );
    surfaceInfos_.SetParentNode( *this );

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
// Name: ADN_FireClass_Data::FireClassInfos::GetNodeName
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
std::string ADN_FireClass_Data::FireClassInfos::GetNodeName()
{
    std::string strResult( "de la classe d'incendie " );
    return strResult + strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireClassInfos::GetItemName
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
std::string ADN_FireClass_Data::FireClassInfos::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireClassInfos::CreateCopy
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_FireClass_Data::FireClassInfos* ADN_FireClass_Data::FireClassInfos::CreateCopy()
{
    FireClassInfos* pCopy = new FireClassInfos();
    pCopy->injuryInfos_.CopyFrom( injuryInfos_ );
    pCopy->modifUrbanBlocks_ = modifUrbanBlocks_;
    pCopy->strName_ = strName_.GetData();
    pCopy->initialHeat_ = initialHeat_.GetData();
    pCopy->maxHeat_ = maxHeat_.GetData();
    pCopy->increaseRate_ = increaseRate_.GetData();
    pCopy->decreaseRate_ = decreaseRate_.GetData();
    pCopy->isSurface_ = isSurface_.GetData();
    for( uint n = 0; n < agents_.size(); ++n )
        pCopy->agents_[ n ]->CopyFrom( *agents_[ n ] );

    for( uint i=0; i<weatherEffects_.size(); ++i )
        pCopy->weatherEffects_[i]->CopyFrom( *weatherEffects_[i] );

    for( uint n = 0; n < surfaceInfos_.size(); ++n )
        pCopy->surfaceInfos_[ n ]->CopyFrom( *surfaceInfos_[ n ] );
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireClassInfos::ReadArchive
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::FireClassInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "initial-heat", initialHeat_ )
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
    IT_ExtinguisherAgentInfos_Vector itAgent = std::find_if( agents_.begin(), agents_.end(), ADN_ExtinguisherAgentInfos::Cmp( agent ));
    if( itAgent == agents_.end() )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Fire class - Invalid extinguisher agent '%1'" ).arg( agent.c_str() ).ascii() );
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
    helpers::IT_UrbanAttritionInfos_Vector it = std::find_if( modifUrbanBlocks_.begin(), modifUrbanBlocks_.end(), helpers::ADN_UrbanAttritionInfos::Cmp( material ) );
    if( it == modifUrbanBlocks_.end() )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Fire class - Invalid urban Material type '%1'" ).arg( material.c_str() ).ascii() );
    ( *it )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::FireClassInfos::ReadSurface
// Created: JSR 2010-12-03
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::FireClassInfos::ReadSurface( xml::xistream& input )
{
    std::string type = input.attribute< std::string >( "type" );
    /*E_Location location = ADN_Tr::ConvertToLocation( type );
    if( location == static_cast< E_Location >( -1 ) )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Fire - Invalid location type '%1'" ).arg( type.c_str() ).ascii() );*/
    IT_FireSurfaceInfos_Vector it = std::find_if( surfaceInfos_.begin(), surfaceInfos_.end(), FireSurfaceInfos::Cmp( type ) );
    if( it == surfaceInfos_.end() )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Fire - Location type not found '%1'" ).arg( type.c_str() ).ascii() );
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
    if( isSurface_.GetData() )
    {
        output << xml::start( "surfaces" );
        for( IT_FireSurfaceInfos_Vector itSurface = surfaceInfos_.begin(); itSurface != surfaceInfos_.end(); ++itSurface )
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
    : cellSize_( 10 )
{
    // NOTHING
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
void ADN_FireClass_Data::FilesNeeded( T_StringList& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szFireClasses_.GetData() );
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
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::ReadFireClass
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::ReadFireClass( xml::xistream& input )
{
    std::auto_ptr< FireClassInfos > spNew( new FireClassInfos() );
    spNew->ReadArchive( input );
    fireClasses_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Data::WriteArchive
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_FireClass_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "fires" );
    ADN_Tools::AddSchema( output, "Fires" );
    output << xml::start( "cell-size" )
                << xml::attribute( "value", cellSize_ )
            << xml::end;
    for( IT_FireClassInfosVector it = fireClasses_.begin(); it != fireClasses_.end(); ++it )
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
