// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __TerrainPicker_h_
#define __TerrainPicker_h_

namespace gui
{
    class TerrainLayer;
    class WeatherLayer;
    class ObjectsLayer;

// =============================================================================
/** @class  TerrainPicker
    @brief  TerrainPicker
*/
// Created: SBO 2010-03-26
// =============================================================================
class TerrainPicker : public QObject
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit TerrainPicker( QObject* parent );
    virtual ~TerrainPicker();
    //@}

    //! @name Operations
    //@{
    void RegisterLayer( TerrainLayer& terrain );
    void RegisterLayer( WeatherLayer& terrain );
    void RegisterLayer( ObjectsLayer& objects );
    void Pick( int x, int y, const geometry::Point2f& terrainCoordinates );
    //@}

signals:
    //! @name Signals
    //@{
    void TerrainPicked( const QString& type );
    void WeatherPicked( const QString& precipitation, const QString& lighting, const QString& windInfos, const QString& temperature );
    void ObjectPicked( const QStringList& infos );
    //@}

private slots:
    //! @name
    //@{
    void OnTimeOut();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TerrainPicker( const TerrainPicker& );            //!< Copy constructor
    TerrainPicker& operator=( const TerrainPicker& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    TerrainLayer* terrain_;
    WeatherLayer* weather_;
    ObjectsLayer* objects_;
    QTimer* timer_;
    int x_;
    int y_;
    geometry::Point2f terrainCoordinates_;
    //@}
};

}

#endif // __TerrainPicker_h_
