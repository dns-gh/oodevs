// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TeamFactory.h"
#include "Model.h"
#include "StaticModel.h"
#include "TeamsModel.h"
#include "AgentsModel.h"
#include "Team.h"
#include "KnowledgeGroup.h"
#include "Diplomacies.h"
#include "TeamHierarchies.h"
#include "TeamCommunications.h"
#include "Object.h"
#include "ObjectPositions.h"

#include "ActivityTimeAttribute.h"
#include "BypassAttribute.h"
#include "ConstructionAttribute.h"
#include "CrossingSiteAttribute.h"
#include "DelayAttribute.h"
#include "FireAttribute.h"
#include "FloodAttribute.h"
#include "InputToxicCloudAttribute.h"
#include "LogisticAttribute.h"
#include "MedicalTreatmentAttribute.h"
#include "MineAttribute.h"
#include "NBCAttribute.h"
#include "ObstacleAttribute.h"
#include "OccupantAttribute.h"
#include "SupplyRouteAttribute.h"
#include "StockAttribute.h"

#include "ObjectAttributesContainer.h"
#include "Inhabitants.h"
#include "Populations.h"
#include "EntityIntelligences.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/AgentTypes.h"
#include "ObjectAttributeFactory_ABC.h"

#include "clients_kernel/ObjectHierarchies.h"

#include <xeumeuleu/xml.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace kernel;

namespace
{
    // -----------------------------------------------------------------------------
    // Name: AttributeFactory
    // Created: JCR 2008-05-22
    // -----------------------------------------------------------------------------
    class AttributeFactory : public ObjectAttributeFactory_ABC
    {
    public:
        //! @name Types
        //@{
        typedef boost::function3< void, Object_ABC&, PropertiesDictionary&, xml::xistream& > T_CallBack;
        //@}

    public:
        AttributeFactory() {}
        virtual ~AttributeFactory() {}

        void Register( const std::string& attribute, const T_CallBack& callback )
        {
            if( ! callbacks_.insert( std::make_pair( attribute, callback ) ).second )
                throw std::invalid_argument( "capacity '" + attribute + "' already registered." );
        }

        void Create( const std::string& attribute, Object_ABC& object, PropertiesDictionary& dico, xml::xistream& xis )
        {
            const CIT_Callbacks it = callbacks_.find( attribute );
            if( it != callbacks_.end() )
                it->second( object, dico, xis );
        }

    private:
        //! @name Type
        //@{
        typedef std::map< std::string, T_CallBack > T_CallBacks;
        typedef T_CallBacks::const_iterator         CIT_Callbacks;
        //@}

    private:
        T_CallBacks callbacks_;
    };

    template< typename I >
    struct AttributeBuilder
    {
        template< typename T >
        static void Attach( Object_ABC& result, PropertiesDictionary& dico, xml::xistream& xis )
        {
            result.Get< ObjectAttributesContainer >().Register( *new T( xis, dico ) );
        }

        template< typename T, typename Helper >
        static void Attach( Object_ABC& result, PropertiesDictionary& dico, const Helper& helper, xml::xistream& xis )
        {
            result.Get< ObjectAttributesContainer >().Register( *new T( xis, helper, dico ) );
        }

        template< typename T, typename Helper >
        static void Attach( Object_ABC& result, PropertiesDictionary& dico, const Helper& helper, xml::xistream& xis, Controllers& controllers )
        {
            result.Get< ObjectAttributesContainer >().Register( *new T( xis, helper, dico, controllers ) );
        }
    };

    template<>
    struct AttributeBuilder< FloodAttribute_ABC >
    {
        template< typename T, typename Helper >
        static void Attach( Object_ABC& result, PropertiesDictionary& dico, const Helper& helper, xml::xistream& xis, Controllers& controllers )
        {
            result.Get< ObjectAttributesContainer >().Register( *new FloodAttribute( xis, helper, result.Get< Positions >(), dico, controllers ) );
        }
    };
}

#define BIND_ATTACH_ATTRIBUTE( CLASS, P1, P2, P3 ) \
    boost::bind( &AttributeBuilder< ##CLASS##_ABC >::Attach< CLASS >, P1, P2, P3 )

#define BIND_ATTACH_ATTRIBUTE_HELPER( CLASS, HELPER, P1, P2, P3, P4, P5 ) \
    boost::bind( &AttributeBuilder< ##CLASS##_ABC >::Attach< CLASS, HELPER >, P1, P2, P3, P4, P5 )

#define BIND_ATTACH_ATTRIBUTE_RESOLVER( CLASS, HELPER, P1, P2, P3, P4, P5 ) \
    boost::bind( &AttributeBuilder< ##CLASS##_ABC >::Attach< CLASS, tools::Resolver_ABC< HELPER > >, P1, P2, P3, P4, P5 )

#define BIND_ATTACH_ATTRIBUTE_STRING_RESOLVER( CLASS, HELPER, P1, P2, P3, P4 ) \
    boost::bind( &AttributeBuilder< ##CLASS##_ABC >::Attach< CLASS, tools::Resolver_ABC< HELPER, std::string > >, P1, P2, P3, P4 )

// -----------------------------------------------------------------------------
// Name: TeamFactory constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
TeamFactory::TeamFactory( Controllers& controllers, Model& model, const StaticModel& staticModel, IdManager& idManager )
    : controllers_( controllers )
    , model_( model )
    , staticModel_( staticModel )
    , idManager_( idManager )
    , factory_    ( )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamFactory destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
TeamFactory::~TeamFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::Initialize
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
void TeamFactory::Initialize()
{
    AttributeFactory* factory = new AttributeFactory();
    // Register capacity
    factory->Register( "activity-time"      , BIND_ATTACH_ATTRIBUTE( ActivityTimeAttribute, _1, _2, _3 ) );
    factory->Register( "bypass"             , BIND_ATTACH_ATTRIBUTE( BypassAttribute, _1, _2, _3 ) );
    factory->Register( "construction"       , BIND_ATTACH_ATTRIBUTE( ConstructionAttribute, _1, _2, _3 ) );
    factory->Register( "crossing-site"      , BIND_ATTACH_ATTRIBUTE( CrossingSiteAttribute, _1, _2, _3 ) );
    factory->Register( "delay"              , BIND_ATTACH_ATTRIBUTE( DelayAttribute, _1, _2, _3 ) );
    factory->Register( "fire"               , BIND_ATTACH_ATTRIBUTE_STRING_RESOLVER( FireAttribute, FireClass, _1, _2, boost::cref( staticModel_.objectTypes_ ), _3 ) );
    factory->Register( "flood"              , BIND_ATTACH_ATTRIBUTE_HELPER( FloodAttribute, DetectionMap, _1, _2, boost::cref( staticModel_.detection_ ), _3, boost::ref( controllers_ ) ) );
    factory->Register( "input-toxic-cloud"  ,
                       boost::bind( &AttributeBuilder< ToxicCloudAttribute_ABC >::Attach< InputToxicCloudAttribute >, _1, _2, _3 ) );
    factory->Register( "medical-treatment"  , BIND_ATTACH_ATTRIBUTE_STRING_RESOLVER( MedicalTreatmentAttribute, MedicalTreatmentType, _1, _2, boost::cref( staticModel_.objectTypes_ ), _3 ) );
    factory->Register( "mine"               , BIND_ATTACH_ATTRIBUTE( MineAttribute, _1, _2, _3 ) );
    factory->Register( "nbc-agents"         , BIND_ATTACH_ATTRIBUTE_STRING_RESOLVER( NBCAttribute, NBCAgent, _1, _2, boost::cref( staticModel_.objectTypes_ ), _3 ) );
    factory->Register( "obstacle"           , BIND_ATTACH_ATTRIBUTE( ObstacleAttribute, _1, _2, _3 ) );
    factory->Register( "supply-route"       , BIND_ATTACH_ATTRIBUTE( SupplyRouteAttribute, _1, _2, _3 ) );
    factory->Register( "tc2"                , BIND_ATTACH_ATTRIBUTE_RESOLVER( LogisticAttribute, Automat_ABC, _1, _2, boost::cref( model_.agents_ ), _3, boost::ref( controllers_ ) ) );
    factory->Register( "max-size"           , BIND_ATTACH_ATTRIBUTE( OccupantAttribute, _1, _2, _3 ) );
    factory->Register( "stock"              , BIND_ATTACH_ATTRIBUTE_STRING_RESOLVER( StockAttribute, kernel::DotationType, _1, _2, boost::cref( staticModel_.objectTypes_ ), _3 ) );

    factory_.reset( factory );
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateTeam
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Team_ABC* TeamFactory::CreateTeam()
{
    Team* result = new Team( controllers_.controller_, *this, idManager_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Diplomacies_ABC >( *new Diplomacies( controllers_.controller_, model_.teams_, *result, dico, staticModel_.teamKarmas_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new TeamHierarchies( controllers_.controller_, *result, 0 ) );
    result->Attach< CommunicationHierarchies >( *new TeamCommunications( controllers_.controller_, *result, 0 ) );
    result->Attach< IntelligenceHierarchies >( *new EntityIntelligences( controllers_.controller_, *result, 0 ) );
    result->Attach( *new Populations() );
    result->Attach( *new Inhabitants() );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateTeam
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Team_ABC* TeamFactory::CreateTeam( xml::xistream& xis )
{
    Team* result = new Team( xis, controllers_.controller_, *this, idManager_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Diplomacies_ABC >( *new Diplomacies( xis, controllers_.controller_, model_.teams_, *result, dico, staticModel_.teamKarmas_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new TeamHierarchies( controllers_.controller_, *result, 0 ) );
    result->Attach< CommunicationHierarchies >( *new TeamCommunications( controllers_.controller_, *result, 0 ) );
    result->Attach< IntelligenceHierarchies >( *new EntityIntelligences( controllers_.controller_, *result, 0 ) );
    result->Attach( *new Populations() );
    result->Attach( *new Inhabitants() );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateObject
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
Object_ABC* TeamFactory::CreateObject( const ObjectType& type, Team_ABC& team, const QString& name, const Location_ABC& location )
{
    Object* result = new Object( controllers_.controller_, staticModel_.coordinateConverter_, type, name, idManager_ );
    result->Attach< Positions >( *new ObjectPositions( staticModel_.coordinateConverter_, result->GetType(), location ) );
    result->Attach< kernel::TacticalHierarchies >( *new ObjectHierarchies( *result, &team ) );
    ObjectAttributesContainer& attributes = *new ObjectAttributesContainer();
    result->Attach< ObjectAttributesContainer >( attributes );
    // Attributes are commited by ObjectPrototype
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateObject
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
Object_ABC* TeamFactory::CreateObject( xml::xistream& xis, Team_ABC& team )
{
    Object* result = new Object( xis, controllers_.controller_, staticModel_.coordinateConverter_, staticModel_.objectTypes_, idManager_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new ObjectPositions( xis, staticModel_.coordinateConverter_, result->GetType() ) );
    result->Attach< kernel::TacticalHierarchies >( *new ObjectHierarchies( *result, &team ) );
    result->Attach< ObjectAttributesContainer >( *new ObjectAttributesContainer() );
    xis >> xml::start( "attributes" )
            >> xml::list( *this, &TeamFactory::ReadAttributes, *result, dico )
        >> xml::end;
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::ReadAttributes
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void TeamFactory::ReadAttributes( const std::string& attr, xml::xistream& xis, Object_ABC& object, PropertiesDictionary& dico )
{
    if( ! factory_.get() )
        Initialize();
    factory_->Create( attr, object, dico, xis );
}
