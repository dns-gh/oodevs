// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "IntelligenceFactory.h"
#include "Intelligence.h"
#include "Model.h"
#include "TeamsModel.h"
#include "IntelligencePositions.h"
#include "EntityIntelligences.h"
#include "Diplomacies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_gui/Tools.h"
#include "protocol/MessengerSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: IntelligenceFactory constructor
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
IntelligenceFactory::IntelligenceFactory( Controllers& controllers, const CoordinateConverter_ABC& converter, const Model& model, const tools::Resolver_ABC< HierarchyLevel_ABC >& levels, Publisher_ABC& publisher )
    : controllers_( controllers )
    , converter_  ( converter )
    , model_      ( model )
    , levels_     ( levels )
    , publisher_  ( publisher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligenceFactory destructor
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
IntelligenceFactory::~IntelligenceFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligenceFactory::Create
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
Intelligence_ABC* IntelligenceFactory::Create( const sword::IntelligenceCreation& message ) const
{
    std::auto_ptr< Intelligence > element( new Intelligence( message, controllers_.controller_, model_.teams_, levels_, publisher_, model_.symbolsFactory_ ) );
    element->Attach< kernel::Positions >( *new IntelligencePositions( converter_, *element, publisher_ ) );
    element->Attach< kernel::IntelligenceHierarchies >( *new EntityIntelligences( controllers_.controller_, *element, model_.teams_, model_.symbolsFactory_ ) );
    element->Update( message );
    element->Polish();
    return element.release();
}

namespace
{
    sword::EnumDiplomacy ConvertToDiplomacy( const Karma& karma )
    {
        if( karma == Karma::friend_ )
            return sword::friendly;
        if( karma == Karma::enemy_ )
            return sword::enemy;
        if( karma == Karma::neutral_ )
            return sword::neutral;
        return sword::unknown;
    }
}

// -----------------------------------------------------------------------------
// Name: IntelligenceFactory::Create
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
Intelligence_ABC* IntelligenceFactory::Create( Entity_ABC& superior, const std::string& symbol, const HierarchyLevel_ABC& level, bool mounted, const Karma& karma, const geometry::Point2f& position )
{
    static unsigned int counter = 0;
    plugins::messenger::IntelligenceCreationRequest message;
    const QString name = tools::translate( "Intelligence", "Intelligence [%1]" ).arg( ++counter );
    message().mutable_intelligence()->set_name( name.ascii() );
    *message().mutable_intelligence()->mutable_nature() = symbol.c_str();
    message().mutable_intelligence()->set_level( (sword::EnumNatureLevel)level.GetId() );
    message().mutable_intelligence()->set_diplomacy( ConvertToDiplomacy( karma ) );
    message().mutable_intelligence()->set_embarked( mounted );
    converter_.ConvertToGeo< sword::CoordLatLong >( position, *message().mutable_intelligence()->mutable_location() );
    message().mutable_intelligence()->mutable_formation()->set_id( superior.GetId() );
    message.Send( publisher_ );
    return 0;
}
