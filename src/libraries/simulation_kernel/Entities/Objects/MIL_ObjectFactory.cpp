// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ObjectFactory.h"
#include "MIL_ObjectLoader.h"
#include "MIL_Object_ABC.h"
#include "CapacityFactory.h"
#include "protocol/Protocol.h"
#include "Entities/MIL_Army_ABC.h"
#include "MT_Tools/MT_ScipioException.h"
#include "Urban/MIL_UrbanObject_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory constructor
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
MIL_ObjectFactory::MIL_ObjectFactory()
    : pObjectLoader_( new MIL_ObjectLoader() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory destructor
// Created: JCR 2008-04-21
// -----------------------------------------------------------------------------
MIL_ObjectFactory::~MIL_ObjectFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::Initialize
// Created: JCR 2008-05-29
// -----------------------------------------------------------------------------
void MIL_ObjectFactory::Initialize( xml::xistream& xis )
{
    pObjectLoader_->Initialize( xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::FindType
// Created: LGY 2012-09-17
// -----------------------------------------------------------------------------
const MIL_ObjectType_ABC& MIL_ObjectFactory::FindType( const std::string& type ) const
{
    return pObjectLoader_->GetType( type );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::GetDangerousObjects
// Created: CMA 2011-11-28
// -----------------------------------------------------------------------------
std::vector< unsigned int > MIL_ObjectFactory::GetDangerousObjects()
{
    return pObjectLoader_->GetDangerousObjects();
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::GetMaxAvoidanceDistance
// Created: CMA 2012-04-25
// -----------------------------------------------------------------------------
double MIL_ObjectFactory::GetMaxAvoidanceDistance() const
{
    return pObjectLoader_->GetMaxAvoidanceDistance();
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::BuildObject
// Created: JCR 2008-05-29
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectFactory::BuildObject( xml::xistream& xis, MIL_Army_ABC* army )
{
    return pObjectLoader_->CreateObject( xis, army );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::BuildObject
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectFactory::BuildObject( const sword::MissionParameters& msg, MIL_Army_ABC* army, sword::ObjectMagicActionAck_ErrorCode& value )
{
    return pObjectLoader_->CreateObject( msg, army, value );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::BuildObject
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectFactory::BuildObject( const std::string& name, const std::string& type, MIL_Army_ABC* army, const TER_Localisation& localisation,
                                                sword::ObstacleType_DemolitionTargetType obstacleType, unsigned int externalIdentifier, unsigned int forcedId /*=0*/, double density /*=0.*/ )
{
    return pObjectLoader_->CreateObject( name, type, army, localisation, obstacleType == sword::ObstacleType_DemolitionTargetType_reserved, externalIdentifier, forcedId, density );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::BuildObject
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_ObjectFactory::BuildObject( const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC* army )
{
    return pObjectLoader_->CreateObject( builder, army );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::BuildUrbanObject
// Created: JSR 2012-08-03
// -----------------------------------------------------------------------------
MIL_UrbanObject_ABC* MIL_ObjectFactory::BuildUrbanObject( xml::xistream& xis, MIL_UrbanObject_ABC* parent )
{
    return pObjectLoader_->CreateUrbanObject( xis, parent );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFactory::Update
// Created: LGY 2012-06-12
// -----------------------------------------------------------------------------
void MIL_ObjectFactory::Update( const std::string& capacity, xml::xistream& xis, MIL_Object_ABC& object ) const
{
    return pObjectLoader_->Update( capacity, xis, object );
}
