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

namespace kernel
{
    class OptionsController;
}

namespace gui
{
    class LightingProxy;

// =============================================================================
/** @class  LightingPanel
    @brief  LightingPanel
*/
// Created: SBO 2007-01-03
// =============================================================================
class LightingPanel : public PreferencePanel_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LightingPanel( QWidget* parent,
                            kernel::OptionsController& options );
    virtual ~LightingPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Load( const GlProxy& view );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTypeChanged( int type );
    void DirectionChanged( const geometry::Vector3f& direction );
    void AmbientChanged( const QColor& color );
    void DiffuseChanged( const QColor& color );
    //@}

private:
    //! @name Member data
    //@{
    std::shared_ptr< LightingProxy > lighting_;
    QGroupBox* fixedBox_;
    //@}
};

} //! namespace gui

#endif // __LightingPanel_h_
