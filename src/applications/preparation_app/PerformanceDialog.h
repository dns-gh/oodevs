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

namespace gui
{
    template< typename T > class RichWidget;
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

    //! @name Types
    //@{
    enum E_PerformanceData
    {
        ePerformanceData_Profiles = 0,
        ePerformanceData_Units,
        ePerformanceData_Populations,
        ePerformanceData_Crowds,
        ePerformanceData_UrbanBlocs,
        ePerformanceData_Objects,
        ePerformanceData_knowledgeGroups,
        ePerformanceData_TerrainMemorySize,
        ePerformanceData_TerrainSize,
        ePerformanceData_MaxAutomatsKG,
        ePerformanceData_MaxUnitsKG,
        ePerformanceData_AvgAutomatsKG,
        ePerformanceData_AvgUnitsKG,
        ePerformanceData_SingleStationLoadLevel,
        ePerformanceData_MultiStationLoadLevel,
        eNbrPerformanceData
    };
    //@}

    Model& model_;
    const StaticModel& staticModel_;
    QProgressBar* progressValueSingle_;
    QProgressBar* progressValueMulti_;
    QStandardItemModel* dataModel_;

    QLabel* loadLevelSingle_;
    QLabel* loadLevelMulti_;
    QLabel* limitValueSingle_;
    QLabel* limitValueMulti_;
    QLabel* limitLineSingle_;
    QLabel* limitLineMulti_;
    gui::RichWidget< QTextEdit >* textEdit_;

    const int progressLimit_;
};

#endif // __PerformanceDialog_h_
