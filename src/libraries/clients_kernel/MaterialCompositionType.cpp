// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "MaterialCompositionType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

unsigned int MaterialCompositionType::nNextID_ = 0;

// -----------------------------------------------------------------------------
// Name: MaterialCompositionType::AttritionData constructor
// Created: JSR 2010-11-17
// -----------------------------------------------------------------------------
MaterialCompositionType::AttritionData::AttritionData( xml::xistream &xis )
    : protection_( xis.attribute< std::string >( "protection" ) )
    , destruction_( xis.attribute< float >( "destruction" ) )
    , repairableWithEvac_( xis.attribute< float >( "repairable-with-evacuation" ) )
    , repairableNoEvac_( xis.attribute< float  >( "repairable-without-evacuation" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaterialCompositionType constructor
// Created: SLG 2010-11-17
// -----------------------------------------------------------------------------
MaterialCompositionType::MaterialCompositionType( xml::xistream& xis )
    : id_( nNextID_++ )
    , name_( xis.attribute< std::string >( "name" ) )
{
    xis >> xml::start( "attritions" )
            >> xml::list( "attrition", *this, &MaterialCompositionType::ReadAttrition )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MaterialCompositionType destructor
// Created: SLG 2010-11-17
// -----------------------------------------------------------------------------
MaterialCompositionType::~MaterialCompositionType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaterialCompositionType::ReadAttrition
// Created: SLG 2010-11-17
// -----------------------------------------------------------------------------
void MaterialCompositionType::ReadAttrition( xml::xistream& xis )
{
    AttritionData* attrition = new AttritionData( xis );
    attritions_.insert( std::pair< std::string, AttritionData* >( attrition->protection_, attrition ) );
}

// -----------------------------------------------------------------------------
// Name: MaterialCompositionType::GetName
// Created: SLG 2010-11-17
// -----------------------------------------------------------------------------
const std::string& MaterialCompositionType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: MaterialCompositionType::GetId
// Created: SLG 2010-11-17
// -----------------------------------------------------------------------------
unsigned int MaterialCompositionType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: MaterialCompositionType::operator=
// Created: SLG 2010-07-06
// -----------------------------------------------------------------------------
MaterialCompositionType& MaterialCompositionType::operator=( const MaterialCompositionType& from )
{
    id_ = from.id_;
    name_ = from.name_;
    attritions_ = from.attritions_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MaterialCompositionType::FindAttrition
// Created: SLG 2010-11-17
// -----------------------------------------------------------------------------
MaterialCompositionType::AttritionData* MaterialCompositionType::FindAttrition( const std::string& protection ) const
{
    CIT_AttritionInfos it = attritions_.find( protection );
    if( it != attritions_.end() )
        return it->second;
    return 0;
}