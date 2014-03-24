// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __ObjectAttributes_h_
#define __ObjectAttributes_h_

void CheckCount( const std::string& name, const sword::MissionParameter_Value& value, int count );
bool GetBool( const std::string& name, const sword::MissionParameter_Value& value, unsigned int i );
int GetQuantity( const std::string& name, const sword::MissionParameter_Value& value, unsigned int i );
float GetReal( const std::string& name, const sword::MissionParameter_Value& value, unsigned int i );

#endif // __ObjectAttributes_h_
