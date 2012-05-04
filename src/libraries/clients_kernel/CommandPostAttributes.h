// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __kernel_CommandPostAttributes_h_
#define __kernel_CommandPostAttributes_h_

#include "CommandPostAttributes_ABC.h"
#include "Drawable_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class AgentType;
// =============================================================================
/** @class  CommandPostAttributes
    @brief  Command post attributes
*/
// Created: LGY 2012-05-04
// =============================================================================
class CommandPostAttributes : public CommandPostAttributes_ABC
                            , public Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             CommandPostAttributes( const Entity_ABC& entity, const AgentType& type, bool commandPost );
    virtual ~CommandPostAttributes();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual bool IsCommandPost() const;
    //@}

protected:
    //! @name Member Data
    //@{
    const kernel::Entity_ABC& entity_;
    const AgentType& type_;
    bool commandPost_;
    //@}
};

}

#endif // __kernel_CommandPostAttributes_h_
