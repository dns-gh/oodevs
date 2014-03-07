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
#include "MultipleResolverEditor.h"
#include "preparation/Model.h"
#include "preparation/AgentsModel.h"
#include "preparation/FormationModel.h"
#include "preparation/GhostModel.h"
#include "preparation/StaticModel.h"
#include "preparation/TeamKarmas.h"
#include "clients_gui/CriticalIntelligenceDialog.h"
#include "clients_gui/RichWidget.h"
#include "clients_gui/ValuedComboBox.h"
#include "clients_gui/Tools.h"
#include "clients_gui/ValueEditor.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/NBCAgent.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/KnowledgeGroupType.h"
#include "clients_kernel/MaterialCompositionType.h"
#include "clients_kernel/RoofShapeType.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/UrbanTemplateType.h"
#include "tools/GeneralConfig.h"
#include "PopulationRepartitionEditor.h"
#include "PositionEditor.h"
#include "UrbanColorEditor.h"

// -----------------------------------------------------------------------------
// Name: EditorFactory constructor
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
EditorFactory::EditorFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel,
                              const tools::GeneralConfig& config )
    : controllers_( controllers )
    , model_      ( model )
    , staticModel_( staticModel )
    , config_     ( config )
    , selected_   ( controllers )
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
// Name: EditorFactory::Call
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
void EditorFactory::Call( kernel::LogisticBaseSuperior* const& value )
{
    if( !selected_ )
        return;
    LogisticSuperiorEditor* editor = new LogisticSuperiorEditor( parent_, controllers_, *model_.agents_, *model_.formations_, *model_.ghosts_, *selected_ );
    editor->SetCurrentItem( (kernel::LogisticBaseSuperior&)*value );
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
                               , public gui::ValueEditor< EditorType >
    {
    public:
        SimpleResolverEditor( QWidget* parent, const Resolver& resolver, bool selectionField = false, const QString& selectionText = " - " )
            : gui::ValuedComboBox< const Entity* >( "SimpleResolverEditor", parent )
        {
            if( selectionField )
                AddItem( selectionText, 0 );
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
    typedef tools::StringResolver< kernel::KnowledgeGroupType > T_Resolver;
    tools::StringResolver< kernel::KnowledgeGroupType >& types = static_cast< tools::StringResolver< kernel::KnowledgeGroupType >& >( staticModel_.types_ );
    SimpleResolverEditor< kernel::KnowledgeGroupType, T_Resolver >* editor = new SimpleResolverEditor< kernel::KnowledgeGroupType, T_Resolver >( parent_, types );
    editor->SetCurrentItem( *value );
    result_ = editor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2007-11-02
// -----------------------------------------------------------------------------
void EditorFactory::Call( kernel::TeamKarma* const& value )
{
    SimpleResolverEditor< kernel::Karma, TeamKarmas, kernel::TeamKarma >* editor = new SimpleResolverEditor< kernel::Karma, TeamKarmas, kernel::TeamKarma >( parent_, staticModel_.teamKarmas_ );
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
    template< typename Enum >
    class EnumEditor : public gui::RichWidget< QComboBox >
                     , public gui::ValueEditor< Enum >
    {
    public:
        explicit EnumEditor( QWidget* parent )
            : gui::RichWidget< QComboBox >( "EnumEditor", parent )
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
// Created: ABR 2011-11-29
// -----------------------------------------------------------------------------
void EditorFactory::Call( Enum_NbcState* const& value )
{
    EnumEditor< Enum_NbcState >* editor = new EnumEditor< Enum_NbcState >( parent_ );
    editor->SetCurrentItem( *value );
    result_ = editor;
}

namespace
{
    typedef tools::Resolver_ABC< kernel::NBCAgent, unsigned long > T_NBCResolver;

    class NBCAgentEditor : public MultipleResolverEditor< kernel::NBCAgent, T_NBCResolver >
    {
    public:
        NBCAgentEditor( const QString& objectName, QWidget* parent, const T_NBCResolver& resolver )
            : MultipleResolverEditor< kernel::NBCAgent, T_NBCResolver >( objectName, parent, resolver )
        {
            setCaption( tools::translate( "EditorFactory", "NBC agent(s) editor" ) );
        }
        virtual ~NBCAgentEditor() {}
    };
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-31
// -----------------------------------------------------------------------------
void EditorFactory::Call( std::vector< kernel::NBCAgent* >* const& value )
{
    NBCAgentEditor* editor = new NBCAgentEditor( "NBCAgentEditor", parent_, static_cast< T_NBCResolver& >( staticModel_.objectTypes_ ) );
    editor->SetCurrentItem( *value );
    result_ = editor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: AME 2010-03-08
// -----------------------------------------------------------------------------
void EditorFactory::Call( kernel::Moveable_ABC** const& value )
{
    PositionEditor* positionEditor = new PositionEditor( parent_, controllers_, staticModel_.coordinateConverter_ );
    positionEditor->SetValue( *value );
    result_ = positionEditor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
void EditorFactory::Call( PopulationRepartition** const& value )
{
    PopulationRepartitionEditor* populationRepartitionEditor = new PopulationRepartitionEditor( parent_, controllers_.controller_ );
    populationRepartitionEditor->SetValue( *value );
    result_ = populationRepartitionEditor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: ABR 2012-05-30
// -----------------------------------------------------------------------------
void EditorFactory::Call( kernel::InfrastructureType** const& value )
{
    typedef tools::Resolver_ABC< kernel::InfrastructureType, std::string > T_Resolver;
    SimpleResolverEditor< kernel::InfrastructureType, T_Resolver >* editor =
        new SimpleResolverEditor< kernel::InfrastructureType, T_Resolver >( parent_, (T_Resolver&)( staticModel_.objectTypes_ ), true, tools::translate( "EditorFactory", "<Select a type>" ) );
    editor->SetCurrentItem( *value );
    result_ = editor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: JSR 2012-06-20
// -----------------------------------------------------------------------------
void EditorFactory::Call( kernel::UrbanTemplateType** const& value )
{
    typedef tools::Resolver_ABC< kernel::UrbanTemplateType, std::string > T_Resolver;
    SimpleResolverEditor< kernel::UrbanTemplateType, T_Resolver >* editor =
        new SimpleResolverEditor< kernel::UrbanTemplateType, T_Resolver >( parent_, (T_Resolver&)( staticModel_.objectTypes_ ), true, tools::translate( "EditorFactory", "<Select a type>" ) );
    editor->SetCurrentItem( *value );
    result_ = editor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: ABR 2012-05-31
// -----------------------------------------------------------------------------
void EditorFactory::Call( kernel::MaterialCompositionType** const& value )
{
    typedef tools::Resolver_ABC< kernel::MaterialCompositionType, std::string > T_Resolver;
    SimpleResolverEditor< kernel::MaterialCompositionType, T_Resolver >* editor = new SimpleResolverEditor< kernel::MaterialCompositionType, T_Resolver >( parent_, (T_Resolver&)( staticModel_.objectTypes_ ) );
    editor->SetCurrentItem( *value );
    result_ = editor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: ABR 2012-05-31
// -----------------------------------------------------------------------------
void EditorFactory::Call( kernel::RoofShapeType** const& value )
{
    typedef tools::Resolver_ABC< kernel::RoofShapeType, std::string > T_Resolver;
    SimpleResolverEditor< kernel::RoofShapeType, T_Resolver >* editor = new SimpleResolverEditor< kernel::RoofShapeType, T_Resolver >( parent_, (T_Resolver&)( staticModel_.objectTypes_ ) );
    editor->SetCurrentItem( *value );
    result_ = editor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
void EditorFactory::Call( kernel::UrbanBlockColor* const& value )
{
    UrbanColorEditor* editor = new UrbanColorEditor( parent_ );
    editor->SetValue( *value );
    result_ = editor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: ABR 2012-07-04
// -----------------------------------------------------------------------------
void EditorFactory::Call( kernel::CriticalIntelligenceType* const& value )
{
    gui::CriticalIntelligenceDialog* intelligenceDialog = new gui::CriticalIntelligenceDialog( parent_, controllers_ );
    intelligenceDialog->SetValue( *value );
    result_ = intelligenceDialog;
}

namespace
{
    class DisasterEditor : public gui::ValuedComboBox< std::string >
                         , public gui::ValueEditor< kernel::DisasterDirectory >
    {
    public:
                DisasterEditor( QWidget* parent, const tools::GeneralConfig& config )
            : gui::ValuedComboBox< std::string >( "DisasterEditor", parent )
        {
            const tools::Path::T_Paths result = ( config.GetRootDir() / "data/propagations" ).ListElements( &tools::IsPropagationDir );
            for( auto it = result.begin(); it != result.end(); ++it )
                AddItem( it->ToUTF8(), it->ToUTF8() );
        }
        virtual ~DisasterEditor() {}

        virtual kernel::DisasterDirectory GetValue()
        {
            return kernel::DisasterDirectory( tools::Path::FromUTF8( gui::ValuedComboBox< std::string >::GetValue() ) );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: LGY 2012-12-13
// -----------------------------------------------------------------------------
void EditorFactory::Call( kernel::DisasterDirectory* const& value )
{
    DisasterEditor* editor= new DisasterEditor( parent_, config_ );
    editor->SetCurrentItem( (*value)().ToUTF8() );
    result_ = editor;
}
