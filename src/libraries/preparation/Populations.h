// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Populations_h_
#define __Populations_h_

#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include <tools/Resolver.h>

class Population;

// =============================================================================
/** @class  Populations
    @brief  Populations
*/
// Created: SBO 2006-11-09
// =============================================================================
class Populations : public kernel::Extension_ABC
                  , public kernel::Serializable_ABC
                  , public tools::Resolver< Population >
{
public:
    //! @name Constructors/Destructor
    //@{
             Populations();
    virtual ~Populations();
    //@}

    //! @name Operations
    //@{
    void AddPopulation( Population& population );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Populations( const Populations& );            //!< Copy constructor
    Populations& operator=( const Populations& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}
};

#endif // __Populations_h_
