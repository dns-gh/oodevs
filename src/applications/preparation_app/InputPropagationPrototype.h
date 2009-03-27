// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InputPropagationPrototype_h_
#define __InputPropagationPrototype_h_

#include "clients_gui/InputPropagationPrototype_ABC.h"

namespace kernel
{
    class Object_ABC;
}

// =============================================================================
/** @class  InputPropagationPrototype
    @brief  InputPropagationPrototype
*/
// Created: SBO 2006-04-20
// =============================================================================
class InputPropagationPrototype : public gui::InputPropagationPrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             InputPropagationPrototype( QWidget* parent, kernel::Object_ABC*& creation );
    virtual ~InputPropagationPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    InputPropagationPrototype( const InputPropagationPrototype& );            //!< Copy constructor
    InputPropagationPrototype& operator=( const InputPropagationPrototype& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    //@}
};

#endif // __InputPropagationPrototype_h_
