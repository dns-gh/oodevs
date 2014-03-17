// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#define ParamRole ( Qt::UserRole + 1 )
#define IdRole    ( Qt::UserRole + 2 )

// -----------------------------------------------------------------------------
// Name: ListParameter constructor
// Created: ABR 2012-01-09
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
ListParameter< ConcreteElement >::ListParameter( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : ListParameterBase( builder, parameter )
    , builder_      ( builder )
    , list_         ( 0 )
    , selected_     ( 0 )
    , min_          ( parameter.MinOccurs() )
    , max_          ( parameter.MaxOccurs() )
    , createEnabled_( true )
    , count_        ( 0 )
    , entity_       ( builder.GetControllers() )
    , potential_    ( 0 )
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
    potential_ = 0; // Because CreateElement uses potential_
    potential_ = CreateElement();
    if( potential_ )
    {
        potential_->RegisterIn();
        potential_->SetEntity( entity_ );
    }
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
    unsigned int children = model_.rowCount();
    if( min_ > children || max_ < children )
        return false;
    if( selected_ )
        if( Param_ABC* param = selected_->data( ParamRole ).value< Param_ABC* >() )
            return param->CheckValidity();
    return true;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::BuildInterface
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
QWidget* ListParameter< ConcreteElement >::BuildInterface( const QString& objectName, QWidget* parent )
{
    Param_ABC::BuildInterface( objectName, parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );
    group_->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    QWidget* vbox = new QWidget();
    vbox->setLayout( new QVBoxLayout );

    //list of parameters
    list_ = new QTreeView();
    list_->setModel( &model_ );
    list_->setContextMenuPolicy( Qt::CustomContextMenu );
    list_->setRootIsDecorated( false );
    list_->setEditTriggers( 0 );
    list_->setHeaderHidden( true );
    model_.setColumnCount( 1 );
    list_->header()->setResizeMode( 0, QHeaderView::ResizeToContents );
    connect( list_->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), SLOT( OnSelectionChanged( const QItemSelection&, const QItemSelection& ) ) );
    connect( list_, SIGNAL( customContextMenuRequested( const QPoint& ) ), SLOT( OnRequestPopup( const QPoint& ) ) );

    vbox->layout()->addWidget( list_ );
    layout->addWidget( vbox );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::OnRequestPopup
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::OnRequestPopup( const QPoint& pos )
{
    CreateListMenu( list_, model_, pos, createEnabled_ );
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
    if( ! list_ || ( model_.rowCount() && ! CheckValidity() ) )
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
    //add param to interface
    param.SetOptional( false );
    QWidget* widget = new QWidget();
    widget->setLayout( new QVBoxLayout );
    list_->parentWidget()->layout()->addWidget( widget );
    widgets_[ &param ] = widget;
    param.BuildInterface( param.GetName(), widget );
    param.SetEntity( entity_ );

    //add element to list
    QStandardItem* item = new QStandardItem( param.GetName() );
    item->setData( QVariant::fromValue( &param ), ParamRole );
    int id = model_.rowCount() != 0? model_.item( model_.rowCount() - 1 )->data( IdRole ).toInt() + 1 : 1 ;
    item->setData( id, IdRole );
    model_.appendRow( item );
    list_->selectionModel()->select( item->index(), QItemSelectionModel::Select | QItemSelectionModel::Clear );

    if( group_ && IsOptional() )
        group_->setChecked( true );
    Update();
}

// -----------------------------------------------------------------------------
// Name: ListParameter::OnDeleteSelectedItem
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::OnDeleteSelectedItem()
{
    if( list_ )
        if( list_->selectionModel()->currentIndex().isValid() )
        {
            list_->selectionModel()->select( list_->selectionModel()->currentIndex(), QItemSelectionModel::Deselect );
            DeleteItem( model_.itemFromIndex( list_->selectionModel()->currentIndex() ) );
            model_.removeRow( list_->selectionModel()->currentIndex().row() );
            Update();
        }
    if( group_ && IsOptional() )
        group_->setChecked( model_.rowCount() != 0 );
    Update();
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
    Update();
}

// -----------------------------------------------------------------------------
// Name: ListParameter::OnSelectionChanged
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::OnSelectionChanged( const QItemSelection& newSelection , const QItemSelection& /*oldSelection*/ )
{
    if( newSelection.indexes().isEmpty() )
        return;
    QStandardItem* item = model_.itemFromIndex( newSelection.indexes().at( 0 ) );
    if( selected_ == item )
        return;
    if( selected_ )
    {
        if( CheckValidity() )
        {
            if( Param_ABC* param = selected_->data( ParamRole ).value< Param_ABC* >() )
            {
                param->RemoveFromController();
                widgets_[ param ]->hide();
            }
        }
        else
        {
            list_->selectionModel()->select( selected_->index(), QItemSelectionModel::Select | QItemSelectionModel::Clear );
            return;
        }
    }
    if( item && item->data( ParamRole ).isValid())
    {
        Param_ABC* param = item->data( ParamRole ).value< Param_ABC* >();
        param->RegisterIn();
        widgets_[ param ]->show();
    }
    selected_ = item;
}

// -----------------------------------------------------------------------------
// Name: ListParameter::DeleteItem
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::DeleteItem( QStandardItem* item )
{
    if( selected_ && item == selected_ )
    {
        if( Param_ABC* param = selected_->data( ParamRole ).value< Param_ABC* >() )
        {
            param->RemoveFromController();
            widgets_[param]->hide();
        }
        list_->selectionModel()->select( selected_->index(), QItemSelectionModel::Deselect );
        selected_ = 0;
    }
    if( Param_ABC* param = item->data( ParamRole ).value< Param_ABC* >() )
        DeleteElement( *param );
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
    {
        for( int row = 0; row < model_.rowCount(); ++row )
            DeleteItem( model_.item( row ) );
        list_->clearSelection();
        model_.removeRows( 0, model_.rowCount() );
    }
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
    for( int row = 0; row < model_.rowCount(); ++row )
    {
        QStandardItem* item = model_.item( row );
        if( item )
            if( Param_ABC* param = item->data( ParamRole ).value< Param_ABC* >() )
                param->CommitTo( parent );
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
    for( int row = 0; row< model_.rowCount(); ++row )
    {
        QStandardItem* item = model_.item( row );
        if( item )
        {
            Param_ABC* paramItem = selected_->data( ParamRole ).value< Param_ABC* >();
            if( param == paramItem )
                list_->selectionModel()->select( item->index(), QItemSelectionModel::Select | QItemSelectionModel::Clear );
        }
    }
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
void ListParameter< ConcreteElement >::Draw( const geometry::Point2f& point, const ::gui::Viewport_ABC& viewport, ::gui::GlTools_ABC& tools ) const
{
    if( !list_ )
        return;
    for( int row = 0; row < model_.rowCount(); ++row )
    {
        QStandardItem* item = model_.item( row );
        if( item )
            if( Param_ABC* param = item->data( ParamRole ).value< Param_ABC* >() )
                param->Draw( point, viewport, tools );
    }
}

// -----------------------------------------------------------------------------
// Name: ListParameter::CreateNextNameAndId
// Created: ABR 2012-02-21
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
QString ListParameter< ConcreteElement >::CreateNextNameAndId()
{
    return GetNextNameAndId( model_ );
}

// -----------------------------------------------------------------------------
// Name: ListParameter::CreateElement
// Created: MMC 2011-12-19
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
Param_ABC* ListParameter< ConcreteElement >::CreateElement()
{
    kernel::OrderParameter param = parameter_;
    param.SetName( ( potential_ != 0 ) ?  CreateNextNameAndId().toStdString() : "POTENTIAL" );
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
bool ListParameter< ConcreteElement >::IsSelected( Param_ABC* parameter ) const
{
    if( selected_ )
    {
        if( const Param_ABC* selected = selected_->data( ParamRole ).value< Param_ABC* >() )
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
bool ListParameter< ConcreteElement >::IsPotential( Param_ABC* param ) const
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
    newParam->SetName( CreateNextNameAndId() );
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

// -----------------------------------------------------------------------------
// Name: ListParameter::SetEntity
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::SetEntity( const kernel::Entity_ABC* entity )
{
    entity_ = entity;
    if( potential_ )
        potential_->SetEntity( entity );
    if( !list_ )
        return;
    for( int row = 0; row < model_.rowCount(); ++row )
    {
        QStandardItem* item = model_.item( row );
        if( item )
            if( Param_ABC* param = item->data( ParamRole ).value< Param_ABC* >() )
                param->SetEntity( entity );
    }
}

// -----------------------------------------------------------------------------
// Name: ListParameter::Visit
// Created: ABR 2013-06-12
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
void ListParameter< ConcreteElement >::Visit( const actions::Parameter_ABC& param )
{
    if( ! list_ || ( model_.rowCount() && ! CheckValidity() ) )
        return;
    auto it = param.CreateIterator();
    while( it.HasMoreElements() )
    {
        const actions::Parameter_ABC& elem = it.NextElement();
        Param_ABC* param = CreateElement();
        if( param )
        {
            AddElement( *param );
            elem.Accept( *param );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ListParameter::HasParameter
// Created: ABR 2013-11-14
// -----------------------------------------------------------------------------
template< typename ConcreteElement >
bool ListParameter< ConcreteElement >::HasParameter( const Param_ABC& param ) const
{
    for( auto it = widgets_.begin(); it != widgets_.end(); ++it )
        if( it->first && it->first->HasParameter( param ) )
            return true;
    return Param_ABC::HasParameter( param ) || ( potential_ && potential_->HasParameter( param ) );
}
