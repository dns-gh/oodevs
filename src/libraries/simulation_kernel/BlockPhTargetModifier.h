// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __BlockPhTargetModifier_h_
#define __BlockPhTargetModifier_h_

#include <urban/BlockPhModifier_ABC.h>

namespace urban
{
    class BlockModel;
    class Architecture; 
    class Soil;
}
// =============================================================================
// @class  BlockPhTargetModifier
// Created: SLG 2009-10-23
// =============================================================================
class BlockPhTargetModifier : public urban::BlockPhModifier_ABC 
{

public:

    //! @name Constructors/Destructor
    //@{
    BlockPhTargetModifier();
    virtual ~BlockPhTargetModifier();
    //@}

    //! @name Operations
    //@{
    virtual float Execute( urban::BlockModel& blockModel, const geometry::Point2f& targetPosition );
    //@}

private:
    //! @name Operations
    //@{
    float ArchitectureModifier( const urban::Architecture& architecture ) const;
    float SoilModifier( const urban::Soil& soil ) const;
    //@}
};

#endif // __BlockPhTargetModifier_h_

