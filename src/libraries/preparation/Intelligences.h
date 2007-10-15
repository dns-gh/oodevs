// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Intelligences_h_
#define __Intelligences_h_

#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Intelligence_ABC;
}

// =============================================================================
/** @class  Intelligences
    @brief  Intelligences
*/
// Created: SBO 2007-10-15
// =============================================================================
class Intelligences : public kernel::Extension_ABC
                    , public kernel::Serializable_ABC
                    , public kernel::Resolver< kernel::Intelligence_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             Intelligences();
    virtual ~Intelligences();
    //@}

    //! @name Operations
    //@{
    void AddIntelligence( kernel::Intelligence_ABC& intelligence );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Intelligences( const Intelligences& );            //!< Copy constructor
    Intelligences& operator=( const Intelligences& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}
};

#endif // __Intelligences_h_
