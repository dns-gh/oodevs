// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef HELPERS_H__
#define HELPERS_H__

#define COUNT_OF(X) (sizeof(X)/sizeof*(X))
#define Q8(X) QString::fromUtf8(X)
#define C8(X) (X).toUtf8().constData()

#endif // HELPERS_H__