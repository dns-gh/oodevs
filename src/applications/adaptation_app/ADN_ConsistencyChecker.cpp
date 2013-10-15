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
#include "ADN_Equipments_Data.h"
#include "ADN_Resources_Data.h"
#include "ADN_WorkspaceElement.h"

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
    CheckValidDatabase();
    return !errors_.empty();
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::AddError
// Created: ABR 2012-11-15
// -----------------------------------------------------------------------------
void ADN_ConsistencyChecker::AddError( ConsistencyError error )
{
    errors_.push_back( error );
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::AddError
// Created: ABR 2012-11-15
// -----------------------------------------------------------------------------
void ADN_ConsistencyChecker::AddError( E_ConsistencyCheck type, const std::string& name, int tab, int subTab /* = -1 */, const std::string& optional /* = "" */ )
{
    ConsistencyError error( type );
    error.items_.push_back( CreateGotoInfo( name, tab, subTab ) );
    if( !optional.empty() )
        error.optional_ = optional;
    errors_.push_back( error );
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::AddLoadingError
// Created: ABR 2013-04-23
// -----------------------------------------------------------------------------
void ADN_ConsistencyChecker::AddLoadingError( ConsistencyError error )
{
    loadingErrors_.push_back( error );
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::AddLoadingError
// Created: JSR 2013-04-12
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
// Name: ADN_ConsistencyChecker::CreateGotoInfoFromNNOElement
// Created: ABR 2012-06-08
// -----------------------------------------------------------------------------
ADN_NavigationInfos::GoTo* ADN_ConsistencyChecker::CreateGotoInfo( const std::string& name, int tab, int subTab /* = -1 */ )
{
    ADN_NavigationInfos::GoTo* result = new ADN_NavigationInfos::GoTo();
    result->targetName_ = name.c_str();
    result->targetTab_ = static_cast< E_WorkspaceElements >( tab );
    if( subTab != -1 )
        result->subTargetTab_ = subTab;
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::CheckValidDatabase
// Created: ABR 2012-11-15
// -----------------------------------------------------------------------------
void ADN_ConsistencyChecker::CheckValidDatabase()
{
    for( int i = 0; i < eNbrWorkspaceElements; ++i )
        ADN_Workspace::GetWorkspace().GetWorkspaceElement( static_cast< E_WorkspaceElements >( i ) ).GetDataABC().CheckDatabaseValidity( *this );
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
// Name: ADN_ConsistencyChecker::GetLoadingErrors
// Created: JSR 2013-04-12
// -----------------------------------------------------------------------------
const ADN_ConsistencyChecker::T_Parent::T_ConsistencyErrors& ADN_ConsistencyChecker::GetLoadingErrors()
{
    return loadingErrors_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyChecker::ClearLoadingErrors
// Created: JSR 2013-04-12
// -----------------------------------------------------------------------------
void ADN_ConsistencyChecker::ClearLoadingErrors()
{
    loadingErrors_.clear();
}
