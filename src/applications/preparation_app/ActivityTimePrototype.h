// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActivityTimePrototype_h_
#define __ActivityTimePrototype_h_

#include "clients_gui/ActivityTimePrototype_ABC.h"

namespace kernel
{
    class Object_ABC;
}

// =============================================================================
/** @class  ActivityTimePrototype
    @brief  ActivityTimePrototype
*/
// Created: SBO 2006-04-20
// =============================================================================
class ActivityTimePrototype : public gui::ActivityTimePrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActivityTimePrototype( QWidget* parent, kernel::Object_ABC*& creation );
    virtual ~ActivityTimePrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ActivityTimePrototype( const ActivityTimePrototype& );            //!< Copy constructor
    ActivityTimePrototype& operator=( const ActivityTimePrototype& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    //@}
};

#endif // __ActivityTimePrototype_h_
