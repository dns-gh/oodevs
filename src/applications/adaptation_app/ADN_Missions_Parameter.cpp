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

#include "ADN_Workspace.h"
#include "ADN_Tr.h"
#include "ADN_Objects_Data.h"
#include "ADN_DataException.h"

#include <boost/bind.hpp>

ADN_Missions_Parameter::ADN_Missions_Parameter()
    : isOptional_( false )
    , minOccurs_ ( 1 )
    , maxOccurs_ ( 1 )
    , minValue_  ( std::numeric_limits< int >::min() )
    , maxValue_  ( std::numeric_limits< int >::max() )
    , genObjects_( ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos() )
{
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
    ADN_Missions_Parameter* newParam = new ADN_Missions_Parameter();
    newParam->strName_    = strName_.GetData();
    newParam->type_       = type_.GetData();
    newParam->isOptional_ = isOptional_.GetData();
    newParam->minOccurs_  = minOccurs_.GetData();
    newParam->maxOccurs_  = maxOccurs_.GetData();
    newParam->minValue_   = minValue_.GetData();
    newParam->maxValue_   = maxValue_.GetData();
    newParam->diaName_    = diaName_.GetData();
    newParam->values_.reserve( values_.size() );
    for( IT_MissionParameterValue_Vector it = values_.begin(); it != values_.end(); ++it )
    {
        ADN_Missions_ParameterValue* newParamValue = (*it)->CreateCopy();
        newParam->values_.AddItem( newParamValue );
    }
    assert( choices_.size() == newParam->choices_.size() );
    for( int i = 0; i < choices_.size(); ++i )
    {
        assert( choices_[ i ]->name_ == newParam->choices_[ i ]->name_ );
        newParam->choices_[ i ]->isAllowed_ = choices_[ i ]->isAllowed_.GetData();
    }
    assert( genObjects_.size() == newParam->genObjects_.size() );
    for( int i = 0; i < genObjects_.size(); ++i )
    {
        assert( genObjects_[ i ]->name_.GetData() == newParam->genObjects_[ i ]->name_.GetData() &&
                genObjects_[ i ]->ptrObject_.GetData() == newParam->genObjects_[ i ]->ptrObject_.GetData() );
        newParam->genObjects_[ i ]->isAllowed_ = genObjects_[ i ]->isAllowed_.GetData();
    }
    return newParam;
}

void ADN_Missions_Parameter::ReadArchive( xml::xistream& input )
{
    std::string type;
    std::string max;
    input >> xml::attribute( "name", strName_ )
            >> xml::attribute( "type", type )
            >> xml::optional >> xml::attribute( "optional", isOptional_ )
            >> xml::attribute( "dia-name", diaName_ )
            >> xml::optional >> xml::attribute( "min-occurs", minOccurs_ )
            >> xml::optional >> xml::attribute( "max-occurs", max )
            >> xml::optional >> xml::attribute( "min-value", minValue_ )
            >> xml::optional >> xml::attribute( "max-value", maxValue_ );
    if( max == "unbounded" )
        maxOccurs_ = std::numeric_limits< int >::max();
    else
        input >> xml::optional >> xml::attribute( "max-occurs", maxOccurs_ );
    type_ = ADN_Tr::ConvertToMissionParameterType( type );
    input >> xml::list( "value", *this, &ADN_Missions_Parameter::ReadValue );
    input >> xml::optional
              >> xml::start( "choice" )
                  >> xml::list( "parameter", boost::bind( &ADN_Missions_Parameter::ReadChoice< T_Choice_Vector >, this, _1, boost::ref( choices_ ) ) )
              >> xml::end;
    if( type_.GetData() == eMissionParameterTypeGenObject )
    {
        if( !input.has_child( "objects" ) )
            FillGenObjects();
        else
        {
            input >> xml::start( "objects" )
                      >> xml::list( "parameter", boost::bind( &ADN_Missions_Parameter::ReadChoice< helpers::T_MissionGenObjectTypes_Infos_Vector >, this, _1, boost::ref( genObjects_ ) ) )
                  >> xml::end;
        }
    }
}

void ADN_Missions_Parameter::FillGenObjects()
{
    for( std::size_t i = 0; i < genObjects_.size(); ++i )
        genObjects_[ i ]->isAllowed_ = true;
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
    std::auto_ptr< ADN_Missions_ParameterValue > spNew( new ADN_Missions_ParameterValue() );
    spNew->ReadArchive( input );
    values_.AddItem( spNew.release() );
}

template< typename T >
void ADN_Missions_Parameter::ReadChoice( xml::xistream& input, T& data )
{
    std::string name;
    input >> xml::attribute( "type", name );
    for( std::size_t i = 0; i < data.size(); ++i )
    {
        if( data[i]->name_ == name )
        {
            data[i]->isAllowed_ = true;
            return;
        }
    }
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
}

bool ADN_Missions_Parameter::HasGenObjects() const
{
    for( std::size_t i = 0; i < genObjects_.size(); ++i )
        if( genObjects_[ i ]->isAllowed_.GetData() )
         return true;
    return false;
}

void ADN_Missions_Parameter::WriteArchive( xml::xostream& output )
{
    std::string diaName = diaName_.GetData();
    if( diaName.empty() )
        diaName = GetFussedDiaName( strName_.GetData().c_str() ).toStdString();

    output << xml::start( "parameter" )
            << xml::attribute( "name", strName_ )
            << xml::attribute( "type", ADN_Tr::ConvertFromMissionParameterType( type_.GetData() ) )
            << xml::attribute( "optional", isOptional_ )
            << xml::attribute( "dia-name", diaName );
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
        values_[i]->WriteArchive( output, i );
    Write( output, choices_, type_.GetData(), eMissionParameterTypeLocationComposite, "choice" );
    if( type_.GetData() == eMissionParameterTypeGenObject && !HasGenObjects() )
        throw ADN_DataException( tools::translate( "ADN_Missions_Parameter",  "Invalid mission" ).toStdString(),
            tools::translate( "ADN_Missions_Parameter", "'%1' parameter should have at least one object." )
                .arg( strName_.GetData().c_str() ).toStdString() );
    Write( output, genObjects_, type_.GetData(), eMissionParameterTypeGenObject, "objects" );
    output << xml::end;
}
