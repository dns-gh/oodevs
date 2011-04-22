// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/Entities/Objects/MedicalCapacity.h"
#include "simulation_kernel/Entities/Objects/MedicalTreatmentAttribute.h"
#include "MockMIL_Object_ABC.h"
#include "MockMIL_Injury_ABC.h"
#include "MockMIL_Time_ABC.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: VerifyMedicalCapacity
// Created: LDC 2010-06-30
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyMedicalCapacity )
{
    MockMIL_Time_ABC time;
    MOCK_EXPECT( time, GetTickDuration ).returns( 10 );
    MOCK_EXPECT( time, GetCurrentTick ).returns( 1 );
    {
        xml::xistringstream xisMedical( "<medical-treatments>"
            "<medical-treatment id='0' name='Wound' death-threshold='95'>"
                "<injuries>"
                   "<injury category='UA' treatment-time='5h' hospitalisation-time='72h' life-expectancy='2h' threshold='80'/>"
                 "</injuries>"
            "</medical-treatment>"
            "<medical-treatment id='1' name='Wound2' death-threshold='95'>"
              "<injuries>"
                   "<injury category='UA' treatment-time='5h' hospitalisation-time='72h' life-expectancy='2h' threshold='80'/>"
                 "</injuries>"
            "</medical-treatment>"
        "</medical-treatments>" );
        MIL_MedicalTreatmentType::Initialize( xisMedical, time );
        MockMIL_Object_ABC object;
        MedicalCapacity capacity;
        xml::xistringstream xis( "<medical-treatment doctors='3' reference='d'>"
                  "<bed-capacities>"
                    "<bed-capacity baseline='1' type='Wound2'/>"
                  "</bed-capacities>"
                "</medical-treatment>" );

        xis >> xml::start( "medical-treatment" );
        MedicalTreatmentAttribute attribute( xis );
        MOCK_EXPECT( object, RegisterAttribute ).once();
        object.GetAttribute< MedicalTreatmentAttribute >() = attribute;
        sword::ObjectAttributes asn;
        object.GetAttribute< MedicalTreatmentAttribute >().SendFullState( asn );
        BOOST_CHECK_EQUAL( 1, asn.medical_treatment().bed_capacities().size() );

        int woundNumber = 1;
        BOOST_CHECK( asn.medical_treatment().bed_capacities( 0 ).has_type_id() );
        BOOST_CHECK_EQUAL( woundNumber, asn.medical_treatment().bed_capacities( 0 ).type_id() );
        BOOST_CHECK( asn.medical_treatment().bed_capacities( 0 ).has_available_count() );
        BOOST_CHECK_EQUAL( 1u, asn.medical_treatment().bed_capacities( 0 ).available_count() );

        MOCK_EXPECT( time, GetCurrentTick ).returns( 2 );
        {
            PHY_InjuredHuman injuredHuman;
            capacity.ReceivePatient( injuredHuman );
            capacity.Update( object, 1 );
            sword::ObjectAttributes asnAfter;
            object.GetAttribute< MedicalTreatmentAttribute >().SendFullState( asnAfter );
            BOOST_CHECK_EQUAL( woundNumber, asn.medical_treatment().bed_capacities( 0 ).type_id() );
            BOOST_CHECK_EQUAL( 1u, asn.medical_treatment().bed_capacities( 0 ).available_count() );
        }
        {
            Mock_MIL_Injury_ABC abstractInjury;
            MOCK_EXPECT( abstractInjury, GetInjuryID ).returns( 1 );
            MOCK_EXPECT( abstractInjury, SetLifeExpectancy ).returns( 42.f );
            MOCK_EXPECT( abstractInjury, GetInjuryCategory ).returns( MIL_MedicalTreatmentType::eUA );
            PHY_InjuredHuman injuredHuman( abstractInjury );
            capacity.ReceivePatient( injuredHuman );
            capacity.Update( object, 1 );
            sword::ObjectAttributes asnAfter;
            object.GetAttribute< MedicalTreatmentAttribute >().SendFullState( asnAfter );
            BOOST_CHECK_EQUAL( woundNumber, asnAfter.medical_treatment().bed_capacities( 0 ).type_id() );
            BOOST_CHECK_EQUAL( 0u, asnAfter.medical_treatment().bed_capacities( 0 ).available_count() );
        }
    }
    MIL_MedicalTreatmentType::Terminate();
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2010-07-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyInitialization )
{
    MockMIL_Time_ABC time;
    MOCK_EXPECT( time, GetTickDuration ).returns( 10 );
    MOCK_EXPECT( time, GetCurrentTick ).returns( 1 );
    {
        xml::xistringstream xisMedical( "<medical-treatments>"
            "<medical-treatment id='0' name='Wound1' death-threshold='95'>"
              "<injuries>"
               "<injury category='UA' treatment-time='5h' hospitalisation-time='72h' life-expectancy='2h' threshold='80'/>"
              "</injuries>"
            "</medical-treatment>"
            "<medical-treatment id='1' name='Wound2' death-threshold='95'>"
              "<injuries>"
               "<injury category='UA' treatment-time='5h' hospitalisation-time='72h' life-expectancy='2h' threshold='80'/>"
              "</injuries>"
            "</medical-treatment>"
        "</medical-treatments>" );
        MIL_MedicalTreatmentType::Initialize( xisMedical, time );

        MockMIL_Object_ABC object;
        MOCK_EXPECT( object, RegisterAttribute ).once();

        MedicalCapacity capacity;
        xml::xistringstream xis( "<medical-treatment doctors='3' reference='d'>"
                  "<bed-capacities>"
                    "<bed-capacity baseline='5' type='Wound2'/>"
                  "</bed-capacities>"
                "</medical-treatment>" );
        xis >> xml::start( "medical-treatment" );
        MedicalTreatmentAttribute attribute( xis );
        object.GetAttribute< MedicalTreatmentAttribute >() = attribute;
        sword::ObjectAttributes asn;
        object.GetAttribute< MedicalTreatmentAttribute >().SendFullState( asn );
        BOOST_CHECK_EQUAL( 3u, asn.medical_treatment().doctors() );
        BOOST_CHECK_EQUAL( 1 /*id Wound1*/, asn.medical_treatment().bed_capacities( 0 ).type_id() );
        BOOST_CHECK( asn.medical_treatment().bed_capacities( 0 ).has_available_count() );
        BOOST_CHECK_EQUAL( 5u, asn.medical_treatment().bed_capacities( 0 ).available_count() );
        BOOST_CHECK( asn.medical_treatment().bed_capacities( 0 ).has_baseline_count() );
        BOOST_CHECK_EQUAL( 5u, asn.medical_treatment().bed_capacities( 0 ).baseline_count() );
    }
    MIL_MedicalTreatmentType::Terminate();
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2010-07-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyMessageOnUpdate )
{
    MockMIL_Time_ABC time;
    MOCK_EXPECT( time, GetTickDuration ).returns( 10 );
    MOCK_EXPECT( time, GetCurrentTick ).returns( 1 );
    {
        xml::xistringstream xisMedical( "<medical-treatments>"
            "<medical-treatment id='0' name='Wound1' death-threshold='95'>"
              "<injuries>"
               "<injury category='UA' treatment-time='5h' hospitalisation-time='72h' life-expectancy='2h' threshold='80'/>"
              "</injuries>"
            "</medical-treatment>"
            "<medical-treatment id='1' name='Wound2' death-threshold='95'>"
              "<injuries>"
               "<injury category='UA' treatment-time='5h' hospitalisation-time='72h' life-expectancy='2h' threshold='80'/>"
              "</injuries>"
            "</medical-treatment>"
        "</medical-treatments>" );
        MIL_MedicalTreatmentType::Initialize( xisMedical, time );

        MockMIL_Object_ABC object;
        MOCK_EXPECT( object, RegisterAttribute ).once();

        MedicalCapacity capacity;
        xml::xistringstream xis( "<medical-treatment doctors='3' reference='d'>"
                  "<bed-capacities>"
                    "<bed-capacity baseline='5' type='Wound2'/>"
                  "</bed-capacities>"
                "</medical-treatment>" );
        xis >> xml::start( "medical-treatment" );
        MedicalTreatmentAttribute attribute( xis );
        object.GetAttribute< MedicalTreatmentAttribute >() = attribute;

        // Update
        sword::MissionParameter_Value parameters;
        parameters.add_list(); // attributeId
        parameters.add_list(); // External ref
        parameters.add_list()->set_quantity( 5 ); // doctors
        parameters.add_list(); // eStatus
        sword::MissionParameter_Value& bed_capacities = *parameters.add_list();
        {
            sword::MissionParameter_Value& bed = *bed_capacities.add_list();
            bed.add_list()->set_identifier( 0 );
            bed.add_list()->set_quantity( 3 ); // baseline
            bed.add_list()->set_quantity( 2 ); // available
            bed.add_list(); // emergency count
            // bed.add_list()->set_quantity( 3 );
        }
        {
            sword::MissionParameter_Value& bed = *bed_capacities.add_list();
            bed.add_list()->set_identifier( 1 );
            bed.add_list()->set_quantity( 5 );
            bed.add_list()->set_quantity( 3 );
            bed.add_list(); // emergency count
            // bed.add_list()->set_quantity( 3 );
        }

        object.GetAttribute< MedicalTreatmentAttribute >().OnUpdate( parameters );

        // Validate
        sword::ObjectAttributes asn;
        object.GetAttribute< MedicalTreatmentAttribute >().SendFullState( asn );
        BOOST_CHECK_EQUAL( 5u, asn.medical_treatment().doctors() );

        BOOST_CHECK_EQUAL( 0, asn.medical_treatment().bed_capacities( 0 /*id Wound1*/ ).type_id() );
        BOOST_CHECK( asn.medical_treatment().bed_capacities( 0 /*id Wound1*/ ).has_baseline_count() );
        BOOST_CHECK_EQUAL( 3u, asn.medical_treatment().bed_capacities( 0 /*id Wound1*/ ).baseline_count() );
        BOOST_CHECK( asn.medical_treatment().bed_capacities( 0 /*id Wound1*/ ).has_available_count() );
        BOOST_CHECK_EQUAL( 2u, asn.medical_treatment().bed_capacities( 0 /*id Wound1*/ ).available_count() );

        BOOST_CHECK_EQUAL( 1, asn.medical_treatment().bed_capacities( 1 /*id Wound1*/ ).type_id() );
        BOOST_CHECK( asn.medical_treatment().bed_capacities( 1 /*id Wound1*/ ).has_baseline_count() );
        BOOST_CHECK_EQUAL( 5u, asn.medical_treatment().bed_capacities( 1 /*id Wound1*/ ).baseline_count() );
        BOOST_CHECK( asn.medical_treatment().bed_capacities( 1 /*id Wound1*/ ).has_available_count() );
        BOOST_CHECK_EQUAL( 3u, asn.medical_treatment().bed_capacities( 1 /*id Wound2*/ ).available_count() );
    }
    MIL_MedicalTreatmentType::Terminate();
}
/*
// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2010-07-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyAsnUpdate )
{
    MockMIL_Time_ABC time;
    MOCK_EXPECT( time, GetTickDuration ).returns( 10 );
    MOCK_EXPECT( time, GetCurrentTick ).returns( 1 );
    {
        xml::xistringstream xisMedical( "<medical-treatments>"
            "<medical-treatment id='0' name='Wound1' death-threshold='95'>"
              "<injuries>"
               "<injury category='UA' treatment-time='5h' hospitalisation-time='72h' life-expectancy='2h' threshold='80'/>"
              "</injuries>"
            "</medical-treatment>"
            "<medical-treatment id='1' name='Wound2' death-threshold='95'>"
              "<injuries>"
               "<injury category='UA' treatment-time='5h' hospitalisation-time='72h' life-expectancy='2h' threshold='80'/>"
              "</injuries>"
            "</medical-treatment>"
        "</medical-treatments>" );
        MIL_MedicalTreatmentType::Initialize( xisMedical );

        MockMIL_Object_ABC object;
        MOCK_EXPECT( object, RegisterAttribute ).once();

        MedicalCapacity capacity;
        xml::xistringstream xis( "<medical-treatment doctors='3' reference='d'>"
                  "<bed-capacities>"
                    "<bed-capacity baseline='5' type='Wound2'/>"
                  "</bed-capacities>"
                "</medical-treatment>" );
        xis >> xml::start( "medical-treatment" );
        MedicalTreatmentAttribute attribute( xis );
        object.GetAttribute< MedicalTreatmentAttribute >() = attribute;

        // Update
        sword::ObjectAttributes asnUpdate;
        asnUpdate.mutable_medical_treatment()->set_doctors( 5 );
        {
            sword::MedicalTreatmentBedCapacity& capacity = *asnUpdate.mutable_medical_treatment()->add_bed_capacities();
            capacity.set_type_id( 0 );
            capacity.set_available_count( 3 );
            capacity.set_baseline_count( 5 );
        }
        {
            sword::MedicalTreatmentBedCapacity& capacity = *asnUpdate.mutable_medical_treatment()->add_bed_capacities();
            capacity.set_type_id( 1 );
            capacity.set_available_count( 5 );
            capacity.set_baseline_count( 3 );
        }

        object.GetAttribute< MedicalTreatmentAttribute >().Update( asnUpdate );

        // Validate
        sword::ObjectAttributes asn;
        object.GetAttribute< MedicalTreatmentAttribute >().SendFullState( asn );
    }
    MIL_MedicalTreatmentType::Terminate();
}*/
