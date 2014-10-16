// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef PickingSelector_h
#define PickingSelector_h

#include "Layer_ABC.h"
#include "GLView_ABC.h"
#include <geometry/Types.h>
#include <boost/noncopyable.hpp>
#include <boost/optional/optional_fwd.hpp>
#include <set>
#include <tuple>

namespace gui
{
// =============================================================================
/** @class  PickingSelector
    @brief  Picking selector
*/
// Created: LGY 2013-03-13
// =============================================================================
class PickingSelector : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PickingSelector();
    virtual ~PickingSelector();
    //@}

    //! @name Operations
    //@{
    void FillSelection( GLView_ABC::T_ObjectsPicking& selection,
            const boost::optional< E_LayerTypes >& type, const std::function< void() >& paint );
    void Picking( QPoint point, unsigned int windowHeight );
    void RenderPicking( const GLView_ABC::T_ObjectPicking& object, std::function< void( float, float, float, float ) > setCurrentColor );
    bool IsPickingMode() const;
    bool ShouldDisplay( E_LayerTypes type ) const;
    QColor GetColor() const;
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< unsigned int, E_LayerTypes > T_Object;
    typedef std::tuple< int, int, int, T_Object >   T_RenderObject;
    typedef std::vector< T_RenderObject >           T_RenderObjects;
    //@}

private:
    //! @name Member data
    //@{
    bool pickingMode_;
    std::set< E_LayerTypes > pickingLayers_;
    T_RenderObjects renderObjects_;
    GLView_ABC::T_ObjectsPicking pickObjects_;
    QColor color_;
    //@}
};

}

#endif // PickingSelector_h
