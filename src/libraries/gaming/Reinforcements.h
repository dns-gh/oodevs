// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Reinforcements_h_
#define __Reinforcements_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include <tools/Resolver_ABC.h>

namespace sword
{
    class UnitAttributes;
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
/** @class  Reinforcements
    @brief  Reinforcements
*/
// Created: AGE 2006-02-13
// =============================================================================
class Reinforcements : public kernel::Extension_ABC
                     , public kernel::Updatable_ABC< sword::UnitAttributes >
{
public:
    //! @name Constructors/Destructor
    //@{
             Reinforcements( kernel::Entity_ABC& entity, kernel::Controller& controller, const tools::Resolver_ABC< kernel::Agent_ABC >& resolver, gui::PropertiesDictionary& dico );
    virtual ~Reinforcements();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Reinforcements( const Reinforcements& );            //!< Copy constructor
    Reinforcements& operator=( const Reinforcements& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( gui::PropertiesDictionary& dico ) const;
    virtual void DoUpdate( const sword::UnitAttributes& message );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::Agent_ABC* > T_Agents;
    //@}

public:
    //! @name Member data
    //@{
    kernel::Entity_ABC& entity_;
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Agent_ABC >& resolver_;

    const kernel::Agent_ABC* reinforced_;
    T_Agents reinforcements_;
    //@}
};

#endif // __Reinforcements_h_
