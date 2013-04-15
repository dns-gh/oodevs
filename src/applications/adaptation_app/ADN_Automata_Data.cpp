// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Automata_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_ConsistencyChecker.h"
#include "ADN_Tr.h"

tools::IdManager ADN_Automata_Data::idManager_;

// -----------------------------------------------------------------------------
// Name: UnitInfos::UnitInfos
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Automata_Data::UnitInfos::UnitInfos( const ADN_Units_Data::T_UnitInfos_Vector& vector, ADN_Units_Data::UnitInfos* element /* = 0 */ )
    : ADN_CrossedRef( vector, element, true, "type" )
    , min_( 0 )
    , max_( -1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitInfos::CreateCopy
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
ADN_Automata_Data::UnitInfos* ADN_Automata_Data::UnitInfos::CreateCopy()
{
    UnitInfos* pCopy = new UnitInfos( GetVector(), GetCrossedElement() );
    pCopy->min_ = min_.GetData();
    pCopy->max_ = max_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: UnitInfos::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::UnitInfos::ReadArchive( xml::xistream& input )
{
    ADN_CrossedRef::ReadArchive( input );
    input >> xml::optional >> xml::attribute( "min-occurs", min_ )
          >> xml::optional >> xml::attribute( "max-occurs", max_ );
}

// -----------------------------------------------------------------------------
// Name: UnitInfos::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::UnitInfos::WriteArchive( xml::xostream& output, const UnitInfos* pc )
{
    output << xml::start( "unit" );
    ADN_CrossedRef::WriteArchive( output );
    if( min_.GetData() != 0 )
        output << xml::attribute( "min-occurs", min_ );
    if( max_.GetData() >= 0 )
        output << xml::attribute( "max-occurs", max_ );
    if( this == pc )
        output << xml::attribute( "command-post", true );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: AutomatonInfos::AutomatonInfos
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Automata_Data::AutomatonInfos::AutomatonInfos()
    : nId_( ADN_Automata_Data::idManager_.GetNextId() )
    , ptrUnit_( vSubUnits_, 0 )
    , ptrModel_( ADN_Workspace::GetWorkspace().GetModels().GetData().GetAutomataModelsInfos(), 0 )
    , bStrengthRatioFeedbackTime_( false )
    , strengthRatioFeedbackTime_( "0s" )
{
    BindExistenceTo( &ptrModel_ );
}

// -----------------------------------------------------------------------------
// Name: AutomatonInfos::AutomatonInfos
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Automata_Data::AutomatonInfos::AutomatonInfos( unsigned int id )
    : nId_( id )
    , ptrUnit_( vSubUnits_, 0 )
    , ptrModel_( ADN_Workspace::GetWorkspace().GetModels().GetData().GetAutomataModelsInfos(), 0 )
    , bStrengthRatioFeedbackTime_( false )
    , strengthRatioFeedbackTime_( "0s" )
{
    BindExistenceTo( &ptrModel_ );
    ADN_Automata_Data::idManager_.Lock( id );
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

    for( auto it = vSubUnits_.begin(); it != vSubUnits_.end(); ++it )
        pCopy->vSubUnits_.AddItem( ( *it )->CreateCopy() );

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::AutomatonInfos::ReadUnit
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Automata_Data::AutomatonInfos::ReadUnit( xml::xistream& input )
{
    std::auto_ptr< UnitInfos > spNew( new UnitInfos( ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsInfos() ) );
    spNew->ReadArchive( input );
    if( spNew->GetCrossedElement() )
    {
        bool cp = false;
        input >> xml::optional >> xml::attribute( "command-post", cp );
        if( cp )
            ptrUnit_ = spNew.get();
        vSubUnits_.AddItem( spNew.release() );
    }
    else
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, strName_.GetData(), eAutomata, -1, tools::translate( "ADN_Automata_Data", "Sub-units" ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: AutomatonInfos::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::AutomatonInfos::ReadArchive( xml::xistream& input )
{
    std::string strType;
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "type", strType )
          >> xml::attribute( "decisional-model", ptrModel_ )
          >> xml::optional >> xml::attribute( "force-ratio-feedback-time", strengthRatioFeedbackTime_ );
    bStrengthRatioFeedbackTime_ = strengthRatioFeedbackTime_ != "0s";
    nAgentType_ = ADN_Tr::ConvertToAgentTypeAutomate( strType );
    input >> xml::list( "unit", *this, &ADN_Automata_Data::AutomatonInfos::ReadUnit );
}

// -----------------------------------------------------------------------------
// Name: AutomatonInfos::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::AutomatonInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "automat" )
            << xml::attribute( "name", strName_ )
            << xml::attribute( "type", nAgentType_.Convert() )
            << xml::attribute( "decisional-model", ptrModel_ )
            << xml::attribute( "id", nId_ );
    if( bStrengthRatioFeedbackTime_.GetData() )
        output << xml::attribute( "force-ratio-feedback-time", strengthRatioFeedbackTime_ );
    for( auto it = vSubUnits_.begin(); it != vSubUnits_.end(); ++it )
        ( *it )->WriteArchive( output, ptrUnit_.GetData() );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: AutomatonInfos::CheckDatabaseValidity
// Created: ABR 2012-07-04
// -----------------------------------------------------------------------------
void ADN_Automata_Data::AutomatonInfos::CheckDatabaseValidity( ADN_ConsistencyChecker& checker )
{
    if( vSubUnits_.empty() )
        checker.AddError( eMissingUnitOnAutomat, strName_.GetData(), eAutomata );

    ptrModel_.CheckValidity( checker, strName_.GetData(), eAutomata, -1, tools::translate( "Automata_Data", "Doctrine model" ).toStdString() );
    ptrUnit_.CheckValidity( checker, strName_.GetData(), eAutomata, -1, tools::translate( "Automata_Data", "Command post" ).toStdString() );

    const UnitInfos* pc = ptrUnit_.GetData();
    bool error = ( pc == 0 );
    if( pc )
        for( auto it = vSubUnits_.begin(); it != vSubUnits_.end(); ++it )
        {
            const ADN_Automata_Data::UnitInfos& unit = **it;
            assert( unit.GetCrossedElement() != 0 );
            if( !unit.GetCrossedElement() )
                continue;
            const ADN_Units_Data::UnitInfos& agent = *unit.GetCrossedElement();
            if( unit.min_.GetData() == 0 && pc->strName_.GetData() == agent.strName_.GetData() )
                error = true;
        }
    if( error )
        checker.AddError( eMissingPCOnAutomat, strName_.GetData(), eAutomata );
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data constructor
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Automata_Data::ADN_Automata_Data()
    : ADN_Data_ABC( eAutomata )
{
    vAutomata_.AddUniquenessChecker( eError, duplicateName_ );
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
void ADN_Automata_Data::FilesNeeded( tools::Path::T_Paths& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szAutomata_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::Reset
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::Reset()
{
    idManager_.Reset();
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
    vAutomata_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::ReadAutomat
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Automata_Data::ReadAutomat( xml::xistream& input )
{
    std::auto_ptr< AutomatonInfos > spNew( new AutomatonInfos( input.attribute< unsigned int >( "id" ) ) );
    spNew->ReadArchive( input );
    vAutomata_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::WriteArchive( xml::xostream& output )
{
    if( vAutomata_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "automats" );
    ADN_Tools::AddSchema( output, "Automats" );
    for( auto it = vAutomata_.begin(); it != vAutomata_.end(); ++it )
        ( *it )->WriteArchive( output );
    output  << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::GetAutomataThatUseForPC
// Created: ABR 2012-08-01
// -----------------------------------------------------------------------------
QStringList ADN_Automata_Data::GetAutomataThatUseForPC( ADN_Units_Data::UnitInfos& unit )
{
    QStringList result;
    for( auto it = vAutomata_.begin(); it != vAutomata_.end(); ++it )
    {
        const UnitInfos* infos = ( *it )->ptrUnit_.GetData();
        if( infos && infos->strName_.GetData() == unit.strName_.GetData() )
            result << ( *it )->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::GetAutomataThatUseForElement
// Created: ABR 2012-08-01
// -----------------------------------------------------------------------------
QStringList ADN_Automata_Data::GetAutomataThatUseForElement( ADN_Units_Data::UnitInfos& unit )
{
    QStringList result;
    for( auto it = vAutomata_.begin(); it != vAutomata_.end(); ++it )
    {
        for( auto it2 = ( *it )->vSubUnits_.begin(); it2 != ( *it )->vSubUnits_.end(); ++it2 )
        {
            const UnitInfos* infos = ( *it )->ptrUnit_.GetData();
            if( infos && ( *it2 )->GetCrossedElement() == &unit && infos->strName_.GetData() != ( *it2 )->GetCrossedElement()->strName_.GetData() ) // all units except pc
            {
                result << ( *it )->strName_.GetData().c_str();
                break;
            }
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::GetAutomataThatUse
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
QStringList ADN_Automata_Data::GetAutomataThatUse( ADN_Units_Data::UnitInfos& unit )
{
    QStringList result;
    result << GetAutomataThatUseForPC( unit ) << GetAutomataThatUseForElement( unit );
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::GetAutomataThatUse
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
QStringList ADN_Automata_Data::GetAutomataThatUse( ADN_Models_Data::ModelInfos& model )
{
    QStringList result;
    for( auto it = vAutomata_.begin(); it != vAutomata_.end(); ++it )
    {
        AutomatonInfos* pAutomaton = *it;
        if( pAutomaton->ptrModel_.GetData() == &model )
            result << pAutomaton->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::CheckDatabaseValidity
// Created: ABR 2012-07-04
// -----------------------------------------------------------------------------
void ADN_Automata_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    for( auto it = vAutomata_.begin(); it != vAutomata_.end(); ++it )
        ( *it )->CheckDatabaseValidity( checker );
}
