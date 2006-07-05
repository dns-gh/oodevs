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
// $Archive: /MVW_v10/Build/SDK/Light2/src/WeatherPanel.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:29 $
// $Revision: 4 $
// $Workfile: WeatherPanel.h $
//
// *****************************************************************************

#ifndef __WeatherPanel_h_
#define __WeatherPanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Meteo_ABC.h"

class ShapeEditorMapEventFilter;
class QDial;
class QSpinBox;
class QListView;


// =============================================================================
/** @class  WeatherPanel
    @brief  WeatherPanel
    @par    Using example
    @code
    WeatherPanel;
    @endcode
*/
// Created: APE 2004-05-17
// =============================================================================
class WeatherPanel : public QVBox
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( WeatherPanel );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
     WeatherPanel( QWidget* pParent );
    ~WeatherPanel();
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

    MT_ValuedComboBox<Meteo_ABC::E_WeatherType>* pWeatherTypeCombo_;
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

    ShapeEditorMapEventFilter* pLineEditor_;
    //@}
};

#endif // __WeatherPanel_h_
