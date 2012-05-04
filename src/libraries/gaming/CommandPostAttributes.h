// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __CommandPostAttributes_h_
#define __CommandPostAttributes_h_

#include "clients_kernel/CommandPostAttributes.h"
#include "tools/Resolver_ABC.h"

namespace sword
{
    class UnitCreation;
    class UnitAttributes;
}

// =============================================================================
/** @class  CommandPostAttributes
    @brief  Command-Post Attributes extension
*/
// Created: SBO 2006-11-30
// =============================================================================
class CommandPostAttributes : public kernel::CommandPostAttributes
                            , public kernel::Updatable_ABC< sword::UnitAttributes >
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
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void DoUpdate( const sword::UnitAttributes& message );
    //@}
};

#endif // __CommandPostAttributes_h_
