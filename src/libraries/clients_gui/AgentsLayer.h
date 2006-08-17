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
#include "astec_kernel/Agent_ABC.h"
#include "astec_kernel/ContextMenuObserver_ABC.h"
#include "astec_kernel/SafePointer.h"

// =============================================================================
/** @class  AgentsLayer
    @brief  AgentsLayer
*/
// Created: AGE 2006-03-23
// =============================================================================
class AgentsLayer : public QObject
                  , public EntityLayer< Agent_ABC >
                  , public ContextMenuObserver_ABC< Agent_ABC >
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
    void Aggregate   ( const Agent_ABC& agent );
    void Disaggregate( const Agent_ABC& agent );
    void Engage      ( const Agent_ABC& agent ); // $$$$ AGE 2006-06-29: ca a pas spécialement besoin d'etre dans le layer opengl...
    void Disengage   ( const Agent_ABC& agent );
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
    virtual void NotifyContextMenu( const Agent_ABC&, ::ContextMenu& );
    virtual void Select( const Entity_ABC&, bool );
    virtual void DisplayTooltip( const Agent_ABC& entity, Displayer_ABC& displayer );
    void Toggle( const Agent_ABC& automat, bool aggregate );
    template< typename Extension >
    void AddToTooltip( const Agent_ABC& entity, Displayer_ABC& displayer );
    //@}

    //! @name Types
    //@{
    typedef std::set< const Agent_ABC* > T_Agents;
    //@}

private:
    //! @name Member data
    //@{
    SafePointer< Agent_ABC > selected_;
    T_Agents aggregated_;
    //@}
};

#endif // __AgentsLayer_h_
