// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentConvexHulls_h_
#define __AgentConvexHulls_h_

#include "ConvexHulls.h"
#include "clients_kernel/Updatable_ABC.h"

namespace sword
{
    class UnitAttributes;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  AgentConvexHulls
    @brief  AgentConvexHulls
*/
// Created: AGE 2007-05-30
// =============================================================================
class AgentConvexHulls : public ConvexHulls
                       , public kernel::Updatable_ABC< sword::UnitAttributes >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentConvexHulls( const kernel::Entity_ABC& holder, const kernel::CoordinateConverter_ABC& converter );
    virtual ~AgentConvexHulls();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentConvexHulls( const AgentConvexHulls& );            //!< Copy constructor
    AgentConvexHulls& operator=( const AgentConvexHulls& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::UnitAttributes& attributes );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};

#endif // __AgentConvexHulls_h_
