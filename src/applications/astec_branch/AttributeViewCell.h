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

class DataDictionary;
class QListBox;

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
             AttributeViewCell( QTable* parent, const DataDictionary& dictionary );
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
    const DataDictionary& dictionary_;
    //@}
};

#endif // __AttributeViewCell_h_
