// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "SuccessFactorConditions.h"
#include "Score_ABC.h"
#include "ScoresModel.h"
#include "SuccessFactorCondition.h"
#include "clients_kernel/Controllers.h"
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditions constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorConditions::SuccessFactorConditions( kernel::Controllers& controllers )
    : controllers_( controllers )
    , operator_( "or" )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditions constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorConditions::SuccessFactorConditions( kernel::Controllers& controllers, xml::xistream& xis, const ScoresModel& model )
    : controllers_( controllers )
{
    xis >> xml::start( "conditions" )
            >> xml::attribute( "operator", operator_ )
            >> xml::list( "condition", *this, &SuccessFactorConditions::ReadCondition, model )
        >> xml::end;
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditions destructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorConditions::~SuccessFactorConditions()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditions::NotifyDeleted
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorConditions::NotifyDeleted( const Score_ABC& score )
{
    IT_Elements it = elements_.begin();
    while( it != elements_.end() )
    {
        IT_Elements itCur = it++;
        if( &itCur->second->GetScore() == &score )
        {
            delete itCur->second;
            Remove( itCur->first );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditions::ReadCondition
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorConditions::ReadCondition( xml::xistream& xis, const ScoresModel& model )
{
    std::unique_ptr< SuccessFactorCondition > condition( new SuccessFactorCondition( xis, model ) );
    Register( static_cast< unsigned long >( elements_.size() ), *condition.release() );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditions::Serialize
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorConditions::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "conditions" )
            << xml::attribute( "operator", operator_ );
    BOOST_FOREACH( const T_Elements::value_type condition, elements_ )
        condition.second->Serialize( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditions::GetOperator
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
std::string SuccessFactorConditions::GetOperator() const
{
    return operator_;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditions::SetOperator
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
void SuccessFactorConditions::SetOperator( const std::string& op )
{
    operator_ = op;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorConditions::AddCondition
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
void SuccessFactorConditions::AddCondition( SuccessFactorCondition& condition )
{
    Register( static_cast< unsigned long >( elements_.size() ), condition );
}
