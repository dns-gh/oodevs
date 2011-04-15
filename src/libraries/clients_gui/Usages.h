// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Usages_h_
#define __Usages_h_

#include "clients_kernel/Usages_ABC.h"
#include "HumanDefs.h"

namespace kernel
{
    class PropertiesDictionary;
}

namespace gui
{
// =============================================================================
/** @class  Usages
    @brief  Usages
*/
// Created: LGY 2011-04-14
// =============================================================================
class Usages : public kernel::Usages_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Usages( kernel::PropertiesDictionary& dictionary );
    virtual ~Usages();
    //@}

    //! @name Operations
    //@{
    virtual void Add( const std::string& usage, unsigned int proportion );
    virtual unsigned int Find( const std::string& usage ) const;
    //@}

private:
    //! @name Member Data
    //@{
    kernel::PropertiesDictionary& dictionary_;
    T_Usages usages_;
    //@}
};

}

#endif // __Usages_h_
