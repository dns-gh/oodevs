// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "EditorFactory.h"
#include "LogisticSuperiorEditor.h"
#include "DotationsEditor.h"
#include "preparation/LogisticSuperior.h"
#include "preparation/Model.h"
#include "preparation/AgentsModel.h"
#include "preparation/StaticModel.h"
#include "preparation/TeamKarmas.h"
#include "preparation/IntelligenceKarmas.h"
#include "clients_gui/ValuedComboBox.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/ValueEditor.h"
#include "clients_kernel/NBCAgent.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/KnowledgeGroupType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include <qvalidator.h>

// -----------------------------------------------------------------------------
// Name: EditorFactory constructor
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
EditorFactory::EditorFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel )
    : gui::EditorFactory()
    , controllers_( controllers )
    , model_( model )
    , staticModel_( staticModel )
    , selected_( controllers )
{
    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: EditorFactory destructor
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
EditorFactory::~EditorFactory()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::BuildLogisticEditor
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
template< typename T >
void EditorFactory::BuildLogisticEditor( T& value )
{
    if( !selected_ )
        return;
    LogisticSuperiorEditor< T >* editor = new LogisticSuperiorEditor< T >( parent_, controllers_, model_.agents_, *selected_ );
    editor->SetCurrentItem( value );
    result_ = editor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
void EditorFactory::Call( TC2* const& value )
{
    BuildLogisticEditor( *value );
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
void EditorFactory::Call( MaintenanceSuperior* const& value )
{
    BuildLogisticEditor( *value );
}
    
// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
void EditorFactory::Call( MedicalSuperior* const& value )
{
    BuildLogisticEditor( *value );
}
    
// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
void EditorFactory::Call( SupplySuperior* const& value )
{
    BuildLogisticEditor( *value );
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::NotifySelected
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
void EditorFactory::NotifySelected( const kernel::Entity_ABC* element )
{
    selected_ = element;
}

namespace
{
    template< typename Entity, typename Resolver, typename EditorType = Entity* >
    class SimpleResolverEditor : public gui::ValuedComboBox< const Entity* >
                               , public kernel::ValueEditor< EditorType >
    {
    public:
        SimpleResolverEditor( QWidget* parent, const Resolver& resolver ) 
            : gui::ValuedComboBox< const Entity* >( parent )
        {
            tools::Iterator< const Entity& > it = resolver.CreateIterator();
            while( it.HasMoreElements() )
            {
                const Entity& entity = it.NextElement();
                AddItem( entity.GetName(), &entity );
            }
        }
        virtual ~SimpleResolverEditor() {}

        virtual EditorType GetValue()
        {
            return EditorType( const_cast< Entity* >( gui::ValuedComboBox< const Entity* >::GetValue() ) );
        }

    };
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
void EditorFactory::Call( kernel::KnowledgeGroupType** const& value )
{
    typedef tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string > T_Resolver;
    SimpleResolverEditor< kernel::KnowledgeGroupType, T_Resolver >* editor = new SimpleResolverEditor< kernel::KnowledgeGroupType, T_Resolver >( parent_, (T_Resolver&)( staticModel_.types_ ) );
    editor->SetCurrentItem( *value );
    result_ = editor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2007-11-02
// -----------------------------------------------------------------------------
void EditorFactory::Call( TeamKarma* const& value )
{
    SimpleResolverEditor< kernel::Karma, TeamKarmas, TeamKarma >* editor = new SimpleResolverEditor< kernel::Karma, TeamKarmas, TeamKarma >( parent_, staticModel_.teamKarmas_ );
    editor->SetCurrentItem( *value );
    result_ = editor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2007-11-02
// -----------------------------------------------------------------------------
void EditorFactory::Call( IntelligenceKarma* const& value )
{
    SimpleResolverEditor< kernel::Karma, IntelligenceKarmas, IntelligenceKarma >* editor = new SimpleResolverEditor< kernel::Karma, IntelligenceKarmas, IntelligenceKarma >( parent_, staticModel_.intelligenceKarmas_ );
    editor->SetCurrentItem( *value );
    result_ = editor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-31
// -----------------------------------------------------------------------------
void EditorFactory::Call( kernel::NBCAgent** const& value )
{
    typedef tools::Resolver_ABC< kernel::NBCAgent, unsigned long > T_Resolver;
    SimpleResolverEditor< kernel::NBCAgent, T_Resolver >* editor = new SimpleResolverEditor< kernel::NBCAgent, T_Resolver >( parent_, (T_Resolver&)( staticModel_.objectTypes_ ) );
    editor->SetCurrentItem( *value );
    result_ = editor;
}

namespace
{
    template< typename Entity, typename Resolver >
    class MultipleResolverEditor : public QListBox
                                 , public kernel::ValueEditor< std::vector< Entity* > >
    {
    public:
        MultipleResolverEditor( QWidget* parent, const Resolver& resolver )
            : QListBox( parent )
        {
            tools::Iterator< const Entity& > it = resolver.CreateIterator();
            while( it.HasMoreElements() )
            {
                const Entity& entity = it.NextElement();
                AddItem( entity.GetName().c_str(), &entity );
            }
            setSelectionMode( QListBox::Multi );
            setMinimumHeight( 3 * itemHeight() );
        }
        virtual ~MultipleResolverEditor() {}

        void AddItem( const QString& name, Entity const* const& entity )
        {
            insertItem( name );
            entities_.push_back( entity );
        }

        void SetCurrentItem( const std::vector< Entity* >& entity )
        {
            for( unsigned int i = 0; i < count(); ++i )
                setSelected( i, std::find( entity.begin(), entity.end(), entities_[i] ) != entity.end() ) ;
        }

        virtual std::vector< Entity* > GetValue()
        {
            std::vector< Entity* > result;
            for( unsigned int i = 0; i < count(); ++i )
                if( isSelected( i ) )
                    result.push_back( const_cast< Entity* >( entities_[i] ) );
            return result;
        }

    private:
        std::vector< const Entity* > entities_;
    };
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-31
// -----------------------------------------------------------------------------
void EditorFactory::Call( std::vector< kernel::NBCAgent* >* const& value )
{
    typedef tools::Resolver_ABC< kernel::NBCAgent, unsigned long > T_Resolver;
    MultipleResolverEditor< kernel::NBCAgent, T_Resolver >* editor = new MultipleResolverEditor< kernel::NBCAgent, T_Resolver >( parent_, (T_Resolver&)( staticModel_.objectTypes_ ));
    editor->SetCurrentItem( *value );
    result_ = editor;
}

namespace
{
    template< typename Enum >
    class EnumEditor : public QComboBox
                     , public kernel::ValueEditor< Enum >
    {
    public:
        explicit EnumEditor( QWidget* parent ) 
            : QComboBox( parent )
        {
            for( int i = 0; i < int( Enum::max() ); ++i )
                insertItem( Enum( i ).ToString() );
        }
        virtual ~EnumEditor() {}

        void SetCurrentItem( Enum& value )
        {
            setCurrentItem( int( value.GetValue() ) );
        }

        virtual Enum GetValue()
        {
            return Enum( currentItem() );
        }

    };
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void EditorFactory::Call( Enum_PopulationAttitude* const& value )
{
    EnumEditor< Enum_PopulationAttitude >* editor= new EnumEditor< Enum_PopulationAttitude >( parent_ );
    editor->SetCurrentItem( *value );
    result_ = editor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void EditorFactory::Call( Enum_DemolitionTargetType* const& value )
{
    EnumEditor< Enum_DemolitionTargetType >* editor= new EnumEditor< Enum_DemolitionTargetType >( parent_ );
    editor->SetCurrentItem( *value );
    result_ = editor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void EditorFactory::Call( DotationsItem** const& value )
{
    DotationsEditor* editor = new DotationsEditor( parent_, staticModel_.objectTypes_, *value );
    editor->SetCurrentItem( *value );
    result_ = 0;
}

// =============================================================================
// QString
// =============================================================================
namespace 
{
    class PositionEditor : public QLineEdit
                         , public kernel::ValueEditor< geometry::Point2f >
    {
    public:
                 PositionEditor( QWidget* parent, const kernel::CoordinateConverter_ABC& converter )
                     : QLineEdit( parent ), converter_( converter ) {}
        virtual ~PositionEditor() {}

        void SetValue( const geometry::Point2f& point )
        {
            setText( converter_.ConvertToMgrs( point ).c_str() );
            lastValid_ = point;
        }

        virtual geometry::Point2f GetValue()
        {
            try
            {
                const geometry::Point2f coordinate = converter_.ConvertToXY( std::string( text().ascii() ) );
                lastValid_ = coordinate;
                return coordinate;
            }
            catch( ... ) {}
            return lastValid_;
        }

    private:
        PositionEditor( const PositionEditor& );
        PositionEditor& operator=( const PositionEditor& );

        const kernel::CoordinateConverter_ABC& converter_;
        geometry::Point2f lastValid_;
    };
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2008-03-25
// -----------------------------------------------------------------------------
void EditorFactory::Call( geometry::Point2f* const& value )
{
    PositionEditor* editor = new PositionEditor( parent_, staticModel_.coordinateConverter_ );
    editor->setValidator( new QRegExpValidator( QRegExp( "^[0-9]{2}[a-zA-Z]{3}[0-9]{10}$" ), editor ) );
    editor->SetValue( *value );
    result_ = editor;
}
