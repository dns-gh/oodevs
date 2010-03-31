// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SessionList_h_
#define __SessionList_h_

namespace tools
{
    class GeneralConfig;
}

// =============================================================================
/** @class  SessionList
    @brief  SessionList
*/
// Created: SBO 2009-12-13
// =============================================================================
class SessionList : public QVBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             SessionList( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~SessionList();
    //@}

    //! @name Operations
    //@{
    void Update( const QString& exercice );
    //@}

signals:
    //! @name Signals
    //@{
    void Select( const QString& session );
    //@}

private slots:
    //! @name Slots
    //@{
    void SelectSession( int index );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SessionList( const SessionList& );            //!< Copy constructor
    SessionList& operator=( const SessionList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadComments( const QString& session );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QListBox* list_;
    QTextEdit* comments_;
    QString exercise_;
    //@}
};

#endif // __SessionList_h_
