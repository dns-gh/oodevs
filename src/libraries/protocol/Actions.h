// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathematiques Appliquees SA (MASA)
//
// *****************************************************************************

#ifndef PROTOCOL_ACTIONS_H
#define PROTOCOL_ACTIONS_H

namespace protocol
{
namespace mapping
{

struct ActionEnum
{
    const char* name;
    const int* values;
    size_t valuesCount;
};

struct Action;
struct ActionParam;

struct ActionUnionType
{
    int id;
    const char* name;
    const ActionParam* params;
    size_t paramsCount;
};

struct ActionUnion
{
    const ActionUnionType* types;
    size_t typesCount;
};

struct ActionParam
{
    const char* name;
    const char* type;
    const ActionEnum* enumeration;        // mandatory if type == "enumeration"
    const Action* list;                   // mandatory if type == "list" && is a dynamic list
    const Action* structure;              // mandatory if type == "list" && is a structure
    const ActionUnion* structure_union;   // mandatory if type == "list" && can be different structures
};

struct Action
{
    const char* name;
    const ActionParam* params;
    size_t paramsCount;
};

extern const ActionEnum* enums[];
extern const size_t enumsCount;

extern const Action actions[];
extern const size_t actionsCount;

}  // namespace mapping
}  // namespace protocol

#endif // PROTOCOL_ACTION_H
