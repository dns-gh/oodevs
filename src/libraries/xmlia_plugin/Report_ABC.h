// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Report_h_
#define __Report_h_

#include "game_asn/Simulation.h"

namespace dispatcher
{
  class Automat;
  class Agent;
  class Side;
  class Object;
}

namespace kernel
{
  class MissionType;
}

namespace xml
{
  class xistream;
  class xostream;
}

namespace plugins
{
  namespace xmlia
  {
    class Point;
    class ReportManager;
    class Unit_ABC;
    class UnitAutomate;
    class UnitAgent;
    // =============================================================================
    /** @class  Rapport
    @brief  Rapport
    */
    // Created: MGD 2009-06-12
    // =============================================================================
    class Report_ABC
    {

    public:
      //! @name Destructor
      //@{
      virtual ~Report_ABC();
      //@}

      ///! @name Operations
      //@{
      void Serialize( xml::xostream& xos ) const;
      //@}

      ///! @name Event
      //@{
      virtual void InsertOrUpdateFriendly( dispatcher::Agent& agent ){};
      virtual void InsertOrUpdateEnemy( dispatcher::Agent& agent ){};
      virtual void InsertOrUpdateNBC( dispatcher::Object& agent ){};
      virtual void UpdateMission( kernel::MissionType& mission, std::vector< Point >& limit1, std::vector< Point >& limit2 ){};
      virtual void UpdateSimulation() = 0 ;
      //@}

    protected:
      //! @name Constructors
      //@{
      Report_ABC( ReportManager& manager, xml::xistream& xis, const std::string& type );
      Report_ABC( ReportManager& manager, const dispatcher::Automat& author, const std::string& type );
      //@}

    private:
      ///! @name Operations
      //@{
      void SerializeHeader( xml::xostream& xos ) const;
      virtual void SerializeContent( xml::xostream& xos ) const = 0 ;
      //@}

    protected:
      //! @name Member data
      //@{
      const std::string type_;
      const std::string serializedContent_;
      UnitAutomate* author_;
      Unit_ABC* dest_;
      ReportManager& reportManager_;

      //@}
    };

  }// xmlia

}//plugin

#endif // __Report_h_
