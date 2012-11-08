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
    return !errors_.empty();
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::IsAlreadyRegistered
// Created: ABR 2012-06-08
// -----------------------------------------------------------------------------
bool ADN_ConsistencyChecker::IsAlreadyRegistered( const std::string& code, E_ConsistencyCheck type ) const
{
    for( CIT_ConsistencyErrors it = errors_.begin(); it != errors_.end(); ++it )
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
    for( CIT_NNOElements it = elements.begin(); it != elements.end(); ++it )
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
    T_NNOElements elements_;
    ADN_Composantes_Data::T_ComposanteInfos_Vector composantes = ADN_Workspace::GetWorkspace().GetComposantes().GetData().GetComposantes();
    for( ADN_Composantes_Data::CIT_ComposanteInfos_Vector itComposante = composantes.begin(); itComposante != composantes.end(); ++itComposante )
    {
        ADN_Composantes_Data::ComposanteInfos& infos = **itComposante;
        elements_.push_back( NNOElement( infos.strName_.GetData(), infos.strCodeNNO_.GetData(), infos.strCodeEMAT8_.GetData(), eComposantes ) );
    }

    ADN_Equipement_Data::T_ResourceInfos_Vector ressourceCategories = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotations(); 
    for( ADN_Equipement_Data::CIT_ResourceInfos_Vector itCategory = ressourceCategories.begin(); itCategory != ressourceCategories.end(); ++itCategory )
    {
        ADN_Equipement_Data::T_CategoryInfos_Vector ressources = ( *itCategory )->categories_;
        for( ADN_Equipement_Data::CIT_CategoryInfos_Vector itRessource = ressources.begin(); itRessource != ressources.end(); ++itRessource )
        {
            ADN_Equipement_Data::CategoryInfo& infos = **itRessource;
            elements_.push_back( NNOElement( infos.strName_.GetData(), infos.strCodeNNO_.GetData(), infos.strCodeEMAT8_.GetData(), eEquipement, ( *itCategory )->nType_ ) );
        }
    }

    // Check errors
    for( std::vector< NNOElement >::iterator itFirst = elements_.begin(); itFirst != elements_.end(); ++itFirst )
    {
        NNOElement& firstElement = *itFirst;

        // Check initialization
        if( firstElement.codeNNO_.empty() )
            AddError( eMissingNNo, firstElement );
        if( firstElement.codeEMAT8_.empty() )
            AddError( eMissingEmat, firstElement );

        // Check uniqueness
        std::vector< NNOElement > NNOelements;
        std::vector< NNOElement > EMATelements;
        if( !firstElement.codeNNO_.empty() || !firstElement.codeEMAT8_.empty() )
        {
            for( std::vector< NNOElement >::iterator itSecond = itFirst + 1; itSecond != elements_.end(); ++itSecond )
            {
                NNOElement& secondElement = *itSecond;
                if( !firstElement.codeNNO_.empty() && !secondElement.codeNNO_.empty() && firstElement.codeNNO_ == secondElement.codeNNO_ && !IsAlreadyRegistered( secondElement.codeNNO_, eNNoUniqueness ) )
                    NNOelements.push_back( secondElement );
                if( !firstElement.codeEMAT8_.empty() && !secondElement.codeEMAT8_.empty() && firstElement.codeEMAT8_ == secondElement.codeEMAT8_ && !IsAlreadyRegistered( secondElement.codeEMAT8_, eEmatUniqueness ) )
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
void ADN_ConsistencyChecker::CheckMissionTypes( const ADN_Missions_Data::T_Mission_Vector& missions, int subTab )
{
    for( ADN_Missions_Data::CIT_Mission_Vector rhs = missions.begin(); rhs != missions.end() - 1; ++rhs )
        for( ADN_Missions_Data::CIT_Mission_Vector lhs = rhs + 1; lhs != missions.end(); ++lhs )
            if( (*lhs)->strName_.GetData() != (*rhs)->strName_.GetData() &&
                (*lhs)->diaType_.GetData() == (*rhs)->diaType_.GetData() )
                    AddError( eMissionTypeUniqueness, **rhs, **lhs, subTab );
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::AddError
// Created: LGY 2012-10-22
// -----------------------------------------------------------------------------
void ADN_ConsistencyChecker::AddError( E_ConsistencyCheck type, const ADN_Missions_Data::Mission& rhs, const ADN_Missions_Data::Mission& lhs, int subTab )
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
