// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledgePositions_h_
#define __ObjectKnowledgePositions_h_

#include "LocationPositions.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class ObjectKnowledge_ABC;
    class ObjectType;
}

namespace sword
{
    class ObjectKnowledgeUpdate;
}

// =============================================================================
/** @class  ObjectKnowledgePositions
    @brief  ObjectKnowledgePositions
*/
// Created: AGE 2006-05-18
// =============================================================================
class ObjectKnowledgePositions : public LocationPositions
                               , public kernel::Updatable_ABC< sword::ObjectKnowledgeUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledgePositions( const kernel::CoordinateConverter_ABC& converter, const kernel::ObjectKnowledge_ABC& knowledge,
                                       const kernel::ObjectType& type );
    virtual ~ObjectKnowledgePositions();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual void Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectKnowledgePositions( const ObjectKnowledgePositions& );            //!< Copy constructor
    ObjectKnowledgePositions& operator=( const ObjectKnowledgePositions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::ObjectKnowledge_ABC& knowledge_;
    const kernel::ObjectType& type_;
    //@}
};

#endif // __ObjectKnowledgePositions_h_
