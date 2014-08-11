// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Missions_Parameter.h"
#include "ADN_Missions_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Tr.h"
#include "ADN_Objects_Data.h"
#include "ADN_WorkspaceElement.h"
#include "ENT/ENT_Tr.h"

#include <boost/bind.hpp>

ADN_Missions_Parameter::ADN_Missions_Parameter(E_MissionType type)
    : isOptional_( false )
    , minOccurs_ ( 1 )
    , maxOccurs_ ( 1 )
    , minValue_  ( std::numeric_limits< int >::min() )
    , maxValue_  ( std::numeric_limits< int >::max() )
    , isContext_( false )
    , missionType_( type )
    , indirectFire_( false )
    , ownedEquipments_( false )
{
    std::string context = ENT_Tr::ConvertFromMissionType( type, ENT_Tr::eToSim );
    if( type != eMissionType_FragOrder )
        context += "-missions";
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eMissions, context + "-parameters" ) );
    description_.SetContext( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissionSheetContext( missionType_ ) );
    FillChoices();
}

ADN_Missions_Parameter::~ADN_Missions_Parameter()
{
    // NOTHING
}

std::string ADN_Missions_Parameter::GetItemName()
{
    return strName_.GetData();
}

ADN_Missions_Parameter* ADN_Missions_Parameter::CreateCopy()
{
    ADN_Missions_Parameter* newParam = new ADN_Missions_Parameter( missionType_ );
    newParam->strName_    = strName_.GetData();
    newParam->description_ = description_.GetData();
    newParam->type_       = type_.GetData();
    newParam->isOptional_ = isOptional_.GetData();
    newParam->minOccurs_  = minOccurs_.GetData();
    newParam->maxOccurs_  = maxOccurs_.GetData();
    newParam->minValue_   = minValue_.GetData();
    newParam->maxValue_   = maxValue_.GetData();
    newParam->diaName_    = diaName_.GetData();
    newParam->indirectFire_ = indirectFire_.GetData();
    newParam->ownedEquipments_ = ownedEquipments_.GetData();
    newParam->isContext_  = isContext_;
    newParam->values_.reserve( values_.size() );
    for( auto it = values_.begin(); it != values_.end(); ++it )
    {
        ADN_Missions_ParameterValue* newParamValue = (*it)->CreateCopy();
        newParam->values_.AddItem( newParamValue );
    }
    assert( choices_.size() == newParam->choices_.size() );
    for( unsigned int i = 0; i < choices_.size(); ++i )
    {
        assert( choices_[ i ]->name_ == newParam->choices_[ i ]->name_ );
        newParam->choices_[ i ]->isAllowed_ = choices_[ i ]->isAllowed_.GetData();
    }
    if( type_.GetData() == eMissionParameterTypeGenObject || type_.GetData() == eMissionParameterTypeObjectKnowledge ||
        type_.GetData() == eMissionParameterTypePhaseLine )
    {
        newParam->objects_.SetFixedVector( ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos() );
        for( unsigned int i = 0; i < objects_.size(); ++i )
            newParam->objects_[ i ]->isAllowed_ = objects_[ i ]->isAllowed_.GetData();
    }
    return newParam;
}

namespace
{
    void ReadObjects( xml::xistream& input, helpers::T_MissionGenObjectTypes_Infos_Vector& data )
    {
        std::string name = input.attribute< std::string >( "type" );
        for( std::size_t i = 0; i < data.size(); ++i )
            if( data[ i ]->GetCrossedElement() && data[ i ]->GetCrossedElement()->strType_.GetData() == name )
            {
                data[ i ]->isAllowed_ = true;
                return;
            }
    }
}

void ADN_Missions_Parameter::ReadArchive( xml::xistream& input )
{
    std::string type;
    std::string max;
    input >> xml::attribute( "name", *this )
          >> xml::attribute( "type", type )
          >> xml::optional >> xml::attribute( "optional", isOptional_ )
          >> xml::attribute( "dia-name", diaName_ )
          >> xml::optional >> xml::attribute( "indirect-fire-only", indirectFire_ )
          >> xml::optional >> xml::attribute( "owned-equipments-only", ownedEquipments_ )
          >> xml::optional >> xml::attribute( "min-occurs", minOccurs_ )
          >> xml::optional >> xml::attribute( "max-occurs", max )
          >> xml::optional >> xml::attribute( "min-value", minValue_ )
          >> xml::optional >> xml::attribute( "max-value", maxValue_ )
          >> xml::optional >> xml::attribute( "is-context", isContext_ );
    if( ADN_Tr::ConvertToContextParameters( diaName_.GetData() ) > -1 )
        isContext_ = true;
    if( max == "unbounded" )
        maxOccurs_ = std::numeric_limits< int >::max();
    else
        input >> xml::optional >> xml::attribute( "max-occurs", maxOccurs_ );
    type_ = ADN_Tr::ConvertToMissionParameterType( type );
    input >> xml::list( "value", *this, &ADN_Missions_Parameter::ReadValue );
    input >> xml::optional
          >> xml::start( "choice" )
            >> xml::list( "parameter", boost::bind( &ADN_Missions_Parameter::ReadChoiceVector, this, _1, boost::ref( choices_ ) ) )
          >> xml::end;

    if( type_.GetData() == eMissionParameterTypeGenObject || type_.GetData() == eMissionParameterTypeObjectKnowledge ||
        type_.GetData() == eMissionParameterTypePhaseLine )
    {
        objects_.SetFixedVector( ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos() );
        bool all = false;
        input >> xml::start( "objects" )
                >> xml::optional >> xml::attribute( "all", all )
                >> xml::list( "parameter", boost::bind( &ReadObjects, _1, boost::ref( objects_ ) ) )
              >> xml::end;
        if( all )
            for( std::size_t i = 0; i < objects_.size(); ++i )
                objects_[ i ]->isAllowed_ = true;
    }
}

void ADN_Missions_Parameter::FillChoices()
{
    choices_.AddItem( new ADN_Missions_Type( "Point" ) );
    choices_.AddItem( new ADN_Missions_Type( "Path" ) );
    choices_.AddItem( new ADN_Missions_Type( "Polygon" ) );
    choices_.AddItem( new ADN_Missions_Type( "Automat" ) );
    choices_.AddItem( new ADN_Missions_Type( "Agent" ) );
    choices_.AddItem( new ADN_Missions_Type( "AgentKnowledge" ) );
    choices_.AddItem( new ADN_Missions_Type( "ObjectKnowledge" ) );
    choices_.AddItem( new ADN_Missions_Type( "CrowdKnowledge" ) );
    choices_.AddItem( new ADN_Missions_Type( "UrbanKnowledge" ) );
}

void ADN_Missions_Parameter::ReadValue( xml::xistream& input )
{
    std::unique_ptr< ADN_Missions_ParameterValue > spNew( new ADN_Missions_ParameterValue( missionType_ ) );
    spNew->ReadArchive( input );
    values_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Parameter::ReadChoiceVector
// Created: JSR 2013-04-10
// -----------------------------------------------------------------------------
void ADN_Missions_Parameter::ReadChoiceVector( xml::xistream& input, T_Choice_Vector& data )
{
    std::string name = input.attribute< std::string >( "type" );
    for( std::size_t i = 0; i < data.size(); ++i )
    {
        if( data[ i ]->name_ == name )
        {
            data[ i ]->isAllowed_ = true;
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Parameter::UpdateObjectsVectors
// Created: JSR 2013-07-26
// -----------------------------------------------------------------------------
void ADN_Missions_Parameter::UpdateObjectsVectors()
{
    if( type_.GetData() == eMissionParameterTypeGenObject || type_.GetData() == eMissionParameterTypeObjectKnowledge ||
        type_.GetData() == eMissionParameterTypePhaseLine )
    {
        if( objects_.empty() )
        {
            objects_.SetFixedVector( ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos() );
            for( std::size_t i = 0; i < objects_.size(); ++i )
                objects_[ i ]->isAllowed_ = type_.GetData() != eMissionParameterTypePhaseLine;
        }
    }
    else
        objects_.ResetFixedVector();
}

namespace
{
    QString GetFussedDiaName( const QString& name )
    {
        QStringList list = QStringList::split( ' ', name );
        for( int i = 0; i < list.size(); ++i )
            list[i][0] = i == 0 ? list[i][0].lower() : list[i][0].upper();
        return list.join( "" ).append( '_' );
    }

    template< typename T >
    void Write(  xml::xostream& output, const T& data, E_MissionParameterType type, E_MissionParameterType expected, const std::string& tag )
    {
        bool hasChoice = false;
        for( std::size_t i = 0; i < data.size() && !hasChoice; ++i )
            hasChoice = data[i]->isAllowed_.GetData();
        if( hasChoice && type == expected )
        {
            output << xml::start( tag );
            for( std::size_t i = 0; i < data.size(); ++i )
                data[i]->WriteArchive( output );
            output << xml::end;
        }
    }

    std::size_t CountObject( const helpers::T_MissionGenObjectTypes_Infos_Vector& vector )
    {
        std::size_t result = 0;
        for( std::size_t i = 0; i < vector.size(); ++i )
            if( vector[ i ]->isAllowed_.GetData() )
                ++result;
        return result;
    }

    void WriteObjects( xml::xostream& output, const helpers::T_MissionGenObjectTypes_Infos_Vector& vector )
    {
        output << xml::start( "objects" );
        if( CountObject( vector ) == vector.size() )
            output << xml::attribute( "all", true );
        else
            for( std::size_t i = 0; i < vector.size(); ++i )
                vector[i]->WriteArchive( output );
        output << xml::end;
    }
}

void ADN_Missions_Parameter::WriteArchive( xml::xostream& output ) const
{
    std::string diaName = diaName_.GetData();
    if( diaName.empty() )
        diaName = GetFussedDiaName( strName_.GetData().c_str() ).toStdString();

    output << xml::start( "parameter" )
            << xml::attribute( "name", *this )
            << xml::attribute( "type", ADN_Tr::ConvertFromMissionParameterType( type_.GetData() ) )
            << xml::attribute( "optional", isOptional_ )
            << xml::attribute( "dia-name", diaName );
    if( indirectFire_.GetData() )
        output << xml::attribute( "indirect-fire-only", true );
    if( ownedEquipments_.GetData() )
        output << xml::attribute( "owned-equipments-only", true );
    if( isContext_ )
        output << xml::attribute( "is-context", isContext_ );
    if( maxOccurs_ != 1 )
    {
        output << xml::attribute( "min-occurs", minOccurs_ );
        if( maxOccurs_ == std::numeric_limits< int >::max() )
            output << xml::attribute( "max-occurs", "unbounded" );
        else
            output << xml::attribute( "max-occurs", maxOccurs_ );
    }
    if( minValue_ != std::numeric_limits< int >::min() )
        output << xml::attribute( "min-value", minValue_ );
    if( maxValue_ != std::numeric_limits< int >::max() )
        output << xml::attribute( "max-value", maxValue_ );
    for( unsigned int i = 0; i < values_.size(); ++i )
        values_[i]->WriteArchive( output );
    Write( output, choices_, type_.GetData(), eMissionParameterTypeLocationComposite, "choice" );

    if( type_.GetData() == eMissionParameterTypeGenObject || type_.GetData() == eMissionParameterTypeObjectKnowledge ||
        type_.GetData() == eMissionParameterTypePhaseLine )
        WriteObjects( output, objects_ );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Parameter::CheckValidity
// Created: ABR 2013-09-26
// -----------------------------------------------------------------------------
void ADN_Missions_Parameter::CheckValidity()
{
    ADN_RefWithLocalizedName::CheckValidity();
    CheckTypeValidity( description_ );
    for( auto it = values_.begin(); it != values_.end(); ++it )
        ( *it )->CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Parameter::CheckDatabaseValidity
// Created: LDC 2014-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Parameter::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    if( ( type_.GetData() == eMissionParameterTypeGenObject || type_.GetData() == eMissionParameterTypeObjectKnowledge )
        && CountObject( objects_ ) == 0 )
        checker.AddError( eMissingObjectParameter, strName_.GetData(), eMissions );
    std::set< int > ids;
    for( auto it = values_.begin(); it != values_.end(); ++it )
    {
        if( !ids.insert( (*it)->id_.GetData() ).second )
        {
            checker.AddError( eDuplicateEnumeration, strName_.GetData(), eMissions );
            break;
        }
    }
}
