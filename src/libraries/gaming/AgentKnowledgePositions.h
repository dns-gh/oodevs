// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentKnowledgePositions_h_
#define __AgentKnowledgePositions_h_

#include "clients_kernel/Positions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "protocol/Protocol.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  AgentKnowledgePositions
    @brief  AgentKnowledgePositions
*/
// Created: AGE 2006-05-17
// =============================================================================
class AgentKnowledgePositions : public kernel::Positions
                              , public kernel::Updatable_ABC< sword::UnitKnowledgeUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentKnowledgePositions( const kernel::CoordinateConverter_ABC& converter );
    virtual ~AgentKnowledgePositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float GetHeight( bool aggregated ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentKnowledgePositions( const AgentKnowledgePositions& );            //!< Copy constructor
    AgentKnowledgePositions& operator=( const AgentKnowledgePositions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::UnitKnowledgeUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    geometry::Point2f position_;
    //@}
};

#endif // __AgentKnowledgePositions_h_
