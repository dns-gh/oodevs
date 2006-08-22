// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AttributeViewCell_h_
#define __AttributeViewCell_h_

#include <qtable.h>

class QListBox;

namespace kernel
{
    class DataDictionary;
}

namespace gui
{

// =============================================================================
/** @class  AttributeViewCell
    @brief  AttributeViewCell
*/
// Created: AGE 2006-06-22
// =============================================================================
class AttributeViewCell : public QTableItem
{

public:
    //! @name Constructors/Destructor
    //@{
             AttributeViewCell( QTable* parent, const kernel::DataDictionary& dictionary );
    virtual ~AttributeViewCell();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AttributeViewCell( const AttributeViewCell& );            //!< Copy constructor
    AttributeViewCell& operator=( const AttributeViewCell& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual QWidget* createEditor() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::DataDictionary& dictionary_;
    //@}
};

}

#endif // __AttributeViewCell_h_
