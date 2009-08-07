// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_OrderType_ABC.h"
#include "MIL_OrderTypeParameter.h"
#include "MIL_OrderContext.h"
#include "Decision/DEC_Tools.h"
#include "Network/NET_AsnException.h"
#include <xeumeuleu/xml.h>
#include "MIL_MissionParameter_ABC.h"
#include <boost/shared_ptr.hpp>

//-----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_OrderType_ABC::MIL_OrderType_ABC( uint nID, xml::xistream& xis )
    : nID_       ( nID )
    , strName_   ( xml::attribute< std::string >( xis, "name" ) )
    , diaType_  ( xml::attribute< std::string >( xis, "dia-type" ) )
{
    xis >> xml::list( "parameter", *this, &MIL_OrderType_ABC::ReadParameter );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC constructor
// Created: LDC 2009-04-27
// -----------------------------------------------------------------------------
MIL_OrderType_ABC::MIL_OrderType_ABC()
    : nID_     ( 0 )
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
bool MIL_OrderType_ABC::Copy( const std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& from, ASN1T_MissionParameters& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_OrderContext& context ) const
{
    if( from.size() < (int)parameters_.size()  )//|| from.GetType() != *pDIAType_ )
        return false;

    unsigned int index = context.Length();
    to.n    = parameters_.size() + index;
    to.elem = new ASN1T_MissionParameter[ parameters_.size() + index ];
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >::const_iterator fromIt = from.begin();
    for( CIT_MissionParameterVector it = parameters_.begin(); it != parameters_.end(); ++it, ++index, ++fromIt )
    {
        if( !(**it).Copy( **fromIt, to.elem[index], knowledgeResolver ) )
        {   
            assert( false );
            delete[] to.elem;
            to.n = 0;
            return false;
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::CleanAfterSerialization
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_OrderType_ABC::CleanAfterSerialization( ASN1T_MissionParameters& to, const MIL_OrderContext& context ) const
{
    unsigned int index = context.Length();
    if( index + parameters_.size() != to.n )
        return;

    for( CIT_MissionParameterVector it = parameters_.begin(); it != parameters_.end(); ++it, ++index )
        (**it).CleanAfterSerialization( to.elem[index] );

    if( to.n > 0 )
        delete [] to.elem;
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
uint MIL_OrderType_ABC::GetID() const
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
