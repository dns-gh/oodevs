// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "IntelligencesModel.h"
#include "Intelligence.h"
#include "IntelligencePositions.h"
#include "IntelligenceHierarchies.h"
#include "Intelligences.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Intelligence_ABC.h"
#include "clients_kernel/Team_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: IntelligencesModel constructor
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
IntelligencesModel::IntelligencesModel( Controller& controller, const CoordinateConverter_ABC& converter, IdManager& idManager )
    : controller_( controller )
    , converter_ ( converter )
    , idManager_ ( idManager )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligencesModel destructor
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
IntelligencesModel::~IntelligencesModel()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: IntelligencesModel::Purge
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
void IntelligencesModel::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: IntelligencesModel::Create
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
Intelligence_ABC& IntelligencesModel::Create( kernel::Team_ABC& team, const std::string& symbol, const std::string& level, const Karma& karma, const geometry::Point2f& position )
{
    std::auto_ptr< Intelligence > intelligence( new Intelligence( controller_, idManager_, symbol, level, karma ) );
    intelligence->Attach< kernel::Positions >( *new IntelligencePositions( converter_, position ) );
    intelligence->Attach< kernel::TacticalHierarchies >( *new IntelligenceHierarchies( *intelligence, &team ) );

    if( Intelligences* intelligences = team.Retrieve< Intelligences >() )
        intelligences->AddIntelligence( *intelligence );
    intelligence->Polish();
    Register( intelligence->GetId(), *intelligence );
    return *intelligence.release();
}

// -----------------------------------------------------------------------------
// Name: IntelligencesModel::Create
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
void IntelligencesModel::Create( xml::xistream& xis, kernel::Team_ABC& team )
{
    std::auto_ptr< Intelligence > intelligence( new Intelligence( controller_, idManager_, xis ) );
    intelligence->Attach< kernel::Positions >( *new IntelligencePositions( converter_, xis ) );
    intelligence->Attach< kernel::TacticalHierarchies >( *new IntelligenceHierarchies( *intelligence, &team ) );

    if( Intelligences* intelligences = team.Retrieve< Intelligences >() )
        intelligences->AddIntelligence( *intelligence );
    intelligence->Polish();
    Register( intelligence->GetId(), *intelligence.release() );
}

// -----------------------------------------------------------------------------
// Name: IntelligencesModel::NotifyDeleted
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
void IntelligencesModel::NotifyDeleted( const kernel::Intelligence_ABC& element )
{
    Remove( element.GetId() );
}
