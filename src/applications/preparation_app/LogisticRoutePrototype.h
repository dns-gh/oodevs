// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticRoutePrototype_h_
#define __LogisticRoutePrototype_h_

#include "clients_gui/LogisticRoutePrototype_ABC.h"

namespace kernel
{
    class Object_ABC;
}

// =============================================================================
/** @class  LogisticRoutePrototype
    @brief  LogisticRoutePrototype
*/
// Created: SBO 2006-04-20
// =============================================================================
class LogisticRoutePrototype : public gui::LogisticRoutePrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticRoutePrototype( QWidget* parent, kernel::Object_ABC*& creation );
    virtual ~LogisticRoutePrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticRoutePrototype( const LogisticRoutePrototype& );            //!< Copy constructor
    LogisticRoutePrototype& operator=( const LogisticRoutePrototype& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    //@}
};

#endif // __LogisticRoutePrototype_h_
