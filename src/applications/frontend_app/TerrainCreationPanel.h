// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TerrainCreationPanel_h_
#define __TerrainCreationPanel_h_

#include <qwidgetstack.h>

class QAction;

// =============================================================================
/** @class  TerrainCreationPanel
    @brief  TerrainCreationPanel
*/
// Created: SBO 2007-10-04
// =============================================================================
class TerrainCreationPanel : public QHBox
{

public:
    //! @name Constructors/Destructor
    //@{
             TerrainCreationPanel( QWidgetStack* widget, QAction& action );
    virtual ~TerrainCreationPanel();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TerrainCreationPanel( const TerrainCreationPanel& );            //!< Copy constructor
    TerrainCreationPanel& operator=( const TerrainCreationPanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    QWidgetStack* stack_;
    //@}
};

#endif // __TerrainCreationPanel_h_
