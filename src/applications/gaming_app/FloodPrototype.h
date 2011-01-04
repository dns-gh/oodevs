// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FloodPrototype_h_
#define __FloodPrototype_h_

#include "clients_gui/FloodPrototype_ABC.h"

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }
}

// =============================================================================
/** @class  FloodPrototype
    @brief  FloodPrototype
*/
// Created: JSR 2011-01-03
// =============================================================================
class FloodPrototype : public gui::FloodPrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FloodPrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList );
    virtual ~FloodPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FloodPrototype( const FloodPrototype& );            //!< Copy constructor
    FloodPrototype& operator=( const FloodPrototype& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    actions::parameters::ParameterList*& attributesList_;
    //@}
};

#endif // __FloodPrototype_h_
