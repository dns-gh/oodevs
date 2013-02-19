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

#include "tools/Observer_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/ActivationObserver_ABC.h"
#include "clients_kernel/MultipleSelectionObserver_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/RectangleSelectionHandler_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "Layer.h"

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class GlTooltip_ABC;
    class Entity_ABC;
    class Team_ABC;
    class Displayer_ABC;
    class Profile_ABC;
    class Viewport_ABC;
}

namespace gui
{
    class ColorStrategy_ABC;
    class View_ABC;
    class LayerFilter_ABC;

// =============================================================================
/** @class  EntityLayerBase
    @brief  Entity layer base
*/
// Created: AGE 2006-03-23
// =============================================================================
class EntityLayerBase : public Layer
{
public:
    //! @name Constructors/Destructor
    //@{
             EntityLayerBase( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const kernel::Profile_ABC& profile, const QString& name );
    virtual ~EntityLayerBase();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( kernel::Viewport_ABC& viewport );
    //@}

protected:
    //! @name Events
    //@{
    virtual bool HandleMouseDoubleClick( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleMouseMove( QMouseEvent* event, const geometry::Point2f& point );
    //@}

    //! @name Helpers
    //@{
    virtual void Initialize( const geometry::Rectangle2f& extent );
    virtual bool IsInSelection( const kernel::Entity_ABC& entity, const geometry::Point2f& point ) const;
    virtual bool IsInside( const kernel::Entity_ABC& entity, const geometry::Rectangle2f& rectangle ) const;
    virtual void Draw( const kernel::Entity_ABC& entity, kernel::Viewport_ABC& viewport );

    template< typename Functor >
    void Apply( Functor& functor ) const
    {
        for( auto it = entities_.begin(); it != entities_.end(); ++it )
            functor( **it );
    }

    virtual void AddEntity( const kernel::Entity_ABC& );
    virtual bool RemoveEntity( const kernel::Entity_ABC& );
    virtual void ActivateEntity( const kernel::Entity_ABC& );
    virtual void SelectEntity( const kernel::Entity_ABC& );

    virtual void SelectColor( const kernel::Entity_ABC& );
    virtual bool ShouldDisplay( const kernel::Entity_ABC& );

    virtual bool ShouldDisplayTooltip( const kernel::Entity_ABC& entity, const geometry::Point2f& point );
    virtual bool DisplayTooltip( const kernel::Entity_ABC& entity, const geometry::Point2f& point );
    virtual bool DisplayTooltip( const kernel::Entity_ABC&, kernel::Displayer_ABC& displayer );

    virtual void SelectInRectangle( const geometry::Point2f& topLeft, const geometry::Point2f& bottomRight );
    //@}

    //! @name Layer_ABC implementation
    //@{
    virtual QString GetName() const;
    virtual void Select( const kernel::GraphicalEntity_ABC&, bool control, bool shift );
    virtual void ContextMenu( const kernel::GraphicalEntity_ABC&, const geometry::Point2f&, const QPoint& );
    virtual void ExtractElements( T_LayerElements& extractedElement, const geometry::Point2f& point );
    //@}

protected:
    //! @name Types
    //@{
    typedef std::vector< const kernel::Entity_ABC* >  T_Entities;
    //@}

protected:
    //! @name Protected member data
    //@{
    const kernel::Profile_ABC& profile_;
    const kernel::GlTools_ABC& tools_;
    T_Entities                 entities_;
    geometry::Rectangle2f      world_;
    //@}

private:
    //! @name Private Member data
    //@{
    QString                                 name_;
    kernel::Controllers&                    controllers_; // TODO protected?? utilisé dans EntityLayer
    ColorStrategy_ABC&                      strategy_;
    View_ABC&                               view_;
    std::auto_ptr< kernel::GlTooltip_ABC >  tooltip_;
    kernel::SafePointer< kernel::Entity_ABC > tooltiped_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}
};

// =============================================================================
/** @class  EntityLayer
    @brief  Entity layer
*/
// Created: AGE 2006-03-23
// =============================================================================
template< typename ConcreteEntity >
class EntityLayer : public EntityLayerBase
                  , public kernel::MultipleSelectionObserver< ConcreteEntity >
                  , public tools::ElementObserver_ABC< ConcreteEntity >
                  , public kernel::ActivationObserver_ABC< ConcreteEntity >
                  , public kernel::RectangleSelectionHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             EntityLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const kernel::Profile_ABC& profile, const QString& name );
    virtual ~EntityLayer();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const ConcreteEntity& );
    virtual void NotifyDeleted( const ConcreteEntity& );
    virtual void NotifyActivated( const ConcreteEntity& );
    virtual void NotifySelectionChanged( const std::vector< const ConcreteEntity* >& elements );
    virtual void SelectColor( const kernel::Entity_ABC& );
    virtual void ContextMenu( const kernel::GraphicalEntity_ABC&, const geometry::Point2f&, const QPoint& );
    virtual void HandleRectangleSelection( const geometry::Point2f& topLeft, const geometry::Point2f& bottomRight );
    virtual bool IsIn( const kernel::GraphicalEntity_ABC& selectable ) const;
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ColorStrategy_ABC& strategy_;
    //@}
};

}

#include "EntityLayer.inl"

#endif // __EntityLayer_h_
