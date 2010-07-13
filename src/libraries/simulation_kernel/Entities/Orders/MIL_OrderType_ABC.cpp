// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_OrderType_ABC.h"
#include "MIL_OrderTypeParameter.h"
#include "MIL_OrderContext.h"
#include "Decision/DEC_Tools.h"
#include "Network/NET_AsnException.h"
#include "protocol/protocol.h"
#include "simulation_orders/MIL_MissionParameter_ABC.h"
#include "MIL_MissionParameterFactory.h"
#include <boost/shared_ptr.hpp>
#include <xeumeuleu/xml.hpp>

//-----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_OrderType_ABC::MIL_OrderType_ABC( unsigned int nID, xml::xistream& xis )
    : nID_    ( nID )
    , strName_( xis.attribute< std::string >( "name" ) )
    , diaType_( xis.attribute< std::string >( "dia-type" ) )
{
    xis >> xml::list( "parameter", *this, &MIL_OrderType_ABC::ReadParameter );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC constructor
// Created: LDC 2009-04-27
// -----------------------------------------------------------------------------
MIL_OrderType_ABC::MIL_OrderType_ABC()
    : nID_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::ReadParameter
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void MIL_OrderType_ABC::ReadParameter( xml::xistream& xis )
{
    parameters_.push_back( new MIL_OrderTypeParameter( *this, xis ) );
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_OrderType_ABC::~MIL_OrderType_ABC()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_OrderType_ABC::Copy( const std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& from, Common::MsgMissionParameters& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_OrderContext& context ) const
{
    if( from.size() < (int)parameters_.size()  )//|| from.GetType() != *pDIAType_ )
        return false;

    unsigned int index = context.Length();
    for( unsigned int i = 0; i < parameters_.size() + index; ++i )
        to.add_elem();
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >::const_iterator fromIt = from.begin();
    for( CIT_MissionParameterVector it = parameters_.begin(); it != parameters_.end(); ++it, ++index, ++fromIt )
        if( !(**it).Copy( **fromIt, *to.mutable_elem( index ), knowledgeResolver ) )
        {
            assert( false );
            to.mutable_elem()->Clear();
            return false;
        }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::InitializeDefault
// Created: LMT 2010-04-19
// -----------------------------------------------------------------------------
void MIL_OrderType_ABC::InitializeDefault( std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& parameters ) const
{
    for( CIT_MissionParameterVector it = parameters_.begin(); it != parameters_.end(); ++it )
        parameters.push_back( MIL_MissionParameterFactory::Create( **it ) );
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::GetDIAType
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
const std::string& MIL_OrderType_ABC::GetDIAType() const
{
    return diaType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::GetID
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
unsigned int MIL_OrderType_ABC::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::GetName
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
const std::string& MIL_OrderType_ABC::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::GetParameterType
// Created: LDC 2009-04-30
// -----------------------------------------------------------------------------
const MIL_ParameterType_ABC& MIL_OrderType_ABC::GetParameterType( unsigned int index ) const
{
    return parameters_[index]->GetType();
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::GetParameterName
// Created: LDC 2009-04-30
// -----------------------------------------------------------------------------
const std::string& MIL_OrderType_ABC::GetParameterName( unsigned int index ) const
{
    return parameters_[index]->GetDIAName();
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::GetParameterIndex
// Created: LDC 2009-07-08
// -----------------------------------------------------------------------------
unsigned int MIL_OrderType_ABC::GetParameterIndex( const std::string& name ) const
{
    unsigned int size = parameters_.size();
    for( unsigned int i = 0; i < size; ++i )
    {
        if( parameters_[i]->GetDIAName() == name )
            return i;
    }
    throw std::runtime_error( "Incorrect parameter name" );
}
