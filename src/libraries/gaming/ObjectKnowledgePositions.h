// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledgePositions_h_
#define __ObjectKnowledgePositions_h_

#include "LocationPositions.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class ObjectKnowledge_ABC;
}

// =============================================================================
/** @class  ObjectKnowledgePositions
    @brief  ObjectKnowledgePositions
*/
// Created: AGE 2006-05-18
// =============================================================================
class ObjectKnowledgePositions : public LocationPositions
                               , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectKnowledgeUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledgePositions( const kernel::CoordinateConverter_ABC& converter, const kernel::ObjectKnowledge_ABC& knowledge );
    virtual ~ObjectKnowledgePositions();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectKnowledgePositions( const ObjectKnowledgePositions& );            //!< Copy constructor
    ObjectKnowledgePositions& operator=( const ObjectKnowledgePositions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::ObjectKnowledge_ABC& knowledge_;
    bool realObjectKnown_;
    bool perceived_;
    //@}
};

#endif // __ObjectKnowledgePositions_h_
