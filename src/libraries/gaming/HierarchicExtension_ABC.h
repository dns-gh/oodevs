// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HierarchicExtension_ABC_h_
#define __HierarchicExtension_ABC_h_


#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "tools/Resolver_ABC.h"

namespace Common
{
    class MsgFormationCreation;
    class MsgUnitChangeSuperior;
    class MsgAutomatChangeSuperior;
}

namespace kernel
{
    class Entity_ABC;
    class Automat_ABC;
    class Formation_ABC;
    class Team_ABC;
}

namespace MsgsSimToClient
{
    class MsgUnitCreation;
    class MsgAutomatCreation;
}

// =============================================================================
/** @class  HierarchicExtension_ABC
    @brief  HierarchicExtension_ABC
*/
// Created: SBO 2007-04-12
// =============================================================================
class HierarchicExtension_ABC : public kernel::Updatable_ABC< MsgsSimToClient::MsgUnitCreation >
                              , public kernel::Updatable_ABC< MsgsSimToClient::MsgAutomatCreation >
                              , public kernel::Updatable_ABC< Common::MsgFormationCreation >
                              , public kernel::Updatable_ABC< Common::MsgUnitChangeSuperior >
                              , public kernel::Updatable_ABC< Common::MsgAutomatChangeSuperior >
{
public:
    //! @name Constructors/Destructor
    //@{
             HierarchicExtension_ABC( const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver
                                    , const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver
                                    , const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver);
    virtual ~HierarchicExtension_ABC();
    //@}

    //! @name Operations
    //@{
    const kernel::Entity_ABC* GetSuperior() const;
    virtual void SetSuperior( const kernel::Entity_ABC& superior ) = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    HierarchicExtension_ABC( const HierarchicExtension_ABC& );            //!< Copy constructor
    HierarchicExtension_ABC& operator=( const HierarchicExtension_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgUnitCreation& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgAutomatCreation& message );
    virtual void DoUpdate( const Common::MsgFormationCreation& message );
    virtual void DoUpdate( const Common::MsgUnitChangeSuperior& message );
    virtual void DoUpdate( const Common::MsgAutomatChangeSuperior& message );

    void UpdateSuperior( const kernel::Entity_ABC& superior );
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::Automat_ABC >&   automatResolver_;
    const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver_;
    const tools::Resolver_ABC< kernel::Team_ABC >&      teamResolver_;
    const kernel::Entity_ABC* superior_;
    //@}
};

#endif // __HierarchicExtension_ABC_h_
