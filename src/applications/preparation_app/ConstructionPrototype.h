// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ConstructionPrototype_h_
#define __ConstructionPrototype_h_

#include "clients_gui/ConstructionPrototype_ABC.h"

namespace kernel
{
    class Object_ABC;
}

// =============================================================================
/** @class  ConstructionPrototype
    @brief  ConstructionPrototype
*/
// Created: SBO 2009-06-09
// =============================================================================
class ConstructionPrototype : public gui::ConstructionPrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ConstructionPrototype( QWidget* parent, kernel::Object_ABC*& creation );
    virtual ~ConstructionPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ConstructionPrototype( const ConstructionPrototype& );            //!< Copy constructor
    ConstructionPrototype& operator=( const ConstructionPrototype& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    //@}
};

#endif // __ConstructionPrototype_h_
