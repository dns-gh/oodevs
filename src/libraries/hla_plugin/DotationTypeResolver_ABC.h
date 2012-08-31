// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef DotationTypeResolver_ABC_h
#define DotationTypeResolver_ABC_h

#include <boost/noncopyable.hpp>

namespace rpr
{
    class EntityType;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  DotationTypeResolver_ABC
    @brief  Dotatio type resolver definition
*/
// Created: VPR 2011-09-26
// =============================================================================
class DotationTypeResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
            DotationTypeResolver_ABC() {}
    virtual ~DotationTypeResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual rpr::EntityType Resolve( unsigned int munitionIdentifier ) const = 0;
    virtual unsigned int    Resolve( const rpr::EntityType& munitionType ) const = 0;
    //@}
};

}
}

#endif // DotationTypeResolver_ABC_h
