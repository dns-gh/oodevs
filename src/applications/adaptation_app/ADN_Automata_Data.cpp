// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Automata_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_DataException.h"
#include "ADN_SaveFile_Exception.h"

#include "ADN_Tr.h"


// -----------------------------------------------------------------------------
// Name: UnitInfos::UnitInfos
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Automata_Data::UnitInfos::UnitInfos()
: ADN_Ref_ABC()
, ADN_DataTreeNode_ABC()
, ptrUnit_( ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsInfos(), 0 )
, min_( 0 )
, max_( -1 )
{
    BindExistenceTo( &ptrUnit_ );
}

// -----------------------------------------------------------------------------
// Name: UnitInfos::GetNodeName
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
std::string ADN_Automata_Data::UnitInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: UnitInfos::GetItemName
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
std::string ADN_Automata_Data::UnitInfos::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: UnitInfos::CreateCopy
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
ADN_Automata_Data::UnitInfos* ADN_Automata_Data::UnitInfos::CreateCopy()
{
    UnitInfos* pCopy = new UnitInfos();
    pCopy->ptrUnit_ = ptrUnit_.GetData();
    pCopy->min_     = min_.GetData();
    pCopy->max_     = max_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: UnitInfos::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::UnitInfos::ReadArchive( xml::xistream& input )
{
    std::string type;
    input >> xml::attribute( "type", type );
    ADN_Units_Data::UnitInfos* pUnit = ADN_Workspace::GetWorkspace().GetUnits().GetData().FindUnit( type );
    if( ! pUnit )
        throw ADN_DataException( tools::translate( "Automata_Data", "Invalid data" ).ascii(), tools::translate( "Automata_Data", "Automat - Invalid unit type '%1'" ).arg( type.c_str() ).ascii() );
    ptrUnit_ = pUnit;
    input >> xml::optional >> xml::attribute( "min-occurs", min_ )
          >> xml::optional >> xml::attribute( "max-occurs", max_ );
}

// -----------------------------------------------------------------------------
// Name: UnitInfos::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::UnitInfos::WriteArchive( xml::xostream& output, const ADN_TypePtr_InVector_ABC<ADN_Units_Data::UnitInfos>& pc )
{
    output << xml::start( "unit" )
             << xml::attribute( "type", ptrUnit_.GetData()->strName_ );
    if( min_.GetData() != 0 )
        output << xml::attribute( "min-occurs", min_ );
    if( max_.GetData() >= 0 )
        output << xml::attribute( "max-occurs", max_ );
    if( ptrUnit_ == pc )
        output << xml::attribute( "command-post", true );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: AutomatonInfos::AutomatonInfos
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Automata_Data::AutomatonInfos::AutomatonInfos()
: ADN_Ref_ABC()
, ADN_DataTreeNode_ABC()
, ptrUnit_( ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsInfos(), 0 )
, ptrModel_( ADN_Workspace::GetWorkspace().GetModels().GetData().GetAutomataModelsInfos(), 0 )
, bStrengthRatioFeedbackTime_( false )
, strengthRatioFeedbackTime_( "0s" )
{
    BindExistenceTo( &ptrUnit_ );
    BindExistenceTo( &ptrModel_ );
}

// -----------------------------------------------------------------------------
// Name: AutomatonInfos::~AutomatonInfos
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Automata_Data::AutomatonInfos::~AutomatonInfos()
{
    vSubUnits_.Reset();
}

// -----------------------------------------------------------------------------
// Name: AutomatonInfos::GetNodeName
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
std::string ADN_Automata_Data::AutomatonInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: AutomatonInfos::GetItemName
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
std::string ADN_Automata_Data::AutomatonInfos::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: AutomatonInfos::CreateCopy
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
ADN_Automata_Data::AutomatonInfos* ADN_Automata_Data::AutomatonInfos::CreateCopy()
{
    AutomatonInfos* pCopy = new AutomatonInfos();

    pCopy->nAgentType_ = nAgentType_.GetData();
    pCopy->ptrModel_ = ptrModel_.GetData();
    pCopy->ptrUnit_ = ptrUnit_.GetData();
    pCopy->bStrengthRatioFeedbackTime_ = bStrengthRatioFeedbackTime_.GetData();
    pCopy->strengthRatioFeedbackTime_ = strengthRatioFeedbackTime_.GetData();

    for( IT_UnitInfosVector it = vSubUnits_.begin(); it != vSubUnits_.end(); ++it )
        pCopy->vSubUnits_.AddItem( (*it)->CreateCopy() );

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::AutomatonInfos::ReadUnit
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Automata_Data::AutomatonInfos::ReadUnit( xml::xistream& input )
{
    std::auto_ptr<UnitInfos> spNew( new UnitInfos() );
    spNew->ReadArchive( input );
    bool cp = false;
    input >> xml::optional >> xml::attribute( "command-post", cp );
    if( cp )
        ptrUnit_ = spNew->ptrUnit_.GetData();
    else if( spNew->ptrUnit_.GetData()->eNatureLevel_.GetData() >= ptrUnit_.GetData()->eNatureLevel_.GetData() )
    {
        std::stringstream str;
        str << ptrUnit_.GetData()->strName_ << " <= " << spNew->ptrUnit_.GetData()->strName_ << " ( level )" ;
        MT_LOG_ERROR_MSG( str.str() );
    }
    vSubUnits_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: AutomatonInfos::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::AutomatonInfos::ReadArchive( xml::xistream& input )
{
    std::string strType, strModel;
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "type", strType )
          >> xml::attribute( "decisional-model", strModel )
          >> xml::optional >> xml::attribute( "force-ratio-feedback-time", strengthRatioFeedbackTime_ );
    bStrengthRatioFeedbackTime_ = strengthRatioFeedbackTime_ != "0s";
    nAgentType_ = ADN_Tr::ConvertToAgentTypeAutomate( strType );
    ADN_Models_Data::ModelInfos* pModel = ADN_Workspace::GetWorkspace().GetModels().GetData().FindAutomataModel( strModel );
    if( !pModel )
        throw ADN_DataException( tools::translate( "Automata_Data", "Invalid data" ).ascii(), tools::translate( "Automata_Data", "Automat - Invalid doctrine model '%1'" ).arg( strModel.c_str() ).ascii() );
    ptrModel_ = pModel;

    input >> xml::list( "unit", *this, &ADN_Automata_Data::AutomatonInfos::ReadUnit );
}

// -----------------------------------------------------------------------------
// Name: AutomatonInfos::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::AutomatonInfos::WriteArchive( xml::xostream& output, int nMosId )
{
    output << xml::start( "automat" )
            << xml::attribute( "name", strName_ )
            << xml::attribute( "type", ADN_Tr::ConvertFromAgentTypeAutomate( nAgentType_.GetData() ) )
            << xml::attribute( "decisional-model", ptrModel_.GetData()->strName_ )
            << xml::attribute( "id", nMosId );
    if( bStrengthRatioFeedbackTime_.GetData() )
        output << xml::attribute( "force-ratio-feedback-time", strengthRatioFeedbackTime_ );
    for( IT_UnitInfosVector it = vSubUnits_.begin(); it != vSubUnits_.end(); ++it )
        (*it)->WriteArchive( output, ptrUnit_ );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data constructor
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Automata_Data::ADN_Automata_Data()
: ADN_Data_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data destructor
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Automata_Data::~ADN_Automata_Data()
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::FilesNeeded
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szAutomata_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::Reset
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::Reset()
{
    vAutomata_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "automats" )
            >> xml::list( "automat", *this, &ADN_Automata_Data::ReadAutomat )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::ReadAutomat
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Automata_Data::ReadAutomat( xml::xistream& input )
{
    std::auto_ptr<AutomatonInfos> spNew( new AutomatonInfos() );
    spNew->ReadArchive( input );
    vAutomata_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::WriteArchive( xml::xostream& output )
{
    int nMosBaseId = static_cast< int >( ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsInfos().size() + 1 );
    output << xml::start( "automats" );
    ADN_Tools::AddSchema( output, "Automats" );
    int n = 0;
    for( IT_AutomatonInfosVector it = vAutomata_.begin(); it != vAutomata_.end(); ++it, ++n )
        (*it)->WriteArchive( output, nMosBaseId + n );
    output  << xml::end;
}


// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::GetAutomataThatUse
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
QStringList ADN_Automata_Data::GetAutomataThatUse( ADN_Units_Data::UnitInfos& unit )
{
    QStringList result;
    for( IT_AutomatonInfosVector it = vAutomata_.begin(); it != vAutomata_.end(); ++it )
    {
        AutomatonInfos* pAutomaton = *it;
        if( pAutomaton->ptrUnit_ == &unit )
            result << pAutomaton->strName_.GetData().c_str();

        for( IT_UnitInfosVector it2 = pAutomaton->vSubUnits_.begin(); it2 != pAutomaton->vSubUnits_.end(); ++it2 )
        {
            if( (*it2)->ptrUnit_.GetData() == &unit  )
            {
                result << pAutomaton->strName_.GetData().c_str();
                break;
            }
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::GetAutomataThatUse
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
QStringList ADN_Automata_Data::GetAutomataThatUse( ADN_Models_Data::ModelInfos& model )
{
    QStringList result;
    for( IT_AutomatonInfosVector it = vAutomata_.begin(); it != vAutomata_.end(); ++it )
    {
        AutomatonInfos* pAutomaton = *it;
        if( pAutomaton->ptrModel_.GetData() == &model )
            result << pAutomaton->strName_.GetData().c_str();
    }
    return result;
}
