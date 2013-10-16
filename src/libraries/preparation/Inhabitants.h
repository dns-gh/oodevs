// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Inhabitants_h_
#define __Inhabitants_h_

#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include <tools/Resolver.h>

class Inhabitant;

// =============================================================================
/** @class  Inhabitants
    @brief  Inhabitants
*/
// Created: SLG 2010-11-23
// =============================================================================
class Inhabitants : public kernel::Extension_ABC
                  , public kernel::Serializable_ABC
                  , public tools::Resolver< Inhabitant >
{
public:
    //! @name Constructors/Destructor
    //@{
             Inhabitants();
    virtual ~Inhabitants();
    //@}

    //! @name Operations
    //@{
    void AddInhabitant( Inhabitant& inhabitant );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Inhabitants( const Inhabitants& );            //!< Copy constructor
    Inhabitants& operator=( const Inhabitants& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}
};

#endif // __Inhabitants_h_
