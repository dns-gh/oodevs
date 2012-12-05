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
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             PreferencesDialog( QWidget* parent, kernel::Controllers& controllers, LightingProxy& lighting, kernel::CoordinateSystems& coordSystems,
                                const Painter_ABC& painter, GlSelector& selector );
    virtual ~PreferencesDialog();
    //@}

    //! @name Operations
    //@{
    virtual QSize sizeHint () const;
    virtual void showEvent( QShowEvent * event );
    virtual void reject();
    GraphicPreferences& GetPreferences() const;

    void AddPage( const QString& name, PreferencePanel_ABC& page );
    void AddLayer( const QString& name, gui::Layer_ABC& layer, bool dynamic = false );
    void AddLayer( const QString& name, gui::Elevation2dLayer& layer ); // $$$$ AGE 2007-01-17:
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
    //@}
};

}

#endif // __PreferencesDialog_h_
