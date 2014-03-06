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

#include "clients_kernel/DisplayableModesObserver_ABC.h"
#include <tools/Observer_ABC.h>

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
                , public tools::Observer_ABC
                , public kernel::DisplayableModesObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             StatusBar( kernel::Controllers& controllers,
                        QStatusBar* bar,
                        TerrainPicker& picker,
                        const kernel::DetectionMap& detection,
                        const kernel::CoordinateConverter_ABC& converter,
                        QObject& selector );
    virtual ~StatusBar();
    //@}

    //! @name DisplayableModesObserver_ABC
    //@{
    virtual void SetVisible( bool visible );
    virtual void ForceEnabled( bool enabled );
    virtual void EnsureIsEnabled();
    virtual bool IsVisible() const;
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
    //! @name Helpers
    //@{
    QLabel* AddField( QStatusBar* parent, unsigned int size, const QString& title, bool checked );
    QLabel* AddField( QStatusBar* parent, unsigned int size, E_CoordinateSystem coordSystem, bool checked );
    void ParameterSelected( int index, bool save );
    void SaveSettings();
    //@}

    //! @name Types
    //@{
    typedef std::map< int, QLabel* > T_CoordinateFields;
    typedef std::vector< QLabel* >   T_MenuFields;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::DetectionMap& detection_;
    const kernel::CoordinateConverter_ABC& converter_;
    TerrainPicker& terrainPicker_;

    T_CoordinateFields coordinateFields_;
    T_MenuFields menuFields_;
    QLabel* pElevation_;
    QLabel* pTerrainType_;
    QLabel* pObjectInfos_;
    QMenu* pMenu_;
    QStatusBar* parent_;
    //@}
};

}

#endif // __Gui_StatusBar_h_
