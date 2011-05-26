// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/Checkpoints/MIL_CheckPointInArchive.h"
#include "simulation_kernel/Checkpoints/MIL_CheckPointOutArchive.h"
#include "simulation_kernel/Entities/Agents/Units/Humans/PHY_Human.h"
#include "simulation_kernel/Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "simulation_kernel/Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "simulation_kernel/Entities/Objects/MIL_NbcAgentType.h"
#include "simulation_kernel/Entities/Objects/MIL_ToxicEffectManipulator.h"
#include "MockHumansComposante.h"
#include "StubMIL_Time_ABC.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    class Configuration
    {
    public :
        Configuration()
        {
            MOCK_EXPECT( composante, NotifyHumanAdded ).once();
            MOCK_EXPECT( composante, NotifyHumanRemoved ).once();
        }
        StubMIL_Time_ABC time;
        MockHumansComposante composante;
    };
    class Fixture : public Configuration
    {
    public:
        Fixture()
            : human( time, composante )
        {
            // NOTHING
        }
        PHY_Human human;
    };
}

// -----------------------------------------------------------------------------
// Name: HumanTest
// Created: MGD 2010-03-15
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( HumanMagicWound, Fixture )
{
    BOOST_CHECK_EQUAL( &human.GetWound(), &PHY_HumanWound::notWounded_ );
    //Return false if same level
    BOOST_CHECK( !human.SetWound( PHY_HumanWound::notWounded_ ) );
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    BOOST_CHECK( human.SetWound( PHY_HumanWound::woundedU1_ ) );
    BOOST_CHECK_EQUAL( &human.GetWound(), &PHY_HumanWound::woundedU1_ );
}

BOOST_FIXTURE_TEST_CASE( HumanMagicHeal, Fixture )
{
    //normal heal
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    human.SetWound( PHY_HumanWound::woundedU1_ );
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    human.Heal();
    BOOST_CHECK_EQUAL( &human.GetWound(), &PHY_HumanWound::notWounded_ );
    //can resurrect
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    human.SetWound( PHY_HumanWound::killed_ );
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    human.Heal();
    BOOST_CHECK_EQUAL( &human.GetWound(), &PHY_HumanWound::notWounded_ );
}

BOOST_FIXTURE_TEST_CASE( HumanNormalWound, Fixture )
{
    //normal wound
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    BOOST_CHECK( human.ApplyWound( PHY_HumanWound::woundedU2_ ) );
    BOOST_CHECK_EQUAL( &human.GetWound(), &PHY_HumanWound::woundedU2_ );
    //try to set a lesser wound
    BOOST_CHECK( !human.ApplyWound( PHY_HumanWound::woundedU3_ ) );
    BOOST_CHECK_EQUAL( &human.GetWound(), &PHY_HumanWound::woundedU2_ );
    //try to wound a man in logistic  $$$$ TODO MGD
    //MIL_AutomateLOG tc2;
    //human.Evacuate( tc2 );
    //BOOST_CHECK( !human.ApplyWound( PHY_HumanWound::woundedU2_ ) );
    //try to wound a dead man
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    BOOST_CHECK( human.ApplyWound( PHY_HumanWound::killed_ ) );
    BOOST_CHECK( !human.ApplyWound( PHY_HumanWound::woundedU2_ ) );
}

BOOST_FIXTURE_TEST_CASE( HumanNormalHeal, Fixture )
{
    //normal heal
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    human.SetWound( PHY_HumanWound::woundedU1_ );
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    human.HealWound();
    BOOST_CHECK_EQUAL( &human.GetWound(), &PHY_HumanWound::notWounded_ );
    //can't resurrect
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    human.SetWound( PHY_HumanWound::killed_ );
    human.HealWound();
    BOOST_CHECK_EQUAL( &human.GetWound(), &PHY_HumanWound::killed_ );
}

BOOST_FIXTURE_TEST_CASE( HumanApplyPoisonous, Fixture )
{
    xml::xistringstream xis( "<nbc>"
                             "   <propagation wind-speed-limit='5' contamination-distance='300' contamination-quantity-given='0.5' />"
                             "   <nbc-suit max-speed-modifier='0.4' reloading-time-modifier='1.5'/>"
                             "   <agents>"
                             "       <agent id='1' name='Biologique'>"
                             "           <effects intoxication='true' type='liquid'>"
                             "               <effect percentage='0' wound='nonblesse'/>"
                             "               <effect percentage='0.2' wound='u1'/>"
                             "               <effect percentage='0.15' wound='u2'/>"
                             "               <effect percentage='0.25' wound='u3'/>"
                             "               <effect percentage='0.2' wound='ue'/>"
                             "               <effect percentage='0.2' wound='mort'/>"
                             "           </effects>"
                             "       </agent>"
                             "   </agents>"
                             "</nbc>" );
    std::vector< const MIL_NbcAgentType* > nbcTypes;
    //MIL_NbcAgentType::Initialize( xis );
    //nbcTypes.push_back( MIL_NbcAgentType::Find( 1 ) );
    //MIL_ToxicEffectManipulator nbcAgent( nbcTypes, 1 );
    //human.ApplyPoisonous( nbcAgent ); // $$$$ TODO MGD Remove singleton
    //BOOST_CHECK( human.IsWounded() );
}

BOOST_FIXTURE_TEST_CASE( HumanContaminationManagement, Fixture )
{
    BOOST_CHECK( !human.IsContaminated() );
    std::vector< const MIL_NbcAgentType* > nbcTypes;
    MIL_ToxicEffectManipulator nbcAgent( nbcTypes, 1 );
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    human.ApplyContamination( nbcAgent );
    BOOST_CHECK( human.IsContaminated() );
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    human.HealContamination();
    BOOST_CHECK( !human.IsContaminated() );
}

BOOST_FIXTURE_TEST_CASE( HumanMentalDiseaseManagement, Fixture )
{
    BOOST_CHECK( !human.IsMentalDiseased() );
    human.ApplyMentalDisease();
    //BOOST_CHECK( human.IsMentalDiseased() ); // $$$$ TODO MGd see why random, update interface corresponding to
    human.HealMentalDisease();
    BOOST_CHECK( !human.IsMentalDiseased() );
}

BOOST_FIXTURE_TEST_CASE( HumanSetRank, Fixture )
{
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    BOOST_CHECK( human.SetRank( PHY_HumanRank::sousOfficier_ ) );
    BOOST_CHECK_EQUAL( &human.GetRank(), &PHY_HumanRank::sousOfficier_ );
    BOOST_CHECK( !human.SetRank( PHY_HumanRank::sousOfficier_ ) );
    //Add logistic notifycation test
}

BOOST_AUTO_TEST_CASE( HumanCancelLogisticRequest )
{
  /* $$$$ TODO MGD
  StubMIL_Time_ABC time;
  PHY_HumansComposante composante;
  PHY_Human human = PHY_Human( time, composante);
  PHY_MedicalHumanState medicalState;
  SetMedicalState( &medicalState );
  human.CancelLogisticRequest();
  BOOST_CHECK_EQUAL( E_Location::eBattleField, human.GetLocation() );
  human.NotifyComposanteHandledByMaintenance();
  BOOST_CHECK_EQUAL( E_Location::eMaintenance, human.GetLocation() );
  human.CancelLogisticRequest();
  BOOST_CHECK_EQUAL( E_Location::eMaintenance, human.GetLocation() );
*/
  //Add component notifycation test
  //Add pion back from medical log notifycation test
  //cancel medical state
}

BOOST_FIXTURE_TEST_CASE( HumanIsUsable, Fixture )
{
    //PHY_MedicalHumanState medicalState;
    BOOST_CHECK( human.IsUsable() );
    BOOST_CHECK( !human.IsDead() );
    BOOST_CHECK( !human.IsWounded() );
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    human.ApplyWound( PHY_HumanWound::woundedU1_ );
    BOOST_CHECK( human.IsUsable() );//  $$$$ TODO MGD Why? we can use a wounded man
    BOOST_CHECK( !human.IsDead() );
    BOOST_CHECK( human.IsWounded() );
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    human.ApplyWound( PHY_HumanWound::killed_ );
    BOOST_CHECK( !human.IsUsable() );
    BOOST_CHECK( human.IsDead() );
    BOOST_CHECK( !human.IsWounded() );
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    human.Heal();
    BOOST_CHECK( human.IsUsable() );
    //SetMedicalState( &medicalState );
    //BOOST_CHECK( !human.IsUsable() );
}

BOOST_FIXTURE_TEST_CASE( HumanIsAnEmergency, Fixture )
{
    BOOST_CHECK( !human.IsAnEmergency() );
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    human.SetWound( PHY_HumanWound::woundedU1_ );
    BOOST_CHECK( human.IsAnEmergency() );
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    human.SetWound( PHY_HumanWound::killed_ );
    BOOST_CHECK( !human.IsAnEmergency() );
}

BOOST_FIXTURE_TEST_CASE( HumanUpdate, Fixture )
{
    human.Update();
    //Add Notification need medical test
    //time.Add( time + delta mort );
    //human.Update();
    //Add MIL_Report report send test
}

BOOST_FIXTURE_TEST_CASE( HumanNeedEvacuation, Fixture )
{
    //PHY_MedicalHumanState medicalState;
    BOOST_CHECK( !human.NeedEvacuation() );
    BOOST_CHECK( !human.NeedMedical() );
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    human.SetWound( PHY_HumanWound::woundedU1_ );
    BOOST_CHECK( human.NeedEvacuation() );
    BOOST_CHECK( human.NeedMedical() );
    //human.SetMedicalState( &mediacalState );
    //BOOST_CHECK( !human.NeedEvacuation() );
    //BOOST_CHECK( human.NeedMedical() );
}

BOOST_FIXTURE_TEST_CASE( HumanNeedEvacuate, Fixture )
{
    //MIL_AutomateLOG tc2;
    //human.Evacuate( tc2 );
    //Add notification test
    //See why in this interface
}

BOOST_FIXTURE_TEST_CASE( HumanNotifyHandledByMedical, Fixture )
{
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    human.NotifyHandledByMedical();
    BOOST_CHECK_EQUAL( human.GetLocation(), PHY_Human::eMedical );
}

BOOST_FIXTURE_TEST_CASE( HumanNotifyBackToWar, Fixture )
{
    //human.NotifyBackToWar(); // $$$$ TODO MGD need PHY_MedicalHumanState to be set
    //Check notification to component and event
}

BOOST_FIXTURE_TEST_CASE( HumanMaintenanceManagement, Fixture )
{
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    human.NotifyComposanteHandledByMaintenance();
    BOOST_CHECK_EQUAL( human.GetLocation(), PHY_Human::eMaintenance );
    MOCK_EXPECT( composante, NotifyHumanChanged ).once();
    human.NotifyComposanteBackFromMaintenance();
    BOOST_CHECK_EQUAL( human.GetLocation(), PHY_Human::eBattleField );
}

// $$$$ TODO MGD find a  way to serialize MockHumansComposante
//BOOST_AUTO_TEST_CASE( HumanSerialization )
//{
//    StubMIL_Time_ABC time;
//    MockHumansComposante composante;
//    MOCK_EXPECT( composante, NotifyHumanAdded );
//    MOCK_EXPECT( composante, NotifyHumanRemoved );
//    PHY_Human humanInitial = PHY_Human( time, composante);
//    MOCK_EXPECT( composante, NotifyHumanChanged );
//    humanInitial.SetWound( PHY_HumanWound::killed_ );
//
//    MockHumansComposante composante2;
//    MOCK_EXPECT( composante2, NotifyHumanAdded );
//    MOCK_EXPECT( composante2, NotifyHumanRemoved );
//    PHY_Human humanRestored = PHY_Human( time, composante2);
//
//    std::stringstream stringstream;
//    {
//        MIL_CheckPointOutArchive outStream( stringstream );
//        outStream << humanInitial;
//    }
//    {
//        MIL_CheckPointInArchive inStream( stringstream );
//        inStream >> humanRestored;
//    }
//    BOOST_CHECK( humanRestored.IsDead() );
//}
