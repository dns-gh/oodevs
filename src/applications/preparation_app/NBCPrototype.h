// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NBCPrototype_h_
#define __NBCPrototype_h_

#include "clients_gui/NBCPrototype_ABC.h"

namespace kernel
{
    class Object_ABC;
}

// =============================================================================
/** @class  NBCPrototype
    @brief  NBCPrototype
*/
// Created: SBO 2006-04-20
// =============================================================================
class NBCPrototype : public gui::NBCPrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             NBCPrototype( QWidget* parent, const tools::Resolver_ABC< kernel::NBCAgent >& resolver, int maxToxic, kernel::Object_ABC*& creation );
    virtual ~NBCPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    NBCPrototype( const NBCPrototype& );            //!< Copy constructor
    NBCPrototype& operator=( const NBCPrototype& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    //@}
};

#endif // __NBCPrototype_h_
