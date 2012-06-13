// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __DelayPrototype_h_
#define __DelayPrototype_h_

#include "clients_gui/DelayPrototype_ABC.h"

namespace kernel
{
    class Object_ABC;
}

// =============================================================================
/** @class  DelayPrototype
    @brief  DelayPrototype
*/
// Created: JSR 2010-07-06
// =============================================================================
class DelayPrototype : public gui::DelayPrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DelayPrototype( QWidget* parent, kernel::Object_ABC*& creation );
    virtual ~DelayPrototype();
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

#endif // __DelayPrototype_h_
