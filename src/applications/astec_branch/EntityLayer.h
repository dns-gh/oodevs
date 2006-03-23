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

#include "Observer_ABC.h"
#include "ElementObserver_ABC.h"
#include "ActivationObserver_ABC.h"
#include "graphics/MapLayer_ABC.h"

class Controllers;
class GlTools_ABC;
class ColorStrategy_ABC;
class MapWidget;
class Entity_ABC;

// =============================================================================
/** @class  EntityLayerBase
    @brief  EntityLayerBase
*/
// Created: AGE 2006-03-23
// =============================================================================
class EntityLayerBase : public MapLayer_ABC
                      , public Observer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{    
             EntityLayerBase( const GlTools_ABC& tools, MapWidget& widget );
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
    //@}

    //! @name Helpers
    //@{
    virtual bool IsInSelection( const Entity_ABC& entity, const geometry::Point2f& point ) const;
    virtual void Draw( const Entity_ABC& entity, const geometry::Rectangle2f& viewport );

    virtual void AddEntity( const Entity_ABC& );
    virtual void RemoveEntity( const Entity_ABC& );
    virtual void ActivateEntity( const Entity_ABC& );

    virtual void SelectColor( const Entity_ABC& );
    virtual void Select     ( const Entity_ABC& );
    virtual void ContextMenu( const Entity_ABC&, const QPoint&  );
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
    const GlTools_ABC& tools_;
    MapWidget& widget_;

    T_Entities entities_;
    unsigned selected_;
    //@}
};

// =============================================================================
/** @class  EntityLayer
    @brief  EntityLayer
*/
// Created: AGE 2006-03-23
// =============================================================================
template< typename ConcreteEntity >
class EntityLayer : public EntityLayerBase
                  , public ElementObserver_ABC< ConcreteEntity >
                  , public ActivationObserver_ABC< ConcreteEntity >
{

public:
    //! @name Constructors/Destructor
    //@{
             EntityLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, MapWidget& widget );
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
    virtual void NotifyUpdated( const ConcreteEntity& );
    virtual void NotifyDeleted( const ConcreteEntity& );
    virtual void NotifyActivated( const ConcreteEntity& );
    virtual void SelectColor( const Entity_ABC& );
    virtual void Select     ( const Entity_ABC& );
    virtual void ContextMenu( const Entity_ABC&, const QPoint&  );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    ColorStrategy_ABC& strategy_;
    //@}
};

#include "EntityLayer.inl"

#endif // __EntityLayer_h_
