// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActivityTimePrototype_h_
#define __ActivityTimePrototype_h_

#include "clients_gui/ActivityTimePrototype_ABC.h"

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }
}

// =============================================================================
/** @class  ActivityTimePrototype
    @brief  ActivityTimePrototype
*/
// Created: SBO 2006-04-19
// =============================================================================
class ActivityTimePrototype : public gui::ActivityTimePrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ActivityTimePrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList );
    virtual ~ActivityTimePrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    //@}

private:
    //! @name Member data
    //@{
    actions::parameters::ParameterList*& attributesList_;
    //@}
};

#endif // __ActivityTimePrototype_h_
