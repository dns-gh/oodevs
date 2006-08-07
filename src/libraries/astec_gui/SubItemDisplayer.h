// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __SubItemDisplayer_h_
#define __SubItemDisplayer_h_

#include "BaseDisplayer.h"
class QListViewItem;
class ItemFactory_ABC;

// =============================================================================
/** @class  SubItemDisplayer
    @brief  SubItemDisplayer
*/
// Created: AGE 2006-02-28
// =============================================================================
class SubItemDisplayer : public BaseDisplayer
{

public:
    //! @name Constructors/Destructor
    //@{
             SubItemDisplayer( const char* name, ItemFactory_ABC& factory );
    virtual ~SubItemDisplayer();
    //@}

    //! @name Operations
    //@{
    virtual void Hide();
    SubItemDisplayer& AddChild( const char* child );

    Displayer_ABC& operator()( QListViewItem* item );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SubItemDisplayer( const SubItemDisplayer& );            //!< Copy constructor
    SubItemDisplayer& operator=( const SubItemDisplayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual Displayer_ABC& SubItem( const char* name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    QListViewItem* FindChild( const char* name ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::string > T_Children;
    //@}

private:
    //! @name Member data
    //@{
    ItemFactory_ABC& factory_;
    const char* name_;
    QListViewItem* parent_;
    QListViewItem* current_;
    QString message_;
    T_Children children_;
    //@}
};

#endif // __SubItemDisplayer_h_
