// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Gui_StatusBar_h_
#define __Gui_StatusBar_h_

#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class CoordinateConverter_ABC;
    class DetectionMap;
}

namespace gui
{
    class TerrainPicker;

// =============================================================================
/** @class  StatusBar
    @brief  Status bar
*/
// Created: SBO 2006-04-14
// =============================================================================
class StatusBar : public QObject
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             StatusBar( QStatusBar* bar, TerrainPicker& picker, const kernel::DetectionMap& detection, const kernel::CoordinateConverter_ABC& converter );
    virtual ~StatusBar();
    //@}

public slots:
    //! @name Slots
    //@{
    void OnMouseMove( const geometry::Point2f& position );
    void OnMouseMove( const geometry::Point3f& position );
    void ParameterSelected( int index );
    void TerrainPicked( const QString& type );
    void ObjectPicked( const QStringList& infos );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StatusBar( const StatusBar& );            //!< Copy constructor
    StatusBar& operator=( const StatusBar& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    QLabel* AddField( QStatusBar* parent, unsigned int size, const QString& title, bool checked );
    QLabel* AddField( QStatusBar* parent, unsigned int size, int id, bool checked );
    //@}

    //! @name Types
    //@{
    typedef std::map< int, QLabel* > T_CoordinateFields;
    typedef std::vector< QLabel* >   T_MenuFields;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::DetectionMap& detection_;
    const kernel::CoordinateConverter_ABC& converter_;
    TerrainPicker& terrainPicker_;

    T_CoordinateFields coordinateFields_;
    T_MenuFields menuFields_;
    QLabel* pElevation_;
    QLabel* pTerrainType_;
    QLabel* pObjectInfos_;
    QPopupMenu* pMenu_;
    //@}
};

}

#endif // __Gui_StatusBar_h_
