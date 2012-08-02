// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __MIL_UrbanObject_h_
#define __MIL_UrbanObject_h_

#include "MIL_UrbanObject_ABC.h"

// =============================================================================
/** @class  MIL_UrbanObject
    @brief  MIL_UrbanObject
*/
// Created: JSR 2012-07-31
// =============================================================================
class MIL_UrbanObject : public MIL_UrbanObject_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_UrbanObject( xml::xistream& xis, MIL_UrbanObject_ABC* parent = 0 );
    virtual ~MIL_UrbanObject();
    //@}

    //! @name Operations
    //@{
    virtual unsigned long GetUrbanId() const;
    virtual const std::string& GetName() const;
    virtual MIL_UrbanObject_ABC* GetParent() const;
    virtual void Accept( MIL_UrbanObjectVisitor_ABC& visitor ) const;
    virtual void Accept( MIL_UrbanMotivationsVisitor_ABC& visitor ) const;
    virtual void ComputeConvexHull();
    virtual void GetUrbanObjectLeaves( std::vector< const MIL_UrbanObject_ABC* >& leaves ) const;
    virtual bool HasChild() const;
    virtual float GetLivingSpace() const;
    virtual float ComputeComplexity() const;
    virtual const std::string& GetInfrastructure() const;
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    unsigned long nUrbanId_;
    std::string name_;
    MIL_UrbanObject_ABC* parent_;
    std::string infrastructure_;
    //@}
};

#endif // __MIL_UrbanObject_h_
