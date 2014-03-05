// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Transports_h_
#define __Transports_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include <tools/Resolver_ABC.h>

namespace sword
{
    class UnitAttributes;
    class UnitDestruction;
}

namespace kernel
{
    class Agent_ABC;
    class Controller;
    class Displayer_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

// =============================================================================
/** @class  Transports
    @brief  Transports
*/
// Created: AGE 2006-02-13
// =============================================================================
class Transports : public kernel::Extension_ABC
                 , public kernel::Updatable_ABC< sword::UnitAttributes >
                 , public kernel::Updatable_ABC< sword::UnitDestruction >
{
public:
    //! @name Constructors/Destructor
    //@{
             Transports( kernel::Entity_ABC& entity, kernel::Controller& controller, const tools::Resolver_ABC< kernel::Agent_ABC >& resolver, gui::PropertiesDictionary& dico );
    virtual ~Transports();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    bool CanRetrieveTransporters() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Transports( const Transports& );            //!< Copy constructor
    Transports& operator=( const Transports& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::Agent_ABC* > T_Agents;
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( gui::PropertiesDictionary& dico ) const;
    virtual void DoUpdate( const sword::UnitAttributes& message );
    virtual void DoUpdate( const sword::UnitDestruction& destruction );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Entity_ABC& entity_;
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Agent_ABC >& resolver_;
    const kernel::Agent_ABC* transporter_;
    T_Agents transported_;
    bool transportersReady_;
    //@}
};

#endif // __Transports_h_
