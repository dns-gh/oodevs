// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ArmyFactory_ABC_h_
#define __ArmyFactory_ABC_h_

#include "tools/Resolver.h"
#include "Entities/MIL_Army_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  ArmyFactory_ABC
    @brief  Army factory declaration
*/
// Created: MGD 2009-10-24 @TODO MGD add serialization for resolver and divide factory-resolver, pass resolver by ref in constructor
// =============================================================================
class ArmyFactory_ABC : public tools::Resolver< MIL_Army_ABC >
                      , private boost::noncopyable
{
public:
    //! @name Destructor
    //@{
    virtual ~ArmyFactory_ABC() { DeleteAll(); }
    //@}

    //! @name Operations
    //@{
    virtual MIL_Army_ABC* Create( xml::xistream& xis ) = 0;
    virtual void Finalize() = 0;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive& file, const unsigned int );
    //@}

protected:
    //! @name Constructor
    //@{
    ArmyFactory_ABC() {}
    //@}
};

// -----------------------------------------------------------------------------
// Name: ArmyFactory_ABC::serialize
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
template< typename Archive >
void ArmyFactory_ABC::serialize( Archive& file, const unsigned int )
{
    file & elements_;
}

#endif // __ArmyFactory_ABC_h_
