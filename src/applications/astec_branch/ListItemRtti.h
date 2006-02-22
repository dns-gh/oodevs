// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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

class Agent;
template< >
struct ListItemRtti< const Agent* > {
    enum { rtti = 1003 };
};

class ObjectType;
template< >
struct ListItemRtti< const ObjectType* > {
    enum { rtti = 1004 };
};

class Object_ABC;
template< >
struct ListItemRtti< const Object_ABC* > {
    enum { rtti = 1005 };
};

class Population;
template< >
struct ListItemRtti< const Population* > {
    enum { rtti = 1006 };
};

class PopulationPart_ABC;
template< >
struct ListItemRtti< const PopulationPart_ABC* > {
    enum { rtti = 1007 };
};

class Dotation;
template< >
struct ListItemRtti< const Dotation* > {
    enum { rtti = 1008 };
};

class Equipment;
template< >
struct ListItemRtti< const Equipment* > {
    enum { rtti = 1009 };
};

class Lend;
template< >
struct ListItemRtti< Lend > { // $$$$ AGE 2006-02-21: copi�s ici car temporaires
    enum { rtti = 1010 };
};

#include "Troops.h" // $$$$ AGE 2006-02-21: crap
template< >
struct ListItemRtti< Troops::Humans > { // $$$$ AGE 2006-02-21: copi�s ici car temporaires
    enum { rtti = 1011 };
};

class AgentKnowledge;
template< >
struct ListItemRtti< const AgentKnowledge* > {
    enum { rtti = 1012 };
};

class Perception;
template< >
struct ListItemRtti< Perception > {
    enum { rtti = 1013 };
};

#endif // __ListItemRtti_h_
