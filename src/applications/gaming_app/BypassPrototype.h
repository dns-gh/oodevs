// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BypassPrototype_h_
#define __BypassPrototype_h_

#include "clients_gui/BypassPrototype_ABC.h"

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }
}

// =============================================================================
/** @class  BypassPrototype
    @brief  BypassPrototype
*/
// Created: NPT 2012-09-05
// =============================================================================
class BypassPrototype : public gui::BypassPrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             BypassPrototype(  QWidget* parent, actions::parameters::ParameterList*& attributesList  );
    virtual ~BypassPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit( const kernel::Team_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    actions::parameters::ParameterList*& attributesList_;
    //@}
};

#endif // __BypassPrototype_h_
