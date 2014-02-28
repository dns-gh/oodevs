// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MaterialCompositionType.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/ptr_container/ptr_map.hpp>

namespace
{
    boost::ptr_map< std::string, PHY_MaterialCompositionType > materialCompositions;
    unsigned int nNextId;
}

namespace
{
    PHY_MaterialCompositionType::AttritionData MakeAttrition( const std::string& protection, xml::xistream& xis )
    {
        PHY_MaterialCompositionType::AttritionData dst;
        dst.protection_ = protection;
        xis >> xml::attribute( "repairable-with-evacuation", dst.repairableWithEvac_ )
            >> xml::attribute( "repairable-without-evacuation", dst.repairableNoEvac_ )
            >> xml::optional >> xml::attribute( "destruction", dst.destruction_ );
        return dst;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MaterialCompositionType::Initialize
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
void PHY_MaterialCompositionType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing material compositions" );
    xis >> xml::start( "urban" )
            >> xml::start( "urban-block-types" )
                >> xml::start( "material-composition-types" )
                    >> xml::list( "material-composition-type", &PHY_MaterialCompositionType::ReadMaterialComposition )
                >> xml::end
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaterialCompositionType::Terminate
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
void PHY_MaterialCompositionType::Terminate()
{
    ::materialCompositions.clear();
    ::nNextId = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaterialCompositionType::Find
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
const PHY_MaterialCompositionType* PHY_MaterialCompositionType::Find( const std::string& strName )
{
    auto it = ::materialCompositions.find( strName );
    if( it == ::materialCompositions.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaterialCompositionType::Count
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
unsigned int PHY_MaterialCompositionType::Count()
{
    return static_cast< unsigned >( ::materialCompositions.size() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaterialCompositionType::GetId
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
unsigned int PHY_MaterialCompositionType::GetId() const
{
    return nId_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaterialCompositionType::GetName
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
const std::string& PHY_MaterialCompositionType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaterialCompositionType::FindAttrition
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
const PHY_MaterialCompositionType::AttritionData* PHY_MaterialCompositionType::FindAttrition( const std::string& protection ) const
{
    auto it = attritions_.find( protection );
    if( it != attritions_.end() )
        return &it->second;
    return 0;
}

namespace
{
    PHY_MaterialCompositionType::T_AttritionInfos ReadAttritions( xml::xistream& xis )
    {
        PHY_MaterialCompositionType::T_AttritionInfos dst;
        xis >> xml::start( "attritions" )
            >> xml::list( "attrition", [&]( xml::xistream& xis ){
                std::string protection = xis.attribute< std::string >( "protection" );
                dst.emplace( std::make_pair( protection, MakeAttrition( protection, xis ) ) );
            }) >> xml::end;
        return dst;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MaterialCompositionType constructor
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
PHY_MaterialCompositionType::PHY_MaterialCompositionType( const std::string& name, xml::xistream& xis )
    : name_      ( name )
    , nId_       ( ::nNextId++ )
    , attritions_( ReadAttritions( xis ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MaterialCompositionType destructor
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
PHY_MaterialCompositionType::~PHY_MaterialCompositionType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MaterialCompositionType::ReadMaterialComposition
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
void PHY_MaterialCompositionType::ReadMaterialComposition( xml::xistream& xis )
{
    std::string strName = xis.attribute< std::string >( "name" );
    if( materialCompositions.count( strName ) )
        throw MASA_EXCEPTION( xis.context() + "Material composition " + strName + " already defined" );
    auto next = std::auto_ptr< PHY_MaterialCompositionType >( new PHY_MaterialCompositionType( strName, xis ) );
    ::materialCompositions.insert( strName, next );
}
