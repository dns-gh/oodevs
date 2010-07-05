// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
             FirePrototype( QWidget* parent, const tools::Resolver_ABC< kernel::FireClass >& resolver, kernel::Object_ABC*& creation );
    virtual ~FirePrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FirePrototype( const FirePrototype& );            //!< Copy constructor
    FirePrototype& operator=( const FirePrototype& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    //@}
};

#endif // __FirePrototype_h_
