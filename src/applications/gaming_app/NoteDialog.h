// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __NoteDialog_h_
#define __NoteDialog_h_

#include <qdialog.h>
#include <qtextedit.h>
class Note;
class Publisher_ABC;

// =============================================================================
/** @class  NoteDialog
    @brief  NoteDialog
*/
// Created: HBD 2010-01-26
// =============================================================================
class NoteDialog : public QDialog
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             NoteDialog( QDockWindow* parent, Publisher_ABC &publisher );
    virtual ~NoteDialog();
    //@}

    //! @name Operations
    //@{
    void setParent(unsigned int note);
    void setUpdate(Note& note);
    //@}
    private slots:
        //! @name Slots
        //@{
        void OnAccept();
        void OnReject();
        void OnFileChanged();
        //@}

private:
    //! @name Copy/Assignment
    //@{
    NoteDialog( const NoteDialog& );            //!< Copy constructor
    NoteDialog& operator=( const NoteDialog& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    QLineEdit* textName_;
    QLineEdit* textId_;
    QTextEdit* textDesc_;
    QPushButton* buttonOk_;
    Publisher_ABC &publisher_;
    unsigned int note_;
    bool        update_;
    unsigned int noteId_;

    //@}
};


#endif // __NoteDialog_h_
