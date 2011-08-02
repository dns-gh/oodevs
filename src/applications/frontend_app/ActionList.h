// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionList_h_
#define __ActionList_h_

#include <QtGui/qtoolbox.h>

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
    void AddAction( const QString& category, Q3Action& action );
    void SelectCategory( const ActionCategory& category );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionList( const ActionList& );            //!< Copy constructor
    ActionList& operator=( const ActionList& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< QString, ActionCategory* > T_Categories;
    typedef T_Categories::const_iterator       CIT_Categories;
    //@}

private:
    //! @name Member data
    //@{
    T_Categories categories_;
    //@}
};

#endif // __ActionList_h_
