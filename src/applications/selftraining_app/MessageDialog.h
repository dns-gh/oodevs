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

#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  MessageDialog
    @brief  MessageDialog
*/
// Created: RDS 2008-08-20
// =============================================================================
class MessageDialog : public QDialog
                    , private boost::noncopyable
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
    //! @name Helpers
    //@{
    QSize sizeHint() const;
    void CreateButton( QWidget* parent, int );
    //@}
};

#endif // __MessageDialog_h_
