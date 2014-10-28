// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef FireColorPanel_h
#define FireColorPanel_h

#include "FireOption.h"
#include "PreferencePanel_ABC.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/Observer_ABC.h>

namespace kernel
{
    class Controllers;
    class ModelLoaded;
    class OptionsController;
    class StaticModel;
}

namespace gui
{
    class Model;

    struct FireContext
    {
        T_FireOptions data;
        QGridLayout*  grid;
        Model*        names;
        Model*        karmas;
    };

// =============================================================================
/** @class  FireColorPanel
    @brief  Fire color panel
*/
// =============================================================================
class FireColorPanel : public PreferencePanel_ABC
                     , public tools::Observer_ABC
                     , public tools::ElementObserver_ABC< kernel::ModelLoaded >
{
    Q_OBJECT;

public:
             FireColorPanel( QWidget* parent,
                             kernel::Controllers& controllers,
                             const kernel::StaticModel& model,
                             FireGroup group );
    virtual ~FireColorPanel();

private:
    void SetupUi( QScrollArea* parent );
    void SetupDirect();
    void SetupIndirect();
    void RemoveFire( int idx );
    void FillGrid();
    void SetupGroup( QLayout* root, const QString& header );
    void Save();

    virtual void Load( const GlProxy& );
    virtual void NotifyUpdated( const kernel::ModelLoaded& );

private slots:
    void AddRule();
    void ClearAll();

private:
    const FireGroup group_;
    kernel::Controllers& controllers_;
    const kernel::StaticModel& model_;
    kernel::OptionsController& options_;
    FireContext ctx_;
};

}

#endif // FireColorPanel_h
