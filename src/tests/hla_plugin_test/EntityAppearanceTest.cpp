// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "rpr/EntityAppearance.h"

BOOST_AUTO_TEST_CASE( set_entity_appearance_land_damage )
{
    {
        rpr::EntityAppearance_Land app;
        app.SetDamage(3);
        BOOST_CHECK_EQUAL(app.value_, 0x18u);
        BOOST_CHECK_EQUAL(app.Damage(), 0x3u);
    }

    {
        rpr::EntityAppearance_Land app(0xFFFFFFFF);
        app.SetDamage(2);
        BOOST_CHECK_EQUAL(app.value_, 0xFFFFFFF7u);
        BOOST_CHECK_EQUAL(app.Damage(), 0x2u);
    }

    {
        rpr::EntityAppearance_Land app(0xFFFFFFFF);
        app.SetDamage(1);
        BOOST_CHECK_EQUAL(app.value_, 0xFFFFFFEFu);
        BOOST_CHECK_EQUAL(app.Damage(), 0x1u);
    }

    {
        rpr::EntityAppearance_Land app(0xFFFFFFFF);
        app.SetDamage(0);
        BOOST_CHECK_EQUAL(app.value_, 0xFFFFFFE7u);
        BOOST_CHECK_EQUAL(app.Damage(), 0x0u);
    }
}

BOOST_AUTO_TEST_CASE( set_entity_appearance_land_masked)
{
    {
        rpr::EntityAppearance_Land app;
        app.SetMasked(1);
        BOOST_CHECK_EQUAL(app.value_, 0x80000000u);
        BOOST_CHECK_EQUAL(app.Masked(), 0x1u);
    }
    {
        rpr::EntityAppearance_Land app(0xFFFFFFFF);
        app.SetMasked(0);
        BOOST_CHECK_EQUAL(app.value_, 0x7FFFFFFFu);
        BOOST_CHECK_EQUAL(app.Masked(), 0x0u);
    }
}
