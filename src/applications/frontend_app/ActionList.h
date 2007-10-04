// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionList_h_
#define __ActionList_h_

#include <qtoolbox.h>

class QAction;
class ActionCategory;

// =============================================================================
/** @class  ActionList
    @brief  ActionList
*/
// Created: SBO 2007-10-04
// =============================================================================
class ActionList : public QToolBox
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionList( QWidget* parent );
    virtual ~ActionList();
    //@}

    //! @name Operations
    //@{
    void AddAction( const QString& category, QAction& action );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionList( const ActionList& );            //!< Copy constructor
    ActionList& operator=( const ActionList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

    //! @name Types
    //@{
    typedef std::map< QString, ActionCategory* > T_Categories;
    //@}

private:
    //! @name Member data
    //@{
    T_Categories categories_;
    //@}
};

#endif // __ActionList_h_
