// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_LogisticSupplyClasses_Data.h"
#include "ADN_Project_Data.h"

tools::IdManager ADN_LogisticSupplyClasses_Data::idManager_;

// -----------------------------------------------------------------------------
// Name: LogisticSupplyClass::LogisticSupplyClass
// Created: RPD 2010-10-29
// -----------------------------------------------------------------------------
ADN_LogisticSupplyClasses_Data::LogisticSupplyClass::LogisticSupplyClass()
    : nId_( ADN_LogisticSupplyClasses_Data::idManager_.GetNextId() )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyClass::LogisticSupplyClass
// Created: RPD 2010-10-29
// -----------------------------------------------------------------------------
ADN_LogisticSupplyClasses_Data::LogisticSupplyClass::LogisticSupplyClass( const std::string& name, int id )
    : ADN_RefWithName( name )
    , nId_( id )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_LogisticSupplyClass::CreateCopy
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
ADN_LogisticSupplyClasses_Data::LogisticSupplyClass* ADN_LogisticSupplyClasses_Data::LogisticSupplyClass::CreateCopy()
{
    LogisticSupplyClass* result = new LogisticSupplyClass();
    return result;
}


// -----------------------------------------------------------------------------
// Name: ADN_LogisticSupplyClasses_Data constructor
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
ADN_LogisticSupplyClasses_Data::ADN_LogisticSupplyClasses_Data()
    : ADN_Data_ABC( eCategories, eLogisticSupplyClasses )
{
    vLogisticSupplyClasses_.AddUniquenessChecker( eError, duplicateName_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_LogisticSupplyClasses_Data destructor
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
ADN_LogisticSupplyClasses_Data::~ADN_LogisticSupplyClasses_Data()
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_LogisticSupplyClasses_Data::FilesNeeded
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_LogisticSupplyClasses_Data::FilesNeeded( tools::Path::T_Paths& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szLogisticSupplyClasses_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_LogisticSupplyClasses_Data::Reset
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_LogisticSupplyClasses_Data::Reset()
{
    idManager_.Reset();
    vLogisticSupplyClasses_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_LogisticSupplyClasses_Data::ReadArchive
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_LogisticSupplyClasses_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "logistic-supply-classes" )
              >> xml::list( "logistic-supply-class", *this, &ADN_LogisticSupplyClasses_Data::ReadLogisticSupplyClass )
          >> xml::end;
    vLogisticSupplyClasses_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_LogisticSupplyClasses_Data::ReadLogisticSupplyClass
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_LogisticSupplyClasses_Data::ReadLogisticSupplyClass( xml::xistream& input )
{
    std::string strName;
    int id ( 0 );
    input >> xml::attribute( "type", strName );
    input >> xml::optional >> xml::attribute( "id", id );
    ADN_LogisticSupplyClasses_Data::T_LogisticSupplyClass_Vector::iterator found = std::find_if( vLogisticSupplyClasses_.begin(), vLogisticSupplyClasses_.end(), ADN_Tools::NameCmp( strName ) );
    if( found != vLogisticSupplyClasses_.end() )
        throw MASA_EXCEPTION( tools::translate( "Categories_Data", "Categories - Duplicated resource logistic category '%1'" ).arg( strName.c_str() ).toStdString() );
    if( !id )
        id = idManager_.GetNextId();
    ADN_LogisticSupplyClasses_Data::LogisticSupplyClass* pNew = new ADN_LogisticSupplyClasses_Data::LogisticSupplyClass( strName, id );
    vLogisticSupplyClasses_.AddItem( pNew );
    idManager_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: ADN_LogisticSupplyClasses_Data::WriteArchive
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_LogisticSupplyClasses_Data::WriteArchive( xml::xostream& output )
{
    if( vLogisticSupplyClasses_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "logistic-supply-classes" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "LogisticSupplyClasses" );
    for( ADN_LogisticSupplyClasses_Data::T_LogisticSupplyClass_Vector::const_iterator it = vLogisticSupplyClasses_.begin(); it != vLogisticSupplyClasses_.end(); ++it )
    {
        if( ( *it )->strName_.GetData().empty() )
            throw MASA_EXCEPTION( tools::translate( "Categories_Data","Categories - Invalid resource nature" ).toStdString() );
        std::string strData( ( *it )->strName_.GetData() );
        output << xml::start( "logistic-supply-class" )
            << xml::attribute( "type", trim( strData ) )
            << xml::attribute( "id", ( *it )->nId_ )
            << xml::end;
    }
    output << xml::end;
}
