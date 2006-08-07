// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntityLayer_h_
#define __EntityLayer_h_

#include "astec_gaming/Observer_ABC.h"
#include "astec_gaming/ElementObserver_ABC.h"
#include "astec_gaming/ActivationObserver_ABC.h"
#include "astec_gaming/SelectionObserver_ABC.h"
#include "astec_gaming/OptionsObserver_ABC.h"
#include "Layer_ABC.h"
#include "astec_gaming/SafePointer.h"

class Controllers;
class GlTools_ABC;
class ColorStrategy_ABC;
class View_ABC;
class Entity_ABC;
class Team;
class GlTooltip;
class Displayer_ABC;

// =============================================================================
/** @class  EntityLayerBase
    @brief  Entity layer base
*/
// Created: AGE 2006-03-23
// =============================================================================
class EntityLayerBase : public Layer_ABC
                      , public Observer_ABC
                      , public OptionsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{    
             EntityLayerBase( Controllers& controllers, const GlTools_ABC& tools, View_ABC& view );
    virtual ~EntityLayerBase();
    //@}    

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    //@}

protected:
    //! @name Events
    //@{    
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleMouseMove( QMouseEvent* event, const geometry::Point2f& point );
    virtual void OptionChanged( const std::string& name, const OptionVariant& value );
    //@}

    //! @name Helpers
    //@{
    virtual bool IsInSelection( const Entity_ABC& entity, const geometry::Point2f& point ) const;
    virtual void Draw( const Entity_ABC& entity, const geometry::Rectangle2f& viewport );

    virtual void AddEntity( const Entity_ABC& );
    virtual bool RemoveEntity( const Entity_ABC& );
    virtual void ActivateEntity( const Entity_ABC& );
    virtual void SelectEntity( const Entity_ABC& );

    virtual void SelectColor( const Entity_ABC& );
    virtual void Select     ( const Entity_ABC&, bool );
    virtual void ContextMenu( const Entity_ABC&, const QPoint& );
    virtual bool ShouldDisplay( const Entity_ABC& );
    virtual bool IsInTeam     ( const Entity_ABC&, const Team& team );

    virtual bool ShouldDisplayTooltip( unsigned i, const geometry::Point2f& point );
    virtual bool DisplayTooltip( unsigned i, const geometry::Point2f& point );
    virtual bool DisplayTooltip( const Entity_ABC&, Displayer_ABC& displayer );
    //@}
    
private:
    //! @name Copy/Assignement
    //@{
    EntityLayerBase( const EntityLayerBase& );            //!< Copy constructor
    EntityLayerBase& operator=( const EntityLayerBase& ); //!< Assignement operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< const Entity_ABC* >  T_Entities;
    typedef T_Entities::iterator             IT_Entities;
    //@}
        
private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    const GlTools_ABC& tools_;
    View_ABC& view_;
    T_Entities entities_;
    unsigned tooltiped_;
    GlTooltip* tooltip_;

protected: // $$$$ AGE 2006-05-17: 
    unsigned selected_;
    SafePointer< Team > currentTeam_;
    //@}
};

// =============================================================================
/** @class  EntityLayer
    @brief  Entity layer
    // $$$$ AGE 2006-05-18: use interfaces instead of templates. On entities and knowledges
*/
// Created: AGE 2006-03-23
// =============================================================================
template< typename ConcreteEntity >
class EntityLayer : public EntityLayerBase
                  , public SelectionObserver< ConcreteEntity >
                  , public ElementObserver_ABC< ConcreteEntity >
                  , public ActivationObserver_ABC< ConcreteEntity >
{

public:
    //! @name Constructors/Destructor
    //@{
             EntityLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view );
    virtual ~EntityLayer();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    EntityLayer( const EntityLayer& );            //!< Copy constructor
    EntityLayer& operator=( const EntityLayer& ); //!< Assignement operator
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const ConcreteEntity& );
    virtual void NotifyDeleted( const ConcreteEntity& );
    virtual void NotifyActivated( const ConcreteEntity& );
    virtual void NotifySelected( const ConcreteEntity* );
    virtual void SelectColor( const Entity_ABC& );
    virtual bool IsInTeam   ( const Entity_ABC&, const Team& team );
    virtual bool DisplayTooltip( const Entity_ABC& entity, Displayer_ABC& displayer );
    virtual void DisplayTooltip( const ConcreteEntity& entity, Displayer_ABC& displayer );
    //@}

protected:
    //! @name Member data
    //@{
    Controllers& controllers_;
    ColorStrategy_ABC& strategy_;
    //@}
};

#include "EntityLayer.inl"

#endif // __EntityLayer_h_
