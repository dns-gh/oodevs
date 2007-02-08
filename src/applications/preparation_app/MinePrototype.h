// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MinePrototype_h_
#define __MinePrototype_h_

#include "clients_gui/MinePrototype_ABC.h"

namespace kernel
{
    class Object_ABC;
}

// =============================================================================
/** @class  MinePrototype
    @brief  MinePrototype
*/
// Created: SBO 2007-02-08
// =============================================================================
class MinePrototype : public gui::MinePrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MinePrototype( QWidget* parent, kernel::Object_ABC*& creation );
    virtual ~MinePrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MinePrototype( const MinePrototype& );            //!< Copy constructor
    MinePrototype& operator=( const MinePrototype& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    //@}
};

#endif // __MinePrototype_h_
