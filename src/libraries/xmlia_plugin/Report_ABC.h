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

namespace dispatcher
{
  class Automat;
  class Agent;
  class Side;
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
    class ReportManager;
    class Unit_ABC;
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
      std::string QName() const;
      void Serialize( xml::xostream& xos ) const;
      virtual void SerializeOtherEntities( xml::xostream& xos) const = 0;
      void SerializeSides( xml::xostream& xos, std::string sQnameRapport ) const;
      virtual void SerializeSide( const dispatcher::Side& side, xml::xostream& xos, std::string sQnameRapport ) const = 0;
      virtual void ReadEntities( xml::xistream& xis ) = 0;
      virtual unsigned int Report_ABC::GetAuthorID() const;
      //@}

      ///! @name Event
      //@{
      virtual void InsertOrUpdate( dispatcher::Agent& agent ){};//@TODO rename
      virtual void UpdateSimulation() = 0 ;
      //@}

    protected:
      //! @name Constructors
      //@{
      Report_ABC( ReportManager& manager, xml::xistream& xis );
      Report_ABC( ReportManager& manager, const dispatcher::Automat& author, const std::string& type );
      //@}

    private:
      ///! @name Operations
      //@{
      void SerializeDestinataires( xml::xostream& xos, const std::string sQNameRapport ) const;
      void ReadUnites( xml::xistream& xis );
      //@}

    protected:
      //! @name Member data
      //@{
      const std::string type_;
      const std::string serializedContent_;
      Unit_ABC* author_;
      Unit_ABC* dest_;
      ReportManager& reportManager_;
      int cpt_;
      std::map< unsigned, UnitAgent* > unites_;
      //@}
    };

  }// xmlia

}//plugin

#endif // __Report_h_
