// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TerrainCreationPanel_h_
#define __TerrainCreationPanel_h_

#include "Panel_ABC.h"

namespace tools
{
    class GeneralConfig;
}
class QLineEdit;
class InfoBubble;

// =============================================================================
/** @class  TerrainCreationPanel
    @brief  TerrainCreationPanel
*/
// Created: SBO 2007-10-04
// =============================================================================
class TerrainCreationPanel : public Panel_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             TerrainCreationPanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config );
    virtual ~TerrainCreationPanel();
    //@}

private slots:
    //! @name Slots
    //@{
    void CreateTerrain();
    void NameChanged( const QString& );
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

    //! @name Operations
    //@{
    virtual void Update();
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QStringList existingTerrains_;
    QLineEdit* name_;
    QPushButton* okay_;
    InfoBubble* bubble_;
    //@}
};

#endif // __TerrainCreationPanel_h_
