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

#include "ADN_DataException.h"
#include "ADN_GuiTools.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"
#include "ADN_Objects_Data_ObjectInfos.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_AiEngine_Data.h"
#include "ADN_Objects_Data.h"
#include "ADN_Tr.h"
#include "tools/Loader_ABC.h"
#include <boost/bind.hpp>
#include <tools/XmlCrc32Signature.h>
#include <xeuseuleu/xsl.hpp>

tools::IdManager ADN_Missions_Data::idManager_;

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
void ADN_Missions_Data::MissionParameterValue::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", name_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameterValue::WriteArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Data::MissionParameterValue::WriteArchive( xml::xostream& output, unsigned int id )
{
    output << xml::start( "value" )
                << xml::attribute( "id", id )
                << xml::attribute( "name", name_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameter::MissionParameter
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::MissionParameter::MissionParameter()
    : isOptional_( false )
    , minOccurs_ ( 1 )
    , maxOccurs_ ( 1 )
    , minValue_  ( std::numeric_limits< int >::min() )
    , maxValue_  ( std::numeric_limits< int >::max() )
    , knowledgeObjects_( ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos() )
{
    ADN_Type_Enum< E_MissionParameterType, eNbrMissionParameterType >::SetConverter( &ADN_Tr::ConvertFromMissionParameterType );
    ADN_Type_Enum< E_AnchorType, eNbrAnchorType >::SetConverter( &ADN_Tr::ConvertFromAnchorType );
    FillChoices();
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
    newParam->strName_    = strName_.GetData();
    newParam->type_       = type_.GetData();
    newParam->anchor_       = anchor_.GetData();
    newParam->isOptional_ = isOptional_.GetData();
    newParam->minOccurs_  = minOccurs_.GetData();
    newParam->maxOccurs_  = maxOccurs_.GetData();
    newParam->minValue_   = minValue_.GetData();
    newParam->maxValue_   = maxValue_.GetData();
    newParam->diaName_    = diaName_.GetData();
    newParam->values_.reserve( values_.size() );
    for( IT_MissionParameterValue_Vector it = values_.begin(); it != values_.end(); ++it )
    {
        MissionParameterValue* newParamValue = (*it)->CreateCopy();
        newParam->values_.AddItem( newParamValue );
    }
    newParam->choices_.reserve( choices_.size() );
    for( IT_Choice_Vector it = choices_.begin(); it != choices_.end(); ++it )
    {
        MissionType* newType = (*it)->CreateCopy();
        newParam->choices_.AddItem( newType );
    }
    assert( knowledgeObjects_.size() == newParam->knowledgeObjects_.size() );
    for( unsigned int i = 0; i < knowledgeObjects_.size(); ++i )
    {
        assert( knowledgeObjects_[ i ]->name_.GetData() == newParam->knowledgeObjects_[ i ]->name_.GetData() &&
                knowledgeObjects_[ i ]->ptrObject_.GetData() == newParam->knowledgeObjects_[ i ]->ptrObject_.GetData() );
        newParam->knowledgeObjects_[ i ]->isAllowed_ = knowledgeObjects_[ i ]->isAllowed_.GetData();
    }
    return newParam;
}

namespace
{
    void FillGenObjects( helpers::T_MissionGenObjectTypes_Infos_Vector& vector )
    {
        for( std::size_t i = 0; i < vector.size(); ++i )
            vector[ i ]->isAllowed_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameter::ReadArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Data::MissionParameter::ReadArchive( xml::xistream& input )
{
    std::string type;
    std::string max;
    std::string anchor;
    input >> xml::attribute( "name", strName_ )
            >> xml::attribute( "type", type )
            >> xml::optional >> xml::attribute( "optional", isOptional_ )
            >> xml::attribute( "dia-name", diaName_ )
            >> xml::optional >> xml::attribute( "min-occurs", minOccurs_ )
            >> xml::optional >> xml::attribute( "max-occurs", max )
            >> xml::optional >> xml::attribute( "min-value", minValue_ )
            >> xml::optional >> xml::attribute( "max-value", maxValue_ )
            >> xml::optional >> xml::attribute( "anchor", anchor );
    if( max == "unbounded" )
        maxOccurs_ = std::numeric_limits< int >::max();
    else
        input >> xml::optional >> xml::attribute( "max-occurs", maxOccurs_ );
    type_ = ADN_Tr::ConvertToMissionParameterType( type );
    anchor_ = ADN_Tr::ConvertToAnchorType( anchor );
    input >> xml::list( "value", *this, &ADN_Missions_Data::MissionParameter::ReadValue );
    input >> xml::optional() >> xml::start( "choice" )
            >> xml::list( "parameter", boost::bind( &ADN_Missions_Data::MissionParameter::ReadChoice< T_Choice_Vector >, this, _1, boost::ref( choices_ ) ) )
          >> xml::end;
    if( type_.GetData() == eMissionParameterTypeObjectKnowledge )
    {
        if( !input.has_child( "objects" ) )
            FillGenObjects( knowledgeObjects_ );
        else
        {
            input >> xml::start( "objects" )
                    >> xml::list( "parameter", boost::bind( &ADN_Missions_Data::MissionParameter::ReadChoice< helpers::T_MissionGenObjectTypes_Infos_Vector >, this, _1, boost::ref( knowledgeObjects_ ) ) )
                  >> xml::end;
        }
    }

}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameter::FillChoices
// Created: LDC 2010-08-19
// -----------------------------------------------------------------------------
void ADN_Missions_Data::MissionParameter::FillChoices()
{
    choices_.AddItem( new MissionType( "Point" ) );
    choices_.AddItem( new MissionType( "Path" ) );
    choices_.AddItem( new MissionType( "Polygon" ) );
    choices_.AddItem( new MissionType( "Automat" ) );
    choices_.AddItem( new MissionType( "Agent" ) );
    choices_.AddItem( new MissionType( "AgentKnowledge" ) );
    choices_.AddItem( new MissionType( "ObjectKnowledge" ) );
    choices_.AddItem( new MissionType( "CrowdKnowledge" ) );
    choices_.AddItem( new MissionType( "UrbanKnowledge" ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameter::ReadValue
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Missions_Data::MissionParameter::ReadValue( xml::xistream& input )
{
    std::auto_ptr< MissionParameterValue > spNew( new MissionParameterValue() );
    spNew->ReadArchive( input );
    values_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameter::ReadChoice
// Created: LDC 2010-08-19
// -----------------------------------------------------------------------------
template< typename T >
void ADN_Missions_Data::MissionParameter::ReadChoice( xml::xistream& input, T& data )
{
    std::string name;
    input >> xml::attribute( "type", name );
    for( std::size_t i = 0; i < data.size(); ++i )
    {
        if( data[i]->GetInputName() == name )
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

    std::size_t CountObject( const helpers::T_MissionGenObjectTypes_Infos_Vector& vector )
    {
        std::size_t result = 0;
        for( std::size_t i = 0; i < vector.size(); ++i )
            if( vector[ i ]->isAllowed_.GetData() )
                ++result;
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameter::WriteArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Data::MissionParameter::WriteArchive( xml::xostream& output )
{
    std::string diaName = diaName_.GetData();
    if( diaName.empty() )
        diaName = GetFussedDiaName( strName_.GetData().c_str() ).toAscii().constData();

    output << xml::start( "parameter" )
            << xml::attribute( "name", strName_ )
            << xml::attribute( "type", ADN_Tr::ConvertFromMissionParameterType( type_.GetData() ) )
            << xml::attribute( "optional", isOptional_ )
            << xml::attribute( "dia-name", diaName );
    if( anchor_.GetData() != "" )
        output << xml::attribute( "anchor", ADN_Tr::ConvertFromAnchorType( anchor_.GetData() ) );
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

    std::size_t nbKnowledgeObject = CountObject( knowledgeObjects_ );
    if( type_.GetData() == eMissionParameterTypeObjectKnowledge && nbKnowledgeObject == 0 )
        throw ADN_DataException( tools::translate( "Categories_Data", "Invalid mission parameter" ).toStdString(), tools::translate( "ADN_Missions_Parameter", "'%1' parameter should have at least one object." ).arg( strName_.GetData().c_str() ).toStdString() );
    if( nbKnowledgeObject != knowledgeObjects_.size() )
        Write( output, knowledgeObjects_, type_.GetData(), eMissionParameterTypeObjectKnowledge, "objects" );
    output << xml::end;
}

// =============================================================================
// Mission_ABC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Mission_ABC
// Created: ABR 2013-05-21
// -----------------------------------------------------------------------------
ADN_Missions_Data::ADN_Missions_ABC::ADN_Missions_ABC()
    : id_( ADN_Missions_Data::idManager_.GetNextId() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ADN_Missions_ABC
// Created: ABR 2013-05-21
// -----------------------------------------------------------------------------
ADN_Missions_Data::ADN_Missions_ABC::ADN_Missions_ABC( unsigned int id )
    : id_( id )
{
    ADN_Missions_Data::idManager_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::~ADN_Missions_ABC
// Created: ABR 2013-05-21
// -----------------------------------------------------------------------------
ADN_Missions_Data::ADN_Missions_ABC::~ADN_Missions_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetItemName
// Created: ABR 2013-05-21
// -----------------------------------------------------------------------------
std::string ADN_Missions_Data::ADN_Missions_ABC::GetItemName()
{
    return strName_.GetData();
}

// =============================================================================
// Missions
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Mission::Mission
// Created: SBO 2009-11-16
// -----------------------------------------------------------------------------
ADN_Missions_Data::Mission::Mission()
    : ADN_Missions_ABC()
    , strPackage_( ADN_Workspace::GetWorkspace().GetMissions().GetData().activitiesData_->GetPackages(), 0 )
{
    symbol_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Mission::Mission
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::Mission::Mission( unsigned int id )
    : ADN_Missions_ABC( id )
    , strPackage_( ADN_Workspace::GetWorkspace().GetMissions().GetData().activitiesData_->GetPackages(), 0 )
{
    symbol_.SetParentNode( *this );
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
// Name: ADN_Missions_Data::Mission::CreateCopy
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::ADN_Missions_ABC* ADN_Missions_Data::Mission::CreateCopy()
{
    Mission* newMission              = new Mission();
    newMission->strName_             = strName_.GetData();
    newMission->diaBehavior_         = diaBehavior_.GetData();
    newMission->cdtDiaBehavior_      = cdtDiaBehavior_.GetData();
    newMission->mrtDiaBehavior_      = mrtDiaBehavior_.GetData();
    newMission->doctrineDescription_ = doctrineDescription_.GetData();
    newMission->usageDescription_    = usageDescription_.GetData();
    newMission->strPackage_          = strPackage_.GetData();
    newMission->symbol_              = symbol_.GetData();
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
void ADN_Missions_Data::Mission::ReadArchive( xml::xistream& input, std::size_t contextLength )
{
    std::string doctrineDesc, usageDesc, symbol, strPackage;
    std::size_t index = 0;
    input >> xml::attribute( "name", strName_ )
        >> xml::attribute( "dia-type", diaType_ )
        >> xml::optional >> xml::attribute( "symbol", symbol )
        >> xml::optional >> xml::attribute( "dia-behavior", diaBehavior_ )
        >> xml::optional >> xml::attribute( "cdt-dia-behavior", cdtDiaBehavior_ )
        >> xml::optional >> xml::attribute( "mrt-dia-behavior", mrtDiaBehavior_ )
        >> xml::optional >> xml::start( "description" )
            >> xml::optional >> xml::start( "doctrine" ) >> doctrineDesc >> xml::end
            >> xml::optional >> xml::start( "usage" ) >> usageDesc >> xml::end
        >> xml::end
        >> xml::optional >> xml::attribute( "package", strPackage );
    if( !strPackage.empty() )
    {
        if( strPackage == "none" )
            strPackage = " - ";
        ADN_Activities_Data::PackageInfos* pPackage = ADN_Workspace::GetWorkspace().GetMissions().GetData().activitiesData_->FindPackage( strPackage );
        if( !pPackage )
            ADN_Workspace::GetWorkspace().GetMissions().GetData().activitiesData_->GetPackages().AddItem( new ADN_Activities_Data::PackageInfos( strPackage ) );
        strPackage_ = pPackage;
    }
    input >> xml::list( "parameter", boost::bind( &ADN_Missions_Data::Mission::ReadParameter, this , _1,  boost::ref( index ), contextLength ) );
    doctrineDescription_ = doctrineDesc;
    usageDescription_ = usageDesc;
    const std::string code = symbol.empty() ? " - " : symbol;
    ADN_Drawings_Data& drawingsData = ADN_Workspace::GetWorkspace().GetDrawings().GetData();
    symbol_.SetVector( drawingsData.GetCategoryDrawings( "tasks" ) );
    symbol_.SetData( drawingsData.GetDrawing( code ), false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Mission::ReadParameter
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Missions_Data::Mission::ReadParameter( xml::xistream& input, std::size_t& index, std::size_t contextLength )
{
    index++;
    if( index > contextLength )
    {
        std::auto_ptr< MissionParameter > spNew( new MissionParameter() );
        spNew->ReadArchive( input );
        parameters_.AddItem( spNew.release() );
    }
}

namespace
{
    QString BuildDiaMissionType( const QString& name )
    {
        QStringList list = QStringList::split( ' ', name );
        for( int i = 0; i < list.size() - 1; ++i )
            if( list[i].length() > 1 && ( list[i] == list[i].upper() || list[i].lower() == "test" ) )
                list[i].append( "_" );
        return list.join( "" );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Mission::WriteArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Data::Mission::WriteArchive( xml::xostream& output, E_MissionType type, const T_MissionParameter_Vector* context )
{
    output << xml::start( "mission" );
    bool isAutomat = type == eMissionType_Automat;
    const QString typeName = type == eMissionType_Pawn ? "Pion" : (isAutomat ? "Automate" : "Population");

    const QString diaName  = BuildDiaMissionType( strName_.GetData().c_str() );
    if( diaType_.GetData().empty() )
        diaType_ = QString( "T_Mission_%1_%2" ).arg( typeName ).arg( diaName ).toAscii().constData();

    output << xml::attribute( "name", strName_ )
           << xml::attribute( "dia-type", diaType_ )
           << xml::attribute( "id", id_ );

    const std::string code = ( symbol_.GetData() ) ? symbol_.GetData()->GetCode() : "";
    if( code != "" && code != " - " )
        output << xml::attribute( "symbol", code );

    if ( strPackage_.GetData() != 0 && ! strPackage_.GetData()->strName_.GetData().empty() )
    {
        std::string val = strPackage_.GetData()->strName_.GetData();
        if( strPackage_.GetData()->strName_.GetData() == " - " )
            output << xml::attribute( "package", "none" );
        else
            output << xml::attribute( "package", strPackage_.GetData()->strName_.GetData() );
    }

    if( !isAutomat )
    {
        if( diaBehavior_.GetData().empty() )
            diaBehavior_ = QString( "MIS_%1_%2" ).arg( typeName ).arg( diaName ).toAscii().constData();
        output << xml::attribute( "dia-behavior", diaBehavior_ );
    }
    else
    {
        if( cdtDiaBehavior_.GetData().empty() )
            cdtDiaBehavior_ = QString( "MIS_%1_CDT_%2" ).arg( typeName ).arg( diaName ).toAscii().constData();
        if( mrtDiaBehavior_.GetData().empty() )
            mrtDiaBehavior_ = QString( "MIS_%1_MRT_%2" ).arg( typeName ).arg( diaName ).toAscii().constData();
        output << xml::attribute( "mrt-dia-behavior", mrtDiaBehavior_ )
               << xml::attribute( "cdt-dia-behavior", cdtDiaBehavior_ );
    }
    if( ! doctrineDescription_.GetData().empty() || ! usageDescription_.GetData().empty() )
    {
        output << xml::start( "description" );
        if( ! doctrineDescription_.GetData().empty() )
            output << xml::start( "doctrine" ) << xml::cdata( doctrineDescription_.GetData() ) << xml::end;
        if( ! usageDescription_.GetData().empty() )
            output << xml::start( "usage" ) << xml::cdata( usageDescription_.GetData() ) << xml::end;
        output << xml::end;
    }
    if( context && !context->empty() )
    {
        for( unsigned i = 0; i < context->size(); ++i )
            ( *context )[i]->WriteArchive( output );
    }
    for( unsigned int i = 0; i < parameters_.size(); ++i )
        parameters_[i]->WriteArchive( output );

    output << xml::end;
}

// =============================================================================
// Frag orders
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FragOrder::FragOrder
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::FragOrder::FragOrder()
    : ADN_Missions_ABC()
    , isAvailableWithoutMission_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FragOrder::FragOrder
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::FragOrder::FragOrder( unsigned int id )
    : ADN_Missions_ABC( id )
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
// Name: ADN_Missions_Data::FragOrder::CreateCopy
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::ADN_Missions_ABC* ADN_Missions_Data::FragOrder::CreateCopy()
{
    FragOrder* newFragOrder = new FragOrder();
    newFragOrder->strName_ = strName_.GetData();
    newFragOrder->doctrineDescription_ = doctrineDescription_.GetData();
    newFragOrder->usageDescription_ = doctrineDescription_.GetData();
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
void ADN_Missions_Data::FragOrder::ReadArchive( xml::xistream& input, std::size_t /* contextLength */ )
{
    std::string doctrineDesc, usageDesc;
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "dia-type", diaType_ )
          >> xml::optional >> xml::attribute( "available-without-mission", isAvailableWithoutMission_ )
          >> xml::optional >> xml::start( "description" )
            >> xml::start( "doctrine" ) >> doctrineDesc >> xml::end
            >> xml::start( "usage" ) >> usageDesc >> xml::end
          >> xml::end
          >> xml::list( "parameter", *this, &ADN_Missions_Data::FragOrder::ReadParameter );
    doctrineDescription_ = doctrineDesc;
    usageDescription_ = usageDesc;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FragOrder::ReadParameter
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Missions_Data::FragOrder::ReadParameter( xml::xistream& input )
{
    std::auto_ptr< MissionParameter > spNew( new MissionParameter() );
    spNew->ReadArchive( input );
    parameters_.AddItem( spNew.release() );
}

namespace
{
    QString BuildDiaFragOrderType( const QString& name )
    {
        QStringList list = QStringList::split( ' ', name );
        if( list.front() == "Pion" || list.front() == "Automate" || list.front() == "Population" )
            list[0].append( "_" );
        for( int i = 1; i < list.size() - 1; ++i )
            if( list[i].length() > 1 && list[i] == list[i].upper() )
                list[i].append( "_" );
        return QString( "Rep_OrderConduite_%1" ).arg( list.join( "" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FragOrder::WriteArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Data::FragOrder::WriteArchive( xml::xostream& output, E_MissionType /*type*/, const T_MissionParameter_Vector* /* context */ )
{
    if( diaType_.GetData().empty() )
        diaType_ = BuildDiaFragOrderType( strName_.GetData().c_str() ).toAscii().constData();

    output << xml::start( "fragorder" )
            << xml::attribute( "name", strName_ )
            << xml::attribute( "dia-type", diaType_ )
            << xml::attribute( "id", id_ )
            << xml::attribute( "available-without-mission", isAvailableWithoutMission_ );
    if( ! doctrineDescription_.GetData().empty() || ! usageDescription_.GetData().empty() )
    {
        output << xml::start( "description" );
        if( ! doctrineDescription_.GetData().empty() )
            output << xml::start( "doctrine" ) << xml::cdata( doctrineDescription_.GetData() ) << xml::end;
        if( ! usageDescription_.GetData().empty() )
            output << xml::start( "usage" ) << xml::cdata( usageDescription_.GetData() ) << xml::end;
        output << xml::end;
    }
    for( unsigned int i = 0; i < parameters_.size(); ++i )
        parameters_[i]->WriteArchive( output );
    output << xml::end;
}

// =============================================================================
// Main data
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data constructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Missions_Data::ADN_Missions_Data()
    : activitiesData_( new ADN_Activities_Data() )
{
    missionsVector_.push_back( T_Mission_ABC_Vector() );
    missionsVector_.push_back( T_Mission_ABC_Vector() );
    missionsVector_.push_back( T_Mission_ABC_Vector() );
    missionsVector_.push_back( T_Mission_ABC_Vector() );
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
    activitiesData_->FilesNeeded( files );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Reset
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::Reset()
{
    idManager_.Reset();
    for( int i = 0; i < eNbrMissionTypes; ++i )
        missionsVector_[ i ].Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Load
// Created: LDC 2010-09-24
// -----------------------------------------------------------------------------
void ADN_Missions_Data::Load( const tools::Loader_ABC& fileLoader )
{
    T_StringList fileList;
    FilesNeeded( fileList );
    if( ! fileList.empty() )
    {
        if( fileList.size() == 2 )
        {
            const std::string strFile2 = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() + fileList.back();
            ReadActivity( *fileLoader.LoadFile( strFile2 ).get() );
        }
        const std::string strFile = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() + fileList.front();
        fileLoader.LoadFile( strFile, boost::bind( &ADN_Missions_Data::ReadArchive, this, _1 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetUnitThatUse
// Created: NPT 2013-04-19
// -----------------------------------------------------------------------------
QStringList ADN_Missions_Data::GetUnitMissionsThatUse( ADN_Activities_Data::PackageInfos& package )
{
    QStringList result;
    for( auto it = missionsVector_[ eMissionType_Pawn ].begin(); it != missionsVector_[ eMissionType_Pawn ].end(); ++it )
    {
        Mission* mission = static_cast< Mission* >( *it );
        if( mission->strPackage_.GetData()->strName_.GetData() == package.strName_.GetData() )
            result << mission->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetAutomataThatUse
// Created: NPT 2013-04-19
// -----------------------------------------------------------------------------
QStringList ADN_Missions_Data::GetAutomataMissionsThatUse( ADN_Activities_Data::PackageInfos& package )
{
    QStringList result;
    for( auto it = missionsVector_[ eMissionType_Automat ].begin(); it != missionsVector_[ eMissionType_Automat ].end(); ++it )
    {
        Mission* mission = static_cast< Mission* >( *it );
        if( mission->strPackage_.GetData()->strName_.GetData() == package.strName_.GetData() )
            result << mission->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadArchive( xml::xistream& input )
{
    // MGD 2010-10-03 : Hack to hide context @TODO remove with context deletion
    xml::xistringstream unitContextFlow(
        "<context>"
        "  <parameter dia-name='dangerDirection_' name='Direction dangereuse' optional='false' type='Heading'/>"
        "  <parameter dia-name='phaseLines_' max-occurs='unbounded' min-occurs='1' name='Limas' optional='true' type='PhaseLine'/>"
        "  <parameter dia-name='boundaryLimit1_' name='Limit 1' optional='true' type='Limit'/>"
        "  <parameter dia-name='boundaryLimit2_' name='Limit 2' optional='true' type='Limit'/>"
        "</context>" );
    xml::xistringstream automatContextFlow(
        "<context>"
        "  <parameter dia-name='dangerDirection_' name='Direction dangereuse' optional='false' type='Heading'/>"
        "  <parameter dia-name='phaseLines_' max-occurs='unbounded' min-occurs='1' name='Limas' optional='true' type='PhaseLine'/>"
        "  <parameter dia-name='boundaryLimit1_' name='Limit 1' optional='false' type='Limit'/>"
        "  <parameter dia-name='boundaryLimit2_' name='Limit 2' optional='false' type='Limit'/>"
        "</context>" );
    xml::xistringstream crowContextFlow( "<context></context>" );
    ReadContext( unitContextFlow, unitContext_ );
    ReadContext( automatContextFlow, automatContext_ );
    ReadContext( crowContextFlow, populationContext_ );

    input >> xml::start( "missions" )
            >> xml::start( "units" )
                >> xml::list( "mission", boost::bind( &ADN_Missions_Data::ReadMission, this, _1, boost::ref( missionsVector_[ eMissionType_Pawn ] ), unitContext_.size() ) )
            >> xml::end
            >> xml::start( "automats" )
                >> xml::list( "mission", boost::bind( &ADN_Missions_Data::ReadMission, this, _1, boost::ref( missionsVector_[ eMissionType_Automat ] ), automatContext_.size() ) )
            >> xml::end
            >> xml::start( "populations" )
                >> xml::list( "mission", boost::bind( &ADN_Missions_Data::ReadMission, this, _1, boost::ref( missionsVector_[ eMissionType_Population ] ), populationContext_.size() ) )
            >> xml::end
            >> xml::start( "fragorders" )
                >> xml::list( "fragorder", *this, &ADN_Missions_Data::ReadFragOrder )
            >> xml::end
          >> xml::end;

    CheckAndFixLoadingErrors();
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadActivity
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadActivity( xml::xistream& input )
{
    activitiesData_->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadFragOrder
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadFragOrder( xml::xistream& xis )
{
    std::auto_ptr< FragOrder > spNew( new FragOrder( xis.attribute< unsigned int >( "id" ) ) );
    spNew->ReadArchive( xis, 0 );
    missionsVector_[ eMissionType_FragOrder ].AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadMission
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadMission( xml::xistream& xis, T_Mission_ABC_Vector& missions, std::size_t contextLength )
{
    std::auto_ptr< ADN_Missions_Data::Mission > spNew( new Mission( xis.attribute< unsigned int >( "id" ) ) );
    spNew->ReadArchive( xis, contextLength );
    missions.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadContext
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadContext( xml::xistream& input, T_MissionParameter_Vector& context )
{
    input >> xml::start("context")
            >> xml::list( "parameter", *this, &ADN_Missions_Data::ReadContextParameter, context )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadContextParameter
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadContextParameter( xml::xistream& input, T_MissionParameter_Vector& context )
{
    std::auto_ptr< ADN_Missions_Data::MissionParameter > spNew( new MissionParameter() );
    spNew->ReadArchive( input );
    context.AddItem( spNew.release() );
}

namespace
{
    void WriteMissions( xml::xostream& output, const std::string& name, E_MissionType type, const ADN_Missions_Data::T_MissionParameter_Vector* context, const ADN_Missions_Data::T_Mission_ABC_Vector& missions )
    {
        output << xml::start( name );
        for( unsigned int i = 0; i < missions.size(); ++i )
            missions[i]->WriteArchive( output, type, context );
        output << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "missions" );
    ADN_Tools::AddSchema( output, "Missions" );
    WriteMissions( output, "units"      , eMissionType_Pawn,        &unitContext_,       missionsVector_[ eMissionType_Pawn ] );
    WriteMissions( output, "automats"   , eMissionType_Automat,     &automatContext_,    missionsVector_[ eMissionType_Automat ] );
    WriteMissions( output, "populations", eMissionType_Population,  &populationContext_, missionsVector_[ eMissionType_Population ] );
    WriteMissions( output, "fragorders" , eMissionType_FragOrder,   0,                   missionsVector_[ eMissionType_FragOrder ] );
    output << xml::end;
    WriteActivityArchive();
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::WriteActivityArchive
// Created: NPT 2013-04-18
// -----------------------------------------------------------------------------
void ADN_Missions_Data::WriteActivityArchive()
{
    T_StringList fileList;
    FilesNeeded( fileList );
    if( ! fileList.empty() && fileList.size() == 2 )
    {
        std::string strFile = ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory() + fileList.back();
        ADN_Tools::CreatePathToFile( strFile );
        {
            xml::xofstream output( strFile );
            activitiesData_->WriteArchive( output );
        }
        tools::WriteXmlCrc32Signature( strFile );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetFragOrders
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::T_Mission_ABC_Vector& ADN_Missions_Data::GetFragOrders()
{
    return missionsVector_[ eMissionType_FragOrder ];
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetUnitMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::T_Mission_ABC_Vector& ADN_Missions_Data::GetUnitMissions()
{
    return missionsVector_[ eMissionType_Pawn ];
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetAutomatMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::T_Mission_ABC_Vector& ADN_Missions_Data::GetAutomatMissions()
{
    return missionsVector_[ eMissionType_Automat ];
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetPopulationMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::T_Mission_ABC_Vector& ADN_Missions_Data::GetPopulationMissions()
{
    return missionsVector_[ eMissionType_Population ];
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FindFragOrder
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::FragOrder* ADN_Missions_Data::FindFragOrder( const std::string& strName )
{
    CIT_Mission_ABC_Vector it = std::find_if( missionsVector_[ eMissionType_FragOrder ].begin(), missionsVector_[ eMissionType_FragOrder ].end(), ADN_Tools::NameCmp< ADN_Missions_ABC >( strName ) );
    if( it == missionsVector_[ eMissionType_FragOrder ].end() )
        return 0;
    return static_cast< FragOrder* >( *it );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FindMission
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::Mission* ADN_Missions_Data::FindMission( ADN_Missions_Data::T_Mission_ABC_Vector& missions, const std::string& strName )
{
    CIT_Mission_ABC_Vector it = std::find_if( missions.begin(), missions.end(), ADN_Tools::NameCmp< ADN_Missions_ABC >( strName ) );
    if( it == missions.end() )
        return 0;
    return static_cast< Mission* >( *it );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionType::MissionType
// Created: LDC 2010-08-19
// -----------------------------------------------------------------------------
ADN_Missions_Data::MissionType::MissionType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionType
// Created: LDC 2010-08-19
// -----------------------------------------------------------------------------
ADN_Missions_Data::MissionType::MissionType( const std::string& name )
        : name_( name )
        , displayName_( qApp->translate( "ADN_Tr", name.c_str() ).toAscii().constData() )
        , isAllowed_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionType::~MissionType
// Created: LDC 2010-08-19
// -----------------------------------------------------------------------------
ADN_Missions_Data::MissionType::~MissionType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionType::GetItemName
// Created: LDC 2010-08-19
// -----------------------------------------------------------------------------
std::string ADN_Missions_Data::MissionType::GetItemName()
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetInputName
// Created: LDC 2013-07-01
// -----------------------------------------------------------------------------
const std::string& ADN_Missions_Data::MissionType::GetInputName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionType::CreateCopy
// Created: LDC 2010-08-19
// -----------------------------------------------------------------------------
ADN_Missions_Data::MissionType* ADN_Missions_Data::MissionType::CreateCopy()
{
    MissionType* newType = new MissionType();
    newType->name_ = name_;
    newType->displayName_ = displayName_;
    newType->isAllowed_ = isAllowed_.GetData();
    return newType;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionType::WriteArchive
// Created: LDC 2010-08-19
// -----------------------------------------------------------------------------
void ADN_Missions_Data::MissionType::WriteArchive( xml::xostream& output )
{
    if( isAllowed_.GetData() )
    {
        output << xml::start( "parameter" )
                 << xml::attribute( "type", name_ )
               << xml::end;
    }
}

#include "ADN_ConsistencyChecker.h"

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::CheckAndFixLoadingErrors
// Created: ABR 2013-04-23
// -----------------------------------------------------------------------------
void ADN_Missions_Data::CheckAndFixLoadingErrors() const
{
    std::vector< int > alreadyReportedIds;
    std::vector< ADN_Missions_ABC* > missionThatNeedANewId;

    // Check for duplicate id
    for( int type = 0; type < eNbrMissionTypes; ++type )
    {
        const ADN_Missions_Data::T_Mission_ABC_Vector& vector = missionsVector_[ type ];
        for( auto it = vector.begin(); it != vector.end(); ++it )
        {
            assert( *it != 0 );
            int id = ( *it )->id_.GetData();

            // If already reported, continue
            if( std::find( alreadyReportedIds.begin(), alreadyReportedIds.end(), id ) != alreadyReportedIds.end() )
                continue;

            // look for others with same id in each vector
            std::map< int, std::vector< ADN_Missions_ABC* > > missionsMap;
            size_t count = 0;
            for( int secondType = type; secondType < eNbrMissionTypes; ++secondType )
            {
                missionsMap[ secondType ] = missionsVector_[ secondType ].FindElements( boost::bind( &ADN_Tools::IdCompare< ADN_Missions_ABC >, _1, boost::cref( id ) ) );
                count += missionsMap[ secondType ].size();
            }

            assert( count != 0 );
            if( count > 1 ) // duplicate id, report it
            {
                alreadyReportedIds.push_back( id );
                ADN_ConsistencyChecker::ConsistencyError error( eInvalidIdInVector );
                for( auto itMap = missionsMap.begin(); itMap != missionsMap.end(); ++itMap )
                    for( auto itVector = itMap->second.begin(); itVector != itMap->second.end(); ++itVector )
                    {
                        error.items_.push_back( ADN_ConsistencyChecker::CreateGotoInfo( ( *itVector )->strName_.GetData(), eMissions, itMap->first ) );
                        if( error.items_.size() > 1 )
                            missionThatNeedANewId.push_back( *itVector );
                    }
                    ADN_ConsistencyChecker::AddLoadingError( error );
            }
        }
    }

    // Fix ids
    for( auto it = missionThatNeedANewId.begin(); it != missionThatNeedANewId.end(); ++it )
        ( *it )->id_ = ADN_Missions_Data::idManager_.GetNextId();
}
