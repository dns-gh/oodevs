// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Layer_ABC_h_
#define __Layer_ABC_h_

#include "ENT/ENT_Enums.h"
#include "LayersHelpers.h"
#include "clients_kernel/GraphicalEntity_ABC.h"
#include "clients_kernel/DisplayableModesObserver_ABC.h"
#include "clients_kernel/ContextMenu.h"
#include <tools/Observer_ABC.h>
#include <graphics/MapLayer_ABC.h>
#include <boost/noncopyable.hpp>

class ViewFrustum;

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{
    class Viewport_ABC;

// =============================================================================
/** @class  Layer_ABC
    @brief  Layer_ABC
*/
// Created: AGE 2006-03-29
// =============================================================================
class Layer_ABC : public QObject
                , public MapLayer_ABC
                , public tools::Observer_ABC
                , public kernel::DisplayableModesObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Layer_ABC() {}
    virtual ~Layer_ABC() {}
    //@}

    //! @name Types
    //@{
    typedef std::map< Layer_ABC*, kernel::GraphicalEntity_ABC::T_GraphicalEntities > T_LayerElements;

    typedef std::pair< unsigned int, E_LayerTypes > T_ObjectPicking;
    typedef std::vector< T_ObjectPicking >          T_ObjectsPicking;
    //@}

    //! @name Abstract methods
    //@{
    virtual void Paint( const ViewFrustum& frustum ) = 0;
    virtual void Paint( Viewport_ABC& viewport ) = 0;
    using MapLayer_ABC::Paint;
    virtual void Draw( const kernel::Entity_ABC& entity, Viewport_ABC& viewport, bool pickingMode ) = 0;

    virtual E_LayerTypes GetType() const = 0;
    virtual void SetAlpha( float ) = 0;
    virtual float GetAlpha() const = 0;
    virtual void Reset() = 0;
    virtual QString GetName() const = 0;
    virtual std::string GetOptionName() const = 0;
    virtual void Select( unsigned int id, bool control ) = 0;
    virtual void ContextMenu( const kernel::GraphicalEntity_ABC&, const geometry::Point2f&, const QPoint& ) = 0;
    virtual bool ContextMenu( const std::vector< const kernel::GraphicalEntity_ABC* >& elements, const geometry::Point2f&, const QPoint& ) = 0;
    virtual void FillContextMenu( unsigned int id, kernel::ContextMenu& menu ) = 0;
    virtual void FillContextMenu( QMouseEvent* mouse, kernel::ContextMenu& menu, const geometry::Point2f& point ) = 0;
    virtual bool ShowTooltip( const T_ObjectPicking& selection ) = 0;
    virtual void HideTooltip() = 0;
    virtual void ExtractElements( T_LayerElements& extractedElement, const T_ObjectsPicking& selection ) = 0;
    virtual bool IsIn( const kernel::GraphicalEntity_ABC& ) const = 0;
    virtual bool IsEnabled() const = 0;
    virtual bool IsConfigurable() const = 0;
    virtual T_Layer GetSubLayer( const T_LayerFunctor& functor ) const = 0;
    //@}
};

}

#endif // __Layer_ABC_h_
