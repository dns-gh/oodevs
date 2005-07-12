#ifndef VMAPCONV_H
#define VMAPCONV_H

#include "MOS_Types.h"

#define UTM_TRANSFO		// Si l'on travail en UTM
#define MGRS_TRANSFO	// Si l'on travail en MGRS 

#define RADDEG	(3.14159265358979323846/180.0)

#define SPECCODE	( params->wgs7284 )

#define dtcc_set_coord3_cart( c3P, u,v,w )    dtcc_set_coord3( c3P, u,v,w, COORD3_XYZ )
#define RADDM			(-2.0)
#define RADDMS			(-3.0)
#define RADMGR			(-4.0)    
#define RADGRS			(-5.0)    

#define DX66	(4)
#define DY66	(3)
#define DZ66	(-7)
#define A72		( 6378135.000 )
#define RF72	( 	298.26 )

#define dtcc_swab_short( sP )    dtcc_swab2N((short*)(sP),1)
#define READ_INCOMPLETE                  (-4)
#define dtcc_swab_double8( dP )  dtcc_swab8N((long*)(dP),1)
#define dtcc_set_units_scaled( cuP, s ) 	( (cuP)->scaled = (s) )
/* Coordinate types */
/* 3D coordinate structure */
typedef enum
{	
	COORD3_UPS = 0,
	COORD3_UTM = 60,
	COORD3_MGRS,			/* encoded UTM */
	COORD3_GRID,			/* (east,north,h) m */
	COORD3_XYZ,				/* (x,y,z) m */
	COORD3_DAZM,			/* (range, azimuth, h ) m,d,m */
	COORD3_GEO,				/* ( lat, lon, h ) d,d,m */
	COORD3_GREF,			/* encoded geographic */ 
	COORD3_UNDEFINED = 255	/* not defined */
} COORD3_TYPES;

typedef enum 
{ 
	LINE=1, 
	AREA, 
	ANNO, 
	VPF_POINT, 
	COMPLEX=6 
}vpf_feature_type;	// VPF feature types

// Output format types
typedef enum
{
	M_VEC=1,
	VPF, 
	ASCII, 
	OPC, 
	MIF, 
	SVF,
	ARC_UNFER, 
	SHAPE, 
	VRML, 
	DXF, 
	GIF
} format_type;

typedef struct
{
   char 	           *db_path;
   char 	           *db_name;
   char 	           *lib_name;
   char 	           *cov_name;
   char 	           *cov_desc;
   char 	           *fclass;
   char 	           *expression;
   vpf_feature_type feature_type;
   format_type      output_format;
   double	        x_min;
   double	        x_max;
   double	        y_min;
   double	        y_max;
}USER_PARAMS;


typedef struct
	{	
		COORD3_TYPES	id;
		short			valid;
	} COORD_GROUP;

/* DTCC labels */
#define DTCC_LABEL_SIZE	(256)
typedef struct
	{	char 	labels[DTCC_LABEL_SIZE];
		short	l1,l2,l3;
		short	valid, type;
	} DTCC_LABELS;

typedef struct 
	{ 	DTCC_LABELS	label;       /* struct id */
		double axis, rflat, ecc; /* ellipsoid values */
	} ELLIPS_PARAMS;


#define _VDATUM_NAME_SIZE		(16)
typedef struct
{ 	
	DTCC_LABELS		label;
	char    		vname[_VDATUM_NAME_SIZE]; 	 /* name of vertical datum */
	void*			vdatum_fp;
    short 			shifts[3];                   /* shifts */
 	short 			sigmas[3];                   /* sigmas */
 	ELLIPS_PARAMS 	ellipsoid; 			 		 /* ellipsoid */
} DATUM_PARAMS;

typedef union
    {	double	c[3];
    	char	a[24];
    } coord3_data;

typedef struct
    {   coord3_data data;
    	short zone;
    } COORD3;

typedef struct
    {   double  dd;    	 	/* value of the angle in decimal degrees */
        double  sinv;     	/* sine of the angle */
        double  cosv;     	/* cosine of the angle */
        short	id;
    } ANGLE;

typedef struct
    {   COORD3       gridNE;  /* False northing & easting N&E */
        COORD3       gridSW;  /* False northing & easting S&W */
        COORD3       geo;     /* Latitude and longitude of origin */
        COORD3       scales;  /* Scale factors at the origin */
        ANGLE        w0;      /* Convergence of the meridians at the origin */
        ANGLE        azm0;    /* Azimuth at origin */
        short		 valid;
        short		 op_status; /* Status code for last operation */
    } COORD_ORIGIN;

#define _NOTHER	(16)
typedef struct 
	{	short	id;
		char 	name[32];
		double 	l0, p0,sinp0,cosp0;
		double	k0,a0, p1,p2; 
		double	axis,e,e2;
		double	glims[4];
		short	which, ncnsts;
		double	other[_NOTHER];
	}proj_dfn;

typedef struct
    {  DTCC_LABELS	  label;					 /* projection label  */
       ELLIPS_PARAMS  ellipsoid;                 /* ellipsoid  */ 
       COORD_ORIGIN   origin;                    /* origin */
       double         stdpars[2];                /* std parallels or DOUBLE_NULLS */
       short		  ncnsts;					 /* Number of constants */
       short          fixed;                  	 /* grid zone if grid is fixed, otherwise -1 */
       proj_dfn       cnstsP; 			 		 /* Computation constants */ 
       void*    	  engineP;                   /* Computational engine  */
    } PROJ_PARAMS;

typedef struct
   {   COORD3   	mn_grid;    /* Minimum values for each grid coordinate */
       COORD3   	mx_grid;    /* Maximum values for each grid coordinate */ 
       COORD3   	mn_geo;     /* Minimum values for each geo coordinate */
       COORD3   	mx_geo;     /* Maximum values for each geo coordinate */
       short		valid;
       short		op_status;  /* Status code for last operation */
       short		userdef;
   } COORD_EXTENTS;

typedef struct  
    {   DTCC_LABELS	label;
    	double 		cf;                      /* conversion factor to m or rads */
        short 		scaled;					 /* has the cf been scaled? */
    } UNITS_INFO;

/* Coordinate units info */
typedef struct 
    {   double 			map_scale;	   /* map scale ( 500000, 5000, etc) */
    	UNITS_INFO 		info[3];       /* Units parameters for coord 1 */
        COORD3_TYPES	mode;		   /* cartesian, geographic, gridref */
    } COORD3_UNITS;

typedef struct
{	
	DTCC_LABELS		label;
	COORD_GROUP		group;                       /* Coord group */
	DATUM_PARAMS	datums;					  	 /* Datums */
	PROJ_PARAMS		projection;					 /* Projection */
	COORD_EXTENTS   extents;					 /* Geo and metric coverage */
	COORD3_UNITS	units;					  	 /* Data exchange units */
	short			dimension;					 /* 2 or 3 dim system */
	char			user_data[64];				 /* Buffer for user's use */
} COORD_SYS;

typedef struct 
	{	COORD_SYS	systems[2];
		double		mpars[8];
		short		valid, valid_input, valid_output;
		short		dshift;
	} TRANSFORM_SYS;

typedef double GFLOAT;
typedef struct
{
    GFLOAT          left;	/* Leftmost extent of the dataset in DD   */
    GFLOAT          right;	/* Rightmost extent of the dataset in DD  */
    GFLOAT          bottom;	/* Bottommost extent of the dataset in DD */
    GFLOAT          top;	/* Topmost extent of the dataset in DD    */
}defEXTENT;

typedef enum
{
	KEEP_CURR_PREF=-1,
	OFFSET_AND_SCALE,
	CENTER_POINT_SCALE_IMAGE_EXTENT,
	GEO_EXTENT_SCALE,
	GEO_AND_IMAGE_EXTENT,
	MAP_AND_IMAGE_EXTENT
}  MAP_PREF;

typedef enum
{
    IMAGE_CM = 0,
    IMAGE_IN = 1,
    IMAGE_PIXELS = 2
} IMAGE_UNIT;    


typedef struct
{
    long magic;
    double pixels_per_meter_x;
    double pixels_per_meter_y;
    double image_height_pixels;
    double image_width_pixels;
    double scale_recip;
    double sx, sy, sz;
    double tx, ty, tz;
    double image_origin_x;
    double image_origin_y; 
    MAP_PREF map_pref;
    IMAGE_UNIT image_unit;
    COORD3 map_center, map_ll, map_ur;
    COORD3 map_center_display, map_ll_display, map_ur_display;
    double image_height_pixels_computed;
    double image_width_pixels_computed;
	double lon_shift_factor; /* 0, 360, or -360 */
	MAP_PREF default_map_pref;
} PLOT_PARAMS;    
#define VMAPOK			0
#define SHORT_NULL      (-32767)       				/* minimum short value */

#define dtcc_set_coord3_geo( c3P, u,v,w )		dtcc_set_coord3( c3P, u,v,w, COORD3_GEO )
#define dtcc_coord31( c3P )						dtcc_coord3K( c3P,0 )

#define dtcc_coord3_is_MGRS(id)			( (id) == COORD3_MGRS )
#define DOUBLE_NULL						(-1.7976931348623158e+308)		/* minimum double8 value */ 
#define dtcc_coord3_is_georef(id)		( (id) == COORD3_GREF )
#define dtcc_coord3_is_encoded(id)		(	dtcc_coord3_is_MGRS(id) || dtcc_coord3_is_georef(id) )
#define dtcc_coord3Z( c3P )				( (c3P) ? (c3P)->zone : SHORT_NULL )
#define dtcc_coord3K( c3P,k )			(	!dtcc_coord3_is_encoded(dtcc_coord3Z(c3P)) ? (c3P)->data.c[k] : DOUBLE_NULL )
#define dtcc_coord32( c3P )					dtcc_coord3K( c3P,1 )
#define dtcc_coord3_is_grid(id)			( (id) <= COORD3_GRID )
#define dtcc_is_coord_group_grid( cgP )		dtcc_coord3_is_grid( dtcc_coord_group_id(cgP) )
#define dtcc_system_groupP(csP)			((const COORD_GROUP*)( (csP) ? &((csP)->group) : NULL ))
#define dtcc_is_system_grid(csP)			dtcc_is_coord_group_grid( dtcc_system_groupP(csP) )
#define dtcc_coord_group_id(cgP)		( (cgP) ? (cgP)->id : COORD3_UNDEFINED )
#define dtcc_coord3_north( c3P )			dtcc_coord32( c3P )
#define dtcc_coord3_east( c3P )				dtcc_coord31( c3P )
#define ANIOTA                   1.0E-4
#define ILLEGAL_ARG                      (-11)
#define COORD_NOT_DEFINED				(-29)
#define _COORD31(c3P)		 ((c3P)->data.c[0]) 
#define _COORD32(c3P)		 ((c3P)->data.c[1]) 
#define _COORD33(c3P)		 ((c3P)->data.c[2])
#define _COORD3Z(c3P)		 ((c3P)->zone) 

#define _INPUTP(tsP)		( &( (tsP)->systems[0] ) )
#define _OUTPUTP(tsP)		( &( (tsP)->systems[1] ) )
#define _SYSTEM_DATUMP(csP)			( &((csP)->datums) )
#define dtcc_coord3_is_geo(id)		( (id) == COORD3_GEO )
#define _DOMOLO(tsP)		(  (tsP)->dshift == 1 )
#define dtcc_coord33( c3P )          dtcc_coord3K( c3P,2 )
#define dtcc_coord3_elv( c3P )        dtcc_coord33( c3P )
#define dtcc_system_mode(csP) 			dtcc_coord_group_id( dtcc_system_groupP(csP) )		 
#define dtcc_system_unitsP( csP )		((const COORD3_UNITS*)( (csP) ? &((csP)->units) : NULL ))
#define dtcc_system_extentsP( csP )	 	((const COORD_EXTENTS*)( (csP) ? &((csP)->extents) : NULL ))
#define amsl_to_ellipsoidal(ghgt,h)		geoid_ellipsoid( ghgt,h,0)
#define dtcc_coord3_lat( c3P )        dtcc_coord31( c3P )
#define dtcc_coord3_lon( c3P )        dtcc_coord32( c3P )
#define _MPARS(tsP)			(  (const void*)((tsP)->mpars )	)
#define ellipsoidal_to_amsl(ghgt,h) geoid_ellipsoid( ghgt,h,1)
#define dtcc_inverse_projection( ppP, c3FromP, c3ToP)  _projection( ppP, DTCC_PROJECTION_INVERSE, c3FromP,1L, c3ToP )
#define dtcc_system_projectionP(csP) ((const PROJ_PARAMS*)( (csP) ? &((csP)->projection) : NULL ))
#define DTCC_PROJECTION_INVERSE    		(2)        /* Grid to geographics */
#define dtcc_coord3_is_xyz(id)		( (id) == COORD3_XYZ )
#define dtcc_datum_ellipsoidP(dpP)		( (dpP) ? (const ELLIPS_PARAMS*)_DATUM_ELLIPSOIDP(dpP) : NULL )
#define _DATUM_ELLIPSOIDP(dpP)	( &( (dpP)->ellipsoid ) )
#define dtcc_system_ellipsoidP( csP )	( (csP) ? dtcc_datum_ellipsoidP(&(csP)->datums) : NULL )
#define ITEM_NOT_FOUND                   (-13)
#define _DATUM_VFILEP(dpP)		( (dpP)->vdatum_fp ) 

typedef struct
    {   COORD3*      dataP;  	/* Data */ 
        long         n;      	/* Number of Coordinates in the array */
        short   	 type;	 	/* Coordinate type (cart,geo,grid) */
        short		 id;		/* COORD3N id number */
    } COORD3N;

#define _label_nameP(idP)		( (idP)->labels )

#define _COORD3_PICK_UNITS(cuP,n) ( ((cuP)->info)+(n) )
#define _COORD3_UNITSNP( cuP,n )  ( (cuP) ? _COORD3_PICK_UNITS(cuP,n) : NULL )			
#define dtcc_units_cf(cuP)				( (cuP) ? (cuP)->cf : DOUBLE_NULL  )
#define dtcc_coord3_unitsNP( cuP, N)	(const UNITS_INFO*)_COORD3_UNITSNP(cuP,N)
#define dtcc_units_are_numeric( cuP )     ( (cuP)->cf > 0.0 )
#define dtcc_set_coord3_grid( c3P, u,v,w, k ) dtcc_set_coord3( c3P, u,v,w, ((short)( (k) <= COORD3_UTM && (k) >= COORD3_UPS ? (k) : COORD3_GRID ) ))
#define dtcc_coord3_zone( c3P )       (short)dtcc_coord3Z(c3P)
#define COPY_ERROR                       (-35)
#define dtcc_coord3_is_defined(id)  ( (id) <= COORD3_GREF && (id) >= COORD3_UPS )

#define _EXTENTS_MAX_GEOP( ceP )      	  ( (ceP) ? &((ceP)->mx_geo) : NULL )
#define dtcc_extents_max_geoP(ceP)	  ((const COORD3*)_EXTENTS_MAX_GEOP(ceP))
#define dtcc_coord3_is_numeric(id)  ( dtcc_coord3_is_defined(id) && !dtcc_coord3_is_encoded(id) )
#define dtcc_extents_max_lat( ceP )  dtcc_coord3_lat( dtcc_extents_max_geoP(ceP) ) 
#define _EXTENTS_MIN_GEOP( ceP )          ( (ceP) ? &((ceP)->mn_geo) : NULL )
#define dtcc_extents_min_geoP(ceP)	  ((const COORD3*)_EXTENTS_MIN_GEOP(ceP))
#define dtcc_extents_min_lat( ceP )   dtcc_coord3_lat( dtcc_extents_min_geoP(ceP) ) 
#define dtcc_extents_min_lon( ceP )   dtcc_coord3_lon( dtcc_extents_min_geoP(ceP) ) 
#define dtcc_extents_max_lon( ceP )   dtcc_coord3_lon( dtcc_extents_max_geoP(ceP) )
#define _EXTENTS_MIN_GRIDP( ceP )     	  ( (ceP) ? &((ceP)->mn_grid) : NULL )
#define _EXTENTS_MAX_GRIDP( ceP )     	  ( (ceP) ? &((ceP)->mx_grid) : NULL  )
#define dtcc_extents_max_east( ceP )      dtcc_coord3_east( _EXTENTS_MAX_GRIDP(ceP) ) 
#define dtcc_extents_max_north( ceP )     dtcc_coord3_north( _EXTENTS_MAX_GRIDP(ceP) ) 
#define dtcc_extents_min_north( ceP )     dtcc_coord3_north( _EXTENTS_MIN_GRIDP(ceP) ) 
#define dtcc_extents_min_east( ceP )      dtcc_coord3_east( _EXTENTS_MIN_GRIDP(ceP) ) 
#define dtcc_extents_min_east( ceP )      dtcc_coord3_east( _EXTENTS_MIN_GRIDP(ceP) ) 
#define _DATUM_VNAMEP(dpP)		( (dpP)->vname ) 
#define DTCC_PROJECTION_FORWARD			(1)        /* Geographics to grid */
#define dtcc_forward_projection( ppP, c3FromP, c3ToP )  _projection( ppP, DTCC_PROJECTION_FORWARD, c3FromP,1L, c3ToP )
#define STRUCTURE_INVALID				 (-30)
typedef short  (*DTCC_ENGINE)( const PROJ_PARAMS* proP, short mode, const COORD3* fromP, COORD3* toP );
#define _PROJECTION_ENGINE( ppP )     		( (ppP)->engineP )
#define dtcc_ellipsoid_axis(ep)   		( (ep) ? (ep)->axis : DOUBLE_NULL ) 
#define dtcc_ellipsoid_ecc2(ep)   		( (ep) ? (ep)->ecc*(ep)->ecc : DOUBLE_NULL )

#define dtcc_coord3_x( c3P )          dtcc_coord31( c3P )
#define dtcc_set_coord3_x( c3P,v )	  dtcc_set_coord31( c3P,v )
#define dtcc_coord3_y( c3P )          dtcc_coord32( c3P )
#define dtcc_set_coord3_y( c3P,v )	  dtcc_set_coord32( c3P,v )
#define dtcc_coord3_z( c3P )          dtcc_coord33( c3P )
#define dtcc_set_coord3_z( c3P,v )	  dtcc_set_coord33( c3P,v )
#define _COORD3N_KP( c3NP,k )         ( (k) >= 0 && (k) <(c3NP)->n ? (c3NP)->dataP + (k) : NULL )
#define dtcc_coord3N_count( c3NP )     ( (c3NP) ? (c3NP)->n : SHORT_NULL  )
#define   FROW_SIZE		(721L)
#define   BYTEBIAS    	(128)				   

struct TRANSFORM_PARAMS
	{	double from_a, from_f;	/* parameters of "from" ellipsoid */
		double to_a,to_f;   	/* "to" - "from" parameters */
		short  from_shifts[3];	/* translations */
		short  to_shifts[3];	/* translations */
		short  from_sigmas[3];	/* errors */
		short  to_sigmas[3];	/* errors */
		short  wgs7284;			/* WGS 72 <-> WGS 84 is a special case */
		FILE*  geoidfp;			/* File containing 30' x 30' geoid hgts */
	};


#define dtcc_units_fmt(cuP)				dtcc_otherP(cuP)
#define _label_otherP(idP)		( (idP)->labels + (idP)->l1 + (idP)->l2 )
#define _label_abbrvP(idP)		( (idP)->labels + (idP)->l1 )

#define PSTYLE_HOLLOW 0 
#define PSTYLE_SOLID  1
#define PSTYLE_PAT	  2
#define PSTYLE_HATCH  3
#define PSTYLE_EMPTY  4

#define  MAGIC_MAPGEO                  	  92000016L

#define dtcc_system_projection_nameP(csP)		dtcc_nameP(dtcc_system_projectionP(csP))			

typedef enum
{
    PROJ_UNDEFINED,
    PROJ_LAMBERT1,
    PROJ_LAMBERT2,
    PROJ_LINEAR,
    PROJ_MERCATOR,
    PROJ_UPS,
    PROJ_UTM,
    PROJ_GRAPH,
    PROJ_POLAR,
    PROJ_ARC,
    PROJ_TS
} defPROJECTION;

typedef enum
{
    NDEFINED,
    HD_WGS84,
    HD_WGS72,
    HD_NAD27,
    HD_EUD50
}HOR_DATUM;

typedef enum
{
    UDEFINED,
    VD_ELIPSOID,
    VD_MSL
}VER_DATUM;

typedef struct
{
    HOR_DATUM       hor_datum;
    VER_DATUM       ver_datum;
}DATUM;

typedef enum
{
    /*DD,*/defDDTEMP,
    defDMS,
    /*GEOREF,*/
    defMGRS,
    defMIN,
    defSECS,
    defUTM
} defGEOGRAPHIC;

typedef enum
{
    VER_METERS,
    VER_FATHOMS,
    VER_FEET
}VER_DISTANCE;

typedef struct
{
    GFLOAT          lon;
    GFLOAT          lat;
    GFLOAT          height;
}               POINT_DD;

typedef struct
{
    GFLOAT          rel_hor;	/* relative horizontal accuracy */
    GFLOAT          abs_hor;	/* absolute horizontal accuracy */
    GFLOAT          rel_ver;	/* relative data value accuracy */
    GFLOAT          abs_ver;	/* absolute data value accuracy */
}               ACCURACY;

typedef struct
{
    char            year[3];
    char            month[3];
    char            day[3];
    char            hour[3];
    char            minute[3];
    char            second[3];
}               MDATE;

typedef struct
{
    char            source[30];	/* Source of the data                  */
    defGEOGRAPHIC   h_units;	/* Horizonal units of measurement      */
    VER_DISTANCE    v_units;	/* Vertical units of measurement       */
    GFLOAT          x_res;	/* X resoultion of the data in h_units */
    GFLOAT          y_res;	/* Y Resolution of the data in h_units */
    defPROJECTION      projection;	/* Projection name of the data         */
    POINT_DD        center;	/* Center of the data in geographics   */
    defEXTENT          extent;	/* Bounding geocoordinates of the data */
    DATUM           datum;	/* Horizonal and vertical datum        */
    ACCURACY        accuracy;	/* Accuricy of the data                */
    MDATE            date;	/* Date the data was created/extracted */
    GFLOAT          zoom;	/* 0->1 for downsampling, >1 for
				 * interpolation */
    GFLOAT          data_min;	/* minimum data value */
    GFLOAT          data_max;	/* maximum data value */
}               DATA_HDR;

typedef enum
{
    ONEBIT,			/* binary data (0 or 1) */
    UINT_4BIT,			/* unsigned 4 bit data (0 -> 15) */
    SINT_8BIT,			/* signed 8 bit data (-128 -> 127) */
    UINT_8BIT,			/* unsigned 8 bit data (0 -> 255) */
    SINT_16BIT,			/* signed 16 bit (-32768 -> 32767) */
    UINT_16BIT,			/* unsigned 16 bit ( 0 -> 64K) */
    DTED_16BIT,			/* Univac signed magnitude */
    RGB_24BIT,			/* RGB (0 -> 255) interleaved by pixel */
    RGB_48BIT,			/* RGB (0 -> 65535) interleaved by pixel */
    SINT_32BIT,			/* signed 32 bit */
    UINT_32BIT,			/* unsigned 32 bit */
    IEEE_32BIT,			/* same as float */
    IEEE_64BIT			/* same as double */
}
                NUM_TYPE;

typedef enum
{
    COLOR_INDIRECT, /* color index numbers */
    COLOR_RGB,      /* rgb values */
    DATA            /* non-color data values */
}  DATA_TYPE;

typedef int MUSE_HANDLE;

typedef struct
{
    long            width;	/* Num of columns */
    long            height;	/* Num of rows */
    long            bits_per_pixel;	/* Num bits for each matrix value */
    long            bytes_per_row;	/* All rows must be word(16 bit)
					 * alligned */
    NUM_TYPE        num_type;	/* Numeric type and size */
    DATA_TYPE       data_type;	/* Data or color type */
    MUSE_HANDLE		handle;
    void *data;
}               DATA_MATRIX;

typedef struct
{
    GFLOAT          sat;	/* 0.0 - 1.0 range */
    GFLOAT          val;	/* 0.0 - 1.0 range */
}               SHADE;


typedef struct
{
    long            num_hues;	/* number of color bands */
    long            num_shades;	/* number of shades of each color */
    GFLOAT          data_range[2];
    GFLOAT          hue_range[2];
    SHADE           shade_range[3];
}               LUT_DESC;

#define MAX_LUT 6

typedef struct
{
    long            magic;	/* LUT version id */
    LUT_DESC        lut_desc[MAX_LUT];	/* The lut descriptors */
    bool         stretch;	/* Fit Colors to LUT range */
    void            (*callback_when_complete)();/*Address of a function to call when view configuration is complete*/
    long            map_window;                 /* the Window ID of the caller*/
    bool         recently_changed;           /* true of the contents are newly modified*/
}               LUT;

typedef struct
{
    unsigned short			r;
    unsigned short			g;
    unsigned short			b;
}RGB;
typedef enum
{
    PAL_UNKNOWN,		// non-algorithmic palette- use min dist
    PAL_NUM_BINS,		// #bins for R, G, and B
    PAL_GREY			// equal R, G, B
}PAL_TYPE;

#define		MAX_COLORS			256       // Max colors that an image may contain

typedef short			ERRSTATUS;			// ERRSTATUS

// A set is represented as an array of characters with each character holding 8 bits of the set.
typedef struct
{
   long int size;
   unsigned char *buf;
}set_type;

typedef enum 
{ 
   DD,			// Decimal Degrees
   //AC,		// Albers Equal Area
   //AK,		// Azimuthal Equal Area
   AL,			// Azimuthal Equal Distance
   //GN,		// Gnomonic
   redoneLE,	//Lambert Conformal Conic
   //LJ,		//Lambert (Cylindrical) Equal Area
   MC,			//Mercator
   //OC,		//Oblique Mercator
   //OD,		//Orthographic
   //PG,		//Polar Stereographic
   TC,			//Transverse Mercator
   PS,			//UPS
   //RB,		//Hotine Oblique Mercator
   //PH,		//Polyconic
   //OS,		//Oblique Stereographic (code to-be-determined)
   //RC,		//Relative coordinates
   //UT,		//UTM
   PC			//Plate-Carree
}vpf_projection_code;

// Units of measure
typedef enum 
{ 
	UNKNOWN_UNITS, 
	METERS, 
	FEET, 
	INCHES, 
	KILOMETERS, 
	OTHER_UNITS, 
	DEC_DEGREES, 
	CENTIMETERS, 
	DECIMETERS,
	FATHOMS_AND_FEET,
	FATHOMS, 
	ARC_MINUTES, 
	STATUTE_MILES, 
	MILS, 
	MILLIMETERS, 
	NAUTICAL_MILES, 
	ARC_SECONDS, 
	MICROMETERS, 
	YARDS 
}vpf_units_type;


// VPF library internal structure
typedef struct
{
   char name[9];					// Name of the library
   bool viewable;				// Is this library accessible to the view?
   char *path;						// DOS path name to the library
   long int ntiles;					// Number of tiles in the library
   set_type tile_set;				// Set of 'active' tiles in the library
   vpf_projection_code projection;	// Projection of stored coord data
   vpf_units_type units;			// Units of the stored coordinate data
}library_type;


// VPF database internal structure
typedef struct
{                     
   char name[9];					// Name of the VPF database
   char *path;						// UNIX path name to the database
   library_type *library;			// Array of library structures for the database
   long int  nlibraries;			// Number of libraries in the database
}database_type;

typedef struct
{
   unsigned char edge;
   unsigned char face;
   unsigned char text;
   unsigned char entity_node;
   unsigned char connected_node;
}primitive_class_type;

// Each theme has a symbol structure associated with it.  Themes on simple feature classes just have relevant symbol information for 
// one of the four primitive types, but complex feature themes may have any or all of the primitive type symbols.
typedef struct
{
   long int point;
   long int point_color;
   long int line_color;
   long int line;
   long int area_color;
   long int area;
   long int text_color;
   long int text;
}theme_symbol_type;

// Same as VPF feature types
typedef enum 
{	
	VEC_LINE = 1, 
	VEC_AREA, 
	VEC_ANNO, 
	VEC_POINTS, 
	VEC_COMPLEX = 6 
}VEC_FEATURE_TYPE;

// Spatial extents of the data-set.
typedef struct
{
    double xmin;
    double ymin;
    double xmax;
    double ymax;
}VEC_EXTENT;

// Spatial extents of the data-set.
typedef struct
{
    char *type;
    char *info;
}VEC_INFO;

// Geospatial coordinate pair.
typedef struct
{
	double x;
	double y;
}COORDINATE;

// Line/edge segment.
typedef struct
{
	long       id;
	long       nr_coords;
	COORDINATE *coords;
}SEGMENT;

typedef long			Pint;				// A PHIGS integer.
typedef double			Pfloat;				// A PHIGS float.

typedef struct
{
    Pint    marker_color;
    Pint    marker_type;
    Pfloat  marker_scale;
    Pfloat  marker_rotate;
}POINT_SYMB;	// Point Symbology structure for display of markers.

// Line Symbology structure for display of lines.
typedef struct
{
    Pint        line_color;
    Pint        line_style;
    Pfloat      line_width;
	POINT_SYMB *point_symb;
}LINE_SYMB;

// A set of flags used in feature editing.
typedef struct
{
    bool selected;		//TRUE if selected
    bool deleted;		//TRUE if flagged for deletion
    bool modified;		//TRUE if recently modified
}FEATURE_EDIT_FLAGS; 

typedef struct
{
    Pfloat x;
    Pfloat y;
} Ppoint;

typedef struct
{
    Pint   num_points;
    Ppoint *points;
} Ppoint_list;


typedef struct
{
    Pint   num_point_lists;
    Ppoint_list *point_lists;
} Ppoint_list_list;

// Line Feature structure.
typedef struct
{
	long					id;
	char*					name;
	short int 			    nWidth;
	long                    idSave;
	long					nr_segs;
	SEGMENT**				segs;
	LINE_SYMB*				line_symb;
	FEATURE_EDIT_FLAGS		flags;
	Ppoint_list_list*		lines;
} LINE_FEATURE;


// Text Symbology structure for display of text.
typedef struct
{
    Pint text_color;
    Pfloat text_scale;
	Pint text_font;
    POINT_SYMB *point_symb;
}TEXT_SYMB;

// Text Feature structure.
typedef struct
{
   long  id;
	char*					name;
   long  idSave;
   char  *string;
   long  nr_coords;
   COORDINATE  *coords;
   TEXT_SYMB   *text_symb;
   FEATURE_EDIT_FLAGS flags;
	Ppoint_list* texts;
}TEXT_FEATURE;

// Point Feature structure.
typedef struct
{
   long       id;
	short int	nSurface;
   char*	name;
   long  idSave;
   COORDINATE *coord;
   POINT_SYMB *point_symb;
   FEATURE_EDIT_FLAGS flags;
   Ppoint_list* points;
}POINT_FEATURE;

// Ring structure.
typedef struct
{
   long    id;
   long    nr_segs;
   SEGMENT **segs;
}RING;

#define		Pint_style			short	// PHIGS Interior Style
 
// Area Symbology structure for display of areas.
typedef struct
{
    Pint        area_color;
    Pint_style  area_style;
	LINE_SYMB  *line_symb;
}AREA_SYMB;

// Symbology structure for displaying of VECs.
typedef struct
{
   long					id;
	char*					name;
   long  idSave;
   short int	        nDensity;
   long					nr_rings;
   RING**				rings;
   AREA_SYMB*			area_symb;
   FEATURE_EDIT_FLAGS	flags;
   Ppoint_list_list*	area;
}AREA_FEATURE;

// Symbology structure for displaying of VECs.
typedef struct
{
	LINE_SYMB    *line_symb;
    TEXT_SYMB    *text_symb;
    POINT_SYMB   *point_symb;
    AREA_SYMB    *area_symb;
}VEC_SYMBOLOGY;


// A MUSE structure to hold vector overlay data.
typedef struct
{
	long				  magic;
	VEC_FEATURE_TYPE	  feature_type;
	E_TerrainObjectTypeID nTypeTerrain;
	long				nr_features;
	long				nr_lines;
	long				nr_text;
	long				nr_points;
	long				nr_areas;
	VEC_EXTENT			*extents;
	VEC_INFO			*info;
	LINE_FEATURE		**lines;
	TEXT_FEATURE		**text;
	POINT_FEATURE		**points;
	AREA_FEATURE		**areas;
	VEC_SYMBOLOGY		*symbology;
} VEC;


// A theme is a single entry for a view of the database.  It can be thought of as a stored query with a description and symbology. Each theme is associated with a feature class.
typedef struct
{
	char*					description;				// Description of the theme
	char*					database;					// Source Database path
	char*					library;					// Source Library name
	char*					coverage;					// Source coverage name
	char*					fc;							// Feature class name for the theme
	char*					ftable;						// Feature table path for the fc
	primitive_class_type	primclass;					// Primitive class(es) of theme
	char*					expression;					// Logical selection expression
	theme_symbol_type		symbol;						// Drawing symbol
	VEC*					vec;						// DGM 02/11/95
}theme_type;

typedef struct linked_list_cell
{
   void *element;
   long int element_size;
   struct linked_list_cell *next;
}cell_type, linked_list_type, position_type;

// Geographic extent or line segment
typedef struct
{
   double x1, y1, x2, y2;
}extent_type, line_segment_type;

typedef struct
{
  short int height;
  short int distance;
  short int gap;
}tick_struct;

typedef struct
{
  short int dashlistlen;
  short int *dashlist;
  short int gap;
}dash_struct;

typedef unsigned long	COLOR; 				// color encapsulation

typedef struct 
{
  long int red, green, blue;
  COLOR pixel;
}color_type;


typedef struct
{
   short int x0, y0;
   short int radius;
   short int FILLED;
   color_type color;
   short int color_idx;
} circle_struct;

// two corners of rect
typedef struct
{
   short int x0, y0, x1, y1; 
   short int FILLED;
   color_type color;
   short int color_idx;
} rect_struct;

// mathematical point
typedef struct s_pnt 
{          
    short v;                    // vertical (y) coordinate
    short h;                    // horizontal (x) coordinate
} PNT, *PNT_PTR;

typedef struct
{
   short int npts;
   PNT *pts;
   color_type color;
   short int color_idx;
}polyline_struct;

typedef struct
{
   short int npts;
   PNT *pts;
   short int FILLED;
   color_type color;
   short int color_idx;
}polygon_struct;

typedef union
{
   circle_struct circle;
   rect_struct rect;
   polyline_struct polyline;
   polygon_struct polygon;
}primitive_type;

typedef struct
{
   short int primtype;
   primitive_type prim_element;
}element_type;


typedef struct
{
   long int id;
   short int cenx, ceny;
   short int xhot, yhot;
   short int nprimitives;
   element_type *prim;
}marker_type;

typedef struct
{
  marker_type point;
  short int just;
  short int distance;
  short int gap;
}marker_line_struct;

typedef struct
{
  short int height;
  short int distance;
  short int radius;
}dot_line_struct;

typedef union
{
   tick_struct tick;
   dash_struct dash;
   marker_line_struct marker;
   dot_line_struct dot;
}line_attribute_type;


typedef struct
{
  short int width;
  short int style;
  line_attribute_type attr;
} linestyle_type;


typedef struct
{
  short int fillstyle;
  char* stipple;	   	 // If fillstyle not FillSolid
  short int outline_width;
  short int outline_style;
  line_attribute_type outline_attr;
}area_type;

typedef char*			text_type;

#define		STAT_SEV_NO_ERR     0x0000
#define		STAT_SEV_MESSAGE    0x2000
#define		STAT_SEV_WARNING    0x4000
#define		STAT_SEV_ERROR      0x6000
#define		STAT_SEV_FATAL      0x8000
#define		STAT_SEV_CRITICAL   0xA000
// Group Codes
#define		STAT_GRP_NO_ERR     0x0000
#define		STAT_GRP_SYSTEM     0x0100
#define		STAT_GRP_FILE       0x0200
#define		STAT_GRP_MATH       0x0300
#define		STAT_GRP_DATA       0x0400
#define		STAT_GRP_PARAM      0x0500
// File Group Error Codes
#define		STAT_FILE_CREAT_ERR (short)( STAT_GRP_FILE | STAT_SEV_ERROR | 0x0001 ) // Unable to create file
#define		STAT_FILE_CLOSE_ERR (short)( STAT_GRP_FILE | STAT_SEV_ERROR | 0x0002 ) // Unable to close file
#define		STAT_FILE_NOT_FOUND (short)( STAT_GRP_FILE | STAT_SEV_ERROR | 0x0003 ) // Unable to close file
#define		STAT_FILE_READ_ERR (short) ( STAT_GRP_FILE | STAT_SEV_ERROR | 0x0004 ) // Error reading from file
#define		STAT_FILE_TYPE_ERR (short) ( STAT_GRP_FILE | STAT_SEV_ERROR | 0x0005 ) // The specified file is of the wrong type
#define		STAT_FILE_WRITE_ERR (short)( STAT_GRP_FILE | STAT_SEV_ERROR | 0x0006 ) 
//System Group Error Codes
#define		STAT_MEM_ALLOC_ERR (short) ( STAT_GRP_SYSTEM | STAT_SEV_CRITICAL | 0x0001 )// Error allocating memory
#define		STAT_OS_ERROR (short)      ( STAT_GRP_SYSTEM | STAT_SEV_CRITICAL | 0x0002 )// An unknown operating system error
#define		STAT_SOFTWARE_BUG (short)  ( STAT_GRP_SYSTEM | STAT_SEV_CRITICAL | 0x0003 )// A possible bug in the software was found
// Abort condition
#define		STAT_ABORT (short)         ( STAT_GRP_SYSTEM | STAT_SEV_MESSAGE | 0x0004 ) //No Error Group Error Codes
#define		STAT_SUCCESS (short)       ( STAT_GRP_NO_ERR | STAT_SEV_NO_ERR  | 0x0000 )
#define		STAT_NO_DATA  (short)      ( STAT_GRP_DATA | STAT_SEV_WARNING | 0x0002 )
#define		STAT_DATA_ERR  (short)     ( STAT_GRP_DATA | STAT_SEV_FATAL   | 0x0001 )

#define		NOREF(a)			a = a

// Delimiter tokens
typedef enum 
{ 
	EQ, 
	NE, 
	LE, 
	GE, 
	LT, 
	GT, 
	AND, 
	OR, 
	QUOTE			// Warning defined twice
}delim_type;

// Symbol set
typedef struct
{
  long int nlinesym;
  linestyle_type *linesym;
  long int nmarkersym;
  marker_type *markersym;
  long int nareasym;
  area_type *areasym;
  long int ntextsym;
  text_type *textsym;
  long int ncolors;
  color_type *color;
}symbol_set_type;

// View structure.  Each view is associated with a particular database and a particular library within that datbase.
typedef struct view_type
{
	char name[9];                   // View name
   database_type *database;        // Array of Databases in the view
   long int ndb;                   // Number of databases in the view
   char *path;                     // Directory path to the view
   long int nthemes;               // Number of themes in the view
   theme_type *theme;              // Array of themes
   set_type selected;              // Set of themes selected for display
   set_type displayed;             // Set of displayed themes
   linked_list_type *sellist;      // List of selected themes (ordered)
   extent_type extent;             // MBR of all library extents
   double tileheight;              // Min of all library tile heights
   char sympath[255];              // Symbol set path
   symbol_set_type sym;            // Symbol set for the view
   int	nFeaturesEx;
}view_type;


// Structure for a color palette
typedef struct
{
    long					magic;				// structure id
    PAL_TYPE				type;				// how it was generated and used
    RGB						bins;				// # bins if type is PAL_RGB_BINS
    unsigned short					first_color;		// index of first valid palette color
    unsigned short					num_colors;			// # of valid colors
    RGB						rgb[MAX_COLORS];	// RGBs range 0 - 65535 (16bit)
}PALETTE;

typedef struct
{
    long            magic;
    DATA_HDR        data_hdr;
    DATA_MATRIX     data_matrix;
    LUT            *lut;	/* NULL unless data_type is DATA */
    PALETTE        *palette;	/* NULL unless data_type is COLOR_INDIRECT */
}RASTER;

typedef enum
{
    PAL_BOTH,
    PAL_RGB_ONLY,
    PAL_GRAY_ONLY,
    PAL_NATIVE
}
                PALETTE_USAGE;

typedef struct
{
    long magic;
    double          shade_threshold;
    double          precision;
    long            sunx;
    long            suny;
    PALETTE_USAGE   palette_usage;
}               RENDER_INFO;

typedef struct BASEMAP
{
    long            magic;
    char            product_name[12]; /*the name of the product*/
    RASTER          area;	/* product dependent */
    RASTER          bit_map;	/* plotable image */
    RENDER_INFO     render_info;/* NULL unless its a DATA product */
    bool         needs_refresh;	/* true if needs any work */
    bool         needs_xform;	/* true if it needs rendering */
    bool         needs_reload;	/* true if it needs to be reloaded */
    bool         warp;	/* true ifit supports projection warp */
}               BASEMAP;

typedef enum 
	{ 	_ENTIRE_LIST = 0,
		_TYPE1_LIST=1, 
		_TYPE2_LIST=2,
		_TYPE4_LIST=4,
		_TYPE8_LIST=8,
		_TYPE16_LIST=16
	} _LISTMODES;


#define STAT_BAD_PARM  (short)     (STAT_GRP_PARAM|STAT_SEV_ERROR|0x0002)
#define dtcc_reset_system(csP, hname )			_reset_system( csP,hname,_ENTIRE_LIST )

#define DEFAULT_GEO_SYSTEM	"World Geodetic System 1984"	
#define DEFAULT_UTM_SYSTEM	"Universal Transverse Mercator 84"
#define DEFAULT_UPS_SYSTEM	"Universal Polar Stereographic 84(North)"
#define DEFAULT_MGS_SYSTEM	"Military Grid Reference System 84"
#define DEFAULT_XYZ_SYSTEM	"Cartesian (XYZ) 84"

typedef enum 
	{	SUPPORTED_DATUMS = 1,
		USER_DATUMS = 2,
		ALL_DATUMS = 3,
		WORLD_WIDE_DATUMS = 4,
		NORTH_AMERICAN_DATUMS = 8,
		SOUTH_AMERICAN_DATUMS = 16,
		AFRICAN_DATUMS = 32,
		ASIAN_DATUMS = 64,
		EUROPEAN_DATUMS = 128,
		MICRONESIAN_DATUMS = 256,
		MISC_ISLAND_DATUMS = 512,
		ANTARCTIC_DATUMS = 1024
	} DATUM_LOCATIONS;


#define dtcc_set_transform_input_system( tsP, sysP )	_set_transform_system( tsP, 0, sysP )
#define dtcc_set_transform_output_system( tsP, sysP )	_set_transform_system( tsP, 1, sysP )
#define _SYSTEM_PROJP(csP)			( &((csP)->projection) )
#define _SYSTEM_ELLIPSOIDP(csP)		( &(_SYSTEM_DATUMP(csP)->ellipsoid) )
#define _SYSTEM_GROUPP(csP)			( &((csP)->group) )
#define dtcc_reset_datum(dpP, hname )			_reset_datum( dpP,hname,ALL_DATUMS )
#define dtcc_is_coord_group_geo( cgP  )		dtcc_coord3_is_geo( dtcc_coord_group_id(cgP) )

#define dtcc_is_system_geo(csP)		dtcc_is_coord_group_geo( dtcc_system_groupP(csP) )
#define dtcc_system_min_geoP(csP) 	dtcc_extents_min_geoP(dtcc_system_extentsP(csP))	  
#define dtcc_system_max_geoP(csP)  	dtcc_extents_max_geoP(dtcc_system_extentsP(csP))	 
#define dtcc_set_coord32( c3P,v )	 dtcc_set_coord3K( c3P, 1, v )
#define dtcc_set_coord3_lon( c3P,v )  dtcc_set_coord32( c3P,v )
#define dtcc_set_coord3_north(c3P,v)  dtcc_set_coord32( c3P,v )
#define dtcc_system_type(csP)		dtcc_coord_group_id( dtcc_system_groupP(csP) )
#define dtcc_set_coord31( c3P,v )	 dtcc_set_coord3K( c3P, 0, v )
#define dtcc_set_coord3_east(c3P,v)   dtcc_set_coord31( c3P,v )
#define dtcc_set_coord3_lat( c3P,v )  dtcc_set_coord31( c3P,v )
#define dtcc_system_labelP(csP)			((const DTCC_LABELS*)( (csP) ? &((csP)->label) : NULL ))
#define dtcc_system_datumP(csP)			((const DATUM_PARAMS*)( (csP) ? &((csP)->datums) : NULL ))
#define dtcc_system_dimension( csP ) 	((short)( (csP) ? (csP)->dimension : 0 ))
#define dtcc_clear_system( csP )		dtcc_clear_projection( (PROJ_PARAMS*)dtcc_system_projectionP(csP) )
#define dtcc_initialize_system( csP )	dtcc_initialize_projection( (PROJ_PARAMS*)dtcc_system_projectionP(csP) )
#define _SYSTEMKP(tsP,k)				(  (tsP)->systems + (k) )
#define dtcc_system_axis(csP) 			 dtcc_ellipsoid_axis(dtcc_system_ellipsoidP(csP))
#define dtcc_ellipsoid_rflat(ep)    	( (ep) ? (ep)->rflat : DOUBLE_NULL ) 
#define dtcc_system_rflat(csP) 			 dtcc_ellipsoid_rflat(dtcc_system_ellipsoidP(csP))
#define dtcc_system_hdatum_shiftsP(csP)	dtcc_datum_shiftsP( dtcc_system_datumP(csP) )
#define _DATUM_SHIFTSP(dpP)				( (dpP)->shifts ) 
#define dtcc_datum_shiftsP(dpP)			( (dpP) ? (const short*)_DATUM_SHIFTSP(dpP) : NULL )
#define _DATUM_SIGMASP(dpP)				( (dpP)->sigmas ) 
#define dtcc_datum_sigmasP(dpP)			( (dpP) ? (const short*)_DATUM_SIGMASP(dpP) : NULL )
#define dtcc_system_hdatum_sigmasP(csP)	dtcc_datum_sigmasP( dtcc_system_datumP(csP) )
#define _GEOID84_FILENAME				"geoid84.byt"
#define dtcc_default_geoid_file			_GEOID84_FILENAME
#define INVALID_STRUCTURE				 (-30)
#define dtcc_datum_vnameP(dpP)			( (dpP) ? (const char*)_DATUM_VNAMEP(dpP) : NULL )
#define dtcc_system_vdatum_nameP(csP)	dtcc_datum_vnameP( dtcc_system_datumP(csP) )
#define dtcc_is_coord_group_cart( cgP )	dtcc_coord3_is_xyz( dtcc_coord_group_id(cgP) )
#define dtcc_is_system_cart(csP)		dtcc_is_coord_group_cart( dtcc_system_groupP(csP) )
#define _PROJECTION_ORIGINP( ppP )      ( &((ppP)->origin) )
#define _PROJECTION_STDPARSP( ppP )      	( (ppP)->stdpars )
#define DUPLICATE_NAME					 (-33)
#define _SYSTEM_LABELP(csP)				( &((csP)->label) )
#define dtcc_coord3_is_geographic(id)	( (id) >= COORD3_GEO && (id) <= COORD3_GREF )
#define dtcc_is_coord_group_geographic( cgP  )	dtcc_coord3_is_geographic( dtcc_coord_group_id(cgP) )
#define dtcc_is_system_geographic(csP)	dtcc_is_coord_group_geographic( dtcc_system_groupP(csP) )
#define dtcc_projection_intrinsic_limits( ppP, mnmx )	_projection( ppP, DTCC_PROJECTION_LIMITS, NULL, 1L, mnmx )
#define DTCC_PROJECTION_LIMITS			(6)		   /* Intrinsic geo limits */
#define _set_valid( xP )				(	(xP)->valid = 1  )
#define _set_invalid( xP )				(	(xP)->valid = 0  )
#define INDEX_OUT_OF_RANGE               (-12)
#define _HDATUMS_FILENAME				"hdatmlst.txt"
#define _SYSTEM_UNITSP(csP)				( &((csP)->units) )
#define _SYSTEM_EXTENTSP(csP)			( &((csP)->extents) )
#define dtcc_system_min_lat(csP)  		dtcc_extents_min_lat( dtcc_system_extentsP(csP) )       
#define dtcc_system_min_lon(csP)  		dtcc_extents_min_lon( dtcc_system_extentsP(csP) )       
#define dtcc_system_max_lat(csP)  		dtcc_extents_max_lat( dtcc_system_extentsP(csP) )       
#define dtcc_system_max_lon(csP)  		dtcc_extents_max_lon( dtcc_system_extentsP(csP) )       
#define _COORD3K(c3P,k)					((c3P)->data.c[k]) 
#define dtcc_round(x)					((long)( (x) < 0.0 ? (x)-0.5 : (x)+0.5 ))
#define dtcc_allocate_ptr(n)			zMallocConv(n)
#define _list_dataP(lstP)				( (lstP)->dataP )
#define _SYSTEMS_FILENAME				"systmlst.txt"
#define dtcc_set_coord3_mode( cuP,m )	( (cuP)->mode = (m) )
#define _PROJECTION_CONSTANTSP( ppP )   ( (&(ppP)->cnstsP) )
#define GEOIDFP							( params->geoidfp )
#define _IS_LESS_THAN					(-1)
#define _IS_EQUAL_TO					(0)
#define _IS_GREATER_THAN				(1)
#define _IS_NOT_EQUAL_TO				(2)
#define _PROJECTION_ELLIPSOIDP( ppP )	( &((ppP)->ellipsoid) )													
#define _PROJECTION_LABELP( ppP )      		( &((ppP)->label) )
#define DTCC_PROJECTION_SETUP    		(4)        /* Compute projection constants */
#define _label_vflag(idP)				( (idP)->valid )
#define _label_tflag(idP)				( (idP)->type )
#define _label_l1(idP)					( (idP)->l1 )
#define _label_l2(idP)					( (idP)->l2 )
#define _label_l3(idP)					( (idP)->l3 )
#define _label_nameP(idP)				( (idP)->labels )
#define _label_abbrvP(idP)				( (idP)->labels + (idP)->l1 )
#define _label_otherP(idP)				( (idP)->labels + (idP)->l1 + (idP)->l2 )
#define FILE_NOT_FOUND                   (-6)
#define _list_type1(lstP)		( (lstP)->type1_count ) 				
#define _list_type2(lstP)		( (lstP)->type2_count ) 				
#define _list_total(lstP)		( _list_type1(lstP) + _list_type2(lstP) ) 				
#define _list_modified(lstP)	( (lstP)->modified ) 				
#define _list_added(lstP)		( (lstP)->added ) 				
#define MEMORY_ALLOCATION_FAILURE        (-10)
#define _DTCC_LABEL_NAME_SIZE			(72)
#define _DEFAULT_HDATUM	"WGS 1984"
#define _DEFAULT_HABBRV	"W84"
#define _DEFAULT_ANAME	"Global Definition"
#define _DEFAULT_VNAME	"AMSL"
#define _DEFAULT_USER_DABBRV	"XXXXX"
#define _DEFAULT_USER_DAREA	"USER DEFINED"
#define _DEFAULT_TYPE	WORLD_WIDE_DATUMS
#define dtcc_projection_ellipsoidP(ppP)		( (const ELLIPS_PARAMS*)( (ppP) ? &((ppP)->ellipsoid) : NULL ))
#define dtcc_system_projection_ellipsoidP(csP)	dtcc_projection_ellipsoidP(dtcc_system_projectionP(csP))		
#define FILE_NOT_OPEN                    (-2)
#define _SYSTEM_DIMENSION(csP)		( (csP)->dimension )
#define _ORIGIN_GRIDSWP( coP )        	(  (coP) ? &((coP)->gridSW)  : NULL )
#define _ORIGIN_GEOP( coP )           	(  (coP) ? &((coP)->geo ) : NULL )
#define _ORIGIN_SCALEP( coP )         	(  (coP) ? &((coP)->scales) : NULL )
#define _ORIGIN_CONVERFERCEP( coP )   	(  (coP) ? &((coP)->w0) : NULL )
#define _ORIGIN_AZIMUTHP( coP )       	(  (coP) ? &((coP)->azm0) : NULL )
#define _ORIGIN_GRIDNEP( coP )          (  (coP) ? &((coP)->gridNE) : NULL )
#define INVALID_DATA					 (-19)
#define dtcc_supported_system_count()	_slist_count( 1,NULL )
#define dtcc_system_count()       		_slist_count( 0,NULL )
#define dtcc_coord3_mode( cuP )		  ( (cuP)->mode )
#define dtcc_ellipsoid_ecc(ep)   		( (ep) ? (ep)->ecc : DOUBLE_NULL )
#define DTCC_DISPLAYBLE_LIST		_TYPE1_LIST
#define DTCC_GRID_LIST				_TYPE4_LIST
#define DTCC_SIMPLE_PROJ_LIST		_TYPE2_LIST


typedef struct										
{	char 		orglab[256];
	char		name[256];
	short		type1_count;
	short		type2_count;
	short		record_size;
	short		added;			/* number of items added to the list */
	short 		modified;		/* number of items marked to delete */
	void*		dataP;
} _DTCC_LIST;

struct 
_PROJECTION_PARS 
    { 	char* name;
    	char* abbrv;
      	DTCC_ENGINE engineP;
		short	type;

    };

#define dtcc_set_coord33( c3P,v )	 dtcc_set_coord3K( c3P, 2, v )

#define dtcc_coord3_north( c3P )      dtcc_coord32( c3P )
#define dtcc_set_coord3_north(c3P,v)  dtcc_set_coord32( c3P,v )
#define dtcc_coord3_east( c3P )       dtcc_coord31( c3P )
#define dtcc_set_coord3_east(c3P,v)   dtcc_set_coord31( c3P,v )
#define dtcc_coord3_hgt( c3P )        dtcc_coord33( c3P )
#define dtcc_set_coord3_zone(c3P,v)   dtcc_set_coord3Z( c3P,v )
#define dtcc_coord3_zone( c3P )       (short)dtcc_coord3Z(c3P)
#define dtcc_set_coord3_hgt(c3P,v)    dtcc_set_coord33( c3P,v )

#define dtcc_coord3_hgt( c3P )        dtcc_coord33( c3P )
#define dtcc_set_coord3_elv( c3P,v )  dtcc_set_coord33( c3P,v )

#define dtcc_set_extents_min_north(ceP,v) ( _set_invalid(ceP), dtcc_set_coord3_north(_EXTENTS_MIN_GRIDP(ceP),v)) 
#define dtcc_set_extents_min_east(ceP,v)  ( _set_invalid(ceP), dtcc_set_coord3_east(_EXTENTS_MIN_GRIDP(ceP),v)) 
#define dtcc_set_extents_min_hgt(ceP,v)   ( _set_invalid(ceP), dtcc_set_coord3_hgt(_EXTENTS_MAX_GRIDP(ceP),v)) 
#define dtcc_set_extents_min_zone(ceP,v)  ( _set_invalid(ceP), dtcc_set_coord3_zone(_EXTENTS_MAX_GRIDP(ceP),v)) 
#define dtcc_set_extents_max_north(ceP,v) ( _set_invalid(ceP), dtcc_set_coord3_north(_EXTENTS_MAX_GRIDP(ceP),v)) 
#define dtcc_set_extents_max_east(ceP,v)  ( _set_invalid(ceP), dtcc_set_coord3_east(_EXTENTS_MAX_GRIDP(ceP),v)) 
#define dtcc_set_extents_max_hgt(ceP,v)   ( _set_invalid(ceP), dtcc_set_coord3_hgt(_EXTENTS_MAX_GRIDP(ceP),v)) 
#define dtcc_set_extents_max_zone(ceP,v)  ( _set_invalid(ceP), dtcc_set_coord3_zone(_EXTENTS_MAX_GRIDP(ceP),v)) 
#define dtcc_set_extents_min_lat(ceP,v)   ( _set_invalid(ceP), dtcc_set_coord3_lat(_EXTENTS_MIN_GEOP(ceP),v)) 
#define dtcc_set_extents_min_lon(ceP,v)   ( _set_invalid(ceP), dtcc_set_coord3_lon(_EXTENTS_MIN_GEOP(ceP),v)) 
#define dtcc_set_extents_min_elv(ceP,v)   ( _set_invalid(ceP), dtcc_set_coord3_elv(_EXTENTS_MIN_GEOP(ceP),v)) 
#define dtcc_set_extents_max_lat(ceP,v)   ( _set_invalid(ceP), dtcc_set_coord3_lat(_EXTENTS_MAX_GEOP(ceP),v)) 
#define dtcc_set_extents_max_lon(ceP,v)   ( _set_invalid(ceP), dtcc_set_coord3_lon(_EXTENTS_MAX_GEOP(ceP),v)) 
#define dtcc_set_extents_max_elv(ceP,v)   ( _set_invalid(ceP), dtcc_set_coord3_elv(_EXTENTS_MAX_GEOP(ceP),v)) 

#define _list_nameP(lstP)		( (lstP)->name )
#define _list_olabP(lstP)		( (lstP)->orglab )
#define _list_rsize(lstP)		( (lstP)->record_size )

#define dtcc_reset_ellipsoid( elP, name )		_reset_ellipsoid( elP, name, _ENTIRE_LIST )
#define dtcc_angle_value(aP)     ( (aP)->dd )    
#define dtcc_angle_sin(aP)       ( (aP)->sinv )    
#define dtcc_angle_cos(aP)       ( (aP)->cosv )
#define dtcc_angle_rads(aP)      ( dtcc_angle_value(aP)*RADDEG )    

#define PROJ_ID(p)	( *(short*)(p) )


#define dtcc_origin_gridSWP(coP)		((const COORD3*)_ORIGIN_GRIDSWP(coP))
#define dtcc_origin_geoP(coP)				((const COORD3*)_ORIGIN_GEOP(coP))
#define dtcc_projection_constantsP(ppP)		( (const void*)( (ppP) ? (&(ppP)->cnstsP) : NULL ) )
#define dtcc_origin_scaleP(coP)			((const COORD3*)_ORIGIN_SCALEP(coP))
#define dtcc_origin_gridNEP(coP)		((const COORD3*)_ORIGIN_GRIDNEP(coP))
#define dtcc_origin_fe( coP )           dtcc_coord31( dtcc_origin_gridNEP(coP) ) 
#define dtcc_origin_lat0( coP )         dtcc_coord31(dtcc_origin_geoP(coP))
#define dtcc_origin_lon0( coP )         dtcc_coord32(dtcc_origin_geoP(coP))
#define dtcc_origin_fn( coP )           dtcc_coord32( dtcc_origin_gridNEP(coP) )
#define dtcc_origin_K0( coP )           dtcc_coord31(dtcc_origin_scaleP(coP))
#define dtcc_origin_fnSW( coP )         dtcc_coord32( dtcc_origin_gridSWP(coP) )
#define dtcc_origin_feSW( coP )         dtcc_coord31( dtcc_origin_gridSWP(coP) )
#define dtcc_projection_stdparsP(ppP)	( (const double*)( (ppP) ? (ppP)->stdpars : (double*)NULL ))


#define dtcc_projection_originP(ppP)		( (const COORD_ORIGIN*)( (ppP) ? &((ppP)->origin) : NULL ))
#define dtcc_projection_lon0( ppP )           dtcc_origin_lon0( dtcc_projection_originP(ppP) )
#define dtcc_projection_lat0( ppP )           dtcc_origin_lat0( dtcc_projection_originP(ppP) )
#define dtcc_projection_fn( ppP )             dtcc_origin_fn( dtcc_projection_originP(ppP) )
#define dtcc_projection_fe( ppP )             dtcc_origin_fe( dtcc_projection_originP(ppP) )
#define dtcc_projection_K0( ppP )             dtcc_origin_K0( dtcc_projection_originP(ppP) )
#define dtcc_projection_fnSW( ppP )           dtcc_origin_fnSW( dtcc_projection_originP(ppP) )
#define dtcc_projection_feSW( ppP )           dtcc_origin_feSW( dtcc_projection_originP(ppP) )
#define dtcc_projection_lower_std_parallel( ppP )  ( dtcc_projection_stdparsP(ppP)[0] )
#define dtcc_projection_upper_std_parallel( ppP )  ( dtcc_projection_stdparsP(ppP)[1] )

#define dtcc_projection_axis( ppP )            dtcc_ellipsoid_axis(dtcc_projection_ellipsoidP(ppP))
#define dtcc_projection_rflat( ppP )           dtcc_ellipsoid_rflat(dtcc_projection_ellipsoidP(ppP))
#define DTCC_PROJECTION_SCALEFACTOR  	(3)        /* Scale factors */
#define UNKNOWN_ITEM_REQUESTED           (-15)
#define CONSTANTS_NOT_INITIALIZED		(-20)
#define DATA_OUT_OF_RANGE                (-17)

#define dtcc_coord3A( c3P )   ((const char*)(  dtcc_coord3_is_encoded(dtcc_coord3Z(c3P)) ? (c3P)->data.a : NULL ))

#define dtcc_projection_fixed_grid(ppP)     ( (ppP)->fixed )
#define dtcc_coord3_mgrsP(c3P)				dtcc_coord3A( c3P )
#define dtcc_set_coord3_MGRS( c3P, mgr )	dtcc_set_coord3A( c3P, mgr, COORD3_MGRS )

#define DTCC_LINEAR             _TYPE1_LIST
#define DTCC_ANGULAR            _TYPE2_LIST
#define DTCC_LATITUDE           ( DTCC_ANGULAR | _TYPE4_LIST )
#define DTCC_LONGITUDE          ( DTCC_ANGULAR | _TYPE8_LIST )
#define DTCC_SHORT_ANGLE		( DTCC_ANGULAR | _TYPE16_LIST )
#define DTCC_SHORT_LINEAR		( DTCC_LINEAR  | _TYPE16_LIST )

#define SLIM			(cnsts->glims[0])
#define WLIM			(cnsts->glims[1])
#define NLIM			(cnsts->glims[2])
#define ELIM			(cnsts->glims[3])

#define AXIS		( params->from_a )
#define FLAT		( params->from_f )
#define TO_AXIS		( params->to_a )
#define TO_FLAT		( params->to_f )
#define FROM_SHIFTS	( params->from_shifts )
#define FROM_DX		( params->from_shifts[0] )
#define FROM_DY		( params->from_shifts[1] )
#define FROM_DZ		( params->from_shifts[2] )
#define FROM_SIGMAS	( params->from_sigmas )
#define FROM_SX		( params->from_sigmas[0] )
#define FROM_SY		( params->from_sigmas[1] )
#define FROM_SZ		( params->from_sigmas[2] )
#define TO_SHIFTS	( params->to_shifts )
#define TO_DX		( params->to_shifts[0] )
#define TO_DY		( params->to_shifts[1] )
#define TO_DZ		( params->to_shifts[2] )
#define TO_SIGMAS	( params->to_sigmas )
#define TO_SX		( params->to_sigmas[0] )
#define TO_SY		( params->to_sigmas[1] )
#define TO_SZ		( params->to_sigmas[2] )
#define ADBFN		( 1.0 / ( 1.0 - FLAT ) )
#define ECC2FN(f)	( (f)*(2.0-(f)) )
#define DAFN		( -( AXIS - TO_AXIS ) )	 
#define DFFN		( -( FLAT - TO_FLAT ) )
#define DXFN		( FROM_DX - TO_DX )
#define DYFN		( FROM_DY - TO_DY )
#define DZFN		( FROM_DZ - TO_DZ )
#define SXFN		( FROM_SX + TO_SX )
#define SYFN		( FROM_SY + TO_SY )
#define SZFN		( FROM_SZ + TO_SZ )
#define SPECCODE	( params->wgs7284 )
#define GEOIDFP		( params->geoidfp )

#define dtcc_coord3_map_scale( cuP )  ( (cuP)->map_scale )
#define _ELLIPSOID_FILENAME			"ellplst.txt"
#define dtcc_reset_units( uiP, nm)			_reset_units( uiP,nm,1 )
#define dtcc_free_ptr(aP)        free((void*)(aP))
#define _COORD3_UNITS1P( cuP )    ( (cuP) ? _COORD3_PICK_UNITS(cuP,0) : NULL )
#define _COORD3_UNITS2P( cuP )    ( (cuP) ? _COORD3_PICK_UNITS(cuP,1) : NULL )
#define _COORD3_UNITS3P( cuP )    ( (cuP) ? _COORD3_PICK_UNITS(cuP,2) : NULL )
#define dtcc_units_are_linear( cuP )      dtcc_is_typeN( dtcc_type_flag(cuP),DTCC_LINEAR )
#define _DEFAULT_USER_EABBRV	"XXX"	
#define DELIMITER	"\\"
#define NUMERIC_NOT_GEO                 (-26)
#define NUMERIC_TOO_BIG                 (-27)
#define NUMERIC_NEGATIVE                (-28)
#define dtcc_origin_H0( coP )           dtcc_coord32(dtcc_origin_scaleP(coP))
#define dtcc_origin_azimuthP(coP)		((const ANGLE*)_ORIGIN_AZIMUTHP(coP))
#define dtcc_origin_azm0( coP )         dtcc_angle_value(dtcc_origin_azimuthP(coP))
#define EPSILON		( 0.0000001 )
#define NM				(cnsts->name)
#define P0				(cnsts->p0)
#define SINP0			(cnsts->sinp0)	/* SIN( ABS(P0) ) */
#define COSP0			(cnsts->cosp0)	/* COS( ABS(P0) ) */
#define L0				(cnsts->l0)
#define RADIUS			(cnsts->axis)
#define defAXIS			(cnsts->axis)
#define defECC			(cnsts->e)
#define defECC2			(cnsts->e2)
#define WH				(cnsts->which)
#define NCNSTS			(cnsts->ncnsts)
#define K0				(cnsts->k0)
#define AZ0				(cnsts->a0)
#define STD1			(cnsts->p1)		
#define STD2			(cnsts->p2)
#define SLIM			(cnsts->glims[0])
#define WLIM			(cnsts->glims[1])
#define NLIM			(cnsts->glims[2])
#define ELIM			(cnsts->glims[3])
#define EQUATORIAL		( cnsts->which == 0 )
#define POLAR			( abs(cnsts->which) == 1 )
#define SOUTH_POLAR		( cnsts->which == -1 )
#define SPHERE			( defECC < EPSILON )
#define OBLIQUE			( cnsts->which == 2 )
#define defM_PI        (3.14159265358979323846) 
#define PI			3.14159265358979323
#define HALF_PI       (PI/2.0) 
#define allocate_memory(n) zMallocConv( (size_t)(n) )
#define deref(x)			(x)
#define defGROUP	((short*)cnsts->other+13)
#define AZONE	((char*)(&cnsts->a0))
#define _COORD3A(c3P)		 ((c3P)->data.a) 
#define RADGRAD        (0.01570796)
#define DEGRAD         (180.0/defM_PI)
#define DEGGRAD        (RADGRAD/RADDEG)
#define TWO_PI      (defM_PI+defM_PI)
struct 
XUNITS_LIST
{	char *name, *abbrv, *fmt; 
	double cf; 
	short valid,type,scaled;  
};

#define METFOOT        (0.3048)  /* International foot */
#define METINCH        (METFOOT/12.0)
#define METNMILE       (METFOOT*6076.1155)
#define METSMILE       (METFOOT*5280.0)
#define METYARD        (METFOOT*3.0)
#define METFATHOM      (METFOOT*6.0)
#define METMM		   ( 0.001 )
#define METCM		   ( 0.01 )
#define METKM		   ( 1000.0 )

enum{	PARAMETER_OUT_OF_RANGE = -100, ELLIPSOID_IS_SPHERE };
#define free_memory( aP ) free(aP)

typedef struct
{	 
	double cr, ci;  
}DCOMPLEX;

typedef struct {
    long ncharo,ltrnmo[3],ltrlco[3];double xltro,yltro,spsouo,spnoro,sleaso,
    slweso,ylowo,yslowo;long iareao,izoneo,ltrlo,ltrho,ltrhyo;double fnltro,
    feltro;
} defComgrda;

typedef struct {
    char mgrso[5],azoneo[2];
} defComgrch;

#define MIN_AXIS	(6377200.0)
#define MAX_AXIS	(6378400.0)
#define MIN_FLAT	(292.0)
#define MAX_FLAT	(302.0)

#define FILE_ERROR                       (-1)
#define _AUNITS_COUNT	(8)
#define _LUNITS_COUNT	(8)
typedef double 		IEEE_DOUBLE8;
#define WRITE_INCOMPLETE                 (-3)

#define  SZ_MAPGEO           	   64
#define  SZ_PLOT_PARAMS      	  512
#define  MAGIC_PLOT_PARAMS             	  92000017L

#define dtcc_get_coord3_geo( c3P, u,v,w )     dtcc_get_coord3( c3P, u,v,w, NULL )
#define  SZ_EXTENT           	  128

#define dtcc_system_hdatum_nameP(csP)			dtcc_nameP( dtcc_system_datumP(csP) )
#define dtcc_coord3_units_labelNP( cuP, N)		dtcc_nameP(_COORD3_UNITSNP(cuP,N))

#define dtcc_system_userdef(ep)  	( dtcc_type_flag(ep) == _TYPE2_LIST ) 
#define dtcc_coord3DataP( c3P )     ( !(c3P) ? NULL : ( !dtcc_coord3_is_encoded(dtcc_coord3Z(c3P)) ? (void*)((c3P)->data.c) : (void*)dtcc_coord3A( c3P ) ))
#define dtcc_system_hdatum_abbrvP(csP)		dtcc_abbrvP( dtcc_system_datumP(csP) )
#define dtcc_coord3_units_abbrvNP( cuP, N)		dtcc_abbrvP(_COORD3_UNITSNP(cuP,N))
#define dtcc_system_originP( csP )	( (csP) ? dtcc_projection_originP(&(csP)->projection) : NULL )
#define dtcc_origin_convergenceP(coP)	((const ANGLE*)_ORIGIN_CONVERFERCEP(coP))
#define dtcc_origin_conv0( coP )        dtcc_angle_value(dtcc_origin_convergenceP(coP))
#define dtcc_system_stdparsP(csP)		dtcc_projection_stdparsP(dtcc_system_projectionP(csP))		
#define dtcc_system_map_scale(csP) 		dtcc_coord3_map_scale( dtcc_system_unitsP(csP) ) 
#define dtcc_datum_count()       		_dlist_count( ALL_DATUMS,NULL )
#define dtcc_supported_datum_count()	_dlist_count( SUPPORTED_DATUMS,NULL )
#define dtcc_supported_ellipsoid_count()	_elist_count(1,NULL)
#define dtcc_ellipsoid_count()       		_elist_count( 0,NULL )


// -----------------------------------------------------------------	
#define NCalbert		(8)
#define Calbert			cnsts->other[0]
#define Nalbert			cnsts->other[1]
#define R0albert		cnsts->other[2]
#define E1albert		cnsts->other[3]
#define AUCFSalbert		(cnsts->other+4)
#define LABELalbert		"Albers Equal Area"


#define NCcylind		(16)
#define AK0cylind		(cnsts->other[0])
#define KAIcylind		(cnsts->other[1])
#define QPcylind		(cnsts->other[1])
#define F3cylind		(cnsts->other[2])
#define M0cylind		(cnsts->other[2])
#define AUCFScylind		(cnsts->other+3)
#define FPCFScylind		(cnsts->other+7)
#define MDCFScylind		(cnsts->other+11)
#define LABELcylind		"Cylindrical Equal Area"

#define NClamb			(7)
#define Flamb			cnsts->other[0]
#define Nlamb			cnsts->other[1]
#define R0lamb			cnsts->other[2]
#define CFCFSlamb		(cnsts->other+3)
#define LABELlamb		"Lambert Conformal Conic"
#define RHOlamb(p,sp) 	( defAXIS*Flamb*pow(tanz(defECC,p,sp),Nlamb ) )

#define NClambeq		(11)
#define RPlambeq		cnsts->other[0]
#define Dlambeq			cnsts->other[1]
#define Glambeq			cnsts->other[2]
#define GElambeq		cnsts->other[3]
#define B1lambeq		cnsts->other[4]
#define SINB1lambeq		cnsts->other[5]
#define COSB1lambeq		cnsts->other[6]
#define AUCFSlambeq		(cnsts->other +  7)
#define LABELlambeq		"ZLambert Equal-Area"

#define NCmerca			(4)
#define CFCFSmerca		(cnsts->other)
#define LABELmerca		"Mercator"
#define FNmerca			(900000.0)
#define FEmerca			(3900000.0)

#define NCortho			(0)
#define LABELortho		"ZOrthographic"

#define NCrect			(1)
#define RCPrect			cnsts->other[0]
#define LABELrect		"Rectangular"

#define NCstereo		(10)
#define M1stereo		cnsts->other[1]
#define FAstereo		cnsts->other[2]
#define CLAT1stereo		cnsts->other[3]
#define SINCLAT1stereo	cnsts->other[4]
#define COSCLAT1stereo	cnsts->other[5]
#define CFCFSstereo		(cnsts->other+6)
#define LABELstereo		"ZStereographic"


#define NCtrmerca		(13)
#define EP2trmerca		cnsts->other[1]
#define M0trmerca		cnsts->other[2]
#define KAtrmerca		cnsts->other[3]
#define MDCFStrmerca	(cnsts->other+4)
#define RCCFStrmerca	(cnsts->other+9)
#define LABELtrmerca	"Transverse Mercator"

#define FNutm	(0.0)
#define FSutm	(10000000.0)
#define FEutm	(500000.0)


#define FNnzeal (6023150.0)
#define FEnzeal (2510000.0)

#define FNmada 				(800000.0)
#define FEmada				(400000.0)
#define PAR0mada			cnsts->other[1]
#define SP0mada				cnsts->other[2]
#define ALmada				cnsts->other[3]
#define CNSTmada			cnsts->other[4]
#define SRADmada			cnsts->other[5]
#define CAmada				cnsts->other[6]
#define CBmada				cnsts->other[7]
#define CCmada				cnsts->other[8]
#define CDmada				cnsts->other[9]
#define NCmada				(10)
#define LABELmada			"Madagascar Laborde"

/* Polar Stereographic Projection functions */
#define NCps				(5)
#define C0ps				cnsts->other[1]
#define CFCFSps				(cnsts->other+2)
#define LABELps				"Polar Stereographic"

#define FNups (2000000.0)
#define FEups (2000000.0)
#define NORTHLIMups	(83.5)
#define SOUTHLIMups	(-79.5)

typedef struct MAPGEO
{
    long            magic;
    COORD_SYS       internal_coord_sys;
	COORD_SYS       map_display_coord_sys;
	COORD_SYS       user_coord_sys;
    COORD_SYS       user2_coord_sys;

    /* May 16 */
	COORD_SYS       data_coord_sys;
	TRANSFORM_SYS   *internal_to_user;
	TRANSFORM_SYS   *user_to_internal;
	TRANSFORM_SYS   *internal_to_display;
	TRANSFORM_SYS   *display_to_internal;
    TRANSFORM_SYS   *internal_to_user2;
    TRANSFORM_SYS   *user2_to_internal;

    
    /* May 16 */
	TRANSFORM_SYS   *data_to_display;
	TRANSFORM_SYS   *display_to_data;
    int             geo_to_bit_errors;
    int             bit_to_geo_errors;
    defEXTENT       geo_extent;	/* bounding geographic rectangle, DD */
	PLOT_PARAMS     plot_params;
    bool         hide_transform_err_messg; /*FALSE if user stops geo_to_bit/bit_to_geo messages*/
    bool         use_arc;	/* use ARC adjustment for all non-grid display systems */
    bool         is_locked;	/* true when locked to basemap */
    bool         recently_changed;	/* true until map is updated */
}MAPGEO;
#endif //VMAPCONV_H


