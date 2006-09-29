// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentPositions_h_
#define __AgentPositions_h_

#include "clients_kernel/Positions.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Aggregatable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Agent_ABC;
}

// =============================================================================
/** @class  AgentPositions
    @brief  AgentPositions. Nothing sexual.
*/
// Created: AGE 2006-03-16
// =============================================================================
class AgentPositions : public kernel::Positions
                     , public kernel::Drawable_ABC
                     , public kernel::Aggregatable_ABC
                     , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentPositions( const kernel::Agent_ABC& agent, const kernel::CoordinateConverter_ABC& converter, const geometry::Point2f& position );
    virtual ~AgentPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition() const;
    virtual float             GetHeight() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void DoSerialize( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    void Set( const geometry::Point2f& point );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentPositions( const AgentPositions& );            //!< Copy constructor
    AgentPositions& operator=( const AgentPositions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Aggregate( const bool& );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Agent_ABC& agent_;
    const kernel::CoordinateConverter_ABC& converter_;
    geometry::Point2f position_;
    float height_;
    bool aggregated_;
    //@}
};

#endif // __AgentPositions_h_
