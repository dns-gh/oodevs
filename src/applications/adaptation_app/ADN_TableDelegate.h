// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_TableDelegate_h_
#define __ADN_TableDelegate_h_

#include "clients_gui/CommonDelegate.h"

// =============================================================================
/** @class  ADN_TableDelegate
    @brief  ADN_TableDelegate
*/
// Created: ABR 2012-10-22
// =============================================================================
class ADN_TableDelegate : public gui::CommonDelegate
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_TableDelegate( QObject* parent = 0 );
    virtual ~ADN_TableDelegate();
    //@}

public:
    //! @name Public Types
    //@{
    typedef std::map< unsigned int, std::pair< double, double > > T_Colors;
    //@}

    //! @name Colors
    //@{
    unsigned int AddColorOnRow( int row, double min, double max );
    unsigned int AddColorOnColumn( int column, double min, double max );
    unsigned int AddColor( int fromRow, int toRow, int fromCol, int toCol, double min, double max );

    const std::pair< double, double >* GetColorType( int row, int col ) const;
    //@}

    //! @name Delay
    //@{
    unsigned int AddDelayEditorOnRow( int row );
    unsigned int AddDelayEditorOnColumn( int column );
    unsigned int AddDelayEditor( int fromRow, int toRow, int fromCol, int toCol );
    //@}

protected:
    //! @name Operations
    //@{
    virtual QWidget *createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    virtual void setEditorData( QWidget* editor, const QModelIndex& index ) const;
    virtual void setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const;
    //@}

private:
    //! @name Member data
    //@{
    T_Positions colorPositions_;
    T_Colors colors_;
    T_SimpleWidget delayEditors_;
    //@}
};

#endif // __ADN_TableDelegate_h_
