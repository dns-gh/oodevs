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

#include "Observer_ABC.h"
#include "ElementObserver_ABC.h"
#include "ActivationObserver_ABC.h"
#include "graphics/MapLayer_ABC.h"

class Agent;
class Controllers;
class GlTools_ABC;
class ColorStrategy_ABC;
class MapWidget;

// =============================================================================
/** @class  AgentsLayer
    @brief  AgentsLayer
*/
// Created: AGE 2006-03-16
// =============================================================================
class AgentsLayer : public MapLayer_ABC
                  , private Observer_ABC
                  , public ElementObserver_ABC< Agent >
                  , public ActivationObserver_ABC< Agent >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, MapWidget& widget );
    virtual ~AgentsLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    //@}

protected:
    //! @name Events
    //@{
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
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
    virtual void NotifyActivated( const Agent& );

    bool IsInSelection( const Agent& agent, const geometry::Point2f& point ) const;
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
    Controllers& controllers_;
    const GlTools_ABC& tools_;
    ColorStrategy_ABC& strategy_;
    MapWidget& widget_;

    T_Agents agents_;
    unsigned selected_;
    //@}
};

#endif // __AgentsLayer_h_
