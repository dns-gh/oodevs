// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Missions_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_Xml_Exception.h"
#include "ADN_Tools.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_AiEngine_Data.h"
#include "ADN_Tr.h"

// =============================================================================
// Mission Parameters
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameterValue::MissionParameterValue
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::MissionParameterValue::MissionParameterValue()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameterValue::~MissionParameterValue
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::MissionParameterValue::~MissionParameterValue()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameterValue::GetItemName
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
std::string ADN_Missions_Data::MissionParameterValue::GetItemName()
{
    return name_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameterValue::CreateCopy
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::MissionParameterValue* ADN_Missions_Data::MissionParameterValue::CreateCopy()
{
    MissionParameterValue* newValue = new MissionParameterValue();
    newValue->name_ = name_.GetData();
    return newValue;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameterValue::ReadArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Data::MissionParameterValue::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.ReadAttribute( "name", name_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameterValue::WriteArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Data::MissionParameterValue::WriteArchive( MT_OutputArchive_ABC& output, unsigned int id )
{
    output.WriteAttribute( "id"  , id );
    output.WriteAttribute( "name", name_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameter::MissionParameter
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::MissionParameter::MissionParameter()
    : isOptional_( false )
{
    ADN_Type_Enum< E_MissionParameterType, eNbrMissionParameterType >::SetConverter( &ADN_Tr::ConvertFromMissionParameterType );
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameter::~MissionParameter
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::MissionParameter::~MissionParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameter::GetItemName
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
std::string ADN_Missions_Data::MissionParameter::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameter::CreateCopy
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::MissionParameter* ADN_Missions_Data::MissionParameter::CreateCopy()
{
    MissionParameter* newParam = new MissionParameter();
    newParam->strName_ = strName_.GetData();
    newParam->type_    = type_.GetData();
    newParam->isOptional_ = isOptional_.GetData();
    newParam->values_.reserve( values_.size() );
    for( IT_MissionParameterValue_Vector it = values_.begin(); it != values_.end(); ++it )
    {
        MissionParameterValue* newParamValue = (*it)->CreateCopy();
        newParam->values_.AddItem( newParamValue );
    }
    return newParam;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameter::ReadArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Data::MissionParameter::ReadArchive( ADN_XmlInput_Helper& input )
{
    std::string type;
    input.ReadAttribute( "name", strName_ );
    input.ReadAttribute( "type", type );
    input.ReadAttribute( "optional", isOptional_, ADN_XmlInput_Helper::eNothing );
    type_ = ADN_Tr::ConvertToMissionParameterType( type );
    while( input.NextListElement() )
    {
        input.Section( "value" );
        std::auto_ptr< MissionParameterValue > spNew( new MissionParameterValue() );
        spNew->ReadArchive( input );
        values_.AddItem( spNew.release() );
        input.EndSection();
    }
}

namespace
{
    QString GetFussedDiaName( const QString& name )
    {
        QStringList list = QStringList::split( ' ', name );
        for( unsigned int i = 0; i < list.size(); ++i )
            list[i][0] = i == 0 ? list[i][0].lower() : list[i][0].upper();
        return list.join( "" ).append( '_' );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameter::WriteArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Data::MissionParameter::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.WriteAttribute( "name", strName_.GetData() );
    output.WriteAttribute( "type", ADN_Tr::ConvertFromMissionParameterType( type_.GetData() ) );
    output.WriteAttribute( "optional", isOptional_.GetData() );
    output.WriteAttribute( "dia-name", GetFussedDiaName( strName_.GetData().c_str() ).ascii() );
    
    for( unsigned int i = 0; i < values_.size(); ++i )
    {
        output.Section( "value" );
        values_[i]->WriteArchive( output, i );
        output.EndSection();
    }
}

// =============================================================================
// Missions
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Mission::Mission
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::Mission::Mission( bool isAutomat /*= false*/ )
    : isAutomat_( isAutomat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Mission::~Mission
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::Mission::~Mission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Mission::GetItemName
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
std::string ADN_Missions_Data::Mission::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Mission::CreateCopy
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::Mission* ADN_Missions_Data::Mission::CreateCopy()
{
    Mission* newMission = new Mission( isAutomat_.GetData() );
    newMission->strName_ = strName_.GetData();
    newMission->parameters_.reserve( parameters_.size() );
    for( IT_MissionParameter_Vector it = parameters_.begin(); it != parameters_.end(); ++it )
    {
        MissionParameter* newParam = (*it)->CreateCopy();
        newMission->parameters_.AddItem( newParam );
    }
    return newMission;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Mission::ReadArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Data::Mission::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.ReadAttribute( "name", strName_ );
    while( input.NextListElement() )
    {
        input.BeginList( "parameter" );
        std::auto_ptr< MissionParameter > spNew( new MissionParameter() );
        spNew->ReadArchive( input );
        parameters_.AddItem( spNew.release() );
        input.EndList();
    }
}

namespace
{
    QString BuildDiaMissionType( const QString& name )
    {
        QStringList list = QStringList::split( ' ', name );
        for( unsigned int i = 0; i < list.size() - 1; ++i )
            if( list[i].length() > 1 && ( list[i] == list[i].upper() || list[i].lower() == "test" ) )
                list[i].append( "_" );
        return list.join( "" );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Mission::WriteArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Data::Mission::WriteArchive( MT_OutputArchive_ABC& output, const std::string& type )
{
    output.WriteAttribute( "name", strName_.GetData() );
    QString typeName = type == "units" ? "Pion" : (type == "automats" ? "Automate" : "Population");
    QString diaName  = BuildDiaMissionType( strName_.GetData().c_str() );
    output.WriteAttribute( "dia-type", QString( "T_Mission_%1_%2" ).arg( typeName ).arg( diaName ).ascii() );
    if( !isAutomat_.GetData() )
        output.WriteAttribute( "dia-behavior", QString( "MIS_%1_%2" ).arg( typeName ).arg( diaName ).ascii() );
    else
    {
        output.WriteAttribute( "mrt-dia-behavior", QString( "MIS_%1_MRT_%2" ).arg( typeName ).arg( diaName ).ascii() );
        output.WriteAttribute( "cdt-dia-behavior", QString( "MIS_%1_CDT_%2" ).arg( typeName ).arg( diaName ).ascii() );
    }

    for( unsigned int i = 0; i < parameters_.size(); ++i )
        if( parameters_[i]->values_.empty() )
        {
            output.Section( "parameter" );
            parameters_[i]->WriteArchive( output );
            output.EndSection();
        }
        else
        {
            output.BeginList( "parameter", parameters_[i]->values_.size() );
            parameters_[i]->WriteArchive( output );
            output.EndList();
        }
}

// =============================================================================
// Frag orders
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FragOrder::FragOrder
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::FragOrder::FragOrder()
    : isAvailableForAllMissions_( false )
    , isAvailableWithoutMission_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FragOrder::~FragOrder
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::FragOrder::~FragOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FragOrder::GetItemName
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
std::string ADN_Missions_Data::FragOrder::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FragOrder::CreateCopy
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::FragOrder* ADN_Missions_Data::FragOrder::CreateCopy()
{
    FragOrder* newFragOrder = new FragOrder();
    newFragOrder->strName_ = strName_.GetData();
    newFragOrder->parameters_.reserve( parameters_.size() );
    for( IT_MissionParameter_Vector it = parameters_.begin(); it != parameters_.end(); ++it )
    {
        MissionParameter* newParam = (*it)->CreateCopy();
        newFragOrder->parameters_.AddItem( newParam );
    }
    return newFragOrder;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FragOrder::ReadArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Data::FragOrder::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.ReadAttribute( "name", strName_ );
    input.ReadAttribute( "available-for-all-mission", isAvailableForAllMissions_, ADN_XmlInput_Helper::eNothing );
    input.ReadAttribute( "available-without-mission", isAvailableWithoutMission_, ADN_XmlInput_Helper::eNothing );
    while( input.NextListElement() )
    {
        input.BeginList( "parameter" );
        std::auto_ptr< MissionParameter > spNew( new MissionParameter() );
        spNew->ReadArchive( input );
        parameters_.AddItem( spNew.release() );
        input.EndList();
    }
}

namespace
{
    QString BuildDiaFragOrderType( const QString& name )
    {
        QStringList list = QStringList::split( ' ', name );
        if( list.front() == "Pion" || list.front() == "Automate" || list.front() == "Population" )
            list[0].append( "_" );
        for( unsigned int i = 1; i < list.size() - 1; ++i )
            if( list[i].length() > 1 && list[i] == list[i].upper() )
                list[i].append( "_" );
        return QString( "Rep_OrderConduite_%1" ).arg( list.join( "" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FragOrder::WriteArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Data::FragOrder::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.WriteAttribute( "name", strName_.GetData() );
    output.WriteAttribute( "available-for-all-mission", isAvailableForAllMissions_.GetData() );
    output.WriteAttribute( "available-without-mission", isAvailableWithoutMission_.GetData() );
    output.WriteAttribute( "dia-type", BuildDiaFragOrderType( strName_.GetData().c_str() ).ascii() );
    for( unsigned int i = 0; i < parameters_.size(); ++i )
        if( parameters_[i]->values_.empty() )
        {
            output.Section( "parameter" );
            parameters_[i]->WriteArchive( output );
            output.EndSection();
        }
        else
        {
            output.BeginList( "parameter", parameters_[i]->values_.size() );
            parameters_[i]->WriteArchive( output );
            output.EndList();
        }
}

// =============================================================================
// Main data
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data constructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Missions_Data::ADN_Missions_Data()
    : ptrSurrenderMission_( automatMissions_, 0 )
    , ptrGoToRefugeeCampMission_( automatMissions_, 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data destructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Missions_Data::~ADN_Missions_Data()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FilesNeeded
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::FilesNeeded( T_StringList& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szMissions_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Reset
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::Reset()
{
    unitMissions_.Reset();
    automatMissions_.Reset();
    populationMissions_.Reset();
    fragOrders_.Reset();
}


namespace
{
    void ReadMissions( ADN_XmlInput_Helper& input, const std::string& name, ADN_Missions_Data::T_Mission_Vector& missions, bool isAutomat )
    {
        input.BeginList( name );
        while( input.NextListElement() )
        {
            input.BeginList( "mission" );
            std::auto_ptr< ADN_Missions_Data::Mission > spNew( new ADN_Missions_Data::Mission( isAutomat ) );
            spNew->ReadArchive( input );
            missions.AddItem( spNew.release() );
            input.EndList();
        }
        input.EndList();
    }
    
    void WriteMissions( MT_OutputArchive_ABC& output, const std::string& name, const ADN_Missions_Data::T_Mission_Vector& missions, unsigned long& id )
    {
        output.BeginList( name, missions.size() );
        for( unsigned int i = 0; i < missions.size(); ++i )
            if( missions[i]->parameters_.empty() )
            {
                output.Section( "mission" );
                output.WriteAttribute( "id", id++ );
                missions[i]->WriteArchive( output, name );
                output.EndSection();
            }
            else
            {
                output.BeginList( "mission", missions[i]->parameters_.size() );
                output.WriteAttribute( "id", id++ );
                missions[i]->WriteArchive( output, name );
                output.EndList();
            }
        output.EndList();
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "missions" );
    
    ReadMissions( input, "units"      , unitMissions_      , false );
    ReadMissions( input, "automats"   , automatMissions_   , true  );
    ReadMissions( input, "populations", populationMissions_, false );

    input.BeginList( "fragorders" );
    while( input.NextListElement() )
    {
        input.BeginList( "fragorder" );
        std::auto_ptr< FragOrder > spNew( new FragOrder() );
        spNew->ReadArchive( input );
        fragOrders_.AddItem( spNew.release() );
        input.EndList();
    }
    input.EndList();

    input.Section( "misc" );
    ReadMiscMission( input, "mission-automat-surrender"         , ptrSurrenderMission_ );
    ReadMiscMission( input, "mission-automat-go-to-refugee-camp", ptrGoToRefugeeCampMission_ );
    input.EndSection();

    input.EndSection();
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadMiscMission
// Created: SBO 2006-12-05
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadMiscMission( ADN_XmlInput_Helper& input, const std::string& name, ADN_TypePtr_InVector_ABC< ADN_Missions_Data::Mission >& ptrMission )
{
    input.Section( name );
    std::string missionName;
    input.ReadAttribute( "name", missionName );
    Mission* mission = FindMission( automatMissions_, missionName );
    if( !mission )
        input.ThrowError( tr( "Mission '%1' does not exist." ).arg( missionName.c_str() ).ascii() );
    ptrMission = mission;
    input.EndSection();
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "missions" );

    unsigned long id = 1;
    WriteMissions( output, "units"      , unitMissions_, id );
    WriteMissions( output, "automats"   , automatMissions_, id );
    WriteMissions( output, "populations", populationMissions_, id );
    
    output.BeginList( "fragorders", fragOrders_.size() );
    for( unsigned int i = 0; i < fragOrders_.size(); ++i )
        if( fragOrders_[i]->parameters_.empty() )
        {
            output.Section( "fragorder" );
            output.WriteAttribute( "id", id++ );
            fragOrders_[i]->WriteArchive( output );
            output.EndSection();
        }
        else
        {
            output.BeginList( "fragorder", fragOrders_[i]->parameters_.size() );
            output.WriteAttribute( "id", id++ );
            fragOrders_[i]->WriteArchive( output );
            output.EndList();
        }
    output.EndList();

    output.Section( "misc" );
        output.Section( "mission-automat-surrender" );
            output.WriteAttribute( "name", ptrSurrenderMission_.GetData()->strName_.GetData() );
        output.EndSection();
        output.Section( "mission-automat-go-to-refugee-camp" );
            output.WriteAttribute( "name", ptrGoToRefugeeCampMission_.GetData()->strName_.GetData() );
        output.EndSection();
    output.EndSection();

    output.EndSection(); // missions
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetFragOrders
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::T_FragOrder_Vector& ADN_Missions_Data::GetFragOrders()
{
    return fragOrders_;
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetUnitMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::T_Mission_Vector& ADN_Missions_Data::GetUnitMissions()
{
    return unitMissions_;
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetAutomatMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::T_Mission_Vector& ADN_Missions_Data::GetAutomatMissions()
{
    return automatMissions_;
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetPopulationMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::T_Mission_Vector& ADN_Missions_Data::GetPopulationMissions()
{
    return populationMissions_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FindFragOrder
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::FragOrder* ADN_Missions_Data::FindFragOrder( const std::string& strName )
{
    IT_FragOrder_Vector it = std::find_if( fragOrders_.begin(), fragOrders_.end(), ADN_Tools::NameCmp< FragOrder >( strName ) );
    if( it == fragOrders_.end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FindMission
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::Mission* ADN_Missions_Data::FindMission( ADN_Missions_Data::T_Mission_Vector& missions, const std::string& strName )
{
    IT_Mission_Vector it = std::find_if( missions.begin(), missions.end(), ADN_Tools::NameCmp< Mission >( strName ) );
    if( it == missions.end() )
        return 0;
    return *it;
}
