// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_ROLE_PION_COMPOSANTES_H
#define SWORD_ROLE_PION_COMPOSANTES_H

#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"

namespace core
{
    class Model;
}

namespace sword
{
// =============================================================================
/** @class  RolePion_Composantes
    @brief  Role pion composantes
*/
// Created: SLI 2012-03-22
// =============================================================================
class RolePion_Composantes : public PHY_RolePion_Composantes
{
public:
    //! @name Constructors/Destructor
    //@{
             RolePion_Composantes();
             RolePion_Composantes( MIL_Agent_ABC& pion, core::Model& model );
    virtual ~RolePion_Composantes();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyComposanteAdded( PHY_ComposantePion& composante, T_Dotations* dotations );
    virtual T_Dotations NotifyComposanteRemoved( PHY_ComposantePion& composante );

    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

private:
    //! @name Member data
    //@{
    core::Model* entity_; // $$$$ MCO 2013-05-27: rename to components_
    core::Model* equipments_;
    std::map< PHY_ComposantePion*, core::Model* > components_; // $$$$ MCO 2013-05-27: rename to composantes_
    //@}
};

}

BOOST_CLASS_EXPORT_KEY( sword::RolePion_Composantes )

#endif // SWORD_ROLE_PION_COMPOSANTES_H
