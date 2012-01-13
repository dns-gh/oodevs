// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: ListParameter constructor
// Created: ABR 2012-01-09
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
ListParameter< ConcreteElement >::ListParameter( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : ListParameterBase( builder, parameter )
    , builder_      ( builder )
    , controller_   ( builder.GetControllers().actions_ )
    , list_         ( 0 )
    , selected_     ( 0 )
    , min_          ( parameter.MinOccurs() )
    , max_          ( parameter.MaxOccurs() )
    , createEnabled_( true )
    , count_        ( 0 )
{
    CreatePotential();
}

// -----------------------------------------------------------------------------
// Name: ListParameter destructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
ListParameter< ConcreteElement >::~ListParameter()
{
    if( potential_ )
        potential_->RemoveFromController();
    delete potential_;
    Clear();
    delete list_;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::CreatePotential
// Created: ABR 2011-12-30
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::CreatePotential()
{
    potential_ = CreateElement();
    if( potential_ )
        potential_->RegisterIn( controller_ );
}

// -----------------------------------------------------------------------------
// Name: ListParameter::InternalCheckValidity
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
bool ListParameter< ConcreteElement >::InternalCheckValidity() const
{
    if( !list_ )
        return false;
    unsigned int children = list_->childCount();
    if( min_ > children || max_ < children )
        return false;
    if( selected_ )
        if( Param_ABC* param = static_cast< const ::gui::ValuedListItem* >( selected_ )->GetValue< Param_ABC >() )
            return param->CheckValidity();
    return true;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::BuildInterface
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
QWidget* ListParameter< ConcreteElement >::BuildInterface( QWidget* parent )
{
    Param_ABC::BuildInterface( parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );
    Q3VBox* vbox = new Q3VBox( parent );
    list_ = new Q3ListView( vbox );
    list_->setSorting( -1 );
    list_->setMaximumHeight( 100 );
    list_->addColumn( GetName() );
    list_->header()->hide();
    list_->setResizeMode( Q3ListView::LastColumn );
    connect( list_, SIGNAL( selectionChanged( Q3ListViewItem* ) ), SLOT( OnSelectionChanged( Q3ListViewItem* ) ) );
    connect( list_, SIGNAL( contextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnRequestPopup( Q3ListViewItem*, const QPoint& ) ) );
    layout->addWidget( vbox );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::OnRequestPopup
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::OnRequestPopup( Q3ListViewItem* item, const QPoint& pos )
{
    kernel::ContextMenu* menu = new kernel::ContextMenu( list_ );
    if( createEnabled_ )
        menu->insertItem( tools::translate( "ListParameter", "Add" ), this, SLOT( OnCreate() ) );
    if( item )
        menu->insertItem( tools::translate( "ListParameter", "Remove" ), this, SLOT( OnDeleteSelectedItem() ) );
    menu->insertItem( tools::translate( "ListParameter", "Clear list" ), this, SLOT( OnClear() ) );
    menu->popup( pos );
}

// -----------------------------------------------------------------------------
// Name: ListParameter::EnableCreation
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::EnableCreation( bool enabled )
{
    createEnabled_ = enabled;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::OnCreate
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::OnCreate()
{
    if( ! list_ || ( list_->childCount() && ! CheckValidity() ) )
        return;
    Param_ABC* param = CreateElement();
    if( param )
        AddElement( *param );
}

// -----------------------------------------------------------------------------
// Name: ListParameter::AddElement
// Created: ABR 2011-12-28
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::AddElement( Param_ABC& param )
{
    param.SetOptional( false );
    Q3VBox* widget = new Q3VBox( list_->parentWidget() );
    widgets_[ &param ] = widget;
    param.BuildInterface( widget );
    ::gui::ValuedListItem* item = new ::gui::ValuedListItem( list_, list_->lastItem() );
    item->SetValue( &param );
    item->setText( 0, param.GetName() );
    list_->setSelected( item, true );
    if( group_ && IsOptional() )
        group_->setChecked( true );
}

// -----------------------------------------------------------------------------
// Name: ListParameter::OnDeleteSelectedItem
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::OnDeleteSelectedItem()
{
    if( list_ )
    {
        DeleteItem( list_->selectedItem() );
        if( group_ && IsOptional() )
            group_->setChecked( list_->childCount() != 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: ListParameter::OnClear
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::OnClear()
{
    Clear();
    if( group_ && IsOptional() )
        group_->setChecked( false );
}

// -----------------------------------------------------------------------------
// Name: ListParameter::OnSelectionChanged
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::OnSelectionChanged( Q3ListViewItem* item )
{
    if( selected_ == item )
        return;
    if( selected_ )
    {
        if( CheckValidity() )
        {
            if( Param_ABC* param = static_cast< ::gui::ValuedListItem* >( selected_ )->GetValue< Param_ABC >() )
            {
                param->RemoveFromController();
                widgets_[param]->hide();
            }
        }
        else
        {
            list_->setSelected( selected_, true );
            return;
        }
    }
    if( item )
    {
        if( Param_ABC* param = static_cast< ::gui::ValuedListItem* >( item )->GetValue< Param_ABC >() )
        {
            param->RegisterIn( controller_ );
            widgets_[param]->show();
        }
    }
    selected_ = item;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::DeleteItem
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::DeleteItem( Q3ListViewItem* item )
{
    if( item == selected_ )
    {
        if( Param_ABC* param = static_cast< ::gui::ValuedListItem* >( selected_ )->GetValue< Param_ABC >() )
        {
            param->RemoveFromController();
            widgets_[param]->hide();
        }
        selected_->setSelected( false );
        selected_ = 0;
    }
    if( Param_ABC* param = static_cast< ::gui::ValuedListItem* >( item )->GetValue< Param_ABC >() )
        DeleteElement( *param );
    delete item;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::DeleteElement
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::DeleteElement( Param_ABC& param )
{
    widgets_.erase( &param );
    delete &param;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::Clear
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::Clear()
{
    if( list_ )
        while( list_->childCount() )
            DeleteItem( list_->firstChild() );
}

// -----------------------------------------------------------------------------
// Name: ListParameter::Count
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
unsigned int ListParameter< ConcreteElement >::Count() const
{
    return list_->childCount();
}

// -----------------------------------------------------------------------------
// Name: ListParameter::CommitChildrenTo
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
bool ListParameter< ConcreteElement >::CommitChildrenTo( actions::ParameterContainer_ABC& parent ) const
{
    bool result = false;
    if( !list_ || !IsChecked() )
        return result;
    for( Q3ListViewItemIterator it = Q3ListViewItemIterator( list_ ); it.current(); ++it )
    {
        ::gui::ValuedListItem* item = static_cast< ::gui::ValuedListItem* >( it.current() );
        item->GetValue< Param_ABC >()->CommitTo( parent );
        if( !result )
            result = true;
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::Select
// Created: SBO 2007-05-02
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::Select( const Param_ABC& param )
{
    ::gui::ValuedListItem* item = ::gui::FindItem( &param, list_->firstChild() );
    if( item )
        list_->setSelected( item, true );
}

// -----------------------------------------------------------------------------
// Name: ListParameter::Invalid
// Created: SBO 2007-04-27
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
bool ListParameter< ConcreteElement >::Invalid()
{
    if( list_ )
        list_->header()->setPaletteForegroundColor( Qt::red );
    QTimer::singleShot( 3000, this, SLOT( TurnHeaderBlack() ) );
    return false;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::TurnHeaderBlack
// Created: SBO 2007-04-27
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::TurnHeaderBlack()
{
    if( list_ )
        list_->header()->setPaletteForegroundColor( Qt::black );
}

// -----------------------------------------------------------------------------
// Name: ListParameter::SetName
// Created: ABR 2011-01-21
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::SetName( const QString& name )
{
    if( list_ )
    {
        assert( list_->columns() == 1 );
        list_->setColumnText( 0, name );
    }
    ListParameterBase::SetName( name );
}

// -----------------------------------------------------------------------------
// Name: ListParameter::CommitTo
// Created: ABR 2011-11-30
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::ParameterList( parameter_ ) );
    param->Set( CommitChildrenTo( *param ) );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ListParameter::Draw
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( !list_ )
        return;
    Q3ListViewItemIterator it( list_ );
    for( unsigned int i = 0; it.current(); ++it, ++i )
    {
        ::gui::ValuedListItem* item = static_cast< ::gui::ValuedListItem* >( it.current() );
        item->GetValue< Param_ABC >()->Draw( point, viewport, tools );
    }
}

// -----------------------------------------------------------------------------
// Name: ListParameter::CreateElement
// Created: MMC 2011-12-19
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
Param_ABC* ListParameter< ConcreteElement >::CreateElement() 
{
    kernel::OrderParameter param = parameter_;
    param.SetName( tools::translate( "ListParameter", "%1 (item %2)" ).arg( parameter_.GetName().c_str() ).arg( ++count_ ).toStdString() );
    param.SetOptional( false );
    param.SetMinMaxOccurs( 1, 1 );
    Param_ABC& parameter = builder_.BuildOne( param, false );
    parameter.SetParentList( this );
    return &parameter;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::IsSelected
// Created: ABR 2011-12-23
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
bool ListParameter< ConcreteElement >::IsSelected( Param_ABC* parameter )
{
    if( selected_ )
    {
        if( Param_ABC* selected = static_cast< ::gui::ValuedListItem* >( selected_ )->GetValue< Param_ABC >() )
        {
            return parameter == selected;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::IsPotential
// Created: ABR 2011-12-28
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
bool ListParameter< ConcreteElement >::IsPotential( Param_ABC* param )
{
    return param == potential_;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::OnMenuClick
// Created: ABR 2011-12-28
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::OnMenuClick()
{
    assert( potential_ );
    Param_ABC* newParam = potential_;
    newParam->RemoveFromController();
    AddElement( *newParam );
    CreatePotential();
}

// -----------------------------------------------------------------------------
// Name: ListParameter::CreateInternalMenu
// Created: ABR 2012-01-09
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::CreateInternalMenu( kernel::ContextMenu& mainMenu )
{
    kernel::ContextMenu* menu = new kernel::ContextMenu( &mainMenu );
    menu->setTitle( GetMenuName() );
    internalMenu_ = menu;
}
