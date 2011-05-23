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

PHY_MaterialCompositionType::T_MaterialCompositionMap PHY_MaterialCompositionType::materialCompositions_;
unsigned int PHY_MaterialCompositionType::nNextId_ = 0;

struct PHY_MaterialCompositionType::LoadingWrapper
{
    void ReadMaterialComposition( xml::xistream& xis )
    {
        PHY_MaterialCompositionType::ReadMaterialComposition( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: PHY_MaterialCompositionType::AttritionData constructor
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
PHY_MaterialCompositionType::AttritionData::AttritionData( const std::string& protection, xml::xistream& xis )
    : protection_        ( protection )
    , destruction_       ( 0 )
    , repairableWithEvac_( xis.attribute< float >( "repairable-with-evacuation" ) )
    , repairableNoEvac_  ( xis.attribute< float  >( "repairable-without-evacuation" ) )
{
    xis >> xml::optional >> xml::attribute( "destruction", destruction_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaterialCompositionType::Initialize
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
void PHY_MaterialCompositionType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing material compositions" );
    LoadingWrapper loader;
    xis >> xml::start( "urban" )
            >> xml::start( "urban-block-types" )
                >> xml::start( "material-composition-types" )
                    >> xml::list( "material-composition-type", loader, &LoadingWrapper::ReadMaterialComposition )
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
    for( CIT_MaterialCompositionMap itMaterial = materialCompositions_.begin(); itMaterial != materialCompositions_.end(); ++itMaterial )
        delete itMaterial->second;
    materialCompositions_.clear();
    nNextId_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaterialCompositionType::Find
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
const PHY_MaterialCompositionType* PHY_MaterialCompositionType::Find( const std::string& strName )
{
    CIT_MaterialCompositionMap itMaterial = materialCompositions_.find( strName );
    if( itMaterial == materialCompositions_.end() )
        return 0;
    return itMaterial->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaterialCompositionType::Count
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
unsigned int PHY_MaterialCompositionType::Count()
{
    return static_cast< unsigned >( materialCompositions_.size() );
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
PHY_MaterialCompositionType::AttritionData* PHY_MaterialCompositionType::FindAttrition( const std::string& protection ) const
{
    CIT_AttritionInfos it = attritions_.find( protection );
    if ( it != attritions_.end() )
        return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaterialCompositionType constructor
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
PHY_MaterialCompositionType::PHY_MaterialCompositionType( const std::string& name, xml::xistream& xis )
    : name_( name )
    , nId_ ( nNextId_++ )
{
    xis >> xml::start( "attritions" )
            >> xml::list( "attrition", *this, &PHY_MaterialCompositionType::ReadAttrition )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaterialCompositionType destructor
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
PHY_MaterialCompositionType::~PHY_MaterialCompositionType()
{
    for( CIT_AttritionInfos itAttrition = attritions_.begin(); itAttrition != attritions_.end(); ++itAttrition )
        delete itAttrition->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaterialCompositionType::ReadMaterialComposition
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
void PHY_MaterialCompositionType::ReadMaterialComposition( xml::xistream& xis )
{
    std::string strName = xis.attribute< std::string >( "name" );
    const PHY_MaterialCompositionType*& pMaterial = materialCompositions_[ strName ];
    if( pMaterial )
        xis.error( "Material composition " + strName + " already defined" );
    pMaterial = new PHY_MaterialCompositionType( strName, xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaterialCompositionType::ReadAttrition
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
void PHY_MaterialCompositionType::ReadAttrition( xml::xistream& xis )
{
    std::string protection = xis.attribute< std::string >( "protection" );
    attritions_[ protection ] = new AttritionData( protection, xis );
}
