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

#include "clients_kernel/Controllers.h"
#include "preparation/StaticModel.h"
#include "preparation/UrbanModel.h"

//namespace
//{
//    bool IsMedicalInfrastructure( urban::StaticModel& staticModel, const urban::InfrastructureAttribute& infrastructure )
//    {
//        tools::Iterator< const urban::InfrastructureType& > it = staticModel.CreateIterator< urban::InfrastructureType >();
//        while( it.HasMoreElements() )
//        {
//            const urban::InfrastructureType& infraType = it.NextElement();
//            if( infraType.GetName() == infrastructure.GetType() )
//                return infraType.HasMedicalCapacity();
//        }
//        return false;
//    }
//
//    unsigned int GetMotivationCapacity( urban::StaticModel& staticModel, const std::string motivation, double surface, double proportion )
//    {
//        tools::Iterator< const urban::MotivationType& > it = staticModel.CreateIterator< urban::MotivationType >();
//        while( it.HasMoreElements() )
//        {
//            const urban::MotivationType& motivationType = it.NextElement();
//            if( motivationType.GetName() == motivation )
//                return static_cast< unsigned int >( surface * motivationType.GetNominalCapacity() * proportion );
//        }
//        return 0;
//    }
//}

// -----------------------------------------------------------------------------
// Name: UrbanInfosDockWidget constructor
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
UrbanInfosDockWidget::UrbanInfosDockWidget( QWidget* parent, kernel::Controllers& controllers, StaticModel& staticModel, UrbanModel& model )
    : RichDockWidget( controllers, parent, "urbanInfosDockWidget", tr( "Urban informations" ), false )
    , controllers_( controllers )
    , staticModel_( staticModel )
    , model_      ( model )
{
    infoEditText_ = new QTextEdit( this );
    infoEditText_->setReadOnly( true );
    infoEditText_->setLineWrapMode( QTextEdit::NoWrap );
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

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanInfosDockWidget destructor
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
UrbanInfosDockWidget::~UrbanInfosDockWidget()
{
    controllers_.Unregister( *this );
}

//// -----------------------------------------------------------------------------
//// Name: UrbanInfosDockWidget::SelectionChanged
//// Created: ABR 2012-05-16
//// -----------------------------------------------------------------------------
//void UrbanInfosDockWidget::SelectionChanged()
//{
//    Update();
//}
//
//// -----------------------------------------------------------------------------
//// Name: UrbanInfosDockWidget::NotifyCreated
//// Created: ABR 2012-05-16
//// -----------------------------------------------------------------------------
//void UrbanInfosDockWidget::NotifyCreated( const urban::TerrainObject_ABC& element )
//{
//    Update();
//}
//
//// -----------------------------------------------------------------------------
//// Name: UrbanInfosDockWidget::NotifyDeleted
//// Created: ABR 2012-05-16
//// -----------------------------------------------------------------------------
//void UrbanInfosDockWidget::NotifyDeleted( const urban::TerrainObject_ABC& element )
//{
//    Update();
//}
//
//// -----------------------------------------------------------------------------
//// Name: UrbanInfosDockWidget::NotifyUpdated
//// Created: ABR 2012-05-16
//// -----------------------------------------------------------------------------
//void UrbanInfosDockWidget::NotifyUpdated( const urban::TerrainObject_ABC& element )
//{
//    Update();
//}

// -----------------------------------------------------------------------------
// Name: UrbanInfosDockWidget::InsertLine
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
void UrbanInfosDockWidget::InsertLine( const QString& line /* = "" */, const QString& value /* = "" */ )
{
    infos_ << line;
    values_ << value;
}

// -----------------------------------------------------------------------------
// Name: UrbanInfosDockWidget::Update
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
void UrbanInfosDockWidget::Update()
{
    //TerrainVisitor terrainVisitor;
    //T_TerrainObjects& selected = const_cast< T_TerrainObjects& >( controller_.GetSelected() );
    //if( selected.empty() )
    //    model_.Accept( terrainVisitor );
    //else
    //    for( IT_TerrainObjects it = selected.begin(); it != selected.end(); ++it )
    //        (*it)->Accept( terrainVisitor );

    //unsigned int nonMedicalInfrastructures = 0, medicalInfrastructures = 0, totalCapacity = 0;
    //std::map< std::string, unsigned int > motivationsCapacities, resourcesProd, resourceConso;
    //for( std::set< urban::TerrainObject_ABC* >::iterator it = terrainVisitor.urbanBlocks_.begin(); it != terrainVisitor.urbanBlocks_.end(); ++it )
    //{
    //    urban::TerrainObject_ABC* pTerrainObject = *it;
    //    urban::InfrastructureAttribute* infrastructure = pTerrainObject->Retrieve< urban::InfrastructureAttribute >();
    //    if( infrastructure )
    //    {
    //        if( IsMedicalInfrastructure( staticModel_, *infrastructure ) )
    //            ++medicalInfrastructures;
    //        else
    //            ++nonMedicalInfrastructures;
    //    }

    //    MotivationsVisitor motivationsVisitor;
    //    pTerrainObject->Accept( motivationsVisitor );
    //    for( std::map< std::string, float >::iterator itMotiv = motivationsVisitor.proportions_.begin(); 
    //        itMotiv != motivationsVisitor.proportions_.end(); ++itMotiv  )
    //    {
    //        unsigned int curCapacity = GetMotivationCapacity( staticModel_, itMotiv->first, pTerrainObject->GetLivingSpace(), itMotiv->second );
    //        totalCapacity += curCapacity;
    //        motivationsCapacities[ itMotiv->first ] += curCapacity;
    //    }

    //    urban::ResourceNetworkAttribute* resourceAttribute = pTerrainObject->Retrieve< urban::ResourceNetworkAttribute >();
    //    if( resourceAttribute )
    //    {
    //        const urban::ResourceNetworkAttribute::T_ResourceNodes& resourceNodes = resourceAttribute->GetResourceNodes();
    //        for( urban::ResourceNetworkAttribute::CIT_ResourceNodes itResource = resourceNodes.begin(); itResource != resourceNodes.end(); ++itResource )
    //        {
    //            const urban::ResourceNetworkAttribute::ResourceNode& resourceNode = itResource->second;
    //            resourcesProd[ itResource->first ] += resourceNode.production_;
    //            resourceConso[ itResource->first ] += resourceNode.consumption_;
    //        }
    //    }
    //}

    //infos_.clear();
    //values_.clear();
    //if( selected.empty() )
    //    InsertLine( tr( "Total urban blocks number: " ), locale().toString( terrainVisitor.urbanBlocks_.size() ) );
    //else
    //    InsertLine( tr( "Selected urban blocks number: " ), locale().toString( terrainVisitor.urbanBlocks_.size() ) );

    //InsertLine();
    //InsertLine( tr( "Non medical infrastructures: " ), locale().toString( nonMedicalInfrastructures ) );
    //InsertLine( tr( "Medical infrastructures: " ), locale().toString( medicalInfrastructures ) );
    //InsertLine();
    //InsertLine( tr( "Total capacity: " ), locale().toString( totalCapacity ) );
    //for( std::map< std::string, unsigned int >::iterator it = motivationsCapacities.begin(); it != motivationsCapacities.end(); ++it )
    //    InsertLine( tr( "Capacity  %1: " ).arg( QString::fromStdString( it->first ) ), locale().toString( it->second ) );
    //InsertLine();
    //for( std::map< std::string, unsigned int >::iterator it = resourcesProd.begin(); it != resourcesProd.end(); ++it )
    //    InsertLine( tr( "Resource %1 production: " ).arg( QString::fromStdString( it->first ) ), locale().toString( it->second ) );
    //InsertLine();
    //for( std::map< std::string, unsigned int >::iterator it = resourceConso.begin(); it != resourceConso.end(); ++it )
    //    InsertLine( tr( "Resource %1 consumption: " ).arg( QString::fromStdString( it->first ) ), locale().toString( it->second ) );

    //assert( infos_.size() == values_.size() );
    //QString info;
    //int maxSize = -1;
    //for( int i = 0; i < infos_.size(); ++i )
    //    maxSize = std::max< int >( maxSize, infos_[ i ].size() );
    //for( int i = 0; i < infos_.size(); ++i )
    //{
    //    info += infos_[ i ];
    //    if( !values_[ i ].isEmpty() )
    //    {
    //        for( int j = infos_[ i ].size(); j < maxSize; ++j )
    //            info += " ";
    //        info += values_[ i ];
    //    }
    //    info += "\n";
    //}
    //infoEditText_->setText( info );
}
