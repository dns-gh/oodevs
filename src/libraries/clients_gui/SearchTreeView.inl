// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#include "SubObjectName.h"

namespace gui
{
    // =============================================================================
    // SearchTreeView_ABC
    // =============================================================================

    // -----------------------------------------------------------------------------
    // Name: SearchTreeView_ABC::AddEnumFilter
    // Created: ABR 2012-06-19
    // -----------------------------------------------------------------------------
    template< typename EnumType >
    void SearchTreeView_ABC::AddEnumFilter( const QString& text, EnumType maxValue, CheckComboBox::T_Extractor extractor,
                                            const std::string& (*converter)( EnumType, ENT_Tr::E_Conversion ), const QString& noneText /* = "" */ )
    {
        // Label
        QLabel* label = new QLabel( text );
        // Combo
        CheckComboBox* combo = new CheckComboBox( this );
        combo->SetExtractor( extractor );
        combo->SetAllText( tr( "All" ) );
        if( !noneText.isEmpty() )
            combo->SetNoneText( noneText );
        combo->FillFromEnum< EnumType >( maxValue, converter );
        // Connection
        connect( combo, SIGNAL( CheckedItemsChanged( const QStringList& ) ), signalMapper_, SLOT( map() ) );
        signalMapper_->setMapping( combo, static_cast< QWidget* >( combo ) );
        // Layout
        filtersLayout_->addWidget( label );
        filtersLayout_->addWidget( combo );
        filtersWidget_->setVisible( true );
        combos_.push_back( combo );
    }

    // -----------------------------------------------------------------------------
    // Name: SearchTreeView_ABC::AddResolverFilter
    // Created: ABR 2012-06-19
    // -----------------------------------------------------------------------------
    template< typename KernelType, typename Identifier >
    void SearchTreeView_ABC::AddResolverFilter( const QString& text, const tools::Resolver_ABC< KernelType, Identifier >& resolver,
                                                CheckComboBox::T_Extractor extractor, const std::string& ( KernelType::* converter )() const, const QString& noneText /* = "" */ )
    {
        // Label
        SubObjectName subObject( "ResolverFilter" + text );
        QLabel* label = new QLabel( text );
        // Combo
        CheckComboBox* combo = new CheckComboBox( "combo", this );
        combo->SetExtractor( extractor );
        combo->SetAllText( tr( "All" ) );
        if( !noneText.isEmpty() )
            combo->SetNoneText( noneText );
        combo->FillFromResolver< KernelType, Identifier >( resolver, converter );
        // Connection
        connect( combo, SIGNAL( CheckedItemsChanged( const QStringList& ) ), signalMapper_, SLOT( map() ) );
        signalMapper_->setMapping( combo, static_cast< QWidget* >( combo ) );
        // Layout
        filtersLayout_->addWidget( label );
        filtersLayout_->addWidget( combo );
        filtersWidget_->setVisible( true );
        combos_.push_back( combo );
    }

    // -----------------------------------------------------------------------------
    // Name: SearchTreeView_ABC::AddNumericFilter
    // Created: ABR 2012-06-20
    // -----------------------------------------------------------------------------
    template< typename NumericType, typename SpinBox >
    void SearchTreeView_ABC::AddNumericFilter( const QString& text, typename NumericLimitsEditor< NumericType, SpinBox >::T_Extractor extractor, NumericType min /* = 0 */, NumericType max /* = 0 */ )
    {
        // Label
        QLabel* label = new QLabel( text );
        // Combo
        NumericLimitsEditor< NumericType, SpinBox >* limits = new NumericLimitsEditor< NumericType, SpinBox >( this );
        if( min != max )
            limits->SetMinMax( min, max );
        limits->SetExtractor( extractor );
        // Connection
        connect( limits, SIGNAL( ValueChanged() ), signalMapper_, SLOT( map() ) );
        signalMapper_->setMapping( limits, static_cast< QWidget* >( limits ) );
        // Layout
        filtersLayout_->addWidget( label );
        filtersLayout_->addWidget( limits );
        filtersWidget_->setVisible( true );
        numericLimits_.push_back( limits );
    }

    // =============================================================================
    // SearchTreeView
    // =============================================================================

    // -----------------------------------------------------------------------------
    // Name: SearchTreeView constructor
    // Created: ABR 2012-03-27
    // -----------------------------------------------------------------------------
    template< typename BaseTreeView >
    SearchTreeView< BaseTreeView >::SearchTreeView( const QString& objectName, QWidget* parent )
        : SearchTreeView_ABC( parent )
    {
        treeView_ = new BaseTreeView( objectName, this );
        CreateGUI( objectName );
    }

    // -----------------------------------------------------------------------------
    // Name: SearchTreeView constructor
    // Created: ABR 2012-03-27
    // -----------------------------------------------------------------------------
    template< typename BaseTreeView >
    template< typename _1 >
    SearchTreeView< BaseTreeView >::SearchTreeView( const QString& objectName, QWidget* parent, _1& first )
        : SearchTreeView_ABC( parent )
    {
        treeView_ = new BaseTreeView( objectName, first, this );
        CreateGUI( objectName );
    }

    // -----------------------------------------------------------------------------
    // Name: SearchTreeView constructor
    // Created: ABR 2012-03-27
    // -----------------------------------------------------------------------------
    template< typename BaseTreeView >
    template< typename _1, typename _2 >
    SearchTreeView< BaseTreeView >::SearchTreeView( const QString& objectName, QWidget* parent, _1& first, _2& second )
        : SearchTreeView_ABC( parent )
    {
        treeView_ = new BaseTreeView( objectName, first, second, this );
        CreateGUI( objectName );
    }

    // -----------------------------------------------------------------------------
    // Name: SearchTreeView constructor
    // Created: ABR 2012-03-27
    // -----------------------------------------------------------------------------
    template< typename BaseTreeView >
    template< typename _1, typename _2, typename _3 >
    SearchTreeView< BaseTreeView >::SearchTreeView( const QString& objectName, QWidget* parent, _1& first, _2& second, _3& third )
        : SearchTreeView_ABC( parent )
    {
        treeView_ = new BaseTreeView( objectName, first, second, third, this );
        CreateGUI( objectName );
    }

    // -----------------------------------------------------------------------------
    // Name: SearchTreeView constructor
    // Created: ABR 2012-03-27
    // -----------------------------------------------------------------------------
    template< typename BaseTreeView >
    template< typename _1, typename _2, typename _3, typename _4 >
    SearchTreeView< BaseTreeView >::SearchTreeView( const QString& objectName, QWidget* parent, _1& first, _2& second, _3& third, _4& fourth )
        : SearchTreeView_ABC( parent )
    {
        treeView_ = new BaseTreeView( objectName, first, second, third, fourth, this );
        CreateGUI( objectName );
    }

    // -----------------------------------------------------------------------------
    // Name: SearchTreeView constructor
    // Created: ABR 2012-03-27
    // -----------------------------------------------------------------------------
    template< typename BaseTreeView >
    template< typename _1, typename _2, typename _3, typename _4, typename _5 >
    SearchTreeView< BaseTreeView >::SearchTreeView( const QString& objectName, QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth )
        : SearchTreeView_ABC( parent )
    {
        treeView_ = new BaseTreeView( objectName, first, second, third, fourth, fifth, this );
        CreateGUI( objectName );
    }

    // -----------------------------------------------------------------------------
    // Name: SearchTreeView constructor
    // Created: ABR 2012-03-27
    // -----------------------------------------------------------------------------
    template< typename BaseTreeView >
    template< typename _1, typename _2, typename _3, typename _4, typename _5, typename _6 >
    SearchTreeView< BaseTreeView >::SearchTreeView( const QString& objectName, QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth, _6& sixth )
        : SearchTreeView_ABC( parent )
    {
        treeView_ = new BaseTreeView( objectName, first, second, third, fourth, fifth, sixth, this );
        CreateGUI( objectName );
    }

    // -----------------------------------------------------------------------------
    // Name: SearchTreeView constructor
    // Created: ABR 2012-03-28
    // -----------------------------------------------------------------------------
    template< typename BaseTreeView >
    template< typename _1, typename _2, typename _3, typename _4, typename _5, typename _6, typename _7 >
    SearchTreeView< BaseTreeView >::SearchTreeView( const QString& objectName, QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth, _6& sixth, _7& seventh )
        : SearchTreeView_ABC( parent )
    {
        treeView_ = new BaseTreeView( objectName, first, second, third, fourth, fifth, sixth, seventh, this );
        CreateGUI( objectName );
    }

    // -----------------------------------------------------------------------------
    // Name: SearchTreeView destructor
    // Created: ABR 2012-03-27
    // -----------------------------------------------------------------------------
    template< typename BaseTreeView >
    SearchTreeView< BaseTreeView >::~SearchTreeView()
    {
        // NOTHING
    }

    // -----------------------------------------------------------------------------
    // Name: SearchTreeView::GetTreeView
    // Created: ABR 2012-03-27
    // -----------------------------------------------------------------------------
    template< typename BaseTreeView >
    inline
    BaseTreeView* SearchTreeView< BaseTreeView >::GetTreeView() const
    {
        return static_cast< BaseTreeView* >( treeView_ );
    }

    // -----------------------------------------------------------------------------
    // Name: SearchTreeView::GetRichTreeView
    // Created: ABR 2012-03-29
    // -----------------------------------------------------------------------------
    template< typename BaseTreeView >
    inline
    RichTreeView* SearchTreeView< BaseTreeView >::GetRichTreeView() const
    {
        return treeView_;
    }
}