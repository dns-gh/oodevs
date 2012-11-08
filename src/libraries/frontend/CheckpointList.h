// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __CheckpointList_h_
#define __CheckpointList_h_

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{

// =============================================================================
/** @class  CheckpointList
    @brief  CheckpointList
*/
// Created: SBO 2010-04-21
// =============================================================================
class CheckpointList : public QWidget
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             CheckpointList( const tools::GeneralConfig& config );
    virtual ~CheckpointList();
    //@}

    //! @name Operations
    //@{
    void Update( const QString& exercice, const QString& session );
    void ClearSelection();
    //@}

signals:
    //! @name Signals
    //@{
    void Select( const QString& checkpoint );
    //@}

public slots:
    //! @name Slots
    //@{
    void Toggle( bool enabled );
    //@}

private slots:
    //! @name Slots
    //@{
    void SelectCheckpoint( int index );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CheckpointList( const CheckpointList& );            //!< Copy constructor
    CheckpointList& operator=( const CheckpointList& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QListWidget* list_;
    QString exercise_;
    QString session_;
    QStringList checkpoints_;
    bool enabled_;
    //@}
};

}

#endif // __CheckpointList_h_
