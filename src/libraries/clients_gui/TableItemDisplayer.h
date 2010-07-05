// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TableItemDisplayer_h_
#define __TableItemDisplayer_h_

#include "NoLinkDisplayer.h"

class QTableItem;

namespace gui
{

// =============================================================================
/** @class  TableItemDisplayer
    @brief  TableItemDisplayer
*/
// Created: SBO 2006-10-18
// =============================================================================
class TableItemDisplayer : public NoLinkDisplayer
{

public:
    //! @name Constructors/Destructor
    //@{
             TableItemDisplayer();
    virtual ~TableItemDisplayer();
    //@}

    //! @name Operations
    //@{
    virtual void Hide() ;
    void SetItem( QTableItem* item );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TableItemDisplayer( const TableItemDisplayer& );            //!< Copy constructor
    TableItemDisplayer& operator=( const TableItemDisplayer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual kernel::Displayer_ABC& SubItem( const QString& name ) ;
    virtual void StartDisplay() ;
    virtual void DisplayFormatted( const QString& formatted ) ;
    virtual void EndDisplay() ;
    //@}

private:
    //! @name Member data
    //@{
    QString message_;
    QTableItem* item_;
    //@}
};

}

#endif // __TableItemDisplayer_h_
