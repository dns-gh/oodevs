// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_SearchTreeView_h_
#define __gui_SearchTreeView_h_

#include "SearchLineEdit.h"
#include "resources.h"
#include "CheckComboBox.h"
#include "NumericLimitsEditor.h"
#include "ExpandableGroupBox.h"

namespace gui
{
    class RichTreeView;

// =============================================================================
/** @class  SearchTreeView
    @brief  SearchTreeView
*/
// Created: ABR 2012-03-27
// =============================================================================
class SearchTreeView_ABC : public QWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit SearchTreeView_ABC( QWidget* parent );
    virtual ~SearchTreeView_ABC();
    //@}

    //! @name Abstract operation
    //@{
    virtual RichTreeView* GetRichTreeView() const = 0;
    //@}

    //! @name Operations
    //@{
    void Load();
    void Purge();
    //@}

    //! @name Filters operations
    //@{
    template< typename EnumType >
    void AddEnumFilter( const QString& text, EnumType maxValue, CheckComboBox::T_Extractor extractor,
                        const std::string& (*converter)( EnumType, ENT_Tr::E_Conversion ), const QString& noneText = "" );

    template< typename KernelType, typename Identifier >
    void AddResolverFilter( const QString& text, const tools::Resolver_ABC< KernelType, Identifier >& resolver,
                            CheckComboBox::T_Extractor extractor, const std::string& ( KernelType::* converter )() const, const QString& noneText = "" );

    template< typename NumericType, typename SpinBox >
    void AddNumericFilter( const QString& text, typename NumericLimitsEditor< NumericType, SpinBox >::T_Extractor extractor, NumericType min, NumericType max );
    //@}

protected:
    //! @name Helpers
    //@{
    void CreateGUI( const QString& objectName );
    //@}

private:
    //! @name Helpers
    //@{
    bool ApplyAllFilter( QStandardItem& item, StandardModel& model ) const;
    //@}

protected slots:
    //! @name Slots
    //@{
    void OnFiltersChanged( QWidget* );
    void OnClearFilters();
    //@}

protected:
    //! @name Member data
    //@{
    SearchLineEdit*                         searchLine_;
    ExpandableGroupBox*                     filtersWidget_;
    RichTreeView*                           treeView_;
    QSignalMapper*                          signalMapper_;
    RichPushButton*                         clearButton_;
    QWidget*                                filtersContainer_;
    QGridLayout*                            filtersLayout_;
    int                                     currentFilterRow_;
    std::vector< CheckComboBox* >           combos_;
    std::vector< NumericLimitsEditor_ABC* > numericLimits_;
    //@}
};

// =============================================================================
/** @class  SearchTreeView
    @brief  SearchTreeView
*/
// Created: ABR 2012-03-27
// =============================================================================
template< typename TreeView >
class SearchTreeView : public SearchTreeView_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SearchTreeView( const QString& objectName, QWidget* parent );
             template< typename _1 >
             SearchTreeView( const QString& objectName, QWidget* parent, _1& first );
             template< typename _1, typename _2 >
             SearchTreeView( const QString& objectName, QWidget* parent, _1& first, _2& second );
             template< typename _1, typename _2, typename _3 >
             SearchTreeView( const QString& objectName, QWidget* parent, _1& first, _2& second, _3& third );
             template< typename _1, typename _2, typename _3, typename _4 >
             SearchTreeView( const QString& objectName, QWidget* parent, _1& first, _2& second, _3& third, _4& fourth );
             template< typename _1, typename _2, typename _3, typename _4, typename _5 >
             SearchTreeView( const QString& objectName, QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth );
             template< typename _1, typename _2, typename _3, typename _4, typename _5, typename _6 >
             SearchTreeView( const QString& objectName, QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth, _6& sixth );
             template< typename _1, typename _2, typename _3, typename _4, typename _5, typename _6, typename _7 >
             SearchTreeView( const QString& objectName, QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth, _6& sixth, _7& seventh );
    virtual ~SearchTreeView();
    //@}

    //! @name Operations
    //@{
    TreeView* GetTreeView() const;
    virtual RichTreeView* GetRichTreeView() const;
    //@}
};

} //! namespace gui

#include "SearchTreeView.inl"

#endif // __gui_SearchTreeView_h_
