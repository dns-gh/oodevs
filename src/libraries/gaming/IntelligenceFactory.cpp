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
#include "ASN_Messages.h"
#include "IntelligencePositions.h"
#include "EntityIntelligences.h"
#include "Diplomacies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_gui/Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: IntelligenceFactory constructor
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
IntelligenceFactory::IntelligenceFactory( Controllers& controllers, const CoordinateConverter_ABC& converter, const Model& model, const Resolver_ABC< HierarchyLevel_ABC >& levels, Publisher_ABC& publisher )
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
Intelligence_ABC* IntelligenceFactory::Create( const ASN1T_MsgIntelligenceCreation& message ) const
{
    std::auto_ptr< Intelligence > element( new Intelligence( message, controllers_.controller_, model_.teams_, levels_, publisher_ ) );
    element->Attach< kernel::Positions >( *new IntelligencePositions( converter_, *element, publisher_ ) );
    element->Attach< kernel::IntelligenceHierarchies >( *new EntityIntelligences( controllers_.controller_, *element, model_.teams_ ) );
    element->Update( message );
    element->Polish();
    return element.release();
}

namespace
{
    ASN1T_EnumDiplomacy ConvertToDiplomacy( const Karma& karma )
    {
        if( karma == Karma::friend_ )
            return EnumDiplomacy::ami;
        if( karma == Karma::enemy_ )
            return EnumDiplomacy::ennemi;
        if( karma == Karma::neutral_ )
            return EnumDiplomacy::neutre;
        return EnumDiplomacy::inconnu;
    }
}

// -----------------------------------------------------------------------------
// Name: IntelligenceFactory::Create
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
Intelligence_ABC* IntelligenceFactory::Create( Entity_ABC& superior, const std::string& symbol, const HierarchyLevel_ABC& level, bool embarked, const Karma& karma, const geometry::Point2f& position )
{
    static unsigned int counter = 0;
    ASN_MsgIntelligenceCreationRequest asn;
    const QString name = tools::translate( "Intelligence", "Intelligence [%1]" ).arg( ++counter );
    asn().intelligence.name = name.ascii();
    asn().intelligence.nature = symbol.c_str();
    asn().intelligence.level = (ASN1T_EnumNatureLevel)level.GetId();
    asn().intelligence.diplomacy = ConvertToDiplomacy( karma );
    asn().intelligence.embarked = embarked;
    asn().intelligence.location = converter_.ConvertToMgrs( position ).c_str();
    asn().intelligence.formation = superior.GetId();
    asn.Send( publisher_ );
    return 0;
}
