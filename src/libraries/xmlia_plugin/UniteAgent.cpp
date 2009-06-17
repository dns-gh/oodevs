// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "UniteAgent.h"

#include "dispatcher/Side.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Agent.h"

#include "Point.h"
#include "EtatOperationnel.h"

#include "boost/bind.hpp"
#include <xeumeuleu/xml.h>

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: UniteAgent constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
UniteAgent::UniteAgent( xml::xistream& xis )
: Unite_ABC( xis )
{
  //xis >> xml::end(); //</Unite>
  //localisation_ = new Point( xis );
  //etatOps_ = new EtatOperationnel( xis );
}

// -----------------------------------------------------------------------------
// Name: UniteAgent constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
UniteAgent::UniteAgent( dispatcher::Agent& agent )
: Unite_ABC( agent.GetId(), agent.name_ )
, idSide_(agent.automat_->team_.GetId() )
{
  localisation_ = new Point( agent.position_.latitude, agent.position_.longitude, id_ );
  etatOps_      = new EtatOperationnel( agent, QName() );
}

// -----------------------------------------------------------------------------
// Name: UniteAgent destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
UniteAgent::~UniteAgent()
{
    delete localisation_;
    delete etatOps_;
}

// -----------------------------------------------------------------------------
// Name: UniteAgent::SerializeExtension
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void UniteAgent::SerializeExtension( xml::xostream& xos, const std::string& sQnameRapport  ) const
{
  std::ostringstream os;
  os << id_;
  std::string sId = os.str();
  std::string sQnameEtatOps = "etat-" + sId;
  std::string sQnameAssociationLoca = "asso-loca-" + sId;

  xos << xml::start( "mpia:APourEtat_EtatOperationnel" )
        << xml::content( "mpia:refid", sQnameEtatOps )
      << xml::end()
      << xml::start( "mpia:EstLocalisePar_AssocInstanceObjetLocalisation" )
        << xml::content( "mpia:refid", sQnameAssociationLoca )
      << xml::end();
}

// -----------------------------------------------------------------------------
// Name: UniteAgent::SerializeInfo
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void UniteAgent::SerializeIncludeEntities( xml::xostream& xos, const std::string& sQnameRapport  ) const
{
  SerializeSideAssociation( xos );
  etatOps_->Serialize( xos, sQnameRapport );
  localisation_->Serialize( xos, sQnameRapport );
  SerializeAssociationLocalisation( xos );
}

// -----------------------------------------------------------------------------
// Name: UniteAgent::SerializeEtatOps
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void UniteAgent::SerializeSideAssociation( xml::xostream& xos ) const
{
  std::ostringstream os;
  os << id_;
  std::string sIdUniteAgent = os.str();
  std::ostringstream os2;
  os2 << idSide_;
  std::string sIdSide = os2.str();

  xos << xml::start( "mpia:AssociationAffiliationInstanceObjet" )
    << xml::attribute( "id", "association-" + sIdSide + "-" + sIdUniteAgent)
    << xml::start( "mpia:Concerne_Affiliation" )
    << xml::content( "mpia:refid", "camp-" + sIdSide )
    << xml::end()
    << xml::start( "mpia:Concerne_InstanceObjet" )
    << xml::content( "mpia:refid", "UniteAgent-" + sIdUniteAgent )
    << xml::end()
    << xml::end();
}

// -----------------------------------------------------------------------------
// Name: UniteAgent::SerializeAssociationLocalisation
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void UniteAgent::SerializeAssociationLocalisation( xml::xostream& xos ) const
{
  std::ostringstream os;
  os << id_;
  std::string sId = os.str();
  std::string sQnameAssociationLoca = "asso-loca-" + sId;
  std::string sQnameLoca = "loca-" + sId;
  std::string sQnameUniteAgent = "UniteAgent-" + sId;

  xos << xml::start( "mpia:AssocInstanceObjetLocalisation" )
        << xml::attribute( "id", sQnameAssociationLoca )
        << xml::content( "mpia:Categorie", "CEOFMA" )//@TODOFORCE Value
        << xml::start( "mpia:EstLocalisation_InstanceObjet")
          << xml::content( "mpia:refid", sQnameUniteAgent )
        << xml::end()
        << xml::start( "mpia:EstLocaliseEn_Localisation")
          << xml::content( "mpia:refid", sQnameLoca )
        << xml::end()
      << xml::end();
}

// -----------------------------------------------------------------------------
// Name: UniteAgent::Update
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void UniteAgent::Update( dispatcher::Agent& agent )
{
  id_        = agent.GetId();
  name_      = agent.name_;
  idSide_    = agent.automat_->team_.GetId();
  
  localisation_->Update( agent );
  etatOps_->Update( agent );
}

// -----------------------------------------------------------------------------
// Name: UniteAgent::IsSide
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
bool UniteAgent::IsSide( unsigned int idSide) const
{
  return idSide == idSide_;
}

// -----------------------------------------------------------------------------
// Name: UniteAgent::GetLocalization
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
Point* UniteAgent::GetLocalization() const
{
  return localisation_;
}

// -----------------------------------------------------------------------------
// Name: UniteAgent::GetOperationalState
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
EtatOperationnel* UniteAgent::GetOperationalState() const
{
  return etatOps_;
}
}

// -----------------------------------------------------------------------------
// Name: UniteAgent::SetPosition
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void UniteAgent::SetPosition( Point* pt )
{
  localisation_ = pt;
}

// -----------------------------------------------------------------------------
// Name: UniteAgent::SetEtatOps
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void UniteAgent::SetEtatOps( EtatOperationnel* etatOps )
{
  etatOps_ = etatOps;
}