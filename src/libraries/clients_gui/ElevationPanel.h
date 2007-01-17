// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ElevationPanel_h_
#define __ElevationPanel_h_

#include "PreferencePanel_ABC.h"

namespace gui
{

class Elevation2dLayer;
class ColorButton;

// =============================================================================
/** @class  ElevationPanel
    @brief  ElevationPanel
*/
// Created: AGE 2007-01-17
// =============================================================================
class ElevationPanel : public PreferencePanel_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ElevationPanel( QWidget* parent, Elevation2dLayer& layer );
    virtual ~ElevationPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Reset();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnColorChanged();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ElevationPanel( const ElevationPanel& );            //!< Copy constructor
    ElevationPanel& operator=( const ElevationPanel& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Elevation2dLayer& layer_;
    ColorButton* min_;
    ColorButton* max_;
    //@}
};

}

#endif // __ElevationPanel_h_
