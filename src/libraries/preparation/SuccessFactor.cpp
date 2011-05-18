// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "SuccessFactor.h"
#include "ProfilesModel.h"
#include "ProfileSelection.h"
#include "SuccessFactorActions.h"
#include "SuccessFactorConditions.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: SuccessFactor constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactor::SuccessFactor( const QString& name, kernel::Controllers& controllers )
    : controller_( controllers.controller_ )
    , name_( name )
    , profiles_( new ProfileSelection( controllers ) )
    , conditions_( new SuccessFactorConditions( controllers ) )
    , actions_( new SuccessFactorActions() )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactor constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactor::SuccessFactor( xml::xistream& xis, kernel::Controllers& controllers, const ProfilesModel& profiles, const ScoresModel& scores, const SuccessFactorActionTypes& actionTypes )
    : controller_( controllers.controller_ )
    , name_( xis.attribute< std::string >( "name" ).c_str() )
    , profiles_( new ProfileSelection( controllers, xis, profiles ) )
    , conditions_( new SuccessFactorConditions( controllers, xis, scores ) )
    , actions_( new SuccessFactorActions( xis, actionTypes ) )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactor destructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactor::~SuccessFactor()
{
    controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactor::GetName
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
QString SuccessFactor::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactor::GetProfiles
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
ProfileSelection& SuccessFactor::GetProfiles() const
{
    return *profiles_;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactor::GetConditions
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorConditions& SuccessFactor::GetConditions() const
{
    return *conditions_;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactor::GetActions
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorActions& SuccessFactor::GetActions() const
{
    return *actions_;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactor::SetName
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactor::SetName( const QString& name )
{
    name_ = name;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactor::Serialize
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactor::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "factor" )
            << xml::attribute( "name", name_.ascii() );
    profiles_->Serialize( xos );
    conditions_->Serialize( xos );
    actions_->Serialize( xos );
    xos << xml::end;
}
