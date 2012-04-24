// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamDotationTypeList.h"
#include "moc_ParamDotationTypeList.cpp"
#include "InterfaceBuilder_ABC.h"
#include "actions/DotationType.h"
#include "actions/DotationTypeList.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/StaticModel.h"
#include "tools/Iterator.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamDotationTypeList constructor
// Created: ABR 2012-01-10
// -----------------------------------------------------------------------------
ParamDotationTypeList::ParamDotationTypeList( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder.GetParentObject(), builder.GetParamInterface(), parameter )
    , resolver_( builder.GetStaticModel().objectTypes_ )
    , list_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamDotationTypeList destructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
ParamDotationTypeList::~ParamDotationTypeList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamDotationTypeList::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* ParamDotationTypeList::BuildInterface( QWidget* parent, kernel::Entity_ABC& entity )
{
    Param_ABC::BuildInterface( parent, entity );
    QVBoxLayout* layout = new QVBoxLayout( group_ );

    list_ = new Q3ListView( parent );
    list_->setRootIsDecorated( true );
    list_->addColumn( "name" );
    list_->addColumn( "id", 0 );
    list_->setSorting( -1, true );
    list_->header()->hide();
    list_->header()->setResizeEnabled( false, 1 );
    list_->hideColumn( 1 );
    list_->setSelectionMode( Q3ListView::Multi );

    tools::Iterator< const kernel::DotationType& > it = resolver_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::DotationType& type = it.NextElement();
        AddItem( type.GetName().c_str(), type.GetName().c_str(), type.GetId() );
    }
    connect( list_, SIGNAL( clicked( Q3ListViewItem* ) ), SLOT( Clicked( Q3ListViewItem* ) ) );
    layout->addWidget( list_ );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamDotationTypeList::Clicked
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
void ParamDotationTypeList::Clicked( Q3ListViewItem* item )
{
    Q3ListViewItem* child = item ? item->firstChild() : 0;
    bool selected = child ? !child->isSelected() : false;
    list_->ensureItemVisible( child );
    while( child )
    {
        list_->setSelected( child, selected );
        child = child->nextSibling();
    }
}

// -----------------------------------------------------------------------------
// Name: ParamDotationTypeList::AddItem
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
void ParamDotationTypeList::AddItem( const QString& parent, const QString& child, unsigned id )
{
    Q3ListViewItem* parentItem = list_->findItem( parent, 0 );
    if( ! parentItem )
    {
        parentItem = new Q3ListViewItem( list_, parent );
        parentItem->setSelectable( false );
    }
    new Q3ListViewItem( parentItem, child, QString::number( id ) );
}

// -----------------------------------------------------------------------------
// Name: ParamDotationTypeList::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ParamDotationTypeList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    if( !list_ )
        return;
    std::auto_ptr< actions::Parameter_ABC > param( new actions::parameters::DotationTypeList( parameter_ ) );
    if( IsChecked() )
    {
        Q3ListViewItemIterator it( list_ );
        while( it.current() )
        {
            if( it.current()->isSelected() )
            {
                const unsigned id = it.current()->text( 1 ).toUInt();
                param->AddParameter( *new actions::parameters::DotationType( parameter_, id, resolver_ ) );
            }
            ++it;
        }
    }
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamDotationTypeList::InternalCheckValidity
// Created: ABR 2011-11-22
// -----------------------------------------------------------------------------
bool ParamDotationTypeList::InternalCheckValidity() const
{
    return list_ && list_->childCount() != 0;
}
