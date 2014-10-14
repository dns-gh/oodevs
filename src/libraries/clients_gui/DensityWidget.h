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

namespace kernel
{
    class OptionsController;
}

namespace gui
{
    class Gradient;
    class GradientWidget;

// =============================================================================
/** @class  DensityWidget
    @brief  Density widget
*/
// Created: LGY 2011-06-01
// =============================================================================
class DensityWidget : public QWidget
                    , public tools::Observer_ABC
                    , public kernel::OptionsObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             DensityWidget( kernel::OptionsController& options,
                            const QString& objectName,
                            const std::string& optionName,
                            QWidget* parent = 0 );
    virtual ~DensityWidget();
    //@}

    //! @name Operations
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnGradientEdited();
    //@}

private:
    //! @name Member Data
    //@{
    kernel::OptionsController& options_;
    const std::string optionName_;
    std::shared_ptr< Gradient > gradient_;
    GradientWidget* gradientWidget_;
    //@}
};

}

#endif // gui_DensityWidget_h
