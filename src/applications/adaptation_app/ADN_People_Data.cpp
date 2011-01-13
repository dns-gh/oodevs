// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_People_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_DataException.h"
#include "ADN_Tr.h"

// =============================================================================
// ADN_People_Data::PeopleInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PeopleInfos::PeopleInfos
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_People_Data::PeopleInfos::PeopleInfos()
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
    , ptrModel_( ADN_Workspace::GetWorkspace().GetPopulation().GetData().GetPopulation(), 0 )
    , male_    ( 0 )
    , female_  ( 0 )
    , children_( 0 )
{
    this->BindExistenceTo( &ptrModel_ );
}

// -----------------------------------------------------------------------------
// Name: PeopleInfos::~PeopleInfos
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_People_Data::PeopleInfos::~PeopleInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PeopleInfos::GetNodeName
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
std::string ADN_People_Data::PeopleInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: PeopleInfos::GetItemName
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
std::string ADN_People_Data::PeopleInfos::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: PeopleInfos::CreateCopy
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
ADN_People_Data::PeopleInfos* ADN_People_Data::PeopleInfos::CreateCopy()
{
    PeopleInfos* pCopy = new PeopleInfos();
    pCopy->ptrModel_ = ptrModel_.GetData();
    pCopy->male_ = male_.GetData();
    pCopy->female_ = female_.GetData();
    pCopy->children_ = children_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: PeopleInfos::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_People_Data::PeopleInfos::ReadArchive( xml::xistream& input )
{
    std::string strModel;
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "associated-crowd", strModel );

    ADN_Population_Data::PopulationInfos* pModel = ADN_Workspace::GetWorkspace().GetPopulation().GetData().FindPopulation( strModel );
    if( !pModel )
        throw ADN_DataException( "Invalid data", tools::translate( "Population_Data", "Population types - Invalid Population type '%1'" ).arg( strModel.c_str() ).ascii() );
    ptrModel_ = pModel;

    input >> xml::start( "repartition" )
              >> xml::attribute( "male", male_ )
              >> xml::attribute( "female", female_ )
              >> xml::attribute( "children", children_ )
          >> xml::end;
    input >> xml::start( "schedule" )
              >> xml::attribute( "transfer-time", transfertTime_ )
          >> xml::end
          >> xml::start( "needs" )
          >> xml::end;
}
// -----------------------------------------------------------------------------
// Name: PeopleInfos::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_People_Data::PeopleInfos::WriteArchive( xml::xostream& output, int mosId )
{
    output << xml::start( "population" )
               << xml::attribute( "name", strName_ )
               << xml::attribute( "id", mosId )
               << xml::attribute( "associated-crowd", ptrModel_.GetData()->strName_ )
               << xml::start( "repartition" )
                   << xml::attribute( "male", male_ )
                   << xml::attribute( "female", female_ )
                   << xml::attribute( "children", children_ )
               << xml::end
               << xml::start( "schedule" )
                   << xml::attribute( "transfer-time", transfertTime_ )
               << xml::end
               << xml::start( "needs" )
               << xml::end
    << xml::end;
}

// =============================================================================
// ADN_People_Data
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_People_Data constructor
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
ADN_People_Data::ADN_People_Data()
    : ADN_Data_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data destructor
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
ADN_People_Data::~ADN_People_Data()
{
    this->Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data::Reset
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
void ADN_People_Data::Reset()
{
    vPeople_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data::FilesNeeded
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void ADN_People_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szPeople_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data::ReadArchive
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
void ADN_People_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "populations" )
              >> xml::list( "population", *this, &ADN_People_Data::ReadPeople )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data::ReadPeople
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void ADN_People_Data::ReadPeople( xml::xistream& input )
{
    std::auto_ptr<PeopleInfos> spNew( new PeopleInfos() );
    spNew->ReadArchive( input );
    vPeople_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data::WriteArchive
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
void ADN_People_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "populations" );
    ADN_Tools::AddSchema( output, "Inhabitants" );
    int n = 0;
    for( IT_PeopleInfosVector it = vPeople_.begin(); it != vPeople_.end(); ++it, ++n )
        (*it)->WriteArchive( output, n );
    output << xml::end;
}