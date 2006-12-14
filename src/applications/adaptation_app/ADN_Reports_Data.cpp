// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Reports_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_Xml_Exception.h"
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::ReportParameterValue
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
ADN_Reports_Data::ReportParameterValue::ReportParameterValue()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::~ReportParameterValue
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
ADN_Reports_Data::ReportParameterValue::~ReportParameterValue()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::GetItemName
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
std::string ADN_Reports_Data::ReportParameterValue::GetItemName()
{
    return name_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::CreateCopy
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
ADN_Reports_Data::ReportParameterValue* ADN_Reports_Data::ReportParameterValue::CreateCopy()
{
    ReportParameterValue* newValue = new ReportParameterValue();
    newValue->name_ = name_.GetData();
    return newValue;
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::ReadArchive
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
void ADN_Reports_Data::ReportParameterValue::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "value" );
    input.ReadAttribute( "name", name_ );
    input.EndSection();
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::WriteArchive
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
void ADN_Reports_Data::ReportParameterValue::WriteArchive( MT_OutputArchive_ABC& output, unsigned int id )
{
    output.Section( "value" );
    output.WriteAttribute( "id", id );
    output.WriteAttribute( "name", name_.GetData() );
    output.EndSection();
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::ReportParameter
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
ADN_Reports_Data::ReportParameter::ReportParameter()
{
    ADN_Type_Enum< E_MissionParameterType, eNbrMissionParameterType >::SetConverter( &ADN_Tr::ConvertFromMissionParameterType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::~ReportParameter
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
ADN_Reports_Data::ReportParameter::~ReportParameter()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::GetItemName
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
std::string ADN_Reports_Data::ReportParameter::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::CreateCopy
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
ADN_Reports_Data::ReportParameter* ADN_Reports_Data::ReportParameter::CreateCopy()
{
    ReportParameter* newParam = new ReportParameter();
    newParam->strName_ = strName_.GetData();
    newParam->type_    = type_.GetData();
    newParam->values_.reserve( values_.size() );
    for( IT_ReportParameterValue_Vector it = values_.begin(); it != values_.end(); ++it )
    {
        ReportParameterValue* newParamValue = (*it)->CreateCopy();
        newParam->values_.AddItem( newParamValue );
    }
    return newParam;
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::ReadArchive
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
void ADN_Reports_Data::ReportParameter::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.BeginList( "parameter" );
    std::string type;
    input.ReadAttribute( "type", type );
    type_ = ADN_Tr::ConvertToMissionParameterType( type );
    while( input.NextListElement() )
    {
        std::auto_ptr< ReportParameterValue > spNew( new ReportParameterValue() );
        spNew->ReadArchive( input );
        values_.AddItem( spNew.release() );
    }
    input.EndList();
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::WriteArchive
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
void ADN_Reports_Data::ReportParameter::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "parameter" );
    output.WriteAttribute( "type", ADN_Tr::ConvertFromMissionParameterType( type_.GetData() ) );
    for( unsigned long i = 0; i < values_.size(); ++i )
        values_[i]->WriteArchive( output, i );
    output.EndSection();
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::ReportInfo
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
ADN_Reports_Data::ReportInfo::ReportInfo()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::~ReportInfo
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
ADN_Reports_Data::ReportInfo::~ReportInfo()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::GetItemName
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
std::string ADN_Reports_Data::ReportInfo::GetItemName()
{
    return "";
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::CreateCopy
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
ADN_Reports_Data::ReportInfo* ADN_Reports_Data::ReportInfo::CreateCopy()
{
    ReportInfo* newReport = new ReportInfo();
    newReport->message_ = message_.GetData();
    newReport->parameters_.reserve( parameters_.size() );
    for( IT_ReportParameter_Vector it = parameters_.begin(); it != parameters_.end(); ++it )
    {
        ReportParameter* newParam = (*it)->CreateCopy();
        newReport->parameters_.AddItem( newParam );
    }
    return newReport;
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::ReadArchive
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
void ADN_Reports_Data::ReportInfo::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.BeginList( "report" );
    input.ReadAttribute( "message", message_ );
    while( input.NextListElement() )
    {
        std::auto_ptr< ReportParameter > spNew( new ReportParameter() );
        spNew->ReadArchive( input );
        parameters_.AddItem( spNew.release() );
    }
    input.EndList();
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::WriteArchive
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
void ADN_Reports_Data::ReportInfo::WriteArchive( MT_OutputArchive_ABC& output, unsigned long id )
{
    output.Section( "report" );
    output.WriteAttribute( "id", id );
    output.WriteAttribute( "message", message_.GetData() );
    for( unsigned long i = 0; i < parameters_.size(); ++i )
        parameters_[i]->WriteArchive( output );
    output.EndSection();
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data constructor
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
ADN_Reports_Data::ADN_Reports_Data()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data destructor
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
ADN_Reports_Data::~ADN_Reports_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::FilesNeeded
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
void ADN_Reports_Data::FilesNeeded( T_StringList& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szReports_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::Reset
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
void ADN_Reports_Data::Reset()
{
    reports_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::ReadArchive
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
void ADN_Reports_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.BeginList( "reports" );
    while( input.NextListElement() )
    {
        std::auto_ptr< ReportInfo > spNew( new ReportInfo() );
        spNew->ReadArchive( input );
        reports_.AddItem( spNew.release() );
    }
    input.EndList();
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::WriteArchive
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
void ADN_Reports_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "reports" );
    for( unsigned long i = 0; i < reports_.size(); ++i )
        reports_[i]->WriteArchive( output, i );
    output.EndSection();
}
