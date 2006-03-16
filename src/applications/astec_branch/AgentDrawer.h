// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentDrawer_h_
#define __AgentDrawer_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "geometry/Types.h"

class Agent;
class CoordinateConverter;
class GlTools_ABC;

// =============================================================================
/** @class  AgentDrawer
    @brief  AgentDrawer
*/
// Created: AGE 2006-03-16
// =============================================================================
class AgentDrawer : public Extension_ABC
                  , public Updatable_ABC< ASN1T_MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentDrawer( Agent& agent, const CoordinateConverter& converter, const GlTools_ABC& tools );
    virtual ~AgentDrawer();
    //@}

    //! @name Operations
    //@{
    void Draw(  const geometry::Rectangle2f& viewport );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentDrawer( const AgentDrawer& );            //!< Copy constructor
    AgentDrawer& operator=( const AgentDrawer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    //@}

private:
    //! @name Member data
    //@{
    const CoordinateConverter& converter_;
    const GlTools_ABC& tools_;
    Agent& agent_;
    MT_Vector2D position_;
    //@}
};

#endif // __AgentDrawer_h_
