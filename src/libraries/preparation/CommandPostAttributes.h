// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CommandPostAttributes_h_
#define __CommandPostAttributes_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  CommandPostAttributes
    @brief  CommandPostAttributes
*/
// Created: SBO 2007-03-27
// =============================================================================
class CommandPostAttributes : public kernel::Extension_ABC
                            , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit CommandPostAttributes( const kernel::Entity_ABC& entity );
    virtual ~CommandPostAttributes();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CommandPostAttributes( const CommandPostAttributes& );            //!< Copy constructor
    CommandPostAttributes& operator=( const CommandPostAttributes& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& entity_;
    //@}
};

#endif // __CommandPostAttributes_h_
