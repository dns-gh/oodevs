// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Lives_h_
#define __Lives_h_

#include "clients_kernel/Updatable_ABC.h"
#include "Lives_ABC.h"

namespace sword
{
    class UnitAttributes;
    class UnitKnowledgeUpdate;
}

namespace kernel
{
    class Controller;
    class Entity_ABC;
}

// =============================================================================
/** @class  Lives
    @brief  Lives
*/
// Created: AGE 2006-04-10
// =============================================================================
class Lives : public Lives_ABC
            , public kernel::Updatable_ABC< sword::UnitAttributes >
            , public kernel::Updatable_ABC< sword::UnitKnowledgeUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             Lives( const kernel::Entity_ABC& entity, kernel::Controller& controller );
    virtual ~Lives();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual void DoUpdate( const sword::UnitAttributes& message );
    virtual void DoUpdate( const sword::UnitKnowledgeUpdate& message );
    virtual float GetLife() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Lives( const Lives& );            //!< Copy constructor
    Lives& operator=( const Lives& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& entity_;
    kernel::Controller& controller_;
    float life_;
    //@}
};

#endif // __Lives_h_
