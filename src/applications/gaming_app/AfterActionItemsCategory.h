// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionItemsCategory_h_
#define __AfterActionItemsCategory_h_

#include <qiconview.h>
class AfterActionModel;
class AfterActionItem;

// =============================================================================
/** @class  AfterActionItemsCategory
    @brief  AfterActionItemsCategory
*/
// Created: AGE 2007-09-17
// =============================================================================
class AfterActionItemsCategory : public QIconView
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionItemsCategory( QWidget* parent, const AfterActionModel& model, const std::string& category );
    virtual ~AfterActionItemsCategory();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterActionItemsCategory( const AfterActionItemsCategory& );            //!< Copy constructor
    AfterActionItemsCategory& operator=( const AfterActionItemsCategory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddItem( const AfterActionItem& item );
    virtual QDragObject* dragObject();
    //@}

private:
    //! @name Member data
    //@{
    std::map< int, const AfterActionItem* > items_;
    //@}
};

#endif // __AfterActionItemsCategory_h_
