// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Rapport_h_
#define __Rapport_h_

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
    class RapportManager;
    class Unite_ABC;
    class UniteAgent;
    // =============================================================================
    /** @class  Rapport
    @brief  Rapport
    */
    // Created: MGD 2009-06-12
    // =============================================================================
    class Rapport
    {

    public:
      //! @name Destructor
      //@{
      virtual ~Rapport();
      //@}

      ///! @name Operations
      //@{
      std::string QName() const;
      void Serialize( xml::xostream& xos ) const;
      virtual void SerializeOtherEntities( xml::xostream& xos) const = 0;
      void SerializeSides( xml::xostream& xos, std::string sQnameRapport ) const;
      virtual void SerializeSide( const dispatcher::Side& side, xml::xostream& xos, std::string sQnameRapport ) const = 0;
      virtual void ReadEntities( xml::xistream& xis ) = 0;
      //@}

      ///! @name Event
      //@{
      virtual void InsertOrUpdate( dispatcher::Agent& agent ){};//@TODO rename
      virtual void UpdateSimulation() = 0 ;
      //@}

    protected:
      //! @name Constructors
      //@{
      Rapport( RapportManager& manager, xml::xistream& xis );
      Rapport( RapportManager& manager, const dispatcher::Automat& author, const std::string& type );
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
      Unite_ABC* author_;
      Unite_ABC* dest_;
      RapportManager& rapportManager_;
      int cpt_;
      std::map< unsigned, UniteAgent* > unites_;
      //@}
    };

  }// xmlia

}//plugin

#endif // __Rapport_h_
