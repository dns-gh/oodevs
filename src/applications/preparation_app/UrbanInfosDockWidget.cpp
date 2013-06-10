// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UrbanInfosDockWidget.h"

#include "clients_gui/Infrastructure_ABC.h"
#include "clients_gui/ResourceNetwork_ABC.h"
#include "clients_gui/RichWidget.h"
#include "clients_gui/Usages.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_kernel/PhysicalAttribute_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/UrbanTypes.h"
#include "clients_kernel/Usages_ABC.h"

#include "preparation/StaticModel.h"
#include "preparation/UrbanHierarchies.h"
#include "preparation/UrbanModel.h"

// -----------------------------------------------------------------------------
// Name: UrbanInfosDockWidget constructor
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
UrbanInfosDockWidget::UrbanInfosDockWidget( QWidget* parent, kernel::Controllers& controllers, UrbanModel& model )
    : RichDockWidget( controllers, parent, "urbanInfosDockWidget", tools::translate( "UrbanInfosDockWidget", "Urban informations" ) )
    , controllers_( controllers )
    , model_      ( model )
{
    gui::SubObjectName subObject( this->objectName() );
    infoEditText_ = new gui::RichWidget< QTextEdit >( "infoEditText", this );
    infoEditText_->setReadOnly( true );
    infoEditText_->setLineWrapMode( gui::RichWidget< QTextEdit >::NoWrap );
    QTextCharFormat textCharFormat;
    textCharFormat.setFont( QFont( "Courrier" ) );
    textCharFormat.setFontFixedPitch( true );
    textCharFormat.setFontPointSize( 8 );
    infoEditText_->setCurrentCharFormat( textCharFormat );

    QWidget* content = new QWidget( this );
    QVBoxLayout* mainLayout = new QVBoxLayout( content );
    mainLayout->setMargin( 5 );
    mainLayout->addWidget( infoEditText_ );
    setWidget( content );

    Update();

    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanInfosDockWidget destructor
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
UrbanInfosDockWidget::~UrbanInfosDockWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanInfosDockWidget::SelectBlocks
// Created: JSR 2012-05-29
// -----------------------------------------------------------------------------
void UrbanInfosDockWidget::SelectBlocks( const kernel::UrbanObject_ABC& urbanObject )
{
    const UrbanHierarchies* urbanHierarchies = static_cast< const UrbanHierarchies* >( urbanObject.Retrieve< kernel::Hierarchies >() );
    if( urbanHierarchies )
    {
        if( urbanHierarchies->GetLevel() == eUrbanLevelBlock )
        {
            if( std::find( selectedElements_.begin(), selectedElements_.end(), &urbanObject ) == selectedElements_.end() )
                selectedElements_.push_back( &urbanObject );
        }
        else
        {
            tools::Iterator< const kernel::Entity_ABC& > it = urbanHierarchies->CreateIterator();
            while( it.HasMoreElements() )
                SelectBlocks( static_cast< const kernel::UrbanObject_ABC& >( it.NextElement() ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanInfosDockWidget::NotifySelectionChanged
// Created: ABR 2012-05-25
// -----------------------------------------------------------------------------
void UrbanInfosDockWidget::NotifySelectionChanged( const T_Elements& elements )
{
    selectedElements_.clear();
    for( auto it = elements.begin(); it != elements.end(); ++it )
    {
        const kernel::UrbanObject_ABC* urbanObject = *it;
        if( urbanObject )
            SelectBlocks( *urbanObject );
    }
    Update();
}

// -----------------------------------------------------------------------------
// Name: UrbanInfosDockWidget::NotifyCreated
// Created: ABR 2012-05-25
// -----------------------------------------------------------------------------
void UrbanInfosDockWidget::NotifyCreated( const kernel::UrbanObject_ABC& )
{
    Update();
}

// -----------------------------------------------------------------------------
// Name: UrbanInfosDockWidget::NotifyDeleted
// Created: ABR 2012-05-25
// -----------------------------------------------------------------------------
void UrbanInfosDockWidget::NotifyDeleted( const kernel::UrbanObject_ABC& element )
{
    selectedElements_.erase( std::remove( selectedElements_.begin(), selectedElements_.end(), &element ), selectedElements_.end() );
    Update();
}

// -----------------------------------------------------------------------------
// Name: UrbanInfosDockWidget::NotifyUpdated
// Created: ABR 2012-05-25
// -----------------------------------------------------------------------------
void UrbanInfosDockWidget::NotifyUpdated( const kernel::UrbanObject_ABC& )
{
    Update();
}

// -----------------------------------------------------------------------------
// Name: UrbanInfosDockWidget::InsertLine
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
void UrbanInfosDockWidget::InsertLine( const QString& line /* = "" */, const QString& value /* = "" */ )
{
    infos_ << line;
    values_ << value;
}

namespace
{
    void ComputeInformations( const kernel::UrbanObject_ABC& urbanObject, unsigned int& nonMedicalInfrastructures, unsigned int& medicalInfrastructures, unsigned int& totalCapacity,
                              std::map< std::string, unsigned int >& motivationsCapacities, std::map< std::string, unsigned int >& resourcesProd, std::map< std::string, unsigned int >& resourceConso )
    {
        // Infra
        auto infrastructure = urbanObject.Retrieve< gui::Infrastructure_ABC >();
        if( infrastructure )
        {
            const kernel::InfrastructureType* infraType = infrastructure->GetType();
            if( infraType )
            {
                if( infraType->IsMedical() )
                    ++medicalInfrastructures;
                else
                    ++nonMedicalInfrastructures;
            }
        }

        // Usages
        const kernel::PhysicalAttribute_ABC& pPhysical = urbanObject.Get< kernel::PhysicalAttribute_ABC >();
        const kernel::Usages_ABC& usages = pPhysical.GetUsages();
        {
            const kernel::Usages_ABC::T_Occupations& occupations = usages.GetOccupations();
            for( kernel::Usages_ABC::CIT_Occupations it = occupations.begin(); it != occupations.end(); ++it )
                if(it->first != gui::Usages::defaultStr_ )
                {
                    totalCapacity += it->second.first;
                    motivationsCapacities[ it->first ] += it->second.first;
                }
        }
        // Resource Network
        auto resourceAttribute = urbanObject.Retrieve< gui::ResourceNetwork_ABC >();
        if( resourceAttribute )
        {
            const auto& resourceNodes = resourceAttribute->GetResourceNodes();
            for( auto itResource = resourceNodes.begin(); itResource != resourceNodes.end(); ++itResource )
            {
                const auto& resourceNode = itResource->second;
                resourcesProd[ itResource->first ] += resourceNode.production_;
                resourceConso[ itResource->first ] += resourceNode.consumption_;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanInfosDockWidget::Update
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
void UrbanInfosDockWidget::Update()
{
    int nbUrbanBlocks = static_cast< int >( selectedElements_.size() );
    unsigned int nonMedicalInfrastructures = 0;
    unsigned int medicalInfrastructures = 0;
    unsigned int totalCapacity = 0;
    std::map< std::string, unsigned int > motivationsCapacities;
    std::map< std::string, unsigned int > resourcesProd;
    std::map< std::string, unsigned int > resourceConso;
    if( selectedElements_.empty() )
    {
        tools::Iterator< const kernel::UrbanObject_ABC& > it = model_.CreateIterator();
        while( it.HasMoreElements() )
        {
            const kernel::UrbanObject_ABC& urbanObject = it.NextElement();
            const UrbanHierarchies& hierarchy = *static_cast< const UrbanHierarchies* >( urbanObject.Retrieve< kernel::Hierarchies >() );
            if( hierarchy.GetLevel() != eUrbanLevelBlock ) // only urban block here
                continue;
            ++nbUrbanBlocks;
            ComputeInformations( urbanObject, nonMedicalInfrastructures, medicalInfrastructures, totalCapacity, motivationsCapacities, resourcesProd, resourceConso );
        }
    }
    else
        for( auto it = selectedElements_.begin(); it != selectedElements_.end(); ++it )
            ComputeInformations( **it, nonMedicalInfrastructures, medicalInfrastructures, totalCapacity, motivationsCapacities, resourcesProd, resourceConso );

    infos_.clear();
    values_.clear();
    InsertLine( ( selectedElements_.empty() ) ? tools::translate( "UrbanInfosDockWidget", "Total urban blocks number: " ) : tools::translate( "UrbanInfosDockWidget", "Selected urban blocks number: " ), locale().toString( nbUrbanBlocks ) );

    InsertLine();
    InsertLine( tools::translate( "UrbanInfosDockWidget", "Non medical infrastructures: " ), locale().toString( nonMedicalInfrastructures ) );
    InsertLine( tools::translate( "UrbanInfosDockWidget", "Medical infrastructures: " ), locale().toString( medicalInfrastructures ) );
    InsertLine();
    InsertLine( tools::translate( "UrbanInfosDockWidget", "Total capacity: " ), locale().toString( totalCapacity ) );
    for( std::map< std::string, unsigned int >::const_iterator it = motivationsCapacities.begin(); it != motivationsCapacities.end(); ++it )
        InsertLine( tools::translate( "UrbanInfosDockWidget", "Capacity  %1: " ).arg( QString::fromStdString( it->first ) ), locale().toString( it->second ) );
    InsertLine();
    for( std::map< std::string, unsigned int >::const_iterator it = resourcesProd.begin(); it != resourcesProd.end(); ++it )
        InsertLine( tools::translate( "UrbanInfosDockWidget", "Resource %1 production: " ).arg( QString::fromStdString( it->first ) ), locale().toString( it->second ) );
    InsertLine();
    for( std::map< std::string, unsigned int >::const_iterator it = resourceConso.begin(); it != resourceConso.end(); ++it )
        InsertLine( tools::translate( "UrbanInfosDockWidget", "Resource %1 consumption: " ).arg( QString::fromStdString( it->first ) ), locale().toString( it->second ) );

    assert( infos_.size() == values_.size() );
    QString info;
    int maxSize = -1;
    for( int i = 0; i < infos_.size(); ++i )
        maxSize = std::max< int >( maxSize, infos_[ i ].size() );
    for( int i = 0; i < infos_.size(); ++i )
    {
        info += infos_[ i ];
        if( !values_[ i ].isEmpty() )
        {
            for( int j = infos_[ i ].size(); j < maxSize; ++j )
                info += " ";
            info += values_[ i ];
        }
        info += "\n";
    }
    infoEditText_->setText( info );
}
