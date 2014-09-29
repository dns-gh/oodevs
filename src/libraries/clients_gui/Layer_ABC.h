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
#include "clients_kernel/GraphicalEntity_ABC.h"
#include "clients_kernel/DisplayableModesObserver_ABC.h"
#include "clients_kernel/ContextMenu.h"
#include <tools/Observer_ABC.h>
#include <graphics/MapLayer_ABC.h>
#include <boost/noncopyable.hpp>

class ViewFrustum;

namespace gui
{
    class Viewport_ABC;

// =============================================================================
/** @class  Layer_ABC
    @brief  Layer_ABC
*/
// Created: AGE 2006-03-29
// =============================================================================
class Layer_ABC : public MapLayer_ABC
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
    typedef std::vector< std::shared_ptr< Layer_ABC > > T_Layers;
    typedef std::map< Layer_ABC*, kernel::GraphicalEntity_ABC::T_GraphicalEntities > T_LayerElements;

    typedef std::pair< unsigned int, E_LayerTypes > T_ObjectPicking;
    typedef std::vector< T_ObjectPicking >          T_ObjectsPicking;
    //@}

    //! @name Abstract methods
    //@{
    virtual void Paint( const ViewFrustum& frustum ) = 0;
    virtual void Paint( Viewport_ABC& viewport ) = 0;
    using MapLayer_ABC::Paint;

    virtual E_LayerTypes GetType() const = 0;
    virtual void SetAlpha( float ) = 0;
    virtual float GetAlpha() const = 0;
    virtual void Reset() = 0;
    virtual QString GetName() const = 0;
    virtual void Select( const kernel::GraphicalEntity_ABC&, bool control, bool shift ) = 0;
    virtual void ContextMenu( const kernel::GraphicalEntity_ABC&, const geometry::Point2f&, const QPoint& ) = 0;
    virtual bool ContextMenu( const std::vector< const kernel::GraphicalEntity_ABC* >& elements, const geometry::Point2f&, const QPoint& ) = 0;
    virtual void FillContextMenu( const kernel::GraphicalEntity_ABC& entity, kernel::ContextMenu& menu ) = 0;
    virtual void FillContextMenu( QMouseEvent* mouse, kernel::ContextMenu& menu, const geometry::Point2f& point ) = 0;
    virtual bool ShowTooltip( const T_ObjectPicking& selection ) = 0;
    virtual void HideTooltip() = 0;
    virtual void ExtractElements( T_LayerElements& extractedElement, const T_ObjectsPicking& selection ) = 0;
    virtual bool IsIn( const kernel::GraphicalEntity_ABC& ) const = 0;
    virtual bool IsEnabled() const = 0;
    virtual void SetPasses( const std::string& passes ) = 0;
    //@}
};

}

#endif // __Layer_ABC_h_
