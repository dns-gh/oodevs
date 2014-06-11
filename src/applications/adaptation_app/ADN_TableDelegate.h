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
             ADN_TableDelegate( QTableView* parent = 0 );
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

    //! @name Colors editor
    //@{
    unsigned int AddColorEditOnRow( int row );
    unsigned int AddColorEditOnColumn( int column );
    unsigned int AddColorEdit( int fromRow, int toRow, int fromCol, int toCol );
    //@}

    //! @name Delay
    //@{
    unsigned int AddDelayEditOnRow( int row, unsigned int min = 0u );
    unsigned int AddDelayEditOnColumn( int column, unsigned int min = 0u );
    //@}

    //! @name Time
    //@{
    unsigned int AddTimeEditOnRow( int row );
    unsigned int AddTimeEditOnColumn( int column );
    unsigned int AddTimeEdit( int fromRow, int toRow, int fromCol, int toCol );
    //@}

    //! @name PtrInVector
    //@{
    unsigned int AddComboPtrInVectorOnRow( int row );
    unsigned int AddComboPtrInVectorOnColumn( int column );
    unsigned int AddComboPtrInVector( int fromRow, int toRow, int fromCol, int toCol );
    //@}

    //! @name LocalizedString
    //@{
    unsigned int AddLocalizedLineEditOnRow( int row );
    unsigned int AddLocalizedLineEditOnColumn( int column );
    unsigned int AddLocalizedLineEdit( int fromRow, int toRow, int fromCol, int toCol );
    //@}

    //! @name boldIndexes
    //@{
    void AddBoldRowIndex( int row );
    void AddBoldColumnIndex( int column );
    const std::set< int >& GetBoldRowIndexes() const;
    const std::set< int >& GetBoldColumnIndexes() const;
    void SetGridPen( QPen gridPen );
    //@}

protected:
    //! @name Operations
    //@{
    virtual QWidget *createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    virtual void setEditorData( QWidget* editor, const QModelIndex& index ) const;
    virtual void setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const;
    virtual void paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void drawCheck( QPainter* painter, const QStyleOptionViewItem& option, const QRect& rect, Qt::CheckState state ) const;
    virtual bool editorEvent( QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index );
    //@}

private:
    //! @name Member data
    //@{
    T_Positions colorPositions_;
    T_Colors colors_;
    T_SimpleWidget timeEdits_;
    T_SimpleWidget comboPtrInVectors_;
    T_SimpleWidget colorEdits_;
    T_SimpleWidget localizedEditLines_;
    QPen gridPen_;
    std::set<int> boldGridRowIndexes_;
    std::set<int> boldGridColIndexes_;
    //@}
};

#endif // __ADN_TableDelegate_h_
