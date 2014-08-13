// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "protocol/Protocol.h"
#include "MIL_LimaListParameter.h"
#include "MIL_LimaOrder.h"
#include <boost/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: MIL_LimaListParameter constructor
// Created: MGD 2010-10-27
// -----------------------------------------------------------------------------
MIL_LimaListParameter::MIL_LimaListParameter( const sword::PhaseLinesOrder& message, const DEC_KnowledgeResolver_ABC& resolver )
{
    for( int i = 0; i < message.elem_size(); ++i )
        limas_.push_back( boost::make_shared< MIL_LimaOrder >( message.elem(i), resolver ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaListParameter destructor
// Created: MGD 2010-10-27
// -----------------------------------------------------------------------------
MIL_LimaListParameter::~MIL_LimaListParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaListParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_LimaListParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::ePhaseLine;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaListParameter::ToLima
// Created: MGD 2010-10-27
// -----------------------------------------------------------------------------
bool MIL_LimaListParameter::ToLima( sword::PhaseLinesOrder& message ) const
{
    if( limas_.size() == 1 )
    {
        limas_[0]->Serialize( *message.add_elem() );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaListParameter::ToLimaList
// Created: MGD 2010-10-27
// -----------------------------------------------------------------------------
bool MIL_LimaListParameter::ToLimaList( sword::PhaseLinesOrder& message ) const
{
    for (unsigned int i = 0; i < limas_.size(); ++i )
        limas_[i]->Serialize( *message.add_elem() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaListParameter::ToLima
// Created: MGD 2010-10-27
// -----------------------------------------------------------------------------
bool MIL_LimaListParameter::ToLima( boost::shared_ptr< TER_Localisation >& value ) const
{
    if( limas_.size() == 1 )
    {
        value = limas_[0]->GetLocalisation();
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaListParameter::ToLimaList
// Created: MGD 2010-10-27
// -----------------------------------------------------------------------------
bool MIL_LimaListParameter::ToLimaList( std::vector< boost::shared_ptr< TER_Localisation > >& values ) const
{
    for (unsigned int i = 0; i < limas_.size(); ++i )
        values.push_back( limas_[i]->GetLocalisation() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaListParameter::ToElement
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
bool MIL_LimaListParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    return ToLimaList( *elem.mutable_phaseline() );
}

