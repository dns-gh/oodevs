// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/ClassBuilders.h"
#include "hla_plugin/HlaObject_ABC.h"
#include <hla/Class.h>  // $$$$ _RC_ SLI 2011-07-26: not mockable
#include "MockFederate.h"
#include "MockObjectRegistration.h" // $$$$ _RC_ SLI 2011-07-26: Don't mock types you don't own
#include "MockAttributeFunctor.h"   // $$$$ _RC_ SLI 2011-07-26: Don't mock types you don't own
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : hlaClass( objectRegistration, true )
        {}
        void Check( ClassBuilder_ABC& builder, const std::string& name, bool publish, bool subscribe )
        {
            MOCK_EXPECT( federate.RegisterClass ).once().with( name, mock::any, publish, subscribe);
            builder.Build( federate, hlaClass );
            mock::sequence s;
            ::hla::MockAttributeFunctor visitor;
            BOOST_FOREACH( const std::string& attribute, attributes )
                MOCK_EXPECT( visitor.Visit ).once().in( s ).with( attribute );
            hlaClass.Apply( visitor );
        }
        MockFederate federate;
        ::hla::MockObjectRegistration< HlaObject_ABC > objectRegistration;
        ::hla::Class< HlaObject_ABC > hlaClass;
        std::vector< std::string > attributes;
    };
}

BOOST_FIXTURE_TEST_CASE( aggegate_entity_builder_registers_attributes, Fixture )
{
    AggregateEntityBuilder builder;
    attributes = boost::assign::list_of( "HLAprivilegeToDeleteObject" )
                                       ( "EntityType" )
                                       ( "EntityIdentifier" )
                                       ( "ForceIdentifier" )
                                       ( "AggregateMarking" )
                                       ( "AggregateState" )
                                       ( "Dimensions" )
                                       ( "Spatial" )
                                       ( "Formation" )
                                       ( "NumberOfSilentEntities" )
                                       ( "SilentEntities" )
                                       ( "SilentAggregates" )
                                       ( "SubAggregateIdentifiers" )
                                       ( "IsPartOf" )
                                       ( "NumberOfVariableDatums" )
                                       ( "RelativeSpatial" )
                                       ( "EntityIdentifiers" )
                                       ( "VariableDatums" );
    Check( builder, "BaseEntity.AggregateEntity", true, true );
}

BOOST_FIXTURE_TEST_CASE( netn_aggegate_entity_builder_registers_attributes, Fixture )
{
    NetnAggregateEntityBuilder builder;
    attributes = boost::assign::list_of( "HLAprivilegeToDeleteObject" )
                                       ( "EntityType" )
                                       ( "EntityIdentifier" )
                                       ( "ForceIdentifier" )
                                       ( "AggregateMarking" )
                                       ( "AggregateState" )
                                       ( "Dimensions" )
                                       ( "Spatial" )
                                       ( "Formation" )
                                       ( "NumberOfSilentEntities" )
                                       ( "SilentEntities" )
                                       ( "SilentAggregates" )
                                       ( "SubAggregateIdentifiers" )
                                       ( "IsPartOf" )
                                       ( "NumberOfVariableDatums" )
                                       ( "RelativeSpatial" )
                                       ( "EntityIdentifiers" )
                                       ( "VariableDatums" )
                                       // NETN
                                       ( "Mounted" )
                                       ( "Echelon" )
                                       ( "UniqueID" )
                                       ( "HigherHeadquarters" )
                                       ( "Callsign" )
                                       ( "Status" )
                                       ( "Symbol" )
                                       ( "EmbeddedUnitList" );
    Check( builder, "BaseEntity.AggregateEntity.NETN_Aggregate", true, true );
}

BOOST_FIXTURE_TEST_CASE( surface_vessel_builder_registers_attributes, Fixture )
{
    SurfaceVesselBuilder builder;
    attributes = boost::assign::list_of( "HLAprivilegeToDeleteObject" )
                                       ( "EntityType" )
                                       ( "EntityIdentifier" )
                                       ( "ForceIdentifier" )
                                       ( "Marking" )
                                       ( "Spatial" )
                                       ( "IsPartOf" )
                                       ( "DamageState" )
                                       ( "IsConcealed" );
    Check( builder, "BaseEntity.PhysicalEntity.Platform.SurfaceVessel", true, true );
}

BOOST_FIXTURE_TEST_CASE( netn_surface_vessel_builder_registers_attributes, Fixture )
{
    NetnSurfaceVesselBuilder builder;
    attributes = boost::assign::list_of( "HLAprivilegeToDeleteObject" )
                                       ( "EntityType" )
                                       ( "EntityIdentifier" )
                                       ( "ForceIdentifier" )
                                       ( "Marking" )
                                       ( "Spatial" )
                                       ( "IsPartOf" )
                                       ( "DamageState" )
                                       ( "IsConcealed" )
                                       // NETN
                                       ( "UniqueID" )
                                       ( "Callsign" )
                                       ( "EmbeddedUnitList" );
    Check( builder, "BaseEntity.PhysicalEntity.Platform.SurfaceVessel.NETN_SurfaceVessel", true, true );
}

BOOST_FIXTURE_TEST_CASE( aircraft_builder_registers_attributes, Fixture )
{
    AircraftBuilder builder;
    attributes = boost::assign::list_of( "HLAprivilegeToDeleteObject" )
                                       ( "EntityType" )
                                       ( "EntityIdentifier" )
                                       ( "ForceIdentifier" )
                                       ( "Marking" )
                                       ( "Spatial" )
                                       ( "IsPartOf" )
                                       ( "DamageState" )
                                       ( "IsConcealed" );
    Check( builder, "BaseEntity.PhysicalEntity.Platform.Aircraft", true, true );
}

BOOST_FIXTURE_TEST_CASE( netn_aircraft_builder_registers_attributes, Fixture )
{
    NetnAircraftBuilder builder;
    attributes = boost::assign::list_of( "HLAprivilegeToDeleteObject" )
                                       ( "EntityType" )
                                       ( "EntityIdentifier" )
                                       ( "ForceIdentifier" )
                                       ( "Marking" )
                                       ( "Spatial" )
                                       ( "IsPartOf" )
                                       ( "DamageState" )
                                       ( "IsConcealed" )
                                       // NETN
                                       ( "UniqueID" )
                                       ( "Callsign" )
                                       ( "EmbeddedUnitList" );
    Check( builder, "BaseEntity.PhysicalEntity.Platform.Aircraft.NETN_Aircraft", true, true );
}

BOOST_FIXTURE_TEST_CASE( groundvehicle_builder_registers_attributes, Fixture )
{
    GroundVehicleBuilder builder;
    attributes = boost::assign::list_of( "HLAprivilegeToDeleteObject" )
                                       ( "EntityType" )
                                       ( "EntityIdentifier" )
                                       ( "ForceIdentifier" )
                                       ( "Marking" )
                                       ( "Spatial" )
                                       ( "IsPartOf" )
                                       ( "DamageState" )
                                       ( "IsConcealed" );
    Check( builder, "BaseEntity.PhysicalEntity.Platform.GroundVehicle", true, false );
}

BOOST_FIXTURE_TEST_CASE( netn_groundvehicle_builder_registers_attributes, Fixture )
{
    NetnGroundVehicleBuilder builder;
    attributes = boost::assign::list_of( "HLAprivilegeToDeleteObject" )
                                       ( "EntityType" )
                                       ( "EntityIdentifier" )
                                       ( "ForceIdentifier" )
                                       ( "Marking" )
                                       ( "Spatial" )
                                       ( "IsPartOf" )
                                       ( "DamageState" )
                                       ( "IsConcealed" )
                                       // NETN
                                       ( "UniqueID" )
                                       ( "Callsign" )
                                       ( "EmbeddedUnitList" );
    Check( builder, "BaseEntity.PhysicalEntity.Platform.GroundVehicle.NETN_GroundVehicle", true, false );
}

BOOST_FIXTURE_TEST_CASE( human_builder_registers_attributes, Fixture )
{
    HumanBuilder builder;
    attributes = boost::assign::list_of( "HLAprivilegeToDeleteObject" )
                                       ( "EntityType" )
                                       ( "EntityIdentifier" )
                                       ( "ForceIdentifier" )
                                       ( "Marking" )
                                       ( "Spatial" )
                                       ( "IsPartOf" )
                                       ( "DamageState" )
                                       ( "IsConcealed" );
    Check( builder, "BaseEntity.PhysicalEntity.Lifeform.Human", true, false );
}

BOOST_FIXTURE_TEST_CASE( netn_human_builder_registers_attributes, Fixture )
{
    NetnHumanBuilder builder;
    attributes = boost::assign::list_of( "HLAprivilegeToDeleteObject" )
                                       ( "EntityType" )
                                       ( "EntityIdentifier" )
                                       ( "ForceIdentifier" )
                                       ( "Marking" )
                                       ( "Spatial" )
                                       ( "IsPartOf" )
                                       ( "DamageState" )
                                       ( "IsConcealed" )
                                       // NETN
                                       ( "UniqueID" )
                                       ( "Callsign" );
    Check( builder, "BaseEntity.PhysicalEntity.Lifeform.Human.NETN_Human", true, false );
}

BOOST_FIXTURE_TEST_CASE( minefield_builder_registers_attributes, Fixture )
{
    MinefieldBuilder builder;
    attributes = boost::assign::list_of ( "HLAprivilegeToDeleteObject" )
                                        ( "ActiveStatus" )
                                        ( "ForceIdentifier" )
                                        ( "MinefieldIdentifier" )
                                        ( "MinefieldLocation" )
                                        ( "MinefieldOrientation" )
                                        ( "MinefieldType" )
                                        ( "MineTypes" )
                                        ( "PerimeterPointCoordinates" )
                                        ( "ProtocolMode" )
                                        ( "State" );
    Check( builder, "Minefield", true, true );
}

BOOST_FIXTURE_TEST_CASE( culturefeature_builder_registers_attributes, Fixture )
{
    CulturalFeaturedBuilder builder;
    attributes = boost::assign::list_of ( "HLAprivilegeToDeleteObject" )
                                        ( "EntityType" )
                                        ( "EntityIdentifier" )
                                        ( "ForceIdentifier" )
                                        ( "Marking" )
                                        ( "Spatial" );
    Check( builder, "BaseEntity.PhysicalEntity.CulturalFeature", true, true );
}

BOOST_FIXTURE_TEST_CASE( otherpointobject_builder_registers_attributes, Fixture )
{
    OtherPointObjectBuilder builder;
    attributes = boost::assign::list_of ( "HLAprivilegeToDeleteObject" )
                                        ( "ObjectIdentifier" )
                                        ( "ReferencedObjectIdentifier" )
                                        ( "ForceIdentifier" )
                                        ( "ObjectType" )
                                        ( "Flaming" )
                                        ( "Smoking" )
                                        ( "Deactivated" )
                                        ( "ObjectPreDistributed" )
                                        ( "PercentComplete" )
                                        ( "DamagedAppearance" )
                                        ( "Orientation" )
                                        ( "Location" );
    Check( builder, "EnvironmentObject.PointObject.OtherPointObject", true, true );
}

BOOST_FIXTURE_TEST_CASE( breachablepointobject_builder_registers_attributes, Fixture )
{
    BreachablePointObjectBuilder builder;
    attributes = boost::assign::list_of ( "HLAprivilegeToDeleteObject" )
                                        ( "ObjectIdentifier" )
                                        ( "ReferencedObjectIdentifier" )
                                        ( "ForceIdentifier" )
                                        ( "ObjectType" )
                                        ( "Flaming" )
                                        ( "Smoking" )
                                        ( "Deactivated" )
                                        ( "ObjectPreDistributed" )
                                        ( "PercentComplete" )
                                        ( "DamagedAppearance" )
                                        ( "Orientation" )
                                        ( "Location" )
                                        ( "BreachedStatus" );
    Check( builder, "EnvironmentObject.PointObject.BreachablePointObject", true, true );
}

BOOST_FIXTURE_TEST_CASE( otherarealobject_builder_registers_attributes, Fixture )
{
    OtherArealObjectBuilder builder;
    attributes = boost::assign::list_of ( "HLAprivilegeToDeleteObject" )
                                        ( "ObjectIdentifier" )
                                        ( "ReferencedObjectIdentifier" )
                                        ( "ForceIdentifier" )
                                        ( "ObjectType" )
                                        ( "Flaming" )
                                        ( "Smoking" )
                                        ( "Deactivated" )
                                        ( "ObjectPreDistributed" )
                                        ( "PercentComplete" )
                                        ( "DamagedAppearance" )
                                        ( "PointsData" );
    Check( builder, "EnvironmentObject.ArealObject.OtherArealObject", true, true );
}

BOOST_FIXTURE_TEST_CASE( minefieldobject_builder_registers_attributes, Fixture )
{
    MinefieldObjectBuilder builder;
    attributes = boost::assign::list_of ( "HLAprivilegeToDeleteObject" )
                                        ( "ObjectIdentifier" )
                                        ( "ReferencedObjectIdentifier" )
                                        ( "ForceIdentifier" )
                                        ( "ObjectType" )
                                        ( "Flaming" )
                                        ( "Smoking" )
                                        ( "Deactivated" )
                                        ( "ObjectPreDistributed" )
                                        ( "PercentComplete" )
                                        ( "DamagedAppearance" )
                                        ( "PointsData" )
                                        ( "MineCount" )
                                        ( "BreachedStatus" );
    Check( builder, "EnvironmentObject.ArealObject.MinefieldObject", true, true );
}

BOOST_FIXTURE_TEST_CASE( breachablelinearobject_builder_registers_attributes, Fixture )
{
    BreachableLinearObjectBuilder builder;
    attributes = boost::assign::list_of ( "HLAprivilegeToDeleteObject" )
                                        ( "ObjectIdentifier" )
                                        ( "ReferencedObjectIdentifier" )
                                        ( "ForceIdentifier" )
                                        ( "ObjectType" )
                                        ( "SegmentRecords" );
    Check( builder, "EnvironmentObject.LinearObject.BreachableLinearObject", true, true );
}

BOOST_FIXTURE_TEST_CASE( rawdatahazardcontourgroup_builder_registers_attributes, Fixture )
{
    RawDataHazardContourGroupBuilder builder;
    attributes = boost::assign::list_of ( "HLAprivilegeToDeleteObject" )
                                        ( "Time" )
                                        ( "Material" )
                                        ( "HazardType" )
                                        ( "Contours" );
    Check( builder, "RawDataHazardContourGroup", true, true );
}

BOOST_FIXTURE_TEST_CASE( ATP45hazardarea_builder_registers_attributes, Fixture )
{
    ATP45HazardAreaBuilder builder;
    attributes = boost::assign::list_of ( "HLAprivilegeToDeleteObject" )
                                        ( "Locations" )
                                        ( "ATP45HazardAreaType" )
                                        ( "AgentClass" );
    Check( builder, "ATP45HazardArea", true, true );
}
