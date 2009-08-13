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

#include "MIL.h"
#include "MT_Tools/Role_ABC.h"

namespace population
{
    class ObjectManager_ABC;
    class Object_ABC;
}

class MIL_AgentPion;

// =============================================================================
/** @class  PHY_RoleAction_FolkInfluence
    @brief  PHY_RoleAction_FolkInfluence
*/
// Created: JCR 2007-09-12
// =============================================================================
class PHY_RoleAction_FolkInfluence : public tools::Role_ABC
                                   , private boost::noncopyable
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
    template< typename Archive > void serialize( Archive&, const uint ) {}
    //@}

    //! @name 
    //@{
    void Update ( bool bIsDead );
    void Clean  ();
    //@}

    //! @name Operations
    //@{
    population::Object_ABC* InfluenceActivity( const MT_Vector2D& vPos, const std::string& activity, double influence, int connectivity );
    population::Object_ABC* InfluenceActivityInArea( const MT_Vector2D& vPos, const std::string& activity, double influence, const TER_Localisation& area );
    void                    ReleaseInfluence( population::Object_ABC& object );
    //@}

public:
    //! @name Types
    //@{
    enum E_ReturnCode
    {
        eRunning        , // -> l'action est en cours d'exécution
        eActivated      , // -> l'action est terminée
        eDeactivated    , // -> l'action est terminée
        eImpossible     , // -> l'action est impossible (cas arrivant, par exemple, quand on essaye de construire une zone d'influence dans une zone vide)
    };

private:
    //! @name Member data
    //@{
    population::ObjectManager_ABC* pManager_;
    //@}
};

#endif // __PHY_RoleAction_FolkInfluence_h_
