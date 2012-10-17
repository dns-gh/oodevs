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

#include <boost/noncopyable.hpp>
#include "clients_gui/ModalDialog.h"

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
    void OnBrowseExportShp();
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
    QLineEdit*                   pathEditor_;
    QPushButton*                 okButton_;
    //@}
};

#endif // __TerrainExportDialog_h_
