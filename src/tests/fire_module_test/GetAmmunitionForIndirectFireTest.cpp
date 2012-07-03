// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "fire_module_test_pch.h"

BOOST_FIXTURE_TEST_CASE( get_ammunition_for_indirect_fire_returns_no_ammunition_when_firer_has_no_component, sword::fire::ModuleFixture )
{
    BOOST_CHECK( ! GetAmmunitionForIndirectFire( core::Convert( &model ), firer, "explosion", MT_Vector2D() ) );
}

BOOST_FIXTURE_TEST_CASE( get_ammunition_for_indirect_fire_returns_no_ammunition_when_firer_has_no_weapon, sword::fire::ModuleFixture )
{
    entity[ "components" ].AddElement()[ "weapons" ];
    BOOST_CHECK( ! GetAmmunitionForIndirectFire( core::Convert( &model ), firer, "explosion", MT_Vector2D() ) );
}

BOOST_FIXTURE_TEST_CASE( get_ammunition_for_indirect_fire_returns_no_ammunition_when_weapon_type_does_not_exist, sword::fire::ModuleFixture )
{
    entity[ "components" ].AddElement()[ "weapons" ].AddElement()[ "type" ] = "non existing weapon";
    entity[ "movement/position/x" ] = 0;
    entity[ "movement/position/y" ] = 0;
    MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_ERROR, std::string( "Unknown weapon type : non existing weapon" ) );
    BOOST_CHECK( ! GetAmmunitionForIndirectFire( core::Convert( &model ), firer, "explosion", MT_Vector2D() ) );
}

BOOST_FIXTURE_TEST_CASE( get_ammunition_for_indirect_fire_returns_no_ammunition_when_no_weapon_can_indirect_fire, sword::fire::ModuleFixture )
{
    entity[ "components" ].AddElement()[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_1";
    entity[ "movement/position/x" ] = 0;
    entity[ "movement/position/y" ] = 0;
    BOOST_CHECK( ! GetAmmunitionForIndirectFire( core::Convert( &model ), firer, "explosion", MT_Vector2D() ) );
}

BOOST_FIXTURE_TEST_CASE( get_ammunition_for_indirect_fire_returns_no_ammunition_when_component_cannot_fire, sword::fire::ModuleFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_2";
    entity[ "movement/position/x" ] = 0;
    entity[ "movement/position/y" ] = 0;
    MOCK_EXPECT( CanComponentFire ).once().with( firer, core::Convert( &component_1 ) ).returns( false );
    BOOST_CHECK( ! GetAmmunitionForIndirectFire( core::Convert( &model ), firer, "explosion", MT_Vector2D() ) );
}

BOOST_FIXTURE_TEST_CASE( get_ammunition_for_indirect_fire_returns_no_ammunition_when_target_position_is_too_close, sword::fire::ModuleFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_2";
    entity[ "movement/position/x" ] = 0;
    entity[ "movement/position/y" ] = 0;
    MOCK_EXPECT( CanComponentFire ).once().with( firer, core::Convert( &component_1 ) ).returns( true );
    BOOST_CHECK( ! GetAmmunitionForIndirectFire( core::Convert( &model ), firer, "explosion", MT_Vector2D() ) );
}

BOOST_FIXTURE_TEST_CASE( get_ammunition_for_indirect_fire_returns_no_ammunition_when_type_does_not_match, sword::fire::ModuleFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_2";
    entity[ "movement/position/x" ] = 0;
    entity[ "movement/position/y" ] = 2500;
    BOOST_CHECK( ! GetAmmunitionForIndirectFire( core::Convert( &model ), firer, "non matching type", MT_Vector2D() ) );
}

BOOST_FIXTURE_TEST_CASE( get_ammunition_for_indirect_fire_returns_no_ammunition_when_target_position_is_too_far, sword::fire::ModuleFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_2";
    entity[ "movement/position/x" ] = 0;
    entity[ "movement/position/y" ] = 6000;
    MOCK_EXPECT( CanComponentFire ).once().with( firer, core::Convert( &component_1 ) ).returns( true );
    BOOST_CHECK( ! GetAmmunitionForIndirectFire( core::Convert( &model ), firer, "explosion", MT_Vector2D() ) );
}

BOOST_FIXTURE_TEST_CASE( get_ammunition_for_indirect_fire_returns_ammunition, sword::fire::ModuleFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_2";
    entity[ "movement/position/x" ] = 0;
    entity[ "movement/position/y" ] = 2500;
    MOCK_EXPECT( CanComponentFire ).once().with( firer, core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( GetDotationValue ).once().with( firer, ammo_2 ).returns( 7 );
    BOOST_CHECK_EQUAL( ammo_2, GetAmmunitionForIndirectFire( core::Convert( &model ), firer, "explosion", MT_Vector2D() ) );
}

BOOST_FIXTURE_TEST_CASE( get_ammunition_for_indirect_fire_returns_ammunition_when_comparing_types_case_insensitively, sword::fire::ModuleFixture )
{
    core::Model& component_1 = entity[ "components" ].AddElement();
    component_1[ "weapons" ].AddElement()[ "type" ] = "launcher_1/ammo_2";
    entity[ "movement/position/x" ] = 0;
    entity[ "movement/position/y" ] = 2500;
    MOCK_EXPECT( CanComponentFire ).once().with( firer, core::Convert( &component_1 ) ).returns( true );
    MOCK_EXPECT( GetDotationValue ).once().with( firer, ammo_2 ).returns( 7 );
    BOOST_CHECK_EQUAL( ammo_2, GetAmmunitionForIndirectFire( core::Convert( &model ), firer, "EXPLOSION", MT_Vector2D() ) );
}
