// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_CheckComboBox_h_
#define __gui_CheckComboBox_h_

#include "ENT/ENT_Tr.h"
#include "Filter_ABC.h"
#include "RichLineEdit.h"
#include "RichWidget.h"
#include <tools/Resolver_ABC.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{
    class StandardModel;

// =============================================================================
/** @class  CheckComboModel
    @brief  CheckComboModel
*/
// Created: ABR 2012-06-18
// =============================================================================
class CheckComboModel : public QStandardItemModel
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit CheckComboModel( QObject* parent = 0 );
    virtual ~CheckComboModel();
    //@}

    //! @name Operations
    //@{
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    //@}

signals:
    //! @name Signals
    //@{
    void checkStateChanged( bool checkAll );
    //@}
};

// =============================================================================
/** @class  CheckComboBox
    @brief  CheckComboBox
*/
// Created: ABR 2012-06-18
// =============================================================================
class CheckComboBox : public RichWidget< QComboBox >
                    , public Filter_ABC
{
    Q_OBJECT

public:
    //! @name Types
    //@{
    typedef std::function< std::string ( const QStandardItem& item,
                                         bool& valid,
                                         bool& empty ) > T_Extractor;
    typedef std::function< void ( CheckComboBox& ) > T_Filler;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit CheckComboBox( const QString& objectName,
                            QWidget* parent,
                            bool useNone,
                            bool useCheckAll,
                            const T_Extractor& extractor,
                            const T_Filler& filler );
    virtual ~CheckComboBox();
    //@}

    //! @name Filter_ABC implementation
    //@{
    virtual bool Apply( QStandardItem& item ) const;
    virtual void Clear();
    //@}

    //! @name Operations
    //@{
    QWidget* CreateListView( QWidget* parent );

    template< typename EnumType >
    void FillFromEnum( int nbrMax, const std::function< const std::string& ( EnumType, ENT_Tr::E_Conversion ) >& converter );
    template< typename KernelType, typename Identifier >
    void FillFromResolver( const tools::Resolver_ABC< KernelType, Identifier >& resolver, const std::string& (KernelType::* converter )() const );
    void FillFromStringList( const std::vector< std::string >& vector );
    void FillFromQStringList( const QStringList& stringList );
    //@}

public slots:
    //! @name Slots
    //@{
    void UpdateCheckedItems( bool checkAll = false );
    //@}

signals:
    //! @name Signals
    //@{
    void CheckedItemsChanged( const QStringList& items );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void mousePressEvent( QMouseEvent* event );
    void AddItem( int& row, const QString text );
    Qt::CheckState ItemCheckState( int index ) const;
    void SetItemCheckState( int index, Qt::CheckState state );
    QStringList CheckedItems() const;
    void SetItemsCheckState( Qt::CheckState state );
    //@}

    //! @name Types
    //@{
    class CheckComboLineEdit : public RichLineEdit
    {
    public:
        //! @name Constructors/Destructor
        //@{
        explicit CheckComboLineEdit( const QString& objectName, QComboBox* parent = 0 );
        virtual ~CheckComboLineEdit();
        //@}

        //! @name Operations
        //@{
        void mousePressEvent( QMouseEvent* event );
        //@}

    private:
        //! @name Member data
        //@{
        QComboBox* parent_;
        //@}
    };
    //@}

public:
    //! @name Member data
    //@{
    QStandardItemModel* dataModel_;
    QSortFilterProxyModel proxyModel_;
    RichLineEdit* lineEdit_;
    const QString checkAllText_;
    const QString allText_;
    const QString noneText_;
    const QString separator_;
    const T_Extractor extractor_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: CheckComboBox::FillFromEnum
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
template< typename EnumType >
void CheckComboBox::FillFromEnum( int nbrMax, const std::function< const std::string& ( EnumType, ENT_Tr::E_Conversion ) >& converter )
{
    int row = 0;
    AddItem( row, checkAllText_ );
    AddItem( row, noneText_ );
    for( int i = 0; i < nbrMax; ++i )
        AddItem( row, converter( static_cast< EnumType >( i ), ENT_Tr::eToTr ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::FillFromResolver
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
template< typename KernelType, typename Identifier >
void CheckComboBox::FillFromResolver( const tools::Resolver_ABC< KernelType, Identifier >& resolver, const std::string& (KernelType::* converter )() const )
{
    int row = 0;
    AddItem( row, checkAllText_ );
    AddItem( row, noneText_ );
    tools::Iterator< const KernelType& > it = resolver.CreateIterator();
    while( it.HasMoreElements() )
    {
        const KernelType& element = it.NextElement();
        AddItem( row, (element.*converter)().c_str() );
    }
}

} //! namespace gui

#endif // __gui_CheckComboBox_h_
