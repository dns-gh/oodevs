// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __MIL_UrbanObject_ABC_h_
#define __MIL_UrbanObject_ABC_h_

#include "UrbanExtension_ABC.h"
#include <tools/Extendable.h>
#include <tools/Resolver.h>

class MIL_UrbanMotivationsVisitor_ABC;

// TODO virer UrbanObjectWrapper à terme et ne garder que cette classe là

// =============================================================================
/** @class  MIL_UrbanObject_ABC
    @brief  MIL_UrbanObject_ABC
*/
// Created: JSR 2012-07-31
// =============================================================================
class MIL_UrbanObject_ABC : public tools::Extendable< UrbanExtension_ABC >
                          , public tools::Resolver< MIL_UrbanObject_ABC >
                          , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_UrbanObject_ABC() {}
    virtual ~MIL_UrbanObject_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual unsigned long GetUrbanId() const = 0;
    virtual const std::string& GetName() const = 0;
    virtual MIL_UrbanObject_ABC* GetParent() const = 0;
    virtual void Accept( MIL_UrbanMotivationsVisitor_ABC& visitor ) const = 0;
    virtual void ComputeConvexHull() = 0;
    virtual void GetUrbanObjectLeaves( std::vector< const MIL_UrbanObject_ABC* >& leaves ) const = 0;
    virtual bool HasChild() const = 0;
    virtual float GetLivingSpace() const = 0;
    virtual float ComputeComplexity() const = 0;
    virtual const std::string& GetInfrastructure() const = 0;
    //@}
};

#endif // __MIL_UrbanObject_ABC_h_
