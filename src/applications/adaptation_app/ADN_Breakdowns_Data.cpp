// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Breakdowns_Data.h"
#include "ADN_ConsistencyChecker.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "ADN_WorkspaceElement.h"
#include "clients_kernel/XmlTranslations.h"

tools::IdManager ADN_Breakdowns_Data::idManager_;

// -----------------------------------------------------------------------------
// Name: RepairPartInfo::RepairPartInfo
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::RepairPartInfo::RepairPartInfo()
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( eDotationFamily_Piece ).categories_, 0, true )
    , nNbr_( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::CreateCopy
// Created: APE 2005-04-07
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::RepairPartInfo* ADN_Breakdowns_Data::RepairPartInfo::CreateCopy()
{
    RepairPartInfo* pCopy = new RepairPartInfo();
    pCopy->SetCrossedElement( GetCrossedElement() );
    pCopy->nNbr_ = nNbr_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: RepairPartInfo::ReadArchive
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::RepairPartInfo::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "resource", *this )
          >> xml::attribute( "quantity", nNbr_ );
}

// -----------------------------------------------------------------------------
// Name: RepairPartInfo::WriteArchive
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::RepairPartInfo::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "part" )
             << xml::attribute( "resource", *this )
             << xml::attribute( "quantity", nNbr_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: BreakdownInfo::BreakdownInfo
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::BreakdownInfo::BreakdownInfo()
    : nId_( ADN_Breakdowns_Data::idManager_.GetNextId() )
    , repairTime_( "0s" )
    , repairTimeVariance_( "0s" )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eBreakdowns, "breakdowns" ) );
}

// -----------------------------------------------------------------------------
// Name: BreakdownInfo::BreakdownInfo
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::BreakdownInfo::BreakdownInfo( unsigned int id )
    : nId_( id )
    , repairTime_( "0s" )
    , repairTimeVariance_( "0s" )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eBreakdowns, "breakdowns" ) );
    ADN_Breakdowns_Data::idManager_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: BreakdownInfo::~BreakdownInfo
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::BreakdownInfo::~BreakdownInfo()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BreakdownInfo::CreateCopy
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::BreakdownInfo* ADN_Breakdowns_Data::BreakdownInfo::CreateCopy()
{
    BreakdownInfo* pCopy = new BreakdownInfo();
    pCopy->nType_ = nType_.GetData();
    pCopy->nNTI_  = nNTI_.GetData();
    pCopy->repairTime_ = repairTime_.GetData();
    pCopy->repairTimeVariance_ = repairTimeVariance_.GetData();
    for( auto it = vRepairParts_.begin(); it != vRepairParts_.end(); ++it )
        pCopy->vRepairParts_.AddItem( ( *it )->CreateCopy() );
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::BreakdownInfo::ReadPart
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::BreakdownInfo::ReadPart( xml::xistream& input )
{
    std::auto_ptr< RepairPartInfo > spNew( new RepairPartInfo() );
    spNew->ReadArchive( input );
    if( spNew->GetCrossedElement() )
        vRepairParts_.AddItem( spNew.release() );
    else
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, strName_.GetData(), eBreakdowns, -1, tools::translate( "ADN_Breakdowns_Data", "Required parts" ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: BreakdownInfo::ReadArchive
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::BreakdownInfo::ReadArchive( xml::xistream& input )
{
    std::string type;
    input >> xml::attribute( "name", *this )
          >> xml::attribute( "type", type )
          >> xml::attribute( "average-repairing-time", repairTime_ )
          >> xml::attribute( "variance", repairTimeVariance_ );
    nType_ = ENT_Tr::ConvertToBreakdownType( type );
    if( nType_ == E_BreakdownType( -1 ) )
        throw MASA_EXCEPTION( tools::translate( "Breakdown_Data", "Breakdowns - Invalid breakdown type '%1'" ).arg( type.c_str() ).toStdString() );
    input >> xml::list( "part", *this, &ADN_Breakdowns_Data::BreakdownInfo::ReadPart );
}

// -----------------------------------------------------------------------------
// Name: BreakdownInfo::WriteArchive
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::BreakdownInfo::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "breakdown" )
           << xml::attribute( "name", *this )
           << xml::attribute( "id", nId_ )
           << xml::attribute( "type", nType_.Convert() )
           << xml::attribute( "average-repairing-time", repairTime_ )
           << xml::attribute( "variance", repairTimeVariance_ );
    for( auto it = vRepairParts_.begin(); it != vRepairParts_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data constructor
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::ADN_Breakdowns_Data()
    : ADN_Data_ABC( eBreakdowns )
    , strAverageDiagnosticTime_( "0s" )
{
    vBreakdowns_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data destructor
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::~ADN_Breakdowns_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::FilesNeeded
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::FilesNeeded( tools::Path::T_Paths& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szBreakdowns_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::FindBreakdown
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::BreakdownInfo* ADN_Breakdowns_Data::FindBreakdown( const std::string& strName )
{
    auto it = std::find_if( vBreakdowns_.begin(), vBreakdowns_.end(), ADN_Tools::NameCmp( strName ) );
    if( it == vBreakdowns_.end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::GetBreakdowns
// Created: ABR 2012-07-30
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::T_BreakdownInfoVector& ADN_Breakdowns_Data::GetBreakdowns()
{
    return vBreakdowns_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::ReadArchive
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "breakdowns" )
            >> xml::start( "diagnosis" )
                >> xml::attribute( "time", strAverageDiagnosticTime_ )
            >> xml::end
            >> xml::list( "category", *this, &ADN_Breakdowns_Data::ReadCategory )
        >> xml::end;
    vBreakdowns_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::ReadCategory
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::ReadCategory( xml::xistream& input )
{
    E_BreakdownNTI nti = ENT_Tr::ConvertToBreakdownNTI( input.attribute< std::string >( "name" ) );
    input >> xml::list( "breakdown", *this, &ADN_Breakdowns_Data::ReadBreakdown, nti );
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::ReadBreakdown
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::ReadBreakdown( xml::xistream& input, const E_BreakdownNTI& nti )
{
    std::auto_ptr<BreakdownInfo> spNew( new BreakdownInfo( input.attribute< unsigned int >( "id" ) ) );
    spNew->ReadArchive( input );
    spNew->nNTI_ = nti;
    vBreakdowns_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::WriteArchive
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::WriteArchive( xml::xostream& output ) const
{
    if( vBreakdowns_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "breakdowns" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Breakdowns" );
    output   << xml::start( "diagnosis" )
               << xml::attribute( "time", strAverageDiagnosticTime_ )
             << xml::end;
    for( unsigned int i = 0; i < eNbrBreakdownNTI; ++i )
    {
        E_BreakdownNTI bd = static_cast< E_BreakdownNTI >( i );
        output << xml::start( "category" )
                 << xml::attribute( "name", ENT_Tr::ConvertFromBreakdownNTI( bd ) );
        for( auto it = vBreakdowns_.begin(); it != vBreakdowns_.end(); ++it )
            if( ( *it )->nNTI_ == bd )
                ( *it )->WriteArchive( output );
        output << xml::end;
    }
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::GetBreakdownsThatUse
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
QStringList ADN_Breakdowns_Data::GetBreakdownsThatUse( ADN_Resources_Data::CategoryInfo& part )
{
    QStringList result;
    for( auto it = vBreakdowns_.begin(); it != vBreakdowns_.end(); ++it )
        for( auto itPart = ( *it )->vRepairParts_.begin(); itPart != ( *it )->vRepairParts_.end(); ++itPart )
        {
            ADN_Resources_Data::CategoryInfo* info = ( *itPart )->GetCrossedElement();
            if( info && info->strName_ == part.strName_.GetData() )
                result << ( *it )->strName_.GetData().c_str();
        }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::CheckDatabaseValidity
// Created: ABR 2012-11-15
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    ADN_Data_ABC::CheckDatabaseValidity( checker );
    for( auto it = vBreakdowns_.begin(); it != vBreakdowns_.end(); ++it )
    {
        if( ( *it )->vRepairParts_.size() == 0 )
            checker.AddError( eMissingPart, ( *it )->strName_.GetData(), eBreakdowns );
        else
            for( auto itPart = ( *it )->vRepairParts_.begin(); itPart != ( *it )->vRepairParts_.end(); ++itPart )
            {
                bool missing = false;
                if( ! missing && ( *itPart )->nNbr_.GetData() == 0 )
                {
                    checker.AddError( eMissingPart, ( *it )->strName_.GetData(), eBreakdowns );
                    missing = true;
                }
                ( *itPart )->CheckValidity( checker, ( *it )->strName_.GetData(), eBreakdowns, -1, tools::translate( "ADN_Breakdowns_Data", "Required parts" ).toStdString() );
            }
    }
}
