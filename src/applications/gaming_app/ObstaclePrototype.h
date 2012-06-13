// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObstaclePrototype_h_
#define __ObstaclePrototype_h_

#include "clients_gui/ObstaclePrototype_ABC.h"

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }
}

// =============================================================================
/** @class  ObstaclePrototype
    @brief  ObstaclePrototype
*/
// Created: SBO 2007-02-08
// =============================================================================
class ObstaclePrototype : public gui::ObstaclePrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    ObstaclePrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList );
    virtual ~ObstaclePrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit( const kernel::Team_ABC& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObstaclePrototype( const ObstaclePrototype& );            //!< Copy constructor
    ObstaclePrototype& operator=( const ObstaclePrototype& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    actions::parameters::ParameterList*& attributesList_;
    //@}
};

#endif // __ObstaclePrototype_h_
