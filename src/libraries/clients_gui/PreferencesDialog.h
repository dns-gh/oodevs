// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PreferencesDialog_h_
#define __PreferencesDialog_h_

#include "ModalDialog.h"
#include "SoundManager.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/Observer_ABC.h>

namespace kernel
{
    class CoordinateConverter_ABC;
    class Controllers;
    class ModelUnLoaded;
}

namespace gui
{
    class Elevation2dLayer;
    class GlProxy;
    class GraphicPreferences;
    class GraphicsPanel;
    class Layer_ABC;
    class LayersPanel;
    class LightingProxy;
    class PreferencesList;
    class PreferencePanel_ABC;
    class CoordinateSystemsPanel;
    class Painter_ABC;

// =============================================================================
/** @class  PreferencesDialog
    @brief  PreferencesDialog
*/
// Created: SBO 2006-05-03
// =============================================================================
class PreferencesDialog : public ModalDialog
                        , public tools::Observer_ABC
                        , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             PreferencesDialog( QWidget* parent,
                                kernel::Controllers& controllers,
                                LightingProxy& lighting,
                                kernel::CoordinateConverter_ABC& coordConverter,
                                const Painter_ABC& painter,
                                GlProxy& proxy,
                                const std::shared_ptr< Elevation2dLayer >& elevation2dLayer,
                                GraphicPreferences& preferences );
    virtual ~PreferencesDialog();
    //@}

    //! @name Operations
    //@{
    virtual QSize sizeHint () const;
    virtual void showEvent( QShowEvent * event );
    virtual void reject();

    void AddPage( const QString& name, PreferencePanel_ABC& page );
    void AddLayer( const QString& name, const std::shared_ptr< gui::Layer_ABC >& layer, bool dynamic = false );

    void PurgeDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnOk();
    void OnCancel();
    //@}

signals:
    //! @name Signals
    //@{
    void OnAddRaster();
    //@}

private:

    //! @name Helpers
    //@{
    void BuildSettings();
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    //@}

    //! @name Types
    //@{
    typedef std::vector< PreferencePanel_ABC* > T_Pages;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Painter_ABC& painter_;
    T_Pages pages_;
    PreferencesList* list_;
    LayersPanel* layersPanel_;
    GraphicsPanel* pGraphicPrefPanel_;
    CoordinateSystemsPanel* pCoordinateSystemsPanel_;
    LightingProxy& lighting_;
    std::shared_ptr< Elevation2dLayer > elevation2dLayer_;
    //@}
};

}

#endif // __PreferencesDialog_h_
