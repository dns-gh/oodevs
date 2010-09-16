// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Missions_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_AiEngine_Data.h"
#include "ADN_Tr.h"

IdentifierFactory ADN_Missions_Data::idFactory_;

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
    , min_( 1 )
    , max_( 1 )
{
    ADN_Type_Enum< E_MissionParameterType, eNbrMissionParameterType >::SetConverter( &ADN_Tr::ConvertFromMissionParameterType );
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
    newParam->isOptional_ = isOptional_.GetData();
    newParam->min_        = min_.GetData();
    newParam->max_        = max_.GetData();
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
    return newParam;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameter::ReadArchive
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
void ADN_Missions_Data::MissionParameter::ReadArchive( xml::xistream& input )
{
    std::string type;
    std::string max;
    input >> xml::attribute( "name", strName_ )
            >> xml::attribute( "type", type )
            >> xml::optional >> xml::attribute( "optional", isOptional_ )
            >> xml::attribute( "dia-name", diaName_ )
            >> xml::optional >> xml::attribute( "min-occurs", min_ )
            >> xml::optional >> xml::attribute( "max-occurs", max );
    if( max == "unbounded" )
        max_ = std::numeric_limits< int >::max();
    else
        input >> xml::optional >> xml::attribute( "max-occurs", max_ );
    type_ = ADN_Tr::ConvertToMissionParameterType( type );
    input >> xml::list( "value", *this, &ADN_Missions_Data::MissionParameter::ReadValue );
    input >> xml::optional() >> xml::start( "choice" )
            >> xml::list( "parameter", *this, &ADN_Missions_Data::MissionParameter::ReadChoice )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::MissionParameter::FillChoices
// Created: LDC 2010-08-19
// -----------------------------------------------------------------------------
void ADN_Missions_Data::MissionParameter::FillChoices()
{
    choices_.AddItem( new MissionType( "PointBM" ) );
    choices_.AddItem( new MissionType( "PathBM" ) );
    choices_.AddItem( new MissionType( "AreaBM" ) );
    choices_.AddItem( new MissionType( "AutomatBM" ) );
    choices_.AddItem( new MissionType( "AgentBM" ) );
    choices_.AddItem( new MissionType( "AgentKnowledgeBM" ) );
    choices_.AddItem( new MissionType( "ObjectKnowledgeBM" ) );
    choices_.AddItem( new MissionType( "PopulationKnowledgeBM" ) );
    choices_.AddItem( new MissionType( "UrbanBlockBM" ) );
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
void ADN_Missions_Data::MissionParameter::ReadChoice( xml::xistream& input )
{
    std::string name;
    input >> xml::attribute( "type", name );
    unsigned int nChoices = choices_.size();
    for( unsigned int i = 0; i < nChoices; ++i )
    {
        if( choices_[i]->name_ == name )
        {
            choices_[i]->isAllowed_ = true;
            return;
        }
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
void ADN_Missions_Data::MissionParameter::WriteArchive( xml::xostream& output )
{
    std::string diaName = diaName_.GetData();
    if( diaName.empty() )
        diaName = GetFussedDiaName( strName_.GetData().c_str() ).ascii();

    output << xml::start( "parameter" )
            << xml::attribute( "name", strName_ )
            << xml::attribute( "type", ADN_Tr::ConvertFromMissionParameterType( type_.GetData() ) )
            << xml::attribute( "optional", isOptional_ )
            << xml::attribute( "dia-name", diaName );
    if( max_ != 1 )
    {
        output << xml::attribute( "min-occurs", min_ );
        if( max_ == std::numeric_limits< int >::max() )
            output << xml::attribute( "max-occurs", "unbounded" );
        else
            output << xml::attribute( "max-occurs", max_ );
    }
    for( unsigned int i = 0; i < values_.size(); ++i )
        values_[i]->WriteArchive( output, i );
    unsigned int nChoices = choices_.size();
    bool hasChoice = false;
    for( unsigned int i = 0; i < nChoices && !hasChoice; ++i )
        hasChoice = choices_[i]->isAllowed_.GetData();
    if( hasChoice )
    {
        output << xml::start( "choice" );
        for( unsigned int i = 0; i < nChoices; ++i )
            choices_[i]->WriteArchive( output );
        output << xml::end;
    }
    output << xml::end;
}

// =============================================================================
// Missions
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Mission::Mission
// Created: SBO 2009-11-16
// -----------------------------------------------------------------------------
ADN_Missions_Data::Mission::Mission( bool isAutomat /*= false*/ )
    : id_( ADN_Missions_Data::idFactory_.Create() )
    , isAutomat_( isAutomat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Mission::Mission
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::Mission::Mission( unsigned int id, bool isAutomat /*= false*/ )
    : id_( id )
    , isAutomat_( isAutomat )
{
    ADN_Missions_Data::idFactory_.Reserve( id );
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
    Mission* newMission         = new Mission( ADN_Missions_Data::idFactory_.Create(), isAutomat_.GetData() );
    newMission->strName_        = strName_.GetData();
    newMission->diaType_        = diaType_.GetData();
    newMission->diaBehavior_    = diaBehavior_.GetData();
    newMission->cdtDiaBehavior_ = cdtDiaBehavior_.GetData();
    newMission->mrtDiaBehavior_ = mrtDiaBehavior_.GetData();
    newMission->doctrineDescription_ = doctrineDescription_.GetData();
    newMission->usageDescription_    = usageDescription_.GetData();
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
void ADN_Missions_Data::Mission::ReadArchive( xml::xistream& input )
{
    std::string doctrineDesc, usageDesc;
    input >> xml::attribute( "name", strName_ )
        >> xml::attribute( "dia-type", diaType_ )
        >> xml::optional >> xml::attribute( "dia-behavior", diaBehavior_ )
        >> xml::optional >> xml::attribute( "cdt-dia-behavior", cdtDiaBehavior_ )
        >> xml::optional >> xml::attribute( "mrt-dia-behavior", mrtDiaBehavior_ )
        >> xml::optional >> xml::start( "descriptions" )
            >> xml::optional >> xml::start( "doctrine" ) >> doctrineDesc >> xml::end
            >> xml::optional >> xml::start( "usage" ) >> usageDesc >> xml::end
        >> xml::end
        >> xml::list( "parameter", *this, &ADN_Missions_Data::Mission::ReadParameter );
    doctrineDescription_ = doctrineDesc;
    usageDescription_ = usageDesc;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Mission::ReadParameter
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Missions_Data::Mission::ReadParameter( xml::xistream& input )
{
    std::auto_ptr< MissionParameter > spNew( new MissionParameter() );
    spNew->ReadArchive( input );
    parameters_.AddItem( spNew.release() );
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
void ADN_Missions_Data::Mission::WriteArchive( xml::xostream& output, const std::string& type )
{
    output << xml::start( "mission" );
    const QString typeName = type == "units" ? "Pion" : (type == "automats" ? "Automate" : "Population");
    const QString diaName  = BuildDiaMissionType( strName_.GetData().c_str() );
    if( diaType_.GetData().empty() )
        diaType_ = QString( "T_Mission_%1_%2" ).arg( typeName ).arg( diaName ).ascii();

    output << xml::attribute( "name", strName_ )
           << xml::attribute( "dia-type", diaType_ )
           << xml::attribute( "id", id_ );

    if( !isAutomat_.GetData() )
    {
        if( diaBehavior_.GetData().empty() )
            diaBehavior_ = QString( "MIS_%1_%2" ).arg( typeName ).arg( diaName ).ascii();
        output << xml::attribute( "dia-behavior", diaBehavior_ );
    }
    else
    {
        if( cdtDiaBehavior_.GetData().empty() )
            cdtDiaBehavior_ = QString( "MIS_%1_CDT_%2" ).arg( typeName ).arg( diaName ).ascii();
        if( mrtDiaBehavior_.GetData().empty() )
            mrtDiaBehavior_ = QString( "MIS_%1_MRT_%2" ).arg( typeName ).arg( diaName ).ascii();
        output << xml::attribute( "mrt-dia-behavior", mrtDiaBehavior_ )
               << xml::attribute( "cdt-dia-behavior", cdtDiaBehavior_ );
    }
    if( ! doctrineDescription_.GetData().empty() || ! usageDescription_.GetData().empty() )
    {
        output << xml::start( "descriptions" );
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
// Frag orders
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FragOrder::FragOrder
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::FragOrder::FragOrder()
    : id_( ADN_Missions_Data::idFactory_.Create() )
    , isAvailableWithoutMission_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FragOrder::FragOrder
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Missions_Data::FragOrder::FragOrder( unsigned int id )
    : id_( id )
    , isAvailableWithoutMission_( false )
{
    ADN_Missions_Data::idFactory_.Reserve( id );
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
    FragOrder* newFragOrder = new FragOrder( ADN_Missions_Data::idFactory_.Create() );
    newFragOrder->strName_ = strName_.GetData();
    newFragOrder->diaType_ = diaType_.GetData();
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
void ADN_Missions_Data::FragOrder::ReadArchive( xml::xistream& input )
{
    std::string doctrineDesc, usageDesc;
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "dia-type", diaType_ )
          >> xml::optional >> xml::attribute( "available-without-mission", isAvailableWithoutMission_ )
          >> xml::optional >> xml::start( "descriptions" )
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
void ADN_Missions_Data::FragOrder::WriteArchive( xml::xostream& output )
{
    if( diaType_.GetData().empty() )
        diaType_ = BuildDiaFragOrderType( strName_.GetData().c_str() ).ascii();

    output << xml::start( "fragorder" )
            << xml::attribute( "name", strName_ )
            << xml::attribute( "dia-type", diaType_ )
            << xml::attribute( "id", id_ )
            << xml::attribute( "available-without-mission", isAvailableWithoutMission_ );
    if( ! doctrineDescription_.GetData().empty() || ! usageDescription_.GetData().empty() )
    {
        output << xml::start( "descriptions" );
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
    idFactory_.Reset();
    unitMissions_.Reset();
    automatMissions_.Reset();
    populationMissions_.Reset();
    fragOrders_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "missions" )
            >> xml::start( "units" )
                >> xml::list( "context", *this, &ADN_Missions_Data::ReadContext, unitContext_ )
                >> xml::list( "mission", *this, &ADN_Missions_Data::ReadMission, unitMissions_, (const bool&)false )
            >> xml::end
            >> xml::start( "automats" )
                >> xml::list( "context", *this, &ADN_Missions_Data::ReadContext, automatContext_ )
                >> xml::list( "mission", *this, &ADN_Missions_Data::ReadMission, automatMissions_, (const bool&)true )
            >> xml::end
            >> xml::start( "populations" )
                >> xml::list( "context", *this, &ADN_Missions_Data::ReadContext, populationContext_ )
                >> xml::list( "mission", *this, &ADN_Missions_Data::ReadMission, populationMissions_, (const bool&)false )
            >> xml::end
            >> xml::start( "fragorders" )
                >> xml::list( "fragorder", *this, &ADN_Missions_Data::ReadFragOrder )
            >> xml::end
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadFragOrder
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadFragOrder( xml::xistream& xis )
{
    std::auto_ptr< FragOrder > spNew( new FragOrder( xis.attribute< unsigned int >( "id" ) ) );
    spNew->ReadArchive( xis );
    fragOrders_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadMission
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadMission( xml::xistream& xis, T_Mission_Vector& missions, const bool& isAutomat )
{
    std::auto_ptr< ADN_Missions_Data::Mission > spNew( new Mission( xis.attribute< unsigned int >( "id" ), isAutomat ) );
    spNew->ReadArchive( xis );
    missions.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadContext
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadContext( xml::xistream& input, T_MissionParameter_Vector& context )
{
    input >> xml::list( "parameter", *this, &ADN_Missions_Data::ReadContextParameter, context );
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
    void WriteMissions( xml::xostream& output, const std::string& name, const ADN_Missions_Data::T_MissionParameter_Vector& context, const ADN_Missions_Data::T_Mission_Vector& missions )
    {
        output << xml::start( name );
        if( ! context.empty() )
        {
            output << xml::start( "context" );
            for( unsigned i = 0; i < context.size(); ++i )
                context[i]->WriteArchive( output );
            output << xml::end;
        }
        for( unsigned int i = 0; i < missions.size(); ++i )
            missions[i]->WriteArchive( output, name );
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
    WriteMissions( output, "units"      , unitContext_,       unitMissions_ );
    WriteMissions( output, "automats"   , automatContext_,    automatMissions_ );
    WriteMissions( output, "populations", populationContext_, populationMissions_ );

    output << xml::start( "fragorders" );
    for( unsigned int i = 0; i < fragOrders_.size(); ++i )
        fragOrders_[i]->WriteArchive( output );
    output << xml::end
        << xml::end;
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
{
    name_ = name;
    displayName_ = qApp->translate( "ADN_Tr", name.c_str() ).ascii();
    isAllowed_ = false;
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
