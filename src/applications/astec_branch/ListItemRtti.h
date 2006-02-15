// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ListItemRtti_h_
#define __ListItemRtti_h_

// =============================================================================
/** @class  ListItemRtti
    @brief  ListItemRtti
*/
// Created: AGE 2006-02-15
// =============================================================================
template< typename T >
struct ListItemRtti
{
    // NOTHING
};

class Team;
template< >
struct ListItemRtti< const Team* > {
    enum { rtti = 1001 };
};

class KnowledgeGroup;
template< >
struct ListItemRtti< const KnowledgeGroup* > {
    enum { rtti = 1002 };
};


#endif // __ListItemRtti_h_
