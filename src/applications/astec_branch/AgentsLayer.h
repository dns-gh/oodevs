// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentsLayer_h_
#define __AgentsLayer_h_

#include "graphics/MapLayer_ABC.h"
#include "Observer_ABC.h"
#include "ElementObserver_ABC.h"
#include "GlTools_ABC.h"

class Agent;
class Controller;
class ActionController;
class CoordinateConverter;

// =============================================================================
/** @class  AgentsLayer
    @brief  AgentsLayer
*/
// Created: AGE 2006-03-16
// =============================================================================
class AgentsLayer : public MapLayer_ABC
                  , private Observer_ABC
                  , public ElementObserver_ABC< Agent >
                  , public GlTools_ABC // $$$$ AGE 2006-03-16: 
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentsLayer( Controller& controller, ActionController& actions, const CoordinateConverter& converter );
    virtual ~AgentsLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );

    virtual void DrawCross( const geometry::Point2f& at ) const;
    virtual void DrawLine( const geometry::Point2f& from, const geometry::Point2f& to ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentsLayer( const AgentsLayer& );            //!< Copy constructor
    AgentsLayer& operator=( const AgentsLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Agent& );
    virtual void NotifyUpdated( const Agent& );
    virtual void NotifyDeleted( const Agent& );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const Agent* > T_Agents;
    typedef T_Agents::iterator         IT_Agents;
    typedef T_Agents::const_iterator  CIT_Agents;
    //@}

private:
    //! @name Member data
    //@{
    ActionController& actions_;
    const CoordinateConverter& converter_;
    T_Agents agents_;
    //@}
};

#endif // __AgentsLayer_h_
