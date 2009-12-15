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

#include "game_asn/Simulation.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Updatable_ABC.h"

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
                              , public kernel::Updatable_ABC< ASN1T_MsgUnitKnowledgeUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentKnowledgePositions( const kernel::CoordinateConverter_ABC& converter );
    virtual ~AgentKnowledgePositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition() const;
    virtual float             GetHeight() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision, float adaptiveFactor ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentKnowledgePositions( const AgentKnowledgePositions& );            //!< Copy constructor
    AgentKnowledgePositions& operator=( const AgentKnowledgePositions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitKnowledgeUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    geometry::Point2f position_;
    //@}
};

#endif // __AgentKnowledgePositions_h_
