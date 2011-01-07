// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef gui_DensityWidget_h
#define gui_DensityWidget_h

namespace gui
{
    class GradientButton;
    class ColorButton;
    class Painter_ABC;

// =============================================================================
/** @class  DensityWidget
    @brief  Density widget
*/
// Created: LGY 2011-06-01
// =============================================================================
class DensityWidget : public QVBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit DensityWidget( QWidget* parent );
    virtual ~DensityWidget();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DensityWidget( const DensityWidget& );            //!< Copy constructor
    DensityWidget& operator=( const DensityWidget& ); //!< Assignment operator
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged( const QColor& color );
    void OnColorChanged( const QColor& color );
    //@}

private:
    //! @name Member Data
    //@{
    std::auto_ptr< Painter_ABC > pPainter_;
    GradientButton* densityEditor_;
    ColorButton* color_;
    //@}
};

}

#endif // gui_DensityWidget_h
