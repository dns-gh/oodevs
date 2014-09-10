// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __clientsgui_Team_h_
#define __clientsgui_Team_h_

#include "EntityImplementation.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/Team_ABC.h"

namespace kernel
{
    class OptionVariant;
    class Controllers;
}

namespace gui
{
// =============================================================================
/** @class  Team
    @brief  Team
*/
// Created: LDC 2012-05-07
// =============================================================================
class Team : public gui::EntityImplementation< kernel::Team_ABC >
           , public kernel::OptionsObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Team( kernel::Controllers& controller,
                   unsigned long id,
                   const QString& name,
                   const T_CanBeRenamedFunctor& canBeRenamedFunctor = T_CanBeRenamedFunctor() );
    virtual ~Team();
    //@}

    //! @name Operations
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

protected:
    //! @name Attributes
    //@{
    kernel::Controllers& controllers_;
    //@}
};
}

#endif // __clientsgui_Team_h_
