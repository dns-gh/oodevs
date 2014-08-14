// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __WeaponRangesPanel_h_
#define __WeaponRangesPanel_h_

#include "PreferencePanel_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class ModelLoaded;
    class StaticModel;
    class VolumeType;
}

namespace gui
{
// =============================================================================
/** @class  WeaponRangesPanel
    @brief  WeaponRangesPanel
*/
// Created: MCO 2014-08-13
// =============================================================================
class WeaponRangesPanel : public PreferencePanel_ABC
                        , public tools::Observer_ABC
                        , public kernel::OptionsObserver_ABC
                        , public tools::ElementObserver_ABC< kernel::ModelLoaded >
{
    Q_OBJECT

public:
             WeaponRangesPanel( QWidget* parent, kernel::Controllers& controllers, const kernel::StaticModel& model );
    virtual ~WeaponRangesPanel();

    virtual void Commit();
    virtual void Reset();

private slots:
    void OnPhChanged( int value );
    void OnVolumeChanged( int index );
    void OnFilterToggled( bool index );
    void OnIndirectWeaponChanged( const QString& value );

private:
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    virtual void NotifyUpdated( const kernel::ModelLoaded& model );

private:
    kernel::Controllers& controllers_;
    const kernel::StaticModel& model_;
    QComboBox* volumeCombo_;
    QSpinBox* phSpinbox_;
    QCheckBox* filterCheckBox_;
    QComboBox* indirectWeaponCombo_;
    int ph_;
    int volume_;
    bool filter_;
    QString indirectWeapon_;
};

}

#endif // __WeaponRangesPanel_h_
