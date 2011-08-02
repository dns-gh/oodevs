// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "Who.h"
#include "dispatcher/Agent_ABC.h"
#include "dispatcher/Automat_ABC.h"
#include "dispatcher/Team_ABC.h"
#include "dispatcher/Equipment.h"
#include "dispatcher/Humans.h"
#include "clients_kernel/Karma.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: Who constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Who::Who( const dispatcher::Agent_ABC& entity )
    : agent_( &entity )
    , automat_( 0 )
    , attributes_( 0 )
    , level_( -1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Who constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Who::Who( const dispatcher::Automat_ABC& entity )
    : agent_( 0 )
    , automat_( &entity )
    , attributes_( 0 )
    , level_( -1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Who constructor
// Created: SBO 2008-07-22
// -----------------------------------------------------------------------------
Who::Who( const dispatcher::Agent_ABC& entity, int detectionLevel )
    : agent_( &entity )
    , automat_( 0 )
    , attributes_( 0 )
    , level_( detectionLevel )
{
    // NOTHING
}

namespace
{
    // $$$$ SBO 2008-07-24: hack to force update of real entities
    int ComputeLevel( const sword::UnitAttributes& attributes )
    {
        return attributes.has_operational_state()
            || attributes.has_equipment_dotations()
            || attributes.has_human_dotations()
            ? sword::UnitVisibility::identified : -1;
    }
}

// -----------------------------------------------------------------------------
// Name: Who constructor
// Created: SBO 2008-07-22
// -----------------------------------------------------------------------------
Who::Who( const dispatcher::Agent_ABC& entity, const sword::UnitAttributes& attributes )
    : agent_( &entity )
    , automat_( 0 )
    , attributes_( &attributes )
    , level_( ComputeLevel( attributes ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Who destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Who::~Who()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Who::operator<<
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
xml::xostream& plugins::bml::operator<<( xml::xostream& xos, const Who& who )
{
    xos << xml::start( "C_BML_Who" )
            << xml::start( "WhoRef" )
                << xml::start( "UnitRef" )
                    << xml::content( "jc3iedm:OID", who.agent_ ? who.agent_->GetName().ascii() : who.automat_->GetName().ascii() );
    if( who.attributes_ )
        who.SendEquipmentStatus( xos );
    if( who.level_ != -1 )
        who.SendStatus( xos );
    xos         << xml::end
            << xml::end
        << xml::end;
    return xos;
}

namespace
{
    void SerializeAvailability( const std::string& category, xml::xostream& xos, unsigned int quantity )
    {
        xos << xml::start( "jc3iedm:ObjectItemCapabilityInObjectItem" )
                << xml::start( "jc3iedm:Capability" )
                    << xml::attribute( "xsi:type", "jc3iedm:MissionCapability" )
                    << xml::content( "jc3iedm:UnitOfMeasureCode", "PRCNTG" )
                    << xml::content( "jc3iedm:CategoryCode", category )
                << xml::end
                << xml::start( "jc3iedm:ObjectItemCapability" )
                    << xml::content( "jc3iedm:Quantity", quantity )
                    << xml::content( "jc3iedm:ReportingDataRef", "" )
                << xml::end
            << xml::end;
    }

    struct AvailabilityComputer
    {
        AvailabilityComputer() : count_( 0 ), available_( 0 ) {}
        void AddEquipment( const dispatcher::Equipment& equipment )
        {
            available_ += equipment.nNbrAvailable_;
            count_ += equipment.nNbrAvailable_ + equipment.nNbrUnavailable_ + equipment.nNbrRepairable_
                   +  equipment.nNbrInMaintenance_ + equipment.nNbrPrisoner_;
        }
        void AddHuman( const dispatcher::Humans& humans )
        {
            available_ += humans.nNbrOperational_;
            count_ += humans.nNbrTotal_;
        }

        unsigned int Percentage()
        {
            return available_ < 0 ? 0 : available_ * 100 / count_;
        }
        unsigned int count_;
        int available_;
    };
}

// -----------------------------------------------------------------------------
// Name: Who::SendEquipmentStatus
// Created: SBO 2008-07-15
// -----------------------------------------------------------------------------
void Who::SendEquipmentStatus( xml::xostream& xos ) const
{
    if( !agent_ )
        return;
    xos << xml::start( "jc3iedm:ObjectItemCapabilityInObjectItemList" );
    if( attributes_->has_equipment_dotations() )
    {
        AvailabilityComputer computer;
        agent_->Equipments().Apply( boost::bind( &AvailabilityComputer::AddEquipment, boost::ref( computer ), _1 ) );
        SerializeAvailability( "SPLC2", xos, computer.Percentage() );
    }
    if( attributes_->has_human_dotations() )
    {
        AvailabilityComputer computer;
        agent_->Troops().Apply( boost::bind( &AvailabilityComputer::AddHuman, boost::ref( computer ), _1 ) );
        SerializeAvailability( "PERSVC", xos, computer.Percentage() );
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Who::SendStatus
// Created: SBO 2008-07-22
// -----------------------------------------------------------------------------
void Who::SendStatus( xml::xostream& xos ) const
{
    xos << xml::start( "jc3iedm:StatusList" )
            << xml::start( "jc3iedm:Status" )
                << xml::attribute( "xsi:type", "jc3iedm:OrganisationStatus" )
                << xml::content( "jc3iedm:HostilityCode", GetFilterHostility() )
                << xml::content( "jc3iedm:ReportingDataRef", "" )
                << xml::content( "jc3iedm:OperationalStatusCode", GetFilterOperationalState() )
            << xml::end
        << xml::end;
}

namespace
{
    std::string Hostility( const dispatcher::Automat_ABC& entity )
    {
        const kernel::Karma& karma = entity.GetTeam().GetKarma();
        if( karma == kernel::Karma::friend_ )
            return "FR";
        else if( karma == kernel::Karma::enemy_ )
            return "HO";
        else if( karma == kernel::Karma::neutral_ )
            return "NEUTRL";
        else
            return "UNK";
    }

    std::string OperationalState( const sword::EnumOperationalStatus& status )
    {
        switch( status )
        {
            case sword::totally_destroyed:
            case sword::tactically_destroyed:
                return "NOP";
            case sword::operational:
                return "OPR";
            default:
                break;
        }
        return "NKN";
    }
}

// -----------------------------------------------------------------------------
// Name: Who::GetFilterHostility
// Created: SBO 2008-07-22
// -----------------------------------------------------------------------------
std::string Who::GetFilterHostility() const
{
    if( level_ > sword::UnitVisibility::detected )
    {
        if( agent_ )
            return Hostility( agent_->GetSuperior() );
        return Hostility( *automat_ );
    }
    return "UNK";
}

// -----------------------------------------------------------------------------
// Name: Who::GetFilterOperationalState
// Created: SBO 2008-07-22
// -----------------------------------------------------------------------------
std::string Who::GetFilterOperationalState() const
{
    if( agent_ && level_ > sword::UnitVisibility::recognized )
        return OperationalState( agent_->GetOperationalState() );
    return "NKN";
}
