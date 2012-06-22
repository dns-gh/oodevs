// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_SearchListView_h_
#define __gui_SearchListView_h_

#include <boost/noncopyable.hpp>
#include "SearchLineEdit.h"
#include "resources.h"
#include "CheckComboBox.h"
#include "NumericLimitsEditor.h"
#include "ExpandableGroupBox.h"

namespace gui
{
    class RichListView;

// =============================================================================
/** @class  SearchListView
    @brief  SearchListView
*/
// Created: ABR 2012-03-27
// =============================================================================
class SearchListView_ABC : public QWidget
                         , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit SearchListView_ABC( QWidget* parent );
    virtual ~SearchListView_ABC();
    //@}

    //! @name Abstract operation
    //@{
    virtual RichListView* GetRichListView() const = 0;
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
                        const std::string& (*converter)( EnumType, ENT_Tr_ABC::E_Conversion ), const QString& noneText = "" );

    template< typename KernelType, typename Identifier >
    void AddResolverFilter( const QString& text, const tools::Resolver_ABC< KernelType, Identifier >& resolver,
                            CheckComboBox::T_Extractor extractor, const std::string& ( KernelType::* converter )() const, const QString& noneText = "" );

    template< typename NumericType, typename SpinBox >
    void AddNumericFilter( const QString& text, typename NumericLimitsEditor< NumericType, SpinBox >::T_Extractor extractor, NumericType min, NumericType max );
    //@}

protected:
    //! @name Helpers
    //@{
    void CreateGUI();
    //@}

private:
    //! @name Helpers
    //@{
    bool ApplyAllFilter( ValuedListItem* item ) const;
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
    QSignalMapper*                          signalMapper_;
    ExpandableGroupBox*                     filtersWidget_;
    QPushButton*                            clearButton_;
    QWidget*                                filtersContainer_;
    QGridLayout*                            filtersLayout_;
    int                                     currentFilterRow_;
    std::vector< CheckComboBox* >           combos_;
    std::vector< NumericLimitsEditor_ABC* > numericLimits_;
    RichListView*                           listView_;
    //@}
};

// =============================================================================
/** @class  SearchListView
    @brief  SearchListView
*/
// Created: ABR 2012-03-27
// =============================================================================
template< typename BaseListView >
class SearchListView : public SearchListView_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SearchListView( QWidget* parent );
             template< typename _1 >
             SearchListView( QWidget* parent, _1& first );
             template< typename _1, typename _2 >
             SearchListView( QWidget* parent, _1& first, _2& second );
             template< typename _1, typename _2, typename _3 >
             SearchListView( QWidget* parent, _1& first, _2& second, _3& third );
             template< typename _1, typename _2, typename _3, typename _4 >
             SearchListView( QWidget* parent, _1& first, _2& second, _3& third, _4& fourth );
             template< typename _1, typename _2, typename _3, typename _4, typename _5 >
             SearchListView( QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth );
             template< typename _1, typename _2, typename _3, typename _4, typename _5, typename _6 >
             SearchListView( QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth, _6& sixth );
             template< typename _1, typename _2, typename _3, typename _4, typename _5, typename _6, typename _7 >
             SearchListView( QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth, _6& sixth, _7& seventh );
    virtual ~SearchListView();
    //@}

    //! @name Operations
    //@{
    BaseListView* GetListView() const;
    virtual RichListView* GetRichListView() const;
    //@}
};

} //! namespace gui

#include "SearchListView.inl"

#endif // __gui_SearchListView_h_
