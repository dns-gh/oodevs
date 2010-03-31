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
    void Pick( int x, int y );
    //@}

signals:
    //! @name Signals
    //@{
    void TerrainPicked( const QString& type );
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
    QTimer* timer_;
    int x_;
    int y_;
    //@}
};

}

#endif // __TerrainPicker_h_
