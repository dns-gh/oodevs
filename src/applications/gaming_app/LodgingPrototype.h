// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LodgingPrototype_h_
#define __LodgingPrototype_h_

#include "clients_gui/LodgingPrototype_ABC.h"

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }
}

// =============================================================================
/** @class  LodgingPrototype
@brief  LodgingPrototype
*/
// Created: MMC 2011-05-02
// =============================================================================
class LodgingPrototype : public gui::LodgingPrototype_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
    LodgingPrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList );
    virtual ~LodgingPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    //@}

private:
    //! @name Member Data
    //@{
    actions::parameters::ParameterList*& attributesList_;
    //@}
};

#endif // __LodgingPrototype_h_
