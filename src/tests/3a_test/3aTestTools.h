// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __3aTestTools_h_
#define __3aTestTools_h_

#include <boost/noncopyable.hpp>
#include "3a/Attributes.h"
#include "protocol/Protocol.h"
#include <geocoord/geodetic.h>
#include <geocoord/mgrs.h>

struct MockClientPublisher;

// =============================================================================
/** @class  TestTools
    @brief  TestTools
*/
// Created: FPO 2011-05-06
// =============================================================================
class TestTools : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    TestTools() {}
    virtual ~TestTools() {}
    //@}

    //! @name Operations
    //@{
    static sword::SimToClient OperationalState( unsigned opstate, unsigned long id );
    static sword::SimToClient BeginTick();
    static sword::SimToClient EndTick();
    static sword::SimToClient MakeUnitCreation( unsigned long id, unsigned long type_id );
    static bool CheckValue( const sword::AarToClient& expected, const sword::AarToClient& actual );
    static sword::SimToClient MakeEquipementVariation( int variation[5], unsigned long id, unsigned long equipmentId = 42 );
    static sword::SimToClient MakePosition( const char* position, unsigned long id );
    static sword::SimToClient MakeResourceVariation( int variation, unsigned long id, unsigned long resourceId = 42 );
    static sword::SimToClient MakeStockVariation( int variation, unsigned long id, unsigned long stockId );
    static sword::SimToClient CreateUnitDetection( unsigned int detector, unsigned int detected, sword::UnitVisibility::Level visibility );
    static sword::SimToClient MakeMounted( bool mounted, unsigned long id );
    static sword::SimToClient MakeHumanVariation( int state[8], unsigned long id );
    static sword::SimToClient MakeAmbulances( int evacuationNumber, int collectionNumber, unsigned long id );
    static sword::SimToClient MakeMaintenance( int repairersNumber, int haulersNumber, unsigned long id );
    static sword::SimToClient UpdatePopulationStates( int healthy, int wounded, int dead, unsigned long id );
    static sword::SimToClient UpdatePopulationStatesbis( int healthy, int wounded, unsigned long id );
    static sword::SimToClient UpdatePopulationSatisfaction ( float lodging, float health, float safety, unsigned long id );
    static sword::SimToClient UpdatePopulationInBlocks( unsigned long populationId, std::map < unsigned long, int > );
    static sword::SimToClient UpdateCrowdDeadConcentration( unsigned long crowdId, int dead );
    static sword::SimToClient CreateDirectFire( unsigned fire_id, unsigned long firer );
    static sword::SimToClient CreateIndirectFire( unsigned fire_id, unsigned long firer );
    static sword::SimToClient StopFire( unsigned fire_id, unsigned int target_id, unsigned long damage_count = 0, unsigned long deadhumans_count = 0 );
    static sword::SimToClient CreateConsign( unsigned long id, unsigned long unit_id = 0, unsigned long equip_id = 0, unsigned long breakdown_id = 0 );
    static sword::SimToClient DestroyConsign( unsigned long id );
    static bool IsCloseCombatPower( const extractors::PowerExtractor_ABC& extractor );
    static sword::SimToClient CreateMedicalConsign( unsigned long unitId, unsigned long requestId, bool isWaiting );
    static sword::SimToClient CreateFunctionalState( unsigned long objectId, int stateValue );
    static sword::SimToClient UpdateFunctionalState( unsigned long objectId, int stateValue );

    template< std::size_t N >
    static void MakeExpectation( MockClientPublisher& mocker, double (&data)[N] );
    //@}

private:
    //! @name Member data
    //@{
    static unsigned int currentTick_;
    //@}
};

template< std::size_t N >
inline
void TestTools::MakeExpectation( MockClientPublisher& mocker, double (&data)[N] )
{
    AarToClient result;
    result.set_context( 0 );
    PlotResult& plot = *result.mutable_message()->mutable_plot_result();
    plot.set_identifier( 42 );
    plot.set_error( "" );
    for( unsigned i = 0; i < N; ++i )
        plot.add_values( static_cast< float >( data[i] ) );
    MOCK_EXPECT( mocker, Send4 ).once().with( boost::bind( &TestTools::CheckValue, result, _1 ) );
}

#endif // __3aTestTools_h_
