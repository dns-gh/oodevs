// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __BlockPhFirerModifier_h_
#define __BlockPhFirerModifier_h_

#include <urban/BlockPhModifier_ABC.h>

namespace urban
{
    class BlockModel;
    class Architecture;
}
// =============================================================================
// @class  BlockPhFirerModifier
// Created: SLG 2009-10-23
// =============================================================================
class BlockPhFirerModifier : public urban::BlockPhModifier_ABC 
{

public:

    //! @name Constructors/Destructor
    //@{
    BlockPhFirerModifier();
    virtual ~BlockPhFirerModifier();
    //@}

    //! @name Operations
    //@{
    virtual float Execute( urban::BlockModel& blockModel, const geometry::Point2f& firerPosition );
    //@}

private:
    //! @name Operations
    //@{
    float HeightModifier(  const urban::Architecture& architecture ) const;
    //@}
};

#endif // __BlockPhFirerModifier_h_

