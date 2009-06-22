// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __XMLIA_OperationalState_h_
#define __XMLIA_OperationalState_h_

#include "Entity_ABC.h"

namespace plugins
{
  namespace xmlia
  {

    // =============================================================================
    /** @class  OperationalState
    @brief  OperationalState
    */
    // Created: MGD 2009-06-12
    // =============================================================================
    class XmliaOperationalState : public Entity_ABC
    {

    public:
      //! @name Constructors/Destructor
      //@{
      XmliaOperationalState( xml::xistream& xis );
      XmliaOperationalState( const dispatcher::Agent& agent, const std::string& sQnameParent );
      virtual ~XmliaOperationalState();
      //@}

      ///! @name Operations
      //@{
      void Serialize( xml::xostream& xos, const std::string& sQnameRapport ) const;
      virtual std::string QName() const;
      void Update( dispatcher::Agent& agent );
      unsigned int GetId() const;
      std::string& GetGeneralOperationalState();
      //@}

    private:
      //! @name Member data
      //@{
      unsigned int id_;
      std::string sQname_;
      std::string sQnameParent_;

      unsigned int iEtatCarburants_;
      unsigned int iEtatEquipements_;
      unsigned int iEtatMunitions_;
      unsigned int iEtatPersonnel_;
      std::string etatOpsGeneral_;
      //@}
    };

  }// xmlia

}//plugin

#endif // __XMLIA_OperationalState_h_
