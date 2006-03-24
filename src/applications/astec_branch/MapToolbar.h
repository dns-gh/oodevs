// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-16 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/MapToolbar.h $
// $Author: Age $
// $Modtime: 13/05/05 17:27 $
// $Revision: 3 $
// $Workfile: MapToolbar.h $
//
// *****************************************************************************

#ifndef __MapToolbar_h_
#define __MapToolbar_h_

#include "OptionsObserver_ABC.h"
#include "Observer_ABC.h"

class MT_ToolListButton;
class QToolButton;
class Controllers;
class Options;

// =============================================================================
/** @class  MapToolbar
    @brief  MapToolbar
*/
// Created: APE 2004-03-16
// =============================================================================
class MapToolbar : public QToolBar
                 , private Observer_ABC
                 , public OptionsObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             MapToolbar( QMainWindow* pParent, Controllers& controllers );
    virtual ~MapToolbar();
    //@}

private:
    //! @name Helpers
    //@{
    void FillButton( MT_ToolListButton& button );
    virtual void OptionChanged( const std::string& name, const OptionVariant& value );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MapToolbar( const MapToolbar& );
    MapToolbar& operator=( const MapToolbar& );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSmallText( float rValue );
    void OnLargeText( float rValue );
    void OnDetails( float rValue );
    void OnTacticalLines( float rValue );
    void OnGrid( float rValue );
    void OnWeatherToggled();
    void On3DToggle();
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    Options&     options_;

    QToolButton* pWeatherButton_;
    QToolButton* p3DButton_;

    MT_ToolListButton* pSmallTextButton_;
    MT_ToolListButton* pLargeTextButton_;
    MT_ToolListButton* pDetailsButton_;
    MT_ToolListButton* pTacticalLinesButton_;
    MT_ToolListButton* pGridStep_;
    //@}
};

#endif // __MapToolbar_h_
