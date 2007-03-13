// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionsListView_h_
#define __ActionsListView_h_

// =============================================================================
/** @class  ActionsListView
    @brief  ActionsListView
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionsListView : public QListView
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionsListView( QWidget* parent );
    virtual ~ActionsListView();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionsListView( const ActionsListView& );            //!< Copy constructor
    ActionsListView& operator=( const ActionsListView& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __ActionsListView_h_
