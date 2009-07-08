// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
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
      XmliaOperationalState( const dispatcher::Agent& agent );
      virtual ~XmliaOperationalState();
      //@}

      ///! @name Operations
      //@{
      void Serialize( xml::xostream& xos ) const;
      void Update( dispatcher::Agent& agent );
      std::string& GetGeneralOperationalState();
      //@}

    private:
      //! @name Member data
      //@{
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
