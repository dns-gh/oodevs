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

#include "EntityLayer.h"
#include "Agent.h"
#include "ContextMenuObserver_ABC.h"

// =============================================================================
/** @class  AgentsLayer
    @brief  AgentsLayer
*/
// Created: AGE 2006-03-23
// =============================================================================
class AgentsLayer : public QObject
                  , public EntityLayer< Agent >
                  , public ContextMenuObserver_ABC< Agent >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             AgentsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view );
    virtual ~AgentsLayer();
    //@}

    //! @name Operations
    //@{
    void Aggregate   ( const Agent& agent );
    void Disaggregate( const Agent& agent );
    void Engage      ( const Agent& agent );
    void Disengage   ( const Agent& agent );
    //@}

private slots:
    //! @name Slots
    //@{
    void Aggregate   ();
    void Disaggregate();
    void Engage      ();
    void Disengage   ();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentsLayer( const AgentsLayer& );            //!< Copy constructor
    AgentsLayer& operator=( const AgentsLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const Agent&, QPopupMenu& );
    virtual void Select( const Entity_ABC&, bool );
    //@}

private:
    //! @name Member data
    //@{
    const Agent* selected_;
    //@}
};

#endif // __AgentsLayer_h_
