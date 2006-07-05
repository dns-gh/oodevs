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

#include "ASN_Types.h"
#include "Positions.h"
#include "Updatable_ABC.h"

class CoordinateConverter_ABC;

// =============================================================================
/** @class  AgentKnowledgePositions
    @brief  AgentKnowledgePositions
*/
// Created: AGE 2006-05-17
// =============================================================================
class AgentKnowledgePositions : public Positions
                              , public Updatable_ABC< ASN1T_MsgUnitKnowledgeUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentKnowledgePositions( const CoordinateConverter_ABC& converter );
    virtual ~AgentKnowledgePositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition() const;
    virtual float             GetHeight() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
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
    const CoordinateConverter_ABC& converter_;
    geometry::Point2f position_;
    //@}
};

#endif // __AgentKnowledgePositions_h_
