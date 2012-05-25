// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Architecture_h_
#define __Architecture_h_

#include "clients_kernel/Architecture.h"

namespace kernel
{
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  Architecture
    @brief  Architecture
*/
// Created: LGY 2011-04-14
// =============================================================================
class Architecture : public kernel::Architecture
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Architecture( kernel::PropertiesDictionary& dictionary );
             Architecture( xml::xistream& xis, kernel::PropertiesDictionary& dictionary );
    virtual ~Architecture();
    //@}

    //! @name Serializable_ABC
    //@{
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}
};

#endif // __Architecture_h_
