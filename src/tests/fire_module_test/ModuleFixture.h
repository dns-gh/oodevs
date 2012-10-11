// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef MODULE_FIXTURE_H
#define MODULE_FIXTURE_H

#include "module_tester/ApiFixture.h"
#include "MT_Tools/MT_Vector2D.h"

#define REGISTERED_HOOKS( APPLY ) \
    APPLY( InitializeLaunchers, 1, void, ( const char* xml ) ) \
    APPLY( InitializeDotations, 1, void, ( const char* xml ) ) \
    APPLY( InitializeWeaponSystems, 2, void, ( const char* xml, double tickDuration ) ) \
    APPLY( InitializeDecisional, 2, void, ( const char* xml, double tickDuration ) ) \
    APPLY( GetDangerosity, 5, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), double distance, bool checkAmmo ) ) \
    APPLY( GetMaxRangeToFireOn, 5, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), double rWantedPH, const char* dotation ) ) \
    APPLY( GetMinRangeToFireOn, 4, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), double rWantedPH ) ) \
    APPLY( GetMaxRangeToFire, 3, double, ( const SWORD_Model* firer, bool(*filter)( const SWORD_Model* component ), double rWantedPH ) ) \
    APPLY( GetMaxRangeToIndirectFire, 4, double, ( const SWORD_Model* firer, bool(*filter)( const SWORD_Model* component ), const char* dotation, bool checkAmmo ) ) \
    APPLY( GetMinRangeToIndirectFire, 4, double, ( const SWORD_Model* firer, bool(*filter)( const SWORD_Model* component ), const char* dotation, bool checkAmmo ) ) \
    APPLY( GetMaxRangeToFireOnWithPosture, 4, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), double rWantedPH ) ) \
    APPLY( GetMinRangeToFireOnWithPosture, 4, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), double rWantedPH ) ) \
    APPLY( GetForceRatio, 2, double, ( const SWORD_Model* model, const SWORD_Model* entity ) ) \
    APPLY( GetDangerousEnemies, 4, void, ( const SWORD_Model* model, const SWORD_Model* entity, void(*visitor)( const SWORD_Model* knowledge, void* userData ), void* userData ) ) \
    APPLY( ComputeForceRatio, 4, double, ( const SWORD_Model* model, const SWORD_Model* entity, bool(*filter)( const SWORD_Model* knowledge, void* userData ), void* userData ) ) \
    APPLY( GetAmmunitionForIndirectFire, 4, const char*, ( const SWORD_Model* model, const SWORD_Model* firer, const char* type, const MT_Vector2D* target ) )

#define USED_HOOKS( APPLY ) \
    APPLY( HasDotation, 2, bool, ( const SWORD_Model* entity, const char* dotation ) ) \
    APPLY( GetDotationValue, 2, double, ( const SWORD_Model* entity, const char* dotation ) ) \
    APPLY( CanFire, 3, bool, ( const SWORD_Model* component, const char* dotation, const SWORD_Model* parameters ) ) \
    APPLY( CanComponentFire, 1, bool, ( const SWORD_Model* component ) ) \
    APPLY( GetWeaponReloadingDuration, 2, double, ( const SWORD_Model* firer, double rDuration ) ) \
    APPLY( ReserveAmmunition, 3, size_t, ( const SWORD_Model* firer, const char* dotation, size_t ammos ) ) \
    APPLY( GetDistance, 2, double, ( const SWORD_Model* firer, const SWORD_Model* target ) ) \
    APPLY( ModifyPh, 4, double, ( const SWORD_Model* firer, const SWORD_Model* target, const char* dotation, double rPh ) ) \
    APPLY( ModifyDangerosity, 2, double, ( const SWORD_Model* compTarget, const char* dotation ) ) \
    APPLY( IsTemporarilyBlocked, 2, bool, ( const SWORD_Model* entity, std::size_t nUrbanCoefficient ) ) \
    APPLY( GetFireRandomInteger, 2, size_t, ( size_t min, size_t max ) ) \
    APPLY( GetFireRandomNumber, 2, double, ( double min, double max ) ) \
    APPLY( GetVolumeId, 1, size_t, ( const char* type ) ) \
    APPLY( CanComponentBeFiredAt, 2, bool, ( const SWORD_Model* component, const SWORD_Model* parameters ) ) \
    APPLY( GetPhModificator, 3, double, ( const SWORD_Model* firer, const SWORD_Model* target, const char* launcher ) ) \
    APPLY( GetPhModificator2, 1, double, ( const char* launcher ) ) \
    APPLY( EvaluateDangerosity, 2, double, ( const SWORD_Model* agent, const SWORD_Model* target ) ) \
    APPLY( EvaluateDangerosity2, 2, double, ( const SWORD_Model* agent, const SWORD_Model* target ) ) \
    APPLY( IsAgentKnowledgeValid, 1, bool, ( const SWORD_Model* knowledge ) ) \
    APPLY( IsPopulationKnowledgeValid, 2, bool, ( const SWORD_Model* entity, const SWORD_Model* knowledge ) ) \
    APPLY( GetClosestAlivePopulationElement, 3, const SWORD_Model*, ( const SWORD_Model* model, const SWORD_Model* population, const SWORD_Model* entity ) ) \
    APPLY( ComputeKilledHumans, 2, size_t, ( const SWORD_Model* firer, const SWORD_Model* element ) ) \
    APPLY( GetPopulationElementPh, 2, double, ( const SWORD_Model* firer, const SWORD_Model* element ) )

#define REGISTERED_AND_USED_HOOKS( APPLY )

HOOK_FIXTURE( HOOKS )

namespace sword
{
namespace fire
{
    struct ModuleFixture : core::HookFixture
    {
        ModuleFixture()
            : HookFixture( "fire_module", boost::assign::list_of( "direct fire" )( "direct fire population" ) )
            , entity   ( model[ "entities" ][ 42 ] )
            , firer    ( core::Convert( &entity ) )
            , target   ( model[ "knowledges" ][ 1242 ][ 51 ] )
            , enemy    ( core::Convert( &target ) )
            , posture_1( 1 )
            , posture_2( 2 )
            , volume_1 ( 0 )
            , ammo_1   ( "ammo_1" )
            , ammo_2   ( "ammo_2" )
        {
            entity[ "components" ];
            entity[ "knowledges" ] = 1242;
            entity[ "identifier" ] = 42;
            model[ "entities" ][ 43 ][ "components" ];
            target[ "components" ];
            target[ "identifier" ] = 43;
            target[ "dead" ] = false;
            mock::sequence s1, s2;
            MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_INFO, std::string( "Initializing launchers" ) ).in( s1 );
            MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_INFO, std::string( "Initializing dotations" ) ).in( s2 );
            MOCK_EXPECT( Log ).once().with( SWORD_LOG_LEVEL_INFO, std::string( "Initializing weapons" ) ).in( s1 ).in( s2 );
            MOCK_EXPECT( GetVolumeId ).with( std::string( "volume_1" ) ).returns( volume_1 );
            InitializeLaunchers( "<launchers>"
                                 "  <launcher name='launcher_1' indirect-fire='true'>"
                                 "    <ph-modifiers posture='posture_1'>"
                                 "      <ph-modifier target-posture='posture_2' value='0.77'/>"
                                 "    </ph-modifiers>"
                                 "  </launcher>"
                                 "</launchers>" );
            InitializeDotations( "<resources>"
                                 "  <resource name='ammo_1'>"
                                 "    <attritions>"
                                 "      <attrition/>"
                                 "    </attritions>"
                                 "  </resource>"
                                 "  <resource name='ammo_2'>"
                                 "    <indirect-fires>"
                                 "      <indirect-fire type='explosion'/>"
                                 "    </indirect-fires>"
                                 "  </resource>"
                                 "</resources>" );
            InitializeWeaponSystems( "<weapons>"
                                     "  <weapon-system launcher='launcher_1' munition='ammo_1'>"
                                     "    <burst munition='3' duration='7s'/>"
                                     "    <reloading munition='4' duration='5s'/>"
                                     "    <direct-fire>"
                                     "      <hit-probabilities target='volume_1'>"
                                     "        <hit-probability distance='0' percentage='0'/>"
                                     "        <hit-probability distance='1000' percentage='1'/>"
                                     "        <hit-probability distance='2000' percentage='0'/>"
                                     "      </hit-probabilities>"
                                     "    </direct-fire>"
                                     "  </weapon-system>"
                                     "  <weapon-system launcher='launcher_1' munition='ammo_2'>"
                                     "    <burst munition='1' duration='2s'/>"
                                     "    <reloading munition='1' duration='5s'/>"
                                     "    <indirect-fire average-speed='300' max-range='5000' min-range='100'/>"
                                     "  </weapon-system>"
                                     "</weapons>", 10 );
            InitializeDecisional( "<decisional>"
                                  "  <force-ratio default-feedback-time='600s'/>"
                                  "</decisional>", 10 );
            mock::verify();
            MOCK_RESET( Log );
        }
        core::Model& entity;
        SWORD_Model* firer;
        core::Model& target;
        SWORD_Model* enemy;
        std::string ammo_1, ammo_2;
        std::size_t posture_1, posture_2;
        std::size_t volume_1;
    };
}
}

#endif // MODULE_FIXTURE_H
