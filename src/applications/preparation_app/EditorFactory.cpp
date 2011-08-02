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
#include "LogisticLevelEditor.h"
#include "preparation/LogisticSuperior.h"
#include "preparation/Model.h"
#include "preparation/AgentsModel.h"
#include "preparation/FormationModel.h"
#include "preparation/StaticModel.h"
#include "preparation/TeamKarmas.h"
#include "preparation/IntelligenceKarmas.h"
#include "preparation/LogisticLevel.h"
#include "clients_gui/DecimalSpinBox.h"
#include "clients_gui/ValuedComboBox.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/ValueEditor.h"
#include "clients_kernel/NBCAgent.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/KnowledgeGroupType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/CoordinateSystems.h"
#include "PopulationRepartitionEditor.h"
#include "PositionEditor.h"

// -----------------------------------------------------------------------------
// Name: EditorFactory constructor
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
EditorFactory::EditorFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel )
    : controllers_( controllers )
    , model_      ( model )
    , staticModel_( staticModel )
    , selected_   ( controllers )
    , modalDialog_( 0 )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EditorFactory destructor
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
EditorFactory::~EditorFactory()
{
    delete modalDialog_;
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
void EditorFactory::Call( LogisticBaseSuperior* const& value )
{
    if( !selected_ )
        return;
    LogisticSuperiorEditor* editor = new LogisticSuperiorEditor( parent_, controllers_, model_.agents_, model_.formations_, *selected_ );
    editor->SetCurrentItem( (LogisticBaseSuperior&)*value );
    result_ = editor;
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
// Created: SBO 2007-11-02
// -----------------------------------------------------------------------------
void EditorFactory::Call( EntityLogisticLevel* const& value )
{
    SimpleResolverEditor< kernel::LogisticLevel, preparation::LogisticLevel, EntityLogisticLevel >* editor = new SimpleResolverEditor< kernel::LogisticLevel, preparation::LogisticLevel, EntityLogisticLevel >( parent_, staticModel_.logisticLevels_ );
    editor->SetCurrentItem( *value );
    result_ = editor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: HBD 2010-11-15
// -----------------------------------------------------------------------------
void EditorFactory::Call( kernel::LogisticLevel** const& value )
{
    if( !selected_ )
        return;
    LogisticLevelEditor* editor = new LogisticLevelEditor( parent_, *selected_ );
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
    class MultipleResolverEditor : public Q3ListBox
                                 , public kernel::ValueEditor< std::vector< Entity* > >
    {
    public:
        MultipleResolverEditor( QWidget* parent, const Resolver& resolver )
            : Q3ListBox( parent )
        {
            tools::Iterator< const Entity& > it = resolver.CreateIterator();
            while( it.HasMoreElements() )
            {
                const Entity& entity = it.NextElement();
                AddItem( entity.GetName().c_str(), &entity );
            }
            setSelectionMode( Q3ListBox::Multi );
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
    EnumEditor< Enum_DemolitionTargetType >* editor = new EnumEditor< Enum_DemolitionTargetType >( parent_ );
    editor->SetCurrentItem( *value );
    result_ = editor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void EditorFactory::Call( DotationsItem** const& value )
{
    if( !modalDialog_ || !modalDialog_->isActiveWindow() )
    {
        delete modalDialog_;
        DotationsEditor* dotationsEditor = new DotationsEditor( modalDialog_, parent_, staticModel_.objectTypes_, *value );
        dotationsEditor->SetCurrentItem( *value );
        result_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: AME 2010-03-08
// -----------------------------------------------------------------------------
void EditorFactory::Call( kernel::Moveable_ABC** const& value )
{
    if( !modalDialog_ || !modalDialog_->isActiveWindow() )
    {
        delete modalDialog_;
        PositionEditor* positionEditor = new PositionEditor( modalDialog_, parent_, controllers_, staticModel_.coordinateConverter_ );
        positionEditor->SetValue( *value );
        result_ = 0;
    }
}

namespace
{
    class BoundedFloatEditor : public gui::DecimalSpinBox
                             , public kernel::ValueEditor< EntityAffinity >
    {
    public:
        explicit BoundedFloatEditor( QWidget* parent, float value = 0.f, unsigned short precision = 2, float min = 0.f, float max = 10.f, float gap = 0.1f )
            : gui::DecimalSpinBox( parent, value, precision, min, max, gap )
            , factor_( std::pow( 10.f, precision ) )
        {
            // NOTHING
        }
        virtual ~BoundedFloatEditor() {}

        virtual EntityAffinity GetValue()
        {
            return value() / factor_;
        }
        const float factor_;
    };
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2011-02-02
// -----------------------------------------------------------------------------
void EditorFactory::Call( EntityAffinity* const& value )
{
    result_ = new BoundedFloatEditor( parent_, *value, 2, -1.f, 1.f, 0.01f );
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
void EditorFactory::Call( PopulationRepartition** const& value )
{
    if( !modalDialog_ || !modalDialog_->isActiveWindow() )
    {
        delete modalDialog_;
        PopulationRepartitionEditor* populationRepartitionEditor = new PopulationRepartitionEditor( modalDialog_, parent_, controllers_.controller_ );
        populationRepartitionEditor->SetValue( *value );
        result_ = 0;
    }
}
