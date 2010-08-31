// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __SpinTableItem_h_
#define __SpinTableItem_h_

// =============================================================================
/** @class  SpinTableItem
    @brief  SpinTableItem
*/
// Created: JSR 2010-08-30
// =============================================================================
class SpinTableItem : public QTableItem
{
public:
    //! @name Constructors/Destructor
    //@{
             SpinTableItem( QTable* table, int minValue, int maxValue, int step = 1 );
    virtual ~SpinTableItem();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* createEditor() const;
    virtual void setContentFromEditor( QWidget* widget );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SpinTableItem( const SpinTableItem& );            //!< Copy constructor
    SpinTableItem& operator=( const SpinTableItem& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    int minValue_;
    int maxValue_;
    int step_;
    //@}
};

#endif // __SpinTableItem_h_
