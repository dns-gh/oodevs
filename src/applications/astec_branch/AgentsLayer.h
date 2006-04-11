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
    // $$$$ AGE 2006-04-11: Trouver autre chose pour gérer l'aggregation.
    // $$$$ AGE 2006-04-11: Quelque chose de plus centré sur l'agent.
    // $$$$ AGE 2006-04-11: Aggregatable_ABC ? Puis selon, affichage, ... ? +
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

public slots:
    //! @name Slots
    //@{
    void Aggregate   ();
    void Disaggregate();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentsLayer( const AgentsLayer& );            //!< Copy constructor
    AgentsLayer& operator=( const AgentsLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void NotifyContextMenu( const Agent&, QPopupMenu& );
    void Aggregate   ( const Agent& automat );
    void Disaggregate( const Agent& automat );
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
    const GlTools_ABC& tools_;
    T_Agents aggregatedAutomats_;
    const Agent* selected_;
    //@}
};

#endif // __AgentsLayer_h_
