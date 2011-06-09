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

// $$$$ SBO 2007-03-09: non status... rename file

#define DECLARE_ICON( name )   extern const char *xpm_##name[]

DECLARE_ICON( radars_on );
DECLARE_ICON( skull );
DECLARE_ICON( silence_radio_incoming );
DECLARE_ICON( silence_radio_outgoing );
DECLARE_ICON( brouillage );
DECLARE_ICON( gas );
DECLARE_ICON( cadenas );
DECLARE_ICON( nbc );
DECLARE_ICON( underground );
DECLARE_ICON( reinforced );
DECLARE_ICON( reinforcing );
DECLARE_ICON( parent );
DECLARE_ICON( mission );
DECLARE_ICON( parameter2 );
DECLARE_ICON( checkpoint );
DECLARE_ICON( checkpoint1 );
DECLARE_ICON( checkpoint2 );
DECLARE_ICON( checkpoint3 );
DECLARE_ICON( activated );
DECLARE_ICON( not_activated );
DECLARE_ICON( replayer );
DECLARE_ICON( timer );
DECLARE_ICON( trash2 );
DECLARE_ICON( aaa_function_copy );
DECLARE_ICON( aaa_function_edit );
DECLARE_ICON( aaa_function_delete );
DECLARE_ICON( aaa_function_new );
DECLARE_ICON( aaa_broken );
DECLARE_ICON( aaa_pending );
DECLARE_ICON( aaa_valid );
DECLARE_ICON( aaa_link );
DECLARE_ICON( aaa_move );

#endif // __statusicons_h_
