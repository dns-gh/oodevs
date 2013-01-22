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
#include "ADN_Resources_Data.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"

tools::IdManager ADN_Breakdowns_Data::idManager_;

// -----------------------------------------------------------------------------
// Name: RepairPartInfo::RepairPartInfo
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::RepairPartInfo::RepairPartInfo()
    : nNbr_               ( 0 )
    , ptrPart_            ( ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( eDotationFamily_Piece ).categories_, 0 )
{
    this->BindExistenceTo( &ptrPart_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::CreateCopy
// Created: APE 2005-04-07
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::RepairPartInfo* ADN_Breakdowns_Data::RepairPartInfo::CreateCopy()
{
    RepairPartInfo* pCopy = new RepairPartInfo();
    pCopy->ptrPart_ = ptrPart_.GetData();
    pCopy->nNbr_ = nNbr_.GetData();

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: RepairPartInfo::ReadArchive
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::RepairPartInfo::ReadArchive( xml::xistream& input )
{
    std::string strCategoryName;
    input >> xml::attribute( "resource", strCategoryName )
          >> xml::attribute( "quantity", nNbr_ );
    ADN_Resources_Data::CategoryInfo* pCategory = ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( eDotationFamily_Piece ).FindCategory( strCategoryName );
    if( pCategory == 0 )
        throw MASA_EXCEPTION( tools::translate( "Breakdown_Data", "Breakdowns - Invalid part '%1'" ).arg( strCategoryName.c_str() ).toStdString() );
    ptrPart_ = pCategory;
}

// -----------------------------------------------------------------------------
// Name: RepairPartInfo::WriteArchive
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::RepairPartInfo::WriteArchive( xml::xostream& output )
{
    output << xml::start( "part" )
            << xml::attribute( "resource", ptrPart_.GetData()->strName_ )
            << xml::attribute( "quantity", nNbr_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: BreakdownInfo::BreakdownInfo
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::BreakdownInfo::BreakdownInfo()
    : nId_                 ( ADN_Breakdowns_Data::idManager_.GetNextId() )
    , repairTime_          ( "0s" )
    , repairTimeVariance_  ( "0s" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BreakdownInfo::BreakdownInfo
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::BreakdownInfo::BreakdownInfo( unsigned int id )
    : nId_                 ( id )
    , repairTime_          ( "0s" )
    , repairTimeVariance_  ( "0s" )
{
    ADN_Breakdowns_Data::idManager_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: BreakdownInfo::~BreakdownInfo
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::BreakdownInfo::~BreakdownInfo()
{
    clear_owned_ptrs( vRepairParts_ );
}

// -----------------------------------------------------------------------------
// Name: BreakdownInfo::CreateCopy
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::BreakdownInfo* ADN_Breakdowns_Data::BreakdownInfo::CreateCopy()
{
    BreakdownInfo* pCopy = new BreakdownInfo();
    pCopy->strName_ = tools::translate( "Breakdown_Data", "New breakdown" ).toStdString();
    pCopy->nType_ = nType_.GetData();
    pCopy->nNTI_  = nNTI_.GetData();
    pCopy->repairTime_ = repairTime_.GetData();
    pCopy->repairTimeVariance_ = repairTimeVariance_.GetData();

    for( IT_RepairPartInfoVector it = vRepairParts_.begin(); it != vRepairParts_.end(); ++it )
        pCopy->vRepairParts_.AddItem( (*it)->CreateCopy() );

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::BreakdownInfo::ReadPart
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::BreakdownInfo::ReadPart( xml::xistream& input )
{
    std::auto_ptr<RepairPartInfo> spNew( new RepairPartInfo() );
    spNew->ReadArchive( input );
    vRepairParts_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: BreakdownInfo::ReadArchive
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::BreakdownInfo::ReadArchive( xml::xistream& input )
{
    std::string type;
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "type", type )
          >> xml::attribute( "average-repairing-time", repairTime_ )
          >> xml::attribute( "variance", repairTimeVariance_ );
    nType_ = ADN_Tr::ConvertToBreakdownType( type );
    if( nType_ == E_BreakdownType( -1 ) )
        throw MASA_EXCEPTION( tools::translate( "Breakdown_Data", "Breakdowns - Invalid breakdown type '%1'" ).arg( type.c_str() ).toStdString() );
    input >> xml::list( "part", *this, &ADN_Breakdowns_Data::BreakdownInfo::ReadPart );
}

// -----------------------------------------------------------------------------
// Name: BreakdownInfo::WriteArchive
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::BreakdownInfo::WriteArchive( xml::xostream& output )
{
    output << xml::start( "breakdown" )
           << xml::attribute( "name", strName_ )
           << xml::attribute( "id", nId_ )
           << xml::attribute( "type", nType_.Convert() )
           << xml::attribute( "average-repairing-time", repairTime_ )
           << xml::attribute( "variance", repairTimeVariance_ );
    for( IT_RepairPartInfoVector it = vRepairParts_.begin(); it != vRepairParts_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data constructor
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::ADN_Breakdowns_Data()
    : ADN_Data_ABC             ( eBreakdowns )
    , strAverageDiagnosticTime_( "0s" )
{
    vBreakdowns_.AddUniquenessChecker( eError, duplicateName_ );
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
void ADN_Breakdowns_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szBreakdowns_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::Reset
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::Reset()
{
    idManager_.Reset();
    vBreakdowns_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::FindBreakdown
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::BreakdownInfo* ADN_Breakdowns_Data::FindBreakdown( const std::string& strName )
{
    IT_BreakdownInfoVector it = std::find_if( vBreakdowns_.begin(), vBreakdowns_.end(), ADN_Tools::NameCmp<BreakdownInfo>( strName ) );
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
    std::string strNti;
    input >> xml::attribute( "name", strNti );
    E_BreakdownNTI nti = ADN_Tr::ConvertToBreakdownNTI( strNti );
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
void ADN_Breakdowns_Data::WriteArchive( xml::xostream& output )
{
    if( vBreakdowns_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "breakdowns" );
    ADN_Tools::AddSchema( output, "Breakdowns" );
    output   << xml::start( "diagnosis" )
               << xml::attribute( "time", strAverageDiagnosticTime_ )
             << xml::end;
    for( unsigned int i = 0; i < eNbrBreakdownNTI; ++i )
    {
        output << xml::start( "category" )
                 << xml::attribute( "name", ADN_Tr::ConvertFromBreakdownNTI( ( E_BreakdownNTI )i ) );
        for( IT_BreakdownInfoVector it = vBreakdowns_.begin(); it != vBreakdowns_.end(); ++it )
            if( (*it)->nNTI_ == ( E_BreakdownNTI )i )
                (*it)->WriteArchive( output );
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
    for( IT_BreakdownInfoVector it = vBreakdowns_.begin(); it != vBreakdowns_.end(); ++it )
        for( IT_RepairPartInfoVector itPart = ( *it )->vRepairParts_.begin(); itPart != ( *it )->vRepairParts_.end(); ++itPart )
            if( ( *itPart )->ptrPart_.GetData()->strName_ == part.strName_.GetData() )
                result << ( *it )->strName_.GetData().c_str();
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::CheckDatabaseValidity
// Created: ABR 2012-11-15
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    for( ADN_Breakdowns_Data::CIT_BreakdownInfoVector it = vBreakdowns_.begin(); it != vBreakdowns_.end(); ++it )
    {
        if( ( *it )->vRepairParts_.size() == 0 )
            checker.AddError( eMissingPart, ( *it )->strName_.GetData(), eBreakdowns );
        else
            for( auto itPart = ( *it )->vRepairParts_.begin(); itPart != ( *it )->vRepairParts_.end(); ++itPart )
            {
                if( ( *itPart )->nNbr_.GetData() == 0 )
                {
                    checker.AddError( eMissingPart, ( *it )->strName_.GetData(), eBreakdowns );
                    break;
                }
            }
    }
}
