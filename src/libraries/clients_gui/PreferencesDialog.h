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
#include "clients_kernel/ModesObserver_ABC.h"
#include "tools/Observer_ABC.h"

namespace kernel
{
    class CoordinateSystems;
    class Controllers;
}

namespace gui
{
    class Elevation2dLayer;
    class GlSelector;
    class GraphicPreferences;
    class GraphicsPanel;
    class Layer;
    class LayersPanel;
    class LightingProxy;
    class PreferencesList;
    class PreferencePanel_ABC;
    class CoordinateSystemsPanel;
    class Painter_ABC;
    class SoundPlayer;

// =============================================================================
/** @class  PreferencesDialog
    @brief  PreferencesDialog
*/
// Created: SBO 2006-05-03
// =============================================================================
class PreferencesDialog : public ModalDialog
                        , public tools::Observer_ABC
                        , public kernel::ModesObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             PreferencesDialog( QWidget* parent, kernel::Controllers& controllers, LightingProxy& lighting, kernel::CoordinateSystems& coordSystems,
                                const Painter_ABC& painter, GlSelector& selector, Elevation2dLayer& elevation2dLayer, SoundPlayer* soundPlayer );
    virtual ~PreferencesDialog();
    //@}

    //! @name Operations
    //@{
    virtual QSize sizeHint () const;
    virtual void showEvent( QShowEvent * event );
    virtual void reject();
    GraphicPreferences& GetPreferences() const;

    void AddPage( const QString& name, PreferencePanel_ABC& page );
    void AddLayer( const QString& name, gui::Layer& layer, bool dynamic = false );

    void PurgeDialog();
    void BuildPreparationSettings();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnOk();
    void OnCancel();
    //@}

signals:
    void OnAddRaster();

private:


    //! @name Helpers
    //@{
    virtual void NotifyModeChanged( E_Modes newMode );
    //@}

    //! @name Types
    //@{
    typedef std::vector< PreferencePanel_ABC* > T_Pages;
    typedef T_Pages::iterator                  IT_Pages;
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
    E_Modes oldMode_;
    LightingProxy& lighting_;
    Elevation2dLayer& elevation2dLayer_;
    SoundPlayer* soundPlayer_;
    //@}
};

}

#endif // __PreferencesDialog_h_
