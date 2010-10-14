// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "protocol/protocol.h"
#include "MIL_AutomatListParameter.h"
#include "simulation_orders/MIL_ParameterType_AutomatList.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/MIL_EntityManager_ABC.h"
#include "Network/NET_ASN_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_AutomatListParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AutomatListParameter::MIL_AutomatListParameter( const Common::AutomatIdList& asn, MIL_EntityManager_ABC& entityManager )
{
    automatList_.reserve( asn.elem_size() );
    for( int i = 0; i < asn.elem_size(); ++i )
    {
        MIL_Automate* pAutomate = entityManager.FindAutomate( asn.elem(i).id() );
        if( !pAutomate )
            throw std::runtime_error( "Automat does not exist" );
        automatList_.push_back( &pAutomate->GetDecision() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatListParameter constructor
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
MIL_AutomatListParameter::MIL_AutomatListParameter( const std::vector< DEC_AutomateDecision* >& automatList )
: automatList_( automatList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatListParameter destructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AutomatListParameter::~MIL_AutomatListParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatListParameter::IsOfType
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AutomatListParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_AutomatList*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatListParameter::ToAutomatList
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AutomatListParameter::ToAutomatList( Common::AutomatIdList& asn ) const
{
    unsigned int size = automatList_.size();
    for( unsigned int i = 0; i < size; ++i )
        asn.add_elem()->set_id( automatList_[i]->GetAutomate().GetID() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatListParameter::ToAutomatList
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AutomatListParameter::ToAutomatList( std::vector< DEC_Decision_ABC* >& value ) const
{
    value.resize( automatList_.size() );
    for( unsigned int i = 0; i < automatList_.size(); ++i )
        value[i] = automatList_[i];
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatListParameter::ToList
// Created: MGD 2010-10-14
// -----------------------------------------------------------------------------
bool MIL_AutomatListParameter::ToList( std::vector< Common::MsgMissionParameter_Value >& result ) const
{
    for( std::vector< DEC_AutomateDecision* >::const_iterator it = automatList_.begin(); it != automatList_.end(); ++it )
    {
        Common::MsgMissionParameter_Value automat;
        automat.mutable_automat()->set_id( (*it)->GetAutomate().GetID() );
        result.push_back( automat );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatListParameter::ToList
// Created: MGD 2010-10-14
// -----------------------------------------------------------------------------
bool MIL_AutomatListParameter::ToList( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& result ) const
{
    for( std::vector< DEC_AutomateDecision* >::const_iterator it = automatList_.begin(); it != automatList_.end(); ++it )
    {
        boost::shared_ptr<MIL_MissionParameter_ABC> param = MIL_MissionParameterFactory::Create( *it );
        result.push_back( param );
    }
    return true;
}
