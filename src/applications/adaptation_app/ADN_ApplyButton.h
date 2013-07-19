// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ApplyButton_h_
#define __ADN_ApplyButton_h_

// =============================================================================
/** @class  ADN_ApplyButton
    @brief  ADN_ApplyButton
*/
// Created: ABR 2013-07-16
// =============================================================================
class ADN_ApplyButton : public QPushButton
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_ApplyButton( QWidget* parent = 0 );
    virtual ~ADN_ApplyButton();
    //@}

    //! @name Operations
    //@{
    void Connect( QWidget* linkedWidget );
    //@}

signals:
    //! @name Signals
    //@{
    void TypeChanged( int );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnClicked( bool );
    void OnTypeChanged( int newType );
    void OnLanguageChanged( const std::string& language );
    //@}

};

#endif // __ADN_ApplyButton_h_
