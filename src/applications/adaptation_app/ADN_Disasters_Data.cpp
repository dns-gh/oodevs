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

//-----------------------------------------------------------------------------
// Name: DisasterInfos::DisasterInfos
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
ADN_Disasters_Data::DisasterInfos::DisasterInfos()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: DisasterInfos::DisasterInfos
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
ADN_Disasters_Data::DisasterInfos::~DisasterInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::CreateCopy
// Created: LGY 2012-11-15
// -----------------------------------------------------------------------------
ADN_Disasters_Data::DisasterInfos* ADN_Disasters_Data::DisasterInfos::CreateCopy()
{
    DisasterInfos* pCopy = new DisasterInfos();
    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: DisasterInfos::ReadArchive
// Created: LGY 2012-11-13
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::DisasterInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", strName_ );
}

// -----------------------------------------------------------------------------
// Name: DisasterInfos::WriteArchive
// Created: LGY 2012-11-13
// -----------------------------------------------------------------------------
void ADN_Disasters_Data::DisasterInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "disaster" )
               << xml::attribute( "name", strName_ )
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
