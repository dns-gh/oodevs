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

#include "tools/Observer_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Options;
}

namespace gui
{
    class GradientButton;
    class ColorButton;
    class Painter_ABC;
    class Gradient;

// =============================================================================
/** @class  DensityWidget
    @brief  Density widget
*/
// Created: LGY 2011-06-01
// =============================================================================
class DensityWidget : public QVBox
                    , public tools::Observer_ABC
                    , public kernel::OptionsObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             DensityWidget( QWidget* parent, kernel::Controllers& controllers );
    virtual ~DensityWidget();
    //@}

    //! @name Operations
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
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
    void OnGradientEdited( Gradient& gradient );
    void Reset();
    //@}

private:
    //! @name Member Data
    //@{
    std::auto_ptr< Painter_ABC > pPainter_;
    kernel::Controllers& controllers_;
    kernel::Options& options_;
    GradientButton* densityEditor_;
    ColorButton* color_;
    //@}
};

}

#endif // gui_DensityWidget_h
