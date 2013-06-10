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

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include "ENT/ENT_Tr_ABC.h"
#include "tools/Resolver_ABC.h"
#include "RichLineEdit.h"
#include "RichWidget.h"

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
    void checkStateChanged();
    //@}
};

// =============================================================================
/** @class  CheckComboBox
    @brief  CheckComboBox
*/
// Created: ABR 2012-06-18
// =============================================================================
class CheckComboBox : public RichWidget< QComboBox >
{
    Q_OBJECT

public:
    //! @name Types
    //@{
    typedef boost::function< std::string ( const kernel::Entity_ABC&, bool& valid, bool& empty ) > T_Extractor;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit CheckComboBox( const QString& objectName, QWidget* parent = 0 );
    virtual ~CheckComboBox();
    //@}

    //! @name Accessors
    //@{
    QString AllText() const;
    void SetAllText( const QString& text );

    QString NoneText() const;
    void SetNoneText( const QString& noneText );

    QString Separator() const;
    void SetSeparator( const QString& separator );

    Qt::CheckState ItemCheckState( int index ) const;
    void SetItemCheckState( int index, Qt::CheckState state );

    QStringList CheckedItems() const;
    //@}

    //! @name Operations
    //@{
    void mousePressEvent( QMouseEvent* event );
    //@}

    //! @name Filling operations
    //@{
    template< typename EnumType >
    void FillFromEnum( EnumType nbrMax, const std::string& (*converter)( EnumType, ENT_Tr_ABC::E_Conversion ) );
    template< typename KernelType, typename Identifier >
    void FillFromResolver( const tools::Resolver_ABC< KernelType, Identifier >& resolver, const std::string& (KernelType::* converter )() const );
    void FillFromStringList( const std::vector< std::string >& vector );
    void FillFromQStringList( const QStringList& stringList );
    //@}

    //! @name Filters
    //@{
    bool ApplyFilter( QStandardItem& item, StandardModel& model ) const;
    void SetItemsCheckState( Qt::CheckState state );
    void SetExtractor( T_Extractor extractor );
    //@}

public slots:
    //! @name Slots
    //@{
    void SetCheckedItems( const QStringList& items );
    void ToggleCheckState( int index );
    void UpdateCheckedItems();
    //@}

signals:
    //! @name Signals
    //@{
    void CheckedItemsChanged( const QStringList& items );
    //@}

private:
    //! @name Helpers
    //@{
    void AddItem( int& row, const QString text );
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
    RichLineEdit*  lineEdit_;
    QString     allText_;
    QString     noneText_;
    QString     separator_;
    bool        showing_;
    T_Extractor extractor_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: CheckComboBox::FillFromEnum
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
template< typename EnumType >
void CheckComboBox::FillFromEnum( EnumType nbrMax, const std::string& (*converter)( EnumType, ENT_Tr_ABC::E_Conversion ) )
{
    int row = 0;
    //AddItem( row, allText_ );
    AddItem( row, noneText_ );
    for( int i = 0; i < static_cast< int >( nbrMax ); ++i )
        AddItem( row, converter( static_cast< EnumType >( i ), ENT_Tr_ABC::eToTr ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::FillFromResolver
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
template< typename KernelType, typename Identifier >
void CheckComboBox::FillFromResolver( const tools::Resolver_ABC< KernelType, Identifier >& resolver, const std::string& (KernelType::* converter )() const )
{
    int row = 0;
    //AddItem( row, allText_ );
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
