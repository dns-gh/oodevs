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

namespace gui
{
    class DrawerShape;
    class DrawerStyle;

// =============================================================================
/** @class  DrawerLayer
    @brief  DrawerLayer
*/
// Created: AGE 2006-08-31
// =============================================================================
class DrawerLayer : public Layer2d_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DrawerLayer();
    virtual ~DrawerLayer();
    //@}

    //! @name Operations
    //@{
    void Show( bool show );

    void StartShape( const DrawerStyle& style );

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

    void Done();
    //@}

    //! @name Types
    //@{
    typedef std::vector< DrawerShape* > T_Shapes;
    typedef T_Shapes::const_iterator  CIT_Shapes;
    //@}

private:
    //! @name Member data
    //@{
    bool         show_;
    DrawerShape* current_;
    T_Shapes     shapes_;
    //@}
};

}

#endif // __DrawerLayer_h_
