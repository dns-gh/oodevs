// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LodgingPrototype_h_
#define __LodgingPrototype_h_

#include "clients_gui/LodgingPrototype_ABC.h"

namespace kernel
{
    class Object_ABC;
}

// =============================================================================
/** @class  LodgingPrototype
    @brief  LodgingPrototype
*/
// Created: MARTIN 2011-05-02
// =============================================================================
class LodgingPrototype : public gui::LodgingPrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LodgingPrototype( QWidget* parent, kernel::Object_ABC*& creation );
    virtual ~LodgingPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LodgingPrototype( const LodgingPrototype& );            //!< Copy constructor
    LodgingPrototype& operator=( const LodgingPrototype& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    //@}
};

#endif // __LodgingPrototype_h_
