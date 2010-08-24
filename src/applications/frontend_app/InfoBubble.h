// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __InfoBubble_h_
#define __InfoBubble_h_

#include <qhgroupbox.h>

class QLabel;

// =============================================================================
/** @class  InfoBubble
    @brief  InfoBubble
*/
// Created: SBO 2007-10-05
// =============================================================================
class InfoBubble : public QHGroupBox
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit InfoBubble( QWidget* parent );
    virtual ~InfoBubble();
    //@}

    //! @name Operations
    //@{
    void ShowError  ( const QString& message );
    void ShowWarning( const QString& message );
    void ShowInfo   ( const QString& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfoBubble( const InfoBubble& );            //!< Copy constructor
    InfoBubble& operator=( const InfoBubble& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    QLabel* icon_;
    QLabel* message_;
    //@}
};

#endif // __InfoBubble_h_
