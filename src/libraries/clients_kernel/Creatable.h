// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Creatable_h_
#define __Creatable_h_

#include "Updatable_ABC.h"

namespace kernel
{
    class InstanciationComplete;
    class Controller;

// =============================================================================
/** @class  Creatable
    @brief  Creatable
*/
// Created: AGE 2006-11-20
// =============================================================================
template< typename Iface >
class Creatable : public Updatable_ABC< InstanciationComplete >
{

public:
    //! @name Constructors/Destructor
    //@{
             Creatable( Controller& controller, Iface* _this );
    virtual ~Creatable();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DoUpdate( const InstanciationComplete& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Creatable( const Creatable& );            //!< Copy constructor
    Creatable& operator=( const Creatable& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    Iface* this_;
    //@}
};

}

#include "Creatable.inl"

#endif // __Creatable_h_
