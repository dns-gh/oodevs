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

#include "clients_kernel/OptionsObserver_ABC.h"
#include <tools/Observer_ABC.h>

namespace kernel
{
    class Controllers;
    class Options;
}

namespace gui
{
    class ColorButton;
    class Gradient;
    class GradientButton;
    class Painter_ABC;
    class RichLineEdit;
}

namespace gui
{
// =============================================================================
/** @class  DensityWidget
    @brief  Density widget
*/
// Created: LGY 2011-06-01
// =============================================================================
class DensityWidget : public Q3VBox
                    , public tools::Observer_ABC
                    , public kernel::OptionsObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             DensityWidget( const QString& objectName, QWidget* parent, kernel::Controllers& controllers, const std::string& category );
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
    void OnUnoccupiedColorChanged( const QColor& color );
    void OnGradientEdited( Gradient& gradient );
    void Reset();
    void OnMinChanged( const QString& value );
    void OnMaxChanged( const QString& value );
    //@}

private:
    //! @name Member Data
    //@{
    const std::string category_;
    std::unique_ptr< Painter_ABC > pPainter_;
    kernel::Controllers& controllers_;
    kernel::Options& options_;
    RichLineEdit* max_;
    RichLineEdit* min_;
    GradientButton* densityEditor_;
    ColorButton* color_;
    ColorButton* unoccupiedColor_;
    bool blockLoaded_;
    bool minLoaded_;
    bool maxLoaded_;
    //@}
};

}

#endif // gui_DensityWidget_h
