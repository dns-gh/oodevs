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
#include "clients_kernel/XmlTranslations.h"

tools::IdManager ADN_LogisticSupplyClasses_Data::idManager_;

// -----------------------------------------------------------------------------
// Name: LogisticSupplyClass::LogisticSupplyClass
// Created: RPD 2010-10-29
// -----------------------------------------------------------------------------
ADN_LogisticSupplyClasses_Data::LogisticSupplyClass::LogisticSupplyClass()
    : nId_( ADN_LogisticSupplyClasses_Data::idManager_.GetNextId() )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eCategories, eLogisticSupplyClasses, "logistic-supply-classes" ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyClass::LogisticSupplyClass
// Created: RPD 2010-10-29
// -----------------------------------------------------------------------------
ADN_LogisticSupplyClasses_Data::LogisticSupplyClass::LogisticSupplyClass( int id )
    : nId_( id )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eCategories, eLogisticSupplyClasses, "logistic-supply-classes" ) );
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
    vLogisticSupplyClasses_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
}

// -----------------------------------------------------------------------------
// Name: ADN_LogisticSupplyClasses_Data destructor
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
ADN_LogisticSupplyClasses_Data::~ADN_LogisticSupplyClasses_Data()
{
    // NOTHING
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
    int id ( 0 );
    input >> xml::optional >> xml::attribute( "id", id );
    if( !id )
        id = idManager_.GetNextId();
    ADN_LogisticSupplyClasses_Data::LogisticSupplyClass* pNew = new ADN_LogisticSupplyClasses_Data::LogisticSupplyClass( id );
    input >> xml::attribute( "type", *pNew);
    vLogisticSupplyClasses_.AddItem( pNew );
    idManager_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: ADN_LogisticSupplyClasses_Data::WriteArchive
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_LogisticSupplyClasses_Data::WriteArchive( xml::xostream& output ) const
{
    if( vLogisticSupplyClasses_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "logistic-supply-classes" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "LogisticSupplyClasses" );
    for( auto it = vLogisticSupplyClasses_.begin(); it != vLogisticSupplyClasses_.end(); ++it )
    {
        output << xml::start( "logistic-supply-class" )
                 << xml::attribute( "type", **it )
                 << xml::attribute( "id", ( *it )->nId_ )
               << xml::end;
    }
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_LogisticSupplyClasses_Data::CheckDatabaseValidity
// Created: LDC 2014-03-21
// -----------------------------------------------------------------------------
void ADN_LogisticSupplyClasses_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    for( auto it = vLogisticSupplyClasses_.begin(); it != vLogisticSupplyClasses_.end(); ++it )
        if( ( *it )->strName_.GetData().empty() )
            checker.AddError( eInvalidSupplyClass, std::string(), eCategories );
}
