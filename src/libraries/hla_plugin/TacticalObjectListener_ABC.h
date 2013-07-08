// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TacticalObjectListener_ABC_h
#define plugins_hla_TacticalObjectListener_ABC_h

#include "rpr/ForceIdentifier.h"
#include <string>
#include <boost/noncopyable.hpp>

namespace rpr
{
    class EntityType;
}

namespace plugins
{
namespace hla
{
    class TacticalObject_ABC;

// =============================================================================
/** @class  TacticalObjectListener_ABC
    @brief  TacticalObjectListener_ABC
*/
// Created: AHC 2012-08-08
// =============================================================================
class TacticalObjectListener_ABC : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    enum GeometryType
    {
        eGeometryType_Point,
        eGeometryType_Line,
        eGeometryType_Polygon,
    };
    //@}
    //! @name Constructors/Destructor
    //@{
            TacticalObjectListener_ABC() {}
    virtual ~TacticalObjectListener_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void ObjectCreated( TacticalObject_ABC& object, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type,
            bool isBreachable, GeometryType geometry, bool isPropagation ) = 0;
    //@}
};

}
}

#endif // plugins_hla_TacticalObjectListener_ABC_h
