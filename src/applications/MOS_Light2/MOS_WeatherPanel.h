// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_WeatherPanel.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:29 $
// $Revision: 4 $
// $Workfile: MOS_WeatherPanel.h $
//
// *****************************************************************************

#ifndef __MOS_WeatherPanel_h_
#define __MOS_WeatherPanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Meteo_ABC.h"

class MOS_ShapeEditorMapEventFilter;
class QDial;
class QSpinBox;
class QListView;


// =============================================================================
/** @class  MOS_WeatherPanel
    @brief  MOS_WeatherPanel
    @par    Using example
    @code
    MOS_WeatherPanel;
    @endcode
*/
// Created: APE 2004-05-17
// =============================================================================
class MOS_WeatherPanel : public QVBox
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_WeatherPanel );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
     MOS_WeatherPanel( QWidget* pParent );
    ~MOS_WeatherPanel();
    //@}

    //! @name Operations
    //@{
    void Init();
    //@}

    //! @name Accessors
    //@{
    //@}

    //! @name Modifiers
    //@{
    //@}
private slots:
    void OnSelectionChanged( QListViewItem* pItem );
    void OnRequestPopup( QListViewItem* pItem, const QPoint& pos );
    void OnDelete();
    void OnCreate();

    void SetLocation();
    void LocationDone();
    void OnDataChanged();

    void OnOk();
    void OnCancel();

private:
    //! @name Helpers
    //@{
    enum E_WeatherLocation
    {
        eGlobal = 1000,
        eLocal  = 1001
    };
    //@}

private:
    //! @name Member data
    //@{
    QListView* pWeatherListView_;

    MT_ValuedComboBox<MOS_Meteo_ABC::E_WeatherType>* pWeatherTypeCombo_;
    QSpinBox* pTemperatureSpinBox_;

    QSpinBox* pWindSpeedSpinBox_;
    QDial* pWindDirectionDial_;

    QSpinBox* pCloudCoverFloorSpinBox_;
    QSpinBox* pCloudCoverCeilingSpinBox_;
    QSpinBox* pCloudDensitySpinBox_;
    
    QLabel* pLocationValue1_;
    QLabel* pLocationValue2_;
    QPushButton* pLocationButton_;
    MT_Vector2D point1_;
    MT_Vector2D point2_;

    QPushButton* pOkButton_;
    QPushButton* pCancelButton_;

    QPopupMenu* pPopupMenu_;

    MOS_ShapeEditorMapEventFilter* pLineEditor_;
    //@}
};

#endif // __MOS_WeatherPanel_h_
