// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "MissionParameterFactory.h"
#include "MissionParameterLimit.h"
#include "MissionParameterPhaseLine.h"
#include "MissionParameterHeading.h"
#include "MissionParameterPolygon.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/OrderParameter.h"
#include <xeumeuleu/xml.h>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: MissionParameterFactory constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameterFactory::MissionParameterFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameterFactory destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameterFactory::~MissionParameterFactory()
{
    // NOTHING
}

namespace
{
    const kernel::OrderParameter* GetParameterByName( const kernel::OrderType& type, const std::string& name )
    {
        kernel::Iterator< const kernel::OrderParameter& > it( type.CreateIterator() );
        while( it.HasMoreElements() )
        {
            const kernel::OrderParameter& parameter = it.NextElement();
            if( parameter.GetName() == name )
                return &parameter;
        }
        return 0;
    }

    int GetParameterIndex( const kernel::OrderType& type, const std::string& name )
    {
        kernel::Iterator< const kernel::OrderParameter& > it( type.CreateIterator() );
        for( unsigned int i = 0; it.HasMoreElements(); ++i )
        {
            const kernel::OrderParameter& parameter = it.NextElement();
            if( parameter.GetName() == name )
                return i;
        }
        return -1;
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameterFactory::CreateParameter
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameter_ABC* MissionParameterFactory::CreateParameter( xml::xistream& xis, const kernel::MissionType& mission ) const
{
    std::string name, code;
    xis >> xml::start( "C_BML_Who" )
            >> xml::start( "WhoInstance" )
                >> xml::start( "ControlFeatureType" )
                    >> xml::content( "jc3iedm:NameText", name )
                    >> xml::content( "jc3iedm:CategoryCode", code )
                >> xml::end()
            >> xml::end()
        >> xml::end();
    const int index = GetParameterIndex( mission, code == "PHLINE" ? "Phase lines" : name );
    if( index < 0 )
        throw std::runtime_error( __FUNCTION__ ": Unknown parameter" );
    const kernel::OrderParameter* definition = GetParameterByName( mission, code == "PHLINE" ? "Phase lines" : name );
    if( definition == 0 )
        throw std::runtime_error( __FUNCTION__ ": Unknown parameter" );

    xis >> xml::start( "C_BML_Where" )
            >> xml::start( "WhereInstance" );
    MissionParameter_ABC* param = 0;
    if( definition->GetType() == "limit" )
        param = new MissionParameterLimit( xis, *definition );
    else if( definition->GetType() == "phaselinelist" )
        param = new MissionParameterPhaseLine( xis, *definition, name );
    else if( definition->GetType() == "direction" )
        param = new MissionParameterHeading( xis, *definition );
    else if( definition->GetType() == "polygon" )
        param = new MissionParameterPolygon( xis, *definition );
    if( !param )
        throw std::runtime_error( std::string( __FUNCTION__ ": Unsupported mission parameter: " ) + definition->GetType() );
    xis     >> xml::end()
        >> xml::end();
    return param;
}
