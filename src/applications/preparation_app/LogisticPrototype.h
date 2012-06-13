// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticPrototype_h_
#define __LogisticPrototype_h_

#include "clients_gui/LogisticPrototype_ABC.h"

namespace kernel
{
    class Object_ABC;
}

class ObjectsModel;

// =============================================================================
/** @class  LogisticPrototype
    @brief  LogisticPrototype
*/
// Created: SBO 2006-04-19
// =============================================================================
class LogisticPrototype : public gui::LogisticPrototype_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             LogisticPrototype( QWidget* parent, kernel::Controllers& controllers, kernel::Object_ABC*& creation );
    virtual ~LogisticPrototype();
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

#endif // __LogisticPrototype_h_
