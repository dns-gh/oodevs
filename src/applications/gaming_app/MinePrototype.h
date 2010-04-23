// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MinePrototype_h_
#define __MinePrototype_h_

#include "clients_gui/MinePrototype_ABC.h"

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }
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
             MinePrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList );
    virtual ~MinePrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
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
    actions::parameters::ParameterList*& attributesList_;
    //@}
};

#endif // __MinePrototype_h_
