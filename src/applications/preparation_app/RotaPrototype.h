// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RotaPrototype_h_
#define __RotaPrototype_h_

#include "clients_gui/RotaPrototype_ABC.h"

namespace kernel
{
    class Object_ABC;
}

// =============================================================================
/** @class  RotaPrototype
    @brief  RotaPrototype
*/
// Created: SBO 2006-04-20
// =============================================================================
class RotaPrototype : public gui::RotaPrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             RotaPrototype( QWidget* parent, const kernel::Resolver_ABC< kernel::NBCAgent >& resolver, kernel::Object_ABC*& creation );
    virtual ~RotaPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    RotaPrototype( const RotaPrototype& );            //!< Copy constructor
    RotaPrototype& operator=( const RotaPrototype& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    //@}
};

#endif // __RotaPrototype_h_
