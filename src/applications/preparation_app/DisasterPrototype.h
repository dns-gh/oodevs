// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DisasterPrototype_h_
#define __DisasterPrototype_h_

#include "clients_gui/DisasterPrototype_ABC.h"

namespace kernel
{
    class Object_ABC;
}

// =============================================================================
/** @class  DisasterPrototype
    @brief  Disaster prototype
*/
// Created: LGY 2012-11-20
// =============================================================================
class DisasterPrototype : public gui::DisasterPrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DisasterPrototype( QWidget* parent, const tools::GeneralConfig& config, kernel::Object_ABC*& creation );
    virtual ~DisasterPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit( const kernel::Team_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    //@}
};

#endif // __DisasterPrototype_h_
