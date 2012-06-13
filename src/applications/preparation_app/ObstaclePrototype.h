// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ObstaclePrototype_h_
#define __ObstaclePrototype_h_

#include "clients_gui/ObstaclePrototype_ABC.h"

namespace kernel
{
    class Object_ABC;
}

class ObjectsModel;

// =============================================================================
/** @class  ObstaclePrototype
    @brief  ObstaclePrototype
*/
// Created: SBO 2006-04-19
// =============================================================================
class ObstaclePrototype : public gui::ObstaclePrototype_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             ObstaclePrototype( QWidget* parent, kernel::Object_ABC*& creation );
    virtual ~ObstaclePrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit( const kernel::Team_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    //@}
};

#endif // __ObstaclePrototype_h_
