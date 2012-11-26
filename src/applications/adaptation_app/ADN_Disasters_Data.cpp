// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Disasters_Data.h"
#include "ADN_Project_Data.h"
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::NbcSuitRatioInfos
// Created: LGY 2012-11-23
// -----------------------------------------------------------------------------
ADN_Disasters_Data::NbcSuitRatioInfos::NbcSuitRatioInfos( const E_AgentNbcSuit& eType )
    : eType_ ( eType )
    , rCoeff_( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::~NbcSuitRatioInfos
// Created: LGY 2012-11-23
// -----------------------------------------------------------------------------
ADN_Disasters_Data::NbcSuitRatioInfos::~NbcSuitRatioInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::ReadArchive
// Created: LGY 2012-11-23
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::NbcSuitRatioInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::WriteArchive
// Created: LGY 2012-11-23
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::NbcSuitRatioInfos::WriteArchive( xml::xostream& output ) const
{
    if( eType_ != eAgentNone )
    {
        output << xml::start( "protection" )
                   << xml::attribute( "type", ENT_Tr::ConvertFromAgentNbcSuit( eType_ ) )
                   << xml::attribute( "value", rCoeff_ )
                << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::AttritionThresholdInfos
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
ADN_Disasters_Data::AttritionThresholdInfos::AttritionThresholdInfos()
    : threshold_( 0. )
    , u1_       ( 0 )
    , u2_       ( 0 )
    , u3_       ( 0 )
    , ue_       ( 0 )
    , dead_     ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::~AttritionThresholdInfos
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
ADN_Disasters_Data::AttritionThresholdInfos::~AttritionThresholdInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::CreateCopy
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
ADN_Disasters_Data::AttritionThresholdInfos* ADN_Disasters_Data::AttritionThresholdInfos::CreateCopy()
{
    AttritionThresholdInfos* pCopy = new AttritionThresholdInfos();
    pCopy->threshold_ = threshold_.GetData();
    pCopy->name_ = name_.GetData();
    pCopy->u1_ = u1_.GetData();
    pCopy->u2_ = u1_.GetData();
    pCopy->u3_ = u3_.GetData();
    pCopy->ue_ = ue_.GetData();
    pCopy->dead_ = dead_.GetData();
    return pCopy;
}

namespace
{
    float SaveWound( int value )
    {
        return static_cast< float >( value / 100. );
    }
    int LoadWound( float value )
    {
        return static_cast< int >( value * 100u );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::ReadArchive
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::AttritionThresholdInfos::ReadArchive( xml::xistream& input )
{
    float u1, u2, u3, ue, dead;
    input >> xml::attribute( "value", threshold_ )
          >> xml::attribute( "u1", u1 )
          >> xml::attribute( "u2", u2 )
          >> xml::attribute( "u3", u3 )
          >> xml::attribute( "ue", ue )
          >> xml::attribute( "dead", dead )
          >> xml::optional
            >> xml::attribute( "name", name_ );
    u1_ = LoadWound( u1 );
    u2_ = LoadWound( u2 );
    u3_ = LoadWound( u3 );
    ue_ = LoadWound( ue );
    dead_ = LoadWound( dead );
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::WriteArchive
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::AttritionThresholdInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "threshold" )
               << xml::attribute( "value", threshold_ )
               << xml::attribute( "u1", SaveWound( u1_.GetData() ) )
               << xml::attribute( "u2", SaveWound( u2_.GetData() ) )
               << xml::attribute( "u3", SaveWound( u3_.GetData() ) )
               << xml::attribute( "ue", SaveWound( ue_.GetData() ) )
               << xml::attribute( "dead", SaveWound( dead_.GetData() ) );
    if( !name_.GetData().empty() )
    output     << xml::attribute( "name", name_ );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::ConcentrationThresholdInfos
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
ADN_Disasters_Data::ConcentrationThresholdInfos::ConcentrationThresholdInfos()
    : threshold_( 0. )
    , color_    ( "0x000000" )
    , name_     ( "" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::~ConcentrationThresholdInfos
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
ADN_Disasters_Data::ConcentrationThresholdInfos::~ConcentrationThresholdInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::CreateCopy
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
ADN_Disasters_Data::ConcentrationThresholdInfos* ADN_Disasters_Data::ConcentrationThresholdInfos::CreateCopy()
{
    ConcentrationThresholdInfos* pCopy = new ConcentrationThresholdInfos();
    pCopy->color_ = color_.GetData();
    pCopy->threshold_ = threshold_.GetData();
    pCopy->name_ = name_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::ReadArchive
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::ConcentrationThresholdInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", threshold_ )
          >> xml::attribute( "color", color_ )
          >> xml::optional
            >> xml::attribute( "name", name_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::WriteArchive
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::ConcentrationThresholdInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "threshold" )
               << xml::attribute( "value", threshold_ )
               << xml::attribute( "color", color_ );
    if( !name_.GetData().empty() )
        output     << xml::attribute( "name", name_ );
    output << xml::end;
}

//-----------------------------------------------------------------------------
// Name: DisasterInfos::DisasterInfos
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
ADN_Disasters_Data::DisasterInfos::DisasterInfos()
{
    for( unsigned int i = 0; i< eNbrAgentNbcSuit; ++i )
    {
        NbcSuitRatioInfos* pInfo = new NbcSuitRatioInfos( static_cast< E_AgentNbcSuit >( i ) );
        nbcSuitRatio_.AddItem( pInfo );
    }
}

//-----------------------------------------------------------------------------
// Name: DisasterInfos::DisasterInfos
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
ADN_Disasters_Data::DisasterInfos::~DisasterInfos()
{
    concentrationThresholds_.Reset();
    attritionThresholds_.Reset();
    nbcSuitRatio_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::CreateCopy
// Created: LGY 2012-11-15
// -----------------------------------------------------------------------------
ADN_Disasters_Data::DisasterInfos* ADN_Disasters_Data::DisasterInfos::CreateCopy()
{
    DisasterInfos* pCopy = new DisasterInfos();
    for( IT_ConcentrationThresholdInfosVector it = concentrationThresholds_.begin(); it != concentrationThresholds_.end(); ++it )
    {
        ConcentrationThresholdInfos* pNew = (*it)->CreateCopy();
        pCopy->concentrationThresholds_.AddItem( pNew );
    }
    for( IT_AttritionThresholdInfosVector it = attritionThresholds_.begin(); it != attritionThresholds_.end(); ++it )
    {
        AttritionThresholdInfos* pNew = (*it)->CreateCopy();
        pCopy->attritionThresholds_.AddItem( pNew );
    }

    for( unsigned int i= 0 ; i< eNbrAgentNbcSuit; ++i )
        pCopy->nbcSuitRatio_[ i ]->rCoeff_ = nbcSuitRatio_[ i ]->rCoeff_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: DisasterInfos::ReadArchive
// Created: LGY 2012-11-13
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::DisasterInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", strName_ )
              >> xml::start( "contamination" )
                  >> xml::list( "threshold", *this, &ADN_Disasters_Data::DisasterInfos::ReadContamination )
              >> xml::end
              >> xml::start( "attrition" )
                  >> xml::list( "threshold", *this, &ADN_Disasters_Data::DisasterInfos::ReadAttrition )
              >> xml::end
              >> xml::start( "protections" )
                  >> xml::list( "protection", *this, &ADN_Disasters_Data::DisasterInfos::ReadProtection )
              >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::ReadProtection
// Created: LGY 2012-11-23
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::DisasterInfos::ReadProtection( xml::xistream& input )
{
    const std::string type = input.attribute< std::string >( "type" );
    for( unsigned int i = 0; i < eNbrAgentNbcSuit; ++i )
        if( ENT_Tr::ConvertFromAgentNbcSuit( E_AgentNbcSuit( i ) ) == type )
        {
            nbcSuitRatio_.at( i )->ReadArchive( input );
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::ReadAttrition
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::DisasterInfos::ReadAttrition( xml::xistream& input )
{
    std::auto_ptr< AttritionThresholdInfos > spNew( new AttritionThresholdInfos() );
    spNew->ReadArchive( input );
    attritionThresholds_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::ReadContamination
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::DisasterInfos::ReadContamination( xml::xistream& input )
{
    std::auto_ptr< ConcentrationThresholdInfos > spNew( new ConcentrationThresholdInfos() );
    spNew->ReadArchive( input );
    concentrationThresholds_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: DisasterInfos::WriteArchive
// Created: LGY 2012-11-13
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::DisasterInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "disaster" )
               << xml::attribute( "name", strName_ )
                   << xml::start( "contamination" );
    for( IT_ConcentrationThresholdInfosVector it = concentrationThresholds_.begin(); it != concentrationThresholds_.end(); ++it )
        (*it)->WriteArchive( output );
    output        << xml::end
                  << xml::start( "attrition" );
    for( IT_AttritionThresholdInfosVector it = attritionThresholds_.begin(); it != attritionThresholds_.end(); ++it )
        (*it)->WriteArchive( output );
    output        << xml::end
                  << xml::start( "protections" );
    for( IT_NbcSuitRatioInfosVector it = nbcSuitRatio_.begin(); it != nbcSuitRatio_.end(); ++it )
        (*it)->WriteArchive( output );
    output        << xml::end
         << xml::end;
}

//-----------------------------------------------------------------------------
// Name: ADN_Disasters_Data constructor
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
ADN_Disasters_Data::ADN_Disasters_Data()
    : ADN_Data_ABC()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Disasters_Data destructor
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
ADN_Disasters_Data::~ADN_Disasters_Data()
{
    Reset();
}

//-----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::FilesNeeded
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
void ADN_Disasters_Data::FilesNeeded( T_StringList& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szDisasters_.GetData() );
}

//-----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::Reset
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
void ADN_Disasters_Data::Reset()
{
    vDisasters_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::ReadArchive
// Created: LGY 2012-11-13
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "disasters" )
              >> xml::list( "disaster", *this, &ADN_Disasters_Data::ReadDisaster )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::ReadDisaster
// Created: LGY 2012-11-15
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::ReadDisaster( xml::xistream& input )
{
    std::auto_ptr< DisasterInfos > spNew( new DisasterInfos() );
    spNew->ReadArchive( input );
    vDisasters_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::WriteArchive
// Created: LGY 2012-11-13
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "disasters" );
    ADN_Tools::AddSchema( output, "Disasters" );
    for( IT_DisasterInfos_Vector it = vDisasters_.begin(); it != vDisasters_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}
