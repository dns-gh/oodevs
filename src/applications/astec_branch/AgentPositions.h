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

#include "ASN_Types.h"
#include "Updatable_ABC.h"
#include "Positions.h"
#include "Drawable_ABC.h"
#include "Aggregatable_ABC.h"

class CoordinateConverter_ABC;
class Agent;

// =============================================================================
/** @class  AgentPositions
    @brief  AgentPositions. Nothing sexual.
*/
// Created: AGE 2006-03-16
// =============================================================================
class AgentPositions : public Positions
                     , public Updatable_ABC< ASN1T_MsgUnitAttributes >
                     , public Drawable_ABC
                     , public Aggregatable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentPositions( const Agent& agent, const CoordinateConverter_ABC& converter );
    virtual ~AgentPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition() const;
    virtual float             GetHeight() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentPositions( const AgentPositions& );            //!< Copy constructor
    AgentPositions& operator=( const AgentPositions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    virtual void Aggregate( const bool& );
    //@}

private:
    //! @name Member data
    //@{
    const Agent& agent_;
    const CoordinateConverter_ABC& converter_;
    geometry::Point2f position_;
    float height_;
    bool aggregated_;
    //@}
};

#endif // __AgentPositions_h_
