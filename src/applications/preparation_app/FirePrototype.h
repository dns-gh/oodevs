// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FirePrototype_h_
#define __FirePrototype_h_

#include "clients_gui/FirePrototype_ABC.h"

namespace kernel
{
    class Object_ABC;
}

// =============================================================================
/** @class  FirePrototype
    @brief  FirePrototype
*/
// Created: SBO 2006-04-20
// =============================================================================
class FirePrototype : public gui::FirePrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             FirePrototype( QWidget* parent, const kernel::Resolver_ABC< kernel::FireClass >& resolver, kernel::Object_ABC*& creation );
    virtual ~FirePrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    FirePrototype( const FirePrototype& );            //!< Copy constructor
    FirePrototype& operator=( const FirePrototype& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    //@}
};

#endif // __FirePrototype_h_
