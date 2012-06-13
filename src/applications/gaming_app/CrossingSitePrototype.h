// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrossingSitePrototype_h_
#define __CrossingSitePrototype_h_

#include "clients_gui/CrossingSitePrototype_ABC.h"

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }
}

// =============================================================================
/** @class  CrossingSitePrototype
    @brief  CrossingSitePrototype
*/
// Created: SBO 2006-04-19
// =============================================================================
class CrossingSitePrototype : public gui::CrossingSitePrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             CrossingSitePrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList );
    virtual ~CrossingSitePrototype();
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

#endif // __CrossingSitePrototype_h_
