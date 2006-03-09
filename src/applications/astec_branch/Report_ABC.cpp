// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-08-04 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Report_ABC.cpp $
// $Author: Ape $
// $Modtime: 4/08/04 11:57 $
// $Revision: 1 $
// $Workfile: Report_ABC.cpp $
//
// *****************************************************************************
#include "astec_pch.h"
#include "Report_ABC.h"
#include "Agent_ABC.h"
#include "Attributes.h"
#include "Simulation.h"

// -----------------------------------------------------------------------------
// Name: Report_ABC constructor
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
Report_ABC::Report_ABC( const Agent_ABC& agent, const Simulation& simulation )
    : agent_  ( agent )
    , vPos_   ( agent_.Get< Attributes >().vPos_ )
    , nTime_  ( simulation.GetTime() )
    , bNew_   ( true )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: Report_ABC destructor
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
Report_ABC::~Report_ABC()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: Report_ABC::GetPos
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
MT_Vector2D Report_ABC::GetPos() const
{
    return vPos_;
}
    

// -----------------------------------------------------------------------------
// Name: Report_ABC::GetTitle
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
std::string Report_ABC::GetTitle() const
{
    return strTitle_;
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::GetStrippedTitle
// Created: HME 2005-11-29
// -----------------------------------------------------------------------------
std::string Report_ABC::GetStrippedTitle() const
{
    std::string strTitle = strTitle_;
    uint pos = strTitle_.find( " - " );
    if( pos >= 0 && pos < strTitle.size() )
        strTitle.resize( pos );
    return strTitle;
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::IsInteresting()
// Created: HME 2006-01-13
// -----------------------------------------------------------------------------
bool Report_ABC::IsInteresting() const
{
    return strTitle_ != GetStrippedTitle()
        || strTitle_ == "Ennemi détecté"
        || strTitle_ == "Ennemi reconnu"
        || strTitle_ == "Ennemi identifié";
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::GetTime
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
int Report_ABC::GetTime() const
{
    return nTime_;
}


// -----------------------------------------------------------------------------
// Name: Report_ABC::IsNew
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
bool Report_ABC::IsNew() const
{
    return bNew_;
}
    

// -----------------------------------------------------------------------------
// Name: Report_ABC::SetNew
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
void Report_ABC::SetNew( bool bNew )
{
    bNew_ = bNew;
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::GetType
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
Report_ABC::E_Type Report_ABC::GetType() const
{
    return eType_;
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::IsRCType
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
bool Report_ABC::IsRCType() const
{
    return eType_ == eRC || eType_ == eMessage || eType_ == eEvent || eType_ == eWarning;
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::GetAgent
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
const Agent_ABC& Report_ABC::GetAgent() const
{
    return agent_;
}
