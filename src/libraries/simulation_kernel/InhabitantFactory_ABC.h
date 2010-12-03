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
    virtual ~InhabitantFactory_ABC() { DeleteAll(); };
    //@}

    //! @name Operations
    //@{
    virtual MIL_Inhabitant& Create( xml::xistream& xis, MIL_Army_ABC& army ) = 0;
    virtual MIL_Inhabitant& Create( const std::string& type, const MT_Vector2D& point, int number, const std::string& name, MIL_Army_ABC& formation ) = 0;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive& file, const unsigned int );
    //@}

protected:
    //! @name Constructor
    //@{
    InhabitantFactory_ABC() {};
    //@}
};

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void InhabitantFactory_ABC::serialize
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
template< typename Archive >
void InhabitantFactory_ABC::serialize( Archive& file, const unsigned int )
{
    file & elements_;
}

#endif // __InhabitantFactory_ABC_h_
