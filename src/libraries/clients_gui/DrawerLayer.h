// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawerLayer_h_
#define __DrawerLayer_h_

#include "Layer_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class GlTools_ABC;
    class Controllers;
}

namespace xml
{
    class xistream;
}

namespace gui
{
    class DrawerShape;
    class DrawerStyle;
    class DrawerFactory;
    class CursorStrategy_ABC;

// =============================================================================
/** @class  DrawerLayer
    @brief  DrawerLayer
*/
// Created: AGE 2006-08-31
// =============================================================================
class DrawerLayer : public Layer2d_ABC
                  , public kernel::Observer_ABC
                  , public kernel::ElementObserver_ABC< DrawerShape >
{

public:
    //! @name Constructors/Destructor
    //@{
             DrawerLayer( kernel::Controllers& controllers, kernel::GlTools_ABC& tools, DrawerFactory& factory );
    virtual ~DrawerLayer();
    //@}

    //! @name Operations
    //@{
    void TakeFocus( bool take );
    void Show( bool show );

    void ChangeColor( const QColor& color );
    void StartShape( const DrawerStyle& style, const QColor& color );

    virtual void Paint( const geometry::Rectangle2f& viewport );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DrawerLayer( const DrawerLayer& );            //!< Copy constructor
    DrawerLayer& operator=( const DrawerLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual bool HandleKeyPress  ( QKeyEvent* key );
    virtual bool HandleMouseMove ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual void NotifyCreated( const DrawerShape& );
    virtual void NotifyDeleted( const DrawerShape& );

    void Done();
    float Precision() const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< DrawerShape* > T_Shapes;
    typedef T_Shapes::iterator         IT_Shapes;
    typedef T_Shapes::const_iterator  CIT_Shapes;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::GlTools_ABC& tools_;
    std::auto_ptr< CursorStrategy_ABC > cursors_;
    DrawerFactory& factory_;
    bool         show_;
    kernel::SafePointer< DrawerShape > current_;
    kernel::SafePointer< DrawerShape > overlined_;
    kernel::SafePointer< DrawerShape > selected_;
    T_Shapes     shapes_;
    geometry::Point2f dragPoint_;

    const DrawerStyle* selectedStyle_;
    QColor             selectedColor_;
    //@}
};

}

#endif // __DrawerLayer_h_
