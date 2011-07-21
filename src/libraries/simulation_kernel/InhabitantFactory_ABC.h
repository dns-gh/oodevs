// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __InhabitantFactory_ABC_h_
#define __InhabitantFactory_ABC_h_

#include "tools/Resolver.h"
#include "Entities/Inhabitants/MIL_Inhabitant.h"

namespace xml
{
    class xistream;
}

class MIL_Army_ABC;

// =============================================================================
/** @class  InhabitantFactory_ABC
    @brief  InhabitantFactory_ABC
*/
// Created: SLG 2010-11-29
// =============================================================================
class InhabitantFactory_ABC : public tools::Resolver< MIL_Inhabitant >
                            , private boost::noncopyable
{
public:
    //! @name Destructor
    //@{
    virtual ~InhabitantFactory_ABC();
    //@}

    //! @name Operations
    //@{
    virtual MIL_Inhabitant& Create( xml::xistream& xis, MIL_Army_ABC& army ) = 0;
    virtual void Finalize() = 0;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive& file, const unsigned int );
    //@}

protected:
    //! @name Constructor
    //@{
    InhabitantFactory_ABC();
    //@}
};

// -----------------------------------------------------------------------------
// Name: InhabitantFactory_ABC constructor
// Created: JSR 2011-01-14
// -----------------------------------------------------------------------------
inline
InhabitantFactory_ABC::InhabitantFactory_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantFactory_ABC::~InhabitantFactory_ABC()
// Created: JSR 2011-01-14
// -----------------------------------------------------------------------------
inline
InhabitantFactory_ABC::~InhabitantFactory_ABC()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void InhabitantFactory_ABC::serialize
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
template< typename Archive >
void InhabitantFactory_ABC::serialize( Archive& file, const unsigned int )
{
    file & elements_;
}

#endif // __InhabitantFactory_ABC_h_
