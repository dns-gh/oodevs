// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __TerrainExportDialog_h_
#define __TerrainExportDialog_h_

#include "clients_gui/ModalDialog.h"
#include <extractor/TerrainExtractionManager.h>

namespace tools
{
    class ExerciseConfig;
}

class UrbanModel;

// =============================================================================
/** @class  TerrainExportDialog
    @brief  TerrainExportDialog
*/
// Created: ABR 2012-06-13
// =============================================================================
class TerrainExportDialog : public ModalDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TerrainExportDialog( QWidget* parent, const tools::ExerciseConfig& config, const UrbanModel& urbanModel );
    virtual ~TerrainExportDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void CheckExportReady();
    void OnBrowseExport();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void accept();
    virtual void showEvent( QShowEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    const UrbanModel&            urbanModel_;
    const tools::ExerciseConfig& config_;
    QLineEdit*                   exportPathEditor_;
    QCheckBox*                   shapeCheck_;
    QCheckBox*                   elevationCheck_;
    QCheckBox*                   rasterCheck_;
    QPushButton*                 okButton_;
    //@}
};

#endif // __TerrainExportDialog_h_
