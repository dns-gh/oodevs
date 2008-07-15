// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "PositionReport.h"
#include "SerializationTools.h"
#include "Publisher_ABC.h"
#include "Who.h"
#include "Where.h"
#include "ReportingData.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Equipment.h"
#include "dispatcher/Humans.h"
#include <xeumeuleu/xml.h>
#include <boost/bind.hpp>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: PositionReport constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
PositionReport::PositionReport( const dispatcher::Agent& entity, const ASN1T_MsgUnitAttributes& attributes )
    : entity_( entity )
    , attributes_( attributes )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PositionReport destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
PositionReport::~PositionReport()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PositionReport::Send
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void PositionReport::Send( Publisher_ABC& publisher ) const
{
    Who who( entity_ );
    xml::xostream& xos = publisher.CreateReport();
    xos << xml::start( "ReportPush" )
            << xml::start( "ReportedByWho" ) << who << xml::end()
            << xml::start( "NewWhere" )      << who << Where( entity_ ) << xml::end()
            << xml::start( "ReportedWho" )   << who << xml::end();
    SendEquipmentStatus( xos );
    SendStatus( xos );
    xos << xml::end();
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
// Name: PositionReport::SendEquipmentStatus
// Created: SBO 2008-07-15
// -----------------------------------------------------------------------------
void PositionReport::SendEquipmentStatus( xml::xostream& xos ) const
{
    if( attributes_.m.dotation_eff_materielPresent )
    {
        AvailabilityComputer computer;
        entity_.equipments_.Apply( boost::bind( &AvailabilityComputer::AddEquipment, boost::ref( computer ), _1 ) );
        SerializeAvailability( "SPLC2", xos, computer.Percentage() );
    }
    if( attributes_.m.dotation_eff_personnelPresent )
    {
        AvailabilityComputer computer;
        entity_.troops_.Apply( boost::bind( &AvailabilityComputer::AddHuman, boost::ref( computer ), _1 ) );
        SerializeAvailability( "PERSVC", xos, computer.Percentage() );
    }
}

namespace
{
    std::string ComputeOperationalStatus( const ASN1T_EnumOperationalStatus& status )
    {
        switch( status )
        {
            case EnumOperationalStatus::detruit_tactiquement:
            case EnumOperationalStatus::detruit_totalement:
                return "NOP";
            case EnumOperationalStatus::operationnel:
                return "OPR";
            default:
                break;
        }
        return "NKN";
    }
}

// -----------------------------------------------------------------------------
// Name: PositionReport::SendStatus
// Created: SBO 2008-07-15
// -----------------------------------------------------------------------------
void PositionReport::SendStatus( xml::xostream& xos ) const
{
    if( attributes_.m.etat_operationnelPresent == 0 )
        return;
    xos << xml::start( "jc3iedm:StatusList" )
            << xml::start( "jc3iedm:Status" ) 
                << xml::attribute( "xsi:type", "jc3iedm:OrganisationStatus" )
                << xml::content( "jc3iedm:HostilityCode", "FR" ) // $$$$ SBO 2008-07-15: karma
                << xml::content( "jc3iedm:OperationalStatusCode", ComputeOperationalStatus( entity_.nOperationalState_ ) )
                << xml::content( "jc3iedm:ReportingDataRef", "" )
            << xml::end()
        << xml::end();
}
