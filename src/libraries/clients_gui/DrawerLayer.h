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

namespace kernel
{
    class GlTools_ABC;
}

namespace xml
{
    class xistream;
}

namespace gui
{
    class DrawerShape;
    class DrawerStyle;
    class DrawerShapeFactory;
    class DrawerModel;
    class CursorStrategy_ABC;

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
    explicit DrawerLayer( kernel::GlTools_ABC& tools );
    virtual ~DrawerLayer();
    //@}

    //! @name Operations
    //@{
    void TakeFocus( bool take );
    void Show( bool show );
    void Load( const std::string& filename, const DrawerModel& model );
    void Save( const std::string& filename ) const;
    void Clear();

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

    void Done();
    void DeleteSelected();
    float Precision() const;

    void ReadShape( xml::xistream& xis, const DrawerModel& model );
    //@}

    //! @name Types
    //@{
    typedef std::vector< DrawerShape* > T_Shapes;
    typedef T_Shapes::const_iterator  CIT_Shapes;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::GlTools_ABC& tools_;
    std::auto_ptr< CursorStrategy_ABC > cursors_;
    DrawerShapeFactory& factory_;
    bool         show_;
    DrawerShape* current_;
    T_Shapes     shapes_;
    DrawerShape* overlined_;
    DrawerShape* selected_;
    geometry::Point2f dragPoint_;

    const DrawerStyle* selectedStyle_;
    QColor             selectedColor_;
    //@}
};

}

#endif // __DrawerLayer_h_
