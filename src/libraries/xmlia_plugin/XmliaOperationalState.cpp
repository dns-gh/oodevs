// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "XmliaOperationalState.h"

#include "dispatcher/Agent.h"
#include "dispatcher/Equipment.h"
#include "dispatcher/Humans.h"
#include "dispatcher/Dotation.h"

#include <xeumeuleu/xml.h>
#include "boost/bind.hpp"

using namespace plugins::xmlia;

namespace
{
  std::string GetXmliaOperationalStateFromAsn( const ASN1T_EnumOperationalStatus& status )
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

  ASN1T_EnumOperationalStatus GetAsnOperationalStateFromXmlia( const std::string& status )
  {
    if( status == "OPR")
      return EnumOperationalStatus::operationnel;
    else if( status == "NOP")
      return EnumOperationalStatus::detruit_tactiquement;
    else
      return EnumOperationalStatus::detruit_totalement;
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
    void AddDotation( const dispatcher::Dotation& dotation )
    {
      //@TODO, no dotation public data in dispatcher
    }

    unsigned int GetValue()
    {
      if( count_ == 0 ) return 4;
      float rapport = available_ < 0 ? 0 : available_ * 100 / count_;
      if( rapport < 0.2) return 0;
      else if( rapport < 0.4) return 1;
      else if( rapport < 0.6) return 2;
      else if( rapport < 0.8) return 3;
      else return 4;
    }
    unsigned int count_;
    int available_;
  };
}

// -----------------------------------------------------------------------------
// Name: XmliaOperationalState constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
XmliaOperationalState::XmliaOperationalState( xml::xistream& xis )
{
   xis >> xml::start( "EtatOperationnelEntiteOrganisationnelle" )
        >> xml::content( "EtatCarburants", iEtatCarburants_ )
        >> xml::content( "EtatEquipements", iEtatEquipements_)
        >> xml::content( "EtatMunitions", iEtatMunitions_)
        >> xml::content( "EtatOperationnel", etatOpsGeneral_ )
        >> xml::content( "EtatPersonnel", iEtatPersonnel_)
       >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: XmliaOperationalState constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
XmliaOperationalState::XmliaOperationalState(  const dispatcher::Agent& agent  )
: Entity_ABC()
, etatOpsGeneral_( GetXmliaOperationalStateFromAsn( agent.nOperationalState_ ) )
{
  AvailabilityComputer computer1;
  agent.equipments_.Apply( boost::bind( &AvailabilityComputer::AddEquipment, boost::ref( computer1 ), _1 ) );
  iEtatEquipements_ = computer1.GetValue();
  AvailabilityComputer computer2;
  agent.troops_.Apply( boost::bind( &AvailabilityComputer::AddHuman, boost::ref( computer2 ), _1 ) );
  iEtatPersonnel_ = computer2.GetValue();
  //@TODO add Dotation management
  iEtatCarburants_ = 4;
  iEtatMunitions_ = 4;
}

// -----------------------------------------------------------------------------
// Name: XmliaOperationalState destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
XmliaOperationalState::~XmliaOperationalState()
{
  // NOTHING
}

// -----------------------------------------------------------------------------
// Name: XmliaOperationalState::Serialize
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void XmliaOperationalState::Serialize( xml::xostream& xos ) const
{
  xos << xml::start( "EtatOperationnelEntiteOrganisationnelle" )
        << xml::content( "Comportement", "NEUTRL" )
        << xml::content( "EtatCarburants", iEtatCarburants_ )
        << xml::content( "EtatEquipements", iEtatEquipements_)
        << xml::content( "EtatMunitions", iEtatMunitions_)
        << xml::content( "EtatOperationnel", etatOpsGeneral_ )
        << xml::content( "EtatPersonnel", iEtatPersonnel_)
      << xml::end();
}

// -----------------------------------------------------------------------------
// Name: XmliaOperationalState::Update
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void XmliaOperationalState::Update( dispatcher::Agent& agent )
{
  etatOpsGeneral_ = GetXmliaOperationalStateFromAsn( agent.nOperationalState_ );

  AvailabilityComputer computer1;
  agent.equipments_.Apply( boost::bind( &AvailabilityComputer::AddEquipment, boost::ref( computer1 ), _1 ) );
  iEtatEquipements_ = computer1.GetValue();

  AvailabilityComputer computer2;
  agent.troops_.Apply( boost::bind( &AvailabilityComputer::AddHuman, boost::ref( computer2 ), _1 ) );
  iEtatPersonnel_ = computer2.GetValue();

  //@TODO add Dotation
  iEtatCarburants_ = 4;
  iEtatMunitions_  = 4;
}

// -----------------------------------------------------------------------------
// Name: XmliaOperationalState GetGeneralOperationalState
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
const std::string& XmliaOperationalState::GetGeneralOperationalState() const
{
  return etatOpsGeneral_;
}

// -----------------------------------------------------------------------------
// Name: XmliaOperationalState GetAverageDotation
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
float XmliaOperationalState::GetAverageDotation() const
{
  return ( iEtatCarburants_ + iEtatMunitions_ ) / 2;
}

