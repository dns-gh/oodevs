// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_RoleAction_FolkInfluence_h_
#define __PHY_RoleAction_FolkInfluence_h_

#include "MT_Tools/Role_ABC.h"

// =============================================================================
/** @class  PHY_RoleAction_FolkInfluence
    @brief  PHY_RoleAction_FolkInfluence
*/
// Created: JCR 2007-09-12
// =============================================================================
class PHY_RoleAction_FolkInfluence : public tools::Role_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_FolkInfluence RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleAction_FolkInfluence();
    virtual ~PHY_RoleAction_FolkInfluence();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive& ar, const unsigned int )
    {
        ar & boost::serialization::base_object< tools::Role_ABC >( *this );
    }
    //@}

    //! @name
    //@{
    void Update( bool bIsDead );
    void Clean();
    //@}

public:
    //! @name Types
    //@{
    enum E_ReturnCode
    {
        eRunning        , // -> l'action est en cours d'exécution
        eActivated      , // -> l'action est terminée
        eDeactivated    , // -> l'action est terminée
        eImpossible       // -> l'action est impossible (cas arrivant, par exemple, quand on essaye de construire une zone d'influence dans une zone vide)
    };
};

BOOST_CLASS_EXPORT_KEY( PHY_RoleAction_FolkInfluence )

#endif // __PHY_RoleAction_FolkInfluence_h_
