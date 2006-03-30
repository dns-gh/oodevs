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

class Controllers;

// =============================================================================
/** @class  MapToolbar
    @brief  MapToolbar
*/
// Created: APE 2004-03-16
// =============================================================================
class MapToolbar : public QToolBar
{

public:
    //! @name Constructors/Destructor
    //@{
             MapToolbar( QMainWindow* pParent, Controllers& controllers );
    virtual ~MapToolbar();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MapToolbar( const MapToolbar& );
    MapToolbar& operator=( const MapToolbar& );
    //@}
};

#endif // __MapToolbar_h_
