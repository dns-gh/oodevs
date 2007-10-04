// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionCategory_h_
#define __ActionCategory_h_

#include <qvbuttongroup.h>

class QAction;
class QToolBox;

// =============================================================================
/** @class  ActionCategory
    @brief  ActionCategory
*/
// Created: SBO 2007-10-04
// =============================================================================
class ActionCategory : public QVButtonGroup
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionCategory( QToolBox* parent );
    virtual ~ActionCategory();
    //@}

    //! @name Operations
    //@{
    void AddAction( QAction& action );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnItemToggled( bool toggled );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionCategory( const ActionCategory& );            //!< Copy constructor
    ActionCategory& operator=( const ActionCategory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    QToolBox* parent_;
    //@}
};

#endif // __ActionCategory_h_
