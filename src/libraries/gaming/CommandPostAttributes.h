// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CommandPostAttributes_h_
#define __CommandPostAttributes_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/CommandPostAttributes_ABC.h"
#include <tools/Resolver_ABC.h>
#include <boost/noncopyable.hpp>

namespace sword
{
    class UnitCreation;
    class UnitAttributes;
}

namespace kernel
{
    class AgentType;
    class Entity_ABC;
}

// =============================================================================
/** @class  CommandPostAttributes
    @brief  Command-Post Attributes extension
*/
// Created: SBO 2006-11-30
// =============================================================================
class CommandPostAttributes : public kernel::CommandPostAttributes_ABC
                            , public kernel::Updatable_ABC< sword::UnitAttributes >
                            , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             CommandPostAttributes( kernel::Entity_ABC& entity, const sword::UnitCreation& message,
                                    const tools::Resolver_ABC< kernel::AgentType >& resolver );
    virtual ~CommandPostAttributes();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual void DoUpdate( const sword::UnitAttributes& message );
    //@}

    //! @name Accessors
    //@{
    virtual bool IsCommandPost() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Entity_ABC& entity_;
    const kernel::AgentType& type_;
    bool commandPost_;
    //@}
};

#endif // __CommandPostAttributes_h_
