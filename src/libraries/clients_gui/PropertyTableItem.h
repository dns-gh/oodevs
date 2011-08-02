// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PropertyTableItem_h_
#define __PropertyTableItem_h_

namespace kernel
{
    class Property_ABC;
    class EditorFactory_ABC;
}

namespace gui
{
    class TableItemDisplayer;

// =============================================================================
/** @class  PropertyTableItem
    @brief  PropertyTableItem
*/
// Created: SBO 2006-10-18
// =============================================================================
class PropertyTableItem : public Q3TableItem
{
public:
    //! @name Constructors/Destructor
    //@{
             PropertyTableItem( Q3Table* table, kernel::Property_ABC& property, TableItemDisplayer& displayer, kernel::EditorFactory_ABC& factory );
    virtual ~PropertyTableItem();
    //@}

    //! @name Operations
    //@{
    virtual void paint( QPainter* p, const QColorGroup& cg, const QRect& cr, bool selected );
    virtual QWidget* createEditor() const;
    virtual void setContentFromEditor( QWidget* w );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PropertyTableItem( const PropertyTableItem& );            //!< Copy constructor
    PropertyTableItem& operator=( const PropertyTableItem& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::EditorFactory_ABC& factory_;
    TableItemDisplayer& displayer_;
    kernel::Property_ABC& property_;
    //@}
};

}

#endif // __PropertyTableItem_h_
