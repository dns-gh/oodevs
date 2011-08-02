// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MessageDialog_h_
#define __MessageDialog_h_

// =============================================================================
/** @class  MessageDialog
    @brief  MessageDialog
*/
// Created: RDS 2008-08-20
// =============================================================================
class MessageDialog : public QDialog
{

    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             MessageDialog( QWidget* parent, const QString& title, const QString& message, int button1, int button2 );
    virtual ~MessageDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnYes();
    void OnNo();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MessageDialog( const MessageDialog& );            //!< Copy constructor
    MessageDialog& operator=( const MessageDialog& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    QSize sizeHint() const;
    QPushButton* CreateButton( QWidget* parent, int );
    //@}
};

#endif // __MessageDialog_h_
