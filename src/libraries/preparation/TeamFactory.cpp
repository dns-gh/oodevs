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
#include "FireAttribute.h"
#include "InputToxicCloudAttribute.h"
#include "LogisticAttribute.h"
#include "MedicalTreatmentAttribute.h"
#include "MineAttribute.h"
#include "NBCAttribute.h"
#include "ObstacleAttribute.h"
#include "SupplyRouteAttribute.h"

#include "ObjectAttributesContainer.h"
#include "Populations.h"
#include "EntityIntelligences.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/AgentTypes.h"
#include "ObjectAttributeFactory_ABC.h"

#include "clients_kernel/ObjectHierarchies.h"

#include <xeumeuleu/xml.h>
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
        typedef boost::function3< void, tools::SortedInterfaceContainer< Extension_ABC >&, kernel::PropertiesDictionary&, xml::xistream& > T_CallBack;
        //@}

    public:
        AttributeFactory() {}
        virtual ~AttributeFactory() {}
        
        void Register( const std::string& attribute, const T_CallBack& callback )
        {
            if ( ! callbacks_.insert( std::make_pair( attribute, callback ) ).second )
		        throw std::invalid_argument( "capacity '" + attribute + "' already registered." );
        }
        
        void Create( const std::string& attribute, tools::SortedInterfaceContainer< Extension_ABC >& result, PropertiesDictionary& dico, xml::xistream& xis )
        {
	        const CIT_Callbacks it = callbacks_.find( attribute );
	        if ( it != callbacks_.end() )
                it->second( result, dico, xis );
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
        static void Attach( tools::SortedInterfaceContainer< Extension_ABC >& result, PropertiesDictionary& dico, xml::xistream& xis )
        {
            result.Register( *new T( xis, dico ) );
        }
        
        template< typename T, typename Helper >
        static void Attach2( tools::SortedInterfaceContainer< Extension_ABC >& result, PropertiesDictionary& dico, const Helper& helper, xml::xistream& xis )
        {
            result.Register( *new T( xis, helper, dico ) );
        }
    };
}

#define BIND_ATTACH_ATTRIBUTE( CLASS, P1, P2, P3 ) \
    boost::bind( &AttributeBuilder< ##CLASS##_ABC >::Attach< CLASS >, P1, P2, P3 )

#define BIND_ATTACH_ATTRIBUTE_HELPER( CLASS, HELPER, P1, P2, P3, P4 ) \
    boost::bind( &AttributeBuilder< ##CLASS##_ABC >::Attach2< CLASS, tools::Resolver_ABC< HELPER > >, P1, P2, P3, P4 )

#define BIND_ATTACH_ATTRIBUTE_STRING_HELPER( CLASS, HELPER, P1, P2, P3, P4 ) \
    boost::bind( &AttributeBuilder< ##CLASS##_ABC >::Attach2< CLASS, tools::Resolver_ABC< HELPER, std::string > >, P1, P2, P3, P4 )

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
    factory->Register( "fire"               , BIND_ATTACH_ATTRIBUTE_STRING_HELPER( FireAttribute, kernel::FireClass, _1, _2, boost::cref( staticModel_.objectTypes_ ), _3 ) );
    factory->Register( "input-toxic-cloud"  , 
                       boost::bind( &AttributeBuilder< ToxicCloudAttribute_ABC >::Attach< InputToxicCloudAttribute >, _1, _2, _3 ) );
    factory->Register( "medical-treatment"  , BIND_ATTACH_ATTRIBUTE_STRING_HELPER( MedicalTreatmentAttribute, kernel::MedicalTreatmentType, _1, _2, boost::cref( staticModel_.objectTypes_ ), _3 ) );    
    factory->Register( "mine"               , BIND_ATTACH_ATTRIBUTE( MineAttribute, _1, _2, _3 ) );
    factory->Register( "nbc-agents"         , BIND_ATTACH_ATTRIBUTE_STRING_HELPER( NBCAttribute, kernel::NBCAgent, _1, _2, boost::cref( staticModel_.objectTypes_ ), _3 ) );    
    factory->Register( "obstacle"           , BIND_ATTACH_ATTRIBUTE( ObstacleAttribute, _1, _2, _3 ) );
    factory->Register( "supply-route"       , BIND_ATTACH_ATTRIBUTE( SupplyRouteAttribute, _1, _2, _3 ) );
    factory->Register( "tc2"                , BIND_ATTACH_ATTRIBUTE_HELPER( LogisticAttribute, kernel::Automat_ABC, _1, _2, boost::cref( model_.agents_ ), _3 ) );
    
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
    result->Attach< kernel::Diplomacies_ABC >( *new Diplomacies( controllers_.controller_, model_.teams_, *result, dico, staticModel_.teamKarmas_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new TeamHierarchies( controllers_.controller_, *result, 0 ) );
    result->Attach< kernel::CommunicationHierarchies >( *new TeamCommunications( controllers_.controller_, *result, 0 ) );
    result->Attach< kernel::IntelligenceHierarchies >( *new EntityIntelligences( controllers_.controller_, *result, 0 ) );
    result->Attach( *new Populations() );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateTeam
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
kernel::Team_ABC* TeamFactory::CreateTeam( xml::xistream& xis )
{
    Team* result = new Team( xis, controllers_.controller_, *this, idManager_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< kernel::Diplomacies_ABC >( *new Diplomacies( xis, controllers_.controller_, model_.teams_, *result, dico, staticModel_.teamKarmas_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new TeamHierarchies( controllers_.controller_, *result, 0 ) );
    result->Attach< kernel::CommunicationHierarchies >( *new TeamCommunications( controllers_.controller_, *result, 0 ) );
    result->Attach< kernel::IntelligenceHierarchies >( *new EntityIntelligences( controllers_.controller_, *result, 0 ) );
    result->Attach( *new Populations() );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateObject
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
kernel::Object_ABC* TeamFactory::CreateObject( const kernel::ObjectType& type, kernel::Team_ABC& team, const QString& name, const kernel::Location_ABC& location )
{
    Object* result = new Object( controllers_.controller_, staticModel_.coordinateConverter_, type, name, idManager_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
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
kernel::Object_ABC* TeamFactory::CreateObject( xml::xistream& xis, kernel::Team_ABC& team )
{
    Object* result = new Object( xis, controllers_.controller_, staticModel_.coordinateConverter_, staticModel_.objectTypes_, idManager_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new ObjectPositions( xis, staticModel_.coordinateConverter_, result->GetType() ) );
    result->Attach< kernel::TacticalHierarchies >( *new ObjectHierarchies( *result, &team ) );
    
    ObjectAttributesContainer& attributes = *new ObjectAttributesContainer();
    result->Attach< ObjectAttributesContainer >( attributes );
    xis >> xml::start( "attributes" )
            >> xml::list( *this, &TeamFactory::ReadAttributes, attributes, dico )
        >> xml::end();

    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::ReadAttributes
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void TeamFactory::ReadAttributes( const std::string& attr, xml::xistream& xis, tools::SortedInterfaceContainer< Extension_ABC >& result, PropertiesDictionary& dico )
{
    if( ! factory_.get() )
        Initialize();
    factory_->Create( attr, result, dico, xis );
}
