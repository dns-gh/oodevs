// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __FormationFactory_ABC_h_
#define __FormationFactory_ABC_h_

#include "Entities/MIL_Formation.h"
#include <tools/Resolver.h>

class MIL_Army;

namespace xml
{
    class xistream;
}
// =============================================================================
/** @class  FormationFactory_ABC
    @brief  FormationFactory_ABC
*/
// Created: MGD 2009-10-22
// =============================================================================
class FormationFactory_ABC : public tools::Resolver< MIL_Formation >
                           , private boost::noncopyable
{
public:
    //! @name Destructor
    //@{
    virtual ~FormationFactory_ABC() { DeleteAll(); };
    //@}

    //! @name Operations
    //@{
    virtual MIL_Formation& Create( xml::xistream& xis, MIL_Army_ABC& army, MIL_Formation* parent = 0 ) = 0;
    virtual MIL_Formation& Create( int level, const std::string& name, const std::string& logLevelStr, MIL_Army_ABC& army, MIL_Formation* parent = 0 ) = 0;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive& file, const unsigned int );
    //@}

protected:
    //! @name Constructor
    //@{
    FormationFactory_ABC() {};
    //@}
};

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void FormationFactory_ABC::serialize
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
template< typename Archive >
void FormationFactory_ABC::serialize( Archive& file, const unsigned int )
{
    file & elements_;
}

#endif // __FormationFactory_ABC_h_
