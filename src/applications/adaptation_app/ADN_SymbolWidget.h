// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_SymbolWidget_h_
#define __ADN_SymbolWidget_h_

#include "ADN_Gfx_ABC.h"
#include "svgl/Point.h"
#include "svgl/BoundingBox.h"
#include "geometry/types.h"
#include <qgl.h>

#undef max

namespace gui
{
    class SvglRenderer;
    class GLSymbols;
}

namespace kernel
{
    class SymbolFactory;
}

// =============================================================================
/** @class  ADN_SymbolWidget
    @brief  ADN_SymbolWidget
*/
// Created: NLD 2010-12-01
// =============================================================================
class ADN_SymbolWidget : public QGLWidget
                       , public ADN_Gfx_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_SymbolWidget( QWidget* parent );
    virtual ~ADN_SymbolWidget();
    //@}

    //! @name OpenGL callbacks
    //@{
    virtual void initializeGL();
    virtual void resizeGL( int w, int h );
    virtual void paintGL();
    void SetSymbolFactory( kernel::SymbolFactory& factory );
    //@}

    //! @name Accessors
    //@{
    bool IsAvailable( const std::string& symbol ) const;
    //@}
private:
    //! @name Copy/Assignment
    //@{
    ADN_SymbolWidget( const ADN_SymbolWidget& );            //!< Copy constructor
    ADN_SymbolWidget& operator=( const ADN_SymbolWidget& ); //!< Assignment operator
    //@}

    //! @name Tools
    //@{
    void DisplaySymbol() const;
    //@}

private:
    geometry::Rectangle2f viewPort_;

    std::auto_ptr< gui::SvglRenderer > renderer_;
    std::auto_ptr< gui::GLSymbols > symbols_;
    kernel::SymbolFactory* symbolFactory_;

    std::string currentSymbol_;

signals:
    //! @name Signals
    //@{
    void SymbolChanged( const QString& symbol );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnNatureChanged( const QString& );
    //@}
};

#endif // __ADN_SymbolWidget_h_
