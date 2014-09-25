// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LightingPanel_h_
#define __LightingPanel_h_

#include "PreferencePanel_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class OptionsController;
}

namespace gui
{
    class LightingProxy;
    class DirectionWidget;
    class ColorButton;
    class ButtonGroup;
    class RichGroupBox;

// =============================================================================
/** @class  LightingPanel
    @brief  LightingPanel
*/
// Created: SBO 2007-01-03
// =============================================================================
class LightingPanel : public PreferencePanel_ABC
                    , public tools::Observer_ABC
                    , public kernel::OptionsObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LightingPanel( QWidget* parent, LightingProxy& lighting, kernel::Controllers& controllers );
    virtual ~LightingPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Reset();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnLightingType( int type );

    void DirectionChanged( const geometry::Vector3f& direction );
    void AmbientChanged( const QColor& color );
    void DiffuseChanged( const QColor& color );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::OptionsController& options_;
    LightingProxy& lighting_;

    RichGroupBox* fixedLightBox_;
    ButtonGroup* lightingType_;
    DirectionWidget* direction_;
    ColorButton* ambient_;
    ColorButton* diffuse_;
    //@}
};

}

#endif // __LightingPanel_h_
