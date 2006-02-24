// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ListItemDisplayer_h_
#define __ListItemDisplayer_h_

#include "BaseDisplayer.h"
class QListViewItem;

// =============================================================================
/** @class  ListItemDisplayer
    @brief  ListItemDisplayer
*/
// Created: AGE 2006-02-22
// =============================================================================
class ListItemDisplayer : public BaseDisplayer
{

public:
    //! @name Constructors/Destructor
    //@{
             ListItemDisplayer();
    virtual ~ListItemDisplayer();
    //@}

    //! @name Operations
    //@{
    virtual void Hide();
    ListItemDisplayer& AddColumn( const char* column );

    Displayer_ABC& operator()( QListViewItem* item );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ListItemDisplayer( const ListItemDisplayer& );            //!< Copy constructor
    ListItemDisplayer& operator=( const ListItemDisplayer& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::string > T_Columns;
    //@}

    //! @name Helpers
    //@{
    virtual Displayer_ABC& SubItem( const char* name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    int FindColumn( const char* name ) const;
    //@}

private:
    //! @name Member data
    //@{
    QListViewItem* item_;
    int column_;
    QString message_;
    T_Columns columns_;
    //@}
};

#endif // __ListItemDisplayer_h_
