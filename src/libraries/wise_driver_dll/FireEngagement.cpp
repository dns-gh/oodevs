// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "FireEngagement.h"
#include "Agent.h"
#include "Model.h"
#include "protocol/Simulation.h"
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/iwisedriversink.h>
#include <wise/WISEAttributeGroupConverter.h> 
#include <wise/wisedriver.h>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: FireEngagement constructor
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
FireEngagement::FireEngagement( const Model& model, const MsgsSimToClient::MsgStartUnitFire& message )
    : model_( model )
    , id_( message.fire().id() )
    , shooter_( model.ResolveAgent( message.firing_unit().id() ) )
    , target_( message.target().has_unit() ? model.ResolveAgent( message.target().unit().id() ) : 0 )
    , type_( message.type() )
    , ammunition_( message.ammunition().id() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireEngagement destructor
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
FireEngagement::~FireEngagement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireEngagement::GetId
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
unsigned long FireEngagement::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: FireEngagement::Update
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void FireEngagement::Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& /*currentTime*/, const MsgsSimToClient::MsgStopUnitFire& message )
{
    try
    {
        // $$$$ SEB 2010-10-14: See if we need to handle when no damage has been done
        if( message.has_units_damages() )
        {
            const MsgsSimToClient::MsgUnitsFireDamages& damages = message.units_damages();
            IWISEStringCache* cache = dynamic_cast< IWISEStringCache* >( driver.GetSink() );
            CHECK_VALID_POINTER_EX( cache, MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_E_NOT_INITIATED ) );
            for( int i = 0; i < damages.elem_size(); ++i )
            {
                WISE_HANDLE handle = WISE_INVALID_HANDLE;
                std::map< std::wstring, WISE_HANDLE > attributes;
                CHECK_WISE_RESULT_EX( driver.GetSink()->CreateEventFromTemplate( database, L"FireEngagement", handle, attributes ) );
                CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Shooter" ], shooter_ ? shooter_->GetHandle() : WISE_INVALID_HANDLE ) );
                CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Target" ], target_ ? target_->GetHandle() : WISE_INVALID_HANDLE ) );
                CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Type" ], unsigned char( type_ ) ) );
                CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Ammunition" ], long( ammunition_ ) ) );
                SendDamages( driver, handle, *cache, attributes, damages.elem( i ) );
                CHECK_WISE_RESULT_EX( driver.GetSink()->SendEvent( database, handle ) );
                driver.NotifyInfoMessage( L"[fire-engagement-event] Created." );
            }
        }
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"[fire-engagement-event] Creation failed.", error );
    }
}

// -----------------------------------------------------------------------------
// Name: FireEngagement::SendDamages
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void FireEngagement::SendDamages( CWISEDriver& driver, const WISE_HANDLE& handle, IWISEStringCache& cache, std::map< std::wstring, WISE_HANDLE >& attributes, const MsgsSimToClient::MsgUnitFireDamages& message ) const
{
    std::list< CWISEAttributeGroup > equipments;
    for( int i = 0; i < message.equipments().elem_size(); ++i )
        SendEquipmentDamages( equipments, cache, message.equipments().elem( i ) );
    CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"EquipmentDamages" ], equipments ) );

    std::list< CWISEAttributeGroup > personnel;
    for( int i = 0; i < message.humans().elem_size(); ++i )
        SendPersonnelDamages( personnel, cache, message.humans().elem( i ) );
    CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"PersonnelDamages" ], personnel ) );
}

// -----------------------------------------------------------------------------
// Name: FireEngagement::SendEquipmentDamages
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void FireEngagement::SendEquipmentDamages( std::list< CWISEAttributeGroup >& list, IWISEStringCache& cache, const MsgsSimToClient::MsgUnitEquipmentFireDamage& message ) const
{
    CWISEAttributeGroupTemplate groupTemplate;
    groupTemplate.Add( L"Type", long( message.equipement_type().id() ) );
    groupTemplate.Add( L"AvailableCount", long( message.available_nbr() ) );
    groupTemplate.Add( L"UnavailableCount", long( message.unavailable_nbr() ) );
    groupTemplate.Add( L"RepairableCount", long( message.repairable_nbr() ) );

    CWISEAttributeGroup group;
    CHECK_WISE_RESULT_EX( CWISEAttributeGroupConverter::TemplateToValueGroup( groupTemplate, &cache, L"EquipmentDamageReport", group ) );
    list.push_back( group );
}

// -----------------------------------------------------------------------------
// Name: FireEngagement::SendPersonnelDamages
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void FireEngagement::SendPersonnelDamages( std::list< CWISEAttributeGroup >& list, IWISEStringCache& cache, const MsgsSimToClient::UnitHumanFireDamage& message ) const
{
    CWISEAttributeGroupTemplate groupTemplate;
    groupTemplate.Add( L"Rank", unsigned char( message.rank() ) );
    groupTemplate.Add( L"AliveCount", long( message.alive_nbr() ) );
    groupTemplate.Add( L"DeadCount", long( message.dead_nbr() ) );
    groupTemplate.Add( L"WoundedPriority1Count", long( message.wounded_u1_nbr() ) );
    groupTemplate.Add( L"WoundedPriority2Count", long( message.wounded_u2_nbr() ) );
    groupTemplate.Add( L"WoundedPriority3Count", long( message.wounded_u3_nbr() ) );
    groupTemplate.Add( L"WoundedHighestPriorityCount", long( message.wounded_ue_nbr() ) );

    CWISEAttributeGroup group;
    CHECK_WISE_RESULT_EX( CWISEAttributeGroupConverter::TemplateToValueGroup( groupTemplate, &cache, L"PersonnelDamageReport", group ) );
    list.push_back( group );
}
