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
#include <memory>

namespace xml
{
    class xistream;
}

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
             Usages( xml::xistream& xis, std::auto_ptr< kernel::Usages_ABC > pUsages );
    virtual ~Usages();
    //@}

    //! @name Operations
    //@{
    virtual void Add( const std::string& usage, unsigned int proportion );
    virtual unsigned int Find( const std::string& usage ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadUsages( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< kernel::Usages_ABC > pUsages_;
    //@}
};

#endif // __Usages_h_
