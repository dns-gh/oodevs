// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __statusicons_h_
#define __statusicons_h_

#define DECLARE_ICON( name )   extern const char *xpm_##name[]

DECLARE_ICON( radars_on );
DECLARE_ICON( skull );
DECLARE_ICON( talkie_interdit );
DECLARE_ICON( brouillage );
DECLARE_ICON( gas );
DECLARE_ICON( cadenas );
DECLARE_ICON( nbc );
DECLARE_ICON( medical );
DECLARE_ICON( maintenance );
DECLARE_ICON( supply );
DECLARE_ICON( dotations );
DECLARE_ICON( lendborrow );
DECLARE_ICON( components );
DECLARE_ICON( reinforced );
DECLARE_ICON( reinforcing );
DECLARE_ICON( morale_fanatic );
DECLARE_ICON( morale_good );
DECLARE_ICON( morale_medium );
DECLARE_ICON( morale_bad );
DECLARE_ICON( experience_rookie );
DECLARE_ICON( experience_experienced );
DECLARE_ICON( experience_veteran );
DECLARE_ICON( tiredness_normal );
DECLARE_ICON( tiredness_tired );
DECLARE_ICON( tiredness_exhausted );
DECLARE_ICON( stance_move );
DECLARE_ICON( stance_discreet );
DECLARE_ICON( stance_stop );
DECLARE_ICON( stance_reflex );
DECLARE_ICON( stance_posted );
DECLARE_ICON( stance_equipped );
DECLARE_ICON( stance_engineered );
DECLARE_ICON( stance_arrow );
DECLARE_ICON( parent );
DECLARE_ICON( mission );
DECLARE_ICON( parameter );

// $$$$ SBO 2007-03-09: non status... rename file
DECLARE_ICON( checkpoint1 );
DECLARE_ICON( checkpoint2 );
DECLARE_ICON( checkpoint3 );

#endif // __statusicons_h_
