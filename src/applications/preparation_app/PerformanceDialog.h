// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PerformanceDialog_h_
#define __PerformanceDialog_h_

namespace kernel
{
    class Controllers;
}

class Model;
class StaticModel;

// =============================================================================
/** @class  PerformanceDialog
    @brief  PerformanceDialog
*/
// Created: MMC 2012-02-02
// =============================================================================
class PerformanceDialog  : public QDialog
{
    Q_OBJECT;

public:
    //! @name Constructor/Destructor
    //@{
             PerformanceDialog( QWidget* parent, Model& model, const StaticModel& staticModel );
    virtual ~PerformanceDialog();
    //@}

    //! @name Operations
    //@{
    virtual void showEvent( QShowEvent* pEvent );
    //@}

private:

    //! @name Operations
    //@{
    void UpdateDisplay();
    //@}

    Model& model_;
    const StaticModel& staticModel_;
    QProgressBar* progressValueSingle_;
    QProgressBar* progressValueMulti_;

    QLabel* units_;
    QLabel* urbanBlocs_;
    QLabel* objects_;
    QLabel* populations_;
    QLabel* crowds_;
    QLabel* knowledges_;
    QLabel* loadLevelSingle_;
    QLabel* loadLevelMulti_;
    QLabel* limitValueSingle_;
    QLabel* limitValueMulti_;
    QLabel* limitLineSingle_;
    QLabel* limitLineMulti_;
    QLabel* terrainLoad_;
    QLabel* terrainSize_;
    QLabel* profiles_;
    QLabel* maxAutomatsKG_;
    QLabel* maxUnitsKG_;
    QLabel* avgAutomatKG_;
    QLabel* avgUnitsKG_;
    QTextEdit* textEdit_;

    const int progressLimit_;
};

#endif // __PerformanceDialog_h_
