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

#include "clients_kernel/Observer_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/ActivationObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "Layer_ABC.h"

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class Entity_ABC;
    class Team_ABC;
    class Displayer_ABC;
}

namespace gui
{
    class ColorStrategy_ABC;
    class View_ABC;
    class GlTooltip;

// =============================================================================
/** @class  EntityLayerBase
    @brief  Entity layer base
*/
// Created: AGE 2006-03-23
// =============================================================================
class EntityLayerBase : public Layer_ABC
                      , public kernel::Observer_ABC
                      , public kernel::OptionsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{    
             EntityLayerBase( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, View_ABC& view );
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
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

    //! @name Helpers
    //@{
    virtual bool IsInSelection( const kernel::Entity_ABC& entity, const geometry::Point2f& point ) const;
    virtual void Draw( const kernel::Entity_ABC& entity, const geometry::Rectangle2f& viewport );

    virtual void AddEntity( const kernel::Entity_ABC& );
    virtual bool RemoveEntity( const kernel::Entity_ABC& );
    virtual void ActivateEntity( const kernel::Entity_ABC& );
    virtual void SelectEntity( const kernel::Entity_ABC& );

    virtual void SelectColor( const kernel::Entity_ABC& );
    virtual void Select     ( const kernel::Entity_ABC&, bool );
    virtual void ContextMenu( const kernel::Entity_ABC&, const geometry::Point2f&, const QPoint& );
    virtual bool ShouldDisplay( const kernel::Entity_ABC& );
    virtual bool IsInTeam     ( const kernel::Entity_ABC&, const kernel::Team_ABC& team );

    virtual bool ShouldDisplayTooltip( unsigned i, const geometry::Point2f& point );
    virtual bool DisplayTooltip( unsigned i, const geometry::Point2f& point );
    virtual bool DisplayTooltip( const kernel::Entity_ABC&, kernel::Displayer_ABC& displayer );
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
    typedef std::vector< const kernel::Entity_ABC* >  T_Entities;
    typedef T_Entities::iterator                     IT_Entities;
    //@}
        
private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::GlTools_ABC& tools_;
    View_ABC& view_;
    T_Entities entities_;
    unsigned tooltiped_;
    GlTooltip* tooltip_;

protected: // $$$$ AGE 2006-05-17: 
    unsigned selected_;
    kernel::SafePointer< kernel::Team_ABC > currentTeam_;
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
                  , public kernel::SelectionObserver< ConcreteEntity >
                  , public kernel::ElementObserver_ABC< ConcreteEntity >
                  , public kernel::ActivationObserver_ABC< ConcreteEntity >
{

public:
    //! @name Constructors/Destructor
    //@{
             EntityLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view );
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
    virtual void SelectColor( const kernel::Entity_ABC& );
    virtual void ContextMenu( const kernel::Entity_ABC&, const geometry::Point2f&, const QPoint& );
    virtual bool DisplayTooltip( const kernel::Entity_ABC& entity, kernel::Displayer_ABC& displayer );
    virtual void DisplayTooltip( const ConcreteEntity& entity, kernel::Displayer_ABC& displayer );
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
