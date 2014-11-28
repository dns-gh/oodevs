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
    class Controllers;
    class ModelUnLoaded;
    class Options;
    class StaticModel;
}

namespace gui
{
    class Elevation2dLayer;
    class GLOptions;
    class GLMainProxy;
    class GLView_ABC;
    class PreferencesList;
    class PreferencePanel_ABC;
    class TerrainSettings;

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
             PreferencesDialog( QWidget* parent,
                                kernel::Controllers& controllers,
                                const kernel::StaticModel& staticModel,
                                GLMainProxy& view );
    virtual ~PreferencesDialog();
    //@}

    //! @name Operations
    //@{
    virtual void showEvent( QShowEvent * event );
    virtual void reject();

    void AddPage( const QString& name,
                  bool showActiveCombo,
                  PreferencePanel_ABC& page );
    //@}

signals:
    //! @name Signals
    //@{
    void OnAddRaster();
    //@}

private:
    //! @name Helpers
    //@{
    void Load( const GLView_ABC& );
    void UpdateComboVisibility();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    GLMainProxy& mainProxy_;
    std::unique_ptr< kernel::Options > previousGeneralOptions_;
    std::map< int, std::unique_ptr< GLOptions > > previousViewsOptions_;
    PreferencesList* list_;
    QStackedWidget* stack_;
    std::map< PreferencePanel_ABC*, bool > panels_;
    QComboBox* activeCombo_;
    //@}
};

} //! namespace gui

#endif // __PreferencesDialog_h_
