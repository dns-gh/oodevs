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

#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: HumanTest
// Created: MGD 2010-03-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( HumanMagicWound )
{
    StubMIL_Time_ABC time;
    MockHumansComposante composante;
    MOCK_EXPECT( composante, NotifyHumanAdded );
    MOCK_EXPECT( composante, NotifyHumanRemoved );
    PHY_Human human = PHY_Human( time, composante);

    BOOST_CHECK_EQUAL( &human.GetWound(), &PHY_HumanWound::notWounded_ );
    
    //Return false if same level
    BOOST_CHECK( !human.SetWound( PHY_HumanWound::notWounded_ ) );

    MOCK_EXPECT( composante, NotifyHumanChanged );
    BOOST_CHECK( human.SetWound( PHY_HumanWound::woundedU1_ ) );
    MOCK_VERIFY( composante, NotifyHumanChanged );
    BOOST_CHECK_EQUAL( &human.GetWound(), &PHY_HumanWound::woundedU1_ );
}

BOOST_AUTO_TEST_CASE( HumanMagicHeal )
{
    StubMIL_Time_ABC time;
    MockHumansComposante composante;
    MOCK_EXPECT( composante, NotifyHumanAdded );
    MOCK_EXPECT( composante, NotifyHumanRemoved );
    PHY_Human human = PHY_Human( time, composante);

    //normal heal
    MOCK_EXPECT( composante, NotifyHumanChanged );
    human.SetWound( PHY_HumanWound::woundedU1_ );
    MOCK_EXPECT( composante, NotifyHumanChanged );
    human.Heal();
    BOOST_CHECK_EQUAL( &human.GetWound(), &PHY_HumanWound::notWounded_ );
  
    //can resurrect
    MOCK_EXPECT( composante, NotifyHumanChanged );
    human.SetWound( PHY_HumanWound::killed_ );
    MOCK_EXPECT( composante, NotifyHumanChanged );
    human.Heal();
    BOOST_CHECK_EQUAL( &human.GetWound(), &PHY_HumanWound::notWounded_ );

    MOCK_VERIFY( composante, NotifyHumanChanged );
}

BOOST_AUTO_TEST_CASE( HumanNormalWound )
{
    StubMIL_Time_ABC time;
    MockHumansComposante composante;
    MOCK_EXPECT( composante, NotifyHumanAdded );
    MOCK_EXPECT( composante, NotifyHumanRemoved );
    PHY_Human human = PHY_Human( time, composante);

    //normal wound
    MOCK_EXPECT( composante, NotifyHumanChanged );
    BOOST_CHECK( human.ApplyWound( PHY_HumanWound::woundedU2_ ) );
    BOOST_CHECK_EQUAL( &human.GetWound(), &PHY_HumanWound::woundedU2_ );

    //try to set a lesser wound
    BOOST_CHECK( !human.ApplyWound( PHY_HumanWound::woundedU3_ ) );
    BOOST_CHECK_EQUAL( &human.GetWound(), &PHY_HumanWound::woundedU2_ );

    //try to wound a man in logistic @TODO MGD
    //MIL_AutomateLOG tc2;
    //human.Evacuate( tc2 );
    //BOOST_CHECK( !human.ApplyWound( PHY_HumanWound::woundedU2_ ) );

    //try to wound a dead man
    BOOST_CHECK( human.ApplyWound( PHY_HumanWound::killed_ ) );
    BOOST_CHECK( !human.ApplyWound( PHY_HumanWound::woundedU2_ ) );

    MOCK_VERIFY( composante, NotifyHumanChanged );
}


BOOST_AUTO_TEST_CASE( HumanNormalHeal )
{
    StubMIL_Time_ABC time;
    MockHumansComposante composante;
    MOCK_EXPECT( composante, NotifyHumanAdded );
    MOCK_EXPECT( composante, NotifyHumanRemoved );
    PHY_Human human = PHY_Human( time, composante);

    //normal heal
    MOCK_EXPECT( composante, NotifyHumanChanged );
    human.SetWound( PHY_HumanWound::woundedU1_ );
    human.HealWound();
    BOOST_CHECK_EQUAL( &human.GetWound(), &PHY_HumanWound::notWounded_ );

    //can't resurrect
    human.SetWound( PHY_HumanWound::killed_ );
    human.HealWound();
    BOOST_CHECK_EQUAL( &human.GetWound(), &PHY_HumanWound::killed_ );

    MOCK_VERIFY( composante, NotifyHumanChanged );
}

BOOST_AUTO_TEST_CASE( HumanApplyPoisonous )
{
    StubMIL_Time_ABC time;
    MockHumansComposante composante;
    MOCK_EXPECT( composante, NotifyHumanAdded );
    MOCK_EXPECT( composante, NotifyHumanRemoved );
    PHY_Human human = PHY_Human( time, composante);

    xml::xistringstream xis( "<nbc>"
        "<propagation wind-speed-limit='5'/>"
        "<nbc-suit max-speed-modifier='0.4' reloading-time-modifier='1.5'/>"
        "<agents>"
        "<agent id='1' name='Biologique'>"
        "<effects affliction='intoxication' type='liquid'>"
        "<effect percentage='0' wound='nonblesse'/>"
        "<effect percentage='0.2' wound='u1'/>"
        "<effect percentage='0.15' wound='u2'/>"
        "<effect percentage='0.25' wound='u3'/>"
        "<effect percentage='0.2' wound='ue'/>"
        "<effect percentage='0.2' wound='mort'/>"
        "</effects>"
        "</agent>"
        "</agents>"
        "</nbc>" );
    std::vector< const MIL_NbcAgentType* > nbcTypes;
    //MIL_NbcAgentType::Initialize( xis );
    //nbcTypes.push_back( MIL_NbcAgentType::Find( 1 ) );
    //MIL_ToxicEffectManipulator nbcAgent( nbcTypes, 1 );
    //human.ApplyPoisonous( nbcAgent ); //@TODO MGD Remove singleton
    //BOOST_CHECK( human.IsWounded() );
}

BOOST_AUTO_TEST_CASE( HumanContaminationManagement )
{
    StubMIL_Time_ABC time;
    MockHumansComposante composante;
    MOCK_EXPECT( composante, NotifyHumanAdded );
    MOCK_EXPECT( composante, NotifyHumanRemoved );
    PHY_Human human = PHY_Human( time, composante);

    BOOST_CHECK( !human.IsContaminated() );
    std::vector< const MIL_NbcAgentType* > nbcTypes;
    MIL_ToxicEffectManipulator nbcAgent( nbcTypes, 1 );
    MOCK_EXPECT( composante, NotifyHumanChanged );
    human.ApplyContamination( nbcAgent );
    BOOST_CHECK( human.IsContaminated() );
    MOCK_EXPECT( composante, NotifyHumanChanged );
    human.HealContamination();
    BOOST_CHECK( !human.IsContaminated() );
}

BOOST_AUTO_TEST_CASE( HumanMentalDiseaseManagement )
{
    StubMIL_Time_ABC time;
    MockHumansComposante composante;
    MOCK_EXPECT( composante, NotifyHumanAdded );
    MOCK_EXPECT( composante, NotifyHumanRemoved );
    PHY_Human human = PHY_Human( time, composante);

    BOOST_CHECK( !human.IsMentalDiseased() );
    human.ApplyMentalDisease();
    //BOOST_CHECK( human.IsMentalDiseased() ); //@TODO MGd see why random, update interface corresponding to
    human.HealMentalDisease();
    BOOST_CHECK( !human.IsMentalDiseased() );
}

BOOST_AUTO_TEST_CASE( HumanSetRank )
{
    StubMIL_Time_ABC time;
    MockHumansComposante composante;
    MOCK_EXPECT( composante, NotifyHumanAdded );
    MOCK_EXPECT( composante, NotifyHumanRemoved );
    PHY_Human human = PHY_Human( time, composante);

    MOCK_EXPECT( composante, NotifyHumanChanged );
    BOOST_CHECK( human.SetRank( PHY_HumanRank::sousOfficier_ ) );
    BOOST_CHECK_EQUAL( &human.GetRank(), &PHY_HumanRank::sousOfficier_ );
    BOOST_CHECK( !human.SetRank( PHY_HumanRank::sousOfficier_ ) );

    //Add logistic notifycation test
}

BOOST_AUTO_TEST_CASE( HumanCancelLogisticRequest )
{
  /*@TODO MGD
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

BOOST_AUTO_TEST_CASE( HumanIsUsable )
{
    StubMIL_Time_ABC time;
    MockHumansComposante composante;
    MOCK_EXPECT( composante, NotifyHumanAdded );
    MOCK_EXPECT( composante, NotifyHumanRemoved );
    PHY_Human human = PHY_Human( time, composante);

    //PHY_MedicalHumanState medicalState;

    BOOST_CHECK( human.IsUsable() );
    BOOST_CHECK( !human.IsDead() );
    BOOST_CHECK( !human.IsWounded() );

    MOCK_EXPECT( composante, NotifyHumanChanged );
    human.ApplyWound( PHY_HumanWound::woundedU1_ );
    BOOST_CHECK( human.IsUsable() );// @TODO MGD Why? we can use a wounded man
    BOOST_CHECK( !human.IsDead() );
    BOOST_CHECK( human.IsWounded() );

    human.ApplyWound( PHY_HumanWound::killed_ );
    BOOST_CHECK( !human.IsUsable() );
    BOOST_CHECK( human.IsDead() );
    BOOST_CHECK( !human.IsWounded() );
    human.Heal();
    BOOST_CHECK( human.IsUsable() );
    //SetMedicalState( &medicalState );
    //BOOST_CHECK( !human.IsUsable() );
}

BOOST_AUTO_TEST_CASE( HumanIsAnEmergency )
{
    StubMIL_Time_ABC time;
    MockHumansComposante composante;
    MOCK_EXPECT( composante, NotifyHumanAdded );
    MOCK_EXPECT( composante, NotifyHumanRemoved );
    PHY_Human human = PHY_Human( time, composante);

    BOOST_CHECK( !human.IsAnEmergency() );
    MOCK_EXPECT( composante, NotifyHumanChanged );
    human.SetWound( PHY_HumanWound::woundedU1_ );
    BOOST_CHECK( human.IsAnEmergency() );
    MOCK_EXPECT( composante, NotifyHumanChanged );
    human.SetWound( PHY_HumanWound::killed_ );
    BOOST_CHECK( !human.IsAnEmergency() );
}

BOOST_AUTO_TEST_CASE( HumanUpdate )
{
    StubMIL_Time_ABC time;
    MockHumansComposante composante;
    MOCK_EXPECT( composante, NotifyHumanAdded );
    MOCK_EXPECT( composante, NotifyHumanRemoved );
    PHY_Human human = PHY_Human( time, composante);

    human.Update();
    //Add Notification need medical test
    //time.Add( time + delta mort );
    //human.Update();
    //Add MIL_Report report send test   
}

BOOST_AUTO_TEST_CASE( HumanNeedEvacuation )
{
    StubMIL_Time_ABC time;
    MockHumansComposante composante;
    MOCK_EXPECT( composante, NotifyHumanAdded );
    MOCK_EXPECT( composante, NotifyHumanRemoved );
    PHY_Human human = PHY_Human( time, composante);
    //PHY_MedicalHumanState medicalState;

    BOOST_CHECK( !human.NeedEvacuation() );
    BOOST_CHECK( !human.NeedMedical() );
    MOCK_EXPECT( composante, NotifyHumanChanged );
    human.SetWound( PHY_HumanWound::woundedU1_ );
    BOOST_CHECK( human.NeedEvacuation() );
    BOOST_CHECK( human.NeedMedical() );
    //human.SetMedicalState( &mediacalState );
    //BOOST_CHECK( !human.NeedEvacuation() );
    //BOOST_CHECK( human.NeedMedical() );
}

BOOST_AUTO_TEST_CASE( HumanNeedEvacuate )
{
    StubMIL_Time_ABC time;
    MockHumansComposante composante;
    MOCK_EXPECT( composante, NotifyHumanAdded );
    MOCK_EXPECT( composante, NotifyHumanRemoved );
    PHY_Human human = PHY_Human( time, composante);
    //MIL_AutomateLOG tc2;

    //human.Evacuate( tc2 );
    //Add notification test
    //See why in this interface
}

BOOST_AUTO_TEST_CASE( HumanNotifyHandledByMedical )
{
    StubMIL_Time_ABC time;
    MockHumansComposante composante;
    MOCK_EXPECT( composante, NotifyHumanAdded );
    MOCK_EXPECT( composante, NotifyHumanRemoved );
    PHY_Human human = PHY_Human( time, composante);

    MOCK_EXPECT( composante, NotifyHumanChanged );
    human.NotifyHandledByMedical();
    BOOST_CHECK_EQUAL( human.GetLocation(), PHY_Human::eMedical );
    MOCK_VERIFY( composante, NotifyHumanChanged );
}

BOOST_AUTO_TEST_CASE( HumanNotifyBackToWar )
{
    StubMIL_Time_ABC time;
    MockHumansComposante composante;
    MOCK_EXPECT( composante, NotifyHumanAdded );
    MOCK_EXPECT( composante, NotifyHumanRemoved );
    PHY_Human human = PHY_Human( time, composante);

    
    //human.NotifyBackToWar(); //@TODO MGD need PHY_MedicalHumanState to be set
    //Check notification to component and event
}

BOOST_AUTO_TEST_CASE( HumanMaintenanceManagement )
{
    StubMIL_Time_ABC time;
    MockHumansComposante composante;
    MOCK_EXPECT( composante, NotifyHumanAdded );
    MOCK_EXPECT( composante, NotifyHumanRemoved );
    PHY_Human human = PHY_Human( time, composante);

    MOCK_EXPECT( composante, NotifyHumanChanged );
    human.NotifyComposanteHandledByMaintenance();
    BOOST_CHECK_EQUAL( human.GetLocation(), PHY_Human::eMaintenance );
    human.NotifyComposanteBackFromMaintenance();
    BOOST_CHECK_EQUAL( human.GetLocation(), PHY_Human::eBattleField );
    MOCK_VERIFY( composante, NotifyHumanChanged );
}

/*@TODO MGD find a  way to serialize MockHumansComposante
BOOST_AUTO_TEST_CASE( HumanSerialization )
{
    StubMIL_Time_ABC time;
    MockHumansComposante composante;
    MOCK_EXPECT( composante, NotifyHumanAdded );
    MOCK_EXPECT( composante, NotifyHumanRemoved );
    PHY_Human humanInitial = PHY_Human( time, composante);
    MOCK_EXPECT( composante, NotifyHumanChanged );
    humanInitial.SetWound( PHY_HumanWound::killed_ );
    
    MockHumansComposante composante2;
    MOCK_EXPECT( composante2, NotifyHumanAdded );
    MOCK_EXPECT( composante2, NotifyHumanRemoved );
    PHY_Human humanRestored = PHY_Human( time, composante2);

    std::stringstream stringstream;
    {
        MIL_CheckPointOutArchive outStream( stringstream );
        outStream << humanInitial;
    }
    {
        MIL_CheckPointInArchive inStream( stringstream );
        inStream >> humanRestored;
    }
    BOOST_CHECK( humanRestored.IsDead() );
}*/