// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectFactory_h_
#define __ObjectFactory_h_

#include "ObjectFactory_ABC.h"

namespace kernel
{
    class Controllers;
}

class Model;
class StaticModel;

// =============================================================================
/** @class  ObjectFactory
    @brief  Object factory
*/
// Created: AGE 2006-02-13
// =============================================================================
class ObjectFactory : public ObjectFactory_ABC
{
private:
   enum EnumObjectType {
      bouchon_mines = 0,
      zone_minee_lineaire = 1,
      zone_minee_par_dispersion = 2,
      fosse_anti_char = 3,
      abattis = 4,
      barricade = 5,
      eboulement = 6,
      destruction_route = 7,
      destruction_pont = 8,
      pont_flottant_continu = 9,
      poste_tir = 10,
      zone_protegee = 11,
      zone_implantation_canon = 12,
      zone_implantation_cobra = 13,
      zone_implantation_lrm = 14,
      site_franchissement = 15,
      nuage_nbc = 16,
      plot_ravitaillement = 17,
      site_decontamination = 18,
      zone_brouillage_brod = 19,
      rota = 20,
      zone_nbc = 21,
      zone_brouillage_bromure = 22,
      aire_poser = 23,
      piste = 24,
      plateforme = 25,
      zone_mobilite_amelioree = 26,
      zone_poser_helicoptere = 27,
      aire_logistique = 28,
      itineraire_logistique = 29,
      camp_prisonniers = 30,
      camp_refugies = 31,
      poste_controle = 32,
      terrain_largage = 33,
      zone_interdite_mouvement = 34,
      zone_interdite_tir = 35,
      zone_implantation_mortier = 36,
      pont_flottant_discontinu = 37,
      installation = 38
   };

public:
    //! @name Constructors/Destructor
    //@{
             ObjectFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel );
    virtual ~ObjectFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Object_ABC* CreateObject( kernel::ObjectType& type, kernel::Team_ABC& team, const kernel::Location_ABC& location );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectFactory( const ObjectFactory& );            //!< Copy constructor
    ObjectFactory& operator=( const ObjectFactory& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const StaticModel& static_;
    //@}
};

#endif // __ObjectFactory_h_
