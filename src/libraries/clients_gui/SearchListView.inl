// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

namespace gui
{
    // =============================================================================
    // SearchListView_ABC
    // =============================================================================

    // -----------------------------------------------------------------------------
    // Name: SearchListView_ABC::AddEnumFilter
    // Created: ABR 2012-06-19
    // -----------------------------------------------------------------------------
    template< typename EnumType >
    void SearchListView_ABC::AddEnumFilter( const QString& text, EnumType maxValue, CheckComboBox::T_Extractor extractor,
                                            const std::string& (*converter)( EnumType, ENT_Tr_ABC::E_Conversion ), const QString& noneText /* = "" */ )
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
    // Name: SearchListView_ABC::AddResolverFilter
    // Created: ABR 2012-06-19
    // -----------------------------------------------------------------------------
    template< typename KernelType, typename Identifier >
    void SearchListView_ABC::AddResolverFilter( const QString& text, const tools::Resolver_ABC< KernelType, Identifier >& resolver,
                                                CheckComboBox::T_Extractor extractor, const std::string& ( KernelType::* converter )() const, const QString& noneText /* = "" */ )
    {
        // Label
        QLabel* label = new QLabel( text );
        // Combo
        CheckComboBox* combo = new CheckComboBox( this );
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
    // Name: SearchListView_ABC::AddNumericFilter
    // Created: ABR 2012-06-20
    // -----------------------------------------------------------------------------
    template< typename NumericType, typename SpinBox >
    void SearchListView_ABC::AddNumericFilter( const QString& text, typename NumericLimitsEditor< NumericType, SpinBox >::T_Extractor extractor, NumericType min /* = 0 */, NumericType max /* = 0 */ )
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
    // SearchListView
    // =============================================================================

    // -----------------------------------------------------------------------------
    // Name: SearchListView constructor
    // Created: ABR 2012-03-27
    // -----------------------------------------------------------------------------
    template< typename BaseListView >
    SearchListView< BaseListView >::SearchListView( QWidget* parent )
        : SearchListView_ABC( parent )
        , searchLine_( 0 )
        , listView_  ( 0 )
    {
        listView_ = new BaseListView( this );
        CreateGUI();
    }

    // -----------------------------------------------------------------------------
    // Name: SearchListView constructor
    // Created: ABR 2012-03-27
    // -----------------------------------------------------------------------------
    template< typename BaseListView >
    template< typename _1 >
    SearchListView< BaseListView >::SearchListView( QWidget* parent, _1& first )
        : SearchListView_ABC( parent )
        , searchLine_( 0 )
        , listView_  ( 0 )
    {
        listView_ = new BaseListView( this, first );
        CreateGUI();
    }

    // -----------------------------------------------------------------------------
    // Name: SearchListView constructor
    // Created: ABR 2012-03-27
    // -----------------------------------------------------------------------------
    template< typename BaseListView >
    template< typename _1, typename _2 >
    SearchListView< BaseListView >::SearchListView( QWidget* parent, _1& first, _2& second )
        : SearchListView_ABC( parent )
        , searchLine_( 0 )
        , listView_  ( 0 )
    {
        listView_ = new BaseListView( this, first, second );
        CreateGUI();
    }

    // -----------------------------------------------------------------------------
    // Name: SearchListView constructor
    // Created: ABR 2012-03-27
    // -----------------------------------------------------------------------------
    template< typename BaseListView >
    template< typename _1, typename _2, typename _3 >
    SearchListView< BaseListView >::SearchListView( QWidget* parent, _1& first, _2& second, _3& third )
        : SearchListView_ABC( parent )
        , searchLine_( 0 )
        , listView_  ( 0 )
    {
        listView_ = new BaseListView( this, first, second, third );
        CreateGUI();
    }

    // -----------------------------------------------------------------------------
    // Name: SearchListView constructor
    // Created: ABR 2012-03-27
    // -----------------------------------------------------------------------------
    template< typename BaseListView >
    template< typename _1, typename _2, typename _3, typename _4 >
    SearchListView< BaseListView >::SearchListView( QWidget* parent, _1& first, _2& second, _3& third, _4& fourth )
        : SearchListView_ABC( parent )
        , searchLine_( 0 )
        , listView_  ( 0 )
    {
        listView_ = new BaseListView( this, first, second, third, fourth );
        CreateGUI();
    }

    // -----------------------------------------------------------------------------
    // Name: SearchListView constructor
    // Created: ABR 2012-03-27
    // -----------------------------------------------------------------------------
    template< typename BaseListView >
    template< typename _1, typename _2, typename _3, typename _4, typename _5 >
    SearchListView< BaseListView >::SearchListView( QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth )
        : SearchListView_ABC( parent )
        , searchLine_( 0 )
        , listView_  ( 0 )
    {
        listView_ = new BaseListView( this, first, second, third, fourth, fifth );
        CreateGUI();
    }

    // -----------------------------------------------------------------------------
    // Name: SearchListView constructor
    // Created: ABR 2012-03-27
    // -----------------------------------------------------------------------------
    template< typename BaseListView >
    template< typename _1, typename _2, typename _3, typename _4, typename _5, typename _6 >
    SearchListView< BaseListView >::SearchListView( QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth, _6& sixth )
        : SearchListView_ABC( parent )
        , searchLine_( 0 )
        , listView_  ( 0 )
    {
        listView_ = new BaseListView( this, first, second, third, fourth, fifth, sixth );
        CreateGUI();
    }

    // -----------------------------------------------------------------------------
    // Name: SearchListView constructor
    // Created: ABR 2012-03-28
    // -----------------------------------------------------------------------------
    template< typename BaseListView >
    template< typename _1, typename _2, typename _3, typename _4, typename _5, typename _6, typename _7 >
    SearchListView< BaseListView >::SearchListView( QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth, _6& sixth, _7& seventh )
        : SearchListView_ABC( parent )
        , searchLine_( 0 )
        , listView_  ( 0 )
    {
        listView_ = new BaseListView( this, first, second, third, fourth, fifth, sixth, seventh );
        CreateGUI();
    }

    // -----------------------------------------------------------------------------
    // Name: SearchListView destructor
    // Created: ABR 2012-03-27
    // -----------------------------------------------------------------------------
    template< typename BaseListView >
    SearchListView< BaseListView >::~SearchListView()
    {
        // NOTHING
    }

    // -----------------------------------------------------------------------------
    // Name: SearchListView::GetListView
    // Created: ABR 2012-03-27
    // -----------------------------------------------------------------------------
    template< typename BaseListView >
    inline
    BaseListView* SearchListView< BaseListView >::GetListView() const
    {
        return listView_;
    }

    // -----------------------------------------------------------------------------
    // Name: SearchListView::GetRichListView
    // Created: ABR 2012-03-29
    // -----------------------------------------------------------------------------
    template< typename BaseListView >
    inline
    RichListView* SearchListView< BaseListView >::GetRichListView() const
    {
        return listView_;
    }

    // -----------------------------------------------------------------------------
    // Name: SearchListView::CreateGUI
    // Created: ABR 2012-03-27
    // -----------------------------------------------------------------------------
    template< typename BaseListView >
    inline
    void SearchListView< BaseListView >::CreateGUI()
    {
        assert( listView_ != 0 );

        // Search box
        searchLine_ = new SearchLineEdit( this );

        // Next Button
        QPushButton* next = new QPushButton( QIcon( MAKE_ICON( search ) ), "", this );
        next->setAccel( Qt::Key_F3 );
        next->setMaximumWidth( searchLine_->height() );
        next->setMaximumHeight( searchLine_->height() );

        // Filters
        filtersWidget_ = new QWidget();
        filtersWidget_->setVisible( false );
        QVBoxLayout* filterMainLayout = new QVBoxLayout( filtersWidget_ );
        filterMainLayout->setSpacing( 5 );
        filterMainLayout->setContentsMargins( 5, 0, 0, 0 );

        // $$$$ ABR 2012-06-21: TODO: Replace filterLabel with a CollapsableGroupBox and clearLabel with a button
        filterLabel_ = new QLabel();
        clearLabel_ = new QLabel( "<a href='clear'>" + tr( "Clear filters" ) + "</a>" );
        filtersContainer_ = new QWidget();
        filtersLayout_ = new QGridLayout( filtersContainer_, 0, 2 );
        filterMainLayout->addWidget( filterLabel_ );
        filterMainLayout->addWidget( clearLabel_ );
        filterMainLayout->addWidget( filtersContainer_ );

        // Connection
        connect( filterLabel_, SIGNAL( linkActivated( const QString& ) ), this,      SLOT( OnLinkActivated( const QString& ) ) );
        connect( clearLabel_,  SIGNAL( linkActivated( const QString& ) ), this,      SLOT( OnLinkActivated( const QString& ) ) );
        connect( searchLine_,  SIGNAL( textChanged( const QString& ) ),   listView_, SLOT( SearchAndSelect( const QString& ) ) );
        connect( searchLine_,  SIGNAL( returnPressed() ),                 listView_, SLOT( SearchAndSelectNext() ) );
        connect( next,         SIGNAL( pressed() ),                       listView_, SLOT( SearchAndSelectNext() ) );

        // Layout
        QGridLayout* layout = new QGridLayout( this, 3, 2, 5 );
        layout->setMargin( 2 );
        layout->addWidget( searchLine_, 0, 0 );
        layout->addWidget( next, 0, 1 );
        layout->addWidget( filtersWidget_, 1, 0, 1, 2 );
        layout->addWidget( listView_, 2, 0, 1, 2 );

        OnLinkActivated( "hide" );
    }
}