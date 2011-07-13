// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UndergroundPrototype_ABC_h_
#define __UndergroundPrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"
#include "tools/Observer_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controller;
    class UndergroundAttribute_ABC;
}

namespace gui
{
    class LoadableComboBox;
    class RichLabel;

// =============================================================================
/** @class  UndergroundPrototype_ABC
    @brief  UndergroundPrototype_ABC
*/
// Created: JSR 2011-07-07
// =============================================================================
class UndergroundPrototype_ABC : public ObjectAttributePrototype_ABC
                               , public tools::Observer_ABC
                               , public tools::ElementObserver_ABC< kernel::UndergroundAttribute_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UndergroundPrototype_ABC( QWidget* parent, kernel::Controller& controller );
    virtual ~UndergroundPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void SetLoader( ObjectPrototypeLoader_ABC* loader );
    virtual void NotifyUpdated( const kernel::UndergroundAttribute_ABC& attribute );
    //@}

protected:
    //! @name Member data
    //@{
    LoadableComboBox* network_;
    RichLabel* networkLabel_;
    kernel::Controller& controller_;
    //@}
};

}

#endif // __UndergroundPrototype_ABC_h_
