// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_LivingAreaBlock.h"
#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Inhabitants/MIL_InhabitantType.h"
#include "Entities/Objects/ResourceNetworkCapacity.h"
#include "Entities/Objects/StructuralCapacity.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Entities/Orders/MIL_PopulationMissionType.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationType.h"
#include "protocol/ClientSenders.h"
#include "Urban/PHY_AccomodationType.h"
#include "Urban/PHY_ResourceNetworkType.h"
#include "Urban/UrbanPhysicalCapacity.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_LivingAreaBlock )

const std::string MIL_LivingAreaBlock::defaultAccomodation_( "default" );

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock constructor
// Created: JSR 2011-04-18
// -----------------------------------------------------------------------------
MIL_LivingAreaBlock::MIL_LivingAreaBlock()
    : urbanObject_ ( 0 )
    , angriness_   ( 0.f )
    , alerted_     ( false )
    , confined_    ( false )
    , evacuated_   ( false )
    , outsideAngry_( false )
    , hasChanged_  ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock constructor
// Created: JSR 2011-03-23
// -----------------------------------------------------------------------------
MIL_LivingAreaBlock::MIL_LivingAreaBlock( MIL_UrbanObject_ABC& urbanObject )
    : urbanObject_ ( &urbanObject )
    , angriness_   ( 0.f )
    , alerted_     ( false )
    , confined_    ( false )
    , evacuated_   ( false )
    , outsideAngry_( false )
    , hasChanged_  ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock destructor
// Created: JSR 2011-03-23
// -----------------------------------------------------------------------------
MIL_LivingAreaBlock::~MIL_LivingAreaBlock()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::SendFullState
// Created: JSR 2011-03-29
// -----------------------------------------------------------------------------
void MIL_LivingAreaBlock::SendFullState( client::PopulationUpdate& msg ) const
{
    sword::PopulationUpdate_BlockOccupation& block = *msg().add_occupations();
    block.mutable_object()->set_id( urbanObject_->GetID() );
    for( auto it = persons_.begin(); it != persons_.end(); ++it )
    {
        sword::PopulationUpdate_BlockOccupation_UsageOccupation* occupation = block.add_persons();
        occupation->set_usage( it->first );
        occupation->set_number( it->second );
    }
    block.set_alerted( alerted_ );
    block.set_confined( confined_ );
    block.set_evacuated( evacuated_ );
    block.set_angriness( angriness_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::UpdateNetwork
// Created: BCI 2011-03-30
// -----------------------------------------------------------------------------
void MIL_LivingAreaBlock::UpdateNetwork( client::PopulationUpdate& msg ) const
{
    if( hasChanged_ )
    {
        SendFullState( msg );
        hasChanged_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::GetObject
// Created: JSR 2011-03-23
// -----------------------------------------------------------------------------
const MIL_UrbanObject_ABC& MIL_LivingAreaBlock::GetObject() const
{
    return *urbanObject_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::SetAlerted
// Created: JSR 2011-03-29
// -----------------------------------------------------------------------------
void MIL_LivingAreaBlock::SetAlerted( bool alerted )
{
    if( alerted_ != alerted )
    {
        alerted_ = alerted;
        hasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::SetEvacuated
// Created: JSR 2011-03-29
// -----------------------------------------------------------------------------
void MIL_LivingAreaBlock::SetEvacuated( bool evacuated )
{
    if( evacuated_ != evacuated )
    {
        evacuated_ = evacuated;
        hasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::SetConfined
// Created: JSR 2011-03-29
// -----------------------------------------------------------------------------
void MIL_LivingAreaBlock::SetConfined( bool confined )
{
    if( confined_ != confined )
    {
        confined_ = confined;
        hasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::CanMove
// Created: JSR 2011-03-29
// -----------------------------------------------------------------------------
bool MIL_LivingAreaBlock::CanMove() const
{
    return !( confined_ || outsideAngry_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::IsAlerted
// Created: JSR 2011-03-29
// -----------------------------------------------------------------------------
bool MIL_LivingAreaBlock::IsAlerted( const TER_Localisation& localisation ) const
{
    return alerted_ && urbanObject_->Intersect2DWithLocalisation( localisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::IsConfined
// Created: CCD 2012-01-10
// -----------------------------------------------------------------------------
bool MIL_LivingAreaBlock::IsConfined( const TER_Localisation& localisation ) const
{
    return confined_ && urbanObject_->Intersect2DWithLocalisation( localisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::IsEvacuated
// Created: ABR 2011-04-11
// -----------------------------------------------------------------------------
bool MIL_LivingAreaBlock::IsEvacuated() const
{
    return evacuated_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::IsEvacuated
// Created: CCD 2012-01-16
// -----------------------------------------------------------------------------
bool MIL_LivingAreaBlock::IsEvacuated( const TER_Localisation& localisation ) const
{
    return evacuated_ && urbanObject_->Intersect2DWithLocalisation( localisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::GetPersonsForAccomodation
// Created: JSR 2011-03-23
// -----------------------------------------------------------------------------
unsigned int MIL_LivingAreaBlock::GetPersonsForAccomodation( const std::string& accomodation ) const
{
    CIT_Persons it = persons_.find( accomodation );
    if( it == persons_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::GetTotalNumberOfPersons
// Created: JSR 2011-03-23
// -----------------------------------------------------------------------------
unsigned int MIL_LivingAreaBlock::GetTotalNumberOfPersons() const
{
    unsigned int sum = 0;
    for( auto it = persons_.begin(); it != persons_.end(); ++it )
        sum += it->second;
    return sum;
}

namespace
{
    float GetStructuralState( const MIL_UrbanObject_ABC& object )
    {
        const StructuralCapacity* structural = object.Retrieve< StructuralCapacity >();
        return structural ? structural->GetStructuralState() : 1.f;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::GetNominalOccupation
// Created: JSR 2011-03-23
// -----------------------------------------------------------------------------
unsigned int MIL_LivingAreaBlock::GetNominalOccupation( const std::string& motivation ) const
{
    auto ptr = PHY_AccomodationType::Find( motivation );
    if( ptr )
        return GetNominalOccupation( motivation, ptr );
    return 0u;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::GetNominalOccupation
// Created: LDC 2011-05-10
// -----------------------------------------------------------------------------
unsigned int MIL_LivingAreaBlock::GetNominalOccupation( const std::string& motivation, const PHY_AccomodationType* accomodation ) const
{
    if( accomodation )
        return static_cast< unsigned int >( urbanObject_->GetLivingSpace() * GetStructuralState( *urbanObject_ ) * GetProportion( motivation ) * accomodation->GetNominalCapacity() );
    return 0u;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::GetMaxOccupation
// Created: JSR 2011-03-23
// -----------------------------------------------------------------------------
unsigned int MIL_LivingAreaBlock::GetMaxOccupation( const std::string& motivation ) const
{
    auto ptr = PHY_AccomodationType::Find( motivation );
    if( ptr )
        return static_cast< unsigned int >( urbanObject_->GetLivingSpace() * GetStructuralState( *urbanObject_ ) * GetProportion( motivation ) * ptr->GetMaxCapacity() );
    return 0u;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::DistributeHumans
// Created: JSR 2011-03-23
// -----------------------------------------------------------------------------
void MIL_LivingAreaBlock::DistributeHumans( unsigned int persons, MIL_LivingArea& livingArea )
{
    unsigned long blockTmp = persons;
    std::string firstAccommodation;
    PHY_AccomodationType::Walk( [&]( const PHY_AccomodationType& accommodation ){
        const auto& role = accommodation.GetRole();
        float proportion = GetProportion( role );
        if( proportion <= 0 )
            return;
        if( firstAccommodation.empty() )
            firstAccommodation = role;
        unsigned long nbr = static_cast< unsigned long >( persons * proportion );
        persons_[ role ] = nbr;
        blockTmp -= nbr;
    });

    if( blockTmp && !firstAccommodation.empty() )
        persons_[ firstAccommodation ] += blockTmp;

    for( auto it = persons_.begin(); it != persons_.end(); ++it )
        urbanObject_->UpdateInhabitants( livingArea, it->first, it->second );

    hasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::ComputeOccupationFactor
// Created: JSR 2011-03-23
// -----------------------------------------------------------------------------
float MIL_LivingAreaBlock::ComputeOccupationFactor() const
{
    unsigned int totalPerson = GetTotalNumberOfPersons();;
    if( totalPerson == 0 )
        return 0;
    int blockOccupation = 0;
    PHY_AccomodationType::Walk( [&]( const PHY_AccomodationType& type ){
        blockOccupation += GetNominalOccupation( type.GetRole(), &type );
    });
    int totalPopulation = urbanObject_->GetTotalInhabitants() - totalPerson;
    return static_cast< float >( std::min( static_cast< int >( totalPerson ), std::max( 0, blockOccupation - totalPopulation ) ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::GetProportion
// Created: JSR 2011-03-23
// -----------------------------------------------------------------------------
float MIL_LivingAreaBlock::GetProportion( const std::string& motivation ) const
{
    const UrbanPhysicalCapacity* pPhysical = urbanObject_->Retrieve< UrbanPhysicalCapacity >();
    if( !pPhysical )
        return 0;
    const std::map< std::string, float >& motivations = pPhysical->GetMotivations();
    if( motivation == defaultAccomodation_ )
    {
        float sum = 0;
        for( auto it = motivations.begin(); it != motivations.end(); ++it )
            if( PHY_AccomodationType::Find( it->first ) )
                sum += it->second;
        return 1 - sum;
    }
    auto it = motivations.find( motivation );
    if( it == motivations.end() )
        return 0.f;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::DecreasePeopleWhenMoving
// Created: JSR 2011-03-24
// -----------------------------------------------------------------------------
void MIL_LivingAreaBlock::DecreasePeopleWhenMoving( const std::string& motivation, unsigned int number, MIL_LivingArea& livingArea )
{
    int newValue = persons_[ motivation ] - number;
    persons_[ motivation ] = std::max( 0, newValue );
    urbanObject_->UpdateInhabitants( livingArea, motivation, persons_[ motivation ] );
    hasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::IncreasePeopleWhenMoving
// Created: JSR 2011-03-24
// -----------------------------------------------------------------------------
unsigned int MIL_LivingAreaBlock::IncreasePeopleWhenMoving( const std::string& motivation, unsigned int number, MIL_LivingArea& livingArea )
{
    if( number == 0 )
        return 0;
    unsigned int remaining = 0;
    if( motivation != "" )
    {
        unsigned int maxOccupation = GetMaxOccupation( motivation );
        if( maxOccupation == 0 )
            return number;
        unsigned int newValue = persons_[ motivation ] + number;
        if( maxOccupation < newValue )
        {
            remaining = newValue - maxOccupation;
            persons_[ motivation ] = maxOccupation;
        }
        else
            persons_[ motivation ] = newValue;
        urbanObject_->UpdateInhabitants( livingArea, motivation, persons_[ motivation ] );
    }
    else
    {
        remaining = number;
        PHY_AccomodationType::Walk( [&]( const PHY_AccomodationType& type ){
            remaining = IncreasePeopleWhenMoving( type.GetRole(), remaining, livingArea );
        });
    }
    hasChanged_ = true;
    return remaining;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::Consume
// Created: JSR 2011-03-25
// -----------------------------------------------------------------------------
float MIL_LivingAreaBlock::Consume( const PHY_ResourceNetworkType& resource, unsigned int consumption, bool& angry )
{
    angry = false;
    unsigned int totalPersons = GetTotalNumberOfPersons();
    if( totalPersons == 0 )
        return 0;
    if( ResourceNetworkCapacity* capacity = urbanObject_->Retrieve< ResourceNetworkCapacity >() )
    {
        float blockSatisfaction = totalPersons * capacity->GetConsumptionState( resource.GetId() );
        capacity->AddConsumption( resource.GetId(), totalPersons * consumption );

        float newAngriness;
        if( ( alerted_ || outsideAngry_ ) && blockSatisfaction < 0.1f && totalPersons > 0 )
            newAngriness = std::min( angriness_ + 0.001f, 1.f );
        else
            newAngriness = std::max( angriness_ - 0.001f, 0.f );
        if( newAngriness != angriness_ )
        {
            angriness_ = newAngriness;
            hasChanged_ = true;
        }

        if( MIL_Population* pAngryCrowd = MIL_AgentServer::GetWorkspace().GetEntityManager().FindPopulation( urbanObject_ ) )
            pAngryCrowd->SetUrbanBlockAngriness( angriness_ );

        angry = ( outsideAngry_ || angriness_ >= 1.f );
        return blockSatisfaction;
    }
    hasChanged_ = true;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingAreaBlock::ManageAngryCrowd
// Created: JSR 2011-03-29
// -----------------------------------------------------------------------------
void MIL_LivingAreaBlock::ManageAngryCrowd( const MIL_InhabitantType& type, MIL_Army_ABC& army )
{
    // $$$$ _RC_ JSR 2011-03-23: A nettoyer, le FindPopulation devrait être renommé prendre un id
    MIL_Population* pAngryCrowd = MIL_AgentServer::GetWorkspace().GetEntityManager().FindPopulation( urbanObject_ );
    if( !pAngryCrowd )
    {
        outsideAngry_ = true;
        // $$$$ _RC_ JSR 2011-03-29: TRADUCTIONS???
        std::string name = "Angry crowd";
        std::string urbanObjectName = urbanObject_->GetName();
        if( !urbanObjectName.empty() )
            name += " from " + urbanObjectName;
        pAngryCrowd = MIL_AgentServer::GetWorkspace().GetEntityManager().CreateCrowd( type.GetAssociatedCrowdType().GetName(), urbanObject_->GetLocalisation().ComputeBarycenter(), 0, name, army, urbanObject_ );
    }
    if( pAngryCrowd->GetAllHumans() == 0 ) // $$$$ BCI 2011-03-21: bidouille parce qu'on ne peut pas supprimer une foule
    {
        if( angriness_ < 1.f )// $$$$ BCI 2011-03-21: bidouille parce qu'on ne peut pas supprimer une foule
            outsideAngry_ = false;// $$$$ BCI 2011-03-21: bidouille parce qu'on ne peut pas supprimer une foule
        else
        {
            pAngryCrowd->ChangeComposition( GetTotalNumberOfPersons(), 0, 0, 0 );
            pAngryCrowd->Move( urbanObject_->GetLocalisation().ComputeBarycenter() );

            const  MIL_MissionType_ABC* pMissionType = MIL_PopulationMissionType::Find( type.GetAngryCrowdMissionType() );
            if( !pMissionType )
                throw MASA_EXCEPTION( "Unknow angry crowd mission type " + type.GetAngryCrowdMissionType() );

            // $$$$ BCI 2011-03-18: comment faire plus simple pour démarrer une tâche?
            sword::CrowdOrder msg;
            msg.mutable_tasker()->set_id( pAngryCrowd->GetID() );
            msg.mutable_type()->set_id( pMissionType->GetID() );
            msg.mutable_parameters();
            pAngryCrowd->OnReceiveOrder( msg );
        }
    }
}
