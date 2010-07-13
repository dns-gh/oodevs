// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Reports_Data.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "ADN_Workspace.h"
#include <xeumeuleu/xml.hpp>

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
void ADN_Reports_Data::ReportParameterValue::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", name_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::WriteArchive
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
void ADN_Reports_Data::ReportParameterValue::WriteArchive( xml::xostream& output, unsigned int id )
{
    output << xml::start( "value" )
            << xml::attribute( "id", id )
            << xml::attribute( "name", name_ )
           << xml::end();
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
void ADN_Reports_Data::ReportParameter::ReadArchive( xml::xistream& input )
{
    std::string type;
    input >> xml::attribute( "type", type );
    type_ = ADN_Tr::ConvertToMissionParameterType( type );
    input >> xml::list( "value", *this, &ADN_Reports_Data::ReportParameter::ReadParameterValue );
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::ReportParameter::ReadParameterValue
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Reports_Data::ReportParameter::ReadParameterValue( xml::xistream& input )
{
    std::auto_ptr< ReportParameterValue > spNew( new ReportParameterValue() );
    spNew->ReadArchive( input );
    values_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::WriteArchive
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
void ADN_Reports_Data::ReportParameter::WriteArchive( xml::xostream& output )
{
    output << xml::start( "parameter" )
           << xml::attribute( "type", ADN_Tr::ConvertFromMissionParameterType( type_.GetData()  ) );
    for( unsigned long i = 0; i < values_.size(); ++i )
        values_[i]->WriteArchive( output, i );
    output << xml::end();
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
void ADN_Reports_Data::ReportInfo::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "id", id_ )
          >> xml::attribute( "message", message_ )
          >> xml::list( "parameter", *this, &ADN_Reports_Data::ReportInfo::ReadParameter );
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::ReportInfo::ReadParameter
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Reports_Data::ReportInfo::ReadParameter( xml::xistream& input )
{
    std::auto_ptr< ReportParameter > spNew( new ReportParameter() );
    spNew->ReadArchive( input );
    parameters_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::WriteArchive
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
void ADN_Reports_Data::ReportInfo::WriteArchive( xml::xostream& output )
{
    output << xml::start( "report" )
            << xml::attribute( "id", id_ )
            << xml::attribute( "message",  message_ );
    for( unsigned long i = 0; i < parameters_.size(); ++i )
        parameters_[i]->WriteArchive( output );
    output << xml::end();
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
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szReports_.GetData() );
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
void ADN_Reports_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "reports" )
            >> xml::list( "report", *this, &ADN_Reports_Data::ReadReport )
          >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::ReadReport
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Reports_Data::ReadReport( xml::xistream& input )
{
    std::auto_ptr< ReportInfo > spNew( new ReportInfo() );
    spNew->ReadArchive( input );
    reports_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_Data::WriteArchive
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
void ADN_Reports_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "reports" );
    ADN_Tools::AddSchema( output, "Reports" );
    for( unsigned long i = 0; i < reports_.size(); ++i )
        reports_[i]->WriteArchive( output );
    output << xml::end();
}
