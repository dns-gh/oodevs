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

#include <Qt3Support/q3buttongroup.h>

class QAction;
class ActionList;

// =============================================================================
/** @class  ActionCategory
    @brief  ActionCategory
*/
// Created: SBO 2007-10-04
// =============================================================================
class ActionCategory : public Q3VButtonGroup
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionCategory( ActionList* parent );
    virtual ~ActionCategory();
    //@}

    //! @name Operations
    //@{
    void AddAction( Q3Action& action );
    void ClearSelection();
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

private:
    //! @name Member data
    //@{
    ActionList* parent_;
    //@}
};

#endif // __ActionCategory_h_
