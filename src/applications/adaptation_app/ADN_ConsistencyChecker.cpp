// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ConsistencyChecker.h"
#include "ADN_Workspace.h"
#include "ADN_Composantes_Data.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Breakdowns_Data.h"

ADN_ConsistencyChecker::T_Parent::T_ConsistencyErrors ADN_ConsistencyChecker::loadingErrors_;

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker constructor
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
ADN_ConsistencyChecker::ADN_ConsistencyChecker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker destructor
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
ADN_ConsistencyChecker::~ADN_ConsistencyChecker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::CheckConsistency
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
bool ADN_ConsistencyChecker::CheckConsistency()
{
    ClearErrors();
    CheckNNOConsistency();
    CheckMissionsTypes();
    CheckBreakdownsBackup();
    CheckMissionParameters();
    CheckComposantes();
    return !errors_.empty();
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::IsAlreadyRegistered
// Created: ABR 2012-06-08
// -----------------------------------------------------------------------------
bool ADN_ConsistencyChecker::IsAlreadyRegistered( const std::string& code, E_ConsistencyCheck type ) const
{
    for( auto it = errors_.begin(); it != errors_.end(); ++it )
        if( it->type_ == type && it->optional_ == code )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::CreateGotoInfoFromNNOElement
// Created: ABR 2012-06-08
// -----------------------------------------------------------------------------
ADN_NavigationInfos::GoTo* ADN_ConsistencyChecker::CreateGotoInfo( const std::string& name, int tab, int subTab /* = -1 */ )
{
    ADN_NavigationInfos::GoTo* result = new ADN_NavigationInfos::GoTo();
    result->targetName_ = name.c_str();
    assert( tab >= 0 && tab < eNbrWorkspaceElements );
    result->targetTab_ = static_cast< E_WorkspaceElements >( tab );
    if( subTab != -1 )
        result->subTargetTab_ = subTab;
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::AddError
// Created: ABR 2012-06-08
// -----------------------------------------------------------------------------
void ADN_ConsistencyChecker::AddError( E_ConsistencyCheck type, const NNOElement& element )
{
    assert( ( type & eMissingMask ) != 0 );
    ConsistencyError error( type );
    error.items_.push_back( CreateGotoInfo( element.name_, element.tab_, element.subTab_ ) );
    errors_.push_back( error );
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::AddError
// Created: ABR 2012-06-08
// -----------------------------------------------------------------------------
void ADN_ConsistencyChecker::AddError( E_ConsistencyCheck type, const T_NNOElements& elements )
{
    assert( ( type & eUniquenessMask ) != 0 );
    assert( elements.size() > 1 );
    ConsistencyError error( type );
    for( auto it = elements.begin(); it != elements.end(); ++it )
    {
        const NNOElement& element = *it;
        error.items_.push_back( CreateGotoInfo( element.name_, element.tab_, element.subTab_ ) );
    }
    error.optional_ = ( type == eNNoUniqueness ) ? elements.front().codeNNO_ : elements.front().codeEMAT8_;
    errors_.push_back( error );
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::CheckNNOConsistency
// Created: ABR 2012-06-08
// -----------------------------------------------------------------------------
void ADN_ConsistencyChecker::CheckNNOConsistency()
{
    // Fill elements
    T_NNOElements elements;
    ADN_Composantes_Data::T_ComposanteInfos_Vector composantes = ADN_Workspace::GetWorkspace().GetComposantes().GetData().GetComposantes();
    for( ADN_Composantes_Data::CIT_ComposanteInfos_Vector itComposante = composantes.begin(); itComposante != composantes.end(); ++itComposante )
    {
        ADN_Composantes_Data::ComposanteInfos& infos = **itComposante;
        elements.push_back( NNOElement( infos.strName_.GetData(), infos.strCodeNNO_.GetData(), infos.strCodeEMAT8_.GetData()
                                      , eComposantes, -1, infos.logInfos_.IsRepairTypeValid() ) );
    }

    ADN_Equipement_Data::T_ResourceInfos_Vector ressourceCategories = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotations(); 
    for( ADN_Equipement_Data::CIT_ResourceInfos_Vector itCategory = ressourceCategories.begin(); itCategory != ressourceCategories.end(); ++itCategory )
    {
        ADN_Equipement_Data::T_CategoryInfos_Vector ressources = ( *itCategory )->categories_;
        for( ADN_Equipement_Data::CIT_CategoryInfos_Vector itRessource = ressources.begin(); itRessource != ressources.end(); ++itRessource )
        {
            ADN_Equipement_Data::CategoryInfo& infos = **itRessource;
            elements.push_back( NNOElement( infos.strName_.GetData(), infos.strCodeNNO_.GetData(), infos.strCodeEMAT8_.GetData()
                                          , eEquipement, ( *itCategory )->nType_ ) );
        }
    }

    // Check errors
    for( std::vector< NNOElement >::iterator itFirst = elements.begin(); itFirst != elements.end(); ++itFirst )
    {
        NNOElement& firstElement = *itFirst;

        const std::string& emat8 = firstElement.codeEMAT8_;
        // Check initialization
        if( firstElement.codeNNO_.empty() )
            AddError( eMissingNNo, firstElement );
        if( emat8.empty() )
            AddError( eMissingEmat, firstElement );
        std::string upperEmat8 = emat8;
        std::transform( upperEmat8.begin(), upperEmat8.end(), upperEmat8.begin(), toupper );
        if( upperEmat8 != emat8 )
            AddError( eLowerCaseEmat, firstElement );
        if( !firstElement.bRepairTypeValid_ )
            AddError( eMissingRepairType, firstElement );

        // Check uniqueness
        std::vector< NNOElement > NNOelements;
        std::vector< NNOElement > EMATelements;
        if( !firstElement.codeNNO_.empty() || !firstElement.codeEMAT8_.empty() )
        {
            for( std::vector< NNOElement >::iterator itSecond = itFirst + 1; itSecond != elements.end(); ++itSecond )
            {
                NNOElement& secondElement = *itSecond;
                if( !firstElement.codeNNO_.empty() && !secondElement.codeNNO_.empty() && firstElement.codeNNO_ == secondElement.codeNNO_ && !IsAlreadyRegistered( secondElement.codeNNO_, eNNoUniqueness ) )
                    NNOelements.push_back( secondElement );
                if( !emat8.empty() && !secondElement.codeEMAT8_.empty() && emat8 == secondElement.codeEMAT8_ && !IsAlreadyRegistered( secondElement.codeEMAT8_, eEmatUniqueness ) )
                    EMATelements.push_back( secondElement );
            }
        }
        if( !NNOelements.empty() )
        {
            NNOelements.insert( NNOelements.begin(), firstElement );
            AddError( eNNoUniqueness, NNOelements );
        }
        if( !EMATelements.empty() )
        {
            EMATelements.insert( EMATelements.begin(), firstElement );
            AddError( eEmatUniqueness, EMATelements );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::CheckMissionsTypes
// Created: LGY 2012-10-22
// -----------------------------------------------------------------------------
void ADN_ConsistencyChecker::CheckMissionsTypes()
{
    CheckMissionTypes( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetUnitMissions(), 0 );
    CheckMissionTypes( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetAutomatMissions(), 1 );
    CheckMissionTypes( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetPopulationMissions(), 2 );
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::CheckMissionTypes
// Created: LGY 2012-10-22
// -----------------------------------------------------------------------------
void ADN_ConsistencyChecker::CheckMissionTypes( const ADN_Missions_Data::T_Mission_ABC_Vector& missions, int subTab )
{
    for( auto rhs = missions.begin(); rhs != missions.end() - 1; ++rhs )
        for( auto lhs = rhs + 1; lhs != missions.end(); ++lhs )
            if( (*lhs)->strName_.GetData() != (*rhs)->strName_.GetData() &&
                (*lhs)->diaType_.GetData() == (*rhs)->diaType_.GetData() )
                    AddError( eMissionTypeUniqueness, **rhs, **lhs, subTab );
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::AddError
// Created: LGY 2012-10-22
// -----------------------------------------------------------------------------
void ADN_ConsistencyChecker::AddError( E_ConsistencyCheck type, const ADN_Missions_Data::ADN_Missions_ABC& rhs, const ADN_Missions_Data::ADN_Missions_ABC& lhs, int subTab )
{
    ConsistencyError error( type );
    error.items_.push_back( CreateGotoInfo( rhs.strName_.GetData(), eMissions, subTab ) );
    error.items_.push_back( CreateGotoInfo( lhs.strName_.GetData(), eMissions, subTab ) );
    errors_.push_back( error );
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::CheckBreakdownsBackup
// Created: ABR 2012-11-08
// -----------------------------------------------------------------------------
void ADN_ConsistencyChecker::CheckBreakdownsBackup()
{
    ADN_Breakdowns_Data::T_BreakdownInfoVector& breakdowns = ADN_Workspace::GetWorkspace().GetBreakdowns().GetData().GetBreakdowns();
    for( ADN_Breakdowns_Data::CIT_BreakdownInfoVector it = breakdowns.begin(); it != breakdowns.end(); ++it )
        if( ( *it )->vRepairParts_.size() == 0 )
        {
            ConsistencyError error( eMissingPart );
            error.items_.push_back( CreateGotoInfo( ( *it )->strName_.GetData(), eBreakdowns ) );
            errors_.push_back( error );
        }
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::CheckMissionParameter
// Created: ABR 2012-11-14
// -----------------------------------------------------------------------------
void ADN_ConsistencyChecker::CheckMissionParameters()
{
    ADN_Missions_Data& data = ADN_Workspace::GetWorkspace().GetMissions().GetData();

    for( int i = 0; i < eNbrMissionTypes; ++i )
        for( auto it = data.missionsVector_[ i ].begin(); it != data.missionsVector_[ i ].end(); ++it )
            CheckParameters( ( *it )->parameters_, ( *it )->strName_.GetData(), i );
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::CheckParameters
// Created: ABR 2012-11-14
// -----------------------------------------------------------------------------
void ADN_ConsistencyChecker::CheckParameters( const ADN_Missions_Data::T_MissionParameter_Vector& parameters, const std::string& missionName, int subTab )
{
    for( ADN_Missions_Data::CIT_MissionParameter_Vector it = parameters.begin(); it != parameters.end(); ++it )
        if( ( *it )->type_.GetData() == eMissionParameterTypeLocationComposite )
        {
            bool hasChoice = false;
            for( std::size_t i = 0; i < ( *it )->choices_.size() && !hasChoice; ++i )
                hasChoice = ( *it )->choices_[ i ]->isAllowed_.GetData();
            if( !hasChoice )
            {
                ConsistencyError error( eMissingChoiceComposite );
                error.items_.push_back( CreateGotoInfo( missionName, eMissions, subTab ) );
                errors_.push_back( error );
            }
        }
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::CheckComposantes
// Created: LDC 2013-09-09
// -----------------------------------------------------------------------------
void ADN_ConsistencyChecker::CheckComposantes()
{
    ADN_Composantes_Data& data = ADN_Workspace::GetWorkspace().GetComposantes().GetData();
    
    auto composantes = data.GetComposantes();
    for( auto it = composantes.begin(); it != composantes.end(); ++it )
    {
        auto sensors = (*it)->vSensors_;
        std::set< std::string > alreadyDefinedSensors;
        for( auto sensor = sensors.begin(); sensor != sensors.end(); ++sensor )
            if( !alreadyDefinedSensors.insert( (*sensor)->GetItemName() ).second )
            {
                ConsistencyError error( eDuplicateSensor );
                error.items_.push_back( CreateGotoInfo( ( *it )->strName_.GetData(), eComposantes ) );
                errors_.push_back( error );
                break;
            }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::AddLoadingError
// Created: ABR 2013-05-21
// -----------------------------------------------------------------------------
void ADN_ConsistencyChecker::AddLoadingError( ConsistencyError error )
{
    loadingErrors_.push_back( error );
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::AddLoadingError
// Created: ABR 2013-05-21
// -----------------------------------------------------------------------------
void ADN_ConsistencyChecker::AddLoadingError( E_ConsistencyCheck type, const std::string& name, int tab, int subTab /*= -1*/, const std::string& optional /*= ""*/ )
{
    ConsistencyError error( type );
    error.items_.push_back( CreateGotoInfo( name, tab, subTab ) );
    if( !optional.empty() )
        error.optional_ = optional;
    loadingErrors_.push_back( error );
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::ClearLoadingErrors
// Created: ABR 2013-05-21
// -----------------------------------------------------------------------------
void ADN_ConsistencyChecker::ClearLoadingErrors()
{
    loadingErrors_.clear();
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::GetLoadingErrors
// Created: ABR 2013-05-21
// -----------------------------------------------------------------------------
const ADN_ConsistencyChecker::T_ConsistencyErrors& ADN_ConsistencyChecker::GetLoadingErrors()
{
    return loadingErrors_;
}
