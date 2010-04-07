// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "Who.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Side.h"
#include "dispatcher/Equipment.h"
#include "dispatcher/Humans.h"
#include "clients_kernel/Karma.h"
#include <xeumeuleu/xml.h>
#include <boost/bind.hpp>

using namespace plugins::bml;

#include "protocol/protocol.h"

using namespace Common;
using namespace MsgsSimToClient;

// -----------------------------------------------------------------------------
// Name: Who constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Who::Who( const dispatcher::Agent& entity )
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
Who::Who( const dispatcher::Automat& entity )
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
Who::Who( const dispatcher::Agent& entity, int detectionLevel )
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
    int ComputeLevel( const MsgUnitAttributes& attributes )
    {
        return attributes.has_etat_operationnel() 
            || attributes.has_dotation_eff_materiel() 
            || attributes.has_dotation_eff_personnel() 
            ? identified : -1;
    }
}

// -----------------------------------------------------------------------------
// Name: Who constructor
// Created: SBO 2008-07-22
// -----------------------------------------------------------------------------
Who::Who( const dispatcher::Agent& entity, const MsgUnitAttributes& attributes )
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
                    << xml::content( "jc3iedm:OID", who.agent_ ? who.agent_->name_ : who.automat_->GetName().ascii() );
    if( who.attributes_ )
        who.SendEquipmentStatus( xos );
    if( who.level_ != -1 )
        who.SendStatus( xos );
    xos         << xml::end()
            << xml::end()
        << xml::end();
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
                << xml::end()
                << xml::start( "jc3iedm:ObjectItemCapability" )
                    << xml::content( "jc3iedm:Quantity", quantity )
                    << xml::content( "jc3iedm:ReportingDataRef", "" )
                << xml::end()
            << xml::end();
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
    if( attributes_->has_dotation_eff_materiel() )
    {
        AvailabilityComputer computer;
        agent_->equipments_.Apply( boost::bind( &AvailabilityComputer::AddEquipment, boost::ref( computer ), _1 ) );
        SerializeAvailability( "SPLC2", xos, computer.Percentage() );
    }
    if( attributes_->has_dotation_eff_personnel() )
    {
        AvailabilityComputer computer;
        agent_->troops_.Apply( boost::bind( &AvailabilityComputer::AddHuman, boost::ref( computer ), _1 ) );
        SerializeAvailability( "PERSVC", xos, computer.Percentage() );
    }
    xos << xml::end();
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
            << xml::end()
        << xml::end();
}

namespace
{
    std::string Hostility( const dispatcher::Automat& entity )
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

    std::string OperationalState( const EnumOperationalStatus& status )
    {
        switch( status )
        {
            case detruit_tactiquement:
            case detruit_totalement:
                return "NOP";
            case operationnel:
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
    if( level_ > detected )
    {
        if( agent_ )
            return Hostility( *agent_->automat_ );
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
    if( agent_ && level_ > recognized )
        return OperationalState( agent_->nOperationalState_ );
    return "NKN";
}
