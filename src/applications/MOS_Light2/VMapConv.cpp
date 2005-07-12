
#include "MOS_Pch.h"

#include <sys/stat.h>
#include <stdio.h>
#include <io.h>
#include <float.h>
//#include <Direct.h>
#include "Vmapconv.h"

#include <vector>
using namespace std;

#pragma warning ( push )
#pragma warning ( disable: 4100 4244 4189 4211 4127 4310 4505 4701 ) // $$$$ Si quelqu'un veux corriger le code, qu'il ne se gène pas !

MAPGEO*						mapgeo = NULL;
static _DTCC_LIST*					dlist =	0;
static _DTCC_LIST*					slist =	0;
static _DTCC_LIST*					elist =	0;


extern USER_PARAMS*					params;
static double		rDefImageOriginX = 0.0;
static double		rDefImageOriginY = 0.0;
static double		rDefPixelsPerMeterX = 1.0;//0.0023;
static double		rDefPixelsPerMeterY = 1.0;//0.0023;
static double		rDefImageWidth = 2048.0;
static double		rDefImageHeight = 2048.0;

static MAP_PREF nDefConv = GEO_AND_IMAGE_EXTENT;

char* _SYSTEM_DIRECTORY_NAME = NULL;


//-------------------------RENDERING-------------------------------------------------

ERRSTATUS			mapgeo_construct( double rImageWidth, double rDefImageHeight );
ERRSTATUS			Trans_vec( view_type* view );
void				Free_Vec( view_type* view );

//-------------------------CONVERTING ROUTINES-------------------------------------------------

void				bit_and_geo_ops_complete( MAPGEO * mapgeo );

void				Free_Vec_Areas( VEC * vec );
ERRSTATUS			Trans_vec_areas(MAPGEO * mapgeo, VEC * vec );

void				Free_Vec_Lines( VEC * vec );
ERRSTATUS			Trans_vec_lines(MAPGEO * mapgeo, VEC * vec );

void				Free_Vec_Points( VEC * vec );
ERRSTATUS			Trans_vec_points(MAPGEO * mapgeo, VEC * vec );

void				Free_Vec_Text( VEC * vec );
ERRSTATUS			Trans_vec_text(MAPGEO * mapgeo, VEC * vec );

ERRSTATUS			geometry_match_basemap(BASEMAP * basemap, MAPGEO * mapgeo);
ERRSTATUS			basemap_match_geometry(BASEMAP * basemap, MAPGEO * mapgeo);

ERRSTATUS			geo_to_bit_for_data(MAPGEO * mapgeo, double londd, double latdd, double * bitx, double * bity);
ERRSTATUS			bit_to_geo_for_data(MAPGEO * mapgeo,double bitx, double bity,double * londd, double * latdd);

ERRSTATUS			Geo_to_Mgrs_for_data( double londd, double latdd, std::string& sMgrs  );
ERRSTATUS			Geo_to_Utm_for_data( double londd, double latdd, long* iZone, double* lonUTM, double* latUTM  );
ERRSTATUS			Mgrs_to_Geo_for_data( const std::string& sMgrs, double *londd, double *latdd  );
ERRSTATUS			Utm_to_Geo_for_data( long iZone, double lonUTM, double latUTM, double *londd, double *latdd  );
ERRSTATUS			Bit_to_Mgrs_for_data( const MT_Vector3D& vPos, std::string& sMgrs  );
ERRSTATUS			Bit_to_Utm_for_data( double bitx, double bity, long* iZone, double* lonUTM, double* latUTM  );
ERRSTATUS			Mgrs_to_Bit_for_data( const std::string&  sMgrs, MT_Vector3D& vPos );
ERRSTATUS			Utm_to_Bit_for_data( long iZone, double lonUTM, double latUTM, double *bitx, double *bity );


double				geo_360_to_180( double *londd );
short				dtcc_set_coord3( COORD3* c3P, double u, double v, double w, short  zone );
short				dtcc_transform( TRANSFORM_SYS* tsP, const void* inptr, void* outptr );
const char*			dtcc_nameP( const void* dtccP );
double				geo_180_to_360( MAPGEO *mapgeo, double *londd );
FILE*				dtcc_vdatum_fileP( const DATUM_PARAMS* dpP );
short				dtcc_are_coord3_units_standard(	const COORD3_UNITS* cuP );
short				dtcc_convert_coord3_from_units( const COORD3_UNITS* cuinP, const COORD3* c3inP, COORD3* c3outP );
short				dtcc_copy_coord3( COORD3* c1P, const COORD3* c2P );
short				dtcc_extents_contains( const COORD_EXTENTS* csP, const COORD3* c3P );
short				dtcc_is_vdatum_AMSL( const DATUM_PARAMS* dpP );
double				geoid_ellipsoid( double N, double hgt, int mode );
double				geoid_height( FILE* infile, double lat, double lon  );
short				molodensky( double from_lat,  double from_lon, double from_hgt,double *lat,  double* lon, double* hgt, const void *moloP );
static short		_convert_geo_to_output(   const COORD_SYS* outsysP,COORD3* outptP 	);
short				_projection( 	const PROJ_PARAMS* proP, short mode, const COORD3* fromP, long n, COORD3* toP );
short				dtcc_usr_to_geo(	const ELLIPS_PARAMS* elP, const COORD3* from_pt, COORD3* to_pt);
short				dtcc_convert_coord3N_to_units( const COORD3_UNITS* cuinP, const COORD3N* c3NinP, COORD3N* c3NoutP );
short				dtcc_convert_coord3_to_units(	const COORD3_UNITS* cuinP, const COORD3* c3inP,COORD3* c3outP );
short				dtcc_valid_flag(const void* dtccP);
int					dtcc_is_valid( const void* dtccP );
double				dtcc_convert_from_units( const UNITS_INFO* uiP,const void* v   );
short				read_hgts( FILE* infile, double* hgts, long row,long col, size_t n );
short				molodensky_shift( double from_lat,  double from_lon, double from_hgt, double *dphi, double* dlam, double* dhgt, const void *moloP );
double				check_180( double a );
short				dtcc_usr_from_geo (	const ELLIPS_PARAMS* elP, const COORD3*	from_pt,COORD3*	to_pt);
int					usr_to_geo(	double a, double e2, double x, double y, double z,double* lat, double *lon, double *h 	);
short				dtcc_convert_to_units( const UNITS_INFO* uiP, double v, void* result   );
int					dtcc_is_valid_name( const void* dtccP );
double				dtcc_decode_units( const UNITS_INFO* uiP, const char* encoded, long* nl   );
short				wgs72 ( double from_lat,  double from_lon, double from_hgt,double *tlat,  double* tlon, double* thgt, int flag72 );
short				wgs66( double from_lat,  double from_lon, double from_hgt, double *tlat,  double* tlon, double* thgt, const struct TRANSFORM_PARAMS *params );
int					geo_to_usr(	double a, double e2, double lat, double lon, double h, double* x, double *y, double *z );
short				dtcc_encode_units( const UNITS_INFO* uiP, const char* prefixes, double  v, char* encoded   );
int					_check_label_form( const char* idP );
double				dtcc_decode_dm( const char* encoded, long* nl );
double				dtcc_decode_dms( const char* encoded, long* nl );
long				_parseData( const char* string, const char* flags, int n, char* prefix, char* nmbr, char* postfix );
void				dtcc_encode_dm(  double  v, const char* label, char* dmP, const char* fmtP  );
const char*			dtcc_otherP( const void* dtccP );
void				dtcc_encode_dms( double  v, const char* label, char* dmsP,	const char* fmtP   );
void				dtcc_encode_degs(   double  v, const char* label, char* degP, const char *fmtP    );
const char*			dtcc_abbrvP( const void* dtccP );
int					_skipWhiteSpace( const char* string );
double				form_round_60( double v, const char* fmt );
bool				geo_extent_hit_test_line( MAPGEO *mapgeo, double lat, double lon, bool check_edge, double last_lon );
bool				geo_extent_hit_test_point( MAPGEO *mapgeo, double lat, double lon );
short				dtcc_reset_default_cartesian_system( COORD_SYS* systemP);
short				dtcc_reset_default_geodetic_system( COORD_SYS* systemP );
short				dtcc_reset_default_UTM_system( COORD_SYS* systemP	);
short				dtcc_reset_default_UPS_system( COORD_SYS* systemP	);
short				dtcc_reset_default_MGRS_system( COORD_SYS* systemP );
ERRSTATUS			setup_transform_struct( const COORD_SYS *in_sys, const COORD_SYS *out_sys, TRANSFORM_SYS *trans_sys );
short				dtcc_reset_system_projection(	COORD_SYS* csP, const char* gname );
short				dtcc_reset_system_hdatum(	COORD_SYS* csP, const char* hname );
ERRSTATUS			mapgeo_adjust( MAPGEO *mapgeo, bool preserve_aspect );
short				_reset_system( COORD_SYS* csP, const char* name, short type );
short				dtcc_clear_transform( TRANSFORM_SYS* tsP );
short				dtcc_initialize_transform( TRANSFORM_SYS* tsP );
short				_set_transform_system( TRANSFORM_SYS* tsP, int k, const COORD_SYS* sysP );
short				dtcc_reset_projection( PROJ_PARAMS* ppP, const char* pname, const ELLIPS_PARAMS* ellipsoid );
short				dtcc_make_system_name( COORD_SYS* csP, char* nm );
short				dtcc_set_system_name( COORD_SYS* csP, const char* name );
short				dtcc_set_default_system_extent( COORD_SYS* csP );
short				dtcc_is_projection_MGRS( const PROJ_PARAMS* ppP	);
short				dtcc_set_coord_group(	COORD_GROUP* cgP,COORD3_TYPES id );
short				dtcc_is_projection_UTM(	const PROJ_PARAMS* ppP	);
short				dtcc_is_projection_UPS( const PROJ_PARAMS* ppP );
short				_reset_datum( DATUM_PARAMS* dpP, const char* name, int type );
short				_check_itcoordsys( COORD_SYS* cpP );
bool				coord_sys_is_worldwide( const COORD_SYS *csP );
short				dtcc_set_coord3K( COORD3* c3P, short k, double v );
double				get_lon_adjust_factor( MAPGEO *mapgeo );
void				map_to_bitmap_fit( MAPGEO *mapgeo, COORD3 *map_min, COORD3 *map_max, bool preserve_aspect, bool preserve_vert_scale, bool preserve_pixel_extent);
ERRSTATUS			geo_to_grid_extent_fit( MAPGEO *mapgeo, COORD3 *geo_min, COORD3 *geo_max, COORD3 *grid_min, COORD3 *grid_max );
ERRSTATUS			grid_to_geo_extent_fit( MAPGEO *mapgeo, COORD3 *grid_min, COORD3 *grid_max, COORD3 *geo_min, COORD3 *geo_max );
void*				dtcc_decode_label( const char* lblP, void* dtccP );
const COORD_SYS*	_find_system( const DTCC_LABELS* dtccP, int type, int onlyvalid );
short				_update_system( COORD_SYS* cpP, const DTCC_LABELS* lblP,const COORD_GROUP* group, const DATUM_PARAMS*	datums, const PROJ_PARAMS* projection, const COORD_EXTENTS* extents, const COORD3_UNITS* units,	short dimension, short type );
short				dtcc_type_flag(const void* dtccP);
short				dtcc_clear_projection( PROJ_PARAMS* proP );
short				dtcc_initialize_projection( PROJ_PARAMS* proP );
short				dtcc_copy_system(	COORD_SYS* cp1, const COORD_SYS* cp2 );
short				dtcc_validate_system(	COORD_SYS* csP	);
int					dtcc_compare_vdatums( const DATUM_PARAMS* dpP1,const DATUM_PARAMS* dpP2 );
short				dtcc_compare_datums(	const DATUM_PARAMS* dpP1,const DATUM_PARAMS* dpP2);
void				_init_molo_parameters(	double fmA, double fmRF, const short* fmShfts, const short* fmSgms,	double toA, double toRF, const short* toShfts, const short* toSgms,	struct TRANSFORM_PARAMS *params );
short				_reset( PROJ_PARAMS* ppP, const char* name, const ELLIPS_PARAMS* elP  );
short				dtcc_encode_coord_group( 	const COORD_GROUP* cgP,char* labelP	);
long				hash( const void* v, long n );
short				dtcc_copy_label( DTCC_LABELS* destP, const DTCC_LABELS* srcP );
void				dtcc_set_invalid(void* dtccP);
void				dtcc_set_type( void* dtccP, int n );
short				dtcc_reset_system_extents( COORD_SYS* csP, const COORD3* mngridP, const COORD3* mxgridP, const COORD3* mngeoP, const COORD3* mxgeoP );
short				dtcc_initialize_datum_list( const char* fname );
const DATUM_PARAMS* _find_datum( const void* dtccP, int type );
short				_update_datum(  DATUM_PARAMS* dpP, const char* nm1, const char* nm2, const char* vnm, const char* ab, const short* shfts,const short* sigs, const ELLIPS_PARAMS* elP, int type );
short				dtcc_validate_coord3_units( COORD3_UNITS* csP	);
short				dtcc_validate_datum( DATUM_PARAMS* dpP );
short				dtcc_validate_projection(	PROJ_PARAMS* csP );
short				dtcc_validate_coord3_extents(	COORD_EXTENTS* csP	);
short				_check_ellipsoids( COORD_SYS* csP );
void				dtcc_set_valid(void* dtccP);
short				adrg_zone_compute(double latitude,long *zone);
void				_get_label_parts ( const char* label, int mx, DTCC_LABELS* idP );
short				_check_listsystem( int type, short *start, short *stop );
short				_check_listdatum( int type, short *start, short *stop );
short				_check_listellipse( int type, short *start, short *stop );
short				dtcc_copy_coord_group( COORD_GROUP* cg1P,	const COORD_GROUP* 	cg2P );
short				dtcc_copy_datum( DATUM_PARAMS* dP1, const DATUM_PARAMS* dP2  );
short				dtcc_copy_projection( PROJ_PARAMS* ppP1, const PROJ_PARAMS* ppP2 );
void				update_projection_constants(PROJ_PARAMS* ppP);
short				dtcc_copy_extents( COORD_EXTENTS* ceP1, const COORD_EXTENTS* ceP2  );
short				dtcc_invalidate_coord3_units(	COORD3_UNITS* csP);
short				dtcc_copy_coord3_units(	COORD3_UNITS* dest,	const COORD3_UNITS* src );
short				dtcc_set_system_dimension( COORD_SYS* csP, short	   dm );
short				dtcc_compare_ellipsoid(	const ELLIPS_PARAMS* elP1, const ELLIPS_PARAMS* elP2);
int					w84( double a, double rf, const short* s );
int					w72( double a, double rf, const short* s );
int					w66( double a, double rf, const short* s );
void				_reset_null(PROJ_PARAMS* ppP);
short				dtcc_copy_ellipsoid( ELLIPS_PARAMS* ep1, const ELLIPS_PARAMS* ep2 );
short				dtcc_set_origin_geo(  COORD_ORIGIN* coP, double latitude, double longitude, double elv );
short				dtcc_set_origin_scale( COORD_ORIGIN* coP, double k0, double h0,  double  v0 );
short				dtcc_set_origin_azimuth( COORD_ORIGIN* coP, double  azm  );
short				dtcc_set_origin_convergence( COORD_ORIGIN* coP, double w0);
short				dtcc_set_origin_gridSW( COORD_ORIGIN* coP, double northing, double easting, double hgt, short zone );
short				dtcc_set_origin_grid( COORD_ORIGIN* coP, double northing, double easting,  double hgt,   short zone  );
short				_get_projection_label( const char* name, void* lblP );
short				_setProjectionEngine( PROJ_PARAMS* ppP );
short				_check_itproj( PROJ_PARAMS* ppP );
short				_check_itdatum( DATUM_PARAMS* dtccP );
short				_check_itcoord3( const COORD3_UNITS* cuP );
short				_group_label( short opt, COORD3_TYPES* idP, char* label );
short				blockcopy( void* destP, const void* srcP, int n );
short				dtcc_reset_extents( COORD_EXTENTS* ceP, const COORD3* mngridP, const COORD3* mxgridP,const COORD3* mngeoP, const COORD3* mxgeoP  );
FILE*				_scan_list_hdr( _DTCC_LIST** lstH, const char* fn );
const char*			_check_datum_name( const char* fn );
short				dtcc_delete_datum_list(_DTCC_LIST** lstP);
short				dtcc_scan_hdatum( DATUM_PARAMS* dtm, FILE* fp );
int					dtcc_is_typeN( short k, int n );
DTCC_LABELS*		dtcc_set_label( void* dtccP, const char* name, const char* abbrv, const char* other,short valid, short type );
short				dtcc_make_hdatum_name( DATUM_PARAMS* csP, char* nm );
short				_check_itcoordextent( COORD_EXTENTS* ceP );
void				dtcc_trim_white(	char* strP	);
const struct		_PROJECTION_PARS* _find_pro( const DTCC_LABELS* lblP );
short				dtcc_initialize_system_list( const char* fname );
short				_slist_count( int k, _DTCC_LIST* listP );
short				dtcc_set_system_units( COORD_SYS* csP, short mode, double mscale, const char* in1, const char* in2, const char* in3 );
int					dtcc_clear_label( void* dtccP );
short				dtcc_reset_default_ellipsoid(	ELLIPS_PARAMS* elP  );
short				dtcc_set_origin( COORD_ORIGIN* coP,const COORD3* gorg,const COORD3* morg,const COORD3* sorg,double worg, double azm  );
short				dtcc_set_angle( ANGLE* aP, double v );
short				AlbersEngine( const PROJ_PARAMS* proP,   short mode, const COORD3* inptP,  COORD3* toP   );
short				CylindricalEqualAreaEngine ( const PROJ_PARAMS* proP,  short mode, const COORD3* inptP,  COORD3* toP );
short				LambertConformalConicEngine( const PROJ_PARAMS* proP, short mode, const COORD3* inptP, COORD3* toP  );
int					lambert_conformal_conic_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon );
short				LambertEqualAreaEngine( const PROJ_PARAMS* proP,short mode,const COORD3* inptP,COORD3* toP  );
short				MercatorEngine( const PROJ_PARAMS* proP,short mode, const COORD3* inptP,COORD3* toP );
short				OrthographicEngine( const PROJ_PARAMS* proP,short mode,const COORD3* inptP,COORD3* toP   );
short				RectangularEngine( const PROJ_PARAMS* proP,short mode,const COORD3* inptP,COORD3* toP );
short				StereographicEngine( const PROJ_PARAMS* proP, short mode, const COORD3* inptP, COORD3* toP );
short				TransverseMercatorEngine( const PROJ_PARAMS* proP,short mode,const COORD3* inptP,COORD3* toP );
short				NewZealandMapEngine( const PROJ_PARAMS* proP, short mode, const COORD3* inptP, COORD3* toP );
short				MadagascarLabordeEngine( const PROJ_PARAMS* proP,short mode,const COORD3* inptP,COORD3* toP );
short				UTMEngine( const PROJ_PARAMS* proP,short mode,const COORD3* inptP,COORD3* toP );
short				UPSEngine( const PROJ_PARAMS* proP,short mode,const COORD3* inptP,COORD3* toP );
short				MGRSEngine( const PROJ_PARAMS* proP,short mode,const COORD3* inptP,COORD3* toP );
const void*			_projection_engineP( const DTCC_LABELS* lblP );
short				dtcc_validate_origin(	COORD_ORIGIN* csP );
short				dtcc_validate_units(	const UNITS_INFO* uiP);
short				dtcc_set_coord3Z( COORD3* c3P, short v );
FILE*				dtcc_open_dat_file( const char* name, const char* mode );
int					_scan_hdr( FILE* fp, char* bfr, int* cnt );
const char*			_check_file_name( const char* fn, const char* dflt );
int					scanTo( FILE* fp, char s, int n, char* bfr );
short				_reset_ellipsoid( ELLIPS_PARAMS* elP, const char* name, int type );
void*				dtcc_check_bfr( void* bfrP, int n );
short				dtcc_projection_count( );
short				dtcc_set_coord3_units( 	COORD3_UNITS* cuP, short mode,double map_scale,const char* in1, const char* in2, const char* in3 );
short				_update_ellipsoid( ELLIPS_PARAMS* epP, const char* name, const char* abbrv,double axis, double rflat, short type );
short				_check_itcoordorigin( const COORD_ORIGIN* coP );
short				dtcc_set_projection_std_parallels( PROJ_PARAMS*  ppP,double        lower,double        upper   );
short				init_albers_equal_area(	void* cnstsP, const char* opt_name, double a, double rf, double lat0, double lon0, double std1, double std2 );
int					geo_to_albers_equal_area( const void* cnstsP, double lat, double lon, double* x, double* y );
void				dtcc_add_origin( const COORD_ORIGIN* csP, double* x, double* y );
void				dtcc_sub_origin( const COORD_ORIGIN* csP, double* x, double* y );
int					albers_equal_area_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon );
int					albers_equal_area_scale( const void* cnstsP, double lat, double lon, double* h, double* k );
int					albers_equal_area_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon );
short				map_projection_status( short n);
int					cylindrical_equal_area_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon );
int					cylindrical_equal_area_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon );
int					geo_to_cylindrical_equal_area( const void* cnstsP, double lat, double lon, double* x, double* y );
const void*			set_cylindrical_equal_area(	const char* opt_name, double a, double rf, double latS, double lon0, double h0 );
short				init_cylindrical_equal_area(	void* cnstsP, const char* opt_name, double a, double rf, double latS, double lon0, double h0 );
int					lambert_conformal_conic_scale( const void* cnstsP, double lat, double lon, double* h, double* k );
int					lambert_conformal_conic_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon );
int					geo_to_lambert_conformal_conic( const void* cnstsP, double lat, double lon, double* x, double* y );
const void*			set_lambert_conformal_conic(	const char* opt_name, double a, double rf, double lat0, double lon0, double std1, double std2 );
short				init_lambert_conformal_conic(	void* cnstsP, const char* opt_name, double a, double rf, double lat0, double lon0, double std1, double std2 );
short				init_lambert_equal_area( void* cnstsP, const char* opt_name, double a, double rf, double lat1, double lon0 );
const				void*	set_lambert_equal_area( const char* opt_name, double a, double rf, double lat1, double lon0 );
int					lambert_equal_area_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon );
int					lambert_equal_area_scale( const void* cnstsP, double lat, double lon, double* h, double* k );
int					lambert_equal_area_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon );
int					geo_to_lambert_equal_area( const void* cnstsP, double lat, double lon, double* x, double* y );
short				geo_to_netherlands_east_indies( const void* tcnsts, double lat, double lon, double* easting, double* northing	);
short				netherlands_east_indies_scale	( const void* tcnsts, double lat, double lon, double* h, double* k 	);
short				netherlands_east_indies_to_geo( const void* tcnsts, double easting, double northing, double* lat, double* lon );
const void*			set_netherlands_east_indies();
const void*			set_mercator(	const char* opt_name, double a, double rf, double lon0 );
short				init_mercator(	void* cnstsP, const char* opt_name, double a, double rf, double lon0 );
int					mercator_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon );
int					mercator_scale( const void* cnstsP, double lat, double lon, double* h, double* k );
int					mercator_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon );
int					geo_to_mercator( const void* cnstsP, double lat, double lon, double* x, double* y );
int					orthographic_scale( const void* cnstsP, double lat, double lon, double* h, double* k );
int					orthographic_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon );	
int					geo_to_orthographic( const void* cnstsP, double lat, double lon, double* x, double* y );
const void*			set_orthographic( const char* opt_name, double a, double rf, double lat0, double lon0 );
short				init_orthographic( void* cnstsP, const char* opt_name, double a, double rf, double lat0, double lon0 );
int					orthographic_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon );
int					rectangular_scale( const void* cnstsP, double lat, double lon, double* h, double* k );
int					rectangular_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon );	
int					geo_to_rectangular( const void* cnstsP, double lat, double lon, double* x, double* y );
const void*			set_rectangular(	const char* opt_name, double a, double rf, double lat0, double lon0 );
short				init_rectangular(	void* cnstsP, const char* opt_name, double a, double rf, double lat0, double lon0 );
int					rectangular_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon );

int					stereographic_scale( const void* cnstsP, double lat, double lon, double* h, double* k );
int					stereographic_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon );
int					geo_to_stereographic( const void* cnstsP, double lat, double lon, double* x, double* y );
const void*			set_stereographic( const char* opt_name, double a, double rf, double lat1, double lon0, double k0 );
short				init_stereographic( void* cnstsP, const char* opt_name, double a, double rf, double lat1, double lon0, double k0 );
int					stereographic_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon );

short				utm_scale( const void* tcnsts, double lat, double lon, double* h, double* k );
short				geo_to_utm( const void* tcnsts, double lat, double lon, long* izone, double* east, double* north );
short				utm_to_geo( const void* tcnsts, long zone, double easting, double northing, double* lat, double* lon );
const void*			set_utm( double a, double rf );
short				init_utm( void* cnstsP, double a, double rf );
int					utm_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon );

int					transverse_mercator_scale( const void* cnstsP, double lat, double lon, double* h, double* k );
int					transverse_mercator_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon );
int					geo_to_transverse_mercator( const void* cnstsP, double lat, double lon, double* x, double* y );
const void*			set_transverse_mercator(	const char* opt_name, double a, double rf, double lat0, double lon0, double k0 );
short				init_transverse_mercator(	void* cnstsP, const char* opt_name, double a, double rf, double lat0, double lon0, double k0 );
int					transverse_mercator_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon );

void				free_nzmg( const void* cnstsP );
short				geo_to_nzmg( const void* cnstsP, double lat, double lon, double* x, double* y );
short				nzmg_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon );
const void*			set_nzmg();
short				init_nzmg( void* cnstsP );
int					nzmg_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon );
void				free_madl( const void* cnstsP );
short				madl_to_geo( const void* cnstsP, double x, double y, double* lat, double *lon );
short				geo_to_madl( const void* cnstsP, double lat, double lon, double* x, double *y );
const void*			set_madl();
short				init_madl( void* cnstsP );
int					madl_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon );
short				ups_scale( const void* tcnsts, double lat, double lon, double* h, double* k );
short				geo_to_ups( const void* tcnsts, double lat, double lon, double* easting, double*  northing );
short				ups_to_geo( const void* tcnsts, double easting, double northing,double* lat, double* lon );
const void*			set_ups( double a, double rf, double lat0 );
short				init_ups( void* cnstsP, double a, double rf,double lat0 );
int					ups_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon );
short				polar_stereographic_scale( const void* cnstsH, double lat, double lon, double* h, double* k );
short				polar_stereographic_to_geo( const void* cnstsH, double x, double y, double* lat, double* lon );
short				geo_to_polar_stereographic( 	const void* cnstsH, double lat, double lon, double* x, double* y );
const void*			set_polar_stereographic( const char* ename, double a, double rf, double lat0, double lon0, double k0  );
short				init_polar_stereographic( void* cnstsP, const char* ename, double a, double rf, double lat0, double lon0, double k0 );
int					polar_stereographic_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon );
short				init_mgrs( void* cnstsP, double a, double rf );			
short				mgrs_to_geo( const void* tcnsts, const char* mgrs, double* lat, double* lon, long* izone, double* x, double* y );
short				geo_to_mgrs( const void* tcnsts, double lat, double lon, char* mgrs,long* zone, double* x, double* y );
short				dtcc_set_coord3A( COORD3* c3P, const char* v, short k );
int					mgrs_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon );
int					isFN(const char* fn );
short				dtcc_initialize_ellipsoid_list( const char* fname );
const ELLIPS_PARAMS* _find_name( DTCC_LABELS* idP, int type );
short				_reset_units(  UNITS_INFO* uiP,const char* name,int dim );
static double		_compute_ecc( double f );
short				dtcc_make_ellipsoid_name( ELLIPS_PARAMS* csP, char* nm );
short				dtcc_validate_ellipsoid(	const ELLIPS_PARAMS* csP);
short				_init_std_pars( proj_dfn* cnsts, int k, const char* name, double a, double rf, double lat0, double lon0  );
void				set_inverse_authalic( double e, double* aucoefs );
double				parallel_radius( double e, double sphi );
double				authalic_q( double e, double sphi );
double				square( double x );
int					projection_limit_check( const void *cnstsP, double lat, double lon );
void				sincostan( double a, double* sina, double* cosa, double* tana );
double				check_PI( double a );
void				inverse_authalic_lat( double B, double sinB, const double* aucoefs, double* phi );
void				set_inverse_rectifying( double e, double* fpcoefs );
void				set_meridian_distance( double e, double* mdcoefs );
double				meridian_distance( double phi, double sphi, const double* mdcoefs );
void*				allocate_projection( int k );
void				inverse_rectifying_lat( double R, double sinR, const double* fpcoefs, double* phi );
void				authalic_lat( double e, double phi, double sphi, double* B, double* sinB );
void				set_inverse_conformal( double e, double* cfcoefs );
double				tanz( double ecc, double phi, double sphi );
void				free_projection( const void* dfnP );
void				r_and_theta( int which, double x, double y, double* r, double* t );
double				affix_sign( double x, double sgn );
void				inverse_conformal_lat( double X, double sinX, const double* cfcoefs, double* phi );
double				tanHZ( double e, double sphi );
double				vertical_radius( double e, double sphi );
void				conformal_lat( double e, double phi, double sphi, double* X  );
double				poly( double x, int n, double* c );
void				nzmggp(double a, double ophi, double olam, double fn, double fe,double *sphi, double *slam, double y, double x);
void				gpnzmg(double a, double ophi, double olam, double fn, double fe, double sphi, double slam, double *y, double *x);
double				check_360( double a );
void				gpmadl(double a,double e,double sphi,double slam, double *y,double *x,double *ygs,double *xgs,double ophis,double olam,double fn,double fe,double ok,double al,double Const,double r,double ca,double cb );
void				madlgp(double a,double e,double *sphi,double *slam,double y,double x,double *ygs,double *xgs,double ophi,double ophis,double olam,double fn,double fe,double ok,double al,double Const,double r,double ca, double cb, double cc, double cd);
short				select_group( double a, double rf );
short				_mgrtxx( const proj_dfn* cnsts, char* mgrs, double *lat, double *lon, long *izone, double *y, double *x );
short				mgrs_limit_check( const char* mgrs, double lat, double lon );
void				xxtmgr( const proj_dfn* cnsts, double *lat, double *lon, long *izone, double *y, double *x, char *mgrs, int mode );
const char*			_check_ellipsoid_name( const char* fn );
short				dtcc_delete_ellipsoid_list( _DTCC_LIST** lstP );
short				dtcc_scan_ellipsoid( ELLIPS_PARAMS* ellp, FILE* fp );
int					_findIt( const char* name  );
short				dtcc_is_ellipsoid_axis_OK(	double axis );
short				dtcc_is_ellipsoid_flattening_OK(  double rflat );
short				_check_itellips( ELLIPS_PARAMS* dtccP );
double				eccentricity2( double rf );
void				_convertSin2468( double* u );
double				_fSin2468( double sphi, const double* u );
double				constN( double e );
DCOMPLEX			dpxdpx( double d1, double d2);
DCOMPLEX			dpxlongpow( DCOMPLEX a, long b);
DCOMPLEX			dpxmul( DCOMPLEX a, DCOMPLEX b);
DCOMPLEX			dpxadd( DCOMPLEX a, DCOMPLEX b);
DCOMPLEX			dpxdbl( double dbl);
DCOMPLEX			dpxdiv( DCOMPLEX a, DCOMPLEX b);
double				dpxreal( DCOMPLEX a );
double				dpxima( DCOMPLEX a );
void				ftnsac( char* s1, int n1, char* s2, int n2);
void				shiftr( char *mgrs, int P1, long n, long *nchar);
void				utmlim(long *n, double sphi, long izone, double *spsou, double *spnor,double *sleast, double *slwest);
extern void			utmlimMGRS( long *n, double lat, long izone, double *spsou, double *spnor, double *sleast, double *slwest);
double				fifdnint( double a);
//void				utmset( long igroup, long izone, long *ltrlow, long *ltrhi, double *fnltr );
extern void			utmsetMGRS( long igroup, long izone, long *ltrlow, long *ltrhi, double *fnltr );
long				get_group( long group, long zone );
void				upnset( long n, long *ltrlow, long *ltrhi, double *feltr, double *fnltr, long *ltrhy);
void				upsset( long n, long *ltrlow, long *ltrhi, double *feltr, double *fnltr, long *ltrhy);
void				milref( const proj_dfn* cnsts, long *iarea, double *lat,long *izone, double y, double x, char *mgrs  );
short				_write_ellipsoid_list(_DTCC_LIST* lstP);
short				dtcc_units_count(void);
double				fifdint( double a);
long				fifmod( long num, long dem);
long				fifnint( double a);
FILE*				_write_list_hdr( const _DTCC_LIST* lstP, short count );
short				_elist_count( int k, _DTCC_LIST* listP );
short				_accept_it( const ELLIPS_PARAMS* elP );
short				dtcc_write_ellipsoid( const ELLIPS_PARAMS* ep,FILE* fp  );
short				dtcc_write_label( const void* dtccP, FILE* fp );
short				dtcc_write_double8( FILE* fp, double dble );
ERRSTATUS			mapgeo_destruct(FILE *fp, bool pointer, bool destruct, MAPGEO **ptr);
ERRSTATUS			decode_mapgeo(unsigned char *record, MAPGEO *ptr);
short				dtcc_read_system(  COORD_SYS* cpP, FILE* fp );
ERRSTATUS			extent_construct(FILE *fp, defEXTENT **ptr);
ERRSTATUS			plot_params_construct(FILE *fp, PLOT_PARAMS **ptr);
ERRSTATUS			mapgeo_ini(FILE *fp, MAPGEO *mapgeo_ptr);
ERRSTATUS			plot_params_destruct(FILE *fp, bool pointer, bool destruct, PLOT_PARAMS **ptr);
ERRSTATUS			decode_plot_params(unsigned char *record, PLOT_PARAMS *ptr);
ERRSTATUS			plot_params_ini(FILE *fp, PLOT_PARAMS *plot_params_ptr);
ERRSTATUS			char_to_ushort(unsigned char *record, unsigned short *s, short big_endian, long *c);
ERRSTATUS			ushort_to_char(unsigned char *record, unsigned short *s, short big_endian, long *c);
ERRSTATUS			encode_plot_params(unsigned char *record, PLOT_PARAMS *ptr);
short				dtcc_get_coord3(	const COORD3* c3P,double *uP, double *vP, double *wP,short* zoneP );
ERRSTATUS			extent_destruct(FILE *fp, bool pointer, bool destruct, defEXTENT **ptr);
ERRSTATUS			extent_ini(FILE *fp, defEXTENT *extent_ptr);
ERRSTATUS			encode_extent(unsigned char *record, defEXTENT *ptr);
ERRSTATUS			encode_mapgeo(unsigned char *record, MAPGEO *ptr);
short				dtcc_read_label ( void* dtccP, FILE* fp );
short 				dtcc_read_short( FILE* fp, short endian );
short				dtcc_read_coord_group( COORD_GROUP* cg1P,FILE* fp);
short				dtcc_read_datum( DATUM_PARAMS* dpP,FILE* fp);
short				dtcc_read_projection( PROJ_PARAMS* ppP,FILE* fp);
short				dtcc_read_extents( COORD_EXTENTS* ceP,FILE* fp );
short				dtcc_read_coord3_units( COORD3_UNITS* cuP, FILE* fp );
short				dtcc_swab2N( short* A, short n );
short				dtcc_decode_coord_group( 	COORD_GROUP* cgP,char* labelP) ;
short 				dtcc_read_shortN( 	FILE* fp, short* bfrP, short N, short endian );
short				dtcc_read_ellipsoid( ELLIPS_PARAMS* ep,FILE* fp );
short				dtcc_read_origin( COORD_ORIGIN* coP, FILE* fp   );
double 				dtcc_read_double8( FILE* fp, short endian );
//short				_update( 	UNITS_INFO* uiP,const DTCC_LABELS*	label,double cf, short scaled );
short				_updateProjection(	PROJ_PARAMS* ppP,const char* name, const ELLIPS_PARAMS* elP, const COORD_ORIGIN* orgP,const double* stdP );
short				dtcc_read_coord3( COORD3* c3P,FILE* fp,short endian  );
short				dtcc_read_units(	UNITS_INFO* uiP,FILE* fp );
short				dtcc_swab8N( long* A, short n );
short				dtcc_copy_origin( COORD_ORIGIN* coP1,const COORD_ORIGIN* coP2  );
short				dtcc_swab4N( short* A, short n );
const char*			_check_system_name( const char* fn );
short				dtcc_scan_system( COORD_SYS* sysP, FILE* fp );
short				dtcc_reset_coord_group(	COORD_GROUP* cgP,const char*	label);
short				dtcc_set_extents( COORD_EXTENTS* ceP,const COORD3* mngridP,const COORD3* mxgridP,const COORD3* mngeoP,const COORD3* mxgeoP  );
short				dtcc_set_projection_origin( PROJ_PARAMS*  ppP,const COORD_ORIGIN* orgP  );
short				dtcc_set_grid_extents( COORD_EXTENTS* ceP,const double* mnP,const double* mxP );
short				dtcc_set_geo_extents( COORD_EXTENTS* ceP,const double* mnP,const double* mxP );
short				dtcc_initialize_lists(	const char* ename, const char* dname, const char* sname );
short				dtcc_delete_lists(void);
short				dtcc_delete_system_list( _DTCC_LIST** lstP );
short				_write_system_list(_DTCC_LIST* lstP);
FILE*				_write_listASCII_hdr( const _DTCC_LIST* lstP, short count );
short				_accept_itcoordsys( const COORD_SYS* csP );
short				dtcc_writeASCII_system( const COORD_SYS* cpP, FILE* fp );
void				_encode( double v, char* bfr );
short				_dlist_count( int k, _DTCC_LIST* listP );

bool				Intersect2D( float rX1, float rY1, float rX2, float rY2, float rX3, float rY3, float rX4, float rY4, float* rXInters, float* rYInters );
bool				IsInside2D( float rX, float rY, vector<MT_Vector3D>& vListLines_ );

extern int			ftncms ( char *s1 , int n1 , char *s2 , int n2 );
extern long			fifidint ( double a );
extern ERRSTATUS	short_to_char(unsigned char *record, short *s, short big_endian, long *c);
extern ERRSTATUS	long_to_char(unsigned char *record, long *l, short big_endian, long *c);
extern ERRSTATUS	double_to_char(unsigned char *record, double *d, short big_endian, long *c);
extern ERRSTATUS	char_to_short(unsigned char *record, short *s, short big_endian, long *c);
extern ERRSTATUS	char_to_long(unsigned char *record, long *l, short big_endian, long *c);
extern int	char_to_double(unsigned char *record, double *d, short big_endian, long *c);

struct _PROJECTION_PARS PROJECTION_LIST[] =
	{   "Albers", "ALB", AlbersEngine, DTCC_DISPLAYBLE_LIST+DTCC_SIMPLE_PROJ_LIST,	
		/* "Azimuthal Equidistant",			"AED",	
			AzimuthalEquidistantEngine,	DTCC_DISPLAYBLE_LIST+DTCC_SIMPLE_PROJ_LIST,	*/
		"Cylindrical Equal Area",			"CEA",	
			CylindricalEqualAreaEngine,	DTCC_DISPLAYBLE_LIST+DTCC_SIMPLE_PROJ_LIST,
		/* "Equidistant Conic",                "EDC",	
			EquidistantConicEngine, DTCC_DISPLAYBLE_LIST+DTCC_SIMPLE_PROJ_LIST,	*/
        "Lambert Conformal Conic",			"LCC",	
        	LambertConformalConicEngine, DTCC_DISPLAYBLE_LIST+DTCC_SIMPLE_PROJ_LIST,
        "Lambert Equal Area",              	"LEA",	
        	LambertEqualAreaEngine, DTCC_DISPLAYBLE_LIST+DTCC_SIMPLE_PROJ_LIST,
        "Mercator",                    		"MRC",	
        	MercatorEngine,	DTCC_DISPLAYBLE_LIST+DTCC_SIMPLE_PROJ_LIST,
        /* "Oblique Mercator",      			"OMA",	
        	ObliqueMercatorEngine,	   		1, */
        "Orthographic",                    	"ORT",	
        	OrthographicEngine,	DTCC_DISPLAYBLE_LIST+DTCC_SIMPLE_PROJ_LIST,
        "Rectangular",                    	"REC",	
        	RectangularEngine, DTCC_DISPLAYBLE_LIST+DTCC_SIMPLE_PROJ_LIST,
        "Stereographic",              		"STE",	
        	StereographicEngine, DTCC_DISPLAYBLE_LIST+DTCC_SIMPLE_PROJ_LIST,
        "Transverse Mercator",     			"TMR",	
        	TransverseMercatorEngine,DTCC_DISPLAYBLE_LIST+DTCC_SIMPLE_PROJ_LIST,
        
        "New Zealand Map Grid",    			"NZMG",	
        	NewZealandMapEngine, DTCC_DISPLAYBLE_LIST,
        "Laborde",    						"MADL",	
        	MadagascarLabordeEngine, DTCC_DISPLAYBLE_LIST,

        "Universal Transverse Mercator",    "UTM",	
        	UTMEngine,	DTCC_GRID_LIST,
        "Universal Polar Stereographic",	"UPS",  
        	UPSEngine,	DTCC_GRID_LIST,
        "Military Grid Reference System",	"MGRS", 
        	MGRSEngine,	DTCC_GRID_LIST
   };


static struct XUNITS_LIST _std_units[] =
	/*	  NAME			    ABBRV 	FMT	     CF		  VALID		TYPE	  SCALED */ 
	{	"meters",    		"m",	"12.2",	1.0,    	1,	DTCC_SHORT_LINEAR,	0,
        "kilometers",     	"km",	"12.5",	1000.0, 	1,	DTCC_LINEAR,		0,
        "feet",           	"ft",   "12.3",	METFOOT,  	1,	DTCC_LINEAR,		0,
        "yards",          	"yd",   "12.2",	METYARD,	1,	DTCC_LINEAR,		0,
        "statute miles",  	"mi",   "12.5",	METSMILE, 	1,	DTCC_LINEAR,		0,
        "nautical miles", 	"nm",   "12.5",	METNMILE, 	1,	DTCC_LINEAR,		0,
        "fathoms",        	"fm",   "12.2",	METFATHOM, 	1,	DTCC_LINEAR,		0,
        "dimensionless",  	"",		"12.3",	-1.0,   	1,	DTCC_LINEAR,		0,       
		"degrees",     		"dd",	"9.5",	1.0,   		1,	DTCC_SHORT_ANGLE,	0,
        "minutes",     		"mn",	"13.5",	1.0/60.0, 	1,	DTCC_ANGULAR,		0,
        "seconds",     		"s",	"20.3", 1.0/3600.0, 1,	DTCC_ANGULAR,		0,
        "sec/10",      		"s10",	"16.5", 1.0/36000.0,1,	DTCC_ANGULAR,		0,
        "radians",     		"r",	"12.8",	DEGRAD,  	1,	DTCC_ANGULAR,		0,
        "grads",       		"g",	"12.6",	DEGGRAD,  	1,	DTCC_ANGULAR,		0,
        "degrees & minutes","dm",   "7.3",	RADDM,  	1,	DTCC_SHORT_ANGLE,	0,
        "degrees, minutes & seconds",         		
        					"dms",	"5.1",	RADDMS, 	1,	DTCC_SHORT_ANGLE,	0
   };

DCOMPLEX bf[6] = {
    {0.7557853228,0.0},{0.249204646,3.371507E-3},{-1.541739E-3,4.105856E-2},
    {-0.10162907,1.727609E-2},{-0.26623489,-0.36249218},{-0.6870983,-1.1651967}
};

double tpsi[10] = {
    0.6399175073,-0.1358797613,6.3294409E-2,-2.526853E-2,1.17879E-2,-5.5161E-3,
    2.6906E-3,-1.333E-3,6.7E-4,-3.4E-4
};

static defComgrda Tomgrda;
static defComgrch Tomgrch;

bool				pacifyVmapConv(char *string, short percent, bool bRender = false);

bool pacifyVmapConv(char *string, short percent, bool bRender )
{
//	return UpdateProgressVmap( (int)percent, string, bRender );
//	OutputDebugString( (char *) string );
//	OutputDebugString( (char *) "\n" );
//	OutputDebugString( (char *) "\n" );
//    NOREF(string);
//    NOREF(percent);
	return true;
}

void iErrorVmapConv (char *fmt, ...)
{
   va_list args;
   va_start (args, fmt);

//   OutputDebugString( (const char *) "\n" );
//   printf ("\n"); fflush(stdout);
   vprintf (fmt,args);
   printf( "[iErrorVmapConv] %s\n", fmt );
//   printf ("\n"); fflush(stdout);
//   OutputDebugString( (const char *) fmt );
//   OutputDebugString( (const char *) "\n" );
//   OutputDebugString( (const char *) "\n" );
   va_end(args);
}

void* zMallocConv( size_t n )
{

	void* ptr = malloc( n );
	memset( ptr, 0, n );
	return ptr;

}

USER_PARAMS*						params = NULL;

ERRSTATUS char_to_long(unsigned char *record, long *l, short big_endian, long *c)
{
    long            i;
    union
    {				/* the long & character union          */
	unsigned long   l;
	unsigned char   c[4];
    }               lngchr;

    for (i = 0; i < 4; i++)
	if (big_endian)
	    lngchr.c[3 - i] = record[(*c)++];
	else
	    lngchr.c[i] = record[(*c)++];

    *l = lngchr.l;
    return STAT_SUCCESS;
}

ERRSTATUS char_to_short(unsigned char *record, short *s, short big_endian, long *c)
{
    long            i;
    union
    {				/* the short & character union          */
	unsigned short  s;
	unsigned char   c[2];
    }               shtchr;

    for (i = 0; i < 2; i++)
	if (big_endian)
	    shtchr.c[1 - i] = record[(*c)++];
	else
	    shtchr.c[i] = record[(*c)++];

    *s = shtchr.s;
    return STAT_SUCCESS;
}


/* the type of conversion 1 = IEEE to VAX 2 =
* VAX to IEEE 3 = IEEE to SUN 4 = SUN to
* IEEE 5 = SUN to VAX 6 = VAX to SUN 7 =
* IEEE to MAC 8 = MAC to IEEE                      */

void convert_double(unsigned char c[8], long type)

{
    int             i;
    unsigned long   exp;
    struct
    {                           /* the double number               */
	unsigned int    sgn:1;
	unsigned int    exp1:7;
	unsigned int    exp2:4;
	unsigned int    mant1:4;
	unsigned int    mant2:16;
	unsigned int    mant3:16;
	unsigned int    mant4:16;
    }               IEEE_dbl;

    /*
     * struct                      * the VAX double number                * {
     * unsigned int sgn:1; unsigned int exp1:7; unsigned int exp2:1; unsigned
     * int mant1:7; unsigned int mant2:16; unsigned int mant3:16; unsigned
     * int mant4:16; } VAX_dbl;
     */

    /*
     * struct                      * the MAC double number                * {
     * unsigned int sgn:1; unsigned int exp1:7; unsigned int exp2:1; unsigned
     * int exp3:7; unsigned int mant1:16; unsigned int mant2:16; unsigned int
     * mant3:16; unsigned int mant4:16; unsigned int mant5:16; } MAC_dbl;
     */
    union
    {                           /* the short & character union          */
	unsigned short  s;
	unsigned char   c[2];
    }               shtchr;
    unsigned char   tc;         /* the temp character value             */

    /* Reverse bytes if IEEE */

    if (type == 1 || type == 3)
	for (i = 0; i < 4; i++)
	{
	    tc = c[i];
	    c[i] = c[7 - i];
	    c[7 - i] = tc;
	}

    /*
     * If from VAX switch bytes, put into switch union, add to the exponent,
     * and put into swithc union
     */

    if (type == 2 || type == 6)
    {

	for (i = 0; i < 4; i++)
	{
	    tc = c[i * 2];
	    c[i * 2] = c[i * 2 + 1];
	    c[i * 2 + 1] = tc;
	}

	IEEE_dbl.sgn = (c[0] >> 7) & 1;
	exp = ((c[0] & 127) << 1) + ((c[1] >> 7) & 1) + 1023 - 129;
	/*printf("exp = %ld\n", exp); */
	IEEE_dbl.exp1 = (exp >> 4) & 127;
	IEEE_dbl.exp2 = exp & 15;
	IEEE_dbl.mant1 = ((c[1] & 127) >> 3) & 15;
	IEEE_dbl.mant2 = ((c[1] & 7) << 13) + (c[2] << 5)
		  + ((c[3] >> 3) & 31);
	IEEE_dbl.mant3 = ((c[3] & 7) << 13) + (c[4] << 5)
		  + ((c[5] >> 3) & 31);
	IEEE_dbl.mant4 = ((c[5] & 7) << 13) + (c[6] << 5)
		  + ((c[7] >> 3) & 31);

	c[0] = (IEEE_dbl.sgn << 7) + IEEE_dbl.exp1;
	c[1] = (IEEE_dbl.exp2 << 4) + IEEE_dbl.mant1;
	shtchr.s = IEEE_dbl.mant2;
	c[2] = shtchr.c[1];
	c[3] = shtchr.c[0];
	shtchr.s = IEEE_dbl.mant3;
	c[4] = shtchr.c[1];
	c[5] = shtchr.c[0];
	shtchr.s = IEEE_dbl.mant4;
	c[6] = shtchr.c[1];
	c[7] = shtchr.c[0];

    }

    /*
     * If to VAX put in switch union, subtract from the exponent, put into
     * switch union, and switch bytes
     */

    if (type == 1 || type == 5)
    {

	/*
	 * for (i=0; i<8; i++) IEEEdbl.c[i] = c[i];
	 * 
	 * IEEEdbl.d.exp -= 1023 - 129;
	 * 
	 * VAXdbl.d.sgn = IEEEdbl.d.sgn; VAXdbl.d.exp = IEEEdbl.d.exp;
	 * VAXdbl.d.mant1 = IEEEdbl.d.mant1; VAXdbl.d.mant2 =
	 * IEEEdbl.d.mant2; VAXdbl.d.mant3 = IEEEdbl.d.mant3; VAXdbl.d.mant4
	 * = IEEEdbl.d.mant4; VAXdbl.d.mant5 = 0;
	 * 
	 * for (i=0; i<8; i++) c[i] = VAXdbl.c[i]; for (i=0; i<4; i++) { tc =
	 * c[i*2]; c[i*2] = c[i*2+1]; c[i*2+1] = tc; }
	 */
    }
    /* Reverse bytes if to VAX to IEEE */

    if (type == 2 || type == 4)
	for (i = 0; i < 4; i++)
	{
	    tc = c[i];
	    c[i] = c[7 - i];
	    c[7 - i] = tc;
	}

}

int char_to_double(unsigned char *record, double *d, short big_endian, long *c)
{
    long            i;
    union
    {				/* the double & character union         */

	double          d;
	unsigned char   c[8];
    }               dblchr;

    if (big_endian)
	convert_double(&record[(*c)], (long) 3);

    for (i = 0; i < 8; i++)
	dblchr.c[i] = record[(*c)++];

    *d = dblchr.d;
    return STAT_SUCCESS;
}


ERRSTATUS bit_to_geo_for_data(MAPGEO * mapgeo,double bitx, double bity,double * londd, double * latdd)
{
    short       status = VMAPOK;
    double      px, py, px_shifted;
//	char        err_mess[100], dtcc_mess[40];
	COORD3 map_point, dd_point;
	/**WR Jan 16 */
 	double     shift = 0.0;

    /* compute projection coords from plotting coords */
    px = (bitx - mapgeo->plot_params.tx) / mapgeo->plot_params.sx;
    py = (bity - mapgeo->plot_params.ty) / mapgeo->plot_params.sy;

	/* mdtcc 'grid' is UPS, UTM, MGRS, and GRID, but UTM and MGRS are not displayable */ 
	if( dtcc_is_system_grid(&mapgeo->map_display_coord_sys) )
	{
  	   if( strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Universal Polar Stereographic South 84") ==  0 )
	       py = -1.0 * fabs(py);
       else if( (strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys),"Irish Transverse Mercator") ==  0) &&
				( py < 0) )
	       py = -1.0 * (py + 1000000.0);


	   dtcc_set_coord3_grid( &map_point, px, py, DOUBLE_NULL, 
		                      dtcc_system_type(&mapgeo->map_display_coord_sys) );
	}
	else   /* GP system */
	{
	   /**WR Jan 08 */
	   px_shifted = px;
       shift = geo_360_to_180( &px_shifted);
	   dtcc_set_coord3_geo( &map_point, py, px_shifted, DOUBLE_NULL );
	}

	status = dtcc_transform(mapgeo->display_to_data, (const void *)&map_point, (void *)&dd_point);
    if( status >= VMAPOK ) /* || 
     (fabs(dtcc_coord3_lat(&map_point))<=90.0) && (fabs(dtcc_coord3_lon(&map_point))<=180.0) */
	{
 	    *latdd = dtcc_coord3_lat(&dd_point);
	    *londd = dtcc_coord3_lon(&dd_point);
	    /**WR TEMP: dt() can shift lon across dateline--check for this*/
	    /**WR Jan 16 */
        /* if( shift ) 
			geo_180_to_360(londd); */
	     *londd -= shift;
	}
    if( status != VMAPOK ) 
    {	
		if( status < 0)
            mapgeo->bit_to_geo_errors++;   
        if( !mapgeo->hide_transform_err_messg ) 
        {  
/*            dtcc_get_status_message( (short)status, dtcc_mess );   
            sprintf(err_mess, "Coordinate conversion problem -- %s. Keep showing error messages? ",dtcc_mess);
		    switch (muse_ask3 ("Show", "Don't Show", NULL, err_mess ) )
		    {
	        case RESP_DEFAULT:	
			    break;
	        case RESP_2:
			    mapgeo->hide_transform_err_messg = true;
			    break;
	        default:		
			    muse_fatal ("invalid return from muse_ask3 ");
		    }
*/
        }
	}
    return (ERRSTATUS)status;
}

ERRSTATUS short_to_char(unsigned char *record, short *s, short big_endian, long *c)
{
    long            i;
    union
    {				/* the short & character union          */
	unsigned short  s;
	unsigned char   c[2];
    }               shtchr;

    shtchr.s = *s;
    for (i = 0; i < 2; i++)
	if (big_endian)
	    record[(*c)++] = shtchr.c[1 - i];
	else
	    record[(*c)++] = shtchr.c[i];

    return STAT_SUCCESS;
}

/* the type of conversion 1 = IEEE to MAC  2 = MAC to IEEE     */
void convert_double_mac10(unsigned char c[10], long type)
{
//    int             i;
    unsigned short exp;
    unsigned short expsign;
    struct
    {                           /* the double number               */
        unsigned char sgn:1;
        unsigned char exp0:7;
        unsigned char exp1:4;
        unsigned char mant0:4;
        unsigned char mant1:8;
        unsigned char mant2:8;
        unsigned char mant3:8;
        unsigned char mant4:8;
        unsigned char mant5:8;
        unsigned char mant6:8;
    } IEEE_dbl;

    struct  
    {                           /* the MAC 10 byte double number        */
        unsigned char sgn:1;
        unsigned char exp0:7;
        unsigned char exp1:8;
        unsigned char intb:1;
        unsigned char mant0:7;
        unsigned char mant1:8;
        unsigned char mant2:8;
        unsigned char mant3:8;
        unsigned char mant4:8;
        unsigned char mant5:8;
        unsigned char mant6:8;
        unsigned char mant7:8;
    } MAC10_dbl;

/*
    union
    {                           // the short & character union
        unsigned short  s;
        unsigned char   c[2];
    }shtchr;
*/

//    unsigned char   tc;         /* the temp character value             */


    /* IEEE to MAC10 */

    if (type == 1)
    {
       MAC10_dbl.sgn   = (c[0] >> 7) & 1;
       exp = ((c[0] & 63) << 4) + ((c[1] >> 4) & 15);
/*       printf("exp = %ld\n", (unsigned short)exp);*/
       expsign = ((c[0] >> 6) & 1);
       if (expsign == 0)
          exp += (15 << 10);
       MAC10_dbl.exp0  = ((exp >> 8) & 127) + (expsign << 6);
       MAC10_dbl.exp1  = (exp & 255);
       MAC10_dbl.intb = 1;
       MAC10_dbl.mant0 = ((c[1] & 15) << 3) + ((c[2] >> 5) & 7);
       MAC10_dbl.mant1 = ((c[2] & 31) << 3) + ((c[3] >> 5) & 7);
       MAC10_dbl.mant2 = ((c[3] & 31) << 3) + ((c[4] >> 5) & 7);
       MAC10_dbl.mant3 = ((c[4] & 31) << 3) + ((c[5] >> 5) & 7);
       MAC10_dbl.mant4 = ((c[5] & 31) << 3) + ((c[6] >> 5) & 7);
       MAC10_dbl.mant5 = ((c[6] & 31) << 3) + ((c[7] >> 5) & 7);
       MAC10_dbl.mant6 = ((c[7] & 31) << 3) + 0;
       MAC10_dbl.mant7 = 0;
       c[0] = (MAC10_dbl.sgn << 7) + MAC10_dbl.exp0;
       c[1] = MAC10_dbl.exp1;
       c[2] = (MAC10_dbl.intb << 7) + MAC10_dbl.mant0;
       c[3] = MAC10_dbl.mant1;
       c[4] = MAC10_dbl.mant2;
       c[5] = MAC10_dbl.mant3;
       c[6] = MAC10_dbl.mant4;
       c[7] = MAC10_dbl.mant5;
       c[8] = MAC10_dbl.mant6;
       c[9] = MAC10_dbl.mant7;
    }

    /* MAC10 to IEEE */

    if (type == 2)
    {
       IEEE_dbl.sgn   = (c[0] >> 7) & 1;
       expsign = ((c[0] >> 6) & 1);
       exp = ((c[0] & 3) << 8) + c[1];
/*       printf("MtoI exp  = %ld\n", exp);*/
       IEEE_dbl.exp0  = ((exp >> 4) & 63) + (expsign << 6);
       IEEE_dbl.exp1  = (exp & 15);
       IEEE_dbl.mant0 = ((c[2] >> 3) & 15);
       IEEE_dbl.mant1 = ((c[2] & 7) << 5) + ((c[3] >> 3) & 31);
       IEEE_dbl.mant2 = ((c[3] & 7) << 5) + ((c[4] >> 3) & 31);
       IEEE_dbl.mant3 = ((c[4] & 7) << 5) + ((c[5] >> 3) & 31);
       IEEE_dbl.mant4 = ((c[5] & 7) << 5) + ((c[6] >> 3) & 31);
       IEEE_dbl.mant5 = ((c[6] & 7) << 5) + ((c[7] >> 3) & 31);
       IEEE_dbl.mant6 = ((c[7] & 7) << 5) + ((c[8] >> 3) & 31);

       c[0] = (IEEE_dbl.sgn << 7) + IEEE_dbl.exp0;
       c[1] = (IEEE_dbl.exp1 << 4) + IEEE_dbl.mant0;
       c[2] = IEEE_dbl.mant1;
       c[3] = IEEE_dbl.mant2;
       c[4] = IEEE_dbl.mant3;
       c[5] = IEEE_dbl.mant4;
       c[6] = IEEE_dbl.mant5;
       c[7] = IEEE_dbl.mant6;
       c[8] = 0;
       c[9] = 0;
    }

}

void ftnsac ( char *s1 , int n1 , char *s2 , int n2 )
{
	if (n2 > n1)
		n2 = n1;

	memcpy(s1, s2, n2);
	if (n1 > n2)
		memset(s1 + n2, ' ', n1 - n2);
}


void shiftr( char *mgrs, int P1, long n, long *nchar)
/*
     *****************************************************************
     *  A.  CLASSIFIICATION:                                         *
     *      1. SOURCE CODE:  UNCLASSIFIED                            *
     *      2. PROCESSED DATA:  UNCLASSIFIED                         *
     *                                                               *
     *  B.  PURPOSE:                                                 *
     *      TO SHIFT THE CHARACTERS OF CHARACTER*20 VARIABLE 'N'     *
     *      PLACES TO RIGHT (EXAMPLES GEOREF -OR- MGRS)              *
     *                                                               *
     *  C.  EXTERNAL INTERFACES                                      *
     *      1.  ARGUMENTS, COMMON BLOCKS, INPUT AND OUTPUT           *
     *          **SEE VARIABLE LIST, ETC BELOW**                     *
     *      2.  CALLING ROUTINES - VARTIONS TO INCL MGRTGP,GREFGP    *
     *      3.  CALLED ROUTINES - NONE                               *
     *                                                               *
     *  D.  USEAGE                                                   *
     *      1.  HOST REQUIREMENTS:  FORTRAN 77                       *
     *      2.  RESTRICTIONS:  NONE                                  *
     *      3.  RECOMPILATION PROCEDURES:  @FTN  SHIFTR              *
     *      4.  ERROR MESSAGES:  NONE                                *
     *                                                               *
     *  E.  HISTORICAL                                               *
     *      1.  FERERATOR:  DEFENSE MAPPING AFERCY                   *
     *                      HYDROGRAPHIC/TOPOGRAPHIC CENTER          *
     *                      6500 BROOKES LANE                        *
     *                      WASHINGTON, D.C.  20315-0030             *
     *                                                               *
     *                      MR. BRADFORD W. DREW - SDAT              *
     *                                                               *
     *      2.  SYSTEM:  DATA FERERAL ECLIPSE  S/140 (AOS)           *
     *                                                               *
     *      3.  DATE:   2 NOV 87                                     *
     *                                                               *
     *      4.  UPDATES                                              *
     *                                                               *
     *****************************************************************
     *
     *
     *****
     *    VARIABLE LIST
     *
     *    TY = TYPE WHERE:  CH = CHARACTER
     *                      DP = DOUBLE PRECISION
     *                      IN = INTEGER
     *                      RE = REAL
     *
     *   REF = REFERENCE FOR VARIABLE OUTSIDE PROGRAM UNIT
     *         WHERE:  COM = COMMON STATEMENT
     *                 SUB = SUBROUTINE CALL STATEMENT
     *
     *NAME  TY REF      EXPLANATION
     ------ -- --- ---------------------------------------------------
     I      IN     WORK VARIABLE
     K      IN     WORK VARIABLE
     MGRS   CH SUB ANY CHARACTER*20 VARIABLE (EXAMPLES MGRS, GEOREF)
     N      IN SUB NUMBER OF CHARACTERS TO SHIFT MGRS TO RIGHT
     NCHAR  IN SUB ACTUAL NUMBER OF CHARACTERS MAKING UP MGRS
     *****************************************************************
     COMMON BLOCKS REFERENCED
     NAME    REMARKS
     ------  ---------------------------------------------------------
     *NONE*
     *****************************************************************
     SUBROUTINES REFERENCED
     NAME    REMARKS
     ------  ---------------------------------------------------------
     *NONE*
     *****************************************************************
     ************************************************************
     ***   THIS SUBROUTINE SHIFTS THE CHARACTERS IN MGRS      ***
     ***         (ANY CHARACTER*20 VARIABLE)                  ***
     ***          'N' PLACES TO THE RIGHT                     ***
     ***                                                      ***
     *** NCHAR = NUMBER OF CHARACTERS IN MGRS                 ***
     ************************************************************
*/
{
static long i,k,D2,D3;

    P1 = P1;
    for(i=1,D2=1,D3=(*nchar-i+D2)/D2; D3>0; D3--,i+=D2) {
        k = *nchar-i+1;
        ftnsac((mgrs+(short)(k+n)-1),(short)(k+n)-((short)(k+n)-1),(mgrs+(short)
          k-1),(short)k-((short)k-1));
    }
/*
     ****** ADJUST THE NUMBER OF CHARACTERS *****
*/
    *nchar += n;
    return;
}


void utmlimMGRS( long *n, double lat, long izone, double *spsou, double *spnor, double *sleast, double *slwest)
{   double rnd1 = 0.1;
    double zero = 0.0;
  	double 	r3 = 3.0;
    double  r8 = 8.0;
    double  r9 = 9.0;
    double  r21 = 21.0;
    double  r33 = 33.0;
    double  r80 = 80.0;
	double  slcm;
	long ilat;
    long icm;
/*
     *** 1ST LETTER NUMBER FOR MGRS ***
*/
    if(*n > 0) goto S40;
/*
     *** DERIVE 1ST LETTER NUMBER OF MGRS FROM ***
     *** LATITUDE AS BROKEN DOWN INTO 8 DEGREE BANDS ***
     *** STARTING WITH LETTER #3 (C) AT 80 DEGREES SOUTH LATITUDE ***
*/
    *n = fifidint((lat+r80)/r8)+3;
/*
     ** CORRECT FOR LETTERS I & O **
*/
    if(*n > 8) *n += 1;
    if(*n > 14) *n += 1;
/*
     ** LETTER 'Y' (25) WILL BE DERIVED FOR LATITUDES 80-84N
     ** RESET TO LETTER 'X' (24) WHICH HAS BEEN MADE
     ** 12 DEGREES IN SIZE IN THE NORTH-SOUTH DIRECTION
*/
    if(*n >= 25) *n = 24;
/*
     ** LATITUDE OF ZERO SHOULD BE MADE LETTER 'N' (14) **
*/
    if(*n == 13 && lat == zero) *n = 14;
S40:
/*
     *************************
     **** LATITUDE LIMITS ****
     *************************
     ** SOUTH LATITUDE LIMIT **
*/
    ilat = (*n-3)*8-80;
/*
     ** CORRECTION FOR LETTERS I (9) & O (15) **
*/
    if(*n > 8) ilat -= 8;
    if(*n > 14) ilat -= 8;
    *spsou = (double)ilat;
/*
     ** NORTH LATITUDE LIMIT - 8 DEGREES NORTH **
*/
    *spnor = *spsou+r8;
/*
     ** LETTER 'X' (24) LIMIT IS 12 DEGREES NORTH **
*/
    if(*n == 24) *spnor = *spsou+12.0;
/*
     ****************************
     ***** LONGITUDE LIMITS *****
     ****************************
     *** SET LONGITUDE LIMITS FOR 'STANDARD' ZONES ***
     ** CENTRAL MERIDIAN
*/
    icm = izone*6-183;
    slcm = (double)icm;
/*
     ** EAST & WEST LONGITUDE LIMITS **
*/
    *sleast = slcm+r3;
    *slwest = slcm-r3;
/*
     ** ADJUST LONGITUDE LIMITS FOR ODD SIZED AREAS **
     ** ALL IN ZONES 31 - 37
     ** LETTER NUMBER 22 (V) AND ABOVE
*/
    if(izone < 31 || izone > 37) return;
    if(*n < 22) return;
/*
     ** ZONES 31 AND 32 AT LETTER NUMBER 22 (V) **
*/
    if(*n == 22 && izone == 31) *sleast = r3;
    if(*n == 22 && izone == 32) *slwest = r3;
/*
     ** REMAINDER OF ODD LONGITUDE ZONES IN LETTER NUMBER 24 (X) **
*/
    if(*n < 24) 
        return;


    switch ( izone )
    {

        case 33:
        {
           *slwest = r9;
            *sleast = r21;
        }
        break;

        case 35:
        {
            *slwest = r21;
            *sleast = r33;
        }
        break;

        case 37:
        {
            *slwest = r33;
        }
        break;
    }

    if ( izone == 31 )
    {
         *sleast = r9;
    }

}
/*
FIFIDINT: FORTRAN Intrinsic Function IDINT

Copyright 1988 Promula Development Corporation ALL RIGHTS RESERVED

Author: Fred Goodman
Revised: 08/16/89, 9:30 AM to reflect the version 2 platform approach.

Synopsis:
*/
long fifidint( double a)
{
/*
Description:

Truncates its double precision argument to a long integer value.
In particular:

     fifidint(a) = 0 if |a| < 1 
               = the largest integer with the same sign as a that
                 does not exceed a if |a| >= 1

Return value:

The value as computed above.

See also:
*/
/* 
Referenced by:

Element       Description of use
-------       ------------------
FORTRAN       Intrinsic function IDINT and the generic functions
              IFIX, INT, INT2, and INT4
fifi2nint     Compute nearest short integer
fifnint       Compute nearest long integer

Process: this value is simply the long cast of the integal return
value from the C "modf" function
*/
#ifdef LONGDBL
auto double temp;
    modf(a,&temp);
    a = temp;
#else
    modf(a,&a);
#endif
    return (long) a;
}

void utmsetMGRS( long igroup, long izone, long *ltrlow, long *ltrhi, double *fnltr )
 {	long iset;
/*
     **************************************
     *** DETERMINE SET FROM ZONE NUMBER ***
*/
    iset = 1;
S3000:
    if((izone-iset)/6*6+iset == izone) goto S3100;
    iset += 1;
    if(iset > 6) goto S3350;
    goto S3000;
S3100:
/*
     *** SET 'LOW' AND 'HIGH' (2ND) LETTER OF ZONE BASED ON ISET ***
*/
    switch((short)iset){case 1: goto S3210;case 2: goto S3220;case 3: goto
      S3230;case 4: goto S3210;case 5: goto S3220;case 6: goto S3230;default:
      break;}
S3210:
    *ltrlow = 1;
    *ltrhi = 8;
    goto S3300;
S3220:
    *ltrlow = 10;
    *ltrhi = 18;
    goto S3300;
S3230:
    *ltrlow = 19;
    *ltrhi = 26;
    goto S3300;
S3300:
/*
     *** SET false NORTHINGS FOR 3RD LETTER ***
*/
    switch((short)igroup){case 1: goto S3320;case 2: goto S3330;default: break;}
S3320:
/*
     * GROUP 1
*/
    *fnltr = 0.0;
    if(fifmod(iset,2L) == 0) *fnltr = 1500000.0;
    goto S3350;
S3330:
/*
     * GROUP 2
*/
    *fnltr = 1000000.0;
    if(fifmod(iset,2L) == 0) *fnltr = 500000.0;
    goto S3350;
S3350:
    return;
}


ERRSTATUS long_to_char(unsigned char *record, long *l, short big_endian, long *c)
{
    long            i;
    union
    {				/* the long & character union          */
	unsigned long   l;
	unsigned char   c[4];
    }               lngchr;

    lngchr.l = *l;
    for (i = 0; i < 4; i++)
	if (big_endian)
	    record[(*c)++] = lngchr.c[3 - i];
	else
	    record[(*c)++] = lngchr.c[i];

    return STAT_SUCCESS;
}

ERRSTATUS double_to_char(unsigned char *record, double *d, short big_endian, long *c)
{
    long            i;
    union
    {				/* the double & character union         */
	double          d;
	unsigned char   c[10];
    }               dblchr;
    long            start;

    dblchr.d = *d;
    start = *c;
   
    if (sizeof(double) == 10)
	  convert_double_mac10(dblchr.c, 2L);  

    for (i = 0; i < 8; i++)
      if (big_endian)
        record[(*c)++] = dblchr.c[7-i];
      else
        record[(*c)++] = dblchr.c[i];

    return STAT_SUCCESS;
}


ERRSTATUS Bit_to_Utm_for_data( double bitx, double bity, long* iZone, double* lonUTM, double* latUTM  )
{
	double londd, latdd;
	bit_to_geo_for_data( mapgeo, bitx, bity, &londd, &latdd );
	Geo_to_Utm_for_data( londd, latdd, iZone, lonUTM, latUTM );
	return VMAPOK;
}

ERRSTATUS Utm_to_Bit_for_data( long iZone, double lonUTM, double latUTM, double *bitx, double *bity )
{
	double londd, latdd;
	Utm_to_Geo_for_data( iZone, lonUTM, latUTM, &londd, &latdd );
	geo_to_bit_for_data( mapgeo, londd, latdd, bitx, bity );
	return VMAPOK;
}


ERRSTATUS Geo_to_Utm_for_data( double londd, double latdd, long* iZone, double* lonUTM, double* latUTM  )
{   
	short status = 0;

    COORD3 dd_point, map_point;
	double lon_shifted = londd;
//	double shift = geo_360_to_180(&lon_shifted);
    dtcc_set_coord3_geo( &dd_point, latdd, lon_shifted, DOUBLE_NULL);
	status = dtcc_transform(mapgeo->internal_to_user2, (const void *)&dd_point, (void *)&map_point);
	*latUTM = map_point.data.c[0];
	*lonUTM = map_point.data.c[1];
	*iZone = (long)map_point.zone;
	return status;
}

ERRSTATUS Utm_to_Geo_for_data( long iZone, double lonUTM, double latUTM, double *londd, double *latdd  )
{
	short status = 0;
    COORD3 dd_point, map_point;
		dtcc_set_coord3_lon( &dd_point, lonUTM );
	dtcc_set_coord3_lat( &dd_point, latUTM );
	dtcc_set_coord3_zone( &dd_point, (short)iZone );
	status = dtcc_transform(mapgeo->user2_to_internal, (const void *)&dd_point, (void *)&map_point);
	*latdd = map_point.data.c[0];
	*londd = map_point.data.c[1];
	return status;
}


////ERRSTATUS Bit_to_Mgrs_for_data( double bitx, double bity, char* sMgrs  )
ERRSTATUS Bit_to_Mgrs_for_data( const MT_Vector2D& vPos, std::string& sMgrs )
{
	double londd, latdd;
	bit_to_geo_for_data( mapgeo, vPos.rX_, vPos.rY_, &londd, &latdd );
	Geo_to_Mgrs_for_data( londd, latdd, sMgrs );
	return VMAPOK;

}

ERRSTATUS Mgrs_to_Bit_for_data( const std::string& sMgrs, MT_Vector2D& vPos )
{
	double londd, latdd;
	Mgrs_to_Geo_for_data( sMgrs.c_str(), &londd, &latdd );
	geo_to_bit_for_data( mapgeo, londd, latdd, &vPos.rX_, &vPos.rY_ );
	return VMAPOK;
}


ERRSTATUS Geo_to_Mgrs_for_data( double londd, double latdd, std::string& sMgrs  )
{   
	short status = 0;

    COORD3 dd_point, map_point;
	double lon_shifted = londd;
//	double shift = geo_360_to_180(&lon_shifted);
    dtcc_set_coord3_geo( &dd_point, latdd, lon_shifted, DOUBLE_NULL);
	status = dtcc_transform(mapgeo->internal_to_user, (const void *)&dd_point, (void *)&map_point);

	int nLen = strlen( map_point.data.a );
	if ( nLen > 24 )
	{
		return ILLEGAL_ARG;
	}
	sMgrs.assign( &map_point.data.a[0], &map_point.data.a[nLen] );

	return status;
}

ERRSTATUS Mgrs_to_Geo_for_data( const std::string& sMgrs, double *londd, double *latdd  )
{
	short status = 0;
    COORD3 dd_point, map_point;
	dtcc_set_coord3_MGRS( &dd_point, sMgrs.c_str() );
	status = dtcc_transform(mapgeo->user_to_internal, (const void *)&dd_point, (void *)&map_point);
	*latdd = map_point.data.c[0];
	*londd = map_point.data.c[1];
	return status;
}
	

ERRSTATUS geo_to_bit_for_data(MAPGEO * mapgeo, double londd, double latdd, double * bitx, double * bity)
{
	if ( !mapgeo)
		return (ERRSTATUS)ILLEGAL_ARG;

    COORD3 dd_point, map_point;
    short  status = VMAPOK;
    double           px, py,lon_shifted;
//	char             err_mess[100], dtcc_mess[40];
	/**WR Jan16 */
	double           shift = 0.0;
#if 0
    if (mapgeo->projection == PROJ_GRAPH || mapgeo->projection == PROJ_LINEAR)
    {
       double merid_180;

       if (mapgeo->geo_extent.left < -180.E0)
          merid_180 = -1.E0;
       else if (mapgeo->geo_extent.right > 180.E0)
          merid_180 =  1.E0;
       else
          merid_180 =  0.E0;

       if (merid_180 != 0.E0 &&
          (londd < mapgeo->geo_extent.left || londd > mapgeo->geo_extent.right))
          londd += merid_180 * 360.E0;
    }
#endif

	/**WR Jan 08 */
	lon_shifted = londd;
	shift = geo_360_to_180(&lon_shifted);
    dtcc_set_coord3_geo( &dd_point, latdd, lon_shifted, DOUBLE_NULL);

	status = dtcc_transform(mapgeo->data_to_display, (const void *)&dd_point, (void *)&map_point);

    if( (status >= VMAPOK) &&
    /*****TEMP: 5/96--dtcc_transform() sometimes returns huge grid values with OK status*/
        (fabs(dtcc_coord31(&map_point))<=80000000) && (fabs(dtcc_coord32(&map_point))<=80000000) )
	{
	    if( dtcc_is_system_grid(&mapgeo->map_display_coord_sys) )
	    {
		    py = dtcc_coord3_north(&map_point);

  		    if( strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Universal Polar Stereographic South 84") ==  0 )
			    py = fabs(py);
      		else if( (strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Irish Transverse Mercator") == 0) && (py < 0) )
			    /* py = dtcc_system_trueY( &mapgeo->map_display_coord_sys, &map_point ); */
				py  = -py - 1000000.0;

		    px = dtcc_coord3_east(&map_point);
        }
	    else  /* GP system */
	    {
 	        py = dtcc_coord31(&map_point);
 	        px = dtcc_coord32(&map_point);
	        /**WR TEMP: dt() can shift lon across dateline--check for this*/
			/* if( shift ) */
		    geo_180_to_360(mapgeo, &px);
			/* px -= shift; */
	     }
        /* compute plotting coordinates from projection coordinates */
        *bitx = (px * mapgeo->plot_params.sx + mapgeo->plot_params.tx);
        *bity = (py * mapgeo->plot_params.sy + mapgeo->plot_params.ty);
    }
    if( status != VMAPOK )
	{
  		if( status < 0)
            mapgeo->geo_to_bit_errors++;   
        if( !mapgeo->hide_transform_err_messg ) 
        {   
/*            dtcc_get_status_message( (short)status, dtcc_mess );   
            sprintf(err_mess, "Coordinate conversion problem -- %s. Keep showing error messages? ",dtcc_mess);
		    switch( muse_ask3( "Show", "Don't Show", NULL, err_mess ) )
		    {
	          case RESP_DEFAULT:	
			    break;
	          case RESP_2:	
			    mapgeo->hide_transform_err_messg = true;
			    break;
	          default:		
			    muse_fatal ("invalid return from muse_ask3 ");
		    }
*/
        }
	}
    return (ERRSTATUS)status;
}


double geo_360_to_180( double *londd )
{
	double old_lon;

	old_lon = *londd;
	while( *londd > 180.0 )
	{
	  *londd -= 360.0;
    }
	if( fabs(old_lon - *londd) > ANIOTA)
		return -360.0;
	while( *londd < -180.0 )
	{
	  *londd += 360.0;
	}
	if( fabs(old_lon - *londd) > ANIOTA)
		return 360.0;
	return 0.0;
}

/* status code. 0=>OK */
short dtcc_set_coord3
	( 	COORD3* c3P,      /* (in/out) Pointer to active structure */
        double u,         /* (in) X value */
        double v,         /* (in) Y value */
        double w,         /* (in) Z value */
        short  zone       /* zone id for Grid */
    )
    {   if( !c3P )
            return ILLEGAL_ARG;
        else
        {	_COORD31(c3P) = u; 
            _COORD32(c3P) = v; 
            _COORD33(c3P) = w; 
            _COORD3Z( c3P ) = zone;
            return ( u != DOUBLE_NULL && v != DOUBLE_NULL ? 
            			VMAPOK : COORD_NOT_DEFINED );
        }
    }

short dtcc_transform( TRANSFORM_SYS* tsP, const void* inptr, void* outptr )
	{	if( !outptr || !inptr || !tsP )
			return ILLEGAL_ARG;
		else
		if( tsP->valid ) 
    	{	const COORD_SYS *insysP = _INPUTP(tsP); 
    		COORD_SYS  *outsysP = _OUTPUTP(tsP);
			FILE* ingeoidfp = dtcc_vdatum_fileP(_SYSTEM_DATUMP(insysP));
    		FILE* outgeoidfp = dtcc_vdatum_fileP(_SYSTEM_DATUMP(outsysP));
    		COORD3 tmp;
			double out1, out2, out3;
    		long done = 0;
    		short instat = VMAPOK, status = VMAPOK, outstat= VMAPOK;
   			short in_mode = (short)dtcc_system_mode(insysP);
   			const COORD3 *inpt = (COORD3*)inptr;
			COORD3 *outptP = (COORD3*)outptr;
			const COORD3_UNITS  *inunitsP  = dtcc_system_unitsP(insysP),
   								*outunitsP = dtcc_system_unitsP(outsysP);
    		if( !dtcc_are_coord3_units_standard(inunitsP) ) 
    			dtcc_convert_coord3_from_units( inunitsP, inpt, &tmp );
			else
				dtcc_copy_coord3( &tmp,inpt );
			if( dtcc_coord3_is_geo(in_mode) )
       		{	instat = (short)(dtcc_extents_contains( 
							dtcc_system_extentsP(insysP),inpt) ? 0 : 1 );
      			if( _DOMOLO(tsP) ) 
       			{	double mh = dtcc_coord3_elv(&tmp);
					if( dtcc_is_vdatum_AMSL(_SYSTEM_DATUMP(_INPUTP(tsP))) ) 
					{	if( ingeoidfp == NULL ) ingeoidfp = outgeoidfp;
						mh = amsl_to_ellipsoidal
								( geoid_height(ingeoidfp,dtcc_coord3_lat(&tmp),
										dtcc_coord3_lon(&tmp)), mh );
					}
					molodensky
       					( 	dtcc_coord3_lat(&tmp),dtcc_coord3_lon(&tmp),mh,
       							&out1, &out2, &out3, _MPARS(tsP) );
       				if( dtcc_is_vdatum_AMSL(_SYSTEM_DATUMP(_OUTPUTP(tsP))) )
					{	if( !dtcc_is_vdatum_AMSL(_SYSTEM_DATUMP(_INPUTP(tsP))) )
						{	if( outgeoidfp == NULL ) outgeoidfp = ingeoidfp;
							out3 = 
								ellipsoidal_to_amsl
									( geoid_height(outgeoidfp, out1,out2), out3 );
						}
						else
							out3 = dtcc_coord3_elv(&tmp);
					}
					dtcc_set_coord3_geo( outptP, out1,out2,out3 );
        		}
        		else
       				dtcc_copy_coord3( outptP, &tmp  ); 
        		status = _convert_geo_to_output( outsysP, outptP );
        	}
        	else
        	if( dtcc_coord3_is_grid(in_mode) )              /* Input is grid */
        	{	short status1 = dtcc_inverse_projection( dtcc_system_projectionP(insysP), &tmp, outptP );
				if( _DOMOLO(tsP) ) 
        		{	double mh = dtcc_coord3_elv(outptP);
					if(dtcc_is_vdatum_AMSL(_SYSTEM_DATUMP(_INPUTP(tsP))))  
						mh = amsl_to_ellipsoidal
								( geoid_height(dtcc_vdatum_fileP(_SYSTEM_DATUMP(_OUTPUTP(tsP))),
									dtcc_coord3_lat(outptP),
										dtcc_coord3_lon(outptP)), mh );
					molodensky
        				( 	dtcc_coord3_lat(outptP),dtcc_coord3_lon(outptP),mh,
        						&out1, &out2, &out3, _MPARS(tsP) );
					if( dtcc_is_vdatum_AMSL(_SYSTEM_DATUMP(_OUTPUTP(tsP))) ) 
					{	if( !dtcc_is_vdatum_AMSL(_SYSTEM_DATUMP(_INPUTP(tsP))) )
							out3 = 
								ellipsoidal_to_amsl
									( geoid_height(dtcc_vdatum_fileP(_SYSTEM_DATUMP(_OUTPUTP(tsP))),
										out1,out2), out3 );
						else
							out3 = dtcc_coord3_elv(outptP);
					}
					dtcc_set_coord3_geo( outptP, out1,out2,out3 );

				}
  				outstat = (short)(dtcc_extents_contains( 
							dtcc_system_extentsP(outsysP), outptP ) ? 0 : 1 );
      			status = (short)_convert_geo_to_output( outsysP, outptP );
				status = (short)( status < 0 ? status :
							( status1 > 0 ? status1 : status ));
        	}
        	else
        	if( dtcc_coord3_is_xyz(in_mode) )    /* Input is USR */
        	{	if( dtcc_system_mode(outsysP) == COORD3_XYZ )
        			status = dtcc_copy_coord3( outptP, &tmp  );
    			else
    			{	dtcc_usr_to_geo
    					( dtcc_system_ellipsoidP(outsysP), inpt, outptP );	 
        			outstat = (short)(dtcc_extents_contains( 
								dtcc_system_extentsP(outsysP), outptP ) ? 0 : 1 );
      				status = _convert_geo_to_output( outsysP, outptP );
        		}
        	}
			else
        			status = ITEM_NOT_FOUND;
        	if( !dtcc_are_coord3_units_standard(outunitsP) && status >= VMAPOK )
        		dtcc_convert_coord3_to_units( outunitsP, outptP, outptP );		
        	return	(short)( status == VMAPOK && instat == VMAPOK && outstat == VMAPOK ? VMAPOK :
					( status == VMAPOK  ? instat | outstat  : status ));
        }
		return ILLEGAL_ARG;

}

const char* dtcc_nameP( const void* dtccP )
{	
	const DTCC_LABELS* idP = (DTCC_LABELS*)dtccP; 
	return ( dtccP ? _label_nameP(idP) : 0 ); 
}

double geo_180_to_360( MAPGEO *mapgeo, double *londd )
{
#if 0
	if( *londd > 0.0 )
	{
	  *londd -= 360.0;
	  return -360.0;
    }
	else if( *londd < 0.0 )
	{
	  *londd += 360.0;
	  return 360.0;
	}
#endif
	if( (*londd > mapgeo->geo_extent.right) || (*londd < mapgeo->geo_extent.left) )
	{
	  *londd += mapgeo->plot_params.lon_shift_factor;
	  return mapgeo->plot_params.lon_shift_factor;
    }
	return 0.0;
}

FILE* dtcc_vdatum_fileP( const DATUM_PARAMS* dpP )
{	
	return ( dpP ? (FILE*)_DATUM_VFILEP(dpP) : (FILE*)NULL );  
}

short dtcc_are_coord3_units_standard(	const COORD3_UNITS* cuP 	) /* 1 if the units are standard */ /* Units */ 
	{	if( cuP->mode == COORD3_MGRS || cuP->mode == COORD3_GREF )
			return 1;
		else
		{	const UNITS_INFO* uiP;		
			uiP = _COORD3_UNITSNP(cuP,0);		
			if( dtcc_units_cf(uiP) != 1.0 ) return 0;		
			uiP = _COORD3_UNITSNP(cuP,1);		
			if( dtcc_units_cf(uiP) != 1.0 ) return 0;		
			uiP = _COORD3_UNITSNP(cuP,2);		
			if( !dtcc_is_valid(uiP) ) return 1;
			if( dtcc_units_cf(uiP) != 1.0 ) return 0;
		}
		return 1;
	}

short dtcc_convert_coord3_from_units( const COORD3_UNITS* cuinP, const COORD3* c3inP, COORD3* c3outP )
    {	const UNITS_INFO* info1 = dtcc_coord3_unitsNP(cuinP,0);
    	const UNITS_INFO* info2 = dtcc_coord3_unitsNP(cuinP,1);
    	const UNITS_INFO* info3 = dtcc_coord3_unitsNP(cuinP,2);
    	if( c3inP != c3outP ) dtcc_copy_coord3( c3outP, c3inP );
    	if( dtcc_units_are_numeric(info1) && 
    		dtcc_units_are_numeric(info2) )
    	{	double	x = dtcc_coord31(c3inP),
    				y = dtcc_coord32(c3inP),
    				z = dtcc_coord33(c3inP);
    		x = dtcc_convert_from_units(info1,&x );
            y = dtcc_convert_from_units(info2,&y );
    		if( dtcc_is_valid(info3) && z != DOUBLE_NULL )
    			z = dtcc_convert_from_units(info3,&z );
    		dtcc_set_coord3_grid( c3outP, x,y,z, dtcc_coord3_zone(c3inP) );
            return VMAPOK;
        }
        else
        	return ITEM_NOT_FOUND;
    }

/* Copy the data to another COORD3 */
short dtcc_copy_coord3( COORD3* c1P, const COORD3* c2P )     
    {   if( c1P && c2P )
        {   void* kP = memcpy( c1P, c2P, sizeof(COORD3) );
            return (  (COORD3*)kP == c1P ?  VMAPOK : COPY_ERROR ); 
        }
        else
            return ILLEGAL_ARG;
    }
    
short dtcc_extents_contains( const COORD_EXTENTS* csP, const COORD3* c3P )
	{	double top, bottom, left, right, v1,v2;
		if( !csP || !c3P )
			return 0;
		else
		if( !dtcc_coord3_is_numeric(dtcc_coord3Z(c3P)) )
			return 0;
		if( dtcc_coord3_is_geo(dtcc_coord3Z(c3P)) )
		{	top = dtcc_extents_max_lat(csP);
			bottom = dtcc_extents_min_lat(csP);
			left = dtcc_extents_min_lon(csP);
			right = dtcc_extents_max_lon(csP);
			v1 = dtcc_coord3_lat(c3P); v2 = dtcc_coord3_lon(c3P);
		}
		else
		{	top = dtcc_extents_max_north(csP);
			bottom = dtcc_extents_min_north(csP);
			left = dtcc_extents_min_east(csP);
			right = dtcc_extents_max_east(csP);
			v1 = dtcc_coord3_north(c3P); v2 = dtcc_coord3_east(c3P);
		}

		if( top == DOUBLE_NULL || bottom == DOUBLE_NULL || 
			left == DOUBLE_NULL || right == DOUBLE_NULL )
			return 0;
		else
			return ( v1 <= top && v1 >= bottom && v2 <= right && v2 >= left );
	}

short dtcc_is_vdatum_AMSL( const DATUM_PARAMS* dpP )
{	
	return  ( _DATUM_VNAMEP(dpP)[0] == 'A' ? 1 : 0 );  
}

double geoid_ellipsoid( double N, double hgt, int mode )
{	double h = hgt;
	/* msl = ellipsoidal(geodetic) hgt - geoid hgt(separation) */
	if( N > -10000.0 )
	{	if( mode == 0 ) /* hgt = amsl, result = ellipsoid hgt */
			h = hgt + N;
		else
		if( mode == 1 )  /* ellipsoid hgt = amsl, result = amsl */
			h = hgt - N;
	}
	return h;
}

double geoid_height( FILE* infile, double lat, double lon  )
{  	long               row_num, col_num;
    double            lat_adjust, lon_adjust, heights[5];
    short 			  bytes_per_height = 1; 

    double geoid_height = DOUBLE_NULL;
    /***TEMP*** bytes_per_height has been set to 1 in calling function
		until support of single-precision file */
    /* adjust lat and lon[0-360) so integer parts gives row and column numbers 
       in data matrix, and fractional parts gives ratios needed for
       interpolation ( (lat - lat1)/(lat2 - lat1)) and
       (lon - lan1)/(lon2 - lon1) ).  90N is in Row 0 and 0E is in
       Column 0.  Rows and columns are spaced at half-degree intervals */
	lat_adjust = 2.0 * (90.0 - lat);
    lon_adjust = 2.0*((lon < 0.0) ? lon + 360.0 : lon);
    row_num = (long)floor( lat_adjust );
    col_num = (long)floor( lon_adjust );
    /* store ratios */
    lat_adjust -= row_num;
    lon_adjust -= col_num;

    if( !infile )
		;
    else
	if( read_hgts(infile,heights+1,row_num,col_num, bytes_per_height ) != 4 )
		;
	else
		geoid_height = heights[1] +
		       lon_adjust * (heights[2] - heights[1]) +
		       lat_adjust * (heights[4] - heights[1]) +
		       lon_adjust * lat_adjust *
		       (heights[1] + heights[3] - heights[2] - heights[4]);
/* printf( "\nHGTS: %lf %lf %lf %lf -> %lf\n", 
	   heights[1],heights[2],heights[3],heights[4], geoid_height); */
	   return geoid_height;
}

short molodensky( double from_lat,  double from_lon, double from_hgt,
				  double *lat,  double* lon, double* hgt, 
				  const void *moloP )
{	double dphi,dlam,dhgt;
	short k;
	double elv = ( from_hgt < -1000000.0 ? 0.0 :  from_hgt );
		k = molodensky_shift( from_lat,from_lon, elv, &dphi,&dlam,&dhgt, moloP );
	if( k == 0 )
	{	if( lat ) *lat = from_lat + dphi;
		if( lon ) *lon = check_180(from_lon + dlam);
		if( hgt ) *hgt = elv + dhgt;
	}
	return k;
}	

static short _convert_geo_to_output(   const COORD_SYS* outsysP,COORD3* outptP 	)
	{	short status = VMAPOK;
		short out_mode = (short)dtcc_system_mode(outsysP);
		if( out_mode < COORD3_XYZ) /* out mode is gridded */
        	status = dtcc_forward_projection
        			( (PROJ_PARAMS*)dtcc_system_projectionP((COORD_SYS*)outsysP),
        				outptP, outptP );
        else
        if( out_mode == COORD3_XYZ )    /* Output is XZY */
        		status = dtcc_usr_from_geo
        			( dtcc_system_ellipsoidP(outsysP), outptP, outptP );	 
        else
		if( out_mode == COORD3_GEO ) /* out is geo */
			status = VMAPOK;
		else
        	status = ITEM_NOT_FOUND;        	
        return status;
    }

short _projection( 	const PROJ_PARAMS* proP, short mode, const COORD3* fromP, long n, COORD3* toP )
    {   if( !dtcc_is_valid(proP)  )
    		return STRUCTURE_INVALID;
    	else
    	{	DTCC_ENGINE engine = (DTCC_ENGINE)_PROJECTION_ENGINE(proP); 
        	if( !engine) 
        		return ILLEGAL_ARG;
        	else
        	{	short k;
        		while( --n >= 0 )
        		{	k = engine( proP, mode, fromP+n, toP+n );
        			if( k != VMAPOK ) return k;
        		}
        		return VMAPOK;
        	}
        }
    } 

short dtcc_usr_to_geo(	const ELLIPS_PARAMS* elP, const COORD3* from_pt, COORD3* to_pt)
	{	short status = -1;
		double lat,lon,h;
		status = (short)usr_to_geo
			( dtcc_ellipsoid_axis(elP), dtcc_ellipsoid_ecc2(elP),
				dtcc_coord3_x(from_pt), dtcc_coord3_y(from_pt), dtcc_coord3_z(from_pt),
					&lat, &lon, &h );
		dtcc_set_coord3_geo( to_pt, lat, lon, h );
		return status;
	}

short dtcc_convert_coord3N_to_units( const COORD3_UNITS* cuinP, const COORD3N* c3NinP, COORD3N* c3NoutP )
    {	long k = dtcc_coord3N_count(c3NinP);
    	while( --k >= 0 )
    		dtcc_convert_coord3_to_units
    			( cuinP, _COORD3N_KP(c3NinP,k),_COORD3N_KP(c3NoutP,k) );
    	return VMAPOK;
    }

short dtcc_convert_coord3_to_units(	const COORD3_UNITS* cuinP, const COORD3* c3inP,COORD3* c3outP )
    {	double 	x = dtcc_coord31(c3inP),
    			y = dtcc_coord32(c3inP),
    			z = dtcc_coord33(c3inP);
    	const UNITS_INFO* info1 = dtcc_coord3_unitsNP(cuinP,0);
    	const UNITS_INFO* info2 = dtcc_coord3_unitsNP(cuinP,1);
    	const UNITS_INFO* info3 = dtcc_coord3_unitsNP(cuinP,2);
    	if( dtcc_units_are_numeric(info1) && x != DOUBLE_NULL )
    		dtcc_convert_to_units(info1,x,&x );
        if( dtcc_units_are_numeric(info2) && y != DOUBLE_NULL )
    	    dtcc_convert_to_units(info2,y,&y );
    	if( dtcc_units_are_numeric(info1) && z != DOUBLE_NULL )
    		dtcc_convert_to_units(info3,z,&z );
        dtcc_set_coord3( c3outP, x,y,z, dtcc_coord3_zone(c3inP) );
        return VMAPOK;
    }

int dtcc_is_valid( const void* dtccP )
{	
	return dtcc_valid_flag(dtccP) && dtcc_is_valid_name(dtccP);  
}

short dtcc_valid_flag(const void* dtccP)
{	
	const DTCC_LABELS* idP = (DTCC_LABELS*)dtccP; 
	return ( dtccP ? idP->valid : -1 );  
}


double dtcc_convert_from_units( const UNITS_INFO* uiP,const void* v   )
{   
	if( dtcc_units_cf(uiP) > 0.0 )
        return (*(double*)v)*dtcc_units_cf(uiP);
    else
        return dtcc_decode_units( uiP, (char*)v, NULL );
}


static short read_hgts( FILE* infile, double* hgts, long row,long col, size_t n ) 
{	unsigned char inbytes[8];
	short k = 0, N = n*2;
    /* find index (how many bytes from beginning of file) */
    long index = (row * FROW_SIZE + col) * n;
	if( fseek(infile, index, 0) == 0 )
	{	k += fread( inbytes,n,2,infile );
		hgts[0] = (double)((int)inbytes[0] - BYTEBIAS);
		hgts[1] = (double)((int)inbytes[1] - BYTEBIAS);
		index += (FROW_SIZE*n-2);
		if( row >= 360 )
		{	k = 4;	  /* data is on last line */
			hgts[2] = hgts[0];
			hgts[3] = hgts[1];
		}
		else
		if(	fseek(infile, index, 0) == 0 )
		{	k += fread( inbytes,n,2,infile );
			hgts[2] = (double)((int)inbytes[0] - BYTEBIAS);
			hgts[3] = (double)((int)inbytes[1] - BYTEBIAS);
		}
	}
	return k;
}

short molodensky_shift( double from_lat,  double from_lon, double from_hgt, double *dphi, double* dlam, double* dhgt, const void *moloP )
{	


	short status = -1;
	double tlat = 0.0, tlon = 0.0, th = 0.0;
	const struct TRANSFORM_PARAMS *params = (struct TRANSFORM_PARAMS*)moloP;
    if( from_hgt < -1000000.0 ) from_hgt = 0.0;
    if( params )
    {	
		double geodetic_hgt = from_hgt;
		if( abs(SPECCODE) == 72 )
			status = wgs72( from_lat, from_lon, geodetic_hgt, &tlat,&tlon,&th, SPECCODE );
		else
		if( abs(SPECCODE) == 66 )
		{	status = wgs66( from_lat, from_lon, geodetic_hgt, &tlat,&tlon,&th, params );
			tlat -= from_lat;
			tlon -= from_lon;
			th -= geodetic_hgt;
		}
		else
		if( fabs(from_lat) <= 89.0 )
		{	
			double	phi = from_lat*RADDEG;
			double  lam = from_lon*RADDEG;
			double  slat = sin(phi);
			double  clat = sqrt( 1.0 - slat*slat );
    		double  slon = sin(lam); 
    		double  clon = cos(lam);
			double  ssqlat = slat * slat;
			double  DA = DAFN;
			double  DF = DFFN;
			double  ADB = ADBFN;
			double  DX = DXFN, DY = DYFN, DZ = DZFN;
			double  ECC2 = ECC2FN( FLAT );
			double  rn = AXIS / sqrt(1. - ECC2 * ssqlat);
			double  rm = AXIS * (1. - ECC2)/pow( (1. - ECC2 * ssqlat), 1.5 );

			tlat = -DX * slat * clon - DY * slat * slon + DZ * clat;
    		tlat +=  DA * (rn * ECC2 * slat * clat) / AXIS;
    		tlat +=  DF * (rm * ADB + rn / ADB) * slat * clat;
    		tlat /= (rm + geodetic_hgt); /* *RADDEG; */
			if( fabs(clat) < 0.00000001 )
				tlon = 0.0;
			else
				tlon = (-DX * slon + DY * clon) / ((rn + geodetic_hgt) * clat); /* /RADDEG;*/
			th = DX * clat * clon + DY * clat * slon + DZ * slat;
    		th = th - DA * AXIS / rn + DF * rn * ssqlat / ADB;
			tlat /= RADDEG;  /* degrees */
			tlon /= RADDEG;   /* degrees */
		   	status = 0;
		}
		else
		{	double x,y,z, lat,lon,h;
			geo_to_usr( AXIS,ECC2FN(FLAT),from_lat,from_lon,geodetic_hgt, &x, &y,&z );
			x += DXFN; y += DYFN; z += DZFN;
			usr_to_geo( TO_AXIS, ECC2FN(TO_FLAT), x,y,z, &lat,&lon,&h );
			tlat = (lat - from_lat); /* degrees */
			tlon = (lon - from_lon); /* degrees */
			th = h - geodetic_hgt;
			status = 0;
		}
		*dphi = tlat;
		*dlam = tlon;
		*dhgt = th;
	}
    return status;
}

static double check_180( double a )
	{	if( a >= 180.0 )
			a -= 360.0;
		else
		if( a < -180.0 )
			a += 360.0;
		return a;
	}


short  dtcc_usr_from_geo (	const ELLIPS_PARAMS* elP, const COORD3*	from_pt,COORD3*	to_pt)
	{	short status = -1;
		double x,y,z;
		double h = dtcc_coord3_elv(from_pt);
		if( h == DOUBLE_NULL ) h = 0.0;
		status = (short)geo_to_usr
			( dtcc_ellipsoid_axis(elP), dtcc_ellipsoid_ecc2(elP),
				dtcc_coord3_lat(from_pt), dtcc_coord3_lon(from_pt), h,
					&x, &y, &z );
		if( status == VMAPOK ) dtcc_set_coord3_cart( to_pt, x,y,z );
		return status;
	}

int usr_to_geo(	double a, double e2, double x, double y, double z,double* lat, double *lon, double *h 	)
{	double sphi,cphi, p, rn, ANIOTA2 = 0.001; 
    double absx = fabs(x), absy = fabs(y);
    /* find longitude, including cases of division by zero */
	if( absx < ANIOTA2 && absy < ANIOTA2 ) 
       	*lon = 0.0;
   	else if( absx < ANIOTA2 && y > 0.0 ) 
       	*lon = 90.0;
   	else if( absx < ANIOTA2 && y < 0.0 )
       	*lon = -90.0;
   	else
   		*lon = atan2( y, x)/RADDEG;

    /* find latitude, including singular cases */
    if( absx < ANIOTA2 && absy < ANIOTA2 )
    	*lat = ( z > ANIOTA2 ? 90.0 : ( z < -ANIOTA2 ? -90.0 : 0.0 ));
    else
    {	int i = 2;
    	double tanlat, beta, cb,sb, phi;
		double epsq = e2 / (1.0 - e2), en = sqrt(1.0-e2), b = a*en;
		p = sqrt(x * x + y * y);
		beta  = atan((a * z) / (b  * p));
		while( --i >= 0 )
	   	{	sb = sin(beta); cb = cos(beta);
		   	tanlat = 
		   		(z + epsq  * b  * sb*sb*sb ) / (p - a  * e2 * cb*cb*cb);
	    	if( i > 0 ) beta = atan(en * tanlat );
		}
		phi = atan(tanlat);
		sphi = sin(phi); cphi = cos(phi);
		*lat = phi/RADDEG;
	}
	/* find geodetic height, including polar case */
    rn = a / sqrt(1.0 - e2 * sphi * sphi);
    if( ( fabs(*lat) - 89.0 ) >= 0.0 )
       	*h = z / sphi + rn * (e2 - 1.0);
    else
		*h = p / cphi - rn;
	return 0;
}

short dtcc_convert_to_units( const UNITS_INFO* uiP, double v, void* result   )
    {   if( dtcc_units_cf(uiP) > 0.0 )
        {   double* converted = (double*)result;
            *converted = v/dtcc_units_cf(uiP);
            return 0;
        }
        else
            return dtcc_encode_units( uiP, NULL, v, (char*)result );     
    }   

int dtcc_is_valid_name( const void* dtccP )
	{	const DTCC_LABELS* idP = (DTCC_LABELS*)dtccP;
		return ( idP ? _check_label_form(_label_nameP(idP)) : 0 ); 
	}

double dtcc_decode_units( const UNITS_INFO* uiP, const char* encoded, long* nl   )
    {   double x; long length; 
        if( dtcc_units_cf(uiP) == RADDM )
            x = dtcc_decode_dm( encoded, &length );
        else
        if( dtcc_units_cf(uiP) == RADDMS )
            x = dtcc_decode_dms( encoded, &length );
        else
        {   char prefix[2], nmbr[32], label[4];
            double sf = ( dtcc_units_cf(uiP) > 0.0 ? dtcc_units_cf(uiP) : 1.0 );
            length = _parseData( encoded, "NSEW", 1, prefix,nmbr,label );
            sscanf( nmbr, "%lf", &x );
            if( prefix[0] == 'S' || prefix[0] == 'W'  ) x *= -1.0;
            x *= sf; 
        }
        if( nl ) *nl = length;
        return x;      
    }

short wgs72 ( double from_lat,  double from_lon, double from_hgt,double *tlat,  double* tlon, double* thgt, int flag72 )
{	double 	dr = 1.4, da = 2.0, a = 6378135.0, df = 0.3121057e-7;
	double 	slat = sin(from_lat*RADDEG),
			clat = sqrt( 1.0 - slat*slat ),
			s2lat = 2.0*slat*clat;
    *tlat = (4.5 * clat / a + df * s2lat)/RADDEG;
    *tlon = 0.554 / 3600.0;
	/* Note: different than equation in 600008p.32 (sin squared vs sin(2*..))*/
     *thgt = 4.5 * slat + a * df * slat*slat - da + dr;
    /* From WGS84 */
	if( flag72 < 0 )
    {  *tlat *= -1; *tlon *= -1; *thgt *= -1;  }
	return 0;
}
short wgs66( double from_lat,  double from_lon, double from_hgt, double *tlat,  double* tlon, double* thgt, const struct TRANSFORM_PARAMS *params )
{	double x,y,z, lat,lon,h;
	if( SPECCODE > 0 ) /*  66->84 */
	{	geo_to_usr( AXIS,ECC2FN(FLAT),from_lat,from_lon,from_hgt, &x, &y,&z );/*  66 */
		x += DX66; y += DY66; z += DZ66; /*  66->72 */
		usr_to_geo( A72, ECC2FN(1.0/RF72), x,y,z, &lat,&lon,&h ); /* 72 */ 
		wgs72( lat,lon,h, tlat,tlon,thgt, SPECCODE ); /* 72->84 */
		*tlat += lat;
		*tlon += lon;
		*thgt += h;
	}
	else   /* 84->66 */
	{	wgs72( from_lat,from_lon,from_hgt, &lat,&lon,&h, SPECCODE );  /* 84->72 */
		lat += from_lat; lon += from_lon; h += from_hgt;
		geo_to_usr( A72, ECC2FN(1.0/RF72), lat,lon,h, &x, &y,&z );  /* 72 */
		x -= DX66; y -= DY66; z -= DZ66;  /* 72->66 */
		usr_to_geo( TO_AXIS,ECC2FN(TO_FLAT), x,y,z, tlat, tlon, thgt );  /* 66 */
	}
	return 0;
}	

int geo_to_usr(	double a, double e2, double lat, double lon, double h, double* x, double *y, double *z )
	{	double phi = lat*RADDEG, lam = lon*RADDEG;
        double sinp = sin(phi), cosp = cos(phi);
		double u = a/sqrt( 1.0 - e2*sinp*sinp );
		*x = (u+h) * cosp * cos(lam);
		*y = (u+h) * cosp * sin(lam);
		*z = ( u * (1.0 - e2) + h) * sinp;
		return 0;
	}

short    dtcc_encode_units( const UNITS_INFO* uiP, const char* prefixes, double  v, char* encoded   )
    {   if( !uiP || !encoded )
    		return ILLEGAL_ARG;
    	else
		if( v <= DOUBLE_NULL )
		{	sprintf( encoded, "NaN" );
			return 0;
		}
		else  
        if( dtcc_units_cf(uiP) == RADDM )
        	dtcc_encode_dm( v, prefixes, encoded, dtcc_units_fmt(uiP) );
        else
        if( dtcc_units_cf(uiP) == RADDMS )
            dtcc_encode_dms( v, prefixes, encoded, dtcc_units_fmt(uiP) );
        else /***NRL--don't go to dtcc_encode_degs() for radians and grads*/
        if( dtcc_units_cf(uiP) == 1 && prefixes != NULL ) 
            dtcc_encode_degs( v, prefixes, encoded, dtcc_units_fmt(uiP) );
        else
        {   char fmt[16];
        	int k = 0;
        	sprintf( fmt, "%c%slf", '%',dtcc_units_fmt(uiP)  );
            if( !prefixes )	prefixes = " -";
			if( prefixes[1] == '-' )
			{	sprintf( encoded,fmt, v/fabs(dtcc_units_cf(uiP)) );
				strcat( encoded, " " );
				strcat( encoded, dtcc_abbrvP(uiP) );
			}
            else
            {	char vcharP[32];
            	if( v < 0.0 ) k = 1;
        		sprintf( vcharP, fmt, fabs(v/dtcc_units_cf(uiP)) );
            	sprintf( encoded, 
                     "%c%s %s  ", prefixes[k], vcharP, dtcc_abbrvP(uiP) );
        	}
        	return 0;      
         }  
		return 1;

   }

static int _check_label_form( const char* idP )
	{	int status = 0;
		if( idP ) 
		{	char a = idP[0];
			char* b = ( a == 0 ? NULL : strchr( idP,'|') );
			status = ( a != 0 && b == NULL);
		}
		return status;
	}	 

double dtcc_decode_dm( const char* encoded, long* nl )
    {   int degs, sw = 0;
        double x, rmins;
        char prefix[2], label[4], nmbr[64];
        long length  = _parseData( encoded, "NSEW", 2, prefix, nmbr, label );
        sscanf( nmbr, "%d%lf", &degs, &rmins );
        x = abs(degs) + fabs(rmins)/60.0;
        if( degs < 0 || rmins < 0.0 || prefix[0] == 'S' || prefix[0] == 'W' )
             x *= -1.0;
        if( nl ) *nl =length;
        return x;
    }

double dtcc_decode_dms( const char* encoded, long* nl )
    {   int degs,mins, sw = 0;
        double x, rsecs;
        char prefix[2], label[4], nmbr[64];
        long length  = _parseData( encoded, "NSEW", 3, prefix, nmbr, label );
        sscanf( nmbr, "%d%d%lf", &degs, &mins, &rsecs );
        x = abs(degs) + ( abs(mins) + fabs(rsecs)/60.0 )/60.0;
        if( degs < 0 || mins < 0 || rsecs < 0.0 || 
        	prefix[0] == 'S' || prefix[0] == 'W' )
             x *= -1.0;
        if( nl ) *nl = length;
        return x;
    }

static long _parseData( const char* string, const char* flags, int n, char* prefix, char* nmbr, char* postfix )
    {   const char *s = string;
    	int i, skip, nf = strlen(flags);
    	long lngth = 0;
    	/* Set outputs to empty */
    	strcpy( prefix, "" );
    	strcpy( nmbr, "" );
    	strcpy( postfix, "" );
    	
    	/* Skip leading whitespace  */
    	skip = _skipWhiteSpace( s );
    	s += skip;
    	lngth += skip;
    	/* If a prefix exists, save it */
    	for( i=0; i<nf; i++ )
        	if( flags[i] == s[0] )
        	{	prefix[0] = flags[i];
        		prefix[1] = 0;
        		s++;
        		lngth++;
        		break;
        	}
        /* Store the number */
        /* Skip leading whitespace  */
    	skip = _skipWhiteSpace( s );
    	s += skip;
    	lngth += skip;
    	while( --n >= 0 )
        {	while( !isspace(s[0]) && s[0] != 0 )
        	{	nmbr[0] = s[0]; nmbr++; s++; lngth++;  }
        		skip = _skipWhiteSpace( s );
    			s += skip;
    			lngth += skip;
    			nmbr[0] = ' '; nmbr++;
        }
        nmbr[0] = 0;
        /* If it exists, store the postfix label */ 
        while( !isspace( s[0] ) && s[0] != 0 )
        {	postfix[0] = s[0]; postfix++; s++; lngth++; }
        postfix[0] = 0;
        
        return lngth;
    }

void dtcc_encode_dm(  double  v, const char* label, char* dmP, const char* fmtP  )
     { 	char format[16];
	 	const char* formP = ( fmtP ? fmtP : "6.3" );
     	char prefix = ( !label ? ' ' : ( v < 0.0 ? label[1] : label[0] ));
        double rdegs = fabs(v); 
        long ndegs = (long)rdegs;
        double rmins = (rdegs - ndegs) * 60.0;
		rmins = form_round_60(rmins,formP);
		while( rmins >= 60.0 )
		{	rmins -= 60.0;
			ndegs++;
		}

        if( v < 0.0 && label == NULL )   /* no label, put the sign back in */
        {   if( ndegs > 0 )
                ndegs *= -1;
            else
                rmins *= -1.0;
        }
        sprintf( format, "%s %s %c%slf",
				"%c","%3ld", '%', formP );	/***NRL*/
		sprintf( dmP, format, prefix, ndegs, rmins );
        return;
    } 

const char* dtcc_otherP( const void* dtccP )
{	
	const DTCC_LABELS* idP = (DTCC_LABELS*)dtccP; 
	return ( dtccP ? _label_otherP(idP) : 0 ); 
}


void dtcc_encode_dms( double  v, const char* label, char* dmsP,	const char* fmtP   )
     {	char format[24];	
		char prefix = ( !label ? ' ' : ( v < 0.0 ? label[1] : label[0] ));
        const char* formP = ( fmtP ? fmtP : "4.1" );
        double rdegs = fabs(v);
        long ndegs = (long)rdegs;
        double rmins = (rdegs - ndegs) * 60.0;
        long nmins = (long)rmins;
        double rsecs = (rmins - nmins) * 60.0;

        rsecs = form_round_60(rsecs,formP);
		while( rsecs >= 60.0 )
		{	rsecs -= 60.0;
			nmins++;
		}
        while( nmins >= 60 )
		{	nmins -= 60;
			ndegs++;
		}

        if( v < 0.0 && label == NULL ) /* no label, put sign back in */
        {   if( ndegs > 0 )
                ndegs *= -1;
            else
            if( nmins > 0 )
                nmins *= -1;
            else
                rsecs *= -1.0;
        }
        sprintf( format, "%s %s %s %c%slf",
				"%c","%3ld","%2ld", '%', formP );	/***NRL*/
		sprintf( dmsP, format, prefix, ndegs, nmins, rsecs );
        return;
    } 

void dtcc_encode_degs(   double  v, const char* label, char* degP, const char *fmtP    )
     {	char prefix = ( !label ? ( v < 0.0 ? '-' : ' ' ) : ( v < 0.0 ? label[1] : label[0] ));
		char format[16];
        sprintf( format, "%s%c%slf",
				"%c ", '%', ( fmtP ? fmtP : "10.6" ) );	/***NRL*/
		sprintf( degP, format, prefix, fabs(v) );
		return;
	  }

const char* dtcc_abbrvP( const void* dtccP )
{	
	const DTCC_LABELS* idP = (DTCC_LABELS*)dtccP; 
	return ( dtccP ? _label_abbrvP(idP) : 0 ); 
}

int _skipWhiteSpace( const char* string )
	{	int i, n = strlen(string);
		for( i=0; i<n; i++ )
			if( isspace( *string ) )
				string++;
			else
				break;
		return i;
	}

static double form_round_60( double v, const char* fmt )
{	
	double x = 0.5;
	const char* f = strchr( fmt, '.' );
	if( f ) 
	{	int n = atoi(++f);
		while( --n >= 0 ) x /= 10.0;
	}
	if( 60.0 - v < x ) v = 60.0;
	return v;
}

bool IsInside2D( float rX, float rY, vector<MT_Vector3D>& vListLines_ )
{
	int i, j;

    bool bInsideFlag = false;

    float rTx = rX;
    float rTz = rY;
	int nNbVerts = vListLines_.size();
    const MT_Vector3D* pVtx0 = &vListLines_[  nNbVerts - 1 ];

	// get test bit for above/below X axis
    bool bYflag0 = ( pVtx0->rZ_ >= rTz ) ;    
    const MT_Vector3D* pVtx1 = &vListLines_[ 0 ];


#ifdef	MT_WINDING
    int	nCrossings = 0 ;
#endif

#ifdef	MT_CONVEX
    int	nLineFlag = 0 ;
#endif
    
	j = nNbVerts + 1;
	i = 0;
	for ( ; --j ;  ) 
	{
		bool bYflag1 = ( pVtx1->rZ_ >= rTz );

		// check if endpoints are on opposite sides of X axis (i.e. the Y's differ). 
		//	if so, +X ray could intersect this edge.
		if ( bYflag0 != bYflag1 ) 
		{
			int bXflag0 = ( pVtx0->rX_ >= rTx ) ;
			// check if endpoints are on same side of the Y axis (i.e. X's are the same).
			// if so, it's easy to test if edge hits or misses.

			if ( bXflag0 == ( pVtx1->rX_ >= rTx ) ) 
			{
				// if edge's X values both right of the point, must hit 
#ifdef	MT_WINDING
				if ( bXflag0 ) 
				{
					nCrossings += ( bYflag0 ? -1 : 1 );
				}
#else
				if ( bXflag0 ) 
				{
					bInsideFlag = !bInsideFlag ;
				}
#endif
		    } 
			else 
			{
				// compute intersection of pgon segment with +X ray, note if >= point's X; if so, the ray hits it.
				if ( ( pVtx1->rX_ - ( pVtx1->rZ_ - rTz ) * ( pVtx0->rX_ - pVtx1->rX_ ) / ( pVtx0->rZ_ - pVtx1->rZ_ ) ) >= rTx ) 
				{
#ifdef	MT_WINDING
					nCrossings += ( bYflag0 ? -1 : 1 );
#else
					bInsideFlag = !bInsideFlag;
#endif
				}
			}
#ifdef	MT_CONVEX
			// if this is second edge hit, then done testing
			if ( nLineFlag ) 
			{
#ifdef	MT_WINDING
				// test if crossings is not zero
				bInsideFlag = ( nCrossings != 0 );
#endif
				return( bInsideFlag );
			}
		    // note that one edge has been hit by the ray's line
			nLineFlag = 1;
#endif
		}
		// move to next pair of vertices, retaining info as possible
		bYflag0 = bYflag1;
		++i;
		pVtx0 = pVtx1 ;
		pVtx1 = &vListLines_[ i ];
    }

#ifdef	MT_WINDING
    // test if crossings is not zero
    bInsideFlag = ( nCrossings != 0 );
#endif
    return( bInsideFlag );
}


bool Intersect2D( float rX1, float rY1, float rX2, float rY2, float rX3, float rY3, float rX4, float rY4, float* rXInters, float* rYInters  )
{

    float rA1, rA2, rB1, rB2, rC1, rC2;	// Coefficients of line eqns.
    float r1, r2, r3, r4;				// 'Sign' values
    float rDenom, rOffset, rNum;		// Intermediate values

    // Compute rA1, rB1, rC1, where line joining points 1 and 2 is "rA1 rX + rB1 rY + rC1 = 0.f"

    rA1 = rY2 - rY1;
    rB1 = rX1 - rX2;
    rC1 = rX2 * rY1 - rX1 * rY2;

    // Compute r3 and r4.

    r3 = rA1 * rX3 + rB1 * rY3 + rC1;
    r4 = rA1 * rX4 + rB1 * rY4 + rC1;

    /// Check signs of r3 and r4. 
	// If both point 3 and point 4 lie on same side of line 1, the line segments do not intersect

    if ( !MT_IsZero( r4 ) && !MT_IsZero( r4 ) && MT_IsSameSigns( r3, r4 ) )
	{
		return ( false );
	}
        
    // Compute a2, b2, c2

    rA2 = rY4 - rY3;
    rB2 = rX3 - rX4;
    rC2 = rX4 * rY3 - rX3 * rY4;

    // Compute r1 and r2

    r1 = rA2 * rX1 + rB2 * rY1 + rC2;
    r2 = rA2 * rX2 + rB2 * rY2 + rC2;

    // Check signs of r1 and r2.
	// If both point 1 and point 2 lie on same side of second line segment, the line segments do not intersect

    if ( !MT_IsZero( r1 ) && !MT_IsZero( r2 ) && MT_IsSameSigns( r1, r2 ) )
	{
		return ( false );
	}
        
    // Line segments intersect: compute intersection point

    rDenom = rA1 * rB2 - rA2 * rB1;
    if ( MT_IsZero( rDenom ) )
	{
		return ( false );
	}
        
    rOffset = rDenom < 0.f ? - rDenom / 2.f : rDenom / 2.f;

    // The rDenom/2 is to get rounding instead of truncating. 
	// It is added or subtracted to the numerator, depending upon the sign of the numerator.

    rNum = rB1 * rC2 - rB2 * rC1;
    *rXInters = ( rNum < 0.f ? rNum - rOffset : rNum + rOffset ) / rDenom;

    rNum = rA2 * rC1 - rA1 * rC2;
    *rYInters = ( rNum < 0.f ? rNum - rOffset : rNum + rOffset ) / rDenom;

	// lines_intersect
    return ( true );
}


ERRSTATUS Trans_vec( view_type* view )
{
	if ( view == NULL  )
		return STAT_NO_DATA;

	for ( int i = 0 ; i < view->nthemes ; ++i)
	{
		VEC* vec = view->theme[i].vec;
		if( vec  )
		{
			bit_and_geo_ops_complete( mapgeo );
			mapgeo->hide_transform_err_messg = true; 

			if (vec->areas != (AREA_FEATURE **)NULL)
			  Trans_vec_areas(mapgeo, vec );
			if (vec->lines != (LINE_FEATURE **)NULL)
			  Trans_vec_lines(mapgeo, vec );
			if (vec->text != (TEXT_FEATURE **)NULL)
			  Trans_vec_text(mapgeo, vec );
			if (vec->points != (POINT_FEATURE **)NULL)
			  Trans_vec_points(mapgeo, vec );

			if((mapgeo->geo_to_bit_errors > 0) || (mapgeo->bit_to_geo_errors > 0)) 
			  pacifyVmapConv("Unable to convert point(s) between DD and map system.", -1 );
		}
	}
	return STAT_SUCCESS;
}


void Free_Vec( view_type* view )
{
	if ( view == NULL  )
		return;

	for ( int i = 0 ; i < view->nthemes ; ++i)
	{
		VEC* vec = view->theme[i].vec;
		if( vec  )
		{
			if (vec->areas != (AREA_FEATURE **)NULL)
			  Free_Vec_Areas( vec );
			if (vec->lines != (LINE_FEATURE **)NULL)
			  Free_Vec_Lines( vec );
			if (vec->text != (TEXT_FEATURE **)NULL)
			  Free_Vec_Text( vec );
			if (vec->points != (POINT_FEATURE **)NULL)
			  Free_Vec_Points( vec );
		}
	}
}


void bit_and_geo_ops_complete( MAPGEO * mapgeo )
{
   mapgeo->bit_to_geo_errors = 0;
   mapgeo->geo_to_bit_errors = 0;
   mapgeo->hide_transform_err_messg = false;
}
void Free_Vec_Lines( VEC * vec )
{
	if(vec == (VEC *) NULL)
	{
	  iErrorVmapConv("drawvec call with a null");
	}
  
	for ( int i = 0; i < vec->nr_lines; i++)
	{
		Ppoint_list_list* lines = vec->lines[i]->lines;
		for( int j = 0 ; j < vec->lines[i]->nr_segs ; j++ )
		{
			if ( lines->point_lists[j].points )
				free( lines->point_lists[j].points );

		}
		if ( lines->point_lists )
			free( lines->point_lists );
		if ( vec->lines[i]->lines )
			free( vec->lines[i]->lines );
	}
}

/*
ERRSTATUS Trans1CoordUtmToBit( float rXCoord, float rYCoord, float* rX, float* rY )
{
	GFLOAT rXT, rYT;
	if ( utm_to_bit_for_data( mapgeo, (GFLOAT)rXCoord,(GFLOAT)rYCoord, &rXT, &rYT ) == 0 )
	{	  
		*rX = (float)rXT;
		*rY = (float)rYT;
		return STAT_SUCCESS;
	}
	return STAT_DATA_ERR;
}

ERRSTATUS Trans1CoordBitToUtm( float rXCoord, float rYCoord, float* rX, float* rY )
{
	GFLOAT rXT, rYT;
	if ( bit_to_utm_for_data( mapgeo, (GFLOAT)rXCoord,(GFLOAT)rYCoord, &rXT, &rYT ) == 0 )
	{	  
		*rX = (float)rXT;
		*rY = (float)rYT;
		return STAT_SUCCESS;
	}
	return STAT_DATA_ERR;
}
*/
ERRSTATUS Trans1CoordGeoToBit( MT_Float rXCoord, MT_Float rYCoord, MT_Float* rX, MT_Float* rY )
{
	GFLOAT rXT, rYT;
	if ( geo_to_bit_for_data( mapgeo, (GFLOAT)rXCoord,(GFLOAT)rYCoord, &rXT, &rYT ) == 0 )
	{	  
		*rX = (float)rXT;
		*rY = (float)rYT;
		return STAT_SUCCESS;
	}
	return STAT_DATA_ERR;
}

ERRSTATUS Trans1CoordBitToGeo( MT_Float rXCoord, MT_Float rYCoord, MT_Float* rX, MT_Float* rY )
{
	GFLOAT rXT, rYT;
	if ( bit_to_geo_for_data( mapgeo, (GFLOAT)rXCoord,(GFLOAT)rYCoord, &rXT, &rYT ) == 0 )
	{	  
		*rX = (float)rXT;
		*rY = (float)rYT;
		return STAT_SUCCESS;
	}
	return STAT_DATA_ERR;
}

/*
ERRSTATUS Trans1CoordMgrsToBit( const std::string& sMgrs, MT_Vector3D& vPos )
{
	GFLOAT rXT, rYT;
	if ( Mgrs_to_Bit_for_data( sMgrs.c_str(), &rXT, &rYT ) == 0 )
	{	  
		vPos.rX_ = rXT;
		vPos.rY_ = rYT;
		vPos.rZ_ = 0;
		return STAT_SUCCESS;
	}
	return STAT_DATA_ERR;
}

ERRSTATUS Trans1CoordBitToMgrs( const MT_Vector3D& vPos, std::string& sMgrs )
{
	char pMgrs[256];
	if ( Bit_to_Mgrs_for_data( vPos.rX_, vPos.rY_, pMgrs ) == 0 )
	{	  
		sMgrs.assign( &pMgrs[0], &pMgrs[15] );
		return STAT_SUCCESS;
	}
	return STAT_DATA_ERR;
}
*/

ERRSTATUS Trans_vec_lines(MAPGEO * mapgeo, VEC * vec )
{
  int            i,j,k, curr_nr_points = 0;
  GFLOAT          x, y;
  double          last_lon;
  
	bool			use_outside_point=false;
	COORDINATE      temp_point;

  if(vec == (VEC *) NULL)
  {
      iErrorVmapConv("drawvec call with a null");
      return -1;
  }
  

  for (i=0; i < vec->nr_lines; i++)
  {
		Ppoint_list_list* lines = vec->lines[i]->lines  = (Ppoint_list_list*)malloc( sizeof( Ppoint_list_list ) );
		if(lines == (Ppoint_list_list *)NULL)
		  return(STAT_MEM_ALLOC_ERR);

		lines->num_point_lists = vec->lines[i]->nr_segs;
		lines->point_lists = (Ppoint_list *)malloc((sizeof(Ppoint_list)*lines->num_point_lists));
		if( lines->point_lists == (Ppoint_list *)NULL)
		  return(STAT_MEM_ALLOC_ERR);
	  
		for( j = 0 ; j < vec->lines[i]->nr_segs ; j++ )
		{	   
			lines->point_lists[j].num_points = 0; 
			lines->point_lists[j].points = (Ppoint *) malloc((vec->lines[i]->segs[j]->nr_coords+2) * sizeof(Ppoint));
			if( lines->point_lists[j].points == (Ppoint *)NULL)
				return(STAT_MEM_ALLOC_ERR);
		  use_outside_point = false;
	      for (k = 0; k < vec->lines[i]->segs[j]->nr_coords;)
	      {	  
			  last_lon = (GFLOAT) vec->lines[i]->segs[j]->coords[k].x;
              while(	( k < vec->lines[i]->segs[j]->nr_coords )
					&&	geo_extent_hit_test_line(mapgeo, vec->lines[i]->segs[j]->coords[k].y, vec->lines[i]->segs[j]->coords[k].x, true, last_lon) 
					)
              {
			      if( use_outside_point ) 
	                  if( geo_to_bit_for_data(mapgeo, temp_point.x, temp_point.y, &x, &y) == 0 ) 
                      {
	                      lines->point_lists[j].points[0].x = x;
	                      lines->point_lists[j].points[0].y = y;
			              curr_nr_points++;
				      }
				  use_outside_point = false;
	              if(  geo_to_bit_for_data(mapgeo, (GFLOAT)vec->lines[i]->segs[j]->coords[k].x,
			                (GFLOAT) vec->lines[i]->segs[j]->coords[k].y, &x, &y)  == 0 )
                  {	  
	                  lines->point_lists[j].points[curr_nr_points].x = x;
	                  lines->point_lists[j].points[curr_nr_points].y = y;
			          curr_nr_points++;
			          last_lon = (GFLOAT) vec->lines[i]->segs[j]->coords[k].x;
				      k++;
				  }
				  else
				  {
			          last_lon = (GFLOAT) vec->lines[i]->segs[j]->coords[k].x;
				      k++;
					  break;
				  }
		      } /* end while */

			  if( fabs(mapgeo->plot_params.lon_shift_factor) < ANIOTA )
			      use_outside_point = true;
		      if( curr_nr_points > 0 )
			  {

			      if( fabs(mapgeo->plot_params.lon_shift_factor) < ANIOTA )
			      {
	                if(  (k < vec->lines[i]->segs[j]->nr_coords) && 
	                   geo_to_bit_for_data(mapgeo, (GFLOAT) vec->lines[i]->segs[j]->coords[k].x,
			                (GFLOAT) vec->lines[i]->segs[j]->coords[k].y, &x, &y)  == 0 ) 
                    {
	                  lines->point_lists[j].points[curr_nr_points].x = x;
	                  lines->point_lists[j].points[curr_nr_points].y = y;
			          curr_nr_points++;
				    }
				  }
			      lines->point_lists[j].num_points = curr_nr_points;
					curr_nr_points = 0;
			  }
			  if( k < vec->lines[i]->segs[j]->nr_coords )
			  {
			      temp_point.x = (GFLOAT) vec->lines[i]->segs[j]->coords[k].x;
				  temp_point.y = (GFLOAT) vec->lines[i]->segs[j]->coords[k].y;
			  }
			  k++;
	      } /* end for loop with k index */
	  
//		   free((char *)lines->points);
	  }
  }
//  free((char *)lines);
  return STAT_SUCCESS;
}

void Free_Vec_Text( VEC * vec )
{
    for (int i = 0; i < vec->nr_text; i++ )
    {
		Ppoint_list* texts = vec->text[i]->texts;
		if ( texts->points )
			free( texts->points );
		if ( vec->text[i]->texts )
			free( vec->text[i]->texts );
	}
}

ERRSTATUS Trans_vec_text(MAPGEO * mapgeo, VEC * vec )
{
    int             i,
                    j;
    GFLOAT          x,
                    y;
	bool			vpf_show_all_text = false;

    for (i=0; i<vec->nr_text; i++)
    {
		Ppoint_list* texts = vec->text[i]->texts  = (Ppoint_list*)malloc( sizeof(Ppoint_list) );
		if( texts == (Ppoint_list *)NULL)
			return(STAT_MEM_ALLOC_ERR);

		texts->num_points = vec->text[i]->nr_coords; 
		texts->points = (Ppoint *) malloc((vec->text[i]->nr_coords) * sizeof(Ppoint));
		if( texts->points == (Ppoint *)NULL)
			return(STAT_MEM_ALLOC_ERR);

/*        if (!vpf_show_all_text)
        {
            bool within_extents = false;
            for (j = 0 ; j < vec->text[i]->nr_coords && !within_extents ; j++)
            {
                if (geo_extent_hit_test_point(mapgeo,vec->text[i]->coords[j].y,vec->text[i]->coords[j].x))
                {
                    if (geo_to_bit_for_data(mapgeo,(GFLOAT)vec->text[i]->coords[j].x,(GFLOAT)vec->text[i]->coords[j].y,&x,&y) == 0)
                    {
                        texts->points[j].x = x;
                        texts->points[j].y = y;
//                        if (vec->text[i]->string != (char *)NULL)
//								DrawTextString( &text, vec->text[i]->string );
                        within_extents = true;
                    }
                }
            }
        }
        else
        {
*/          for (j = 0; j < vec->text[i]->nr_coords ; j++)
            {	  
                if (geo_extent_hit_test_point(mapgeo,vec->text[i]->coords[j].y,vec->text[i]->coords[j].x ))
                { 
                    if (geo_to_bit_for_data(mapgeo,(GFLOAT)vec->text[i]->coords[j].x,(GFLOAT)vec->text[i]->coords[j].y,&x,&y) == 0)
                    {	
                        texts->points[j].x = x;
                        texts->points[j].y = y;
                    }	
                } 
            }
//        }
    }
    return STAT_SUCCESS;

}

void Free_Vec_Points( VEC * vec )
{
	for( int i = 0; i < vec->nr_points; i++ )
	{
		Ppoint_list* points = vec->points[i]->points;
		if ( points->points )
			free( points->points );
		if ( vec->points[i]->points )
			free( vec->points[i]->points );
		
	}
}

ERRSTATUS Trans_vec_points(MAPGEO * mapgeo, VEC * vec )
{
  int            i;
  GFLOAT          x, y;
  
  for( i = 0; i < vec->nr_points; i++ )
  {
	  Ppoint_list* points = vec->points[i]->points = (Ppoint_list *) malloc(sizeof(Ppoint_list));
	  if(points == (Ppoint_list *)NULL)
		  return(STAT_MEM_ALLOC_ERR);
	  points->num_points = 1;
	  points->points = (Ppoint *) malloc(points->num_points * sizeof(Ppoint));
	  if(points->points == (Ppoint *)NULL)
		  return(STAT_MEM_ALLOC_ERR);

      if( geo_extent_hit_test_point(mapgeo, vec->points[i]->coord->y, vec->points[i]->coord->x ) )
      {
          if( geo_to_bit_for_data(mapgeo,(GFLOAT) vec->points[i]->coord->x,(GFLOAT) vec->points[i]->coord->y,&x, &y) == 0 )
          {
				points->points->x = x;
				points->points->y = y;
          }
	  }

  }
//  free((char *)points->points);
//  free((char *)points);
  return STAT_SUCCESS;
}

void Free_Vec_Areas( VEC * vec )
{
    for ( int i = 0 ; i < vec->nr_areas; i++ )
    {
		Ppoint_list_list* area = vec->areas[i]->area;
        for( int j = 0; j < vec->areas[i]->nr_rings; j++)
	    {
			if ( area->point_lists[j].points )
				free( area->point_lists[j].points );
		}
		if ( area->point_lists )
			free( area->point_lists );
		if ( vec->areas[i]->area )
			free( vec->areas[i]->area );
	}
}

ERRSTATUS Trans_vec_areas(MAPGEO * mapgeo, VEC * vec)
{
  long            i,j,k,l;
  long            tcoords, ncoord;
  GFLOAT          x, y;
  //bool         draw_markers_feat=false, draw_markers_rec=false;
  
    for (i=0; i < vec->nr_areas; i++)
    {

		Ppoint_list_list* area = vec->areas[i]->area = (Ppoint_list_list *) malloc(sizeof(Ppoint_list_list));
		if(area == (Ppoint_list_list *)NULL)
			return(STAT_MEM_ALLOC_ERR);

        area->num_point_lists = vec->areas[i]->nr_rings;
        area->point_lists = (Ppoint_list *)malloc((sizeof(Ppoint_list)*area->num_point_lists));
        if(area->point_lists == (Ppoint_list *)NULL)
	        return(STAT_MEM_ALLOC_ERR);
        for(j=0; j< vec->areas[i]->nr_rings; j++)
	    {
	        tcoords=0;
	        for(k=0; k< vec->areas[i]->rings[j]->nr_segs; k++)
	        {
	            tcoords = tcoords + vec->areas[i]->rings[j]->segs[k]->nr_coords;
            }
	        area->point_lists[j].num_points = tcoords;
            area->point_lists[j].points = (Ppoint *)malloc(area->point_lists[j].num_points * sizeof(Ppoint));
	        if(area->point_lists[j].points == (Ppoint *)NULL)
	            return(STAT_MEM_ALLOC_ERR);
	  
	        ncoord = 0;
	        for(k=0; k< vec->areas[i]->rings[j]->nr_segs; k++)
	        {
	            for(l=0; l < vec->areas[i]->rings[j]->segs[k]->nr_coords; l++)
		        {
		            geo_to_bit_for_data(mapgeo,
			            (GFLOAT) vec->areas[i]->rings[j]->segs[k]->coords[l].x,
			            (GFLOAT) vec->areas[i]->rings[j]->segs[k]->coords[l].y,
			            &x, &y);
		            area->point_lists[j].points[ncoord].x = x;
		            area->point_lists[j].points[ncoord].y = y;
		            ncoord++;
		        }
	        }
	    }
  
    }
    return STAT_SUCCESS;

}


bool geo_extent_hit_test_line( MAPGEO *mapgeo, double lat, double lon, bool check_edge, double last_lon )
{
	double shift_lon, lon_edge;

	  /* if lat is at or outside extents on any map, hit test has failed and you're done 
	     (consider point at extent to be outside map to prevent unnecessary drawing to another point 
	      outside the map) */
      if( (lat < mapgeo->geo_extent.bottom) ||
		  (lat > mapgeo->geo_extent.top) )
	      return false;
	  /* lon hit test depends on whether map is shifted +-360 */
	  if( fabs(mapgeo->plot_params.lon_shift_factor) < ANIOTA )
	  {
	      /* if lon is inside extents on a +-180 map, hit test has succeeded and you're done */
          if( (lon >= mapgeo->geo_extent.left) && (lon <= mapgeo->geo_extent.right) )
		      return true;
	  }
	  else	 /* map is shifted +-360 */
	  {
		  if( check_edge)
		  {
 			  /*  return false if crossing left or right map edges */
			  if( last_lon < lon )
			  {
				  lon_edge = mapgeo->geo_extent.right;
				  geo_360_to_180( &lon_edge );
				  if( (last_lon < lon_edge) && (lon > lon_edge) )
					  return false;
			  }
			  else if( last_lon > lon )
 			  {
				  lon_edge = mapgeo->geo_extent.left;
				  geo_360_to_180( &lon_edge );
				  if( (last_lon > lon_edge) && (lon < lon_edge) )
					  return false;
			  }
		  }
		  /* Otherwise, if lon is inside extents on a +-360 map, hit test has succeeded and you're done */ 
          if( (lon >= mapgeo->geo_extent.left) && (lon <= mapgeo->geo_extent.right) )
		  {
			  return true;
		  }
		  else  /* outside extents */
		  {
		      /* if lon is at or outside extents on a +-360 map, shift and check again */
		      shift_lon = lon;
		      shift_lon += mapgeo->plot_params.lon_shift_factor;
		      if(  (shift_lon >= mapgeo->geo_extent.left) &&
		       (shift_lon <= mapgeo->geo_extent.right) )
		      {
			      return true;
              }
		      else
			      return false;
		  }
	  }

	 return false;
 }

bool geo_extent_hit_test_point( MAPGEO *mapgeo, double lat, double lon )
{
	double shift_lon;


 	shift_lon = lon;
	geo_180_to_360( mapgeo, &shift_lon );
    if( (shift_lon >= mapgeo->geo_extent.left) &&
		(shift_lon <= mapgeo->geo_extent.right) && 
		(lat >= mapgeo->geo_extent.bottom) &&
		(lat <= mapgeo->geo_extent.top) )
	    return true;
    else
	    return false;
 }

ERRSTATUS	mapgeo_construct( double rImageWidth, double rImageHeight )
{
	rDefImageWidth = rImageWidth;
	rDefImageHeight = rImageHeight;
	FILE *fp=NULL;
   char record[SZ_MAPGEO];
   size_t items_read = 0;
   MAPGEO  *mapgeo_ptr = mapgeo;

   void *temp_ptr;

   mapgeo_destruct((FILE *)NULL, true, true, &mapgeo_ptr);

	if( dtcc_initialize_lists( "D:\\VMap1\\EURNASIA\\ellplst.txt", "D:\\VMap1\\EURNASIA\\hdatmlst.txt", "D:\\VMap1\\EURNASIA\\systmlst.txt") < 0)
	    iErrorVmapConv("Failed to open the DT&CC files ellplst.txt, hdatmlst.txt, and systmlst.txt. Please install them.");


  if((mapgeo_ptr = (MAPGEO *)zMallocConv(sizeof(MAPGEO))) == (MAPGEO *)NULL)
      return STAT_MEM_ALLOC_ERR;

	 mapgeo = mapgeo_ptr;

   if(fp)
   {
      items_read = fread(&record, (size_t)SZ_MAPGEO, (size_t)1, fp);
      if(items_read != 1)
      {
         {
            iErrorVmapConv("Unable to read mapgeo data");
            mapgeo_destruct((FILE *)NULL, true, true, &mapgeo_ptr);
            mapgeo_ptr = (MAPGEO *)NULL;
            return STAT_FILE_READ_ERR;
         }
      }
      decode_mapgeo((unsigned char *)record, mapgeo_ptr);
      if(mapgeo_ptr->magic != MAGIC_MAPGEO)
      {
         iErrorVmapConv("The mapgeo data read in is incorrect\n");
         mapgeo_destruct((FILE *)NULL, true, true, &mapgeo_ptr);
         mapgeo_ptr = (MAPGEO *)NULL;
         return STAT_DATA_ERR;
      }


      dtcc_reset_system(&mapgeo->map_display_coord_sys, "WGS84" );
/*	  if( dtcc_read_system( &mapgeo_ptr->map_display_coord_sys, fp) )
      {
          iErrorVmap("Unable to read map display coordinate system");
          return STAT_FILE_READ_ERR;

      }
*/
      dtcc_reset_system(&mapgeo->user_coord_sys, "WGS84");
/*    if( dtcc_read_system( &mapgeo_ptr->user_coord_sys, fp) )
      {
          iErrorVmap("Unable to read user coordinate system");
          return STAT_FILE_READ_ERR;
      }
*/
 }

   if(extent_construct(fp, (defEXTENT **)&temp_ptr) != STAT_SUCCESS)
   {
      mapgeo_destruct((FILE *)NULL, true, true, &mapgeo_ptr);
      mapgeo_ptr = (MAPGEO *)NULL;
      return STAT_DATA_ERR;
   }
   mapgeo_ptr->geo_extent = *((defEXTENT *)temp_ptr);
   free((char *)((defEXTENT *)temp_ptr));

  if(plot_params_construct(fp, (PLOT_PARAMS **)&temp_ptr) != STAT_SUCCESS)
   {
      mapgeo_destruct((FILE *)NULL, true, true, &mapgeo_ptr);
      mapgeo_ptr = (MAPGEO *)NULL;
      return STAT_DATA_ERR;
   }
   mapgeo_ptr->plot_params = *((PLOT_PARAMS *)temp_ptr);
   free((char *)((PLOT_PARAMS *)temp_ptr));

  
   if(mapgeo_ini(fp, mapgeo_ptr) != STAT_SUCCESS)
   {
      mapgeo_destruct((FILE *)NULL, true, true, &mapgeo_ptr);
      mapgeo_ptr = (MAPGEO *)NULL;
      return STAT_DATA_ERR;
   }

/*	
	double londd = 02.36103;
	double latdd = 48.75138;

	char sMgrs[24];
	Geo_to_Mgrs_for_data( mapgeo_ptr, londd , latdd, sMgrs );
	double bitx, bity;
	Mgrs_to_Bit_for_data( mapgeo_ptr, sMgrs, &bitx, &bity);
	double londd2, latdd2;
	bit_to_geo_for_data( mapgeo_ptr, bitx, bity, &londd2, &latdd2 );
*/	
	 
	
	return STAT_SUCCESS;
}

ERRSTATUS mapgeo_construct( MT_Float rImageWidth, MT_Float rImageHeight, MT_Float rXMin, MT_Float rXMax, MT_Float rYMin, MT_Float rYMax, const std::string& strGeoEllipsoid, const std::string& strGeoDatum, const std::string& strGeoSystem )
{
    if ( params == 0 )
        params = (USER_PARAMS*)zMallocConv(sizeof (USER_PARAMS));

	params->x_max = rXMax;
	params->y_max = rYMax;
	params->x_min = rXMin;
	params->y_min = rYMin;

	rDefImageWidth = rImageWidth;
	rDefImageHeight = rImageHeight;
	FILE *fp=NULL;
   char record[SZ_MAPGEO];
   size_t items_read = 0;
   MAPGEO  *mapgeo_ptr = mapgeo;

   void *temp_ptr;

   mapgeo_destruct((FILE *)NULL, true, true, &mapgeo_ptr);

//	if( dtcc_initialize_lists( "SCIPIO datas\\TER\\Geo\\ellplst.txt", "SCIPIO datas\\TER\\Geo\\hdatmlst.txt", "SCIPIO datas\\TER\\Geo\\systmlst.txt") < 0)
//	    iErrorVmapConv("Failed to open the DT&CC files ellplst.txt, hdatmlst.txt, and systmlst.txt. Please install them.");

   //$$$ Mettre dans fichier de conf
//   if( dtcc_initialize_lists( "D:\\VMap1\\EURNASIA\\ellplst.txt", "D:\\VMap1\\EURNASIA\\hdatmlst.txt", "D:\\VMap1\\EURNASIA\\systmlst.txt") < 0)
//	    iErrorVmapConv("Failed to open the DT&CC files ellplst.txt, hdatmlst.txt, and systmlst.txt. Please install them.");

   if( dtcc_initialize_lists( strGeoEllipsoid.c_str(), strGeoDatum.c_str(), strGeoSystem.c_str() ) < 0)
     iErrorVmapConv("Failed to open the DT&CC files ellplst.txt, hdatmlst.txt, and systmlst.txt. Please install them.");

  if((mapgeo_ptr = (MAPGEO *)zMallocConv(sizeof(MAPGEO))) == (MAPGEO *)NULL)
      return STAT_MEM_ALLOC_ERR;

	 mapgeo = mapgeo_ptr;

   if(fp)
   {
      items_read = fread(&record, (size_t)SZ_MAPGEO, (size_t)1, fp);
      if(items_read != 1)
      {
         {
            iErrorVmapConv("Unable to read mapgeo data");
            mapgeo_destruct((FILE *)NULL, true, true, &mapgeo_ptr);
            mapgeo_ptr = (MAPGEO *)NULL;
            return STAT_FILE_READ_ERR;
         }
      }
      decode_mapgeo((unsigned char *)record, mapgeo_ptr);
      if(mapgeo_ptr->magic != MAGIC_MAPGEO)
      {
         iErrorVmapConv("The mapgeo data read in is incorrect\n");
         mapgeo_destruct((FILE *)NULL, true, true, &mapgeo_ptr);
         mapgeo_ptr = (MAPGEO *)NULL;
         return STAT_DATA_ERR;
      }


      dtcc_reset_system(&mapgeo->map_display_coord_sys, "WGS84" );
/*	  if( dtcc_read_system( &mapgeo_ptr->map_display_coord_sys, fp) )
      {
          iErrorVmapConv("Unable to read map display coordinate system");
          return STAT_FILE_READ_ERR;

      }
*/
      dtcc_reset_system(&mapgeo->user_coord_sys, "WGS84");
/*    if( dtcc_read_system( &mapgeo_ptr->user_coord_sys, fp) )
      {
          iErrorVmapConv("Unable to read user coordinate system");
          return STAT_FILE_READ_ERR;
      }
*/
      dtcc_reset_system(&mapgeo->user2_coord_sys, "WGS84");
/*    if( dtcc_read_system( &mapgeo_ptr->user2_coord_sys, fp) )
      {
          iErrorVmapConv("Unable to read user2 coordinate system");
          return STAT_FILE_READ_ERR;
      }
*/

}

   if(extent_construct(fp, (defEXTENT **)&temp_ptr) != STAT_SUCCESS)
   {
      mapgeo_destruct((FILE *)NULL, true, true, &mapgeo_ptr);
      mapgeo_ptr = (MAPGEO *)NULL;
      return STAT_DATA_ERR;
   }
   mapgeo_ptr->geo_extent = *((defEXTENT *)temp_ptr);
   free((char *)((defEXTENT *)temp_ptr));

  if(plot_params_construct(fp, (PLOT_PARAMS **)&temp_ptr) != STAT_SUCCESS)
   {
      mapgeo_destruct((FILE *)NULL, true, true, &mapgeo_ptr);
      mapgeo_ptr = (MAPGEO *)NULL;
      return STAT_DATA_ERR;
   }
   mapgeo_ptr->plot_params = *((PLOT_PARAMS *)temp_ptr);
   free((char *)((PLOT_PARAMS *)temp_ptr));

  
   if(mapgeo_ini(fp, mapgeo_ptr) != STAT_SUCCESS)
   {
      mapgeo_destruct((FILE *)NULL, true, true, &mapgeo_ptr);
      mapgeo_ptr = (MAPGEO *)NULL;
      return STAT_DATA_ERR;
   }

/*	
	double londd = 02.36103;
	double latdd = 48.75138;
	double bitx, bity;

	geo_to_bit_for_data( mapgeo_ptr, londd, latdd, &bitx, &bity);

	char sMgrs[24];
	Geo_to_Mgrs_for_data( londd , latdd, sMgrs );
	Mgrs_to_Bit_for_data( sMgrs, &bitx, &bity);
	double londd2, latdd2;
	bit_to_geo_for_data( bitx, bity, &londd2, &latdd2 );
*/	

/*
	long iZone;
	double lonUtm, latUtm;
	Geo_to_Utm_for_data( londd , latdd, &iZone, &lonUtm, &latUtm );
	Utm_to_Bit_for_data( iZone, lonUtm, latUtm, &bitx, &bity);
	double londd2, latdd2;
	bit_to_geo_for_data( mapgeo_ptr, bitx, bity, &londd2, &latdd2 );
*/
	return STAT_SUCCESS;
}

ERRSTATUS mapgeo_ini(FILE *fp, MAPGEO *mapgeo_ptr)
{
   ERRSTATUS errcode = STAT_SUCCESS;
   /* Initialization code for the MAPGEO constructor */

   if(fp)
   {

	    /* WGS84/GP is data_coord_sys. If/when data products do not use it, code must be modified.
	       Setup of other systems is in settings_ini() so they're initialized properly if read in from muse.ini*/
	    dtcc_reset_default_geodetic_system( &mapgeo_ptr->data_coord_sys);
       /* 2/98 */
		if( !(mapgeo_ptr->use_arc==0 || mapgeo_ptr->use_arc==1) )
			mapgeo_ptr->use_arc=false;
   }
   else
   {
	    dtcc_reset_default_geodetic_system( &mapgeo_ptr->internal_coord_sys); 
        dtcc_reset_default_geodetic_system( &mapgeo_ptr->map_display_coord_sys);

#ifdef MGRS_TRANSFO
	    dtcc_reset_default_MGRS_system( &mapgeo_ptr->user_coord_sys); 
#endif //MGRS_TRANSFO

#ifdef UTM_TRANSFO
		dtcc_reset_default_UTM_system( &mapgeo_ptr->user2_coord_sys); 
#endif //UTM_TRANSFO
	    dtcc_reset_default_geodetic_system( &mapgeo_ptr->data_coord_sys); 
		/* compute center */
		dtcc_set_coord3_geo(&mapgeo_ptr->plot_params.map_center, ( mapgeo->geo_extent.bottom + mapgeo->geo_extent.top ) / 2.0,( mapgeo->geo_extent.left + mapgeo->geo_extent.right ) / 2.0, DOUBLE_NULL );
		dtcc_set_coord3_lon(&mapgeo_ptr->plot_params.map_ll, mapgeo->geo_extent.left);
		dtcc_set_coord3_lon(&mapgeo_ptr->plot_params.map_ur, mapgeo->geo_extent.right);
		dtcc_set_coord3_lat(&mapgeo_ptr->plot_params.map_ll, mapgeo->geo_extent.bottom);
		dtcc_set_coord3_lat(&mapgeo_ptr->plot_params.map_ur, mapgeo->geo_extent.top);

		mapgeo->internal_to_user = (TRANSFORM_SYS *) zMallocConv(sizeof(TRANSFORM_SYS)); 
    		mapgeo->internal_to_user2 = (TRANSFORM_SYS *) zMallocConv(sizeof(TRANSFORM_SYS)); 
		mapgeo->internal_to_display = (TRANSFORM_SYS *)zMallocConv(sizeof(TRANSFORM_SYS)); 
		mapgeo->user_to_internal = (TRANSFORM_SYS *)zMallocConv(sizeof(TRANSFORM_SYS)); 
    		mapgeo->user2_to_internal = (TRANSFORM_SYS *)zMallocConv(sizeof(TRANSFORM_SYS)); 
		mapgeo->display_to_internal = (TRANSFORM_SYS *)zMallocConv(sizeof(TRANSFORM_SYS)); 
		mapgeo->data_to_display = (TRANSFORM_SYS *)zMallocConv(sizeof(TRANSFORM_SYS)); 
		mapgeo->display_to_data = (TRANSFORM_SYS *)zMallocConv(sizeof(TRANSFORM_SYS)); 

		if( (errcode = setup_transform_struct((const COORD_SYS *)&mapgeo->internal_coord_sys,(const COORD_SYS *)&mapgeo->user_coord_sys, mapgeo->internal_to_user)) < 0)
		   return errcode;
		    if( (errcode = setup_transform_struct((const COORD_SYS *)&mapgeo->internal_coord_sys,(const COORD_SYS *)&mapgeo->user2_coord_sys, mapgeo->internal_to_user2)) < 0)
		       return errcode;

		if( (errcode = setup_transform_struct((const COORD_SYS *)&mapgeo->user_coord_sys,(const COORD_SYS *)&mapgeo->internal_coord_sys, mapgeo->user_to_internal)) < 0 )
			return errcode;
            if( (errcode = setup_transform_struct((const COORD_SYS *)&mapgeo->user2_coord_sys,(const COORD_SYS *)&mapgeo->internal_coord_sys, mapgeo->user2_to_internal)) < 0 )
			    return errcode;

        
		if( (errcode = setup_transform_struct((const COORD_SYS *)&mapgeo->internal_coord_sys,(const COORD_SYS *)&mapgeo->map_display_coord_sys , mapgeo->internal_to_display)) < 0  )
		   return errcode;
		if( (errcode = setup_transform_struct((const COORD_SYS *)&mapgeo->map_display_coord_sys,(const COORD_SYS *)&mapgeo->internal_coord_sys,mapgeo->display_to_internal)) < 0 )
			return errcode;
		if( (errcode = setup_transform_struct((const COORD_SYS *)&mapgeo->data_coord_sys,(const COORD_SYS *)&mapgeo->map_display_coord_sys ,mapgeo->data_to_display)) < 0  )
		   return errcode;
		if( (errcode = setup_transform_struct((const COORD_SYS *)&mapgeo->map_display_coord_sys,(const COORD_SYS *)&mapgeo->data_coord_sys, mapgeo->display_to_data)) < 0 )
		   return errcode;

	   mapgeo_adjust( mapgeo, false );

   } 

	mapgeo_ptr->plot_params.lon_shift_factor = 0.0;
	if( mapgeo_ptr->geo_extent.left < -180.0)
	{
		mapgeo_ptr->plot_params.lon_shift_factor = -360.0;
	}
	else if( mapgeo_ptr->geo_extent.right > 180.0)
	{
		mapgeo_ptr->plot_params.lon_shift_factor = 360.0;
	}
   /* setup of transform structures is in settings_ini() so they're initialized properly if display
      and user sys's are read in from muse.ini */
   return STAT_SUCCESS;
}

ERRSTATUS plot_params_construct(FILE *fp, PLOT_PARAMS **ptr)
{
//   char record[SZ_PLOT_PARAMS];
   size_t items_read = 0;
   PLOT_PARAMS  *plot_params_ptr;


   if(sizeof(PLOT_PARAMS) >= SZ_PLOT_PARAMS)
      iErrorVmapConv("Increase SZ_PLOT_PARAMS");
   if((plot_params_ptr = (PLOT_PARAMS *)
            zMallocConv(sizeof(PLOT_PARAMS))) == (PLOT_PARAMS *)NULL)
      return STAT_MEM_ALLOC_ERR;
   plot_params_ptr->magic = MAGIC_PLOT_PARAMS;
   if(plot_params_ini(fp, plot_params_ptr) != STAT_SUCCESS)
   {
      plot_params_destruct((FILE *)NULL, true, true, &plot_params_ptr);
      *ptr = (PLOT_PARAMS *)NULL;
      return STAT_DATA_ERR;
   }
   *ptr = plot_params_ptr;
   return STAT_SUCCESS;
}

ERRSTATUS plot_params_ini(FILE *fp, PLOT_PARAMS *plot_params_ptr)
{
   /* Initialization code for the PLOT_PARAMS constructor */

   if(fp)
   {	 /* 12/97--Now reading and writing map_pref.  Earlier maps may have
	        garbage in the field. Use Map Pref I as default, because muse.ini can later reset 
	        center point, scale and image size, but not extents */
         /* plot_params_ptr->map_pref = CENTER_POINT_SCALE_IMAGE_EXTENT; */
	     if( (plot_params_ptr->map_pref < CENTER_POINT_SCALE_IMAGE_EXTENT) ||
			 (plot_params_ptr->map_pref > MAP_AND_IMAGE_EXTENT) )
			 plot_params_ptr->map_pref = CENTER_POINT_SCALE_IMAGE_EXTENT;
   }
   else
   {

        plot_params_ptr->image_origin_x = rDefImageOriginX;
        /*plot_params_ptr->image_origin_y = TOOLBAR_HEIGHT;*/
        plot_params_ptr->image_origin_y = rDefImageOriginY;
        plot_params_ptr->pixels_per_meter_x = rDefPixelsPerMeterX;
        plot_params_ptr->pixels_per_meter_y = rDefPixelsPerMeterY;

		dtcc_set_coord3_geo(&plot_params_ptr->map_center, 0.0, 0.0, DOUBLE_NULL);
        dtcc_set_coord3_geo(&plot_params_ptr->map_ll, -90, -180, DOUBLE_NULL);
        dtcc_set_coord3_geo(&plot_params_ptr->map_ur, 90.0, 180.0, DOUBLE_NULL);
  		dtcc_set_coord3_geo(&plot_params_ptr->map_ll_display, -90.0, -180.0, DOUBLE_NULL);
		dtcc_set_coord3_geo(&plot_params_ptr->map_ur_display, 90.0, 180.0, DOUBLE_NULL);

        plot_params_ptr->magic = MAGIC_PLOT_PARAMS;
        plot_params_ptr->map_pref = nDefConv;
		plot_params_ptr->scale_recip = 306665156;
        plot_params_ptr->image_unit = IMAGE_PIXELS;
        plot_params_ptr->image_height_pixels = rDefImageHeight;
        plot_params_ptr->image_width_pixels = rDefImageWidth;
        plot_params_ptr->image_height_pixels_computed = rDefImageHeight;
        plot_params_ptr->image_width_pixels_computed = rDefImageWidth;
  }
   return STAT_SUCCESS;
}


ERRSTATUS decode_plot_params(unsigned char *record, PLOT_PARAMS *ptr)
{
   char 	 *end_char;
   short	 big_endian, end_check = 1;
   long  	 c = 0L;
   double t1, t2, t3;
   short         tmpshort;

   /* Check for endian type */

    end_char = (char *)&end_check;
    big_endian = 1 - (short)(*end_char);

   char_to_long(record, &ptr->magic, big_endian, &c);
   char_to_double(record, &ptr->pixels_per_meter_x, big_endian, &c);
   char_to_double(record, &ptr->pixels_per_meter_y, big_endian, &c);
   char_to_double(record, &ptr->image_height_pixels, big_endian, &c);
   char_to_double(record, &ptr->image_width_pixels, big_endian, &c);
   char_to_double(record, &ptr->scale_recip, big_endian, &c);
   char_to_double(record, &ptr->sz, big_endian, &c);
   char_to_double(record, &ptr->sy, big_endian, &c);
   char_to_double(record, &ptr->sx, big_endian, &c);
   char_to_double(record, &ptr->tz, big_endian, &c);
   char_to_double(record, &ptr->ty, big_endian, &c);
   char_to_double(record, &ptr->tx, big_endian, &c);
   char_to_double(record, &ptr->image_origin_x, big_endian, &c);
   char_to_double(record, &ptr->image_origin_y, big_endian, &c);
   char_to_short(record, &tmpshort, big_endian, &c);
   ptr->image_unit = (IMAGE_UNIT) tmpshort;   
   char_to_double(record, &t1, big_endian, &c);
   char_to_double(record, &t2, big_endian, &c);
   char_to_double(record, &t3, big_endian, &c);
   dtcc_set_coord3_geo(&ptr->map_center, t1, t2, t3);

   char_to_double(record, &t1, big_endian, &c);
   char_to_double(record, &t2, big_endian, &c);
   char_to_double(record, &t3, big_endian, &c);
   dtcc_set_coord3_geo(&ptr->map_ll, t1, t2, t3);

   char_to_double(record, &t1, big_endian, &c);
   char_to_double(record, &t2, big_endian, &c);
   char_to_double(record, &t3, big_endian, &c);
   dtcc_set_coord3_geo(&ptr->map_ur, t1, t2, t3);

   char_to_double(record, &ptr->image_height_pixels_computed, big_endian, &c);
   char_to_double(record, &ptr->image_width_pixels_computed, big_endian, &c);
   /* SHR 12/97 */
   char_to_short(record, &tmpshort, big_endian, &c);
   ptr->map_pref = (MAP_PREF) tmpshort;   
   return STAT_SUCCESS;
}

ERRSTATUS plot_params_destruct(FILE *fp, bool pointer, bool destruct, PLOT_PARAMS **ptr)
{
   size_t items_written = 0;
   char record[SZ_PLOT_PARAMS];
   PLOT_PARAMS  *plot_params_ptr = *ptr;


   if(ptr == NULL)
      iErrorVmapConv("plot_params_destruct called with NULL");
   if(fp)
   {
      encode_plot_params((unsigned char *)record, plot_params_ptr);
      items_written = fwrite((void *)record, SZ_PLOT_PARAMS, (size_t)1, fp);
      if(items_written != 1)
      {
         iErrorVmapConv("Unable to write plot_params data");
         return STAT_FILE_WRITE_ERR;
      }
   }
   if(destruct)
   {
		if(pointer)
		{
		  free((char *)(plot_params_ptr));
		  *ptr = (PLOT_PARAMS *)NULL;
		}
   }
   return STAT_SUCCESS;
}


ERRSTATUS encode_plot_params(unsigned char *record, PLOT_PARAMS *ptr)
{
   char 	 *end_char;
   short	 big_endian, end_check = 1;
   long  	 c = 0L;
   double    t1, t2, t3;
   short         tmpshort;


   /* Check for endian type */

   end_char = (char *)&end_check;
   big_endian = 1 - (short)(*end_char);

   long_to_char(record, &ptr->magic, big_endian, &c);
   double_to_char(record, &ptr->pixels_per_meter_x, big_endian, &c);
   double_to_char(record, &ptr->pixels_per_meter_y, big_endian, &c);
   double_to_char(record, &ptr->image_height_pixels, big_endian, &c);
   double_to_char(record, &ptr->image_width_pixels, big_endian, &c);
   double_to_char(record, &ptr->scale_recip, big_endian, &c);
   double_to_char(record, &ptr->sz, big_endian, &c);
   double_to_char(record, &ptr->sy, big_endian, &c);
   double_to_char(record, &ptr->sx, big_endian, &c);
   double_to_char(record, &ptr->tz, big_endian, &c);
   double_to_char(record, &ptr->ty, big_endian, &c);
   double_to_char(record, &ptr->tx, big_endian, &c);
   double_to_char(record, &ptr->image_origin_x, big_endian, &c);
   double_to_char(record, &ptr->image_origin_y, big_endian, &c);
   tmpshort = ptr->image_unit;
   short_to_char(record, &tmpshort, big_endian, &c);

   dtcc_get_coord3_geo(&ptr->map_center, &t1, &t2, &t3);
   double_to_char(record, &t1, big_endian, &c);
   double_to_char(record, &t2, big_endian, &c);
   double_to_char(record, &t3, big_endian, &c);

   dtcc_get_coord3_geo(&ptr->map_ll, &t1, &t2, &t3);
   double_to_char(record, &t1, big_endian, &c);
   double_to_char(record, &t2, big_endian, &c);
   double_to_char(record, &t3, big_endian, &c);

   dtcc_get_coord3_geo(&ptr->map_ur, &t1, &t2, &t3);
   double_to_char(record, &t1, big_endian, &c);
   double_to_char(record, &t2, big_endian, &c);
   double_to_char(record, &t3, big_endian, &c);

   double_to_char(record, &ptr->image_height_pixels_computed, big_endian, &c);
   double_to_char(record, &ptr->image_width_pixels_computed, big_endian, &c);
   /* SHR 12/97 */
   tmpshort = ptr->map_pref;
   short_to_char(record, &tmpshort, big_endian, &c);
   return STAT_SUCCESS;
}

ERRSTATUS extent_construct(FILE *fp, defEXTENT **ptr)
{
//   char record[SZ_EXTENT];
   size_t items_read = 0;
   defEXTENT  *extent_ptr;


   if(sizeof(defEXTENT) >= SZ_EXTENT)
      iErrorVmapConv("Increase SZ_EXTENT");
   if((extent_ptr = (defEXTENT *)
            zMallocConv(sizeof(defEXTENT))) == (defEXTENT *)NULL)
      return STAT_MEM_ALLOC_ERR;

   if(extent_ini(fp, extent_ptr) != STAT_SUCCESS)
   {
      extent_destruct((FILE *)NULL, true, true, &extent_ptr);
      *ptr = (defEXTENT *)NULL;
      return STAT_DATA_ERR;
   }
   *ptr = extent_ptr;
   return STAT_SUCCESS;
}

ERRSTATUS extent_ini(FILE *fp, defEXTENT *extent_ptr)
{
   /* Initialization code for the defEXTENT constructor */
   NOREF(extent_ptr);
   if(fp)
   {
     ;
   }
	else
	if ( params )
   {
	extent_ptr->left = params->x_min;
	extent_ptr->right = params->x_max;
	extent_ptr->bottom = params->y_min;
	extent_ptr->top = params->y_max;
   }
   return STAT_SUCCESS;
}

ERRSTATUS extent_destruct(FILE *fp, bool pointer, bool destruct, defEXTENT **ptr)
{
   size_t items_written = 0;
   char record[SZ_EXTENT];
   defEXTENT  *extent_ptr = *ptr;


   if(ptr == NULL)
      iErrorVmapConv("extent_destruct called with NULL");
   if(fp)
   {
      encode_extent((unsigned char *)record, extent_ptr);
      items_written = fwrite((void *)record, SZ_EXTENT, (size_t)1, fp);
      if(items_written != 1)
      {
         iErrorVmapConv("Unable to write extent data");
         return STAT_FILE_WRITE_ERR;
      }
   }
   if(destruct)
   {
	   if(pointer)
	   {
		  free((char *)(extent_ptr));
		  *ptr = (defEXTENT *)NULL;
	   }
   }
   return STAT_SUCCESS;
}


ERRSTATUS encode_extent(unsigned char *record, defEXTENT *ptr)
{
   char 	 *end_char;
   short	 big_endian, end_check = 1;
   long  	 c = 0L;


   /* Check for endian type */

   end_char = (char *)&end_check;
   big_endian = 1 - (short)(*end_char);

   double_to_char(record, &ptr->left, big_endian, &c);
   double_to_char(record, &ptr->right, big_endian, &c);
   double_to_char(record, &ptr->bottom, big_endian, &c);
   double_to_char(record, &ptr->top, big_endian, &c);
   return STAT_SUCCESS;
}

ERRSTATUS decode_mapgeo(unsigned char *record, MAPGEO *ptr)
{
   char 	 *end_char;
   short	 big_endian, end_check = 1;
   long  	 c = 0L;

   /* Check for endian type */

   end_char = (char *)&end_check;
   big_endian = 1 - (short)(*end_char);

	ptr->magic = MAGIC_MAPGEO;
//   char_to_long(record, &ptr->magic, big_endian, &c);
	ptr->use_arc = true;
//   char_to_short(record, (short*)&ptr->use_arc, big_endian, &c);
	ptr->is_locked = true;
//   char_to_short(record, (short*)&ptr->is_locked, big_endian, &c);
   return STAT_SUCCESS;
}


ERRSTATUS mapgeo_destruct(FILE *fp, bool pointer, bool destruct, MAPGEO **ptr)
{
   dtcc_delete_lists();

   size_t items_written = 0;
   char record[SZ_MAPGEO];
   MAPGEO  *mapgeo_ptr = *ptr;
   void *temp_ptr;

   if(ptr == NULL)
      iErrorVmapConv("mapgeo_destruct called with NULL");
   if(fp)
   {
      encode_mapgeo((unsigned char *)record, mapgeo_ptr);
      items_written = fwrite((void *)record, SZ_MAPGEO, (size_t)1, fp);
      if(items_written != 1)
      {
         iErrorVmapConv("Unable to write mapgeo data");
         return STAT_FILE_WRITE_ERR;
      }

/*    if( dtcc_write_system( &mapgeo_ptr->map_display_coord_sys, fp) )
      {
         iErrorVmapConv("Unable to write map display coordinate system");
         return STAT_FILE_WRITE_ERR;
      }
      if( dtcc_write_system( &mapgeo_ptr->user_coord_sys, fp) )
      {
         iErrorVmapConv("Unable to write user coordinate system");
         return STAT_FILE_WRITE_ERR;
      }
      if( dtcc_write_system( &mapgeo_ptr->user2_coord_sys, fp) )
      {
         iErrorVmapConv("Unable to write user2 coordinate system");
         return STAT_FILE_WRITE_ERR;
      }
*/ }

   temp_ptr = (void *)&mapgeo_ptr->geo_extent;
   if(extent_destruct(fp, false, destruct, ((defEXTENT **)&temp_ptr)) != STAT_SUCCESS)
	  return STAT_DATA_ERR;
   temp_ptr = (void *)&mapgeo_ptr->plot_params;
   if(plot_params_destruct(fp, false, destruct, ((PLOT_PARAMS **)&temp_ptr)) != STAT_SUCCESS)
	  return STAT_DATA_ERR;
   if(destruct)
   {
		if ( mapgeo )
		{
			free((char *)(mapgeo_ptr->internal_to_user));
    			free((char *)(mapgeo_ptr->internal_to_user2));
			free((char *)(mapgeo_ptr->internal_to_display)); 
			free((char *)(mapgeo_ptr->user_to_internal)); 
    			free((char *)(mapgeo_ptr->user2_to_internal)); 
			free((char *)(mapgeo_ptr->display_to_internal)); 
			free((char *)(mapgeo_ptr->data_to_display));
			free((char *)(mapgeo_ptr->display_to_data));
		}
		if(pointer)
		{
		  free((char *)(mapgeo_ptr));
		  *ptr = (MAPGEO *)NULL;
		}
   }
   return STAT_SUCCESS;
}

ERRSTATUS encode_mapgeo(unsigned char *record, MAPGEO *ptr)
{
   char 	 *end_char;
   short	 big_endian, end_check = 1;
   long  	 c = 0L;


   /* Check for endian type */

   end_char = (char *)&end_check;
   big_endian = 1 - (short)(*end_char);

   long_to_char(record, &ptr->magic, big_endian, &c);
   short_to_char(record, (short*)&ptr->use_arc, big_endian, &c);
   short_to_char(record, (short*)&ptr->is_locked, big_endian, &c);
   return STAT_SUCCESS;
}
short dtcc_reset_default_cartesian_system( COORD_SYS* systemP)
{	
	return dtcc_reset_system( systemP,	DEFAULT_XYZ_SYSTEM );   
}	 
short dtcc_reset_default_geodetic_system( COORD_SYS* systemP )
{	
	return dtcc_reset_system( systemP,	DEFAULT_GEO_SYSTEM );   
}	
short dtcc_reset_default_UTM_system( COORD_SYS* systemP	)
{	
	return dtcc_reset_system ( systemP,	DEFAULT_UTM_SYSTEM );   
}
	
short dtcc_reset_default_UPS_system( COORD_SYS* systemP	)
{	
	return dtcc_reset_system ( systemP,	DEFAULT_UPS_SYSTEM );   
}

short dtcc_reset_default_MGRS_system( COORD_SYS* systemP )
{	
	return dtcc_reset_system ( systemP,	DEFAULT_MGS_SYSTEM );   
}

ERRSTATUS setup_transform_struct( const COORD_SYS *in_sys, const COORD_SYS *out_sys, TRANSFORM_SYS *trans_sys )
{
	int       errcode = 0;

    dtcc_clear_transform( trans_sys );
 	dtcc_initialize_transform( trans_sys);
	if( (errcode = dtcc_set_transform_input_system(trans_sys, in_sys)) >=0 )
	    errcode = dtcc_set_transform_output_system( trans_sys, out_sys  );
	return (ERRSTATUS)errcode;
}

/* Modify the system's projection */
/* Change the system's projection to a supported one by name */ 
/* Maintain current values of ancilliary data */
short dtcc_reset_system_projection(	COORD_SYS* csP, const char* gname )
{	
	char name[256];
		short k =  dtcc_reset_projection( _SYSTEM_PROJP(csP), gname, _SYSTEM_ELLIPSOIDP(csP) );
		dtcc_make_system_name( csP, name );
		dtcc_set_system_name( csP, name ); 
		dtcc_set_default_system_extent(csP);
		if( dtcc_is_projection_MGRS(_SYSTEM_PROJP(csP)) )
			dtcc_set_coord_group( _SYSTEM_GROUPP(csP), COORD3_MGRS );
		else
		if( dtcc_is_projection_UTM(_SYSTEM_PROJP(csP)) )
			dtcc_set_coord_group( _SYSTEM_GROUPP(csP), COORD3_UTM );
		else
		if( dtcc_is_projection_UPS(_SYSTEM_PROJP(csP)) )
			dtcc_set_coord_group( _SYSTEM_GROUPP(csP), COORD3_UPS );
		else
		if( dtcc_is_valid(_SYSTEM_PROJP(csP)) )
			dtcc_set_coord_group( _SYSTEM_GROUPP(csP), COORD3_GRID );
	 	else
			dtcc_set_coord_group( _SYSTEM_GROUPP(csP), COORD3_GEO );
		dtcc_set_system_name( csP,NULL );
		return k;
	}

short dtcc_reset_system_hdatum(	COORD_SYS* csP, const char* hname )
	{	if( csP && hname )
 		{	short status = dtcc_reset_datum( _SYSTEM_DATUMP(csP), hname );
			if( status == VMAPOK ) 
			{	dtcc_set_system_name( csP,NULL );
				status = _check_itcoordsys(csP);
			}
			else
				dtcc_set_system_name( csP,NULL );
			return status;
		}
		else
			return ILLEGAL_ARG;
	}

ERRSTATUS mapgeo_adjust( MAPGEO *mapgeo, bool preserve_aspect )
{
    /* reconciles the map geometry according to the preference */
	/* when geo_extents are independent (map_pref == 2-4), they are not checked against
	geo system extents because it was assumed that extents were already validated in the point-entry library.
	System-checking code should be added, since other sources of geo_extents are now possible */
	
    double height = DOUBLE_NULL;
	double image_height_m, image_width_m, map_center_x, map_center_y,
		   scale_recip_lat, save_height_computed;
	double pxmin, pxmax, pymin, pymax, lon_shifted,
		   lon_adjust_factor = 1.0;
	COORD3 geo_min, geo_max, grid_min, grid_max,
		   map_point, temppnt;
	ERRSTATUS errcode1 = STAT_SUCCESS, errcode2 = STAT_SUCCESS;
	bool new_clipped_size = false, /* prev_clipped_size = false, */
	        preserve_pixels = false, shifted = false;

    if (mapgeo == (MAPGEO *) NULL)
	{
		iErrorVmapConv("Invalid map geometry in map adjusting routine.");
	    return STAT_BAD_PARM;
	}
	if( mapgeo->plot_params.image_unit == IMAGE_PIXELS )
	    preserve_pixels = true;
	else
	    preserve_pixels = false;

	/* preserve_aspect is mapgeo_adjust's 2nd arg.  Originally it was a flag for whether to impose equal scaling 
	   in the x and y directions.  Was only false in the call in basemap_match_geometry(), 
	   and when importing a TIFF file (i.e. the original scaling of a raster file was to be retained, not reset.)  
	   With the introduction of ARC, preserve_aspect is sometimes reset from T to false.  The values of preserve_aspect and 
	   use_arc are as follows: 
	   1. GP system with no raster background, or with raster product background for second and subsequent imports 
	      (use_arc==T): preserve_aspect==F.
	   2. GP system with first import of raster product background: 
	      use_arc==T unless DBDB5, DTED, Muse Tiled GeoTIFF, or GeoTIFF ; preserve_aspect==F, map_pref==GEO_AND_IMAGE_EXTENT
	   3. GP system with raster file background: use_arc==F; preserve_aspect==F, map_pref==GEO_AND_IMAGE_EXTENT.
	   4. grid/projection system (no raster background): use_arc==F; preserve_aspect==T */
    /* 2/98--reset preserve_aspect to F for any GP system */
	if( mapgeo->use_arc || dtcc_is_system_geo(&mapgeo->map_display_coord_sys) )
		preserve_aspect = false;

    switch (mapgeo->plot_params.map_pref){
    case CENTER_POINT_SCALE_IMAGE_EXTENT:
    { 
  	  double wlimit, elimit, slimit, nlimit;

	  if( dtcc_is_system_geo(&mapgeo->map_display_coord_sys) &&
		  coord_sys_is_worldwide(&mapgeo->map_display_coord_sys) )
	  {
		  wlimit = -360.0;
		  elimit = 360.0;
	  }
	  else
	  {
		  wlimit = dtcc_coord32(dtcc_system_min_geoP(&mapgeo->map_display_coord_sys));
		  elimit = dtcc_coord32(dtcc_system_max_geoP(&mapgeo->map_display_coord_sys));
	  }

 	  nlimit = dtcc_coord31(dtcc_system_max_geoP(&mapgeo->map_display_coord_sys));
	  slimit = dtcc_coord31(dtcc_system_min_geoP(&mapgeo->map_display_coord_sys));
      image_height_m = mapgeo->plot_params.image_height_pixels / mapgeo->plot_params.pixels_per_meter_y;
      image_width_m = mapgeo->plot_params.image_width_pixels / mapgeo->plot_params.pixels_per_meter_x;
#if 0
	  save_height_computed = mapgeo->plot_params.image_height_pixels_computed;
	  save_width_computed = mapgeo->plot_params.image_width_pixels_computed;
#endif
      if( dtcc_is_system_geo(&mapgeo->map_display_coord_sys) )
	  {
        /* compute geographic extent*/
        double dlat_dd, dlon_dd;

        /* compute lat and lon deltas such that the system limits are not exceeded*/
        /* beware that the image size may be changed from user-entered values */

		/* normalize new map center */
		lon_shifted = dtcc_coord3_lon( &mapgeo->plot_params.map_center );
		geo_360_to_180( &lon_shifted );
		dtcc_set_coord3_lon( &mapgeo->plot_params.map_center, lon_shifted );

        dlat_dd = 0.5 * image_height_m * mapgeo->plot_params.scale_recip / 60.0 / 1852.0;
        if ( dtcc_coord3_lat(&mapgeo->plot_params.map_center) - dlat_dd < slimit)
		{
           dlat_dd = dtcc_coord3_lat(&mapgeo->plot_params.map_center) - slimit;
		}
        if ( dtcc_coord3_lat(&mapgeo->plot_params.map_center) + dlat_dd > nlimit)
		{
           dlat_dd =  nlimit - dtcc_coord3_lat(&mapgeo->plot_params.map_center);
		}
        lon_adjust_factor = get_lon_adjust_factor( mapgeo );
        dlon_dd = 0.5 * image_width_m * mapgeo->plot_params.scale_recip / 
                (60.0  * /*cos(dtcc_coord3_lat(&mapgeo->plot_params.map_center)*DEG2RAD)*/lon_adjust_factor ) / 1852.0;

		if( dlon_dd > 180.0 )
			dlon_dd = 180.0;

        if ( dtcc_coord3_lon(&mapgeo->plot_params.map_center) - dlon_dd  < wlimit)
          dlon_dd = dtcc_coord3_lon(&mapgeo->plot_params.map_center) - wlimit;
        if ( dtcc_coord3_lon(&mapgeo->plot_params.map_center) + dlon_dd > elimit)
          dlon_dd =  elimit - dtcc_coord3_lon(&mapgeo->plot_params.map_center);

        dtcc_set_coord3_geo(&mapgeo->plot_params.map_ll, 
              dtcc_coord3_lat(&mapgeo->plot_params.map_center) - dlat_dd,
              dtcc_coord3_lon(&mapgeo->plot_params.map_center) -  dlon_dd,
              height );
        dtcc_set_coord3_geo(&mapgeo->plot_params.map_ur, 
              dtcc_coord3_lat(&mapgeo->plot_params.map_center) + dlat_dd,
              dtcc_coord3_lon(&mapgeo->plot_params.map_center) + dlon_dd,
              height );

        mapgeo->plot_params.image_height_pixels_computed = 
            mapgeo->plot_params.pixels_per_meter_y * (2 * dlat_dd * 60.0 * 1852 / mapgeo->plot_params.scale_recip);
		/**07/23/97**/
        mapgeo->plot_params.image_width_pixels_computed = 
			mapgeo->plot_params.pixels_per_meter_x * (2 * dlon_dd * 60.0 * lon_adjust_factor * 1852  / mapgeo->plot_params.scale_recip);

        map_to_bitmap_fit( mapgeo,&mapgeo->plot_params.map_ll,&mapgeo->plot_params.map_ur,
			               preserve_aspect, true, preserve_pixels);

        mapgeo->geo_extent.left = dtcc_coord3_lon(&mapgeo->plot_params.map_ll);
        mapgeo->geo_extent.right = dtcc_coord3_lon(&mapgeo->plot_params.map_ur);
        mapgeo->geo_extent.bottom = dtcc_coord3_lat(&mapgeo->plot_params.map_ll);
        mapgeo->geo_extent.top = dtcc_coord3_lat(&mapgeo->plot_params.map_ur);

		/**WR Jan 15 */
		mapgeo->plot_params.lon_shift_factor = 0.0;
		if( mapgeo->geo_extent.left < -180.0)
		{
			mapgeo->plot_params.lon_shift_factor = -360.0;
		}
		else if( mapgeo->geo_extent.right > 180.0)
		{
			mapgeo->plot_params.lon_shift_factor = 360.0;
		}

	  }
	  else /* grid system */
	  {
          /* compute grid extent*/
          double y_dd, x_dd;
	      COORD3 grid_center;

          /* compute y and x deltas such that the system limits are not exceeded*/
          /* beware that the image size may be changed from user-entered values */
 	      if( dtcc_transform( mapgeo->internal_to_display, &mapgeo->plot_params.map_center, &grid_center) < 0 )
		  {
			  iErrorVmapConv("Unable to convert map center to display coordinates.");
			  return STAT_BAD_PARM;
		  }
		  /* get drawing y's from transform y's for UPS-S and ITM */
  		  if( strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Universal Polar Stereographic South 84") ==  0 )
			  dtcc_set_coord3_north( &grid_center, fabs(dtcc_coord3_north(&grid_center)) );
          else if( (strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Irish Transverse Mercator") == 0) && (dtcc_coord3_north(&grid_center) < 0) )
			  dtcc_set_coord3_north( &grid_center, (-1.0 * dtcc_coord3_north(&grid_center) - 1000000.0) );

          y_dd = 0.5 * image_height_m * mapgeo->plot_params.scale_recip;
		  /* Use scale_recip to get first-cut horiz grid delta for checking against system extents */
          x_dd = 0.5 * image_width_m * mapgeo->plot_params.scale_recip;

		  /* get system grid limits for clipping */
          if( geo_to_grid_extent_fit(mapgeo, (COORD3 *)dtcc_system_min_geoP(&mapgeo->map_display_coord_sys), 
			                         (COORD3 *)dtcc_system_max_geoP(&mapgeo->map_display_coord_sys),
									 &grid_min, &grid_max) != STAT_SUCCESS )
		  {
			  iErrorVmapConv("Unable to get coordinate system grid extents");
		      return STAT_BAD_PARM;
          }
		  /* get drawing y's from transform y's for UPS-S and ITM */
   		  if( strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Universal Polar Stereographic South 84") ==  0 )
		  {
			  dtcc_set_coord3_north( &grid_min, fabs(dtcc_coord3_north(&grid_min)) );
			  dtcc_set_coord3_north( &grid_max, fabs(dtcc_coord3_north(&grid_max)) );
          }
          else if( (strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys),"Irish Transverse Mercator") == 0) )
          {
			  if( dtcc_coord3_north(&grid_min) < 0 )
			      dtcc_set_coord3_north( &grid_min, (-1.0 * dtcc_coord3_north(&grid_min) - 1000000.0) );
			  if( dtcc_coord3_north(&grid_max) < 0 )
			      dtcc_set_coord3_north( &grid_max, (-1.0 * dtcc_coord3_north(&grid_max) - 1000000.0) );
		  }

          if ( dtcc_coord3_north(&grid_center) - y_dd < dtcc_coord3_north(&grid_min) )
		  {
              y_dd = dtcc_coord3_north(&grid_center) - dtcc_coord3_north(&grid_min);
		  }
          if ( dtcc_coord3_north(&grid_center) + y_dd > dtcc_coord3_north(&grid_max) )
		  {
              y_dd = dtcc_coord3_north(&grid_max) - dtcc_coord3_north(&grid_center);
		  }

	      if ( dtcc_coord3_east(&grid_center) - x_dd < dtcc_coord3_east(&grid_min) )
		  {
              x_dd = dtcc_coord3_east(&grid_center) - dtcc_coord3_east(&grid_min);
		  }
          if ( dtcc_coord3_east(&grid_center) + x_dd > dtcc_coord3_east(&grid_max) )
		  {
              x_dd = dtcc_coord3_east(&grid_max) - dtcc_coord3_east(&grid_center);
		  }

 		  dtcc_set_coord3_grid( &grid_min, dtcc_coord3_east(&grid_center) - x_dd, 
			                     dtcc_coord3_north(&grid_center) - y_dd, DOUBLE_NULL, 
								 dtcc_system_type(&mapgeo->map_display_coord_sys) );
		  /* translate drawing y back to transform y for grid_to_geo_extent_fit() and
		     transform to internal sys for storage */
  	      if( strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys),"Universal Polar Stereographic South 84") ==  0 )
	           dtcc_set_coord3_north( &grid_min, -1.0 * fabs(dtcc_coord3_north(&grid_min)) );
          else if( (strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys),"Irish Transverse Mercator") ==  0) &&
				( dtcc_coord3_north(&grid_min) < 0) )
	           dtcc_set_coord3_north( &grid_min, -1.0 * (dtcc_coord3_north(&grid_min) + 1000000.0) );
  		  dtcc_set_coord3_grid( &grid_max, dtcc_coord3_east(&grid_center) + x_dd, 
			                     dtcc_coord3_north(&grid_center) + y_dd, DOUBLE_NULL, 
								 dtcc_system_type(&mapgeo->map_display_coord_sys) );
		  /* translate drawing y back to transform y for grid_to_geo_extent_fit() and
		     transform to internal sys for storage */
  	      if( strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys),"Universal Polar Stereographic South 84") ==  0 )
	           dtcc_set_coord3_north( &grid_max, -1.0 * fabs(dtcc_coord3_north(&grid_max)) );
          else if( (strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys),"Irish Transverse Mercator") ==  0) &&
				( dtcc_coord3_north(&grid_max) < 0) )
	           dtcc_set_coord3_north( &grid_max, -1.0 * (dtcc_coord3_north(&grid_max) + 1000000.0) );

          mapgeo->plot_params.image_height_pixels_computed = 
              mapgeo->plot_params.pixels_per_meter_y * (2 * y_dd / mapgeo->plot_params.scale_recip);
		  /* image_width_pixels_computed found in map_to_bitmap_fit: */

          map_to_bitmap_fit( mapgeo, &grid_min, &grid_max, preserve_aspect, true, preserve_pixels);

		  x_dd = 0.5 * (mapgeo->plot_params.image_width_pixels_computed / mapgeo->plot_params.sx);
          dtcc_set_coord3_east( &grid_max, (dtcc_coord3_east(&grid_center) + x_dd) );
          dtcc_set_coord3_east( &grid_min, (dtcc_coord3_east(&grid_center) - x_dd) );

	      if( dtcc_transform(mapgeo->display_to_internal, &grid_min, &temppnt) < 0 )
          {
			  iErrorVmapConv("Warning: Unable to convert new lower left corner to DD coordinates.");
			  dtcc_set_coord3_geo( &temppnt, DOUBLE_NULL, DOUBLE_NULL, DOUBLE_NULL );
		  }
		  dtcc_copy_coord3( &mapgeo->plot_params.map_ll, &temppnt );

	      if( dtcc_transform(mapgeo->display_to_internal, &grid_max, &temppnt)  < 0 )
          {
			  iErrorVmapConv("Warning: Unable to convert new upper right corner to DD coordinates.");
			  dtcc_set_coord3_geo( &mapgeo->plot_params.map_ur, DOUBLE_NULL, DOUBLE_NULL, DOUBLE_NULL );
		  }
	      dtcc_copy_coord3( &mapgeo->plot_params.map_ur, &temppnt );

		  if( grid_to_geo_extent_fit(mapgeo, &grid_min, &grid_max, &geo_min, &geo_max) != STAT_SUCCESS )
          {
			  iErrorVmapConv("Unable to get new geo extents.");
              return STAT_BAD_PARM;
          }
		  if( dtcc_coord3_lon(&geo_max) > elimit )
			  dtcc_set_coord3_lon(&geo_max, elimit);
		  if( dtcc_coord3_lat(&geo_max) > nlimit )
			  dtcc_set_coord3_lat(&geo_max, nlimit);
		  if( dtcc_coord3_lon(&geo_min) < wlimit )
			  dtcc_set_coord3_lon(&geo_min, wlimit);
		  if( dtcc_coord3_lat(&geo_min) < slimit )
			  dtcc_set_coord3_lat(&geo_min, slimit);
		  
  		  mapgeo->geo_extent.right = dtcc_coord3_lon(&geo_max);
		  mapgeo->geo_extent.left = dtcc_coord3_lon(&geo_min);
		  mapgeo->geo_extent.top =  dtcc_coord3_lat(&geo_max);
		  mapgeo->geo_extent.bottom = dtcc_coord3_lat(&geo_min);

	  }
     break;
    }
    case GEO_EXTENT_SCALE: 
    {

	  if( dtcc_is_system_grid(&mapgeo->map_display_coord_sys) )
      {

	      if( ((dtcc_coord3_lat(&mapgeo->plot_params.map_ll)-DOUBLE_NULL) < ANIOTA) ||
		      ((dtcc_coord3_lon(&mapgeo->plot_params.map_ll)-DOUBLE_NULL) < ANIOTA) )
		  {
			iErrorVmapConv("Corners are not valid values.");
			return STAT_BAD_PARM;
		  }
	      if( ((dtcc_coord3_lat(&mapgeo->plot_params.map_ur)-DOUBLE_NULL) < ANIOTA) ||
		      ((dtcc_coord3_lon(&mapgeo->plot_params.map_ur)-DOUBLE_NULL) < ANIOTA) )
		  {
			iErrorVmapConv("Corners are not valid values.");
			return STAT_BAD_PARM;
		  }
          if( dtcc_transform(mapgeo->internal_to_display, (const void *)&mapgeo->plot_params.map_ll, 
			(void *)&grid_min) < 0 )
		  {
		    iErrorVmapConv("Unable to convert lower left map corner to display system coordinates.");
			return STAT_BAD_PARM;
		  }
          if( dtcc_transform(mapgeo->internal_to_display, (const void *)&mapgeo->plot_params.map_ur, 
			(void *)&grid_max) < 0 )
		  {
		    iErrorVmapConv("Unable to convert upper right map corner to display system coordinates.");
			return STAT_BAD_PARM;
		  }
		  /* get new geo extents */
	      if( grid_to_geo_extent_fit( mapgeo, &grid_min, &grid_max, &geo_min, &geo_max) != STAT_SUCCESS )
		  {
			  iErrorVmapConv("Unable to get new geo extents from map corners.");
	          return STAT_BAD_PARM;
          }
		  pxmin = dtcc_coord3_east(&grid_min);
		  pxmax = dtcc_coord3_east(&grid_max);
		  pymin = dtcc_coord3_north(&grid_min);
		  pymax = dtcc_coord3_north(&grid_max);
		  /* find "drawing" y's for ITM and UPS-S  */
          if( strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Universal Polar Stereographic South 84") ==  0 )
          {
	          pymin = fabs(pymin);
 	          pymax = fabs(pymax);
          }
          else if( (strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Irish Transverse Mercator") == 0) )
	      {
		      if( pymin < 0 )
		        pymin  = -pymin - 1000000.0;
		      if( pymax < 0 )
		        pymax  = -pymax - 1000000.0;
          }

           /* compute center */
	        map_center_x = (pxmax + pxmin) / 2.0;
	        map_center_y = (pymax + pymin) / 2.0;
	        dtcc_set_coord3_grid( &map_point, map_center_x, map_center_y, DOUBLE_NULL,
		                    dtcc_system_type(&mapgeo->map_display_coord_sys));

            if( dtcc_transform(mapgeo->display_to_internal, &map_point, &temppnt) < 0 )
		    {
		      iErrorVmapConv("Unable to find geo coordinates for new map center");
			  return STAT_BAD_PARM;
		    }
            dtcc_copy_coord3(&mapgeo->plot_params.map_center, &temppnt);
			
		    /* compute image size */
            mapgeo->plot_params.image_height_pixels = (pymax - pymin) 
                     / mapgeo->plot_params.scale_recip
                     * mapgeo->plot_params.pixels_per_meter_y;

			/* get geo extents */
		    mapgeo->geo_extent.right = dtcc_coord3_lon(&geo_max);
		    mapgeo->geo_extent.left = dtcc_coord3_lon(&geo_min);
		    mapgeo->geo_extent.top =  dtcc_coord3_lat(&geo_max);
		    mapgeo->geo_extent.bottom = dtcc_coord3_lat(&geo_min);

	        mapgeo->plot_params.image_height_pixels_computed = mapgeo->plot_params.image_height_pixels;
	        /* compute scale and translation for map-to-bitmap conversion */
            map_to_bitmap_fit( mapgeo, &grid_min, &grid_max, preserve_aspect, true, false);
            mapgeo->plot_params.image_width_pixels = mapgeo->plot_params.image_width_pixels_computed;
	  }		 /* end grid system */

	  else	 /* GP system */	   
	  {
		  pxmin = dtcc_coord3_lon(&mapgeo->plot_params.map_ll);
		  pxmax = dtcc_coord3_lon(&mapgeo->plot_params.map_ur);
		  pymin = dtcc_coord3_lat(&mapgeo->plot_params.map_ll);
		  pymax = dtcc_coord3_lat(&mapgeo->plot_params.map_ur);

          mapgeo->plot_params.image_height_pixels =  /* delta lat in degrees: */
                     ( pymax - pymin )
                     * 60.0            /* NM per degree latitude */
                     * 1852.0          /* M per NM */ 
                     / mapgeo->plot_params.scale_recip
                     * mapgeo->plot_params.pixels_per_meter_y;

          /* compute center */
          dtcc_set_coord3_geo(&mapgeo->plot_params.map_center, 
              (dtcc_coord3_lat(&mapgeo->plot_params.map_ll) + dtcc_coord3_lat(&mapgeo->plot_params.map_ur) ) / 2.0,
              (dtcc_coord3_lon(&mapgeo->plot_params.map_ll) + dtcc_coord3_lon(&mapgeo->plot_params.map_ur) ) / 2.0,
              DOUBLE_NULL );

         lon_adjust_factor = get_lon_adjust_factor( mapgeo );
         mapgeo->plot_params.image_width_pixels =  /* delta lon in degrees: */
                     ( pxmax - pxmin )
                     * 60.0 *    lon_adjust_factor   /* NM per degree latitude */
                     * 1852.0          /* M per NM */ 
                     / mapgeo->plot_params.scale_recip
                     * mapgeo->plot_params.pixels_per_meter_x;

	      mapgeo->plot_params.image_height_pixels_computed = mapgeo->plot_params.image_height_pixels;
		  mapgeo->plot_params.image_width_pixels_computed = mapgeo->plot_params.image_width_pixels;

          mapgeo->geo_extent.left = dtcc_coord3_lon(&mapgeo->plot_params.map_ll);
          mapgeo->geo_extent.right = dtcc_coord3_lon(&mapgeo->plot_params.map_ur);
          mapgeo->geo_extent.bottom = dtcc_coord3_lat(&mapgeo->plot_params.map_ll);
          mapgeo->geo_extent.top = dtcc_coord3_lat(&mapgeo->plot_params.map_ur);

	      /* compute scale and translation for map-to-bitmap conversion, and possibly adjust image width  */
          map_to_bitmap_fit( mapgeo, &mapgeo->plot_params.map_ll, 
			                 &mapgeo->plot_params.map_ur, preserve_aspect, true, false);
          mapgeo->plot_params.image_width_pixels = mapgeo->plot_params.image_width_pixels_computed;
	  }	 /* end Pref II / GP system */

    break;
    }
    case GEO_AND_IMAGE_EXTENT:
    {

	  if( dtcc_is_system_geo(&mapgeo->map_display_coord_sys) )
      {
		int index = 0;

        /* compute center */
        dtcc_set_coord3_geo(&mapgeo->plot_params.map_center, 
          ( mapgeo->geo_extent.bottom + mapgeo->geo_extent.top ) / 2.0,
          ( mapgeo->geo_extent.left + mapgeo->geo_extent.right ) / 2.0,
          height );

        dtcc_set_coord3_lon(&mapgeo->plot_params.map_ll, mapgeo->geo_extent.left);
        dtcc_set_coord3_lon(&mapgeo->plot_params.map_ur, mapgeo->geo_extent.right);
        dtcc_set_coord3_lat(&mapgeo->plot_params.map_ll, mapgeo->geo_extent.bottom);
        dtcc_set_coord3_lat(&mapgeo->plot_params.map_ur, mapgeo->geo_extent.top);
        
        /* compute preliminary scale, used to get ARC adjustment */ 
        image_height_m = mapgeo->plot_params.image_height_pixels_computed / mapgeo->plot_params.pixels_per_meter_y;
        image_width_m = mapgeo->plot_params.image_width_pixels_computed / mapgeo->plot_params.pixels_per_meter_x;
	    mapgeo->plot_params.scale_recip = 
          ( mapgeo->geo_extent.top - mapgeo->geo_extent.bottom )  
                     * 60.0            /* NM per degree latitude */
                     * 1852.0            /* M per NM */ 
                     / image_height_m; 
        save_height_computed = 0.0;

        while( (fabs(mapgeo->plot_params.image_height_pixels_computed - save_height_computed) > 0.5 ) 
			    && (index < 5) )
        {
			index++;
			save_height_computed = mapgeo->plot_params.image_height_pixels_computed;
			map_to_bitmap_fit( mapgeo, &mapgeo->plot_params.map_ll,
							   &mapgeo->plot_params.map_ur, preserve_aspect, false, preserve_pixels );

	    }
		/* recompute scale, using any changes that may have been made to image size in fit() */
		image_height_m = mapgeo->plot_params.image_height_pixels_computed / mapgeo->plot_params.pixels_per_meter_y;
		image_width_m = mapgeo->plot_params.image_width_pixels_computed / mapgeo->plot_params.pixels_per_meter_x;
		mapgeo->plot_params.scale_recip = 
		    ( mapgeo->geo_extent.top - mapgeo->geo_extent.bottom )  
					 * 60.0            /* NM per degree latitude */
					 * 1852.0            /* M per NM */ 
					 / image_height_m; 
	  }
	  else /* grid system */
	  {
          dtcc_set_coord3_lon(&geo_min, mapgeo->geo_extent.left);
          dtcc_set_coord3_lon(&geo_max, mapgeo->geo_extent.right);
          dtcc_set_coord3_lat(&geo_min, mapgeo->geo_extent.bottom);
          dtcc_set_coord3_lat(&geo_max, mapgeo->geo_extent.top);
          if( geo_to_grid_extent_fit(mapgeo, &geo_min, &geo_max, &grid_min, &grid_max) != STAT_SUCCESS )
		  {
			  iErrorVmapConv("Unable to find map corners from geo extents.");
		      return STAT_BAD_PARM;
		  }
		  pxmin = dtcc_coord3_east(&grid_min);
		  pxmax = dtcc_coord3_east(&grid_max);
		  pymin = dtcc_coord3_north(&grid_min);
		  pymax = dtcc_coord3_north(&grid_max);
          if( strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Universal Polar Stereographic South 84") ==  0 )
          {
	          pymin = fabs(pymin);
 	          pymax = fabs(pymax);
          }
          else if( (strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Irish Transverse Mercator") == 0) )
	      {
		      if( pymin < 0 )
		        pymin  = -pymin - 1000000.0;
		      if( pymax < 0 )
		        pymax  = -pymax - 1000000.0;
          }

          /* compute center */
	      map_center_x = (pxmax + pxmin) / 2.0;
	      map_center_y = (pymax + pymin) / 2.0;
	      dtcc_set_coord3_grid( &map_point, map_center_x, map_center_y, DOUBLE_NULL,
		                    dtcc_system_type(&mapgeo->map_display_coord_sys));
          if( dtcc_transform(mapgeo->display_to_internal, &map_point, &temppnt) < 0 )
		  {
		      iErrorVmapConv("Unable to find geo coordinates for new map center");
			  return STAT_BAD_PARM;
		  }
          dtcc_copy_coord3(&mapgeo->plot_params.map_center, &temppnt);
		  
 		  map_to_bitmap_fit( mapgeo, &grid_min, &grid_max, preserve_aspect, false, preserve_pixels );

	      /* get scale, using any changes that may have been made to image size in fit() */
          image_height_m = mapgeo->plot_params.image_height_pixels_computed / mapgeo->plot_params.pixels_per_meter_y;
	      mapgeo->plot_params.scale_recip = (pymax - pymin) / image_height_m;

          if( dtcc_transform(mapgeo->display_to_internal, &grid_min, &temppnt) < 0 )
		  {
			  iErrorVmapConv("Warning: Unable to convert new lower left corner to DD coordinates.");
			  dtcc_set_coord3_geo( &mapgeo->plot_params.map_ll, DOUBLE_NULL, DOUBLE_NULL, DOUBLE_NULL );
		  }
          dtcc_copy_coord3(&mapgeo->plot_params.map_ll, &temppnt); 
          if( dtcc_transform(mapgeo->display_to_internal, &grid_max, &temppnt) < 0 )
		  {
			  iErrorVmapConv("Warning: Unable to convert new upper right corner to DD coordinates.");
			  dtcc_set_coord3_geo( &mapgeo->plot_params.map_ur, DOUBLE_NULL, DOUBLE_NULL, DOUBLE_NULL );
		  }
          dtcc_copy_coord3(&mapgeo->plot_params.map_ur, &temppnt); 
	  }

     break;
    }
	case MAP_AND_IMAGE_EXTENT:
	{/* non-gui map preference for sweep area: calculate center and scale from corners and image size */
      image_height_m = mapgeo->plot_params.image_height_pixels / mapgeo->plot_params.pixels_per_meter_y;
  
 	  if( dtcc_is_system_grid(&mapgeo->map_display_coord_sys) )
      {

  	      if( ((dtcc_coord3_lat(&mapgeo->plot_params.map_ll)-DOUBLE_NULL) < ANIOTA) ||
		      ((dtcc_coord3_lon(&mapgeo->plot_params.map_ll)-DOUBLE_NULL) < ANIOTA) )
		  {
			iErrorVmapConv("Corners are not valid values.");
			return STAT_BAD_PARM;
		  }
	      if( ((dtcc_coord3_lat(&mapgeo->plot_params.map_ur)-DOUBLE_NULL) < ANIOTA) ||
		      ((dtcc_coord3_lon(&mapgeo->plot_params.map_ur)-DOUBLE_NULL) < ANIOTA) )
		  {
			iErrorVmapConv("Corners are not valid values.");
			return STAT_BAD_PARM;
		  }
          if( dtcc_transform(mapgeo->internal_to_display, (const void *)&mapgeo->plot_params.map_ll, 
			(void *)&grid_min) < 0 )
		  {
		    iErrorVmapConv("Unable to convert lower left map corner to display system coordinates.");
			return STAT_BAD_PARM;
		  }
          if( dtcc_transform(mapgeo->internal_to_display, (const void *)&mapgeo->plot_params.map_ur, 
			(void *)&grid_max) < 0 )
		  {
		    iErrorVmapConv("Unable to convert upper right map corner to display system coordinates.");
			return STAT_BAD_PARM;
		  }

		  pxmin = dtcc_coord3_east(&grid_min);
		  pxmax = dtcc_coord3_east(&grid_max);
		  pymin = dtcc_coord3_north(&grid_min);
		  pymax = dtcc_coord3_north(&grid_max);
          if( strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Universal Polar Stereographic South 84") ==  0 )
          {
	          pymin = fabs(pymin);
 	          pymax = fabs(pymax);
          }
          else if( (strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Irish Transverse Mercator") == 0) )
	      {
		      if( pymin < 0 )
		        pymin  = -pymin - 1000000.0;
		      if( pymax < 0 )
		        pymax  = -pymax - 1000000.0;
          }

	      /* reset geo extents for grid system by sampling across grid limits */ 
		   if( grid_to_geo_extent_fit(mapgeo, &grid_min, &grid_max, &geo_min, &geo_max) != STAT_SUCCESS )
		   {
			   iErrorVmapConv("Unable to get new geo extents.");
               return  STAT_BAD_PARM;
		   }
            /* compute center */
	        map_center_x = (pxmax + pxmin) / 2.0;
	        map_center_y = (pymax + pymin) / 2.0;
	        dtcc_set_coord3_grid( &map_point, map_center_x, map_center_y, DOUBLE_NULL,
		                    dtcc_system_type(&mapgeo->map_display_coord_sys));
            if( dtcc_transform(mapgeo->display_to_internal, &map_point, &temppnt) < 0 )
		    {
		      iErrorVmapConv("Unable to get DD value for new map center.");
			  return STAT_BAD_PARM;
		    }
            dtcc_copy_coord3(&mapgeo->plot_params.map_center, &temppnt);
		  
		     mapgeo->geo_extent.right = dtcc_coord3_lon(&geo_max);
		     mapgeo->geo_extent.left = dtcc_coord3_lon(&geo_min);
		     mapgeo->geo_extent.top =  dtcc_coord3_lat(&geo_max);
		     mapgeo->geo_extent.bottom = dtcc_coord3_lat(&geo_min);
	        /* compute screen scales and translations for map-to-bitmap conversion */
            map_to_bitmap_fit( mapgeo, &grid_min, &grid_max, preserve_aspect, false, preserve_pixels);

	        /* compute scale, using any changes that may have been made to image size in map_to_bitmap_fit() */
            image_height_m = mapgeo->plot_params.image_height_pixels_computed / mapgeo->plot_params.pixels_per_meter_y;
            scale_recip_lat = ( pymax - pymin ) / image_height_m;
            mapgeo->plot_params.scale_recip = scale_recip_lat;
	  }
	  else	/* geo system */
	  {
        /* compute center */
         dtcc_set_coord3_geo(&mapgeo->plot_params.map_center, 
              (dtcc_coord3_lat(&mapgeo->plot_params.map_ll) + dtcc_coord3_lat(&mapgeo->plot_params.map_ur) ) / 2.0,
              (dtcc_coord3_lon(&mapgeo->plot_params.map_ll) + dtcc_coord3_lon(&mapgeo->plot_params.map_ur) ) / 2.0,
              DOUBLE_NULL );

  	    /* compute screen scales and translations for map-to-bitmap conversion */
        map_to_bitmap_fit( mapgeo, &mapgeo->plot_params.map_ll, 
			               &mapgeo->plot_params.map_ur, preserve_aspect, false, preserve_pixels);

	    /* compute scale, using any changes that may have been made to image size in map_to_bitmap_fit() */
        image_height_m = mapgeo->plot_params.image_height_pixels_computed / mapgeo->plot_params.pixels_per_meter_y;
        image_width_m = mapgeo->plot_params.image_width_pixels_computed / mapgeo->plot_params.pixels_per_meter_x;
        scale_recip_lat = 
          ( dtcc_coord3_lat(&mapgeo->plot_params.map_ur) - dtcc_coord3_lat(&mapgeo->plot_params.map_ll) )  
                     * 60.0            /* NM per degree latitude */
                     * 1852.0            /* M per NM */ 
                     / image_height_m;
        mapgeo->plot_params.scale_recip = scale_recip_lat;

	    /* set geo extents to corner values */ 
        mapgeo->geo_extent.left = dtcc_coord3_lon(&mapgeo->plot_params.map_ll);
        mapgeo->geo_extent.right = dtcc_coord3_lon(&mapgeo->plot_params.map_ur);
        mapgeo->geo_extent.bottom = dtcc_coord3_lat(&mapgeo->plot_params.map_ll);
        mapgeo->geo_extent.top = dtcc_coord3_lat(&mapgeo->plot_params.map_ur);
     }
	 break;
	}

 	default :
	{ 
	  iErrorVmapConv("Invalid map preference in mapgeo_adjust()");
      return STAT_BAD_PARM;
	  break;
	}

  }

  return STAT_SUCCESS;
}

short _reset_system( COORD_SYS* csP, const char* name, short type )
	{	DTCC_LABELS id;
		if( dtcc_decode_label( name, &id ) == NULL )
			return ILLEGAL_ARG;
		else
		{	const COORD_SYS* fndP = _find_system( &id,type,1);
			if( fndP )
			{	short status = _update_system( csP, dtcc_system_labelP(fndP), dtcc_system_groupP(fndP), 
						   dtcc_system_datumP(fndP), dtcc_system_projectionP(fndP), 
						   dtcc_system_extentsP(fndP), dtcc_system_unitsP(fndP),
						   dtcc_system_dimension(fndP), dtcc_type_flag(fndP) );
				return status;
			}
			else
				return ITEM_NOT_FOUND;
		}
	}

short dtcc_clear_transform( TRANSFORM_SYS* tsP )
	{	dtcc_clear_system(_INPUTP(tsP));
		dtcc_clear_system(_OUTPUTP(tsP));
		return VMAPOK;  
	}	

short dtcc_initialize_transform( TRANSFORM_SYS* tsP )
	{	dtcc_initialize_system(_INPUTP(tsP));
		dtcc_initialize_system(_OUTPUTP(tsP));
		tsP->valid_input = tsP->valid = tsP->valid_output = 0;
		return VMAPOK;  
	}

short _set_transform_system( TRANSFORM_SYS* tsP, int k, const COORD_SYS* sysP )
	{	short status = 0;
		dtcc_clear_system( _SYSTEMKP(tsP,k) );
		dtcc_copy_system( _SYSTEMKP(tsP,k),sysP );
		if( k == 0 )
			status = tsP->valid_input  = 
				(short)( dtcc_validate_system(_INPUTP(tsP))== VMAPOK ? 1 : 0 );
		else
			status = tsP->valid_output  = 
				(short)( dtcc_validate_system(_OUTPUTP(tsP))== VMAPOK ? 1 : 0 );
		
		if( tsP->valid_input && tsP->valid_output )
		{	COORD_SYS *inP = _INPUTP(tsP), *outP = _OUTPUTP(tsP);
			tsP->valid = 1;
			if( dtcc_is_valid( dtcc_system_datumP(inP)  ) && 
				dtcc_is_valid( dtcc_system_datumP(outP) ) &&
				( dtcc_compare_vdatums( dtcc_system_datumP(inP),dtcc_system_datumP(outP) ) != 0 ||
				dtcc_compare_datums( dtcc_system_datumP(inP),dtcc_system_datumP(outP) ) != 0 ))
			{	tsP->dshift = 1;
				_init_molo_parameters(	dtcc_system_axis(inP), dtcc_system_rflat(inP),
					dtcc_system_hdatum_shiftsP(inP),dtcc_system_hdatum_sigmasP(inP),
					dtcc_system_axis(outP), dtcc_system_rflat(outP),
					dtcc_system_hdatum_shiftsP(outP),dtcc_system_hdatum_sigmasP(outP),
					(TRANSFORM_PARAMS*)tsP->mpars );
				if(dtcc_compare_vdatums( dtcc_system_datumP(inP),dtcc_system_datumP(outP) ) != 0)
				{	if( _DATUM_VFILEP(dtcc_system_datumP(inP)) == NULL )
						_DATUM_VFILEP(_SYSTEM_DATUMP(inP)) = 
							fopen( dtcc_default_geoid_file,"r" );
					if( _DATUM_VFILEP(dtcc_system_datumP(outP)) == NULL )
						_DATUM_VFILEP(_SYSTEM_DATUMP(outP)) = 
							fopen( dtcc_default_geoid_file,"r" );
					
				}
			}
			else
				tsP->dshift = 0;
		}
		return (short)( status  ?  VMAPOK : INVALID_STRUCTURE ); 
	}

short dtcc_reset_projection( PROJ_PARAMS* ppP, const char* pname, const ELLIPS_PARAMS* ellipsoid )
    {   if( ppP )
        {   short k = _reset( ppP, pname, ellipsoid );
        	return 	k;
        } 
        else
            return ILLEGAL_ARG;
    }

short dtcc_make_system_name( COORD_SYS* csP, char* nm )
	{	if( csP && nm  )
		{	const int dim = dtcc_system_dimension(csP);
	    	const char* hab = dtcc_abbrvP(dtcc_system_datumP(csP));
	    	const char* vab = dtcc_system_vdatum_nameP(csP);
			int isgeo = dtcc_is_system_geo(csP), iscart = dtcc_is_system_cart(csP);
			const char* pnm = ( isgeo || iscart ? "" : dtcc_abbrvP(_SYSTEM_PROJP(csP)));
	    	char gab[12];
			long hsum;
			const char* pab = ( isgeo || iscart ? gab : dtcc_abbrvP(dtcc_system_projectionP(csP)));
			if(  isgeo || iscart )
			{	dtcc_encode_coord_group( dtcc_system_groupP(csP), gab );
				hsum = 0;
			}
			else
			{	hsum = hash( _PROJECTION_ORIGINP(_SYSTEM_PROJP(csP)),sizeof(COORD_ORIGIN));
				hsum += hash( _PROJECTION_STDPARSP(_SYSTEM_PROJP(csP)),2*sizeof(double));
			}
	    	/* const char* eab = dtcc_abbrvP(dtcc_system_ellipsoidP(csP)); */
			sprintf( nm, /* "%s_%s_%s_%s_%s_%d_%ld", hab, vab, pab, eab,gab,dim,hsum );	*/
						"%s_%s_%s_%s_%d_%ld", hab, pab, vab,pnm, dim,hsum );
	    	return 0;
		}
		else
			return -1;
	}

short dtcc_set_system_name(	COORD_SYS* csP, const char* name )
	{	short status = VMAPOK;
		DTCC_LABELS id; const COORD_SYS* fndP;
		char dfname[256];
		if( !name ) 
		{	dtcc_make_system_name(csP,dfname);
			name = dfname;
		}	
		dtcc_decode_label( name, &id );
		fndP = _find_system( &id, _ENTIRE_LIST,1 );
		if( fndP )	status = DUPLICATE_NAME;
		dtcc_copy_label( _SYSTEM_LABELP(csP),&id );
		dtcc_set_invalid(csP);
		if( status != DUPLICATE_NAME )
			dtcc_set_type( csP,_TYPE2_LIST );
		else
			dtcc_set_type( csP, dtcc_type_flag(fndP) );
		return status;
	}

short dtcc_set_default_system_extent( COORD_SYS* csP )
	{	if( csP )
		{	COORD3 mnmx[2];
			if( dtcc_is_system_geographic(csP) )
			{	dtcc_set_coord3_geo( mnmx,  -90.0, -180.0, DOUBLE_NULL );
				dtcc_set_coord3_geo( mnmx+1, 90.0,  180.0, DOUBLE_NULL );
			}
			else
				dtcc_projection_intrinsic_limits( _SYSTEM_PROJP(csP), mnmx ); 
			return dtcc_reset_system_extents( csP, NULL,NULL, NULL, NULL );
		}
		else
			return ILLEGAL_ARG;
	}

short dtcc_is_projection_MGRS(	const PROJ_PARAMS* ppP	)
	{	if( strncmp( dtcc_nameP(ppP),"Military Grid Reference System",15 ) == 0 )
			return 1;
		else
			return 0;
	}

short dtcc_set_coord_group(	COORD_GROUP* cgP,COORD3_TYPES id )
    {	if( !cgP )
    		return ILLEGAL_ARG;
    	else
    	if( dtcc_coord3_is_defined(id) )
    	{	cgP->id = id;
    		_set_valid(cgP);
    		return VMAPOK;
    	}
    	else
    	{	_set_invalid(cgP);
    		return INDEX_OUT_OF_RANGE;
    	}
    }

short dtcc_is_projection_UTM(	const PROJ_PARAMS* ppP	)
	{	if( strncmp( dtcc_nameP(ppP), "Universal Transverse Mercator",20 ) == 0 )
			return 1;
		else
			return 0;
	}

short dtcc_is_projection_UPS( const PROJ_PARAMS* ppP )
	{	if( strncmp( dtcc_nameP(ppP), "Universal Polar Stereographic", 15 ) == 0 )
			return 1;
		else
			return 0;
	}

short _reset_datum( DATUM_PARAMS* dpP, const char* name, int type )
	{	DTCC_LABELS id;
		if( dtcc_decode_label( name, &id ) == NULL )
			return ILLEGAL_ARG;
		else
		{	const DATUM_PARAMS* fndP;
			if( dlist == NULL ) dtcc_initialize_datum_list(NULL);
			fndP = _find_datum( &id,type);
			if( fndP )
				return _update_datum( dpP, dtcc_nameP(fndP), dtcc_otherP(fndP), _DATUM_VNAMEP(fndP),
							dtcc_abbrvP(fndP), _DATUM_SHIFTSP(fndP), _DATUM_SIGMASP(fndP), 
							_DATUM_ELLIPSOIDP(fndP), dtcc_type_flag(fndP) );
			else
				return ITEM_NOT_FOUND;
		}
	}

short _check_itcoordsys( COORD_SYS* cpP )
	{	if( !cpP )
			return ILLEGAL_ARG;
		else
		{	short status,status1,status2;
			
			/* Check the units */
			status = dtcc_validate_coord3_units( _SYSTEM_UNITSP(cpP) );
			if( status < 0 ) return status;
		
			if( dtcc_is_coord_group_cart( _SYSTEM_GROUPP(cpP)) )
				return VMAPOK;
				
			/* Check the datums */
			status1 = dtcc_validate_datum( _SYSTEM_DATUMP(cpP) );
			/* Check the grid */
			status2 = dtcc_validate_projection( _SYSTEM_PROJP(cpP) );
			if( status2 < 0 && status1 < 0 ) return status1;
		
			/* Check the Extents */
			status = dtcc_validate_coord3_extents( _SYSTEM_EXTENTSP(cpP) );
			if( status < 0 ) return status;
		
			/* If projection & datum ellipsoids don't match, use datum's */
			_check_ellipsoids( cpP );
					  
			/* Rename the system, if necessary */
			if( !dtcc_is_valid_name(dtcc_nameP(cpP) ) )
			{	char nm[256];
				dtcc_make_system_name( cpP,nm );
				dtcc_set_system_name( cpP, nm );
			}
			dtcc_set_valid(cpP);
			return VMAPOK;
		}  
	}

bool coord_sys_is_worldwide( const COORD_SYS *csP )
{
	if( ((dtcc_system_max_lat(csP)-90.0) < ANIOTA ) &&
		((dtcc_system_min_lat(csP)+90.0) < ANIOTA ) &&
		((dtcc_system_min_lon(csP)+180.0) < ANIOTA ) &&
		((dtcc_system_max_lon(csP)-180.0) < ANIOTA ) )
       return 1;
	else 
	   return 0;
}

short dtcc_set_coord3K( COORD3* c3P, short k, double v )
	{	if( !c3P )
			return ILLEGAL_ARG;
		else
		if(  k < 0 || k > 2 )
			return -2;
		else
		{	_COORD3K(c3P,k) = v;	
			return VMAPOK;
		}
	}

double get_lon_adjust_factor( MAPGEO *mapgeo )
{
	/* uses code from rast_imp\adrg\a_gtimg.c's aGetIMG() */
	double lon_adjust_factor = 1.0,
		   map_center_lat = 0.0;

	static double  zone_arv[] = { 369664.E0, 302592.E0, 245760.E0,
								199168.E0, 163328.E0, 137216.E0,
								110080.E0,  82432.E0, 400384.E0 };
	double  arv, brv, N = 1.E0;
    long    arc_zone  = 0L;


	if( mapgeo->use_arc )
	{
	    N = 1000000.E0 / mapgeo->plot_params.scale_recip;
	    adrg_zone_compute(dtcc_coord3_lat(&mapgeo->plot_params.map_center),&arc_zone);

		/* are not currently supporting Zones 9 and 18: */
		if( arc_zone==9 || arc_zone==18 )
			arc_zone--;

	    arv = dtcc_round(zone_arv[((arc_zone - 1L) % 9L)] * N);
	    if(((long)arv % 512L) != 0L)
		    arv += 512L - (long)arv % 512L;

	    brv  = dtcc_round(400384.E0 * N);
	    if(((long)brv % 512L) != 0)
		    brv += 512L - (long)brv % 512L;
	    lon_adjust_factor = arv/brv;
	}
	return lon_adjust_factor;
}

void map_to_bitmap_fit( MAPGEO *mapgeo, COORD3 *map_min, COORD3 *map_max, bool preserve_aspect, bool preserve_vert_scale, bool preserve_pixel_extent)
{
	double pixasp, minX, maxX, minY, maxY,
		   pxmin, pxmax, pymin, pymax,
		   sx = 1.0, sy = 1.0, tx = 0.0, ty = 0.0,
		   lon_adjust_factor = 1.0;

    minX = 0.0;
    minY = 0.0;

#if 0 /*****TEMP? */
	if( preserve_pixel_extent ) 
	    pixasp = 1.0;
    else 
#endif
	    pixasp = mapgeo->plot_params.pixels_per_meter_x / mapgeo->plot_params.pixels_per_meter_y;

 	if( dtcc_is_system_grid(&mapgeo->map_display_coord_sys) )
    {
	    pxmin = dtcc_coord3_east(map_min);
	    pxmax = dtcc_coord3_east(map_max);
	    pymin = dtcc_coord3_north(map_min);
	    pymax = dtcc_coord3_north(map_max);
	    /* map_min and map_max contain "dtcc_transform" y values; i.e. grid y's used 
		for input and output by d_t() that are flagged with minus signs and/or false origin values.  
		These must be translated to "drawing" y's */
        if( strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Universal Polar Stereographic South 84") ==  0 )
        {
	         pymin = fabs(pymin);
 	         pymax = fabs(pymax);
        }
        else if( (strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Irish Transverse Mercator") == 0) )
	    {
		     if( pymin < 0 )
		       pymin  = -pymin - 1000000.0;
		     if( pymax < 0 )
		       pymax  = -pymax - 1000000.0;
        }
    }
	else
	{
	    pxmin = dtcc_coord3_lon(map_min);
	    pxmax = dtcc_coord3_lon(map_max);
	    pymin = dtcc_coord3_lat(map_min);
	    pymax = dtcc_coord3_lat(map_max);
	}

	if( preserve_vert_scale )
	{   /* average vertical scale must be retained; Map Pref I and II */
        maxY = mapgeo->plot_params.image_height_pixels_computed;
        sy = (maxY - minY) / (pymax - pymin);
		if( preserve_aspect)
		{
	       sx = sy * pixasp;
           maxX = sx * (pxmax - pxmin);
		   if( (maxX > mapgeo->plot_params.image_width_pixels) && 
			   (mapgeo->plot_params.map_pref == CENTER_POINT_SCALE_IMAGE_EXTENT) )       
		   {
			   maxX = mapgeo->plot_params.image_width_pixels;
		   }
           mapgeo->plot_params.image_width_pixels_computed = maxX;
		}
		else
		{
           maxX = mapgeo->plot_params.image_width_pixels_computed;
	       sx = (maxX - minX) / (pxmax - pxmin);
		}
	}
	else  /* !preserve_vert_scale;  Map Pref III and IV */
	{
        maxY = mapgeo->plot_params.image_height_pixels;
		maxX = mapgeo->plot_params.image_width_pixels;
        sy = (maxY - minY) / (pymax - pymin);
	    sx = (maxX - minX) / (pxmax - pxmin);

		if( mapgeo->use_arc || preserve_aspect)
        {
            lon_adjust_factor = get_lon_adjust_factor( mapgeo );

		/* Archive: The graph projection does not enforce uniform x and y scaling of the projection space */
		/* Archive: correct for nonsquare pixel. select scale of smaller absolute magnitude to maintain the map
	       aspect ratio, the pixel aspect ratio must be considered  */

			if( fabs(sx) < fabs(sy * pixasp * lon_adjust_factor) )
			{	/* x is limiting */
				if (sy < 0.)
				   sy = -fabs(sx / lon_adjust_factor / pixasp);
				else
				   sy = fabs(sx / lon_adjust_factor / pixasp);
				maxY = sy * (pymax - pymin);
			}
			else
			{	/* y is limiting */
				if (sx < 0.)
				   sx = -fabs(sy * pixasp) * lon_adjust_factor;
				else
				   sx = fabs(sy * pixasp)* lon_adjust_factor;
				maxX = sx * (pxmax - pxmin);
			}
        }
        mapgeo->plot_params.image_height_pixels_computed = maxY;
        mapgeo->plot_params.image_width_pixels_computed = maxX;
    }

    /* translation factors */
    ty = minY - (pymin * sy);
    /* move the image space to the upper left corner of the PHIGS space */
    tx = minX - (pxmin * sx);

#if 0 /*JLL, may need re-enable this code using new DTCC */
        /* The ADRG polar projection does not need scaling of the projection space. */
        if (mapgeo->projection == PROJ_POLAR)
        {
           sx = 1.;
           sy = 1.;
           tx = ty = 0.;            /* reset translation factors */
        }
#endif
    mapgeo->plot_params.sx = sx;
    mapgeo->plot_params.sy = sy;
    mapgeo->plot_params.tx = tx;
    mapgeo->plot_params.ty = ty;
}

ERRSTATUS geo_to_grid_extent_fit( MAPGEO *mapgeo, COORD3 *geo_min, COORD3 *geo_max, COORD3 *grid_min, COORD3 *grid_max )
{
    double LARGE = 1.0e30;
    short NCHECK = 25;	/* gives NCHECK**2 sample points */

    COORD3 dd_point, map_point;
	int bad_transform_count = 0;
	char err_mess[80];

    /* variables to receive the projection extent */
    double          pxmax = -LARGE;
    double          pxmin = LARGE;
    double          pymax = -LARGE;
    double          pymin = LARGE;

    /* sample increments */
    double          checkx, checky;

    /* sample points */
    double          rlon, rlat;

    /* projected point */
    double          x, y;

    /* counters for for-loops */
    short           counter1, counter2;

    /* window sizes */
    /* float xsize,ysize; */
    double     rlone, rlonw, rlatn, rlats, /* sx, sy, tx, ty, */
               relv = 0.0;

    rlonw = dtcc_coord3_lon(geo_min);
    rlone = dtcc_coord3_lon(geo_max);
    rlats = dtcc_coord3_lat(geo_min);
    rlatn = dtcc_coord3_lat(geo_max);

    /* determine the project extent by sampling NCHECK*NCHECK points */
    checkx = (rlone - rlonw) / (float) (NCHECK - 1) ;
    checky = (rlatn - rlats) / (float) (NCHECK - 1) ;

    for (counter1 = 0, rlat = rlats; (counter1 < NCHECK) /* && (rlat <= rlatn) */; rlat += checky,
	 counter1++)
    {
    for (counter2 = 0, rlon = rlonw; (counter2 < NCHECK) /* && (rlon <= rlone) */; rlon += checkx,
	     counter2++)
	{
 		if( rlat > rlatn )
			rlat = rlatn;
 		if( rlon > rlone )
			rlon = rlone;

       if(dtcc_set_coord3_geo( &dd_point, rlat, rlon, relv))
            iErrorVmapConv("dtcc_set_coord3_geo failed");
        if( dtcc_transform(mapgeo->internal_to_display, (const void *)&dd_point, (void *)&map_point) < 0 )
			bad_transform_count++;
		else
		{
	        if( dtcc_is_system_grid(&mapgeo->map_display_coord_sys) )
            {
  	            x = dtcc_coord3_east(&map_point);
 	            y = dtcc_coord3_north(&map_point);
				/* translate "transform" y's to "drawing" y's */
		        if( strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			                "Universal Polar Stereographic South 84") ==  0 )
				   y = fabs(y);
          		else if( (strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Irish Transverse Mercator") == 0)  && (y < 0) )
			       /* y = dtcc_system_trueY( &mapgeo->map_display_coord_sys, &map_point ); */
				   y  = -y - 1000000.0;
           }
	       else
           {
		        x = dtcc_coord32(&map_point);
		        y = dtcc_coord31(&map_point);
           }
		   /* testing for projection extent */
           if (x > pxmax)
		       pxmax = x;
	       if (x < pxmin)
		        pxmin = x;	
	       if (y > pymax)
		        pymax = y;
	       if (y < pymin)
		       pymin = y;
		 }
	}
    }
	if( (pxmax == -LARGE) || (pxmin == LARGE) || (pymax == -LARGE) || (pymin == LARGE) )
	{
		sprintf( err_mess, "Unable to find projection extents." );
        iErrorVmapConv( err_mess );
		return STAT_BAD_PARM;
	}
	if( bad_transform_count > 0)
	{
		sprintf( err_mess, "Unable to convert %d DD point(s) to map system.",
                            bad_transform_count);
        iErrorVmapConv( err_mess );
	}

	/* translate "drawing" y's to "transform" y's */
    if( strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			                "Universal Polar Stereographic South 84") ==  0 )
	{
		pymin = -1.0 * pymin;
		pymax = -1.0 * pymax;
    }
    else if( (strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), "Irish Transverse Mercator") == 0) )
	{
		if( pymin < 0)
		pymin  = -1.0 * (pymin + 1000000.0);
		if( pymax < 0)
		pymax  = -1.0 * (pymax + 1000000.0);
	}
	dtcc_set_coord3_north(grid_min, pymin);
	dtcc_set_coord3_north(grid_max, pymax);
	dtcc_set_coord3_east(grid_min, pxmin);
	dtcc_set_coord3_east(grid_max, pxmax);

	return STAT_SUCCESS;
}

ERRSTATUS grid_to_geo_extent_fit( MAPGEO *mapgeo, COORD3 *grid_min, COORD3 *grid_max, COORD3 *geo_min, COORD3 *geo_max )
{
    double LARGE = 1.0e30;
    short NCHECK = 25;	/* gives NCHECK**2 sample points */

    COORD3 dd_point, map_point;
	char err_mess[80];
	double wlimit,elimit,nlimit,slimit,
		   pxmin, pxmax, pymin, pymax;

    /* variables to receive the geo extents */
    double          maxlat = -LARGE;
    double          minlat = LARGE;
    double          maxlon = -LARGE;
    double          minlon = LARGE;

    /* sample increments */
    double          checkx, checky;

    /* sample geo points */
    double          rlon, rlat;

    /* sample grid points */
    double          x, y, transform_y;

    /* counters for for-loops */
    short           counter1, counter2, bad_transform = 0,
		            num_samples = 13;

    double  relv = 0.0;

	pymin = dtcc_coord3_north(grid_min);
 	pymax = dtcc_coord3_north(grid_max);
	pxmin = dtcc_coord3_east(grid_min);
	pxmax = dtcc_coord3_east(grid_max);

	/* translate "transform" y's to "drawing" y's for loops */
     if( strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Universal Polar Stereographic South 84") ==  0 )
     {
	      pymin = fabs(pymin);
 	      pymax = fabs(pymax);
     }
     else if( (strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Irish Transverse Mercator") == 0) )
	 {
		 if( pymin < 0 )
		   pymin  = -pymin - 1000000.0;
		 if( pymax < 0 )
		   pymax  = -pymax - 1000000.0;
     }

    checkx = (pxmax - pxmin) / (float) (num_samples - 1);
    checky = (pymax - pymin) / (float) (num_samples - 1);
    for (counter1 = 0, y = pymin; (counter1 < num_samples) /* && (y <= pymax) */; y += checky,
	     counter1++)
    {
      for (counter2 = 0, x = pxmin; (counter2 < num_samples)/*  && (x <= pxmax) */; x += checkx,
	       counter2++)
	  {
		if( x > pxmax )
			x = pxmax;
 		if( y > pymax )
			y = pymax;

		transform_y = y;
		/* translate "drawing" y's to "transform" y's */
  	    if( strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Universal Polar Stereographic South 84") ==  0 )
	       transform_y = -1.0 * fabs(transform_y);
        else if( (strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys),"Irish Transverse Mercator") ==  0) &&
				( y < 0) )
	       transform_y = -1.0 * (transform_y + 1000000.0);

        if(dtcc_set_coord3_grid( &map_point, x, transform_y, relv,dtcc_system_type(&mapgeo->map_display_coord_sys)))
            iErrorVmapConv("dtcc_set_coord3_grid failed");
        if( dtcc_transform(mapgeo->display_to_internal, (const void *)&map_point, (void *)&dd_point) < 0 )
			bad_transform++;
		else
		{
		   rlon = dtcc_coord32(&dd_point);
		   rlat = dtcc_coord31(&dd_point);
           if (rlon > maxlon)
		       maxlon = rlon;
	       if (rlon < minlon)
		       minlon = rlon;
	       if (rlat > maxlat)
		       maxlat = rlat;
	       if (rlat < minlat)
		       minlat = rlat;
		}
	  }
    }
    if( (maxlat == -LARGE) || (maxlon == -LARGE) || (minlat == LARGE) || (minlon == LARGE) )
	{	
		sprintf( err_mess, "Unable to find new map geo extents.");
        iErrorVmapConv( err_mess );
		return STAT_BAD_PARM;
	}
	if( bad_transform > 0)
	{	
		sprintf( err_mess, "Unable to convert %d projection point(s) to decimal degrees",
                            bad_transform);
        iErrorVmapConv( err_mess );
	}

    if( strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Universal Polar Stereographic North 84") ==  0 )
    {
		if( (pxmin <= 2000000) && (pxmax >= 2000000) && (pymax >= 2000000) )
		{
			maxlon = 180.0;
			minlon = -180.0;
			if( pymin <= 2000000 )
				maxlat = 90.0;
		}
	}
    if( strcmp( dtcc_nameP( &mapgeo->map_display_coord_sys), 
			            "Universal Polar Stereographic South 84") ==  0 )
    {
		if( (pxmin <= 2000000) && (pxmax >= 2000000) && (pymin <= 2000000) )
		{
			maxlon = 180.0;
			minlon = -180.0;
			if( pymax >= 2000000 )
				minlat = -90.0;
		}
	}
	
	/* check new geo extents against system extents */
	wlimit = dtcc_coord32(dtcc_system_min_geoP(&mapgeo->map_display_coord_sys));
	elimit = dtcc_coord32(dtcc_system_max_geoP(&mapgeo->map_display_coord_sys));
	nlimit = dtcc_coord31(dtcc_system_max_geoP(&mapgeo->map_display_coord_sys));
	slimit = dtcc_coord31(dtcc_system_min_geoP(&mapgeo->map_display_coord_sys));
    if ( minlat<slimit )  
 	    minlat = slimit;	
    if ( minlon<wlimit )  
 	    minlon = wlimit;	
    if ( maxlat>nlimit )  
 	    maxlat = nlimit;	
    if ( maxlon>elimit )  
 	    maxlon = elimit;	
	dtcc_set_coord3_lon(geo_max, maxlon);
	dtcc_set_coord3_lon(geo_min, minlon);
	dtcc_set_coord3_lat(geo_max, maxlat);
	dtcc_set_coord3_lat(geo_min, minlat);
	return STAT_SUCCESS;
}

void* dtcc_decode_label( const char* lblP, void* dtccP )
	{	DTCC_LABELS* idP = (DTCC_LABELS*)dtccP;
		if( !lblP  )
			return NULL;
		else
		if( idP == NULL )
			idP = (DTCC_LABELS*)dtcc_allocate_ptr( sizeof(DTCC_LABELS) );
		
		if( idP ) _get_label_parts( lblP, 255, idP ); 
		return idP;
	}

const COORD_SYS* _find_system( const DTCC_LABELS* dtccP, int type, int onlyvalid )
	{	const char *lbls[3];
		int n[3];
		short k, start,stop;
		COORD_SYS* csP = (COORD_SYS*)_list_dataP(slist);
		COORD_SYS* result = NULL;
		lbls[0] =  dtcc_nameP(dtccP); 
		n[0] = strlen(lbls[0]);
		lbls[1] =  dtcc_abbrvP(dtccP);
		n[1] = strlen(lbls[1]); 
		lbls[2] = dtcc_otherP(dtccP);
		n[2] = strlen(lbls[2]); 
		_check_listsystem( type, &start, &stop );
		/* compare names & abbreviations */
		for( csP += start, k=start; k<stop; k++, csP++ )
		{	if( onlyvalid && !dtcc_is_valid(csP) ) 
				continue;
			if( n[1] > 0 )
			{	if( strncmp( lbls[1], dtcc_abbrvP(csP),n[1] ) == 0 || 
					strncmp( lbls[1], dtcc_nameP(csP),n[1] ) == 0 )
				{	result = csP;	 /* abbrv match is sufficient */
					break;
				}
			}
			if( n[0] > 0 )
			{	if( strncmp( lbls[0], dtcc_abbrvP(csP),n[0] ) == 0 || 
				strncmp( lbls[0], dtcc_nameP(csP),n[0] ) == 0 )
				{	result = csP;	 /* abbrv match is sufficient */
					break;
				}
			}
		}
		return result;
	}

short _update_system( COORD_SYS* cpP, const DTCC_LABELS* lblP,const COORD_GROUP* group, const DATUM_PARAMS*	datums, const PROJ_PARAMS* projection, const COORD_EXTENTS* extents, const COORD3_UNITS* units,	short dimension, short type )
	{	short k;		
		if( !cpP || !lblP )
			return ILLEGAL_ARG;
		else
		{	if( lblP )
			{	dtcc_copy_label( _SYSTEM_LABELP(cpP), lblP );
				dtcc_set_invalid( cpP );
			}
			if( group ) 
			{	_set_invalid(_SYSTEM_GROUPP(cpP)); 
				dtcc_copy_coord_group( _SYSTEM_GROUPP(cpP),group );
			}
			if( datums ) 
			{	dtcc_set_invalid(_SYSTEM_DATUMP(cpP));
				dtcc_copy_datum( _SYSTEM_DATUMP(cpP),datums );
			}
            if( projection ) 
            {	dtcc_set_invalid(_SYSTEM_PROJP(cpP));
				dtcc_copy_projection( _SYSTEM_PROJP(cpP),projection );
				update_projection_constants(_SYSTEM_PROJP(cpP));
			}
			if( extents ) 
			{	_set_invalid(_SYSTEM_EXTENTSP(cpP));
				dtcc_copy_extents( _SYSTEM_EXTENTSP(cpP),extents );
			}
			if( units ) 
			{	dtcc_invalidate_coord3_units(_SYSTEM_UNITSP(cpP));
				dtcc_copy_coord3_units( _SYSTEM_UNITSP(cpP),units );
				dtcc_set_coord3_mode( _SYSTEM_UNITSP(cpP), dtcc_coord_group_id(_SYSTEM_GROUPP(cpP)) );
			}
			if( dimension != SHORT_NULL ) dtcc_set_system_dimension( cpP,dimension );
			dtcc_set_type( cpP,type);
			_check_ellipsoids(cpP);
			 k = _check_itcoordsys( cpP );
    		if( k == VMAPOK ) dtcc_set_valid(cpP);
    		return k;
        }
    }   	

short dtcc_type_flag(const void* dtccP)
{	
	const DTCC_LABELS* idP = (DTCC_LABELS*)dtccP; 
	return ( dtccP ? idP->type : -1 );  
}

short dtcc_clear_projection( PROJ_PARAMS* proP )
	{	
	return dtcc_initialize_projection(proP);
	}

short dtcc_initialize_projection( PROJ_PARAMS* proP )
{	
	_PROJECTION_CONSTANTSP(proP)->id = 0;
    _PROJECTION_ENGINE(proP) = NULL; 
	return VMAPOK;  
}

short dtcc_copy_system(	COORD_SYS* cp1, const COORD_SYS* cp2 )
	{   short status =  ILLEGAL_ARG;;
		if( cp1 && cp2 )
		 	status = ( memcpy( cp1,cp2, sizeof(COORD_SYS) ) != NULL ? VMAPOK : -100 );
		return status;
    }
short dtcc_validate_system(	COORD_SYS* csP	)
{	
	return _check_itcoordsys( csP );   
}

int dtcc_compare_vdatums( const DATUM_PARAMS* dpP1,const DATUM_PARAMS* dpP2 )
	{	if( _DATUM_VNAMEP(dpP1) == NULL || _DATUM_VNAMEP(dpP2) == NULL )
			return  _IS_NOT_EQUAL_TO;
		else
		if( strcmp( _DATUM_VNAMEP(dpP1),_DATUM_VNAMEP(dpP2)) != 0 )
			return  _IS_NOT_EQUAL_TO;
		else
			return _IS_EQUAL_TO;
	}

short dtcc_compare_datums(	const DATUM_PARAMS* dpP1,const DATUM_PARAMS* dpP2)
    {	if( dtcc_compare_vdatums(dpP1,dpP2) == _IS_NOT_EQUAL_TO )
    		 return _IS_NOT_EQUAL_TO;
		else
		if(	 _DATUM_SHIFTSP(dpP1)[0] != _DATUM_SHIFTSP(dpP2)[0] ||
    		 _DATUM_SHIFTSP(dpP1)[1] != _DATUM_SHIFTSP(dpP2)[1] ||
    		 _DATUM_SHIFTSP(dpP1)[2] != _DATUM_SHIFTSP(dpP2)[2]  )
    		 return _IS_NOT_EQUAL_TO;
    	else
    	if(	 _DATUM_SIGMASP(dpP1)[0] != _DATUM_SIGMASP(dpP2)[0] ||
    		 _DATUM_SIGMASP(dpP1)[1] != _DATUM_SIGMASP(dpP2)[1] ||
    		 _DATUM_SIGMASP(dpP1)[2] != _DATUM_SIGMASP(dpP2)[2]  )
    		 return _IS_NOT_EQUAL_TO;
    	else
			return dtcc_compare_ellipsoid( _DATUM_ELLIPSOIDP(dpP2), _DATUM_ELLIPSOIDP(dpP1) );
	}
void _init_molo_parameters(	double fmA, double fmRF, const short* fmShfts, const short* fmSgms,	double toA, double toRF, const short* toShfts, const short* toSgms,	struct TRANSFORM_PARAMS *params )
	{	double fmF = 1.0/fmRF;
		if( w72(fmA,fmRF,fmShfts)  && w84(toA,toRF,toShfts) ) /* 72 -> 84 */
			SPECCODE = 72; /* WGS72 to WGS84 */
		else     
		if( w84(fmA,fmRF,fmShfts)  && w72(toA,toRF,toShfts) ) /* 84 -> 72 */
			SPECCODE = -72;	/* WGS84 to WGS72 */	
	    else      
		if( w66(fmA,fmRF,fmShfts)  && w84(toA,toRF,toShfts) ) /* 72 -> 84 */
			SPECCODE = 66; /* WGS72 to WGS84 */
		else     
		if( w84(fmA,fmRF,fmShfts)  && w66(toA,toRF,toShfts) ) /* 84 -> 72 */
			SPECCODE = -66;	/* WGS84 to WGS72 */	
	    else      
		{	if( w84(toA,toRF,toShfts) ) /* ? -> 84 */
				SPECCODE = 2; /* to WGS84 */
			else     
			if( w84(fmA,fmRF,fmShfts) ) /* 84 -> ? */
				SPECCODE = -2;	/* WGS84 to ? */	
	    	else
	    		SPECCODE = 0;
			FROM_DX =  fmShfts[0]; TO_DX = toShfts[0];
	   	 	FROM_DY =  fmShfts[1]; TO_DY = toShfts[1];
			FROM_DZ =  fmShfts[2]; TO_DZ = toShfts[2];
			FROM_SX =  fmSgms[0];  TO_SX = toSgms[0];
	    	FROM_SY =  fmSgms[1];  TO_SY = toSgms[1];
			FROM_SZ =  fmSgms[2];  TO_SZ = toSgms[2];
		}
		AXIS = fmA; TO_AXIS = toA;
/*	   Check for sphere 8/1/97 rjl	
		FLAT = 1.0/fmRF; TO_FLAT = 1.0/toRF;
*/		
		FLAT = ( fmRF <= 0.0 ? 0.0 : 1.0/fmRF ); 
		TO_FLAT = ( toRF <= 0.0 ? 0.0 : 1.0/toRF );
		GEOIDFP = NULL;
		return;
	}

static short _reset( PROJ_PARAMS* ppP, const char* name, const ELLIPS_PARAMS* elP  )
	{	COORD_ORIGIN* coP;
		short k = VMAPOK;
		if( !ppP  )  
			return ILLEGAL_ARG;
		else
		if( !name )
			_reset_null(ppP);
		else
		if( name[0] == 0 || strncmp( name, "GEOG",4) == 0 )
			_reset_null(ppP);
		/* Ellipsoid */
		else
		{	if( elP )
				dtcc_copy_ellipsoid( _PROJECTION_ELLIPSOIDP(ppP), elP );
			/* Origin */
        	coP = _PROJECTION_ORIGINP( ppP ); 
        	dtcc_set_origin_gridSW
        		( coP, DOUBLE_NULL, DOUBLE_NULL, DOUBLE_NULL, SHORT_NULL );
        	dtcc_set_origin_grid
        	    ( coP, DOUBLE_NULL, DOUBLE_NULL, DOUBLE_NULL, SHORT_NULL );
        	dtcc_set_origin_geo
         	   ( coP, DOUBLE_NULL, DOUBLE_NULL, DOUBLE_NULL ); 
        	dtcc_set_origin_scale
        	    ( coP, DOUBLE_NULL, DOUBLE_NULL, DOUBLE_NULL ); 
        	dtcc_set_origin_azimuth( coP, DOUBLE_NULL );
        	dtcc_set_origin_convergence( coP, DOUBLE_NULL );

			/* Standard parallels */
			ppP->stdpars[0] = ppP->stdpars[1] = DOUBLE_NULL;
		 
			_get_projection_label( name, _PROJECTION_LABELP(ppP) );
				_setProjectionEngine( ppP );  
			_projection( ppP, DTCC_PROJECTION_SETUP, NULL,1L, NULL );
		}
		dtcc_set_invalid(ppP);
		k = _check_itproj(ppP); 
		if( k == VMAPOK )  dtcc_set_valid(ppP); 
		return k;	
		
	}		

short dtcc_encode_coord_group( 	const COORD_GROUP* cgP,char* labelP	) 
	{	COORD3_TYPES id = dtcc_coord_group_id(cgP);
		return _group_label( 0, &id, labelP );
	} 
long hash( const void* v, long n )
{	const unsigned char* s = (unsigned char*)v;
	long sum = 0, k = 0;
	while( --n >= 0 )
	{	sum += ( *s++ << k++ );
		if( k >= 16 ) k = 0;
	}
	return sum;
}

short dtcc_copy_label( DTCC_LABELS* destP, const DTCC_LABELS* srcP )
	{	return blockcopy( destP, srcP, sizeof(DTCC_LABELS) );  }

void dtcc_set_invalid(void* dtccP)
	{	DTCC_LABELS* idP = (DTCC_LABELS*)dtccP; _label_vflag(idP) = 0; return;  }

void dtcc_set_type( void* dtccP, int n )
	{	DTCC_LABELS* idP = (DTCC_LABELS*)dtccP; _label_tflag(idP) = n; return;  }

short dtcc_reset_system_extents( COORD_SYS* csP, const COORD3* mngridP, const COORD3* mxgridP, const COORD3* mngeoP, const COORD3* mxgeoP )	
	{	return dtcc_reset_extents
			( _SYSTEM_EXTENTSP(csP),mngridP,mxgridP,mngeoP,mxgeoP );  }

short dtcc_initialize_datum_list( const char* fname ) 
	{	FILE* fp = _scan_list_hdr( &dlist, _check_datum_name(fname)  );
		if( !fp )
			return FILE_NOT_FOUND;
		else
		{	int i, n = _list_total(dlist);
			_list_dataP(dlist) = dtcc_allocate_ptr( (n+1)*sizeof(DATUM_PARAMS));
			if( !_list_dataP(dlist) )
			{	dtcc_delete_datum_list(&dlist);
				fclose(fp);
				return MEMORY_ALLOCATION_FAILURE;
			}
			else
			{	DATUM_PARAMS* dpP = (DATUM_PARAMS*)_list_dataP(dlist);
				for( i=0; i<n; i++, dpP++ )
				   dtcc_scan_hdatum( dpP,fp );
				fclose(fp);
				_list_added(dlist) = 0;
				_list_modified(dlist) = 0;
				return VMAPOK;
			}
		}
	}

const DATUM_PARAMS* _find_datum( const void* dtccP, int type )
	{	const char *lbls[3];
		int i,n[3];
		short k, start,stop;
		DATUM_PARAMS* dpP = (DATUM_PARAMS*)_list_dataP(dlist);
		lbls[0] =  dtcc_nameP(dtccP); 
		n[0] = strlen(lbls[0]);
		lbls[1] =  dtcc_abbrvP(dtccP);
		n[1] = strlen(lbls[1]); 
		lbls[2] = dtcc_otherP(dtccP);
		n[2] = strlen(lbls[2]); 
		_check_listdatum( type, &start, &stop );
		 
		for( dpP += start, k=start; k<stop; k++, dpP++ )
		{	if( dtcc_is_typeN(dtcc_type_flag(dpP), SUPPORTED_DATUMS) )  
			{	for( i=0; i<3; i++ )  
					if( strcmp( lbls[i], dtcc_abbrvP(dpP) ) == 0 )
						return dpP;	 /* abbrv match is sufficient */
			}
			if( strcmp( lbls[0], dtcc_nameP(dpP) ) == 0 )
			{	if( strcmp( lbls[1], dtcc_otherP(dpP) ) == 0 ||
					strcmp( lbls[2], dtcc_otherP(dpP) ) == 0 )
					return dpP;  /* name & area match is sufficient for any type */
			}				
		}		
		return NULL;
	}

short _update_datum(  DATUM_PARAMS* dpP, const char* nm1, const char* nm2, const char* vnm, const char* ab, const short* shfts,const short* sigs, const ELLIPS_PARAMS* elP, int type )
	{	if( dpP )
		{	short  zero[3] = {0,0,0};
			char nm[_DTCC_LABEL_NAME_SIZE];
    		ab = (ab && !dtcc_is_typeN((short)type, USER_DATUMS) ? ab : _DEFAULT_USER_DABBRV);
			shfts = (shfts ? shfts : zero);
			sigs = (sigs ? sigs : zero);
			vnm = ( !vnm ? _DEFAULT_VNAME :
				  (	strlen(vnm) >= _VDATUM_NAME_SIZE ? _DEFAULT_VNAME : vnm ));
			dpP->vdatum_fp = NULL;
			strcpy( _DATUM_VNAMEP(dpP),vnm );
			memcpy( _DATUM_SHIFTSP(dpP), shfts, 3*sizeof(short) );
			memcpy( _DATUM_SIGMASP(dpP), sigs, 3*sizeof(short) );
			dtcc_copy_ellipsoid( _DATUM_ELLIPSOIDP(dpP), elP );
			if( nm1 )
    			strcpy( nm,nm1 );
			else
				dtcc_make_hdatum_name( dpP,nm );
			nm2 = (nm2 ? nm2 : _DEFAULT_USER_DAREA );
			dtcc_set_label( dpP, nm, ab, nm2, 0, (short)type );
			dtcc_validate_datum( dpP );
			return VMAPOK;
		}
		return ILLEGAL_ARG;
	}

short dtcc_validate_coord3_units( COORD3_UNITS* csP	)
	{	return _check_itcoord3( csP );  }

short dtcc_validate_datum( DATUM_PARAMS* dpP )
	{	_check_itdatum( dpP );
		return ( dtcc_valid_flag(dpP) ? VMAPOK : 1 );
	}

short dtcc_validate_projection(	PROJ_PARAMS* csP )
	{	short k = _check_itproj( csP );
		if( k >= VMAPOK ) dtcc_set_valid(csP);
		return k;
	}

short dtcc_validate_coord3_extents(	COORD_EXTENTS* csP	)
	{	short status = _check_itcoordextent( csP );
		if( status >= VMAPOK ) 
		 	_set_valid(csP); 
		else
			_set_invalid(csP); 
		return status;
	}

static short _check_ellipsoids( COORD_SYS* csP )
	{	if( dtcc_compare_ellipsoid( dtcc_system_ellipsoidP(csP), 
				dtcc_system_projection_ellipsoidP(csP)) != VMAPOK )
		{	dtcc_copy_ellipsoid
				( (ELLIPS_PARAMS*)dtcc_system_projection_ellipsoidP(csP),
					 dtcc_system_ellipsoidP(csP) );
			return 1;
		}
		else
			return 0;
	}
void dtcc_set_valid(void* dtccP)
	{	DTCC_LABELS* idP = (DTCC_LABELS*)dtccP; _label_vflag(idP) = 1; return;  }

short adrg_zone_compute(double latitude,long *zone)
{
  if(latitude >= 0.E0 && latitude < 32.E0 )
     *zone = 1;
  else if(latitude >=  32.E0 && latitude <   48.E0 )
     *zone = 2;
  else if(latitude >=  48.E0 && latitude <   56.E0 )
     *zone = 3;
  else if(latitude >=  56.E0 && latitude <   64.E0 )
     *zone = 4;
  else if(latitude >=  64.E0 && latitude <   68.E0 )
     *zone = 5;
  else if(latitude >=  68.E0 && latitude <   72.E0 )
     *zone = 6;
  else if(latitude >=  72.E0 && latitude <   76.E0 )
     *zone = 7;
  else if(latitude >=  76.E0 && latitude <   80.E0 )
     *zone = 8;
  else if(latitude >=  80.E0 && latitude <=  90.E0 )
     *zone = 9;
  else if(latitude <    0.E0 && latitude >  -32.E0 )
     *zone = 10;
  else if(latitude <= -32.E0 && latitude >  -48.E0 )
     *zone = 11;
  else if(latitude <= -48.E0 && latitude >  -56.E0 )
     *zone = 12;
  else if(latitude <= -56.E0 && latitude >  -64.E0 )
     *zone = 13;
  else if(latitude <= -64.E0 && latitude >  -68.E0 )
     *zone = 14;
  else if(latitude <= -68.E0 && latitude >  -72.E0 )
     *zone = 15;
  else if(latitude <= -72.E0 && latitude >  -76.E0 )
     *zone = 16;
  else if(latitude <= -76.E0 && latitude >  -80.E0 )
     *zone = 17;
  else if(latitude <= -80.E0 && latitude >= -90.E0 )
     *zone = 18;
  else
     return false;
  return true;
}

static void _get_label_parts ( const char* label, int mx, DTCC_LABELS* idP )
{	char part[3][DTCC_LABEL_SIZE], *lookP;
	int i,j,k, np = 3, n = strlen(label), cnt = 0;
	part[0][0] = part[1][0] = part[2][0] = 0;
	for( i=0, j=0, k=0; i<=n; i++, label++ )
	{	if( *label == '|' || *label == 0 )
		{	part[k][j] = 0;
			dtcc_trim_white( part[k] ); 
			if( j > 6 )
			{	lookP = part[k] + (j-6);
				if( strcmp( lookP,", etc.") == 0 )
					part[k][j-6] = 0;
			}
			cnt += j;
			k++; j = 0; 
		}
		else
		if( isspace(*label) && j == 0 )
			;
		else
		if( j+cnt < mx ) 
		part[k][j++] = *label;
		
	}
	dtcc_set_label( idP, part[0], part[1], part[2],0, _ENTIRE_LIST );  
	return;
}

static short _check_listsystem( int type, short *start, short *stop )
	{	if( slist == NULL )
		{	if( dtcc_initialize_system_list(NULL) != VMAPOK )
				return FILE_NOT_OPEN;
		}
		*start = ( type == _TYPE2_LIST ? dtcc_supported_system_count() : 0 );
		*stop =  ( type == _TYPE1_LIST ? dtcc_supported_system_count() : 
					dtcc_system_count() );
		return VMAPOK;
    }	
short dtcc_copy_coord_group( COORD_GROUP* cg1P,	const COORD_GROUP* 	cg2P )
	{	return blockcopy( cg1P, cg2P, sizeof(COORD_GROUP) );  }

short dtcc_copy_datum( DATUM_PARAMS* dP1, const DATUM_PARAMS* dP2  )
	{	return blockcopy( dP1, dP2, sizeof(DATUM_PARAMS) );  }

short dtcc_copy_projection( PROJ_PARAMS* ppP1, const PROJ_PARAMS* ppP2 )
    {   short status = 0;
    	if( ppP1 != NULL )
		{	if( ppP2 == NULL ) ppP2 = (PROJ_PARAMS*)dtcc_allocate_ptr( sizeof(PROJ_PARAMS) );
			if( ppP2 ) 
			{	memcpy( ppP1,ppP2,sizeof(PROJ_PARAMS));
				status = 0;
			}
			else
				status = MEMORY_ALLOCATION_FAILURE;
		}
        return status;
    }

void update_projection_constants(PROJ_PARAMS* ppP)
{	_setProjectionEngine( ppP ); return;  }	

short dtcc_copy_extents( COORD_EXTENTS* ceP1, const COORD_EXTENTS* ceP2  ) 
	{	return blockcopy( ceP1, ceP2, sizeof(COORD_EXTENTS) );  }

short dtcc_invalidate_coord3_units(	COORD3_UNITS* csP)
	{	short old = _check_itcoord3(csP);
		dtcc_set_invalid(_COORD3_UNITSNP(csP,0));  
		dtcc_set_invalid(_COORD3_UNITSNP(csP,1));  
		dtcc_set_invalid(_COORD3_UNITSNP(csP,2));  
		return old;   
	}

short dtcc_copy_coord3_units(	COORD3_UNITS* dest,	const COORD3_UNITS* src )
	{	return blockcopy( dest, src, sizeof(COORD3_UNITS) );  }

short dtcc_set_system_dimension( COORD_SYS* csP, short dm )
{	if( !csP )
		return ILLEGAL_ARG;
	else
	{	
		dtcc_set_system_units( 
					csP, 
					(short)dtcc_coord3_mode( _SYSTEM_UNITSP(csP) ), 
					1.0, 
					NULL,
					NULL, 
					( dm == 3 ? "m" : "") 
					);
		 _SYSTEM_DIMENSION(csP) = dm;
	/*	_set_default_name( csP );  */
		return VMAPOK;
	}
}              

short dtcc_compare_ellipsoid(	const ELLIPS_PARAMS* elP1, const ELLIPS_PARAMS* elP2)
    {	int k = ( elP1->axis == elP2->axis ? _IS_EQUAL_TO :
					( elP1->axis < elP2->axis ? _IS_LESS_THAN : _IS_GREATER_THAN ));
    	if( k != _IS_EQUAL_TO  )
    		return k;
    	else
			return 
				( dtcc_ellipsoid_ecc(elP1) == dtcc_ellipsoid_ecc(elP2) ? _IS_EQUAL_TO :
				( dtcc_ellipsoid_ecc(elP1) < dtcc_ellipsoid_ecc(elP2) ? _IS_LESS_THAN : 
					_IS_GREATER_THAN ));
    }

static int w84( double a, double rf, const short* s )
{	double a84 = 6378137.000, rf84 = 298.257223563;
	if( fabs(a-a84) < 0.01 && fabs(rf-rf84) < 0.0000001 )
		if( s[0] == 0 && s[1] == 0 && s[2] == 0 )
			return 1;
	return 0;
}
static int w72( double a, double rf, const short* s )
{	double a72 = 6378135.000, rf72 = 298.26;
	if( fabs(a-a72) < 0.01 && fabs(rf-rf72) < 0.000001 )
		if( s[0] == 0 && s[1] == 0 && s[2] == 0 )
			return 1;
	return 0;
}
static int w66( double a, double rf, const short* s )
{	double a66 = 6378145.000, rf66 = 298.25;
	if( fabs(a-a66) < 0.01 && fabs(rf-rf66) < 0.000001 )
		if( s[0] == 0 && s[1] == 0 && s[2] == 0 )
			return 1;
	return 0;
}

static void _reset_null(PROJ_PARAMS* ppP)
{	dtcc_set_invalid(ppP); 
	_PROJECTION_ENGINE(ppP) = NULL; 
	dtcc_clear_label(ppP);
	dtcc_reset_default_ellipsoid(_PROJECTION_ELLIPSOIDP(ppP));
	dtcc_set_origin( _PROJECTION_ORIGINP(ppP), 	NULL,NULL,NULL,DOUBLE_NULL,DOUBLE_NULL );
	ppP->stdpars[0] = ppP->stdpars[1] = DOUBLE_NULL;
	ppP->ncnsts = ppP->fixed = 0;
	return;  
}  

short dtcc_copy_ellipsoid( ELLIPS_PARAMS* ep1, const ELLIPS_PARAMS* ep2 )
	{	return blockcopy( ep1, ep2, sizeof(ELLIPS_PARAMS) );  }


short dtcc_set_origin_geo(  COORD_ORIGIN* coP, double latitude, double longitude, double elv )
    {   if( coP )
        { 	short k;
        	_set_invalid(coP);
        	dtcc_set_coord3
        		( _ORIGIN_GEOP(coP),
        		  latitude,longitude,elv,SHORT_NULL );
        	/* k = _check_itcoord(coP); */
			k = VMAPOK;
        	if( k == VMAPOK ) _set_valid(coP);
        	return k;
        }
        else
        	return ILLEGAL_ARG;
    }
short dtcc_set_origin_scale( COORD_ORIGIN* coP, double k0, double h0,  double  v0 )
    {   if( coP )
        {   short k;
        	_set_invalid(coP);
        	dtcc_set_coord3
        		( _ORIGIN_SCALEP(coP),
        			k0,h0,v0,SHORT_NULL );
        	/* k = _check_itcoord(coP); */
			k = VMAPOK;
        	if( k == VMAPOK ) _set_valid(coP);
        	return k;
        }
        return ILLEGAL_ARG;
    }


short dtcc_set_origin_azimuth( COORD_ORIGIN* coP, double  azm  )
    {   if( coP )
        {   short k;
        	_set_invalid(coP);
        	dtcc_set_angle( _ORIGIN_AZIMUTHP(coP), azm );
        	/* k = _check_itcoord(coP); */
			k = VMAPOK;
        	if( k == VMAPOK ) _set_valid(coP);
        	return k;
        }
        else
            return ILLEGAL_ARG;
    }
    
short dtcc_set_origin_convergence( COORD_ORIGIN* coP, double w0)
    {   if( coP )
        {   short k;
        	_set_invalid(coP);
        	dtcc_set_angle( _ORIGIN_CONVERFERCEP(coP), w0 );
        	/* k = _check_itcoord(coP); */
			k = VMAPOK;
        	if( k == VMAPOK ) _set_valid(coP);
        	return k;
        }
        else
            return ILLEGAL_ARG;
    }
short dtcc_set_origin_gridSW( COORD_ORIGIN* coP, double northing, double easting, double hgt, short zone )
    {   if( coP )
        {   short k;
        	_set_invalid(coP);
        	dtcc_set_coord3
        		( _ORIGIN_GRIDSWP(coP),
        			easting,northing, hgt,zone );
        	/* k = _check_itcoord(coP); */
			k = VMAPOK;
        	if( k == VMAPOK ) _set_valid(coP);
        	return k;
        }
        return ILLEGAL_ARG;
    }

short dtcc_set_origin_grid( COORD_ORIGIN* coP, double northing, double easting,  double hgt,   short zone  )
    {   if( coP )
        {   short k;
        	_set_invalid(coP);
        	dtcc_set_coord3
        		( _ORIGIN_GRIDNEP(coP),
        			easting,northing,hgt,
						(short)(zone==SHORT_NULL ? COORD3_GRID : zone) );
        	/* k = _check_itcoord(coP); */
			k = VMAPOK;
        	if( k == VMAPOK ) _set_valid(coP);
        	return k;
        }
        return ILLEGAL_ARG;
    }

short _get_projection_label( const char* name, void* lblP )
{	if(  lblP && name )
    {	DTCC_LABELS* label = (DTCC_LABELS*)dtcc_decode_label( name, lblP );
    	const struct _PROJECTION_PARS* proP = _find_pro( label );
		if( proP )
		{	dtcc_set_label( lblP, proP->name, proP->abbrv, NULL, 1,proP->type );
    		return VMAPOK;
		}
		else
			return ITEM_NOT_FOUND;
    }
    else
    	return ILLEGAL_ARG;
}

static short _setProjectionEngine( PROJ_PARAMS* ppP )
    {   if( ppP )
    	{	_PROJECTION_ENGINE(ppP) = 
    			(void*)_projection_engineP(_PROJECTION_LABELP(ppP));
            _PROJECTION_CONSTANTSP(ppP)->id = 0;
           	if( _PROJECTION_ENGINE(ppP) == NULL )
            {	dtcc_set_invalid(ppP);
				return ITEM_NOT_FOUND;
			}
           	else
           	{	dtcc_set_valid(ppP);
				return VMAPOK;
			}
        }
        else
        	return ILLEGAL_ARG;
    }
static short _check_itproj( PROJ_PARAMS* ppP )
	{	short status = 0;
		if( !dtcc_is_valid( _PROJECTION_ELLIPSOIDP(ppP) ) )
			status = STRUCTURE_INVALID;
		else
		if( !dtcc_is_valid_name(dtcc_nameP(ppP)) )
			status = STRUCTURE_INVALID;
		else
		if( _PROJECTION_ENGINE(ppP) == 0) 
		  	status = STRUCTURE_INVALID;
		else
		if( dtcc_validate_origin(_PROJECTION_ORIGINP(ppP)) < 0 )
			status = STRUCTURE_INVALID;   	
		else 
		if( _PROJECTION_STDPARSP(ppP)[1] > DOUBLE_NULL &&
			_PROJECTION_STDPARSP(ppP)[1] < _PROJECTION_STDPARSP(ppP)[0] )
  			status = STRUCTURE_INVALID;   	
		else
			status = 0;
		
		if( status >= 0 )
			dtcc_set_valid(ppP);
		else
			dtcc_set_invalid(ppP);	 
		return  status;
    }

static short _check_itdatum( DATUM_PARAMS* dtccP )
	{	if( !dtcc_is_valid_name(dtccP) )
		{	char nm[128];
			dtcc_make_hdatum_name( dtccP,nm );
			dtcc_set_label( dtccP, nm, _DEFAULT_USER_DABBRV, NULL,1,USER_DATUMS );
		}
		dtcc_set_valid(dtccP); 
		return VMAPOK;  
	}

static short _check_itcoord3( const COORD3_UNITS* cuP )
	{	if( dtcc_validate_units(dtcc_coord3_unitsNP(cuP,0)) >= VMAPOK &&
			dtcc_validate_units(dtcc_coord3_unitsNP(cuP,1)) >= VMAPOK &&
			dtcc_validate_units(dtcc_coord3_unitsNP(cuP,2)) >= VMAPOK )
			return 0;
		else
			return 1;
   }

static short _group_label( short opt, COORD3_TYPES* idP, char* label )
	{	COORD3_TYPES id;
		char labels[][8] = 
			{	"UPS", "UTM",  "MGRS", "GRID",  
				"XYZ", "DAZM", "GEO",  "GREF"	};		
								
		if( opt == 0 )
		{  char* glab;
		   id = *idP;
		   glab = ( id == COORD3_UPS 	?	labels[0] :
				  ( id <= COORD3_UTM	?	labels[1] :
				  ( id == COORD3_MGRS 	?	labels[2] :
				  ( id == COORD3_GRID 	?	labels[3] :
				  ( id == COORD3_XYZ 	?	labels[4] :
				  ( id == COORD3_DAZM 	?	labels[5] :
				  ( id == COORD3_GEO 	?	labels[6] :
				  ( id == COORD3_GREF 	?	labels[7] :
				        		"" ))))))));
			strcpy( label, glab );
			return ( glab[0] != 0 ? VMAPOK : ITEM_NOT_FOUND );
		}
		else
		{  id = ( strcmp(label,labels[0]) ==  0  ? COORD3_UPS :
				( strcmp(label,labels[1]) ==  0  ? COORD3_UTM :
				( strcmp(label,labels[2]) ==  0  ? COORD3_MGRS :
				( strcmp(label,labels[3]) ==  0  ? COORD3_GRID :
				( strcmp(label,labels[4]) ==  0  ? COORD3_XYZ :
				( strcmp(label,labels[5]) ==  0  ? COORD3_DAZM :	
				( strcmp(label,labels[6]) ==  0  ? COORD3_GEO :
				( strcmp(label,labels[7]) ==  0  ? COORD3_GREF :
				         COORD3_UNDEFINED ))))))));
			*idP = id;
			return ( id == COORD3_UNDEFINED ? ITEM_NOT_FOUND : VMAPOK );
		}
	}

short blockcopy( void* destP, const void* srcP, int n )
	{	if( !destP || !srcP )
			return ILLEGAL_ARG;
		else
		if( memcpy( destP, srcP,n ) != destP )
			return COPY_ERROR;
		else
			return 0;
	}
short dtcc_reset_extents( COORD_EXTENTS* ceP, const COORD3* mngridP, const COORD3* mxgridP,const COORD3* mngeoP, const COORD3* mxgeoP  )
    {	if( !ceP )
    		return ILLEGAL_ARG;
    	if( mngridP )
    	{	dtcc_set_extents_min_north( ceP, dtcc_coord3_north(mngridP) );
    	    dtcc_set_extents_min_east( ceP, dtcc_coord3_east(mngridP) );
    	    dtcc_set_extents_min_hgt( ceP, dtcc_coord3_hgt(mngridP) );
    	    dtcc_set_extents_min_zone( ceP, dtcc_coord3_zone(mngridP) );
    	}
    	if( mxgridP )
    	{	dtcc_set_extents_max_north( ceP, dtcc_coord3_north(mxgridP) );
    	    dtcc_set_extents_max_east( ceP, dtcc_coord3_east(mxgridP) );
    	    dtcc_set_extents_max_hgt( ceP, dtcc_coord3_hgt(mxgridP) );
    	    dtcc_set_extents_max_zone( ceP, dtcc_coord3_zone(mxgridP) );
    	}
    	if( mngeoP )
    	{	dtcc_set_extents_min_lat( ceP, dtcc_coord3_lat(mngeoP) );
    	    dtcc_set_extents_min_lon( ceP, dtcc_coord3_lon(mngeoP) );
    	    dtcc_set_extents_min_elv( ceP, dtcc_coord3_elv(mngeoP) );
    	}
    	if( mxgeoP )
    	{	dtcc_set_extents_max_lat( ceP, dtcc_coord3_lat(mxgeoP) );
    	    dtcc_set_extents_max_lon( ceP, dtcc_coord3_lon(mxgeoP) );
    	    dtcc_set_extents_max_elv( ceP, dtcc_coord3_elv(mxgeoP) );
    	}
    	return _check_itcoordextent(ceP);
    }
FILE* _scan_list_hdr( _DTCC_LIST** lstH, const char* fn )
	{	int cnt;
		int where = 0;
		FILE* fp = NULL;
		short flag = 0;
		_DTCC_LIST* lstP = NULL;
		fp = dtcc_open_dat_file( fn,"r" );
		if( !fp )
			return NULL;
		else
		if( (lstP = (_DTCC_LIST*)dtcc_allocate_ptr(sizeof(_DTCC_LIST)))== NULL )
		{	fclose(fp);
			return NULL;
		}
		else
			*lstH = lstP;

		strcpy( _list_nameP(lstP), fn );
		if( _scan_hdr( fp, _list_olabP(lstP), &cnt ) == 0 )
		{	_list_type1(lstP) = cnt;
			_list_type2(lstP) = 0;
			_list_rsize(lstP) = 0;
			_list_added(lstP) = 0;
			_list_modified(lstP) = 0;
		}
		else
		{	fclose(fp);
			fp = NULL;
		}
		return fp;
	}

static const char* _check_datum_name( const char* fn )
	{	return _check_file_name(fn, _HDATUMS_FILENAME );  }

short dtcc_delete_datum_list(_DTCC_LIST** lstP)  {  return VMAPOK;  }

short dtcc_scan_hdatum( DATUM_PARAMS* dtm, FILE* fp )
{	short status = 0;
	char name[128], desc[128], abbrv[8], where[64], ellp_abbrv[8];
	short d[3],s[3];
	char bfr[128];
	desc[0] = 0;
	if( scanTo( fp, ',', 128, name ) == EOF ) 
		status = -1;
	else
	{	for(;;)
		{	if( scanTo( fp, ',', 128, bfr ) == EOF ) 
				status = -2;
			else
			if( strcmp( "_DATUMS", bfr + strlen(bfr)-7 ) != 0 )
			{	if( desc[0] == 0 )
					strcpy( desc,bfr );
				else
					strcat( strcat( desc, ", " ), bfr );
			}
			else
			{	strcpy( where, bfr );
				break;
			}
		}
		if( scanTo( fp, ',', 8, abbrv ) == EOF ) 
			status = -3;
		else
		if( scanTo( fp, ',', 8, ellp_abbrv ) == EOF ) 
			status = -4;
		else
		{	int j;
			for( j=0; j<3; j++ )
			{	if( scanTo( fp, ',', 8, bfr ) == EOF )
				{	status = -5;
					break;
				}
				sscanf( bfr, "%hd", d+j );
			}
			for( j=0; j<3; j++ )
			{	if( scanTo( fp, ',', 8, bfr ) == EOF )
				{	status = -6;
					break;
				}
				sscanf( bfr, "%hd", s+j );
			}
		}
	}
	if( status == 0 )
	{	ELLIPS_PARAMS ellp;
		status = dtcc_reset_ellipsoid( &ellp, ellp_abbrv );
		if( status == 0 )
		{	int nwhere = 
				( strncmp( where,"WORLD_WIDE_DATUMS",5 ) == 0 ? WORLD_WIDE_DATUMS :
				( strncmp( where,"NORTH_AMERICAN_DATUMS",5 ) == 0 ? NORTH_AMERICAN_DATUMS :
				( strncmp( where,"SOUTH_AMERICAN_DATUMS",5 ) == 0 ? SOUTH_AMERICAN_DATUMS :
				( strncmp( where,"AFRICAN_DATUMS",5 ) == 0 ? AFRICAN_DATUMS :
				( strncmp( where,"ASIAN_DATUMS",5 ) == 0 ? ASIAN_DATUMS :
				( strncmp( where,"EUROPEAN_DATUMS",5 ) == 0 ? EUROPEAN_DATUMS :
				( strncmp( where,"MICRONESIAN_DATUMS",5 ) == 0 ? MICRONESIAN_DATUMS :
				( strncmp( where,"MISC_ISLAND_DATUMS",5 ) == 0 ? MISC_ISLAND_DATUMS :
				( strncmp( where,"ANTARCTIC_DATUMS",5 ) == 0 ? ANTARCTIC_DATUMS : 0 )))))))));
			status = 
				_update_datum( dtm, name, desc, "AMSL", abbrv,d,s, &ellp, nwhere+1 );
		}
	}
	return status;
}

int dtcc_is_typeN( short k, int n )
	{	int m = (int)k & n;  
		return m != 0;  
	}

DTCC_LABELS* dtcc_set_label( void* dtccP, const char* name, const char* abbrv, const char* other,short valid, short type )
	{	int max = DTCC_LABEL_SIZE-2;
		DTCC_LABELS* lblP = (DTCC_LABELS*)dtcc_check_bfr( dtccP,sizeof(DTCC_LABELS));
		const char* empty = "";
		const char	*nameP = ( name ? name : empty ),
			 		*abbrvP = ( abbrv ? abbrv : empty ),
			 		*otherP = ( other ? other : empty );
		if( !lblP )
			return NULL;
		else
		{	lblP->l1 = strlen( nameP )+1;
			if( lblP->l1 < max )
				strcpy( _label_nameP(lblP), nameP );
			else
			{	lblP->l1 = max;
				strncpy( _label_nameP(lblP), nameP, max-1 );
				abbrvP = otherP = empty;
			}
			max -= lblP->l1;
			lblP->l2 = strlen( abbrvP )+1;
			if( lblP->l2 < max )
				strcpy( _label_abbrvP(lblP), abbrvP );
			else
			{	lblP->l2 = max;
				strncpy( _label_abbrvP(lblP), abbrvP, max-1 );
				otherP = empty;
			}
			max -= lblP->l2;
			lblP->l3 = strlen( otherP )+1;
			if( lblP->l3 < max )
				strcpy( _label_otherP(lblP), otherP );
			else
			{	lblP->l3 = max;
				strncpy( _label_otherP(lblP), otherP, max-1 );
			}
		}
		lblP->labels[255] = 0;
		lblP->valid = valid;
		lblP->type = type;
		return lblP;
	}

short dtcc_make_hdatum_name( DATUM_PARAMS* csP, char* nm )
	{	if( csP && nm )
		{	const short* sg = dtcc_datum_sigmasP(csP);
			const short* sh = dtcc_datum_sigmasP(csP);
			const char* eab = dtcc_nameP(dtcc_datum_ellipsoidP(csP));
			sprintf( nm, "%s_%d%d%d%d%d%d", 
				eab, sh[0],sh[1],sh[2],sg[0],sg[1],sg[2] );
	    	return 0;
		}
		else
			return -1;
	}

static short _check_itcoordextent( COORD_EXTENTS* ceP )
	{	int status = VMAPOK;
		if( dtcc_extents_min_lat(ceP) != DOUBLE_NULL &&
			dtcc_extents_max_lat(ceP) != DOUBLE_NULL &&
			dtcc_extents_min_lon(ceP) != DOUBLE_NULL &&
			dtcc_extents_max_lon(ceP) != DOUBLE_NULL )
		{	if( dtcc_extents_min_lat(ceP) > dtcc_extents_max_lat(ceP) )
				status = INVALID_DATA;
			else
			if( dtcc_extents_min_lon(ceP) > dtcc_extents_max_lon(ceP) )
				status = INVALID_DATA;
		}
		else
			status = 1;
			
		if( dtcc_extents_min_north(ceP) != DOUBLE_NULL &&
			dtcc_extents_max_north(ceP) != DOUBLE_NULL &&
			dtcc_extents_min_east(ceP) != DOUBLE_NULL  &&
			dtcc_extents_max_east(ceP) != DOUBLE_NULL  )
		{	if( dtcc_extents_min_north(ceP) > dtcc_extents_max_north(ceP) )
				status = INVALID_DATA;
			else
			if( dtcc_extents_min_east(ceP) > dtcc_extents_max_east(ceP) )
				status = INVALID_DATA;
		}
		else
			status = 1;
		if( status >= VMAPOK ) {  status = VMAPOK; _set_valid(ceP);	 }
		return status;  
	}

void dtcc_trim_white(	char* strP	)
	{	int k = strlen(strP);
		while( --k >= 0 )
			if( !isspace(strP[k]) )
				return;
			else
				strP[k] = 0;
		return; 
	}

static const struct _PROJECTION_PARS* _find_pro( const DTCC_LABELS* lblP )
{	if( lblP )
	{	int k = dtcc_projection_count();
		while( --k >= 0 )
			if( strcmp(dtcc_nameP(lblP), PROJECTION_LIST[k].name ) == 0 || 
				strcmp(dtcc_nameP(lblP), PROJECTION_LIST[k].abbrv) == 0  ) 
				return PROJECTION_LIST+k;
	}
	return NULL;
}

short dtcc_initialize_system_list( const char* fname ) 
	{	short xpr;
		FILE* fp = _scan_list_hdr( &slist, _check_system_name(fname)  );
		double dbl = 9.0;
		if( !fp )
			return FILE_NOT_FOUND;
		else
		{	int i, n = _list_total(slist);
			_list_dataP(slist) = 
				(COORD_SYS*)dtcc_allocate_ptr( (n+1)*sizeof(COORD_SYS));
			if( !_list_dataP(slist) )
			{	dtcc_delete_datum_list(&slist);
				fclose(fp);
				return MEMORY_ALLOCATION_FAILURE;
			}
			else
			{	COORD_SYS* csP = (COORD_SYS*)_list_dataP(slist);
				for( i=0; i<n; i++, csP++ )
				{   xpr = dtcc_scan_system( csP,fp );
					if( dtcc_system_userdef(csP) )
					{	_list_type2(slist)++;
						_list_type1(slist)--;
					}
				}
				fclose(fp);
				_list_added(slist) = 0;
				_list_modified(slist) = 0;
				return VMAPOK;
			}
		}
	}

/* Number of systems in list */
short _slist_count( int k, _DTCC_LIST* listP )
	{	if( listP == NULL ) listP = slist;
		if( !listP )	
			return 0;
		else
		if( k == 1 )
			return	_list_type1( listP );
		else
		if( k == 2 )
			return _list_type2( listP ) - _list_modified(listP);
		else
			return _list_type1( listP ) + _list_type2( listP );
	}

short dtcc_set_system_units( COORD_SYS* csP, short mode, double mscale, const char* in1, const char* in2, const char* in3 )
	{	if( in3 ) _SYSTEM_DIMENSION(csP) = ( in3[0]==0 ? 2 : 3 );
		return dtcc_set_coord3_units( _SYSTEM_UNITSP(csP),mode,mscale,in1,in2,in3);
	}

int dtcc_clear_label( void* dtccP )
	{	DTCC_LABELS* idP = (DTCC_LABELS*)dtccP;
		if( idP )
		{	_label_nameP(idP)[0] = 0; _label_l1(idP) = 1;
			_label_nameP(idP)[0] = 0; _label_l2(idP) = 1;
			_label_nameP(idP)[0] = 0; _label_l3(idP) = 1;
			return VMAPOK;
		}
		else
			return ILLEGAL_ARG;
	}

short dtcc_reset_default_ellipsoid(	ELLIPS_PARAMS* elP  )
    {	return _update_ellipsoid( elP, "WGS 84","WE",	
    			6378137.000,298.257223563, _TYPE1_LIST );  
    }

short dtcc_set_origin( COORD_ORIGIN* coP,const COORD3* gorg,const COORD3* morg,const COORD3* sorg,double worg, double azm  )
    {   if( coP )
        {   short k;
        	_set_invalid(coP);
        	dtcc_set_origin_gridSW( coP, 0.0, 0.0, DOUBLE_NULL, SHORT_NULL );
        	if( morg )
               dtcc_set_origin_grid
                    ( coP, dtcc_coord3_north(morg), dtcc_coord3_east(morg), 
                                DOUBLE_NULL, dtcc_coord3_zone(morg) );
           else
               dtcc_set_origin_grid( coP, 0.0, 0.0, DOUBLE_NULL, SHORT_NULL );
        
           if( gorg )
               dtcc_set_origin_geo
                    ( coP, dtcc_coord3_lat(gorg), dtcc_coord3_lon(gorg), 
                                DOUBLE_NULL );
           else
               dtcc_set_origin_geo( coP, 0.0, 0.0, DOUBLE_NULL );
        
           if( sorg  )
               dtcc_set_origin_scale
                    ( coP, dtcc_coord3_x(sorg), dtcc_coord3_y(sorg), 
                                dtcc_coord3_z(sorg) );
           else
               dtcc_set_origin_scale( coP, 1.0, 1.0, 1.0 );
               
           dtcc_set_origin_azimuth( coP, azm );
           dtcc_set_origin_convergence( coP, worg );
           
           k = _check_itcoordorigin(coP);
           if( k == VMAPOK ) _set_valid(coP);
           return k;
       }
        return ILLEGAL_ARG;
    }

short dtcc_set_angle( ANGLE* aP, double v )
    {   dtcc_angle_value(aP) = v;
    	if( v > DOUBLE_NULL )
    	{	v *= RADDEG;
        	dtcc_angle_sin(aP) = sin(v);
        	dtcc_angle_cos(aP) = cos(v);
        }
        else
        {	dtcc_angle_sin(aP) = DOUBLE_NULL;
        	dtcc_angle_cos(aP) = DOUBLE_NULL;
        }
        return VMAPOK;         
    }
 
short AlbersEngine( const PROJ_PARAMS* proP,   short mode, const COORD3* inptP,  COORD3* toP   )
/* Snyder, John P., MAP PROJECTIONS, A WORKING MANUAL. */
/* US Government Printing Office 1987 */
/* Albers Projection, p 38-47.  */    
{   short status = 0;
	double x,y, phi,lam;
 	
    if( mode == DTCC_PROJECTION_SETUP || PROJ_ID(dtcc_projection_constantsP(proP)) == 0 )
	{	COORD_ORIGIN* coP = (COORD_ORIGIN*)(&proP->origin);
		double lat0 = dtcc_projection_lat0(proP);
		double lon0 = dtcc_projection_lon0(proP);
		double k0 =	dtcc_projection_K0(proP);
		
		double fn =  dtcc_projection_fn(proP);
		double fe =  dtcc_projection_fe(proP);
		double fw =  dtcc_projection_feSW(proP);
		double fs =  dtcc_projection_fnSW(proP);

		double lat1 =  dtcc_projection_lower_std_parallel(proP);
		double lat2 =  dtcc_projection_upper_std_parallel(proP);

		if( fn == DOUBLE_NULL || fe == DOUBLE_NULL ) 
		{	fn = ( fn == DOUBLE_NULL ? 0.0 : fn );
        	fe = ( fe == DOUBLE_NULL ? 0.0 : fe );
        	dtcc_set_origin_grid( coP, fe,fn, DOUBLE_NULL, SHORT_NULL ); 
        }
		if( fs == DOUBLE_NULL || fw == DOUBLE_NULL ) 
		{	fs = ( fs == DOUBLE_NULL ? 0.0 : fs );
        	fw = ( fw == DOUBLE_NULL ? 0.0 : fw );
        	dtcc_set_origin_gridSW( coP, fw,fs, DOUBLE_NULL, SHORT_NULL ); 
        }
		
		if( lat1 == DOUBLE_NULL || lat2 == DOUBLE_NULL ) 
		{	lat1 = ( lat1 == DOUBLE_NULL ? 23.0 : lat1 );
        	lat2 = ( lat2 == DOUBLE_NULL ? 37.0 : lat2 );
        	dtcc_set_projection_std_parallels( (PROJ_PARAMS*)proP, lat1,lat2 ); 
        }
		
	/*	if( k0 == DOUBLE_NULL ) 
		{	k0 = 1.0;
        	dtcc_set_origin_scale( coP, k0, DOUBLE_NULL, DOUBLE_NULL ); 
        }
	*/
		if( lat0 == DOUBLE_NULL || lon0 == DOUBLE_NULL ) 
		{	lat0 = 37.0; lon0 = -100.0;
 			dtcc_set_origin_geo( coP, lat0, lon0, DOUBLE_NULL ); 
        }
		init_albers_equal_area( (void*)dtcc_projection_constantsP(proP),NULL,
					dtcc_projection_axis(proP), 
            		dtcc_projection_rflat(proP), lat0, lon0, lat1,lat2 );
		if( DTCC_PROJECTION_SETUP == mode ) return VMAPOK;
    }       	
	            			     	   
      			     	   
	switch (mode)
    {   case DTCC_PROJECTION_FORWARD:
           	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = geo_to_albers_equal_area
           		( dtcc_projection_constantsP(proP), phi,lam, &x, &y );
           	if( status >= VMAPOK )
			{	dtcc_add_origin( &proP->origin, &x, &y );	
				dtcc_set_coord3_grid
					( toP, x,y, dtcc_coord3_elv(inptP), SHORT_NULL );
			}
			break;              

        case DTCC_PROJECTION_INVERSE:
			x = dtcc_coord3_east(inptP);
			y = dtcc_coord3_north(inptP); 
         	dtcc_sub_origin( &proP->origin, &x, &y );	
			status = albers_equal_area_to_geo
				( dtcc_projection_constantsP(proP), x,y, &phi,&lam );  
            dtcc_set_coord3_geo
				( toP, phi,lam, dtcc_coord3_hgt(inptP) );
			break;
        
        case DTCC_PROJECTION_SCALEFACTOR:
        	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = albers_equal_area_scale
           		( dtcc_projection_constantsP(proP), phi,lam, &x, &y ); 
            dtcc_set_coord3_cart( toP, x,y, 1.0 );
			break;
		
    	case DTCC_PROJECTION_LIMITS:
			albers_equal_area_limits( dtcc_projection_constantsP(proP), &y,&x, &phi,&lam );
			dtcc_set_coord3_geo( toP, y,x,DOUBLE_NULL );
			dtcc_set_coord3_geo( toP+1, phi,lam,DOUBLE_NULL );
			status = 0;
			break;
		default:
            status = UNKNOWN_ITEM_REQUESTED;
            break;
    }
    status = map_projection_status(status);
    return status;
}

short CylindricalEqualAreaEngine ( const PROJ_PARAMS* proP,  short mode, const COORD3* inptP,  COORD3* toP )
/* Snyder, John P., MAP PROJECTIONS, A WORKING MANUAL. */
/* US Government Printing Office 1987 */
/* cylindrical equal area projection, p 76-85.  */    
{   short status = 0;
	double x,y, phi,lam;
    
    if( mode == DTCC_PROJECTION_SETUP || PROJ_ID(dtcc_projection_constantsP(proP)) == 0 )
	{	COORD_ORIGIN* coP = (COORD_ORIGIN*)(&proP->origin);
		double lat0 = dtcc_projection_lat0(proP);
		double lon0 = dtcc_projection_lon0(proP);
		double k0 =	dtcc_projection_K0(proP);
		
		double fn =  dtcc_projection_fn(proP);
		double fe =  dtcc_projection_fe(proP);
		double fw =  dtcc_projection_feSW(proP);
		double fs =  dtcc_projection_fnSW(proP);

		if( fn == DOUBLE_NULL || fe == DOUBLE_NULL ) 
		{	fn = ( fn == DOUBLE_NULL ? 0.0 : fn );
        	fe = ( fe == DOUBLE_NULL ? 0.0 : fe );
        	dtcc_set_origin_grid( coP, fe,fn, DOUBLE_NULL, SHORT_NULL ); 
        }
		if( fs == DOUBLE_NULL || fw == DOUBLE_NULL ) 
		{	fs = ( fs == DOUBLE_NULL ? 0.0 : fs );
        	fw = ( fw == DOUBLE_NULL ? 0.0 : fw );
        	dtcc_set_origin_gridSW( coP, fw,fs, DOUBLE_NULL, SHORT_NULL ); 
        }
		
        dtcc_set_projection_std_parallels( (PROJ_PARAMS*)proP, DOUBLE_NULL,DOUBLE_NULL ); 
		
		if( k0 == DOUBLE_NULL ) 
		{	k0 = 1.0;
        	dtcc_set_origin_scale( coP, k0, DOUBLE_NULL, DOUBLE_NULL ); 
        }

		if( lat0 == DOUBLE_NULL || lon0 == DOUBLE_NULL ) 
		{	lat0 = 0.0; lon0 = 0.0;
 			dtcc_set_origin_geo( coP, lat0, lon0, DOUBLE_NULL ); 
        }
		init_cylindrical_equal_area( (void*)dtcc_projection_constantsP(proP),NULL,
					dtcc_projection_axis(proP), 
            		dtcc_projection_rflat(proP), lat0, lon0, k0 );
		if( DTCC_PROJECTION_SETUP == mode ) return VMAPOK;
    }       	
            			     	   
	switch (mode)
    {   case DTCC_PROJECTION_FORWARD:
           	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = geo_to_cylindrical_equal_area
           		( dtcc_projection_constantsP(proP), phi,lam, &x, &y );
           	if( status >= VMAPOK )
			{	dtcc_add_origin( &proP->origin, &x, &y );	
				dtcc_set_coord3_grid
					( toP, x,y, dtcc_coord3_elv(inptP), SHORT_NULL );
			}
			break;              

        case DTCC_PROJECTION_INVERSE:
			x = dtcc_coord3_east(inptP);
			y = dtcc_coord3_north(inptP); 
         	dtcc_sub_origin( &proP->origin, &x, &y );	
			status = cylindrical_equal_area_to_geo
				( dtcc_projection_constantsP(proP), x,y, &phi,&lam );  
            dtcc_set_coord3_geo
				( toP, phi,lam, dtcc_coord3_hgt(inptP) );
			break;
        
       /*
        case DTCC_PROJECTION_SCALEFACTOR:
        	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = mercator_scale( proP->cnstsP, phi,lam, &x, &y ); 
            dtcc_set_coord3_cart( toP, x,y, 1.0 );
			break;
		*/
    	case DTCC_PROJECTION_LIMITS:
			cylindrical_equal_area_limits
				(dtcc_projection_constantsP(proP), &y,&x, &phi,&lam );
			dtcc_set_coord3_geo( toP, y,x,DOUBLE_NULL );
			dtcc_set_coord3_geo( toP+1, phi,lam,DOUBLE_NULL );
			status = 0;
			break;
		default:
            status = UNKNOWN_ITEM_REQUESTED;
            break;
    }
    status = map_projection_status(status);
    return status;
}

short LambertConformalConicEngine( const PROJ_PARAMS* proP, short mode, const COORD3* inptP, COORD3* toP  )
/* Snyder, John P., MAP PROJECTIONS, A WORKING MANUAL. */
/* US Government Printing Office 1987 */
/* LambertConformalConic Projection, p 38-47.  */    
{   short status = 0;
	double x,y, phi,lam;
    
    if( mode == DTCC_PROJECTION_SETUP || PROJ_ID(dtcc_projection_constantsP(proP)) == 0 )
	{	COORD_ORIGIN* coP = (COORD_ORIGIN*)(&proP->origin);
		double lat0 = dtcc_projection_lat0(proP);
		double lon0 = dtcc_projection_lon0(proP);
		double k0 =	dtcc_projection_K0(proP);
		
		double fn =  dtcc_projection_fn(proP);
		double fe =  dtcc_projection_fe(proP);
		double fw =  dtcc_projection_feSW(proP);
		double fs =  dtcc_projection_fnSW(proP);

		double lat1 =  dtcc_projection_lower_std_parallel(proP);
		double lat2 =  dtcc_projection_upper_std_parallel(proP);

		if( fn == DOUBLE_NULL || fe == DOUBLE_NULL ) 
		{	fn = ( fn == DOUBLE_NULL ? 0.0 : fn );
        	fe = ( fe == DOUBLE_NULL ? 0.0 : fe );
        	dtcc_set_origin_grid( coP, fe,fn, DOUBLE_NULL, SHORT_NULL ); 
        }
		if( fs == DOUBLE_NULL || fw == DOUBLE_NULL ) 
		{	fs = ( fs == DOUBLE_NULL ? 0.0 : fs );
        	fw = ( fw == DOUBLE_NULL ? 0.0 : fw );
        	dtcc_set_origin_gridSW( coP, fw,fs, DOUBLE_NULL, SHORT_NULL ); 
        }
		
		if( lat1 == DOUBLE_NULL || lat2 == DOUBLE_NULL ) 
		{	lat1 = ( lat1 == DOUBLE_NULL ? 23.0 : lat1 );
        	lat2 = ( lat2 == DOUBLE_NULL ? 37.0 : lat2 );
        	dtcc_set_projection_std_parallels( (PROJ_PARAMS*)proP, lat1,lat2 ); 
        }
		
	/*	if( k0 == DOUBLE_NULL ) 
		{	k0 = 1.0;
        	dtcc_set_origin_scale( coP, k0, DOUBLE_NULL, DOUBLE_NULL ); 
        }
	*/
		if( lat0 == DOUBLE_NULL || lon0 == DOUBLE_NULL ) 
		{	lat0 = 37.0; lon0 = -100.0;
 			dtcc_set_origin_geo( coP, lat0, lon0, DOUBLE_NULL ); 
        }
		init_lambert_conformal_conic( (void*)dtcc_projection_constantsP(proP),NULL,
					dtcc_projection_axis(proP), 
            		dtcc_projection_rflat(proP), lat0, lon0, lat1,lat2 );
		if( DTCC_PROJECTION_SETUP == mode ) return VMAPOK;
    }       	
	            			     	   
	switch (mode)
    {   case DTCC_PROJECTION_FORWARD:
           	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = geo_to_lambert_conformal_conic
           		( dtcc_projection_constantsP(proP), phi,lam, &x, &y );
           	if( status >= VMAPOK )
			{	dtcc_add_origin( &proP->origin, &x, &y );	
				dtcc_set_coord3_grid
					( toP, x,y, dtcc_coord3_elv(inptP), SHORT_NULL );
			}
			break;              

        case DTCC_PROJECTION_INVERSE:
			x = dtcc_coord3_east(inptP);
			y = dtcc_coord3_north(inptP); 
         	dtcc_sub_origin( &proP->origin, &x, &y );	
			status = lambert_conformal_conic_to_geo
				( dtcc_projection_constantsP(proP), x,y, &phi,&lam );  
            dtcc_set_coord3_geo
				( toP, phi,lam, dtcc_coord3_hgt(inptP) );
			break;
        
        case DTCC_PROJECTION_SCALEFACTOR:
        	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = lambert_conformal_conic_scale
           		( dtcc_projection_constantsP(proP), phi,lam, &x, &y ); 
            dtcc_set_coord3_cart( toP, x,y, 1.0 );
			break;
		
    	case DTCC_PROJECTION_LIMITS:
			lambert_conformal_conic_limits
				( dtcc_projection_constantsP(proP), &y,&x, &phi,&lam );
			dtcc_set_coord3_geo( toP, y,x,DOUBLE_NULL );
			dtcc_set_coord3_geo( toP+1, phi,lam,DOUBLE_NULL );
			status = 0;
			break;

		default:
            status = UNKNOWN_ITEM_REQUESTED;
            break;
    }
    status = map_projection_status(status);
    return status;
}

int lambert_conformal_conic_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon )
	{	if( !cnstsP || !mnlat || !mxlat || !mnlon || !mxlon)
			return -1;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			*mnlat = SLIM; *mxlat = NLIM; 
			*mnlon = WLIM; *mxlon = ELIM;
			return 0;
		}
	}

short LambertEqualAreaEngine( const PROJ_PARAMS* proP,short mode,const COORD3* inptP,COORD3* toP  )
/* Snyder, John P., MAP PROJECTIONS, A WORKING MANUAL. */
/* US Government Printing Office 1987 */
/* LambertEqualArea Projection, p 38-47.  */    
{   short status = 0;
	double x,y, phi,lam;
    
    if( mode == DTCC_PROJECTION_SETUP || PROJ_ID(dtcc_projection_constantsP(proP)) == 0 )
	{	COORD_ORIGIN* coP = (COORD_ORIGIN*)(&proP->origin);
		double lat0 = dtcc_projection_lat0(proP);
		double lon0 = dtcc_projection_lon0(proP);
		double k0 =	dtcc_projection_K0(proP);
		
		double fn =  dtcc_projection_fn(proP);
		double fe =  dtcc_projection_fe(proP);
		double fw =  dtcc_projection_feSW(proP);
		double fs =  dtcc_projection_fnSW(proP);

		double lat1 =  dtcc_projection_lower_std_parallel(proP);
		double lat2 =  dtcc_projection_upper_std_parallel(proP);
		
		if( fn == DOUBLE_NULL || fe == DOUBLE_NULL ) 
		{	fn = ( fn == DOUBLE_NULL ? 0.0 : fn );
        	fe = ( fe == DOUBLE_NULL ? 0.0 : fe );
        	dtcc_set_origin_grid( coP, fe,fn, DOUBLE_NULL, SHORT_NULL ); 
        }
		if( fs == DOUBLE_NULL || fw == DOUBLE_NULL ) 
		{	fs = ( fs == DOUBLE_NULL ? 0.0 : fs );
        	fw = ( fw == DOUBLE_NULL ? 0.0 : fw );
        	dtcc_set_origin_gridSW( coP, fw,fs, DOUBLE_NULL, SHORT_NULL ); 
        }
		
        if( lat1 != DOUBLE_NULL || lat2 != DOUBLE_NULL )
        	dtcc_set_projection_std_parallels( (PROJ_PARAMS*)proP, DOUBLE_NULL,DOUBLE_NULL ); 
		
		if( k0 != DOUBLE_NULL ) 
		{	k0 = DOUBLE_NULL;
        	dtcc_set_origin_scale( coP, k0, DOUBLE_NULL, DOUBLE_NULL ); 
        }
	
		if( lat0 == DOUBLE_NULL || lon0 == DOUBLE_NULL ) 
		{	lat0 = 90.0; lon0 = 0.0;
 			dtcc_set_origin_geo( coP, lat0, lon0, DOUBLE_NULL ); 
        }
		init_lambert_equal_area( (void*)dtcc_projection_constantsP(proP),NULL,
					dtcc_projection_axis(proP), 
            		dtcc_projection_rflat(proP), lat0, lon0 );
		if( DTCC_PROJECTION_SETUP == mode ) return VMAPOK;
    }       	
            			     	   
	switch (mode)
    {   case DTCC_PROJECTION_FORWARD:
           	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = geo_to_lambert_equal_area
           		( dtcc_projection_constantsP(proP), phi,lam, &x, &y );
           	if( status >= VMAPOK )
			{	dtcc_add_origin( &proP->origin, &x, &y );	
				dtcc_set_coord3_grid
					( toP, x,y, dtcc_coord3_elv(inptP), SHORT_NULL );
			}
			break;              

        case DTCC_PROJECTION_INVERSE:
			x = dtcc_coord3_east(inptP);
			y = dtcc_coord3_north(inptP); 
         	dtcc_sub_origin( &proP->origin, &x, &y );	
			status = lambert_equal_area_to_geo
				( dtcc_projection_constantsP(proP), x,y, &phi,&lam );  
            dtcc_set_coord3_geo
				( toP, phi,lam, dtcc_coord3_hgt(inptP) );
			break;
        
        case DTCC_PROJECTION_SCALEFACTOR:
        	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = lambert_equal_area_scale
           		( dtcc_projection_constantsP(proP), phi,lam, &x, &y ); 
            dtcc_set_coord3_cart( toP, x,y, 1.0 );
			break;
		
    	case DTCC_PROJECTION_LIMITS:
			lambert_equal_area_limits
				( dtcc_projection_constantsP(proP), &y,&x, &phi,&lam );
			dtcc_set_coord3_geo( toP, y,x,DOUBLE_NULL );
			dtcc_set_coord3_geo( toP+1, phi,lam,DOUBLE_NULL );
			status = 0;
			break;
		default:
            status = UNKNOWN_ITEM_REQUESTED;
            break;
    }
    status = map_projection_status(status);
    return status;
}

short MercatorEngine( const PROJ_PARAMS* proP,short mode, const COORD3* inptP,COORD3* toP )
/* Snyder, John P., MAP PROJECTIONS, A WORKING MANUAL. */
/* US Government Printing Office 1987 */
/* Mercator Projection, p 38-47.  */    
{   short status = 0;
	double x,y, phi,lam;
    
    if( mode == DTCC_PROJECTION_SETUP || PROJ_ID(dtcc_projection_constantsP(proP)) == 0 )
	{	COORD_ORIGIN* coP = (COORD_ORIGIN*)(&proP->origin);
		double lat0 = dtcc_projection_lat0(proP);
		double lon0 = dtcc_projection_lon0(proP);
		double k0 =	dtcc_projection_K0(proP);
		
		double fn =  dtcc_projection_fn(proP);
		double fe =  dtcc_projection_fe(proP);
		double fw =  dtcc_projection_feSW(proP);
		double fs =  dtcc_projection_fnSW(proP);

		double lat1 =  dtcc_projection_lower_std_parallel(proP);
		double lat2 =  dtcc_projection_upper_std_parallel(proP);

		if( fn == DOUBLE_NULL || fe == DOUBLE_NULL ) 
		{	fn = ( fn == DOUBLE_NULL ? 0.0 : fn );
        	fe = ( fe == DOUBLE_NULL ? 0.0 : fe );
        	dtcc_set_origin_grid( coP, fe,fn, DOUBLE_NULL, SHORT_NULL ); 
        }
		if( fs == DOUBLE_NULL || fw == DOUBLE_NULL ) 
		{	fs = ( fs == DOUBLE_NULL ? 0.0 : fs );
        	fw = ( fw == DOUBLE_NULL ? 0.0 : fw );
        	dtcc_set_origin_gridSW( coP, fw,fs, DOUBLE_NULL, SHORT_NULL ); 
        }
		
		if( lat1 != DOUBLE_NULL || lat2 != DOUBLE_NULL ) 
			dtcc_set_projection_std_parallels( (PROJ_PARAMS*)proP, DOUBLE_NULL,DOUBLE_NULL ); 
		
		if( k0 == DOUBLE_NULL ) 
		{	k0 = 1.0;
			dtcc_set_origin_scale( coP, 1.0, DOUBLE_NULL, DOUBLE_NULL ); 
		}
		if( lat0 == DOUBLE_NULL || lon0 == DOUBLE_NULL ) 
		{	lat0 = 0.0; lon0 = 0.0;
 			dtcc_set_origin_geo( coP, lat0, lon0, DOUBLE_NULL ); 
        }
		init_mercator( (void*)dtcc_projection_constantsP(proP),NULL,
					dtcc_projection_axis(proP), 
            		dtcc_projection_rflat(proP), lon0 );
		if( DTCC_PROJECTION_SETUP == mode ) return VMAPOK;
    }       	
            			     	   
	switch (mode)
    {   case DTCC_PROJECTION_FORWARD:
           	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = geo_to_mercator
           		( dtcc_projection_constantsP(proP), phi,lam, &x, &y );
           	if( status >= VMAPOK )
			{	dtcc_add_origin( &proP->origin, &x, &y );	
				dtcc_set_coord3_grid
					( toP, x,y, dtcc_coord3_elv(inptP), SHORT_NULL );
			}
			break;              

        case DTCC_PROJECTION_INVERSE:
			x = dtcc_coord3_east(inptP);
			y = dtcc_coord3_north(inptP); 
         	dtcc_sub_origin( &proP->origin, &x, &y );	
			status = mercator_to_geo
				( dtcc_projection_constantsP(proP), x,y, &phi,&lam );  
            dtcc_set_coord3_geo
				( toP, phi,lam, dtcc_coord3_hgt(inptP) );
			break;
        
        case DTCC_PROJECTION_SCALEFACTOR:
        	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = mercator_scale
           		( dtcc_projection_constantsP(proP), phi,lam, &x, &y ); 
            dtcc_set_coord3_cart( toP, x,y, 1.0 );
			break;
		
    	case DTCC_PROJECTION_LIMITS:
			mercator_limits
				( dtcc_projection_constantsP(proP), &y,&x, &phi,&lam );
			dtcc_set_coord3_geo( toP, y,x,DOUBLE_NULL );
			dtcc_set_coord3_geo( toP+1, phi,lam,DOUBLE_NULL );
			status = 0;
			break;
		default:
            status = UNKNOWN_ITEM_REQUESTED;
            break;
    }
    status = map_projection_status(status);
    return status;
}

short OrthographicEngine( const PROJ_PARAMS* proP,short mode,const COORD3* inptP,COORD3* toP   )
/* Snyder, John P., MAP PROJECTIONS, A WORKING MANUAL. */
/* US Government Printing Office 1987 */
/* Orthographic Projection, p 38-47.  */    
{   short status = 0;
	double x,y, phi,lam;
    
    if( mode == DTCC_PROJECTION_SETUP || PROJ_ID(dtcc_projection_constantsP(proP)) == 0 )
	{	COORD_ORIGIN* coP = (COORD_ORIGIN*)(&proP->origin);
		double lat0 = dtcc_projection_lat0(proP);
		double lon0 = dtcc_projection_lon0(proP);
		double k0 =	dtcc_projection_K0(proP);
		
		double fn =  dtcc_projection_fn(proP);
		double fe =  dtcc_projection_fe(proP);
		double fw =  dtcc_projection_feSW(proP);
		double fs =  dtcc_projection_fnSW(proP);

		double lat1 =  dtcc_projection_lower_std_parallel(proP);
		double lat2 =  dtcc_projection_upper_std_parallel(proP);
		
		if( fn == DOUBLE_NULL || fe == DOUBLE_NULL ) 
		{	fn = ( fn == DOUBLE_NULL ? 0.0 : fn );
        	fe = ( fe == DOUBLE_NULL ? 0.0 : fe );
        	dtcc_set_origin_grid( coP, fe,fn, DOUBLE_NULL, SHORT_NULL ); 
        }
		if( fs == DOUBLE_NULL || fw == DOUBLE_NULL ) 
		{	fs = ( fs == DOUBLE_NULL ? 0.0 : fs );
        	fw = ( fw == DOUBLE_NULL ? 0.0 : fw );
        	dtcc_set_origin_gridSW( coP, fw,fs, DOUBLE_NULL, SHORT_NULL ); 
        }
		
        if( lat1 != DOUBLE_NULL || lat2 != DOUBLE_NULL )
        	dtcc_set_projection_std_parallels( (PROJ_PARAMS*)proP, DOUBLE_NULL,DOUBLE_NULL ); 
		
		if( k0 != DOUBLE_NULL ) 
		{	k0 = DOUBLE_NULL;
        	dtcc_set_origin_scale( coP, k0, DOUBLE_NULL, DOUBLE_NULL ); 
        }
	
		if( lat0 == DOUBLE_NULL || lon0 == DOUBLE_NULL ) 
		{	lat0 = 90.0; lon0 = 0.0;
 			dtcc_set_origin_geo( coP, lat0, lon0, DOUBLE_NULL ); 
        }
		init_orthographic( (void*)dtcc_projection_constantsP(proP),NULL,
					dtcc_projection_axis(proP), 
            		dtcc_projection_rflat(proP), lat0, lon0 );
		if( DTCC_PROJECTION_SETUP == mode ) return VMAPOK;
    }       	
            			     	   
	switch (mode)
    {   case DTCC_PROJECTION_FORWARD:
           	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = geo_to_orthographic( dtcc_projection_constantsP(proP), phi,lam, &x, &y );
           	if( status == VMAPOK )
			{	dtcc_add_origin( &proP->origin, &x, &y );	
				dtcc_set_coord3_grid
					( toP, x,y, dtcc_coord3_elv(inptP), SHORT_NULL );
			}
			else
				status = ( status == -1 ? CONSTANTS_NOT_INITIALIZED : DATA_OUT_OF_RANGE );
			break;              

        case DTCC_PROJECTION_INVERSE:
			x = dtcc_coord3_east(inptP);
			y = dtcc_coord3_north(inptP); 
         	dtcc_sub_origin( &proP->origin, &x, &y );	
			status = orthographic_to_geo( dtcc_projection_constantsP(proP), x,y, &phi,&lam );  
            dtcc_set_coord3_geo
				( toP, phi,lam, dtcc_coord3_hgt(inptP) );
			if( status != 0 )
				status = ( status == -1 ? CONSTANTS_NOT_INITIALIZED : DATA_OUT_OF_RANGE );
			break;
        
        case DTCC_PROJECTION_SCALEFACTOR:
        	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = orthographic_scale( dtcc_projection_constantsP(proP), phi,lam, &x, &y ); 
            dtcc_set_coord3_cart( toP, x,y, 1.0 );
			break;
		
     	case DTCC_PROJECTION_LIMITS:
			orthographic_limits( dtcc_projection_constantsP(proP), &y,&x, &phi,&lam );
			dtcc_set_coord3_geo( toP, y,x,DOUBLE_NULL );
			dtcc_set_coord3_geo( toP+1, phi,lam,DOUBLE_NULL );
			status = 0;
			break;

		default:
            status = UNKNOWN_ITEM_REQUESTED;
            break;
    }
    return status;
}


short RectangularEngine( const PROJ_PARAMS* proP,short mode,const COORD3* inptP,COORD3* toP )
/* Snyder, John P., MAP PROJECTIONS, A WORKING MANUAL. */
/* US Government Printing Office 1987 */
/* Rectangular Projection, p 38-47.  */    
{   short status = 0;
	double x,y, phi,lam;
    
    if( mode == DTCC_PROJECTION_SETUP || PROJ_ID(dtcc_projection_constantsP(proP)) == 0 )
	{	COORD_ORIGIN* coP = (COORD_ORIGIN*)(&proP->origin);
		double lat0 = dtcc_projection_lat0(proP);
		double lon0 = dtcc_projection_lon0(proP);
		double k0 =	dtcc_projection_K0(proP);
		
		double fn =  dtcc_projection_fn(proP);
		double fe =  dtcc_projection_fe(proP);
		double fw =  dtcc_projection_feSW(proP);
		double fs =  dtcc_projection_fnSW(proP);

		double lat1 =  dtcc_projection_lower_std_parallel(proP);
		double lat2 =  dtcc_projection_upper_std_parallel(proP);
		
		if( fn == DOUBLE_NULL || fe == DOUBLE_NULL ) 
		{	fn = ( fn == DOUBLE_NULL ? 0.0 : fn );
        	fe = ( fe == DOUBLE_NULL ? 0.0 : fe );
        	dtcc_set_origin_grid( coP, fe,fn, DOUBLE_NULL, SHORT_NULL ); 
        }
		if( fs == DOUBLE_NULL || fw == DOUBLE_NULL ) 
		{	fs = ( fs == DOUBLE_NULL ? 0.0 : fs );
        	fw = ( fw == DOUBLE_NULL ? 0.0 : fw );
        	dtcc_set_origin_gridSW( coP, fw,fs, DOUBLE_NULL, SHORT_NULL ); 
        }
		
        if( lat1 != DOUBLE_NULL || lat2 != DOUBLE_NULL )
        	dtcc_set_projection_std_parallels( (PROJ_PARAMS*)proP, DOUBLE_NULL,DOUBLE_NULL ); 
		
		if( k0 != DOUBLE_NULL ) 
		{	k0 = DOUBLE_NULL;
        	dtcc_set_origin_scale( coP, k0, DOUBLE_NULL, DOUBLE_NULL ); 
        }
	
		if( lat0 == DOUBLE_NULL || lon0 == DOUBLE_NULL ) 
		{	lat0 = 0.0; lon0 = 0.0;
 			dtcc_set_origin_geo( coP, lat0, lon0, DOUBLE_NULL ); 
        }
		init_rectangular( (void*)dtcc_projection_constantsP(proP),NULL,
					dtcc_projection_axis(proP), 
            		dtcc_projection_rflat(proP), lat0, lon0 );
		if( DTCC_PROJECTION_SETUP == mode ) return VMAPOK;
    }       	
           			     	   
	switch (mode)
    {   case DTCC_PROJECTION_FORWARD:
           	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = geo_to_rectangular( dtcc_projection_constantsP(proP), phi,lam, &x, &y );
           	if( status >= VMAPOK )
			{	dtcc_add_origin( &proP->origin, &x, &y );	
				dtcc_set_coord3_grid
					( toP, x,y, dtcc_coord3_elv(inptP), SHORT_NULL );
			}
			break;              

        case DTCC_PROJECTION_INVERSE:
			x = dtcc_coord3_east(inptP);
			y = dtcc_coord3_north(inptP); 
         	dtcc_sub_origin( &proP->origin, &x, &y );	
			status = rectangular_to_geo( dtcc_projection_constantsP(proP), x,y, &phi,&lam );  
            dtcc_set_coord3_geo
				( toP, phi,lam, dtcc_coord3_hgt(inptP) );
			break;
        
        case DTCC_PROJECTION_SCALEFACTOR:
        	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = rectangular_scale( dtcc_projection_constantsP(proP), phi,lam, &x, &y ); 
            dtcc_set_coord3_cart( toP, x,y, 1.0 );
			break;
		
     	case DTCC_PROJECTION_LIMITS:
			rectangular_limits( dtcc_projection_constantsP(proP), &y,&x, &phi,&lam );
			dtcc_set_coord3_geo( toP, y,x,DOUBLE_NULL );
			dtcc_set_coord3_geo( toP+1, phi,lam,DOUBLE_NULL );
			status = 0;
			break;

		default:
            status = UNKNOWN_ITEM_REQUESTED;
            break;
    }
    status = map_projection_status(status);
    return status;
}


short StereographicEngine( const PROJ_PARAMS* proP, short mode, const COORD3* inptP, COORD3* toP )
/* Snyder, John P., MAP PROJECTIONS, A WORKING MANUAL. */
/* US Government Printing Office 1987 */
/* Stereographic Projection, p 38-47.  */    
{   short status = 0;
	double x,y, phi,lam;
  	  
    if( mode == DTCC_PROJECTION_SETUP || PROJ_ID(dtcc_projection_constantsP(proP)) == 0 )
	{	COORD_ORIGIN* coP = (COORD_ORIGIN*)(&proP->origin);
		double lat0 = dtcc_projection_lat0(proP);
		double lon0 = dtcc_projection_lon0(proP);
		double k0 =	dtcc_projection_K0(proP);
		
		double fn =  dtcc_projection_fn(proP);
		double fe =  dtcc_projection_fe(proP);
		double fw =  dtcc_projection_feSW(proP);
		double fs =  dtcc_projection_fnSW(proP);

		double lat1 =  dtcc_projection_lower_std_parallel(proP);
		double lat2 =  dtcc_projection_upper_std_parallel(proP);
		
		if( fn == DOUBLE_NULL || fe == DOUBLE_NULL ) 
		{	fn = ( fn == DOUBLE_NULL ? 0.0 : fn );
        	fe = ( fe == DOUBLE_NULL ? 0.0 : fe );
        	dtcc_set_origin_grid( coP, fe,fn, DOUBLE_NULL, SHORT_NULL ); 
        }
		if( fs == DOUBLE_NULL || fw == DOUBLE_NULL ) 
		{	fs = ( fs == DOUBLE_NULL ? 0.0 : fs );
        	fw = ( fw == DOUBLE_NULL ? 0.0 : fw );
        	dtcc_set_origin_gridSW( coP, fw,fs, DOUBLE_NULL, SHORT_NULL ); 
        }
		
        if( lat1 != DOUBLE_NULL || lat2 != DOUBLE_NULL )
        	dtcc_set_projection_std_parallels( (PROJ_PARAMS*)proP, DOUBLE_NULL,DOUBLE_NULL ); 
		
		if( k0 == DOUBLE_NULL ) 
		{	k0 = 1.0;
        	dtcc_set_origin_scale( coP, k0, DOUBLE_NULL, DOUBLE_NULL ); 
        }
	
		if( lat0 == DOUBLE_NULL || lon0 == DOUBLE_NULL ) 
		{	lat0 = 90.0; lon0 = 0.0;
 			dtcc_set_origin_geo( coP, lat0, lon0, DOUBLE_NULL ); 
        }
		init_stereographic( (void*)dtcc_projection_constantsP(proP),NULL,
					dtcc_projection_axis(proP), 
            		dtcc_projection_rflat(proP), lat0, lon0, k0 );
		if( DTCC_PROJECTION_SETUP == mode ) return VMAPOK;
    }       	
            			     	   
	switch (mode)
    {   case DTCC_PROJECTION_FORWARD:
           	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = geo_to_stereographic( dtcc_projection_constantsP(proP), phi,lam, &x, &y );
           	if( status >= VMAPOK )
			{	dtcc_add_origin( &proP->origin, &x, &y );	
				dtcc_set_coord3_grid
					( toP, x,y, dtcc_coord3_elv(inptP), SHORT_NULL );
			}
			break;              

        case DTCC_PROJECTION_INVERSE:
			x = dtcc_coord3_east(inptP);
			y = dtcc_coord3_north(inptP); 
         	status = stereographic_to_geo( dtcc_projection_constantsP(proP), x,y, &phi,&lam );  
            dtcc_set_coord3_geo
				( toP, phi,lam, dtcc_coord3_hgt(inptP) );
			break;
        
        case DTCC_PROJECTION_SCALEFACTOR:
        	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = stereographic_scale( dtcc_projection_constantsP(proP), phi,lam, &x, &y ); 
            dtcc_set_coord3_cart( toP, x,y, 1.0 );
			break;
		
     	case DTCC_PROJECTION_LIMITS:
			stereographic_limits( dtcc_projection_constantsP(proP), &y,&x, &phi,&lam );
			dtcc_set_coord3_geo( toP, y,x,DOUBLE_NULL );
			dtcc_set_coord3_geo( toP+1, phi,lam,DOUBLE_NULL );
			status = 0;
			break;

		default:
            status = UNKNOWN_ITEM_REQUESTED;
            break;
    }
    status = map_projection_status(status);
    return status;
}

short TransverseMercatorEngine( const PROJ_PARAMS* proP,short mode,const COORD3* inptP,COORD3* toP )
/* Snyder, John P., MAP PROJECTIONS, A WORKING MANUAL. */
/* US Government Printing Office 1987 */
/* Transverse Mercator Projection, p 48-65.  */    
              
{   short status = 0;
	double x,y, phi,lam;
    
    if( mode == DTCC_PROJECTION_SETUP || PROJ_ID(dtcc_projection_constantsP(proP)) == 0 )
	{	COORD_ORIGIN* coP = (COORD_ORIGIN*)(&proP->origin);
		double lat0 = dtcc_projection_lat0(proP);
		double lon0 = dtcc_projection_lon0(proP);
		double k0 =	dtcc_projection_K0(proP);
		
		double fn =  dtcc_projection_fn(proP);
		double fe =  dtcc_projection_fe(proP);
		double fw =  dtcc_projection_feSW(proP);
		double fs =  dtcc_projection_fnSW(proP);

		double lat1 =  dtcc_projection_lower_std_parallel(proP);
		double lat2 =  dtcc_projection_upper_std_parallel(proP);
		
		if( fn == DOUBLE_NULL || fe == DOUBLE_NULL ) 
		{	fn = ( fn == DOUBLE_NULL ? 0.0 : fn );
        	fe = ( fe == DOUBLE_NULL ? 0.0 : fe );
        	dtcc_set_origin_grid( coP, fe,fn, DOUBLE_NULL, SHORT_NULL ); 
        }
		if( fs == DOUBLE_NULL || fw == DOUBLE_NULL ) 
		{	fs = ( fs == DOUBLE_NULL ? 0.0 : fs );
        	fw = ( fw == DOUBLE_NULL ? 0.0 : fw );
        	dtcc_set_origin_gridSW( coP, fw,fs, DOUBLE_NULL, SHORT_NULL ); 
        }
		
        if( lat1 != DOUBLE_NULL || lat2 != DOUBLE_NULL )
        	dtcc_set_projection_std_parallels( (PROJ_PARAMS*)proP, DOUBLE_NULL,DOUBLE_NULL ); 
		
		if( k0 == DOUBLE_NULL ) 
		{	k0 = 0.9996;
        	dtcc_set_origin_scale( coP, k0, DOUBLE_NULL, DOUBLE_NULL ); 
        }
	
		if( lat0 == DOUBLE_NULL || lon0 == DOUBLE_NULL ) 
		{	lat0 = 0.0; lon0 = 0.0;
 			dtcc_set_origin_geo( coP, lat0, lon0, DOUBLE_NULL ); 
        }
		init_transverse_mercator( (void*)dtcc_projection_constantsP(proP),NULL,
					dtcc_projection_axis(proP), 
            		dtcc_projection_rflat(proP), lat0, lon0, k0 );
		if( DTCC_PROJECTION_SETUP == mode ) return VMAPOK;
    }       	
    		     	   
    switch (mode)
    {   case DTCC_PROJECTION_FORWARD:
            phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = geo_to_transverse_mercator
           		( dtcc_projection_constantsP(proP), phi,lam, &x, &y );
           	if( status >= VMAPOK )
			{	dtcc_add_origin( &proP->origin, &x, &y );	
				dtcc_set_coord3_grid
					( toP, x,y, dtcc_coord3_elv(inptP), SHORT_NULL );
			}
			break;
			 
        case DTCC_PROJECTION_INVERSE:
			x = dtcc_coord3_east(inptP);
			y = dtcc_coord3_north(inptP); 
         	dtcc_sub_origin( &proP->origin, &x, &y );	
			status = transverse_mercator_to_geo
				( dtcc_projection_constantsP(proP), x,y, &phi,&lam );  
            dtcc_set_coord3_geo
				( toP, phi,lam, dtcc_coord3_hgt(inptP) );
			if( status != 0 )
				status = ( status == -1 ? CONSTANTS_NOT_INITIALIZED : DATA_OUT_OF_RANGE );
			break;
			
        case DTCC_PROJECTION_SCALEFACTOR:
        	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = transverse_mercator_scale
           		( dtcc_projection_constantsP(proP), phi,lam, &x, &y ); 
            dtcc_set_coord3_cart( toP, x,y, 1.0 );
			break;
			
      	case DTCC_PROJECTION_LIMITS:
			transverse_mercator_limits( dtcc_projection_constantsP(proP), &y,&x, &phi,&lam );
			dtcc_set_coord3_geo( toP, y,x,DOUBLE_NULL );
			dtcc_set_coord3_geo( toP+1, phi,lam,DOUBLE_NULL );
			status = 0;
			break;

       default:
            status = UNKNOWN_ITEM_REQUESTED;
            break;
    }
    status = map_projection_status(status);
    return status;
}

short NewZealandMapEngine( const PROJ_PARAMS* proP, short mode, const COORD3* inptP, COORD3* toP )              
{   short status = 0;
	double x,y, phi,lam;
    
    if( mode == DTCC_PROJECTION_SETUP || PROJ_ID(dtcc_projection_constantsP(proP)) == 0 )
	{	double lat0 = -41.0, lon0 = 173.0;
		double fe = 2510000.0, fn = 6023150.0;
		double fs = fn, fw = fe;
		COORD_ORIGIN* coP = (COORD_ORIGIN*)(&proP->origin);
		dtcc_reset_ellipsoid( _PROJECTION_ELLIPSOIDP((PROJ_PARAMS*)proP),"International");
		dtcc_set_origin_grid( coP, fn,fe, DOUBLE_NULL, SHORT_NULL ); 
		dtcc_set_origin_gridSW( coP, fs,fw, DOUBLE_NULL, SHORT_NULL ); 
		dtcc_set_origin_scale( coP, DOUBLE_NULL, DOUBLE_NULL, DOUBLE_NULL ); 
		dtcc_set_origin_geo( coP, lat0, lon0, DOUBLE_NULL );
		init_nzmg((void*)dtcc_projection_constantsP(proP));
		if( DTCC_PROJECTION_SETUP == mode ) return VMAPOK;
    }       	
    		     	   
    switch (mode)
    {   case DTCC_PROJECTION_FORWARD:
            phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = geo_to_nzmg
           		( dtcc_projection_constantsP(proP), phi,lam, &x, &y );
           	if( status >= VMAPOK )
				dtcc_set_coord3_grid
					( toP, x,y, dtcc_coord3_elv(inptP), SHORT_NULL );
			break;
			 
        case DTCC_PROJECTION_INVERSE:
			x = dtcc_coord3_east(inptP);
			y = dtcc_coord3_north(inptP); 
         	status = nzmg_to_geo( dtcc_projection_constantsP(proP), x,y, &phi,&lam );  
            dtcc_set_coord3_geo
				( toP, phi,lam, dtcc_coord3_hgt(inptP) );
			break;
			
    	case DTCC_PROJECTION_LIMITS:
			nzmg_limits( dtcc_projection_constantsP(proP), &y,&x, &phi,&lam );
			dtcc_set_coord3_geo( toP, y,x,DOUBLE_NULL );
			dtcc_set_coord3_geo( toP+1, phi,lam,DOUBLE_NULL );
			status = 0;
			break;

        default:
            status = UNKNOWN_ITEM_REQUESTED;
            break;
    }
    status = map_projection_status(status);
    return status;
}


short MadagascarLabordeEngine( const PROJ_PARAMS* proP,short mode,const COORD3* inptP,COORD3* toP )              
{   short status = 0;
	double x,y, phi,lam;
    
    if( mode == DTCC_PROJECTION_SETUP || PROJ_ID(dtcc_projection_constantsP(proP)) == 0 )
	{	double k0 = DOUBLE_NULL;
		double lat0 = -(18.0 + 54.0/60.0), lon0 = 46.0 + (26.0 + 13.95/60.0)/60.0;
		double fe = 400000.0, fn = 800000.0;
		double fs = fn, fw = fe;
		COORD_ORIGIN* coP = (COORD_ORIGIN*)(&proP->origin);
		dtcc_reset_ellipsoid( _PROJECTION_ELLIPSOIDP((PROJ_PARAMS*)proP),"International");
		dtcc_set_origin_grid( coP, fe,fn, DOUBLE_NULL, SHORT_NULL ); 
		dtcc_set_origin_gridSW( coP, fw,fs, DOUBLE_NULL, SHORT_NULL ); 
		dtcc_set_origin_scale( coP, k0, DOUBLE_NULL, DOUBLE_NULL ); 
		dtcc_set_origin_geo( coP, lat0, lon0, DOUBLE_NULL );
		init_madl((void*)dtcc_projection_constantsP(proP));
		if( DTCC_PROJECTION_SETUP == mode ) return VMAPOK;
    }       	
    		     	   
    switch (mode)
    {   case DTCC_PROJECTION_FORWARD:
            phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = geo_to_madl
           		( dtcc_projection_constantsP(proP), phi,lam, &x, &y );
           	if( status >= VMAPOK )
				dtcc_set_coord3_grid
					( toP, x,y, dtcc_coord3_elv(inptP), SHORT_NULL );
			break;
			 
        case DTCC_PROJECTION_INVERSE:
			x = dtcc_coord3_east(inptP);
			y = dtcc_coord3_north(inptP); 
         	status = madl_to_geo
         		( dtcc_projection_constantsP(proP), x,y, &phi,&lam );  
            dtcc_set_coord3_geo
				( toP, phi,lam, dtcc_coord3_hgt(inptP) );
			break;
			
     	case DTCC_PROJECTION_LIMITS:
			madl_limits( dtcc_projection_constantsP(proP), &y,&x, &phi,&lam );
			dtcc_set_coord3_geo( toP, y,x,DOUBLE_NULL );
			dtcc_set_coord3_geo( toP+1, phi,lam,DOUBLE_NULL );
			status = 0;
			break;
       default:
            status = UNKNOWN_ITEM_REQUESTED;
            break;
    }
    status = map_projection_status(status);
    return status;
}

short UTMEngine( const PROJ_PARAMS* proP,short mode,const COORD3* inptP,COORD3* toP )
/* Snyder, John P., MAP PROJECTIONS, A WORKING MANUAL. */
/* US Government Printing Office 1987 */
/* Transverse Mercator Projection, p 38-47.  */    
{   short status = 0;
	double x,y, phi,lam;
    long izone;

    if( mode == DTCC_PROJECTION_SETUP || PROJ_ID(dtcc_projection_constantsP(proP)) == 0 )
	{	double fe = 500000.0, fn = 0.0;
		double fs = 1000000.0, fw = fe;
		COORD_ORIGIN* coP = (COORD_ORIGIN*)(&proP->origin);
		dtcc_set_origin_grid( coP, fn,fe, DOUBLE_NULL, SHORT_NULL ); 
		dtcc_set_origin_gridSW( coP, fs,fw, DOUBLE_NULL, SHORT_NULL ); 
		dtcc_set_origin_scale( coP, 0.9996, DOUBLE_NULL, DOUBLE_NULL ); 
		dtcc_set_origin_geo( coP, 0.0, 0.0, DOUBLE_NULL );
		init_utm( (void*)dtcc_projection_constantsP(proP),
			dtcc_projection_axis(proP), dtcc_projection_rflat(proP) );
		if( DTCC_PROJECTION_SETUP == mode ) return VMAPOK;
    }       	
            			     	   
	switch (mode)
    {   case DTCC_PROJECTION_FORWARD:
           	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
			izone = dtcc_projection_fixed_grid(proP); 
           	status = geo_to_utm( dtcc_projection_constantsP(proP), phi,lam, &izone, &x, &y );
           	if( status >= VMAPOK )
				dtcc_set_coord3_grid
					( toP, x,y, dtcc_coord3_elv(inptP), (short)izone );
			break;              

        case DTCC_PROJECTION_INVERSE:
			x = dtcc_coord3_east(inptP);
			y = dtcc_coord3_north(inptP);
			izone = dtcc_coord3_zone(inptP); 
         	status = utm_to_geo( dtcc_projection_constantsP(proP), izone, x,y, &phi,&lam );  
            dtcc_set_coord3_geo
				( toP, phi,lam, dtcc_coord3_hgt(inptP) );
			break;
        
       	case DTCC_PROJECTION_LIMITS:
			utm_limits( dtcc_projection_constantsP(proP), &y,&x, &phi,&lam );
			dtcc_set_coord3_geo( toP, y,x,DOUBLE_NULL );
			dtcc_set_coord3_geo( toP+1, phi,lam,DOUBLE_NULL );
			status = 0;
			break;
        
        default:
            status = UNKNOWN_ITEM_REQUESTED;
            break;
    }
    status = map_projection_status(status);
    return status;
}


short UPSEngine( const PROJ_PARAMS* proP,short mode,const COORD3* inptP,COORD3* toP )
/* Snyder, John P., MAP PROJECTIONS, A WORKING MANUAL. */
/* US Government Printing Office 1987 */
/* Stereographic Projection, p 38-47.  */    
{   int zone = 0;
	short status = 0;
	double x,y, phi,lam;
    
    if( mode == DTCC_PROJECTION_SETUP || PROJ_ID(dtcc_projection_constantsP(proP)) == 0 )
	{	double fe = 2000000.0, fn = 2000000.0;
		double fs = fn, fw = fe;
		double latorg = dtcc_projection_lat0(proP);
		COORD_ORIGIN* coP = (COORD_ORIGIN*)(&proP->origin);
		dtcc_set_origin_grid( coP, fn,fe, DOUBLE_NULL, SHORT_NULL ); 
		dtcc_set_origin_gridSW( coP, fs,fw, DOUBLE_NULL, SHORT_NULL ); 
		dtcc_set_origin_scale( coP, 0.994, DOUBLE_NULL, DOUBLE_NULL );
		if( latorg == DOUBLE_NULL ) latorg = 90.0; 
		dtcc_set_origin_geo( coP, latorg, 0.0, DOUBLE_NULL );
		init_ups( (void*)dtcc_projection_constantsP(proP), dtcc_projection_axis(proP), dtcc_projection_rflat(proP), latorg );
		if( DTCC_PROJECTION_SETUP == mode ) return VMAPOK;
    }       	
            			     	   
	switch (mode)
    {   case DTCC_PROJECTION_FORWARD:
           	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = geo_to_ups( dtcc_projection_constantsP(proP), phi,lam, &x, &y );
           	if( status >= VMAPOK )
				dtcc_set_coord3_grid
					( toP, x,y, dtcc_coord3_elv(inptP), zone );
			break;              

        case DTCC_PROJECTION_INVERSE:
			x = dtcc_coord3_east(inptP);
			y = dtcc_coord3_north(inptP); 
         	status = ups_to_geo( dtcc_projection_constantsP(proP), x,y, &phi,&lam );  
            dtcc_set_coord3_geo
				( toP, phi,lam, dtcc_coord3_hgt(inptP) );
			break;
        
        case DTCC_PROJECTION_SCALEFACTOR:
        	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = ups_scale( dtcc_projection_constantsP(proP), phi,lam, &x, &y ); 
            dtcc_set_coord3_cart( toP, x,y, 1.0 );
			break;
		
     	case DTCC_PROJECTION_LIMITS:
			ups_limits( dtcc_projection_constantsP(proP), &y,&x, &phi,&lam );
			dtcc_set_coord3_geo( toP, y,x,DOUBLE_NULL );
			dtcc_set_coord3_geo( toP+1, phi,lam,DOUBLE_NULL );
			status = 0;
			break;

		default:
            status = UNKNOWN_ITEM_REQUESTED;
            break;
    }
    status = map_projection_status(status);
    return status;
}


short MGRSEngine( const PROJ_PARAMS* proP,short mode,const COORD3* inptP,COORD3* toP )
/* Snyder, John P., MAP PROJECTIONS, A WORKING MANUAL. */
/* US Government Printing Office 1987 */
/* MGRS Projection, p 38-47.  */    
{   short status = 0;
	long izone;
	char mgrs[24];
	double x,y, phi,lam;
    
    if( mode == DTCC_PROJECTION_SETUP || PROJ_ID(dtcc_projection_constantsP(proP)) == 0 )
	{	double lat0 = 0.0, lon0 = 0.0;
		double fe = DOUBLE_NULL, fn = DOUBLE_NULL;
		double fs = fn, fw = fe;
		COORD_ORIGIN* coP = (COORD_ORIGIN*)(&proP->origin);
		dtcc_set_origin_grid( coP, fe,fn, DOUBLE_NULL, SHORT_NULL ); 
		dtcc_set_origin_gridSW( coP, fw,fs, DOUBLE_NULL, SHORT_NULL ); 
		dtcc_set_origin_scale( coP, DOUBLE_NULL, DOUBLE_NULL, DOUBLE_NULL ); 
		dtcc_set_origin_geo( coP, lat0, lon0, DOUBLE_NULL );
		init_mgrs( (void*)dtcc_projection_constantsP(proP),
			dtcc_projection_axis(proP),dtcc_projection_rflat(proP));
		if( DTCC_PROJECTION_SETUP == mode ) return VMAPOK;
    }       	
            			     	   
	switch (mode)
    {   case DTCC_PROJECTION_FORWARD:
           	phi = dtcc_coord3_lat(inptP);
           	lam = dtcc_coord3_lon(inptP);
           	status = geo_to_mgrs( dtcc_projection_constantsP(proP), phi,lam, mgrs, &izone,&x,&y );
			dtcc_set_coord3_MGRS( toP, mgrs );
			break;              

        case DTCC_PROJECTION_INVERSE:
			status = mgrs_to_geo( dtcc_projection_constantsP(proP), 
						dtcc_coord3_mgrsP( inptP), &phi,&lam, &izone, &x, &y );  
            dtcc_set_coord3_geo( toP, phi,lam, DOUBLE_NULL ); /* dtcc_coord3_hgt(inptP) ); */
			break;
        
     	case DTCC_PROJECTION_LIMITS:
			mgrs_limits( dtcc_projection_constantsP(proP), &y,&x, &phi,&lam );
			dtcc_set_coord3_geo( toP, y,x,DOUBLE_NULL );
			dtcc_set_coord3_geo( toP+1, phi,lam,DOUBLE_NULL );
			status = 0;
			break;
       default:
            status = UNKNOWN_ITEM_REQUESTED;
            break;
    }
    status = map_projection_status(status);
    return status;
}

const void* _projection_engineP( const DTCC_LABELS* lblP )
{	if(  lblP  )
    {	const struct _PROJECTION_PARS* proP = _find_pro( lblP );
    	if(proP) return (void*)proP->engineP;
    }
    return NULL;
}
short dtcc_validate_origin(	COORD_ORIGIN* csP )
	{	short k = _check_itcoordorigin( csP );
		if( k != VMAPOK ) _set_invalid(csP);
		return k;
	}


short dtcc_validate_units(	const UNITS_INFO* uiP)
	{	short k = VMAPOK;
		if( !uiP )
			k = ILLEGAL_ARG;
		else
		if( !dtcc_is_valid_name(uiP) )
			k = INVALID_DATA;
		else
		if( !(dtcc_is_typeN(dtcc_type_flag(uiP),DTCC_LINEAR) || 
			  dtcc_is_typeN(dtcc_type_flag(uiP),DTCC_ANGULAR) ) )
			k = INVALID_DATA;
		else
		if( !dtcc_units_cf(uiP) )	
			k = INVALID_DATA;
		else
			k = VMAPOK;

		if( k == VMAPOK )
			dtcc_set_valid((UNITS_INFO*)uiP);
		else
			dtcc_set_invalid((UNITS_INFO*)uiP);
		return k;
	}

short dtcc_set_coord3Z( COORD3* c3P, short v )
	{	if( !c3P  )
			return ILLEGAL_ARG;
		else
		{	_COORD3Z(c3P) = v;
			return VMAPOK;
		}
	}

FILE* dtcc_open_dat_file( const char* name, const char* mode )
{    	FILE* fp = NULL;
		if( (fp = fopen( name, mode )) == NULL )
    	{	/* Try upper case */
			int n = strlen(name);
			char * tmp = (char*)dtcc_allocate_ptr(n+1);
			if( tmp ) 
			{	while( --n >= 0 ) 
					tmp[n] = toupper(name[n]);
				fp = fopen( tmp, mode );
				dtcc_free_ptr(tmp);
			}
		}
		return fp;
}

int _scan_hdr( FILE* fp, char* bfr, int* cnt )
{	int status = 0;
	char c[8];
	if( scanTo( fp, '|', 256, bfr ) < 0 )
		status = -1;
	else
	if( scanTo( fp, ';', 8, c ) < 0 )
		status = -2;
	else
		*cnt = atoi(c);
	return status;
}

const char* _check_file_name( const char* fn, const char* dflt )
	{	int k = 0, n=0;
		char *path = "", *name = NULL, *fullpath = NULL;
		if( fn && fn[0] != 0 )
		{	n = strlen(fn);
			if( isFN(fn) )
				name = (char*)fn;	/* name is a complete pathname, pass it on.*/
			else
			{	/* name is a directory, add default name */
				path = (char*)fn;
				name = (char*)dflt;
			}
		}
		else	/* default full pathname */
		{	path = ( isFN(fn) ? (char*)fn : "" ); /*_SYSTEM_DIRECTORY_NAME; */
			name = (char*)dflt;
		}
		k = strlen(path)+strlen(name);
		fullpath = (char*)dtcc_allocate_ptr(k+2);
		if( fullpath ) 
		{	fullpath[0] = 0;
			if( path[0] != 0 )	/* don't add to path */
				strcat(strcpy(fullpath,path),DELIMITER);
			strcat(fullpath,name);
		}
		return fullpath;
	}

int scanTo( FILE* fp, char s, int n, char* bfr )
{	int item, lead=1;
	while( ((item=fgetc(fp)) != EOF)  &&  (n >= 0) )
	{	char c = (char)item;
		if( isspace(item) && (lead == 1) )
			;
		else
		if( c == '/' )
		{	int xitem, lead=1;
			char xc;
			while( ((xitem=fgetc(fp)) != EOF) )
			{	xc= (char)xitem;
				if( xc == '*' )
					if( (char)fgetc(fp) == '/' )
						break;
			}
		}
		else
		if( c != s )
		{	*bfr++ = c;
			lead = 0;
			--n;
		}
		else
			break;
	}
	*bfr = 0;
	return item;
}

short _reset_ellipsoid( ELLIPS_PARAMS* elP, const char* name, int type )
	{	const ELLIPS_PARAMS* fndP;
		DTCC_LABELS id;
		if( elist == NULL ) dtcc_initialize_ellipsoid_list(NULL); 
		dtcc_decode_label( name, &id );
		fndP = _find_name( &id,type );
		if( fndP )
			return _update_ellipsoid( elP, dtcc_nameP(fndP), dtcc_abbrvP(fndP), 
						fndP->axis, fndP->rflat, dtcc_type_flag(fndP) );
		else
			return ITEM_NOT_FOUND;
	}

void* dtcc_check_bfr( void* bfrP, int n )
	{	if( !bfrP ) bfrP = (void*)dtcc_allocate_ptr(n);
		return bfrP;
	}

short dtcc_projection_count( ) 
    {   return sizeof(PROJECTION_LIST)/sizeof(struct _PROJECTION_PARS);  }

short  dtcc_set_coord3_units( 	COORD3_UNITS* cuP, short mode,double map_scale,const char* in1, const char* in2, const char* in3 )            
    {	short k = 0;
    	if( mode != SHORT_NULL ) dtcc_set_coord3_mode( cuP, (COORD3_TYPES) mode );
    	if( map_scale != DOUBLE_NULL ) dtcc_coord3_map_scale(cuP) = map_scale;
    	if( in1 ) 
    	{	k += dtcc_reset_units( _COORD3_UNITS1P(cuP), in1 );
    	 	if( !dtcc_units_are_linear(	_COORD3_UNITS1P(cuP) )) /* set the longitude too */
				 dtcc_reset_units( _COORD3_UNITS2P(cuP), in1 );
    	} 
    	if( in2 ) k += dtcc_reset_units( _COORD3_UNITS2P(cuP), in2 );
    	if( in3 ) 
    		k += dtcc_reset_units( _COORD3_UNITS3P(cuP), in3 );
		else
		if( dtcc_valid_flag(_COORD3_UNITS3P(cuP)) != 1 )
			dtcc_reset_units( _COORD3_UNITS3P(cuP), in3 );
    	return ( k == 0 ? VMAPOK : ILLEGAL_ARG );
    }	

    		
short _update_ellipsoid	( ELLIPS_PARAMS* epP, const char* name, const char* abbrv,double axis, double rflat, short type )
	{	if( epP  )
    	{	char nm[256];
    		epP->axis = axis;
    		epP->rflat = rflat;
			epP->ecc = _compute_ecc( epP->rflat );
			if( name )
    			strcpy( nm,name );
			else
				dtcc_make_ellipsoid_name( epP,nm );
			abbrv = (abbrv && type != _TYPE2_LIST ? abbrv : _DEFAULT_USER_EABBRV);
			dtcc_set_label( epP, nm, abbrv, NULL, 0, type );
    	dtcc_validate_ellipsoid( epP );
			return VMAPOK;
    	}
    	else
    		return ILLEGAL_ARG;
    }

static short _check_itcoordorigin( const COORD_ORIGIN* coP )
	{	short flag = 0; 
		
		/* geo */
        if( (dtcc_origin_lat0(coP)>90.0) || (dtcc_origin_lat0(coP)<-90.0) ||
			(dtcc_origin_lon0(coP)>180.0 ) || (dtcc_origin_lon0(coP)<-180.0) )
		/* if( fabs(dtcc_origin_lat0(coP)) > 90.0 || 
			fabs(dtcc_origin_lon0(coP)) > 180.0 ) */
			flag = NUMERIC_NOT_GEO;
		/* grid */
		else
		if( (/*fabs(dtcc_origin_fn(coP)) > 30000000.0*/( (dtcc_origin_fn(coP)>30000000.0) || (dtcc_origin_fn(coP)<-30000000.0) ) && 
				 dtcc_origin_fn(coP) != DOUBLE_NULL ) || 
			(/*fabs(dtcc_origin_fe(coP)) > 30000000.0*/( (dtcc_origin_fe(coP)>30000000.0) || (dtcc_origin_fe(coP)<-30000000.0) ) &&
				 dtcc_origin_fe(coP) != DOUBLE_NULL ) )  
			flag = NUMERIC_TOO_BIG;
		/* scales */
		else
		if( (dtcc_origin_K0(coP) < 0.0 && dtcc_origin_K0(coP) != DOUBLE_NULL) || 
			(dtcc_origin_H0(coP) < 0.0 && dtcc_origin_H0(coP) != DOUBLE_NULL)  )
			flag = NUMERIC_NEGATIVE;
		/* convergence
		else
		if( 0 )
			flag = 1;
		*/
		/* Azimuth */
		else
		if( dtcc_origin_azm0(coP) != DOUBLE_NULL && 
			/*fabs(dtcc_origin_azm0(coP)) > 360.0*/ ((dtcc_origin_azm0(coP)>360.0) || (dtcc_origin_azm0(coP)<-360.0)) )
			flag = NUMERIC_TOO_BIG;
		else
			flag = VMAPOK;
		
		if( flag == VMAPOK ) _set_valid((COORD_ORIGIN*)coP);
		return flag;	
		 
	}

short dtcc_set_projection_std_parallels( PROJ_PARAMS*  ppP,double        lower,double        upper   )
    {   if( ppP )
    	{	_PROJECTION_STDPARSP(ppP)[0] = lower; 
    		_PROJECTION_STDPARSP(ppP)[1] = upper;
			dtcc_set_invalid(ppP);
    		return 0;
		}
		else
			return ILLEGAL_ARG;
    }

short init_albers_equal_area(	void* cnstsP, const char* opt_name, double a, double rf, double lat0, double lon0, double std1, double std2 )
	{	proj_dfn* cnsts = (proj_dfn*)cnstsP;
		if( fabs(std1 + std2) < EPSILON )
			return -2;
		if( cnsts )
		{	double phi0 = lat0*RADDEG, lam0 = lon0*RADDEG;
			double phi1 = ( fabs(std1) < fabs(std2) ? std1 : std2 )*RADDEG;
			double phi2 = ( fabs(std1) < fabs(std2) ? std2 : std1 )*RADDEG;
			double sphi1 = sin(fabs(phi1)), sphi2 = sin(fabs(phi2));
			_init_std_pars( cnsts, NCalbert, (opt_name ? opt_name : LABELalbert), a, rf, lat0, lon0 );
			STD1 = std1; STD2 = std2;
			E1albert = 1.0 - defECC*defECC;
			set_inverse_authalic( defECC, AUCFSalbert );
			if( !SPHERE )
			{	double m1 = parallel_radius( defECC, sphi1 );
				double m2 = parallel_radius( defECC, sphi2 );
				double q0 = E1albert*authalic_q( defECC,SINP0 );
				double q1 = E1albert*authalic_q( defECC,sphi1 );
				double q2 = E1albert*authalic_q( defECC,sphi2 );
				if( fabs(std1-std2) < EPSILON )
					Nalbert = sphi1;
				else
					Nalbert = (m1*m1 - m2*m2) / (q2 - q1);
				Calbert = m1*m1 + Nalbert*q1;
				R0albert = sqrt(Calbert-Nalbert*q0)/Nalbert;
			}
			else
			{	Nalbert = 0.5*( sphi1 + sphi2 );
				Calbert = square(cos(phi1)) + 2.0*Nalbert*sphi1;
				R0albert = sqrt(  Calbert - 2.0*Nalbert*SINP0 )/Nalbert;
			}
 			ELIM = 180.0; WLIM = -180.0;
			SLIM =  -89.5; NLIM = 89.5;
			return 0; 
		}
		return -1;
	}

int geo_to_albers_equal_area( const void* cnstsP, double lat, double lon, double* x, double* y )
	{	int status = 0;
		if( !cnstsP  )
			return -1;
		else
		if( (status = projection_limit_check(cnstsP, lat,lon)) < 0 )
			return -2;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			double q, rho;
			double sphi,cphi, theta, sint, cost;
			sincostan( lat*RADDEG, &sphi,&cphi,NULL ); 
			theta = Nalbert*check_PI(lon*RADDEG-L0); 
			sincostan( theta, &sint, &cost, NULL );
			q = ( SPHERE ? 2.0*sphi : E1albert*authalic_q(defECC, sphi) );
			rho = sqrt(Calbert - Nalbert*q)/Nalbert;
			*y = defAXIS*(R0albert - rho*cost); *x = defAXIS*rho*sint;
			if( Nalbert < 0.0 ) *y *= -1.0;
			return status;
		}
	}

void dtcc_add_origin( const COORD_ORIGIN* csP, double* x, double* y )
{	double xx = *x + dtcc_origin_fe(csP), yy = *y + dtcc_origin_fn(csP);
	if( dtcc_origin_fe(csP) != dtcc_origin_feSW(csP) && xx < 0.0 ) 
	{	xx += dtcc_origin_feSW(csP);
		xx = -xx;
	}
	if( dtcc_origin_fn(csP) != dtcc_origin_fnSW(csP) && yy < 0.0 ) 
	{	yy += dtcc_origin_fnSW(csP);
		yy = -yy;
	}
	*x = xx; *y = yy;
	return;
}

void dtcc_sub_origin( const COORD_ORIGIN* csP, double* x, double* y )
{	double xx = *x, yy = *y;
	if( dtcc_origin_fe(csP) != dtcc_origin_feSW(csP) && xx < 0.0 )
		xx = fabs(xx) - dtcc_origin_feSW(csP);
	*x = xx - dtcc_origin_fe(csP);
	if( dtcc_origin_fn(csP) != dtcc_origin_fnSW(csP) && yy < 0.0 )
		yy = fabs(yy) - dtcc_origin_fnSW(csP);
	*y = yy - dtcc_origin_fn(csP);
	return;
}

int albers_equal_area_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon )
	{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
		double q, theta, rphi,rlam;
		y = R0albert - ( Nalbert < 0.0 ? -y : y )/defAXIS; x = x/defAXIS;
		if( Nalbert < 0.0 )
			theta = atan2( -x, -y );
		else
			theta = atan2( x, y );
		rlam = check_PI(L0 + theta/Nalbert);
		q = Calbert/Nalbert - Nalbert*( x*x + y*y );
		if( SPHERE)
			rphi = asin( 0.5*q ); 
		else
		{	double sinB = q/(1.0 - E1albert*log((1.0-defECC)/(1.0+defECC))/(defECC+defECC));
			inverse_authalic_lat( asin(sinB), sinB, AUCFSalbert, &rphi ) ; 
		}
		*lon = rlam/RADDEG;
		*lat = rphi/RADDEG;
		if( STD2 < 0.0 ) *lat *= -1.0;
		return projection_limit_check(cnstsP, *lat,*lon);
	}

int albers_equal_area_scale( const void* cnstsP, double lat, double lon, double* h, double* k )
	{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
		double s,q, rho, rphi = fabs(lat)*RADDEG;
		double sphi=sin(rphi);
		q = ( SPHERE ? 2.0*sphi : E1albert*authalic_q(defECC, sphi) );
		rho = sqrt(Calbert - Nalbert*q)/Nalbert;
		s = Nalbert*rho/( SPHERE ? cos(rphi) : parallel_radius(defECC,sphi) );
		*k = s;
		*h = 1.0/s;		
		return VMAPOK;
	}

int albers_equal_area_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon )
	{	if( !cnstsP || !mnlat || !mxlat || !mnlon || !mxlon)
			return -1;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			*mnlat = SLIM; *mxlat = NLIM; 
			*mnlon = WLIM; *mxlon = ELIM;
			return 0;
		}
	}
short map_projection_status( short n)
{	return	 ( n == -1 ? MEMORY_ALLOCATION_FAILURE :
			 ( n == -2 ? DATA_OUT_OF_RANGE : n ) );
}


short init_cylindrical_equal_area(	void* cnstsP, const char* opt_name, double a, double rf, double latS, double lon0, double h0 )
	{	proj_dfn* cnsts = (proj_dfn*)cnstsP;
		if( cnsts ) 
		{	_init_std_pars( cnsts,NCcylind, (opt_name ? opt_name : LABELcylind),a, rf, latS, lon0 );
			if( h0 <= 0.0 )
			{	double qp = (1.0-defECC2)*authalic_q(defECC,1.0);
				set_inverse_authalic( defECC,AUCFScylind );
				K0 = parallel_radius(defECC,SINP0);
				AK0cylind = defAXIS*K0;
				KAIcylind = defAXIS/(K0+K0);
				F3cylind = (K0+K0)/(defAXIS*qp);
				WH = 0;
			}
			else
			{	K0 = 1.0/h0;
				AK0cylind = defAXIS*K0;
				QPcylind = (1.0-defECC2)*authalic_q(defECC,1.0);
				set_inverse_authalic( defECC,AUCFScylind );
				set_inverse_rectifying( defECC,FPCFScylind );
				set_meridian_distance( defECC,MDCFScylind );
				M0cylind = meridian_distance( P0,SINP0, MDCFScylind );
				WH = 1;
			}
			SLIM = -89.0; WLIM = check_180(lon0 - 90.0);
			NLIM =  89.0; ELIM = check_180(lon0 + 90.0);
		}
		return 0;
	}


const void* set_cylindrical_equal_area(	const char* opt_name, double a, double rf, double latS, double lon0, double h0 )
	{	proj_dfn* cnsts = (proj_dfn*)allocate_projection(1);
		init_cylindrical_equal_area( cnsts,opt_name,a,rf,latS,lon0,h0 );
		return cnsts;
	}

int geo_to_cylindrical_equal_area( const void* cnstsP, double lat, double lon, double* x, double* y )
	{	int status;
		if( !cnstsP  )
			return -1;
		else
		if( (status = projection_limit_check(cnstsP, lat,lon)) < 0 )
			return -2;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			if( WH == 0 )
			{	if( fabs(lat) > 89.995 )
					return -2;
				else
				{	double q = (1.0-defECC2)*authalic_q( defECC,sin(lat*RADDEG) );
					*x = AK0cylind*check_PI( lon*RADDEG - L0 );
					*y = KAIcylind*q;
				}
				return ( fabs(lat) > 88.0 ? 1 : VMAPOK );
			}
			else
			{	double dlam = lon*RADDEG - L0, phi = lat*RADDEG;
				double MC, phic, beta, betac, sphiA = (1.0-defECC2)*authalic_q(defECC,sin(phi))/QPcylind;
				beta = asin(sphiA);
				betac = atan2( tan(beta), cos(dlam) ); 
				inverse_authalic_lat( betac,sin(betac), AUCFScylind, &phic );
				*x = AK0cylind*cos(beta)*sin(dlam)*parallel_radius(defECC,sin(phic))/cos(betac);
				MC = meridian_distance( phic,sin(phic), MDCFScylind );
				*y = defAXIS*(MC - M0cylind)/K0;
				return VMAPOK;
			} 	
		}
	}

int cylindrical_equal_area_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon )
	{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
		if( WH == 0 )
		{	double beta = asin( F3cylind*y );
			inverse_authalic_lat( beta,sin(beta), AUCFScylind, lat );
			*lat /= RADDEG;
			*lon = check_PI( L0 + x/AK0cylind )/RADDEG;
		}
		else
		{   double mu = (M0cylind + y*K0/defAXIS)/MDCFScylind[0];
			double phic, betac, betap, beta;
			inverse_rectifying_lat( mu,sin(mu), FPCFScylind, &phic );
			authalic_lat( defECC, phic,sin(phic), &betac, NULL );
			betap = -asin( x*cos(betac)/( AK0cylind*parallel_radius(defECC,sin(phic)) ));
			beta = asin( cos(betap)*sin(betac) );
			*lon = check_PI( L0 - atan2( tan(betap),cos(betac) ))/RADDEG;
			inverse_authalic_lat( beta, sin(beta), AUCFScylind, lat );
			*lat /= RADDEG;
		}
		return projection_limit_check(cnstsP, *lat,*lon);  
	}		

int cylindrical_equal_area_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon )
	{	if( !cnstsP || !mnlat || !mxlat || !mnlon || !mxlon)
			return -1;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			*mnlat = SLIM; *mxlat = NLIM; 
			*mnlon = WLIM; *mxlon = ELIM;
			return 0;
		}
	}


short init_lambert_conformal_conic(	void* cnstsP, const char* opt_name, double a, double rf, double lat0, double lon0, double std1, double std2 )
	{	proj_dfn* cnsts = (proj_dfn*)cnstsP;
		if( fabs(std1 + std2) < EPSILON )
			return -2;
		else
		if( cnsts && cnsts->id == 0 )
		{	double m1,t1;
			double phi1 = ( fabs(std1) <= fabs(std2) ? std1 : std2 )*RADDEG;
			double phi2 = ( fabs(std1) < fabs(std2) ? std2 : std1 )*RADDEG;
			double sphi1 = sin(phi1), sphi2 = sin(phi2);
			_init_std_pars( cnsts, NClamb, (opt_name ? opt_name : LABELlamb), a, rf, lat0, lon0 );
			set_inverse_conformal( defECC, CFCFSlamb );
			t1 = tanz( defECC,phi1,sphi1 );
			m1 = parallel_radius( defECC, sphi1 );
			if( fabs(std1-std2) < EPSILON )
				Nlamb = sphi1;
			else
				Nlamb = log(m1/parallel_radius(defECC,sphi2))/log(t1/tanz(defECC,phi2,sphi2));
			Flamb = m1/(Nlamb*pow(t1,Nlamb) );
			R0lamb = RHOlamb(P0,SINP0);
			STD1 = std1; STD2 = std2;
			ELIM = 180.0; WLIM = -180.0;
			SLIM = ( std1 < 0.0 ? -90.0 : -89.5 ); 
			NLIM = ( std1 < 0.0 ? 89.5 : 90.0 );
			return 0; 
		}
		return -1;
	}


const void* set_lambert_conformal_conic(	const char* opt_name, double a, double rf, double lat0, double lon0, double std1, double std2 )
	{	if( fabs(std1 + std2) < EPSILON )
			return NULL;
		else
		{	proj_dfn* cnsts = (proj_dfn*)allocate_projection(1);
			if( init_lambert_conformal_conic(cnsts,opt_name,a,rf,lat0,lon0,std1,std2 ) != 0 )
			{	if(cnsts) free_projection(cnsts);
				return NULL;
			}
			return (const void*)cnsts;
		}
	}

int geo_to_lambert_conformal_conic( const void* cnstsP, double lat, double lon, double* x, double* y )
	{	int status = 0;
		if( !cnstsP  )
			return -1;
		else
		if( (status = projection_limit_check(cnstsP, lat,lon)) < 0 )
			return -2;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			double rphi = lat*RADDEG, rlam = lon*RADDEG;
			double dlam = check_PI(rlam-L0);
			double sphi = sin(rphi);
			double rho = RHOlamb(rphi,sphi);
			double sint, cost, theta = check_PI(Nlamb*dlam);
			sincostan( theta, &sint,&cost, NULL );
			*x = rho * sint;
			*y = R0lamb - rho*cost;
			return status;
		}
	}

		
int lambert_conformal_conic_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon )
	{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
		double r, rho, theta,t, X, rphi;
		double rho0 = R0lamb;
		if( Nlamb < 0.0 ) {	x = -x; y = -y; rho0 = -rho0;  }
		r = rho0 - y;
		r_and_theta( 0, x,r, &rho, &theta );
		if( fabs(rho) < EPSILON )
		{	*lat = affix_sign( 90.0, Nlamb );
			*lon = 0.0;
			return 2;
		}
		else
		{	double ZZ = rho/(defAXIS*Flamb), YY = 1.0/Nlamb;
			theta = check_PI(PI - theta);
			t = pow( fabs(ZZ), YY);
			X = HALF_PI - 2.0*atan(t);
			inverse_conformal_lat( X, sin(X), CFCFSlamb, &rphi );
			*lon = check_PI( theta/Nlamb + L0 )/RADDEG;
			*lat = rphi/RADDEG;
			return projection_limit_check(cnstsP, *lat,*lon);
		}
	}
	
int lambert_conformal_conic_scale( const void* cnstsP, double lat, double lon, double* h, double* k )
	{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
		double rphi = lat*RADDEG;
		double sphi = sin(rphi); 
		double rho = RHOlamb(rphi,sphi);
		*h = *k = rho*Nlamb/( defAXIS*parallel_radius(defECC,sphi) ); 
		return VMAPOK;
	}


int geo_to_lambert_equal_area( const void* cnstsP, double lat, double lon, double* x, double* y )
	{	int status = 0;
		if( !cnstsP  )
			return -1;
		else
		if( (status = projection_limit_check(cnstsP, lat,lon)) < 0 )
			return -2;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			double xa,ya, B, sinB, cosB;
			double rphi = lat*RADDEG, rlam = lon*RADDEG;
			double sdlam, cdlam, dlam = check_PI(rlam - L0);
			sincostan( dlam, &sdlam, &cdlam,NULL );
			authalic_lat( defECC, rphi,sin(rphi),&B, &sinB );
			cosB = sqrt(1.0-sinB*sinB);
			if( OBLIQUE )	
			{	double b = RPlambeq*sqrt( 2.0/(1.0+SINB1lambeq*sinB+COSB1lambeq*cosB*cdlam));
				ya = b*(COSB1lambeq*sinB - SINB1lambeq*cosB*cdlam)/Dlambeq;
				xa = b*cosB*sin(dlam)*Dlambeq;
			}
			else
			if( EQUATORIAL )	
			{	double t = sqrt(2.0/(1.0+cosB*cdlam));
				ya = RPlambeq*sinB*t;
				xa = cosB*sin(dlam)*t;
			}
			else			/* polar */
			{	double sphi = sin(rphi);
				double q = GElambeq*authalic_q(defECC,sphi);
				double rho = sqrt( RPlambeq - ( SOUTH_POLAR ? -q : q ) );
				xa = rho * sin(dlam);
				ya = -rho*cdlam;
				if( SOUTH_POLAR ) ya *= -1.0;
			}
			*y = RADIUS*ya;
			*x = RADIUS*xa;
			return status;
		}
	}

	
int lambert_equal_area_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon )
	{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
		double rho,dlam, B,sinB, rphi;
		x /= RADIUS; y /= RADIUS;
		if( POLAR )
		{	r_and_theta( WH, x,y, &rho, &dlam );
			sinB = 1.0 - rho*rho/(1.0 - (1.0-defECC2)*log((1.0-defECC)/(1.0+defECC))/(defECC+defECC));
			B = asin(sinB);
		}
		else
		{	double xd = x/Dlambeq, yd = y*Dlambeq;
			rho = sqrt( xd*xd + yd*yd );
			if( rho < EPSILON )
			{	B = B1lambeq;
				sinB = SINB1lambeq;
				dlam = 0.0;
			}
			else
			{	double num,den, ce, sinCE,cosCE;
				ce = 2.0*asin(0.5*rho/RPlambeq);
				sincostan( ce, &sinCE, &cosCE, NULL );
				sinB = cosCE*SINB1lambeq + (yd*sinCE*COSB1lambeq)/rho;
				B = asin(sinB);
				num = x*sinCE;
				den = Dlambeq*(rho*COSB1lambeq*cosCE - yd*SINB1lambeq*sinCE);
				dlam = atan2( num,den );
			}
		}
		inverse_authalic_lat( B,sinB, AUCFSlambeq, &rphi );
		*lat = rphi/RADDEG;
		*lon = check_PI(dlam+L0)/RADDEG;
		return projection_limit_check(cnstsP, *lat,*lon);
	}

int lambert_equal_area_scale( const void* cnstsP, double lat, double lon, double* h, double* k )
	{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
		double rphi = lat*RADDEG, rlam = lon*RADDEG;
		if( OBLIQUE && SPHERE)	
		{	double b, B, sinB, cosB;
			authalic_lat( defECC, rphi,sin(rphi),&B, &sinB );
			cosB = sqrt(1.0-sinB*sinB);
		 	b = RPlambeq*sqrt( 2.0/(1.0+SINB1lambeq*sinB+COSB1lambeq*cosB*cos(rlam-L0)) );
			*k = b; *h = 1.0/b;
			return VMAPOK;
		}
		else			
		if( POLAR )
		{	double sphi = sin(rphi);
			double q = GE*authalic_q(defECC,sphi);
			double rho = sqrt( RPlambeq - ( SOUTH_POLAR ? -q : q ) );
			double s = rho/parallel_radius( defECC,sphi );
			*k =  s; *h = 1.0/s;
			return VMAPOK;
		}
		else
		{	if( k ) *k = DOUBLE_NULL;
			if( h ) *h = DOUBLE_NULL;
			return -1;
		}
	}
	
int lambert_equal_area_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon )
	{	if( !cnstsP || !mnlat || !mxlat || !mnlon || !mxlon)
			return -1;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			*mnlat = SLIM; *mxlat = NLIM; 
			*mnlon = WLIM; *mxlon = ELIM;
			return 0;
		}
	}

const void* set_lambert_equal_area( const char* opt_name, double a, double rf, double lat1, double lon0 )
	{	proj_dfn* cnsts = (proj_dfn*)allocate_projection(1);
		init_lambert_equal_area( cnsts,opt_name,a,rf,lat1,lon0 );
		return cnsts;
	}

short init_lambert_equal_area( void* cnstsP, const char* opt_name, double a, double rf, double lat1, double lon0 )
	{	proj_dfn* cnsts = (proj_dfn*)cnstsP;
		if( cnsts )
		{	_init_std_pars( cnsts, NClambeq, (opt_name ? opt_name : LABELlambeq), a, rf, lat1, lon0 );
			GElambeq = 1.0 - defECC*defECC;
			set_inverse_authalic( defECC, AUCFSlambeq );
			RPlambeq = GE*authalic_q(defECC,1.0);
			if( EQUATORIAL )
			{	RPlambeq /= 2.0;
				SLIM = -90.0; NLIM = 90.0;
				WLIM = check_180( lon0 - 89.95 );
				ELIM = check_180( lon0 + 89.95 );
			}
			else
			if( POLAR )
			{	Glambeq = 1.0 - GElambeq*log( (1.0-defECC)/(1.0+defECC) )/(defECC+defECC);
				if( SOUTH_POLAR )
				{	WLIM = -180.0; ELIM = 180.0;
					NLIM = -0.0001; SLIM = -90.0;
				}
				else
				if( POLAR )
				{	WLIM = -180.0; ELIM = 180.0;
					SLIM = -.0001; NLIM = 90.0;
				}
			}
			else
			{	authalic_lat( defECC, P0,SINP0, &B1lambeq, &SINB1lambeq );
				COSB1lambeq = sqrt(1.0-SINB1lambeq*SINB1lambeq);
				if( !SPHERE ) 
				{	RPlambeq = sqrt(RPlambeq/2.0);
					Dlambeq = parallel_radius( defECC,SINP0 )/(RPlambeq*COSB1lambeq);
				}
				else
					Dlambeq = RPlambeq = 1.0;
			}
			return 0;
		}
		return -1;
	}

int geo_to_mercator( const void* cnstsP, double lat, double lon, double* x, double* y )
	{	int status;
		if( !cnstsP  )
			return -1;
		else
		if( (status = projection_limit_check(cnstsP, lat,lon)) < 0 )
			return -2;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			if( fabs(lat) > 89.999995 )
				return -2;
			else
			{	*x = defAXIS*check_PI(lon*RADDEG-L0); 
				*y = defAXIS*log( tanHZ( defECC,sin(lat*RADDEG)) );
			}
			return status;
		}
	}
	
int mercator_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon )
	{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
		double X, rphi;
		y /= defAXIS; x /= defAXIS;
		X = HALF_PI - 2.0*atan(exp( -y ));
		inverse_conformal_lat( X, sin(X), CFCFSmerca, &rphi ); 
		*lon = check_PI(L0 + x)/RADDEG;
		*lat = rphi/RADDEG;
		return projection_limit_check(cnstsP, *lat,*lon);  
	}		

int mercator_scale( const void* cnstsP, double lat, double lon, double* h, double* k )
	{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
		*h = *k = 1.0/parallel_radius( defECC,sin(lat*RADDEG) );
		return VMAPOK;
	}


int mercator_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon )
	{	if( !cnstsP || !mnlat || !mxlat || !mnlon || !mxlon)
			return -1;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			*mnlat = SLIM; *mxlat = NLIM; 
			*mnlon = WLIM; *mxlon = ELIM;
			return 0;
		}
	}


short init_mercator(	void* cnstsP, const char* opt_name, double a, double rf, double lon0 )
	{	proj_dfn* cnsts = (proj_dfn*)cnstsP;
		if( cnsts ) 
		{	_init_std_pars( cnsts, NCmerca, (opt_name ? opt_name : LABELmerca), a, rf, 0.0, lon0 );
			set_inverse_conformal( defECC,CFCFSmerca );
			SLIM = -89.0; WLIM = -180.0;
			NLIM = 89.0; ELIM = 180.0;
			return 0;
		}
		return -1;
	}

const void* set_mercator(	const char* opt_name, double a, double rf, double lon0 )
	{	proj_dfn* cnsts = (proj_dfn*)allocate_projection(1);
		init_mercator( cnsts, opt_name, a, rf, lon0 );
		if( cnsts ) 
		{	set_inverse_conformal( defECC,CFCFSmerca );
			SLIM = -89.0; WLIM = -180.0;
			NLIM = 89.0; ELIM = 180.0;
		}
		return cnsts;
	}

/* Netherlands East Indies Equatorial Zone */
const void* set_netherlands_east_indies()
	{	double a = 6377397.155, rf = 299.1528128;	/* Bessel */
		double k0 = 0.997;
		double lat0 = 0.0, lon0 = 110.0;
		const proj_dfn* cnsts = 
			(proj_dfn*)set_mercator( "Netherlands East Indies", a,rf, lon0 );
		return cnsts;
	}

short netherlands_east_indies_to_geo( const void* tcnsts, double easting, double northing, double* lat, double* lon )
	{	return mercator_to_geo( tcnsts, easting-FEmerca,northing-FNmerca, lat,lon );  } 


short geo_to_netherlands_east_indies( const void* tcnsts, double lat, double lon, double* easting, double* northing	)
	{	short status =  geo_to_mercator( tcnsts, lat,lon, easting,northing );
		*northing += FNmerca; *easting += FEmerca;
		return status;
	}


short netherlands_east_indies_scale	( const void* tcnsts, double lat, double lon, double* h, double* k 	)
	{	return mercator_scale( tcnsts, lat,lon, h,k );	 }


/* ON sphere only */
int orthographic_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon )
	{	if( !cnstsP || !mnlat || !mxlat || !mnlon || !mxlon)
			return -1;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			*mnlat = SLIM; *mxlat = NLIM; 
			*mnlon = WLIM; *mxlon = ELIM;
			return 0;
		}
	}
 

short init_orthographic( void* cnstsP, const char* opt_name, double a, double rf, double lat0, double lon0 )
	{	proj_dfn* cnsts = (proj_dfn*)cnstsP;
		if( cnsts )
			 _init_std_pars( cnsts, NCortho, (opt_name ? opt_name : LABELortho), a,0.0,lat0,lon0 );
		else
			return -1;
		if( EQUATORIAL )
		{	SLIM = -90.0; NLIM = 90.0;
			WLIM = check_180( lon0 - 89.95 );
			ELIM = check_180( lon0 + 89.95 );
		}
		else
		if( SOUTH_POLAR )
		{	WLIM = -180.0; ELIM = 180.0;
			NLIM = -0.0001; SLIM = -90.0;
		}
		else
		if( POLAR )
		{	WLIM = -180.0; ELIM = 180.0;
			SLIM = -.0001; NLIM = 90.0;
		}
		return 0;
	}

const void* set_orthographic( const char* opt_name, double a, double rf, double lat0, double lon0 )
	{	proj_dfn* cnsts = (proj_dfn*)allocate_projection(1);
		init_orthographic( cnsts, opt_name,a,rf,lat0,lon0 );
		return cnsts;
	}


int geo_to_orthographic( const void* cnstsP, double lat, double lon, double* x, double* y )
	{	int status = 0;
		if( !cnstsP  )
			return -1;
		else
		if( (status = projection_limit_check(cnstsP, lat,lon)) < 0 )
			return -2;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			double rphi = lat*RADDEG, rlam = lon*RADDEG;
			double sdlam,cdlam,dlam = check_PI(rlam - L0 );
			double sphi, cphi, cosc;
			sincostan( dlam, &sdlam, &cdlam, NULL );
			sincostan( rphi, &sphi, &cphi, NULL );
			*x = RADIUS*cphi*sdlam; *y = 0.0;
			cosc = SINP0*sphi + COSP0*cphi*cdlam;
			if( cosc < 0.0 )
				status = -10;
			else
			if( OBLIQUE )	/* oblique */
				*y = RADIUS*( COSP0*sphi - SINP0*cphi*cdlam);
			else
			if( EQUATORIAL )	/* equatorial */
				*y = RADIUS*sphi;
			else			/* polar */
				*y = (  P0 < 0.0 ? 1.0 : -1.0 )*RADIUS*cphi*cdlam;
			return status;
		}
	}

int orthographic_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon )
	{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
		double rho, phi,lam;
		rho = sqrt( x*x + y*y );
		if( rho <= 0.0 )
		{	*lat = P0/RADDEG;
			*lon = L0/RADDEG;
		}
		else
		{	double sinc = rho/RADIUS ,cosc = sqrt(1.0 - sinc*sinc);
			phi = asin( cosc*SINP0 + ( y*sinc*COSP0)/rho );
			if( !POLAR )
				lam = L0 + atan2( x*sinc, rho*COSP0*cosc - y*SINP0*sinc );
			else
				lam = atan2( x, ( P0 > 0.0 ? -y : y ) );
			*lon = check_PI(lam)/RADDEG;
			*lat = phi/RADDEG;
		}
		return projection_limit_check(cnstsP, *lat,*lon);
	}
	

int orthographic_scale( const void* cnstsP, double lat, double lon, double* h, double* k )
	{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
		double rphi = lat*RADDEG, rlam = lon*RADDEG;
		*k = 1.0;
		if( !POLAR )	/* oblique or equatorial */
			*h = SINP0*sin(rphi) + COSP0*cos(rphi)*cos(rlam-L0);
		else			/* polar */
		{	*h = -sin(rphi);
			if( P0 < 0.0 ) *h *= -1.0;
		}
		
		return VMAPOK;
	}

int rectangular_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon )
	{	if( !cnstsP || !mnlat || !mxlat || !mnlon || !mxlon)
			return -1;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			*mnlat = SLIM; *mxlat = NLIM; 
			*mnlon = WLIM; *mxlon = ELIM;
			return 0;
		}
	}
	 

short init_rectangular(	void* cnstsP, const char* opt_name, double a, double rf, double lat0, double lon0 )
	{	proj_dfn* cnsts = (proj_dfn*)cnstsP;
		if( fabs( lat0*RADDEG ) > HALF_PI - EPSILON ) 
			return -2;
		else
		if( !cnsts )
			return -1;
		else
		{	_init_std_pars( cnsts, NCrect, (opt_name ? opt_name : LABELrect), a, 0.0, lat0, lon0 );
			RCPrect = RADIUS*COSP0;
			if( P0 == 0.0 ) strcpy( NM, "Plate Carree" );
			SLIM = -89.0; WLIM = -180.0;
			NLIM = 89.0; ELIM = 180.0;
			return 0;
		}
	}

const void* set_rectangular(	const char* opt_name, double a, double rf, double lat0, double lon0 )
	{	proj_dfn *cnsts = (proj_dfn*)allocate_projection(1);
		if( init_rectangular( cnsts,opt_name,a,rf,lat0,lon0) != 0 )
		{	if(cnsts) free_projection(cnsts);
			return NULL;
		}
		return cnsts;
	}


int geo_to_rectangular( const void* cnstsP, double lat, double lon, double* x, double* y )
	{	int status = 0;
		if( !cnstsP  )
			return -1;
		else
		if( (status = projection_limit_check(cnstsP, lat,lon)) < 0 )
			return -2;
		else
		{	double phi = lat*RADDEG;
			if( fabs(phi) > HALF_PI - EPSILON )
				return -2;
			else
			{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
				*y = RADIUS*phi;
				*x = RCPrect*check_PI(lon*RADDEG-L0);
				return status;
			}
		}
	}

int rectangular_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon )
	{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
		y /= defAXIS; x /= defAXIS;
		*lat = y/RADDEG; 
		*lon = check_PI(L0 + x/COSP0)/RADDEG; 
		return projection_limit_check(cnstsP, *lat,*lon);  
	}		


int rectangular_scale( const void* cnstsP, double lat, double lon, double* h, double* k )
	{	double phi = lat*RADDEG;
		if( fabs(phi) > HALF_PI - EPSILON )
			return -2;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			*h = 1.0;
			*k = COSP0/cos(phi);
			return VMAPOK;
		}
	}


int stereographic_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon )
	{	if( !cnstsP || !mnlat || !mxlat || !mnlon || !mxlon)
			return -1;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			*mnlat = SLIM; *mxlat = NLIM; 
			*mnlon = WLIM; *mxlon = ELIM;
			return 0;
		}
	}
	 


short init_stereographic( void* cnstsP, const char* opt_name, double a, double rf, double lat1, double lon0, double k0 )
	{	proj_dfn* cnsts = (proj_dfn*)cnstsP;
		if( !cnsts )
			return -1;
		else
		{	double phi1 = lat1*RADDEG, lam0 = lon0*RADDEG;
			if( fabs(lat1) < 90.0 )
			{	_init_std_pars( cnsts, NCstereo, (opt_name ? opt_name : LABELstereo), a, rf, lat1, lon0 );
				K0 = k0;  
				FAstereo = 2.0 * defAXIS * K0;
				M1stereo = COSP0*vertical_radius( defECC, SINP0 );
				conformal_lat( defECC, P0,SINP0, &CLAT1stereo );
				sincostan( CLAT1stereo, &SINCLAT1stereo, &COSCLAT1stereo,NULL );
	    		/* Inverse conformal latitude coefficients */
				set_inverse_conformal( defECC, CFCFSstereo );
				if( EQUATORIAL )
				{	SLIM = -90.0; NLIM = 90.0;
					WLIM = check_180( lon0 - 89.95 );
					ELIM = check_180( lon0 + 89.95 );
				}
				return 0;
			}
			else
				return init_polar_stereographic( cnsts,NULL, a,rf,lat1,lon0, k0 );
		}
	}

const void* set_stereographic( const char* opt_name, double a, double rf, double lat1, double lon0, double k0 )
	{	proj_dfn* cnsts = (proj_dfn*)allocate_projection(1);
		init_stereographic(cnsts,opt_name,a,rf,lat1,lon0,k0 );
		return cnsts;
	}


int geo_to_stereographic( const void* cnstsP, double lat, double lon, double* x, double* y )
	{	int status = 0;
		if( !cnstsP  )
			return -1;
		else
		if( (status = projection_limit_check(cnstsP, lat,lon)) < 0 )
			return -2;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			double rphi = lat*RADDEG, rlam = lon*RADDEG;
			double xx,yy, sphi,cphi;
			double sdlam,cdlam, dlam = check_PI( rlam - L0 );
			sincostan( rphi, &sphi, &cphi, NULL );
			if( !POLAR )	/* Oblique or equatorial */
			{	double A, X,sinX,cosX;
				sincostan( dlam, &sdlam, &cdlam,NULL );
				conformal_lat( defECC, rphi,sphi, &X );
				sincostan( X, &sinX, &cosX, NULL );
				A = FAstereo*M1stereo/(COSCLAT1stereo*(1.0+SINCLAT1stereo*sinX + COSCLAT1stereo*cosX*cdlam) );
				xx = A * cosX * sdlam;
				yy = A * ( COSCLAT1stereo*sinX - SINCLAT1stereo*cosX * cdlam );
			}
			else
				geo_to_polar_stereographic( cnsts, lat,lon, &xx,&yy );
			*y = yy; *x = xx;
			return VMAPOK;		 
		}
	}


int stereographic_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon )
	{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
		double rho, rphi,rlam;
		rho = sqrt( x*x + y*y );
		if( !POLAR )	/* Oblique or equatorial */
		{	double sinX;
			double sinCE, cosCE, ce = 2.0*atan2( rho*COSCLAT1stereo, FAstereo*M1stereo );
			sincostan( ce, &sinCE, &cosCE, NULL );
			sinX = cosCE*SINCLAT1stereo + ( y*sinCE*COSCLAT1stereo/rho);
			inverse_conformal_lat( asin(sinX), sinX, CFCFSstereo, &rphi );
			rlam = L0 + atan2( x*sinCE, (rho*COSCLAT1stereo*cosCE - y*SINCLAT1stereo*sinCE ) );
			*lat = rphi/RADDEG;
			*lon = check_PI(rlam)/RADDEG;			
		}
		else
			polar_stereographic_to_geo( cnsts, x,y,  lat, lon ); 
		return projection_limit_check(cnstsP, *lat,*lon);
	}


int stereographic_scale( const void* cnstsP, double lat, double lon, double* h, double* k )
	{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
		double rphi = lat*RADDEG, rlam = lon*RADDEG;
		double s, sphi = sin(rphi);
		if( !POLAR )	/* Oblique or equatorial */
		{	double  A, X, cosX;
			conformal_lat( defECC, rphi,sphi, &X );
			cosX = cos(X);
			A = FAstereo*M1stereo/(COSCLAT1stereo*(1.0+SINCLAT1stereo*sin(X) + COSCLAT1stereo*cosX*cos(rlam-L0)) );
			s = A * cosX / ( defAXIS*cos(rphi)*vertical_radius(defECC,sphi) ); 
		}
		else
			polar_stereographic_scale( cnsts, lat,lon, &s, &s );
		*k = *h = s;
		return VMAPOK;
	}

int transverse_mercator_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon )
	{	if( !cnstsP || !mnlat || !mxlat || !mnlon || !mxlon)
			return -1;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			*mnlat = SLIM; *mxlat = NLIM; 
			*mnlon = WLIM; *mxlon = ELIM;
			return 0;
		}
	}
	 
short init_transverse_mercator(	void* cnstsP, const char* opt_name, double a, double rf, double lat0, double lon0, double k0 )
	{	if( !cnstsP )
			return -1;
		else
		{	proj_dfn* cnsts = (proj_dfn*)cnstsP;
			_init_std_pars
				( cnsts, NCtrmerca, (opt_name ? opt_name : LABELtrmerca), a,rf, lat0,lon0 );
			K0 = k0; EP2trmerca = defECC2/(1.0-defECC2);
			KAtrmerca = K0*defAXIS; 
	    	/* Meridian distance coefficients */
	    	set_meridian_distance( defECC, MDCFStrmerca );
			/* Inverse rectifying latitude coefficients */
			set_inverse_rectifying( defECC, RCCFStrmerca );
			M0trmerca = meridian_distance( P0, SINP0, MDCFStrmerca )*K0; 
			SLIM = -89.95;
			WLIM = check_180(lon0 - 30.0);
			NLIM = 89.95;
			ELIM = check_180(lon0 + 30.0);
			return 0;
		}
	}

const void* set_transverse_mercator(	const char* opt_name, double a, double rf, double lat0, double lon0, double k0 )
	{	void* cnsts = allocate_memory( sizeof(proj_dfn) );
		init_transverse_mercator( cnsts, (opt_name ? opt_name : LABELtrmerca), a, rf, lat0, lon0,k0 );
		return cnsts;
	}

int geo_to_transverse_mercator( const void* cnstsP, double lat, double lon, double* x, double* y )
	{	int status = 0;
		if( !cnstsP  )
			return -1;
		else
		if( (status = projection_limit_check(cnstsP, lat,lon)) < 0 )
			return -2;
		else
		{	double cfs[5],T2,T3,T4,T5;
			const proj_dfn *cnsts = (const proj_dfn*)cnstsP;
			double rphi = lat*RADDEG, rlam = lon*RADDEG, dlam = rlam - L0;
			double dlam2 = dlam*dlam;
			double sphi = sin(rphi), cphi = cos(rphi), tphi = tan(rphi);
			double T = tphi*tphi, G = EP2trmerca*cphi*cphi, cp2 = cphi*cphi;
			double N = defAXIS*K0*vertical_radius(defECC,sphi);
			double Q = 0.5*sphi*cphi*N;
			double T1 = meridian_distance( rphi,sphi, MDCFStrmerca )*K0*defAXIS;
			
			T2 = Q;
			
			Q *= (cp2/12.0);
			cfs[0] = 5.0-T; cfs[1] = 9.0; cfs[2] = 4.0;
			T3 = Q*poly(G,2,cfs);

			Q *= (cp2/30.0);
			cfs[0] = 61.0; cfs[1] = -58.0; cfs[2] = 1.0;
			cfs[0] = poly(T,2,cfs);
			cfs[1] = 270.0 - 330.0*T;
			cfs[2] = 445.0 - 680.0*T;
			cfs[3] = 324.0 - 600.0*T;
			cfs[4] =  88.0 - 192.0*T;
			T4 = Q*poly( G,4,cfs );

			Q *= (cp2/56.0);
			cfs[0] = 1385.0; cfs[1] = -3111.0; cfs[2] = 543.0; cfs[3] = -1.0;
			T5 = Q*poly(T,3,cfs);

			cfs[0] = T1; cfs[1] = T2; cfs[2] = T3; cfs[3] = T4; cfs[4] = T5;
			*y = poly( dlam2, 4, cfs ) - M0trmerca*defAXIS; 

			Q = N*cphi;
			T1 = Q;

			Q *= (cp2/6.0);
			T2 = Q*(1.0 - T + G);

			Q *= (cp2/20.0);
			cfs[0] = 5.0; cfs[1] = -18.0; cfs[2] = 1.0;
			cfs[0] = poly(T,2,cfs);
			cfs[1] = 14.0 - 58.0*T;
			cfs[2] = 13.0 - 64.0*T;
			cfs[3] = 4.0 - 24.0*T;
			T3 = Q*poly( G,3,cfs );

			Q *= (cp2/42.0);
			cfs[0] = 61.0; cfs[1] = -479.0; cfs[2] = 179.0; cfs[3] = -1.0;
			T4 = Q*poly(T,3,cfs);

			cfs[0] = T1; cfs[1] = T2; cfs[2] = T3; cfs[3] = T4;
			*x = dlam*poly( dlam2,3,cfs );

			return status;
		}
	}


int transverse_mercator_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon )
	{	const proj_dfn* cnsts = (const proj_dfn*)cnstsP;
		double phi1,sphi1,cphi1,tphi1, lam1, T,G,Q,K2, cfs[5], T1,T2,T3,T4, rho,v;
		double yy = y/defAXIS, x2 = x*x;
		double mu = (M0trmerca + yy)/K0/MDCFStrmerca[0];
		inverse_rectifying_lat( mu,sin(mu), RCCFStrmerca, &phi1 );
		sincostan( phi1, &sphi1, &cphi1, &tphi1 );
		T = tphi1*tphi1;
		rho = 1.0/(1.0-defECC2*sphi1*sphi1);
		rho *= defAXIS*(1.0-defECC2)*sqrt(rho);
		G = EP2trmerca * cphi1*cphi1;
		v = rho*(1.0+G);
		K2 = K0*K0;
		Q = tphi1/(2.0*rho*v*K2);
		K2 *= (v*v);

		T1 = Q;
		Q /= (12.0*K2);
		cfs[0] = 5.0 + 3.0*T; cfs[1] = 1.0 - 9.0*T; cfs[2] = -4.0;
		T2 = Q*poly(G,2,cfs);

		Q /= (30.0*K2);
		cfs[0] = 61.0; cfs[1] = 90.0; cfs[2] = 45.0;
		cfs[0] = poly(T,2,cfs);
		cfs[1] = 46.0; cfs[2] = -252.0; cfs[3] = -90.0;
		cfs[1] = poly( T,2,cfs+1 );
		cfs[2] = -3.0; cfs[3] = -66.0; cfs[4] = 225.0;
		cfs[2] = poly( T,2,cfs+2 );
		cfs[3] = 100.0 + 84.0*T;
		cfs[4] = 88.0 - 192.0*T;
		T3 = Q*poly( G,4,cfs );

		Q /= (56.0*K2);
		cfs[0] = 1385.0; cfs[1] = 3633.0; cfs[2] = 4095.0; cfs[3] = 1575.0;
		T4 = Q*poly( T,3,cfs);

		cfs[0] = -T1; cfs[1] = T2; cfs[2] = -T3; cfs[3] = T4;
		phi1 += x2*poly( x2,3,cfs ); 

		Q = 1.0/(v*cphi1*K0);
		T1 = Q;

		Q /= (6.0*K2);

		T2 = Q*(1.0 + 2.0*T + G );

		Q /= (20.0*K2);
		cfs[0] = 5.0; cfs[1] = 28.0; cfs[2] = 24.0;
		cfs[0] = poly(T,2,cfs);
		cfs[1] = 6.0 + 8.0*T;
		cfs[2] = -3.0 + 4.0*T;
		cfs[3] = -4.0 + 24.0*T;
		T3 = Q* poly(G,3,cfs);

		Q /= (42.0*K2);
		cfs[0] = 61.0; cfs[1] = 662.0; cfs[2] = 1320.0; cfs[3] = 720.0;
		T4 = Q*poly(T,3,cfs);

 		cfs[0] = T1; cfs[1] = -T2; cfs[2] = T3; cfs[3] = -T4;
		lam1 = L0 + x*poly(x2,3,cfs);


		*lon = check_PI(lam1)/RADDEG;
		*lat = phi1/RADDEG;  
		return projection_limit_check( cnstsP, *lat,*lon );
	}


int transverse_mercator_scale( const void* cnstsP, double lat, double lon, double* h, double* k )
	{	int status = 0;
		if( !cnstsP  )
			return -1;
		else
		if( (status = projection_limit_check(cnstsP, lat,lon)) < 0 )
			return -2;
		else
		{	const proj_dfn *cnsts = (const proj_dfn*)cnstsP;
			double rphi = lat*RADDEG, rlam = lon*RADDEG;
			double s,T,N,C,A,A2, sphi,cphi,tphi;
			sincostan( rphi, &sphi, &cphi, &tphi );
			T = tphi*tphi;
			N = vertical_radius( defECC, sphi );   
			C = EP2trmerca * cphi*cphi;
			A = check_PI(rlam-L0)*cphi;
			A2 = A*A;
			s = K0*( 1.0 + A2*( (1.0+C)/2.0 + 
						   A2*( (5.0-4.0*T + C*(42.0+13.0*C) - 28.0*EP2trmerca)/24.0 +
						   A2*(61.0-T*(148.0-16.0*T))/720.0 )));
			*k = *h = s;
			return status;
		}
	}


/* UTM functions */
int utm_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon )
	{	if( !mnlat || !mxlat || !mnlon || !mxlon)
			return -1;
		else
		{	*mnlat = -80.0; *mxlat = 84.0; 
			*mnlon = -180.0; *mxlon = 180.0;
			return 0;
		}
	}

	 
short init_utm( void* cnstsP, double a, double rf )
	{	double k0 = 0.9996;
		double lat0 = 0.0, lon0 = 0.0;
		return init_transverse_mercator
			( cnstsP, "Universal Transverse Mercator", a,rf, lat0,lon0, k0 );
	}


const void* set_utm( double a, double rf )
	{	void* cnsts = allocate_memory( sizeof(proj_dfn) );
		init_utm( cnsts, a,rf );
		return cnsts;
	}
	

short utm_to_geo( const void* tcnsts, long zone, double easting, double northing, double* lat, double* lon )
	{	const proj_dfn* cnsts = (proj_dfn*)tcnsts;
		double x = easting - FEutm;
		/* double y = ( northing < 0.0 ? (northing+FS) : northing ); */
		double y = ( northing < 0.0 ? fabs(northing)-FSutm : northing );
		short status = transverse_mercator_to_geo( cnsts, x,y, lat,lon );
		*lon += (zone*6-183);
		return status;
	}
short geo_to_utm( const void* tcnsts, double lat, double lon, long* izone, double* east, double* north )
	{	const proj_dfn* cnsts = (proj_dfn*)tcnsts;	
		short status;
		long zone = *izone;
		if( zone <= 0 || zone > 60 )	/* if a zone is provided use it, otherwise compute it. */
		{	zone = (long)(31.0 + lon / 6.0);
			if(zone >= 61) zone = 60;	
    		if(zone <= 0) zone = 1;
    		*izone = zone;
		}
    	lon -= (zone*6-183);		/* Change the longitude to offset */
		status = geo_to_transverse_mercator( cnsts, lat,lon, east,north );
		*east += FEutm;
		if( *north < 0.0 ) *north = -(*north + FSutm);
		return status;
	}


short utm_scale( const void* tcnsts, double lat, double lon, double* h, double* k )
	{	short zone = (short)(31.0 + lon / 6.0);
		const proj_dfn* cnsts = (proj_dfn*)tcnsts;	
		lon -= (zone*6-183);		/* Change the longitude to offset */
		return (short)transverse_mercator_scale( cnsts, lat,lon, h,k );
	}

int nzmg_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon )
	{	if( !cnstsP || !mnlat || !mxlat || !mnlon || !mxlon)
			return -1;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			*mnlat = SLIM; *mxlat = NLIM; 
			*mnlon = WLIM; *mxlon = ELIM;
			return 0;
		}
	}


short init_nzmg( void* cnstsP )
{	double lat0 = -41.0, lon0 = 173.0;
	double a = 6378388.000,	rf = 297.0;  /* International spheroid */
	double geolims[] = { -47.5, 164.5, -33.5, 179.999999999 };
	double grdlims[] = { 5300000.0, 1900000.0, 3000000.0, 6800000.0  };

	proj_dfn* cnsts = (proj_dfn*)cnstsP;
		if( cnsts )
	{	_init_std_pars
			( cnsts, 0, "New Zealand Map Grid", a, rf, lat0, lon0  );
		NLIM = geolims[2]; ELIM = geolims[3];
		SLIM = geolims[0]; WLIM = geolims[1];
		return 0;
	}
	return -1;
}

const void* set_nzmg()
{	proj_dfn* cnsts = (proj_dfn*)allocate_projection(1);
	init_nzmg(cnsts);
	return cnsts;
}


short nzmg_to_geo( const void* cnstsP, double x, double y, double* lat, double* lon )
{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
	nzmggp( defAXIS, P0, L0, FNnzeal,FEnzeal, lat,lon, y,x );
    *lat /= (defM_PI/180.0);
	*lon /= (defM_PI/180.0);
	*lon = check_180(*lon);
	return projection_limit_check(cnstsP, *lat,*lon);
}


short geo_to_nzmg( const void* cnstsP, double lat, double lon, double* x, double* y )
	{	int status;
		if( !cnstsP  )
			return -1;
		else
		if( (status = projection_limit_check(cnstsP, lat,lon)) < 0 )
			return -2;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			gpnzmg( defAXIS, P0, L0, FNnzeal,FEnzeal, lat*defM_PI/180.0,check_360(lon)*defM_PI/180.0, y,x );
    		return status;
    	}
	}

void free_nzmg( const void* cnstsP )	
{	free_projection(cnstsP);  }


int madl_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon )
	{	if( !cnstsP || !mnlat || !mxlat || !mnlon || !mxlon)
			return -1;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			*mnlat = SLIM; *mxlat = NLIM; 
			*mnlon = WLIM; *mxlon = ELIM;
			return 0;
		}
	}
short init_madl( void* cnstsP )
{	double a = 6378388.000,	rf = 297.0;
	double	lat0 = -(18.0 + 54.0/60.0), 
			lon0 = 46.0 + (26.0 + 13.95/60.0)/60.0;
	double 	plon0 = 2.0 + ( 20.0 + 13.95/60.0)/60.0;
	double 	azm0 = 18.0 + 54.0/60.0, k0 = 0.9995;
	double  geolims[] = {  -26.0, 42.0, -11.0, 52.0 };	 /* S,W,N,E */
	double  grdlims[] = { 0.0, -100000.0, 2000000.0, 1100000.0 }; /* B,L,T,R */
	
	proj_dfn* cnsts = (proj_dfn*)cnstsP;
	if( cnsts )
	{	double az,denom,e,es,rok,s, spn,spr;
    	_init_std_pars( cnsts, NCmada, LABELmada, a, rf, lat0,lon0 );
		e=defECC; es=defECC2; s=SINP0;
		/******  MADAGASCAR LABORDE PROJECTION ORIGIN PARAMETERS  ***/
		az = azm0*defM_PI/180.0;
		K0 = k0;
		PAR0mada = plon0*defM_PI/180.0;
		/*** DERIVE OTHER PROJECTION CONSTANTS ***
     		** ORIGIN LATITUDE TERMS **/
		/*** RADIUS OF CURVATURE IN MERIDIAN ***/
    	denom = sqrt(1.0-es*s*s);
    	spr = a*(1.0-es)/pow(denom,3.0);
		/*** RADIUS OF CURVATURE IN PRIME VERTICAL ***/
    	spn = a/sqrt(1.0-es*s*s);
		/*** RADIUS OF GAUSS SPHERE ****/
    	SRADmada = sqrt(spr*spn);
		/** LATITUDE OF ORIGIN ON SPHERE ***/
    	SP0mada = atan(tan(P0)*sqrt(spr/spn));
		/*** RATIO OF DIFFERENCE IN LONGITUDE - SPHERE TO ELLIPSOID ***/
    	ALmada = SINP0/sin(SP0mada);
		/** CONSTANT ***/
    	CNSTmada = AL*e*log((1.0+e*s)/(1.0-e*s))/2.0 -
    			AL*log(tan(defM_PI/4.0+P0/2.0))+log(tan(defM_PI/4.0+SP0mada/2.0));
		/*** (RADIUS OF GAUSS SPHERE)X(ORIGIN SCALE FACTOR) */
    	rok = SRADmada * k0;
		/** CONSTANTS OF TRANSFORMATION -  A,B,C & D */
    	CAmada = (1.0-cos(2.0*az))/(12.0*pow(rok,2.0));
    	CBmada = sin(2.0*az)/(12.0*pow(rok,2.0));
    	CCmada = 3.0*(CAmada*CAmada-CBmada*CBmada);
    	CDmada = 6.0*CAmada*CBmada;
    	NLIM =  geolims[2]; ELIM = geolims[3];
    	SLIM =  geolims[0]; WLIM = geolims[1];
		return 0;
	}
    return -1;;
}


const void* set_madl()
{	proj_dfn* cnsts = (proj_dfn*)allocate_projection(1);
	init_madl( cnsts );
    return cnsts;
}


short geo_to_madl( const void* cnstsP, double lat, double lon, double* x, double *y )
	{	int status = 0;
		if( !cnstsP  )
			return -1;
		else
		if( (status = projection_limit_check(cnstsP, lat,lon)) < 0 )
			return -2;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			double xgs,ygs;
			gpmadl( defAXIS, defECC, lat*defM_PI/180.0, lon*defM_PI/180.0, y,x, &ygs,&xgs,SP0mada,L0, FNmada,FEmada, K0,AL, CNSTmada, SRADmada, CAmada,CBmada ); 
			return status;
		}
	}


short madl_to_geo( const void* cnstsP, double x, double y, double* lat, double *lon )
{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
	double xgs, ygs;
	madlgp( defAXIS, defECC, lat, lon, y,x, &ygs,&xgs,P0,SP0mada,L0, FNmada,FEmada, K0,AL, CNSTmada, SRADmada, CAmada,CBmada,CCmada,CDmada );
	*lat *= ( 180.0/defM_PI); *lon *= (180.0/defM_PI); 
	return projection_limit_check(cnstsP, *lat,*lon);
} 


void free_madl( const void* cnstsP )
{	free_projection( (void*)cnstsP );  }


int polar_stereographic_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon )
	{	if( !cnstsP || !mnlat || !mxlat || !mnlon || !mxlon)
			return -1;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			*mnlat = SLIM; *mxlat = NLIM; 
			*mnlon = WLIM; *mxlon = ELIM;
			return 0;
		}
	}
	 
short init_polar_stereographic( void* cnstsP, const char* ename, double a, double rf, double lat0, double lon0, double k0 )
	{	double phi1 = HALF_PI, lam0 = lon0*RADDEG;
		if( lat0 < 0.0 ) phi1 = -phi1;
		if( !cnstsP )
			return -1;
		else
		{ 	proj_dfn* cnsts = (proj_dfn*)cnstsP;
			_init_std_pars( cnsts,NCps,LABELps, a,rf, phi1,lon0 );
			K0 = k0;
			C0ps = 2.0*defAXIS * pow( (1.0-defECC)/(1.0+defECC), 0.5*defECC ) / sqrt(1.0-defECC2); 
			/* Inverse conformal latitude coefficients */
			set_inverse_conformal( defECC, CFCFSps );
			NLIM = ( lat0 >= 0.0 ? 90.0 : 0.0 ); ELIM = 180.0;
			SLIM = ( lat0 >= 0.0 ? 0.0 : -90.0 ); WLIM = -180.0;
			return 0;
		}
	}

const void* set_polar_stereographic( const char* ename, double a, double rf, double lat0, double lon0, double k0  )
	{	void* cnsts = allocate_memory(sizeof(proj_dfn));
		init_polar_stereographic( cnsts, LABELps, a,rf, lat0, lon0,k0 );
		return cnsts;
	}


short geo_to_polar_stereographic( 	const void* cnstsH, double lat, double lon, double* x, double* y )
	{	int status = 0;
		if( !cnstsH )
			return -1;
		else
		if( (status = projection_limit_check(cnstsH, lat,lon)) < 0 )
			return -2;
		else
		if( fabs(fabs(lat) - HALF_PI) < EPSILON )
		{	*x = *y = 0.0;
			return 0;
		}
		else
		{	const proj_dfn* cnsts = (const proj_dfn*)deref(cnstsH);
			double pole = ( P0 < 0.0 ? 1.0 : -1.0 );
			double rphi = fabs(lat)*RADDEG, rlam = lon*RADDEG;
			double dlam = check_PI( rlam - L0 );
			double sphi = sin(rphi);
			double v = defECC*sphi;
			double t = tan( 0.5*(HALF_PI - rphi) ) * pow( (1.0+v)/(1.0-v), 0.5*defECC );
			double rho = K0*C0ps*t;
	
			*x = rho * sin(dlam);
			*y = rho * cos(dlam) * pole;
			return status;
		}
	}

	
short polar_stereographic_to_geo( const void* cnstsH, double x, double y, double* lat, double* lon )
	{	double rphi,rlam;
		double R,X;
		const proj_dfn* cnsts = (const proj_dfn*)deref(cnstsH);
		double pole = ( P0 < 0.0 ? -1.0 : 1.0 );
		int xzero = fabs(x) < EPSILON, yzero = fabs(y) < EPSILON;
		if( xzero && yzero )
		{	*lat = affix_sign( 90.0,pole );
			*lon = L0/RADDEG;
			return 0;
		}
		
		/* Longitude */
	    rlam = ( yzero ? affix_sign( HALF_PI, x ) : 
					atan2( x,affix_sign(y,-pole) ));
		/* Radius */
		R = fabs( yzero ? x : ( xzero ? y : 
				( fabs(x) < fabs(y) ? y/cos(rlam) : x/sin(rlam) )));
		/* Latitude	*/	
		X = HALF_PI - 2.0*atan(R/(K0*C0ps));
		inverse_conformal_lat( X, sin(X), CFCFSps, &rphi );
		*lat = affix_sign( rphi/RADDEG, pole );
		*lon = check_PI(rlam+L0)/RADDEG;
		if( y < 0 ) *lon = check_180(180.0 - *lon);
		return projection_limit_check(cnsts, *lat,*lon);
	}


short polar_stereographic_scale( const void* cnstsH, double lat, double lon, double* h, double* k )
	{	const proj_dfn* cnsts = (const proj_dfn*)deref(cnstsH);
		double rphi = fabs(lat)*RADDEG;
		double sphi = sin(rphi);
		double v = defECC*sphi;
		double t = tan( 0.5*(HALF_PI - rphi) ) * pow( (1.0+v)/(1.0-v), 0.5*defECC );
		double rho = K0*C0ps*t;
		double s = rho/( defAXIS*cos(rphi)*vertical_radius(defECC,sphi) );
		*k = *h = s;
		return 0;
	}

/* Universal Polar Stereographic */


int ups_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon )
	{	if( !cnstsP || !mnlat || !mxlat || !mnlon || !mxlon)
			return -1;
		else
		{	const proj_dfn* cnsts = (proj_dfn*)cnstsP;
			*mnlat = SLIM; *mxlat = NLIM; 
			*mnlon = WLIM; *mxlon = ELIM;
			return 0;
		}
	}

 
short init_ups( void* cnstsP, double a, double rf,double lat0 )
	{	/* double a = 6378388.0, rf = 297.0;  */	/* International */
		double fn = 2000000.0, fe = 2000000.0;
		double fs = DOUBLE_NULL, fw = DOUBLE_NULL;
		double k0 = 0.994;
		double lon0 = 0.0;
		return init_polar_stereographic
				( cnstsP, "Universal Polar Stereographic",
					 a,rf, lat0, lon0, k0 );
	}
const void* set_ups( double a, double rf, double lat0 )
	{	void* cnsts = allocate_memory(sizeof(proj_dfn));
		init_ups( cnsts,a,rf,lat0 );
		return cnsts;
	}
short ups_to_geo( const void* tcnsts, double easting, double northing,double* lat, double* lon )
	{	short status;
		short izone = ( northing < 0.0 ? -1 : 0 );
		status = 
			polar_stereographic_to_geo
				( tcnsts, easting-FEups,fabs(northing)-FNups, lat,lon );  
		if( status == 0 )
		{	if( ( *lat < 0.0 && *lat > SOUTHLIMups ) ||
				( *lat >= 0.0 && *lat < NORTHLIMups ) )
				status = 2;
		}
		return status;
	}


short geo_to_ups( const void* tcnsts, double lat, double lon, double* easting, double*  northing )
	{	short status, south = lat < 0.0;
		status = 
			geo_to_polar_stereographic
				( tcnsts, lat,lon, easting,northing );  
		*northing += FNups;
		*easting += FEups;
		if( south ) *northing = -fabs(*northing);
		if( status == 0 )
		{	if( ( south && lat > SOUTHLIMups ) ||
				( !south && lat < NORTHLIMups ) )
				return -2;
		}
		return status;
	}


short ups_scale( const void* tcnsts, double lat, double lon, double* h, double* k )
	{	return polar_stereographic_scale( tcnsts, lat,lon, h,k );  }


short init_mgrs( void* cnstsP, double a, double rf )
	{	proj_dfn* cnsts = (proj_dfn*)cnstsP;
		if( cnsts )	
		{	init_utm(cnsts,a,rf);
			*defGROUP = select_group(a,rf);
			AZONE[0] = 0;
			return 0;
		}
		else
			return -1;
	}


short mgrs_to_geo( const void* tcnsts, const char* mgrs, double* lat, double* lon, long* izone, double* x, double* y )
	{	char tmp[64];
		short k = _mgrtxx( (proj_dfn*)tcnsts, strcpy(tmp,mgrs),
                                   lat, lon, izone, y,x );
		return ( k == 0 ? mgrs_limit_check( tmp,*lat,*lon ) : k ); 
	} 
		 
short geo_to_mgrs( const void* tcnsts, double lat, double lon, char* mgrs,long* zone, double* x, double* y ) 
	{	xxtmgr( (proj_dfn*)tcnsts, &lat,&lon, zone,y,x, mgrs, 1 );
		return VMAPOK;
	} 

short dtcc_set_coord3A( COORD3* c3P, const char* v, short k )
	{	if( !c3P )
			return ILLEGAL_ARG;
		else
		{	int n = strlen(v);
			if( n >= 23 ) n = 22;
			_COORD3Z(c3P) = k;
			strncpy( _COORD3A(c3P), v, n );
			if( dtcc_coord3_is_MGRS(k) ) 
				while( n < 23 ) _COORD3A(c3P)[n++] = ' ';
			_COORD3A(c3P)[n] = 0;
			return VMAPOK;
		}
	}

int mgrs_limits(	const void* cnstsP, double* mnlat, double* mnlon, double* mxlat, double* mxlon )
	{	if( !cnstsP || !mnlat || !mxlat || !mnlon || !mxlon )
			return -1;
		else
		{	*mnlat = -90.0; *mxlat = 90.0; 
			*mnlon = -180.0; *mxlon = 180.0;
			return 0;
		}
	}


int isFN(const char* fn )
{	if( !fn )
		return 0;
	else
		return ( strchr(fn,'.') != NULL ? 1 : 0 );  
}

short dtcc_initialize_ellipsoid_list( const char* fname ) 
	{	FILE* fp = _scan_list_hdr( &elist, _check_ellipsoid_name(fname) );
		if( !fp )
			return FILE_NOT_FOUND;
		else
		{	int i, n = _list_total(elist);
			_list_dataP(elist) = dtcc_allocate_ptr( (n+1)*sizeof(ELLIPS_PARAMS));
			if( !_list_dataP(elist) )
			{	dtcc_delete_ellipsoid_list(&elist);
				fclose(fp);
				return MEMORY_ALLOCATION_FAILURE;
			}
			else
			{	ELLIPS_PARAMS* elP = (ELLIPS_PARAMS*)_list_dataP(elist);
				for( i=0; i<n; i++, elP++ )
				   dtcc_scan_ellipsoid( elP,fp );
				fclose(fp);
				_list_modified(elist) = 0;
				return VMAPOK;
			}
		}
	}

static const ELLIPS_PARAMS* _find_name( DTCC_LABELS* idP, int type )
	{	short k, start,stop;
		ELLIPS_PARAMS* elP = (ELLIPS_PARAMS*)_list_dataP(elist);
		const char* lbl = dtcc_nameP(idP);
		_check_listellipse( type, &start, &stop );
		for( elP += start, k=start; k<stop; k++, elP++ )
			if( strcmp( lbl, dtcc_nameP(elP) ) == 0 ||
				( strcmp( lbl, dtcc_abbrvP(elP) ) ) == 0 && 
					strncmp(dtcc_abbrvP(elP), "XX",2) != 0  )
						return elP;
		return NULL;
	}

short _reset_units(  UNITS_INFO* uiP,const char* name,int dim )
	{	int k = _findIt( ( dim == 1 ? name : strchr(name,' ')+1) );
    	if( k >= 0  )
    	{	char abfr[64];
			sprintf( abfr,"%s%s", 
				( dim == 1 ? "" : ( dim == 2 ? "sq " : "cu " )),_std_units[k].abbrv );
    		dtcc_set_label
    			( uiP, name, abfr, _std_units[k].fmt, 1, _std_units[k].type );
			uiP->cf = _std_units[k].cf;
			if( dim > 1 ) uiP->cf *= uiP->cf;
			if( dim > 2 ) uiP->cf *= uiP->cf;
			uiP->scaled = 0;
			return VMAPOK;
		}
    	else
    	{	uiP->cf = 1.0;
			uiP->scaled = 0;
			dtcc_set_label( uiP, NULL,NULL, NULL,1, 0 ); 
    		return ITEM_NOT_FOUND;
		}
    }

static double _compute_ecc( double f )
	{	/* if( f > 0.0 ) f = 1.0/f; */
		/* extended80 xf = f; */
		long double xf = f;
		if( xf > 0.0 )
		{	xf = 1.0/xf;
			f = sqrt(xf*(2.0-xf));
		}
		else
			f = 0.0;
		return f;
	}

short dtcc_make_ellipsoid_name( ELLIPS_PARAMS* csP, char* nm )
	{	if( csP && nm )
		{	char *a, *e;
			short k;
			k = dtcc_is_ellipsoid_axis_OK(dtcc_ellipsoid_axis(csP));
			a = ( k < 0 ? "SMALL_" : ( k > 0 ? "BIG_" : "") );
			k = dtcc_is_ellipsoid_flattening_OK(dtcc_ellipsoid_rflat(csP));
			e = ( k == ELLIPSOID_IS_SPHERE ? "SPHERE" : 
				( k < 0 ? "ODD_ELLP" : "ELLP") ); 
		
			sprintf( nm, "%s%s_%lf_%lf_%ld", a,e, 
					dtcc_ellipsoid_axis(csP), dtcc_ellipsoid_rflat(csP), csP );
	    	return 0;
		}
		else
			return -1;
	}

short dtcc_validate_ellipsoid(	const ELLIPS_PARAMS* csP)
	{	
		_check_itellips( (ELLIPS_PARAMS*)csP );
		return ( dtcc_valid_flag(csP) ? VMAPOK : 1 );  
	}

short _init_std_pars( proj_dfn* cnsts, int k, const char* name, double a, double rf, double lat0, double lon0  )
	 {	if( !cnsts )
	 		return -1;
		else
		{	int n = _NOTHER;
			cnsts->id = 1; /* sizeof(proj_dfn); */
			while( --n >= 0 ) cnsts->other[n] = DOUBLE_NULL;
			K0 = AZ0 = STD1 = STD2 = DOUBLE_NULL;
			ELIM = 180.0; WLIM = -180.0;NLIM = 90.0; SLIM = -90.0;
			NCNSTS = k;
			P0 = lat0*RADDEG; sincostan( P0, &SINP0, &COSP0,NULL ); 
			L0 = check_PI(lon0*RADDEG);
			defAXIS = a;
			if( rf <= 0.0 )
				defECC2 = 0.0;
			else
			if( rf > 1000000.0 )
				defECC2 = 1.0 - (rf*rf)/(defAXIS*defAXIS); 
			else
			if( rf > 1.0 )
				defECC2 = eccentricity2(rf);
			else
				defECC2 = rf; 
			defECC = sqrt(defECC2); 
			WH = ( fabs(P0) < EPSILON ? 0 :
				 ( P0 > HALF_PI - EPSILON ? 1 :
				 ( P0 < -HALF_PI + EPSILON ? -1 : 2 )));
			if( name[0] == 'Z' )
			{	strcpy( NM, ( EQUATORIAL  ? "Equatorial " : 
							( POLAR ? "Polar " : "Oblique " )) );
				strcat( NM, name+1 );
			}
			else
				strcpy( NM,name );
			return 0;
		}
	}

void set_inverse_authalic( double e, double* aucoefs )
{   if( e < EPSILON )
		aucoefs[0] = 0.0;
	else
	{	double e2 = e*e;
		double e4 = e2*e2;
    	aucoefs[0] = e2*( (1.0/3.0) + e2*( (31.0/180.0) + e2*(517.0/5040.0) ));      
    	aucoefs[1] = e4*( (23.0/360.0) + e2*(251.0/3780.0) );
    	aucoefs[2] = e4*e2*(761.0/45360.0);
    	aucoefs[3] = 0.0; 
    	_convertSin2468(aucoefs);
    }
   	 return;
}

double parallel_radius( double e, double sphi )  
	{	double esp = sphi*e;
		return sqrt( (1.0-sphi*sphi)/(1.0- esp*esp) );
	}

double authalic_q( double e, double sphi ) 
{	if( e < EPSILON )
		return sphi;
	else
	{	double es = e*sphi; 
		return sphi/( 1.0 - es*es ) - log( (1.0-es)/(1.0+es) )/(e+e);
	} 
}

double square( double x )
{	
	return x*x;  
}

int projection_limit_check( const void *cnstsP, double lat, double lon )
{	double min,max;
	int status1 = 0, status2 = 0, outofrange = 17;
	proj_dfn* cnsts = (proj_dfn*)cnstsP;
	if( lat < SLIM || lat > NLIM )
	{	min = SLIM-2.0; max = NLIM+2.0;
		if( min < -90.0 ) min = -90.0;
		if( max > 90.0 ) max = 90.0;
		if( lat > max || lat < min )
			return -outofrange;
		else
			status1 = outofrange;
	}

	if( WLIM < ELIM && lon <= ELIM && lon >= WLIM  )
		return status1;
	else
	if( WLIM > ELIM && (lon <= ELIM || lon >= WLIM) )
		return status1;
	else
	{	min = check_180(WLIM-2.0); max = check_180(ELIM+2.0);
		if( min < max)
			return ( lon <= max && lon >= min ? outofrange : -outofrange );
		else
			return ( lon <= ELIM || lon >= WLIM ? outofrange : -outofrange ); 
	}
}

void sincostan( double a, double* sina, double* cosa, double* tana )
	{	if( sina ) *sina = sin(a); 
		if( cosa ) *cosa = cos(a);  
		if( tana ) *tana = tan(a);
		return;
	}

double check_PI( double a )
	{	int k=2;
		while( fabs(a) > PI && --k >= 0 )
		{	if( a > PI )
				a -= TWO_PI;
			else
			if( a < -PI )
				a += TWO_PI;
		}
		return a;
	}

void inverse_authalic_lat( double B, double sinB, const double* aucoefs, double* phi ) 
{	if( aucoefs[0] == 0.0 )
		*phi = B;
	else
		*phi = B +  _fSin2468( sinB, aucoefs );
	return;
}

void set_inverse_rectifying( double e, double* fpcoefs )
{	if( e < EPSILON )
		fpcoefs[0] = 0.0;
	else
	{	double n = constN(e);
		double n2 = n*n;
		fpcoefs[0] = n*(3.0/2.0 - n2*27.0/32.0);
		fpcoefs[1] = n2*( 21.0/16.0 -n2*55.0/32.0 );
		fpcoefs[2] = n2*n*151.0/96.0;
		fpcoefs[3] = n2*n2*1097.0/512.0;
   	 	_convertSin2468(fpcoefs); 
   	}            
	return;
}			

void set_meridian_distance( double e, double* mdcoefs ) 
{   if( e < EPSILON )
		mdcoefs[0] = 1.0;
	else
	{	double e2 = e*e;
		double e4 = e2*e2;
		mdcoefs[0] = 1.0 - e2*( 1.0/4.0 + e2*( 3.0/64.0 + e2*(5.0/256.0) ));
    	mdcoefs[1] =  -e2*( (3.0/8.0) + e2*( (3.0/32.0) + e2*(45.0/1024.0) ));
    	mdcoefs[2] = e4*( (15.0/256.0) + e2*(45.0/1024.0) );
    	mdcoefs[3] = -e4*e2*(35.0/3072);
    	mdcoefs[4] = 0.0; 
    	_convertSin2468(mdcoefs+1);
    }             
    return;
}

double meridian_distance( double phi, double sphi, const double* mdcoefs ) 
{	if( mdcoefs[0] == 1.0 )
		return phi;
	else
		return phi*mdcoefs[0] + _fSin2468(sphi, mdcoefs+1); 
}

void* allocate_projection( int k )
	{	return allocate_memory( k*sizeof(proj_dfn) );  }

void inverse_rectifying_lat( double R, double sinR, const double* fpcoefs, double* phi ) 
{	if( fpcoefs[0] == 0.0 )
		*phi = R;
	else
		*phi = R + _fSin2468( sinR, fpcoefs );  
	return;
} 

void authalic_lat( double e, double phi, double sphi, double* B, double* sinB ) 	
{	if( e <= 0.0 )
	{	if(B) *B = phi;
		if(sinB) *sinB = sphi;
	}
	else
	{	double q = authalic_q(e,sphi);
		double q90 = authalic_q(e,1.0);
		double sinv = q/q90;
		if(sinB) *sinB = sinv;
		if(B) *B = asin(sinv);
	}
	return;
}

void set_inverse_conformal( double e, double* cfcoefs )
{   if( e < EPSILON )
		cfcoefs[0] = 0.0;
	else
	{	double e2 = e*e;
		double e4 = e2*e2;
  	  	cfcoefs[0] = e2*( (1.0/2.0) + e2*( (5.0/24.0) + e2*(1.0/12.0) + e2*(13.0/360.0)));      
    	cfcoefs[1] = e4*( (7.0/48.0) + e2*(29.0/240.0) + e2*(811.0/11520.0));
    	cfcoefs[2] = e4*e2*(7.0/120.0 + e2*(81.0/1120.0));
    	cfcoefs[3] = e4*e4*(4279.0/161280.0); 
    	_convertSin2468(cfcoefs);
    }             
    return;
}

double tanz( double ecc, double phi, double sphi )
{	double f = ( 1 - ecc*sphi)/( 1 + ecc*sphi );
	double z = tan( 0.5*(HALF_PI-phi) )/pow( f, 0.5*ecc );
	return z;
} 

void free_projection( const void* dfnP )
	{	if( dfnP ) free_memory((void*)dfnP); return;  }


void r_and_theta( int which, double x, double y, double* r, double* t )
	{	if( r ) *r = sqrt( x*x + y*y );
		if( t ) *t = atan2( x, ( which < 0 ? y : -y ) );
		return;
	}

double affix_sign( double x, double sgn )
	{	double fx = fabs(x); 
		return ( sgn < 0.0 ? -fx : fx );  
	}	

void inverse_conformal_lat( double X, double sinX, const double* cfcoefs, double* phi ) 
{	if( cfcoefs[0] == 0.0 )
		*phi = X;
	else
		*phi = X + _fSin2468( sinX, cfcoefs );
	return ;
}

double tanHZ( double e, double sphi )
{	double esp = e*sphi;
	double v1 = ( 1.0 + sphi ) / ( 1.0 - sphi );
	double v2 = ( 1.0 - esp ) / ( 1.0 + esp );
	return sqrt(v1*pow( v2,e ));
}

/* Radius  of curvature of prime vertical @ phi	*/
double vertical_radius( double e, double sphi )
	{	double esp = e*sphi;
		return 1.0/sqrt(1.0-esp*esp);  
	}

void conformal_lat( double e, double phi, double sphi, double* X  ) 
{	if( e < EPSILON )
		*X = phi;
	else
	{	double esp = e*sphi;
		double v1 = ( 1.0 + sphi ) / ( 1.0 - sphi );
		double v2 = ( 1.0 - esp ) / ( 1.0 + esp );
		double hz = sqrt(v1*pow( v2,e ));
		*X = 2.0*atan(hz) - HALF_PI;
	}   
	return;
}	

static double poly( double x, int n, double* c )
{	double p = 0.0;
	while( n >= 0 ) p = p*x + c[n--];
	return p;
}

static void nzmggp(double a, double ophi, double olam, double fn, double fe,double *sphi, double *slam, double y, double x)
{
	static DCOMPLEX bi[6] = {
    {1.3231270439,0.0},{-0.577245789,-7.809598E-3},{0.508307513,-0.112208952},
    {-0.15094762,0.18200602},{1.01418179,1.64497696},{1.9660549,2.5127645}
		};
	static double tphi[9] = {
    1.5627014243,0.5185406398,-3.333098E-2,-0.1052906,-3.68594E-2,7.317E-3,
    1.22E-2,3.94E-3,-1.3E-3
	};
/*static*/ double ten = 10.0;
/*static*/ double dphi,dlam,dpsi,dx,dy;
/*static*/ long i,j;
/*static*/ DCOMPLEX arg1,arg2,xi,z;

/*
     *** DELTA X & DELTA Y ***
*/
    dx = x-fe;
    dy = y-fn;
/*
     *** INSERT DELTAS INTO REAL & IMAGINARY PART OF Z ***
         * ADJUSTED FOR SEMI-MAJOR AXIS (A) *
*/
    z = dpxdpx(dy/a,dx/a);
/*
     ** SET COMPLEX VARIABLE XI TO ZERO
*/
    xi = dpxdpx(0.0,0.0);
/*
     ** COMPUTE FIRST APPROXIMATION **
*/
    for(i=1; i<=6; i++) xi = dpxadd(xi,dpxmul(bi[(int)i-1],dpxlongpow(z, i)));
/*
     ** ITERATE NEXT APPROXIMATION TWICE TO GET FINAL VALUE OF XI **
*/
    for(j=1; j<=2; j++) {
/*
     ** NUMERATOR - ARG1 **
*/
        arg1 = z;
        for(i=2; i<=6; i++)
            arg1 = dpxadd( arg1,
                dpxmul( dpxmul(bf[(int)i-1],dpxdbl((double)(i-1))),dpxlongpow(xi,i) ) );
/*
     ** DENOMINATOR - ARG2 **
*/
        arg2 = bf[0];
        for(i=2; i<=6; i++)
            arg2 = dpxadd( arg2,
                dpxmul( dpxmul(bf[(int)i-1],dpxdbl((double)i)),dpxlongpow(xi,i-1) ) );
/*
     ****
*/
        xi = dpxdiv(arg1,arg2);
    }
/*
     *** REAL PART IS DIFFERENCE IN ISOMETRIC LATITUDE ***
*/
    dpsi = dpxreal(xi);
/*
     *** DIFFERENCE IN LATITUDE ***
*/
    dphi = 0.0;
    for(i=1; i<=9; i++) dphi += (tphi[(int)i-1]*pow(dpsi,(double)i));
/*
     *** DIFFERENCE IN LATITUDE IN SECONDS OF ARC ***
*/
    dphi *= pow(ten,5.0);
/*
     *** CONVERT TO RADIANS AND ADD TO ORIGIN LATITUDE ***
*/
    *sphi = ophi + dphi * defM_PI / (3600.0*180.0);
/*
     ** DIFFERENCE IN LONGITUDE FROM IMAGINARY PART OF XI **
*/
    dlam = dpxima(xi);
/*
     ** LONGITUDE OF POINT IN RADIANS **
*/
    *slam = olam+dlam;
    return;
}

static void gpnzmg(double a, double ophi, double olam, double fn, double fe, double sphi, double slam, double *y, double *x)
{   /*static*/ double ten = 10.0;
    /*static*/ double dlam,dphi,dpsi;
    /*static*/ long i;
    /*static*/ DCOMPLEX xi,z;

/*
     *** DIFFERENCE IN LATITUDE - RADIANS ***
*/
    dphi = sphi-ophi;
/*
     ** UNDERFLOW TRAP TEST **
    unflow(&dphi,1L);
*/
/*
     ** CONVERT TO SECONDS OF ARC & SCALE DOWN BY POWER OF 10**5
*/
    dphi = dphi*3600.0*180.0/defM_PI/pow(ten,5.0);
/*
     *** DIFFERENCE (DELTA) PSI (SECONDS ) - ISOMETRIC LATITUDE ***
*/
    dpsi = 0.0;
    for(i=1; i<=10; i++) dpsi += (tpsi[(int)i-1]*pow(dphi,(double)i));
/*
     *** DIFFERENCE IN LONGITUDE - RADIANS
*/
    dlam = slam-olam;
/*
     ** UNDERFLOW TRAP TEST **
    unflow(&dlam,1L);
*/
/*
     *** INSERT DPSI AND DLAM INTO COMPLEX VARIABLE ***
*/
    xi = dpxdpx(dpsi,dlam);
/*
     *** SET GRID COMPONENT COMPLEX VARIABLE TO ZERO ***
*/
    z = dpxdpx(0.0,0.0);
/*
     *** USING COMPLEX NOTATION COMPUTE DIFFERENT COMPONENTS ***
*/
    for(i=1; i<=6; i++) z = dpxadd(z,dpxmul(bf[(int)i-1],dpxlongpow(xi,i)
      ));
/*
     *** NORTHING - REAL PART OF Z ***
*/
    *y = fn+a*dpxreal(z);
/*
     *** EASTING - IMAGINARY PART OF Z ***
*/
    *x = fe+a*dpxima(z);
    return;
}

double check_360( double a )
	{	return ( a < 0.0 ? a+360.0 : ( a >=360.0 ? a-360.0 : a ));
	}

static void gpmadl(double a,double e,double sphi,double slam, double *y,double *x,double *ygs,double *xgs,double ophis,double olam,double fn,double fe,double ok,double al,double Const,double r,double ca,double cb )
{	double s,c,dlam,ri,rii,riii,riv,rv,rvi,sphis,t,v1,v2;
	double es = e*e;
	/**** LATITUDE ON SPHERE ****/
    v1 = al*log(tan(defM_PI/4.0+sphi/2.0));
    v2 = al*e*log((1.0+e*sin(sphi))/(1.0-e*sin(sphi)))/2.0;
    sphis = 2.0*(atan(exp(v1-v2+Const))- defM_PI / 4.0);
	/**** ROMAN NUMERAL TERM I ****/
    ri = r*ok*(sphis-ophis);
	/**** TRIG FUNCTIONS FOR LATITUDE OF SPHERE ****/
    s = sin(sphis);
    c = cos(sphis);
    t = tan(sphis);
    rii = r*s*c*al*al*ok/2.0;
    riii = r*s*pow(c,3.0)*(5.0-t*t)*pow(al,4.0)*ok/24.0;
    riv = r*c*al*ok;
    rv = r*pow(c,3.0)*(1.0-t*t)*pow(al,3.0)*ok/6.0;
    rvi = r*pow(c,5.0)*(5.0-18.0*t*t+pow(t,4.0))*pow(al,5.0)*ok/120.0;
	/*** DELTA LONGITUDE ** DIFFERENCE BETWEEN LONGITUDE AND CENTRAL MERIDIAN **/
    dlam = slam-olam;
	/**** GAUSS-SCHREIBER GRID COORDINATES ****/
    *ygs = ri+pow(dlam,2.0)*(rii+pow(dlam,2.0)*riii);
    *xgs = dlam*(riv+pow(dlam,2.0)*(rv+pow(dlam,2.0)*rvi));
    v1 = 3.0**xgs*pow(*ygs,2.0)-pow(*xgs,3.0);
    v2 = pow(*ygs,3.0)-3.0*pow(*xgs,2.0)**ygs;
	/**** LABORDE GRID COORDINATES ****/
    *y = fn+*ygs-cb*v1+ca*v2;
    *x = fe+*xgs+ca*v1+cb*v2;
    return;
}

static void madlgp(double a,double e,double *sphi,double *slam,double y,double x,double *ygs,double *xgs,double ophi,double ophis,double olam,double fn,double fe,double ok,double al,double Const,double r,double ca, double cb, double cc, double cd)
{	double 	c,denom,fte,fts,rvii,rviii,rix,rx,rxi,
			sphis,spr,t,v1,v2,v3,v4,xb,yb;
	long i;
	double es = e*e;
	/**** DELTA X & Y ****/
    xb = x-fe;
    yb = y-fn;
	/**** GAUSS-SCHREIBER GRID COORDINATS ****/
    v1 = 3.0*xb*pow(yb,2.0)-pow(xb,3.0);
    v2 = pow(yb,3.0)-3.0*xb*xb*yb;
    v3 = pow(xb,5.0)-10.0*pow(xb,3.0)*pow(yb,2.0)+5.0*xb*pow(yb,4.0);
    v4 = 5.0*pow(xb,4.0)*yb-10.0*pow(xb,2.0)*pow(yb,3.0)+pow(yb,5.0);
    *xgs = xb-ca*v1-cb*v2+cc*v3+cd*v4;
    *ygs = yb+cb*v1-ca*v2-cd*v3+cc*v4;
	/**** FOOT POINT LATITUDE - SPHERE ****/
    fts = ophis+*ygs/(r*ok);
	/**** FOOT POINT LATITUDE ELLIPSOID ****/
    fte = fts+(ophi-ophis);
	/**** ITERATE ****/
    for(i=1; i<=10; i++) {
	v1 = al*log(tan(defM_PI/4.0+fte/2.0));
	v2 = al*e*log((1.0+e*sin(fte))/(1.0-e*sin(fte)))/2.0;
        sphis = 2.0*(atan(exp(v1-v2+Const))-defM_PI/4.0);
        fte += (fts-sphis);
    }
	/*** RADIUS OF CURVATURE MERIDIAN - FOOT POINT LATITUDE ELLIPSOID ***/
    denom = sqrt(1.0-es*pow(sin(fte),2.0));
    spr = a*(1.0-es)/pow(denom,3.0);
	/*** TRIG FUNCTIONS - FOOT POINT LATITUDE - SPHERE ***/
    c = cos(fts);
    t = tan(fts);
	/*** ROMAN NUMERAL TERMS   VII-XI ***/
    rvii = t/(2.0*spr*r*ok*ok);
    rviii = t*(5.0+3.0*t*t)/(24.0*spr*pow(r,3.0)*pow(ok,4.0));
    rix = 1.0/(c*r*ok*al);
    rx = (1.0+2.0*t*t)/(6.0*c*pow(r,3.0)*pow(ok,3.0)*al);
    rxi = (5.0+28.0*t*t+24.0*pow(t,4.0))/(120.0*c*pow(r,5.0)*pow(ok,5.0)*al);
	/**** LATITUDE ****/
    *sphi = fte-rvii*pow(*xgs,2.0)+rviii*pow(*xgs,4.0);
	/*** LONGITUDE ****/
    *slam = olam+rix**xgs-rx*pow(*xgs,3.0)+rxi*pow(*xgs,5.0);
    return;
}

static short select_group( double a, double rf )
	{	short group = 0;
		if( a == 6377397.155 || a == 6378249.145 )	/* Bessel 1841 & Clarke 1880 */
			group = 2;
		else
		if( a == 6378206.4 )   /* Clarke 1866 */
			group = 3;
		else
			group = 1;
		return group;
	}

int fifichar ( unsigned char *c1 )
{
	return (int) *c1;
}


/*
FTNCMS: FORTRAN Compare Strings

Copyright 1988-90 PROMULA Development Corporation ALL RIGHTS RESERVED

Author: Fred Goodman
Revised: 08/16/89, 9:30 AM to reflect the version 2 platform approach.

Synopsis:
*/
int ftncms( char* s1, int n1, char* s2, int n2)
{
/*
Description:

Compares two FORTRAN style strings to determine their lexical
relationship. The comparison proceeds character by character
until either the end of both stings is reached or until a spec-
ific character difference is found. If the strings are of unequal
length, the shorter string is treated as though it were padded
with blanks to the length of the longer string. Note that the
"lexical" value of the character is obtained from the "fifichar"
function. Thus, this function does not necessarily assume the
display values of the host processor, but rather can assume the
display code values of another processor.

Return value:

 0  if no character difference is found
-1  if a character in the first string is less than the corres-
    ponding character in the second string.
+1  if a character in the first string is greater that the cor-
    responding character in the secong string.

Process: Note the hostile user assumption in the while statement.
If a simple zero test were used and if a negative length were
sent then a very long loop would result. Other than that the code 
is straightforward.
*/
auto int blank;              /* Display code for a blank */
auto int c1;                 /* Current display code from s1 */
auto int c2;                 /* Current display code from s2 */

    
  /*blank = fifichar(" ");*/
    blank = fifichar((unsigned char *)" ");
    while(n1 > 0 || n2 > 0) {
      /*if(n1 > 0) c1 = fifichar(s1++);*/ 
        if(n1 > 0) 
           c1 = fifichar((unsigned char *)s1++);
        else c1 = blank;
      /*if(n2 > 0) c2 = fifichar(s2++);*/
        if(n2 > 0) 
           c2 = fifichar((unsigned char *)s2++);
        else c2 = blank;
        if(c1 < c2) 
           return -1;
        else if(c1 > c2) 
           return 1;
        n1--;
        n2--;
    }
    return 0;
}

static short _mgrtxx( const proj_dfn* cnsts, char* mgrs, double *lat, double *lon, long *izone, double *y, double *x )
{	char *blank = " ", *num = "0123456789", *albet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const double 	zero = 0.0, pi = defM_PI;
	const double	oneht = 100000.0, twomil = 2000000.0;
	const double  	spslim = -80.0, spnlim = 84.0;
	const double	r3=3.0,r8=3.0,r9=3.0,
	 				r21=3.0,r33=3.0,r56=3.0,
	 				r64=3.0,r72=3.0,r80=3.0;
	const double  	spcorr = 4.85E-10, ten = 10.0;
	long 			imin = 1, imax = 15, isize = 20;
	double 			feltr,fnltr,sign,slcm,sleast,slwest,spnor,spsou,
					xltr,xnum,ylow,yltr,ynum,yslow;
	long 			ltrloc[3],ltrnum[3],nchar,ileft,iright,nltrs,iarea,
    				ltrlow,ltrhi,ltrhy,nnum;
	long 			i,j,k, mgr_group = 0;
	short			ierr = 0, areaOK = 0;
	proj_dfn		ups;
	void*			UPS = &ups;
/*
     ***** SET ERROR CODE TO ZERO *****
     ***** FIND LEFT MOST NON-BLANK *****
*/	
    nchar = 0;
    i = 1;
S1000:
    if(ftncms((mgrs+(short)i-1),1,blank,1) != 0)
      goto S1100;
    if(i == isize) goto S8802;
    i += 1;
    goto S1000;
S1100:
    ileft = i;
S1200:
/*
     ***** FIND RIGHT MOST NON-BLANK *****
*/
    i += 1;
    if(ftncms((mgrs+(short)i-1),1,blank,1) == 0)
      goto S1400;
    if(i == isize) goto S1300;
    goto S1200;
S1300:
    i = isize+1;
S1400:
    iright = i-1;
/*
     *** NUMBER OF CHARACTERS IN MGRS ***
*/
    nchar = iright-ileft+1;
/*
     *** TEST FOR MININUM NUMBER OF CHARACTERS ***
*/
    if(nchar < imin) goto S8802;
/*
     *** TEST FOR MAXIMUM NUMBER OF CHARACTERS ***
*/
    if(nchar > imax) goto S8802;
/*
     ***** SHIFT TO LEFT IF NECESSARY TO REMOVE ANY BLANKS *****
*/
    if(ileft == 1) goto S1600;
    for(i=1; i<=nchar; i++) {
        j = i+ileft-1;
        ftnsac((mgrs+(short)i-1),1,(mgrs+(short)j-1),(short)j-((short)j-1));
    }
S1600:
/*
     *****************************************************
     *****          MGRS = 2 CHARACTERS = 99         *****
     *****                                           *****
     ***** FOR ABOVE CONDITION NO COMPUTATION IS TO  *****
     ***** BE DONE - USER HAS REQUESTED *MENU*       *****
     ***** MODULE IN MAIN PROGRAM                    *****
     *****         SET IZONE (ZONE) = 99             *****
     *****        RETURN TO MAIN PROGRAM             *****
     *****************************************************
*/
    if(nchar == 2 && ftncms(mgrs,1,&num[9],1) == 0 && 
    	ftncms((mgrs+1),1, &num[9],1) == 0) 
    		goto S8600;
/*
     *****************************************************
     ***** VALIDITY CHECK - AN MGRS CAN ONLY BE MADE *****
     ***** UP OF THE FOLLOWING:                      *****
     *****      1. LETTERS (A-Z) (EXCEPT 'I' & 'O')  *****
     *****      2. NUMBERS (0-9)                     *****
     *****                                           *****
     ***** ALL OTHERS CHARACTERS ARE INVALID         *****
     *****************************************************
*/
    for(i=1; i<=nchar; i++) {
/*
     ** CHECK TO SEE IF CHARACTER IS A LETTER **
*/
        for(j=1; j<=26; j++) {
            if(ftncms((mgrs+(short)i-1),1,&albet[j-1],1)==0) 
            	goto S1750;
        }
/*
     ** CHECK TO SEE IF CHARACTER IS A NUMBER **
*/
        for(j=1; j<=10; j++) {
            if(ftncms((mgrs+(short)i-1),1,&num[j-1],1) == 0)
            	 goto S1750;
        }
/*
     ** CHARACTER IS INVALID - NOT A LETTER OR NUMBER **
     ** TRANSFER TO ERROR DISPLAY AND RETURN TO MAIN PROGRAM **
*/
        goto S8806;
S1750:
        continue;
    }
/*
     *****************************************************
     *****               LETTER PART                 *****
     *****          1-3 LETTERS IN LOCATIONS 1-5     *****
     *****************************************************
*/
    k = 0;
    for(i=1; i<=nchar; i++) {
        for(j=1; j<=26; j++) {
            if(ftncms((mgrs+(short)i-1),1,&albet[j-1],1)!=0) 
            	goto S1800;
            else
            	k += 1;
/*
     ** MAX NUMBER OF LETTERS ALLOWED -3- **
*/
            if(k > 3) goto S8804;
/*
     ** MAX LETTER LOCATION IN MGRS -5- **
*/
            if(i > 5) goto S8804;
/*
     ** LETERS 'I' (9) AND 'O' (15) NOT ALLOWED **
*/
            if(j == 9 || j == 15) goto S8806;
/*
     ** LETTER LOCATION IN MGRS **
*/
            ltrloc[k-1] = i;
/*
     ** LETTER NUMBER IN ALPHABET
*/
            ltrnum[k-1] = j;
S1800:
            continue;
        }
    }
/*
     *** NUMBER OF LETTERS ***
*/
    nltrs = k;
/*
     ***** VALIDITY CHECK *****
     ***   LETTERS MUST BE NEXT TO EACH OTHER (IN SEQUENCE)  ***
*/
    if(nltrs <= 1) goto S2200;
    k = nltrs-1;
    for(i=1; i<=k; i++) {
        if(ltrloc[i-1] != ltrloc[i]-1) goto S8808;
    }
S2200:
/*
     *****************************************************************
     ***** USE NUMBER OF LETTERS TO DETERMINE TYPE OF MGRS INPUT *****
     **                                                             **
     **    NLTRS = 0 = NUMBERS ONLY                                 **
     **                USE OLD GEOGRAPHIC AREA COORDINATES          **
     **                USE OLD ZONE NUMBER AND 1ST LETTER           **
     **                USE OLD 100,000 METER GRID LETTERS           **
     **                                                             **
     **    NLTRS = 1 = ZONE NUMBER AND 1ST LETTER **ONLY**          **
     **                                                             **
     **    NLTRS = 2 = 100,000 METER GRID LETTERS AND NUMBERS **ONLY**
     **                USE OLD GEOGRAPHIC AREA COORDINATES          **
     **                USE OLD ZONE NUMBER AND 1ST LETTER           **
     **                                                             **
     **    NLTRS = 3 = COMPLETELY NEW MGRS ENTRY                    **
     **                                                             **
     *****************************************************************
     ***** NUMBERS ONLY *****
*/
    if(nltrs == 0) goto S6000;
/*
     ***** 100,000 METER LETTERS (2) AND NUMBERS ONLY *****
*/
    if(nltrs == 2) goto S5500;
/*
     ***** NUMBER OF LETTERS IS 1 -OR- 3 *****
     *** SIGN OF HEMISPHERE (NORTH OR SOUTH ) FOR GRID NORTHING  ***
     *** LETTERS A-M ( 1-13) SOUTH - SIGN = -1.0
     *** LETTERS N-Z (14-26) NORTH - SIGN =  1.0
     ** NORTH HEMISPHERE
*/
    sign = 1.0;
/*
     ** SOUTH HEMIPSHERE
*/
    if(ltrnum[0] < 14) sign = -1.0;
/*
     *****************************************************************
     ***      DETERMINE AREA NUMBER (IAREA) FROM 1ST LETTER        ***
     ***                                                           ***
     ***   IAREA = 1 = UTM - LETTERS - C TO W                      ***
     ***                     EXCEPTION OF LETTERS I & O            ***
     ***                                                           ***
     ***           2 = UPS - NORTH ZONE - LETTERS Y & Z            ***
     ***                                                           ***
     ***           3 = UPS - SOUTH ZONE - LETTERS A & B            ***
     *****************************************************************
     ***** UPS -  NORTH ZONE *****
*/
    if(ltrnum[0] == 25 || ltrnum[0] == 26) goto S4000;
/*
     ***** UPS -  SOUTH ZONE *****
*/
    if(ltrnum[0] == 1 || ltrnum[0] == 2) goto S4200;
/*
     *****************************************************************
     ***** UTM ***** UTM ***** UTM ***** UTM ***** UTM ***** UTM *****
*/
    iarea = 1;
/*
     *** VALIDITY CHECKS ***
     ** MUST HAVE A NUMBER FOR ZONE IN LOCATION 1 (NOT A LETTER)**
*/
    if(ltrloc[0] == 1) goto S8810;
/*
     ** ZONE NUMBER CANNOT BE 3 DIGITS OR MORE **
*/
    if(ltrloc[0] > 3) goto S8810;
/*
     ** CHECK IF ZONE NUMBER IS ONE OR TWO DIGITS **
*/
    if(ltrloc[0] == 3) goto S2600;
/*
     ** ZONE NUMBER IS ONE DIGIT  **
     ** SHIFT MGRS TO RIGHT BY 1 SPACE **
*/
    shiftr(mgrs,20,1L,&nchar);
/*
     ** INCREMENT APPLICABLE VARIABLES BY 1 **
*/
    ftnsac(mgrs,1,blank,1);
    for(i=1; i<=nltrs; i++) ltrloc[i-1] += 1;
S2600:
/*
     *** VALIDITY CHECK - MAXIMUM NUMBER OF CHARACTERS ***
*/
    if(nchar > imax) goto S8802;
/*
     ***** UTM ZONE NUMBER *****
*/
    *izone = 0;
    for(i=1; i<=2; i++) {
        ftnsac((AZONE+(short)i-1),1,blank,1);
        if(ftncms((mgrs+(short)i-1),1,blank,1) ==
          0) goto S2900;
        for(j=1; j<=11; j++) {
            if(ftncms((mgrs+(short)i-1),1,&num[j-1],1) == 0) 
            	goto S2800;
            if(j == 11) goto S8806;
        }
S2800:
        ftnsac((AZONE+(short)i-1),1,&num[j-1],1);
        *izone = *izone*10+j-1;
S2900:
        continue;
    }
/*
     ***** VALIDITY CHECKS *****
     ** ZONE NUMBER RANGE 1 -60 **
*/
    if(*izone < 1 || *izone > 60) goto S8810;
/*** BASIC Madtran sets mgrs to "  INVALID MGRS  ", sets ierr = 1, and exits
     when X used with zones 32, 34, 36 as below.  CMAD program has a separate
     function, check_for_x_ltr_error() which does this check before calling
     mgrtgp(). */
/*
     ** ZONE 32 - LETTER 'X' DOES NOT EXIST **
*/
    if(*izone == 32 && ltrnum[0] == 24) goto S8810;
/*
     ** ZONE 34 - LETTER 'X' DOES NOT EXIST **
*/
    if(*izone == 34 && ltrnum[0] == 24) goto S8810;
/*
     ** ZONE 36 - LETTER 'X' DOES NOT EXIST **
*/
    if(*izone == 36 && ltrnum[0] == 24) goto S8810;
/*
     *********************************************************
     ***** DETERMINE GEOGRAPHIC COORDINATES OF RECTANGLE *****
     *****    AS DEFINED BY ZONE NUMBER AND 1ST LETTER   *****
     *********************************************************
     *** LATITUDE AND LONGITUDE LIMITS OF UTM GEOGRAPHIC AREA ***
     *** AN 8 DEGREE N-S BY 6 DEGREE E-W AREA FOR STD ZONES   ***
     *** WHERE:
     ***        LTRNUM(1) = 1ST LETTER NUMBER
     ***        lat      = LATITUDE OF POINT
     ***        IZONE     = ZONE NUMBER
     ***        SPSOU     = SOUTH LATITUDE OF AREA
     ***        SPNOR     = NORTH LATITUDE OF AREA
     ***        SLEAST    = EAST LONGITUDE OF AREA
     ***        SLWEST    = WEST LONGITUDE OF AREA
     ***
*/
    utmlimMGRS(&ltrnum[0],*lat,*izone,&spsou,&spnor,&sleast,&slwest);
/*
     *** COMPUTE GRID COORDINATES ***
         HOLDING ZONE FIXED
*/
    *lat = spsou;
    *lon = slwest;
	geo_to_utm( cnsts,*lat,*lon,izone,x,y );
	if( *lat < 0.0 ) *y *= -1.0;
/*
     *** CENTRAL MERIDIAN ***
*/
    slcm = (double)(*izone*6-183);
/*
     *** DETERMINE LOWEST NORTHING OF GEOGRAPHIC AREA ***
         SOUTH LATITUDE AT CENTRAL MERIDIAN
*/
    geo_to_utm( cnsts, spsou,slcm,izone,&xltr,&yltr );
	if( *lat < 0.0 ) yltr *= -1.0;
/*
     *** SCALE NUMBER DOWN TO NEAREST 100,000 UNIT ***
*/
    ylow = fifdnint((double)fifidint(yltr/oneht)*oneht);
/*
     *** SCALE NUMBER DOWN TO LESS THAN 2 MILLION ***
*/
    yslow = ylow;
S3020:
    if(yslow < twomil) goto S3030;
    yslow -= twomil;
    goto S3020;
S3030:
    yslow = fifdnint(yslow);
/*
     *** USING THE ZONE NUMBER, SPHEROID CODE, ETC ***
     *** DETERMINE
     *** 'LOW' AND 'HIGH' (2ND) LETTER NUMBER
     *** false NORTHING FOR 3RD LETTER
*/
    utmsetMGRS( get_group( *defGROUP, *izone), *izone,&ltrlow,&ltrhi,&fnltr );
/*
     *** DUMMY VALUES FOR UPS VARIABLES FOR SAVE ROUTINE ***
*/
    ltrhy = 0;
    feltr = zero;
/*
     ****** 3 CHARACTERS ONLY ******
*/
    Tomgrda.ncharo = 3;
    if(nchar == 3) goto S8100;
S3050:
/*
     *********************************************************
     ****** -UTM- 100,000 METER LETTERS (2) AND NUMBERS ******
     *********************************************************
     **** ENTRY WHEN 'OLD' ZONE NUMBER & 1ST LETTER ARE UNCHANGED ****
     ***** VALIDITY CHECKS *****
     ** 2ND LETTER **
*/
    if(ltrnum[1] < ltrlow) ierr = areaOK = 17;
    if(ltrnum[1] > ltrhi) ierr = areaOK = 17;
/*
     ** 3RD LETTER ** MAY ONLY BE 'A' TO 'V' **
*/
    if(ltrnum[2] > 22) goto S8822;
/*
     ***** -UTM- 100,000 METER GRID SQUARES *****
*/
    yltr = (double)(ltrnum[2]-1)*oneht+fnltr;
    xltr = (double)(ltrnum[1]-ltrlow+1)*oneht;
/*
     ** CORRECT EASTING FOR LETTER 'O' (15) **
     ** WHEN LTRLOW = 10 = LETTER 'J'
*/
    if(ltrlow == 10 && ltrnum[1] > 15) xltr -= oneht;
/*
     ** CORRECT NORTHING FOR LETTERS 'I' AND 'O' **
*/
    if(ltrnum[2] > 15) yltr -= oneht;
    if(ltrnum[2] > 9) yltr -= oneht;
/*
     *** SCALE NUMBER DOWN UNTIL 2 MILLION OR LESS ***
*/
    if(fifdnint(yltr) >= fifdnint(twomil)) yltr -= twomil;
    yltr = fifdnint(yltr);
/*
     *** DIFFERENCE IN NORTHING BETWEEN TWO SCALED DOWN VALUES
     *** YSLOW - VALUE COMPUTED BASED ON LOWEST LATITUDE AT CM
     *** YLTR  - VALUE COMPUTED FOR LETTER INPUT
*/
    yltr -= yslow;
/*
     ** IF LESS THAN ZERO ADD 2 MILLION **
*/
    if(yltr < zero) yltr += twomil;
/*
     *** NORTHING OF MGRS TO NEAREST 100,000 UNIT ***
*/
    yltr = fifdnint(ylow+yltr);
/*
     ***** NORTHING AND EASTING *****
*/
    *x = xltr;
    *y = yltr*sign;
/*
     *****************************************************
     *****    NUMBER OF CHARACTERS  = 5 - DONE       *****
     ***** TRANSFER TO COORDINATE CONVERSION ROUTINE *****
     ***** CHECK AGAINST AREA COORDINATE LIMITS      *****
*/
    Tomgrda.ncharo = nchar;
    if(nchar == 5) goto S7900;
/*
     ***** TRANSFER TO NUMBER DERIVATION SECTION *****
*/
    goto S7000;
S4000:
/*
     ***** UTM ***** UTM ***** UTM ***** UTM ***** UTM ***** UTM *****
     *****************************************************************
     ***** UPS ***** UPS ***** UPS ***** UPS ***** UPS ***** UPS *****
     ***** UPS - NORTH ZONE *****
*/
    iarea = 2;
/*
     ** LATITUDE LIMIT
*/
    spsou = spnlim;
/*
     ** LONGITUDE FOR 1 LETTER (90 DEG EAST OR WEST) **
*/
    *lon = 90.0;
    if(ltrnum[0] == 25) *lon = -*lon;
/*
     ** ZONE NUMBER 1ST LETTER 'N' **
*/
    ftnsac(AZONE,1,&albet[13],1);
    goto S4300;
S4200:
/*
     ***** UPS - SOUTH ZONE *****
*/
    iarea = 3;
/*
     ** LATITUDE LIMIT **
*/
    spsou = spslim;
/*
     ** LONGITUDE FOR 1 LETTER (90 DEG EAST OR WEST) **
*/
    *lon = 90.0;
    if(ltrnum[0] == 1) *lon = -*lon;
/*
     ** ZONE NUMBER 1ST LETTER 'S' **
*/
    ftnsac(AZONE,1,&albet[18],1);
    goto S4300;
S4300:
/*
     *** VALIDITY CHECK ***  1ST LETTER MUST BE IN LOCATION 1 ***
*/
    if(ltrloc[0] != 1) goto S8830;
/*
     *** SHIFT MGRS RIGHT BY TWO LOCATIONS ***
*/
    shiftr(mgrs,20,2L,&nchar);
/*
     ** SET ZONE NUMBER OF MGRS TO BLANKS **
*/
    ftnsac(mgrs,1,blank,1);
    ftnsac((mgrs+1),1,blank,1);
/*
     ** SET ZONE NUMBER AND 2ND ZONE LETTER 'P' **
*/
    *izone = 0;
    ftnsac((AZONE+1),1,&albet[15],1);
/*
     ** LATITUDE **
*/
    *lat = spsou;
/*
     ** VALIDITY CHECK **
*/
    if(nchar > imax) goto S8802;
/*
     *** CONVERT GEOGRAPHIC TO GRID COORDINATES ***
*/
    init_ups(UPS, defAXIS, defECC2, ( *lat < 0.0 ? -90.0 : 90.0 ) );
    geo_to_ups( UPS, *lat,*lon, x,y);
/*
     *** DEFINE DUMMY VARIABLES FOR UTM ***
     *** FOR SAVE SUBPROGRAM
*/
    spnor = *lat;
    sleast = slwest = *lon;
    ylow = yslow = zero;
/*
     ***** FOR AREA IN USE SET *****
     *** 'LOW' AND 'HIGH' (2ND) LETTER LIMITS
     *** false EASTING FOR 2ND LETTER
     *** false NORTHING FOR 3RD LETTER
     *** HIGH (3RD) LETTER NUMBR
*/
    if(iarea == 2) upnset(ltrnum[0],&ltrlow,&ltrhi,&feltr,&fnltr,&ltrhy);
    if(iarea == 3) upsset(ltrnum[0],&ltrlow,&ltrhi,&feltr,&fnltr,&ltrhy);
    Tomgrda.ncharo = nchar;
    if(nchar == 3) goto S8100;
S5000:
/*
     *********************************************************
     ****** -UPS- 100,000 METER LETTERS (2) AND NUMBERS ******
     *********************************************************
     **** ENTRY WHEN 'OLD' ZONE NUMBER & 1ST LETTER ARE UNCHANGED ****
     *** VALIDITY CHECKS ***
     ** 2ND LETTER **
*/
    if(ltrnum[1] < ltrlow) goto S8820;
    if(ltrnum[1] > ltrhi) goto S8820;
/*
     ** 3RD LETTER **
*/
    if(ltrnum[2] > ltrhy) goto S8822;
/*
     ** VALIDITY CHECK FOR EXCLUDED 2ND LETTERS **
     ** 'D'
*/
    if(ltrnum[1] == 4) goto S8820;
/*
     ** 'E'
*/
    if(ltrnum[1] == 5) goto S8820;
/*
     ** 'M'
*/
    if(ltrnum[1] == 13) goto S8820;
/*
     ** 'N'
*/
    if(ltrnum[1] == 14) goto S8820;
/*
     ** 'V'
*/
    if(ltrnum[1] == 22) goto S8820;
/*
     ** 'W'
*/
    if(ltrnum[1] == 23) goto S8820;
/*
     *** NORTHING ***
*/
    yltr = (double)(ltrnum[2]-1)*oneht+fnltr;
/*
     ** CORRECTION FOR 'I' & 'O'
*/
    if(ltrnum[2] > 9) yltr -= oneht;
    if(ltrnum[2] > 15) yltr -= oneht;
/*
     *** EASTING ***
*/
    xltr = (double)(ltrnum[1]-ltrlow)*oneht+feltr;
/*
     *** CORRECT LONGITUDE FOR EXCLUDED LETTERS BASED ON HEMISPHERE ***
*/
    if(ltrlow == 1) goto S5200;
/*
     ** WESTERN HEMISPHERE **
     ** LETTERS 'M', 'N', 'O', 'V', 'W' **
*/
    if(ltrnum[1] > 12) xltr -= (3.0*oneht);
    if(ltrnum[1] > 21) xltr -= (2.0*oneht);
    goto S5300;
S5200:
/*
     ** EASTERN HEMISPHERE **
     ** LETTERS 'D', 'E', 'I', 'M', 'N', 'O'  **
*/
    if(ltrnum[1] > 3) xltr -= (2.0*oneht);
    if(ltrnum[1] > 9) xltr -= oneht;
    if(ltrnum[1] > 13) xltr -= (3.0*oneht);
    goto S5300;
S5300:
/*
     *** NORTHING AND EASTING ***
*/
    *x = xltr;
    *y = yltr*sign;
/*
     *****************************************************
     *****    NUMBER OF CHARACTERS  = 5 - DONE       *****
     ***** TRANSFER TO COORDINATE CONVERSION ROUTINE *****
     ***** CHECK AGAINST AREA COORDINATE LIMITS      *****
*/
    Tomgrda.ncharo = nchar;
    if(nchar == 5) goto S7900;
/*
     ***** TRANSFER TO NUMBER DERIVATION SECTION *****
*/
    goto S7000;
S5500:
/*
     ***** UPS ***** UPS ***** UPS ***** UPS ***** UPS ***** UPS *****
     *****************************************************************
     ********* 2 LTRS ***** 2 LTRS ***** 2 LTRS ***** 2 LTRS *********
     *****  100,000 METER LETTERS (2) AND NUMBERS  *****
     ***** USE PREVIOUS ZONE NUMBER AND 1ST LETTER *****
     *** VALIDITY CHECK ***
     *** 1ST LETTER MUST BE IN LOCATION 1 ***
*/
    if(ltrloc[0] != 1) goto S8830;
/*
     ** SHIFT MGRS TO RIGHT 3 PLACES **
*/
    shiftr(mgrs,20,3L,&nchar);
/*
     ** RESTORE 'OLD' MGRS **
*/
    for(i=1; i<=3; i++) 
    	ftnsac((mgrs+(short)i-1),1,
    		(Tomgrch.mgrso+(short)i-1),1);
/*
     ** VALIDITY CHECK **
*/
    if(nchar > imax) goto S8802;
/*
     ** SHIFT LETTER LOCATIONS **
*/
    for(i=1; i<=3; i++) ltrloc[i-1] = Tomgrda.ltrlco[i-1];
/*
     ** LTR NUMBER CHANGE **
*/
    ltrnum[2] = ltrnum[1];
    ltrnum[1] = ltrnum[0];
/*
     *** USE PREVIOUS 'OLD' VALUES FOR REMAINDER OF MGRS ***
*/
    ltrnum[0] = Tomgrda.ltrnmo[0];
    spsou = Tomgrda.spsouo;
    spnor = Tomgrda.spnoro;
    sleast = Tomgrda.sleaso;
    slwest = Tomgrda.slweso;
    ltrlow = Tomgrda.ltrlo;
    ltrhi = Tomgrda.ltrho;
    ltrhy = Tomgrda.ltrhyo;
    ylow = Tomgrda.ylowo;
    yslow = Tomgrda.yslowo;
    *izone = Tomgrda.izoneo;
    iarea = Tomgrda.iareao;
    fnltr = Tomgrda.fnltro;
    feltr = Tomgrda.feltro;
    ftnsac(AZONE,1,Tomgrch.azoneo,1);
    ftnsac((AZONE+1),1,(Tomgrch.azoneo+1),1);
/*
     *** NORTH OR SOUTH HEMISPHERE SIGN ***
*/
    sign = 1.0;
    if(ltrnum[0] < 14) sign = -1.0;
/*
     ***** TRANSFER TO APPROPRIATE 2 LETTER SECTION BASED ON AREA *****
     *** UTM ***
*/
    if(iarea == 1) goto S3050;
/*
     *** UPS ***
*/
    goto S5000;
S6000:
/*
     ********* 2 LTRS ***** 2 LTRS ***** 2 LTRS ***** 2 LTRS *********
     *****************************************************************
     ***** NUM ***** NUM ***** NUM ***** NUM ***** NUM ***** NUM *****
     ***           MGRS NUMBERS PART ONLY             ***
     ***  USE PREVIOUS ZONE NUMBER AND 1ST LETTER
     ***  USE PREVIOUS 2ND AND 3RD LETTER FOR 100,000 METER GRID SQUARE
     ** VALIDITY CHECK - NCHAR FROM LAST COMP MUST BE 5 OR MORE **
*/
    if(Tomgrda.ncharo < 5) goto S8834;
/*
     *** SHIFT MGRS TO RIGHT 5 PLACES ***
*/
    shiftr(mgrs,20,5L,&nchar);
/*
     ** RESTORE 'OLD' MGRS VALUES
*/
    for(i=1; i<=5; i++) 
    	ftnsac((mgrs+(short)i-1),1,
    		(Tomgrch.mgrso+(short)i-1),1);
/*
     ** VALIDITY CHECK **
*/
    if(nchar > imax) goto S8802;
/*
     *** USE PREVIOUS 'OLD' VALUES FOR REMAINDER OF MGRS ***
*/
    for(i=1; i<=3; i++) {
        ltrnum[i-1] = Tomgrda.ltrnmo[i-1];
        ltrloc[i-1] = Tomgrda.ltrlco[i-1];
    }
    xltr = Tomgrda.xltro;
    yltr = Tomgrda.yltro;
    spsou = Tomgrda.spsouo;
    spnor = Tomgrda.spnoro;
    sleast = Tomgrda.sleaso;
    slwest = Tomgrda.slweso;
    ylow = Tomgrda.ylowo;
    yslow = Tomgrda.yslowo;
    *izone = Tomgrda.izoneo;
    iarea = Tomgrda.iareao;
    ltrlow = Tomgrda.ltrlo;
    ltrhi = Tomgrda.ltrho;
    ltrhy = Tomgrda.ltrhyo;
    fnltr = Tomgrda.fnltro;
    feltr = Tomgrda.feltro;
    ftnsac(AZONE,1,Tomgrch.azoneo,1);
    ftnsac((AZONE+1),1,(Tomgrch.azoneo+1),1);
/*
     *** NORTH OR SOUTH HEMISPHERE SIGN ***
*/                                   
    sign = 1.0;
    if(ltrnum[0] < 14) sign = -1.0;
/*
     ***** TRANSFER TO NUMBER DERIVATION SECTION ******
                   (NEXT AREA OF CODE)
*/
    goto S7000;
S7000:
/*
     *****************************************************************
     *****************************************************************
     ****** NUMBER PART OF MGRS (LOCATIONS 6 TO NCHAR) ******
     ** NUMBER OF NUMBERS **
*/
    nnum = nchar-5;
/*
     *** VALIDITY CHECK - NUMBER OF NUMBERS MUST BE EVEN ***
*/
    if(nnum/2*2 != nnum) goto S8840;
/*
     *** EASTING PART ***
*/
    xnum = 0.0;
    k = nnum/2+5;
    for(i=6; i<=k; i++) {
        for(j=1; j<=11; j++) {
            if(ftncms((mgrs+(short)i-1),1,&num[j-1],1) == 0) 
            	goto S7200;
            if(j == 11) goto S8806;
        }
S7200:
        xnum = xnum*ten+(double)(j-1);
    }
    xnum *= pow(ten,(double)(5-k+6-1));
/*
     *** NORTHING PART ***
*/
    ynum = 0.0;
    k += 1;
    for(i=k; i<=nchar; i++) {
        for(j=1; j<=11; j++) {
            if(ftncms((mgrs+(short)i-1),1,&num[j-1],1) == 0) 
            	goto S7600;
            if(j == 11) goto S8806;
        }
S7600:
        ynum = ynum*ten+(double)(j-1);
    }
    ynum *= pow(ten,(double)(5-nchar+k-1));
/*
     ***** NORTHING AND EASTING *****
*/
    *y = (yltr+ynum)*sign;
    *x = xltr+xnum;
S7900:
/*
     ***** COMPUTE GEOGRAPHIC COORDINATES *****
     *************** - MGRS COORDINATES ONLY - ***********************
     *** 1 METER IN GEOGRAPHIC COORD OVERLAP ALLOWANCE FOR ROUNDUP ***
     *** 1 METER OF LATITUDE = DEGRAD/(31*3600) = 1.56D-7 (APPROX) ***
     *** 1 METER OF LONGITUDE = 1 METER LATITUDE/(COS(LATITUDE))   ***
     ** LATITUDE ALLOWANCE FOR OVERLAP **
*/
    ynum = 1.56E-7;
    
    if(iarea > 1) goto S8000;
/*
     ** UTM **
*/
    if( lat && lon ) 
    {	utm_to_geo( cnsts, *izone, *x,*y, lat,lon );
/*
     ** VALIDITY CHECK FOR INSIDE AREA GEOGRAPHIC LIMITS **
*/
    	if(*lat+ynum < spsou || *lat-ynum > spnor) ierr = areaOK = 17;
/*
     ** LONGITUDE ALLOWANCE FOR OVERLAP **
*/
    	xnum = ynum/cos(*lat);
    	if(*lon+xnum < slwest || *lon-xnum > sleast) ierr = areaOK = 17;
    }
    goto S8100;
S8000:
/*
     ** UPS **
*/
    if( lat && lon )     
    {	ups_to_geo( UPS, *x,*y, lat,lon );
/*
     ** VALIDITY CHECK FOR INSIDE AREA GEOGRAPHIC LIMIT **
     * NORTH ZONE
*/
    	if(iarea == 2 && *lat+ynum < spsou) goto S8844;
/*
     * SOUTH ZONE
*/
    	if(iarea == 3 && *lat-ynum > spsou) goto S8844; 
    }
    else
    	*izone = 0;
    goto S8100;
S8100:
/*
     ***** BLANK OUT UNUSED PART OF MGRS *****
*/
    j = nchar+1;
    for(i=j; i<=isize; i++) 
    	ftnsac((mgrs+(short)i-1),1,blank,1);
    mgrs[isize] = 0;
    return areaOK;
S8600:
/*
     *******************************************************
     ***** MGRS = 99 - USER WANTS MENU MODULE          *****
     ***** SET ZONE TO 99 AND RETURN TO MAIN PROGRAM   *****
*/
    *izone = 99;
    return ierr;
S8802:
/*
     **********************************************************
     ***** ERROR IN MGRS DATA *****
     *** NUMBER OF CHARARCTERS ***
*/
    goto S8888;
S8804:
/*
     *** MAX NUMBER OF LETTERS OR LOCATION OF THEM ***
*/
    goto S8888;
S8806:
/*
     *** INVALID CHARACTER - I, O, DECIMAL (.), ETC ***
*/
    goto S8888;
S8808:
/*
     *** LETTERS NOT IN SEQUENCE ***
*/
    goto S8888;
S8810:
/*
     *** UTM ZONE NUMBER ***
*/
    goto S8888;
S8820:
/*
     *** 2ND LETTER ***
*/
    goto S8888;
S8822:
/*
     *** 3RD LETTER ***
*/
    goto S8888;
S8830:
/*
     *** 1ST CHARACTER MUST BE A LETTER ***
*/
    goto S8888;
S8834:
/*
     *** NOT ENOUGH CHARACTERS FROM PERVIOUS COMPUTATION ***
*/
    goto S8888;
S8840:
	goto S8888;
/*
     *** NUMBER OF CHARACTERS FOR NUMBERS NOT EVEN ***
*/
	/*
     		*** OUTSIDE GEOGRAPHIC AREA LIMITS [Latitude] ***
		*/
S8844:
		goto S8888;

#if 0
S8845:
		/*
     		*** OUTSIDE GEOGRAPHIC AREA LIMITS [Longitude] ***
		*/
    		/*	06/06/96 - fix to get SW corner of partial cell for F-16
				avionics system.  RLacey DMA/ATIRS.
		*/
		return ierr;
#endif
S8888:

/*
     ***** SET ERROR CODE TO 'ON' = 1 *****
*/
    ierr = 1;
    return ierr;
}

static short mgrs_limit_check( const char* mgrs, double lat, double lon )
	{	if( isdigit(mgrs[0]) )
		{ 	long zone;
			int k = 0;
			char azone[3];
			azone[k++] = mgrs[0];
			if( isdigit(mgrs[1]) ) azone[k++] = mgrs[1];
			azone[k] = 0;
			zone = atol(azone);
			zone = zone*6-186;
			return ( lon >= zone && lon <= zone+6 ? 0 : 1 );
		}
		else
		if( mgrs[0] <= 'B' )
			return ( lat <= -80.0 ? 0 : 1 );
		else
		if( mgrs[0] >= 'Y' )
			return ( lat >= 84.0 ? 0 : 1 );
		return 0;
	}

static void xxtmgr( const proj_dfn* cnsts, double *lat, double *lon, long *izone, double *y, double *x, char *mgrs, int mode )
{	const double  	spslim = -80.0, spnlim = 84.0;
	const double	r3=3.0,r8=8.0,r9=9.0,
	 				r21=21.0,r33=33.0,r56=56.0,
	 				r64=64.0,r72=72.0,r80=80.0;
	const double  	spcorr = 4.85E-10;
	proj_dfn		ups;
	void*			UPS = &ups;
	long    		iarea;

	if( mode == 0 ) 
    {	iarea = ( *izone > 0 ? 1 : ( *y >= 0.0 ? 2 : 3 ));
    	utm_to_geo( cnsts, *izone, *x,*y, lat,lon );
    	goto S570;
    }
    else
		iarea = ( *lat < spslim ? 3 : ( *lat > spnlim ? 2: 1 ));
	
/*
     *** DETERMINE AREA FROM LATITUDE ***
     ** UPS - SOUTH ZONE **
*/
    if(*lat < spslim) goto S555;
/*
     ** UPS - NORTH ZONE **
*/
    if(*lat > spnlim) goto S555;
/*
     *** UTM ***
*/
/*
     *** CONVERT GEOGRAPHIC COORDINATES TO UTM GRID COORDINATES ***
         (LET ZONE BE COMPUTED FROM LONGITUDE - IFIXZ = 0)
*/
    *izone = -100;
    geo_to_utm( cnsts,*lat,*lon,izone,x,y );
	 if( *lat < 0.0 ) *y *= -1.0;
/*
     *** CHECK FOR ODD ZONE LIMITS ***
         ALL NORTH OF 56 DEGREES NORTH LATITUDE
         IN ZONE 31 - 37 AREAS
*/
/*    if(*lat-spcorr <= r56) goto S570;
	Northern limit not included 10/17/97 rjl
*/
    if(*lat+spcorr < r56) goto S570;
    if(*izone < 31 || *izone > 37) goto S570;
/*
     ** ZONES 31 AND 32 - LATITUDES 56 TO 64 **
*/
/*  if(*lat > r64) goto S520;
	64N included 10/17/97 rjl
*/
    if(*lat >= r64) goto S520;
    if(*izone < 31 || *izone > 32) goto S570;
    *izone = 32;
    if(*lon < r3) *izone = 31;
    goto S522;
S520:
/*
     *** ZONES 31 TO 37 ***
     *** NORTH OF 72 DEGREES LATITUDE ***
*/
/*   if(*lat-spcorr <= r72) goto S570; 
	72N not included 10/17/97 rjl
*/
    if(*lat+spcorr < r72) goto S570;
    *izone = 31;
/*	Western limits included 10/17/97 rjl
    if(*lon > r9) *izone += 2;
    if(*lon > r21) *izone += 2;
    if(*lon > r33) *izone += 2;
*/
    if(*lon >= r9) *izone += 2;
    if(*lon >= r21) *izone += 2;
    if(*lon >= r33) *izone += 2;
    goto S522;
S522:
/*
     *** RECOMPUTE GRID COORDINATES FOR ODD SIZED ZONES ***
         (HOLD ZONE NUMBER FIXED - IFIXZ = 1)
*/
    geo_to_utm( cnsts, *lat,*lon,izone,x,y );
	if( *lat < 0.0 ) *y *= -1.0;
	goto S570;

S555:
/*
     *** CONVERT GEOGRAPHIC TO GRID - UPS ***
*/
    init_ups(UPS, defAXIS, defECC2, ( *lat < 0.0 ? -90.0 : 90.0 ) );
    geo_to_ups( UPS,*lat,*lon, x,y );
    goto S570;
S570:
/*
     ****************************************************
     ***** CONVERT TO MILITARY GRID REFERENCE COORDINATES *****
*/
    milref( cnsts, &iarea,lat,izone,fabs(*y),*x,mgrs );
    return;
}

static const char* _check_ellipsoid_name( const char* fn )
	{	return _check_file_name(fn, _ELLIPSOID_FILENAME );  }

short dtcc_delete_ellipsoid_list( _DTCC_LIST** lstP )
    {	lstP = ( lstP ? lstP : &elist );
    	if( *lstP ) 
    	{ 	if( _list_modified(*lstP)>0 || 	_list_added(*lstP)>0 ) 
    			_write_ellipsoid_list(*lstP);
    		if( _list_dataP(*lstP) ) dtcc_free_ptr(_list_dataP(*lstP)); 
    		dtcc_free_ptr(*lstP);
    		*lstP = NULL; 
    	}
		return VMAPOK;
    }

short dtcc_scan_ellipsoid( ELLIPS_PARAMS* ellp, FILE* fp )
{	short status = 0;
	char name[128], abbrv[8],bfr[128];
	double a,rf;
	if( scanTo( fp, ',', 128, name ) == EOF )
		status = -1;
	else
	if( scanTo( fp, ',', 8, abbrv ) == EOF ) 
		status = -2;
	else
	if( scanTo( fp, ',', 64, bfr ) == EOF ) 
		status = -3;
	else
	if( scanTo( fp, ',', 64, bfr+64 ) == EOF ) 
		status = -4;
	if( status == 0 )
	{	sscanf( bfr, "%lf", &a );
		sscanf( bfr+64, "%lf", &rf );
		status = _update_ellipsoid( ellp, name,abbrv, a, rf, _TYPE1_LIST );
	}
	return status;
}

static int _findIt( const char* name  )                 
    {	if( name )
    	{	short n = dtcc_units_count();
       		while( --n >= 0 )
       		    if( strcmp( name, _std_units[n].name ) == 0 ||
       				 strcmp( name, _std_units[n].abbrv ) == 0 ) 
       					return n;
       	}
       	return -1;
    }

short dtcc_is_ellipsoid_axis_OK(	double axis )
{	return 	( axis < MIN_AXIS  ? -1 : ( axis > MAX_AXIS  ?  1 : VMAPOK ));  }
short dtcc_is_ellipsoid_flattening_OK(  double rflat )
{	
	return	( rflat > MIN_FLAT && rflat < MAX_FLAT ? VMAPOK : ( rflat == 0.0 ? ELLIPSOID_IS_SPHERE : PARAMETER_OUT_OF_RANGE ));  
}

static short _check_itellips( ELLIPS_PARAMS* dtccP )
	{	if( !dtcc_is_valid_name(dtccP) )
		{	char nm[256];
			dtcc_make_ellipsoid_name( dtccP,nm );
			dtcc_set_label( dtccP, nm, _DEFAULT_USER_EABBRV, NULL,1,_TYPE2_LIST );
		}
		dtcc_set_valid(dtccP); 
		return VMAPOK;  
	}

double eccentricity2( double rf )	
{	
	double f=( rf > EPSILON ? 1.0/rf : 0.0 ); 
	return f*(2.0-f);  
}	

static void _convertSin2468( double* u )
{   u[0] -= u[2];
    u[1] = 2.0*u[1] - 4.0*u[3];
    u[2] *= 4.0;
    u[3] *= 8.0;
    return;
}

static double _fSin2468( double sphi, const double* u )
{   double cos2x = 1.0 - 2.0*sphi*sphi;
	double sin2x = 2.0* sphi * sqrt( 1.0 - sphi*sphi );
	return sin2x*( u[0] + cos2x*( u[1] + cos2x*( u[2] + cos2x*u[3] )));  
}

double constN( double e )	
{  
	double v = sqrt(1.0-e*e); return ( 1.0-v )/( 1.0+v);  
}

static DCOMPLEX dpxdpx( double d1, double d2)
{
    DCOMPLEX dcomplex;

    dcomplex.cr = d1;
    dcomplex.ci = d2;
    return dcomplex;
}

static DCOMPLEX dpxadd( DCOMPLEX a, DCOMPLEX b)
{
    DCOMPLEX dcomplex;

    dcomplex.cr = a.cr + b.cr;
    dcomplex.ci = a.ci + b.ci;
    return dcomplex;
}


static DCOMPLEX dpxmul( DCOMPLEX a, DCOMPLEX b)
{
    DCOMPLEX dcomplex;

    dcomplex.cr = (a.cr * b.cr) - (a.ci * b.ci);
    dcomplex.ci = (a.ci * b.cr) + (a.cr * b.ci);
    return dcomplex;
}


static DCOMPLEX dpxlongpow( DCOMPLEX a, long b)
{
    /***DO: Switch to formula using polar coordinates if this isn't accurate enough */
    DCOMPLEX dcomplex;
    long     i;

    if( b == 0L )
    {
        dcomplex.cr = 1.0;
        dcomplex.ci = 0.0;
    }
    else
    {
        dcomplex.cr = a.cr;
        dcomplex.ci = a.ci;
    }
    for( i = 2; i <= b; i++ )
    {
        dcomplex = dpxmul( dcomplex, a );
    }
    return dcomplex;    
}

static DCOMPLEX dpxdbl( double dbl)
{
    DCOMPLEX dcomplex;

    dcomplex.cr = dbl;
    dcomplex.ci = 0.0;
    return dcomplex;
}


static DCOMPLEX dpxdiv( DCOMPLEX a, DCOMPLEX b)
{
/* This function assumes that b != 0 + 0i */
    DCOMPLEX dcomplex;

    dcomplex.cr = ((a.cr * b.cr) + (a.ci * b.ci)) / ((b.cr * b.cr) + (b.ci * b.ci));
    dcomplex.ci = ((b.cr * a.ci) - (a.cr * b.ci)) / ((b.cr * b.cr) + (b.ci * b.ci));
    return dcomplex;
}

static double dpxreal( DCOMPLEX a )
{
    double realpart = 0;

    realpart = a.cr;
    return realpart;
}

static double dpxima( DCOMPLEX a )
{
    double imagpart = 0;

    imagpart = a.ci;
    return imagpart;
}
static double fifdnint( double a)
{
    if(a < 0.0) a -= 0.5;
    else a += 0.5;
    return fifdint(a);
}
//static void utmset( long igroup, long izone, long *ltrlow, long *ltrhi, double *fnltr )
// {	long iset;
///*
//     **************************************
//     *** DETERMINE SET FROM ZONE NUMBER ***
//*/
//    iset = 1;
//S3000:
//    if((izone-iset)/6*6+iset == izone) goto S3100;
//    iset += 1;
//    if(iset > 6) goto S3350;
//    goto S3000;
//S3100:
///*
//     *** SET 'LOW' AND 'HIGH' (2ND) LETTER OF ZONE BASED ON ISET ***
//*/
//    switch((short)iset){case 1: goto S3210;case 2: goto S3220;case 3: goto
//      S3230;case 4: goto S3210;case 5: goto S3220;case 6: goto S3230;default:
//      break;}
//S3210:
//    *ltrlow = 1;
//    *ltrhi = 8;
//    goto S3300;
//S3220:
//    *ltrlow = 10;
//    *ltrhi = 18;
//    goto S3300;
//S3230:
//    *ltrlow = 19;
//    *ltrhi = 26;
//    goto S3300;
//S3300:
///*
//     *** SET false NORTHINGS FOR 3RD LETTER ***
//*/
//    switch((short)igroup){case 1: goto S3320;case 2: goto S3330;default: break;}
//S3320:
///*
//     * GROUP 1
//*/
//    *fnltr = 0.0;
//    if(fifmod(iset,2L) == 0) *fnltr = 1500000.0;
//    goto S3350;
//S3330:
///*
//     * GROUP 2
//*/
//    *fnltr = 1000000.0;
//    if(fifmod(iset,2L) == 0) *fnltr = 500000.0;
//    goto S3350;
//S3350:
//    return;
//}

static long get_group( long group, long zone )
	{	if( group > 2 )
			group = ( zone > 32 ? 1 : 2 );
		return group;
	}
		


static void upnset( long n, long *ltrlow, long *ltrhi, double *feltr, double *fnltr, long *ltrhy)
{
/*
     ***** UPS - NORTH ZONE *****
*/
    if(n == 25) goto S30;
/*
     ** EASTERN HEMISPHERE - NORTH ZONE ** 2ND LETTER **
*/
    *ltrlow = 1;
    *ltrhi = 10;
/*
     ** false EASTING **
*/
    *feltr = 2000000.0;
    goto S32;
S30:
/*
     ** WESTERN HEMIPSHERE - NORTH ZONE ** 2ND LETTER **
*/
    *ltrlow = 10;
    *ltrhi = 26;
/*
     ** false EASTING **
*/
    *feltr = 800000.0;
    goto S32;
S32:
/*
     ** false NORTHING FOR 3RD LETTER **
*/
    *fnltr = 1300000.0;
/*
     ** 3RD LETTER MAXIMUM **
*/
    *ltrhy = 16;
    return;
}

static void upsset( long n, long *ltrlow, long *ltrhi, double *feltr, double *fnltr, long *ltrhy)
{
/*
     ***** UPS - SOUTH ZONE *****
*/
    if(n == 1) goto S20;
/*
     ** EASTERN HEMISPHERE - SOUTH ZONE ** 2ND LETTER **
*/
    *ltrlow = 1;
    *ltrhi = 18;
/*
     ** false EASTING **
*/
    *feltr = 2000000.0;
    goto S22;
S20:
/*
     ** WESTERN HEMIPSHERE - SOUTH ZONE ** 2ND LETTER **
*/
    *ltrlow = 10;
    *ltrhi = 26;
/*
     ** false EASTING **
*/
    *feltr = 800000.0;
    goto S22;
S22:
/*
     ** false NORTHING FOR 3RD LETTER **
*/
    *fnltr = 800000.0;
/*
     ** 3RD LETTER MAXIMUM LIMIT **
*/
    *ltrhy = 26;
    return;
}
static void milref( const proj_dfn* cnsts, long *iarea, double *lat,long *izone, double y, double x, char *mgrs  )
{   char *blank = " ", *albet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    double spnlim = 84.0, spslim = -80.0;
    double oneht = 100000.0,twomil=2000000.0,eight=800000.0,one3ht=1300000.0;
	double feltr,fnltr,slcm,sleast,slwest,spnor,spsou,xltr,xnum,ylow,yltr,ynum,yslow;
	long ixnum,iynum,ltrnum[3],ltrlow,ltrhi,ltrhy;
	char   string[20];

/*
     *** BRANCH OFF ACCORDING TO AREA CODE - IAREA ***
     *** IAREA = 1 = UTM
     ***         2 = UPS - NORTH ZONE
     ***         3 = UPS - SOUTH ZONE
*/
    switch((short)*iarea){case 1: goto S620;case 2: goto S640;case 3: goto
      S650;default: break;}
S620:
/*
     *** UTM ***
     *** FROM ZONE NUMBER AND SPHEROID CODES(S) ***
     *** DETERMINE THE FOLLOWING USEING SUBPROGRAM UTMSET
     *** 2ND LETTER RANGE (LTRLOW, LTRHI)
     *** false NORTHING FOR 3RD LETTER (FNLTR)
*/
    utmsetMGRS( get_group( *defGROUP, *izone ), *izone, &ltrlow, &ltrhi, &fnltr );
/*
     *** DETERMINE 1ST LETTER NUMBER AND
     *** LATITUDE & LONGITUDE LIMITS OF UTM GEOGRAPHIC AREA
     *** USING LATITUDE AND ZONE NUMBER
     ***
*/
    ltrnum[0] = 0;
    utmlimMGRS(&ltrnum[0],*lat,*izone,&spsou,&spnor,&sleast,&slwest);
/*
     *** DETERMINE LOWEST NORTHING OF GEOGRAPHIC AREA ***
         USING LATITUDE SPSOU AT CENTRAL MERIDIAN
     ** CENTRAL MERIDIAN **
*/
    slcm = (double)(*izone*6-183);
/*
     ** CONVERT TO GRID COORDINATES **
*/
    *izone = -100L;
    geo_to_utm( cnsts,spsou,slcm,izone,&xltr,&yltr );
	if( spsou < 0.0 ) yltr *= -1.0;	 
/*
     ** SCALE DOWN TO NEAREST 100,000 UNIT **
*/
    ylow = fifdnint((double)fifidint(yltr/oneht)*oneht);
/*
     ** SCALE NUMBER DOWN TO LESS THAN 2 MILLION **
*/
    yslow = ylow;
S621:
    if(yslow < twomil) goto S622;
    yslow -= twomil;
    goto S621;
S622:
    yslow = fifdnint(yslow);
/*
     ***********************************
     *** TRANSFER UTM ZONE NUMBER TO ***
     *** 1ST TWO CHARACTERS OF MGRS  ***
     *** AND TWO CHARACTERS OF AZONE ***
*/
    sprintf(mgrs,"%02ld", *izone);
    sprintf(AZONE,"%02ld", *izone);

/*
     *** UTM *** 3RD LETTER ***
     ** OBTAIN NEAREST WHOLE NUMBER **
*/
    yltr = fifdnint(y);
/*
     ***********************************************
     ***   SPECIAL CASE - SOUTHERN HEMISPHERE    ***
     ***     10 MILLION NORTHING (EQUATOR)       ***
     *** NOT POSSIBLE IN MGRS - SUBTRACT 1 METER ***
     ***********************************************
*/
    if(fifdnint(yltr) == fifdnint(10000000.0)) yltr = fifdnint(yltr-1.0);
S624:
/*
     ** SCALE DOWN NUMBER UNTIL LESS THAN 2 MILLION **
*/
    if(yltr < twomil) goto S626;
    yltr -= twomil;
    goto S624;
S626:
/*
     ** SUBTRACT false NORTHING **
*/
    yltr -= fnltr;
/*
     ** IF LESS THAN ZERO CORRECT BY ADDING 2 MILLION **
*/
    if(yltr < 0.0) yltr += twomil;
/*
     *** DETERMINE 3RD LETTER NUMBER ***
*/
    ltrnum[2] = fifidint((yltr+0.1)/oneht)+1;
/*
     ** CORRECT FOR LETTERS 'I' (9) AND 'O' (15)  **
*/
    if(ltrnum[2] > 8) ltrnum[2] += 1;
    if(ltrnum[2] > 14) ltrnum[2] += 1;
/*
     *** UTM - 2ND LETTER ***
     ** OBTAIN NEAREST WHOLE NUMBER **
*/
    xltr = fifdnint(x);
/*
     ******************************************************
     ***   SPECIAL CASE - ZONE 31                       ***
     *** 56-64 DEGREES NORTH LATITUDE - LETTER 'V' (22) ***
     *** 500,000 EASTING NOT POSSIBLE IN MGRS           ***
     *** SUBTRACT 1 METER                               ***
     ******************************************************
*/
    if(ltrnum[0] == 22 && *izone == 31 && fifdnint(xltr) == fifdnint(500000.0))
      xltr = fifdnint(xltr-1.0);
    ltrnum[1] = ltrlow+fifidint((xltr+0.1)/oneht)-1;
/*
     ** CORRECT FOR LETTER 'O' (15) **
     ** WHEN LTRLOW IS 'J' (10)     **
*/
    if(ltrlow == 10 && ltrnum[1] > 14) ltrnum[1] += 1;
/*
     *** DUMMY VALUES FOR UPS VARIABLES FOR *SAVE* ROUTINE ***
*/
    ltrhy = 0;
    feltr = 0.0;
/*
     ***** TRANSFER TO NUMBER SECTION *****
*/
    goto S670;
S640:
/*
     ************************************************************
     *** UPS -  NORTH ZONE ***
     ** 1ST LETTER ** 'Y' OR 'Z' **
*/
    ltrnum[0] = 25;
    if(fifdnint(x) >= twomil) ltrnum[0] = 26;
/*
     ** 1ST LETTER OF ZONE NUMBER 'N' **
*/
    ftnsac(AZONE,1,&albet[13],1);
/*
     *** FROM 1ST LETTER NUMBER DETERMINE
     *** 2ND LETTER RANGE (LTRLOW, LTRHI)
     *** 2ND LETTER false EASTING (FELTR)
     *** 3RD LETTER false NORTHING (FNLTR)
     *** 3RD LETTER 'HIGH' LETTER NUMBER (LTRHY)
*/
    upnset(ltrnum[0],&ltrlow,&ltrhi,&feltr,&fnltr,&ltrhy);
/*
     ** AREA LIMIT
*/
    spsou = spnlim;
    goto S652;
S650:
/*
     ***************************
     *** UPS -  SOUTH ZONE ***
     ** 1ST LETTER ** 'A' OR 'B' **
*/
    ltrnum[0] = 1;
    if(fifdnint(x) >= twomil) ltrnum[0] = 2;
/*
     ** 1ST LETTER OF ZONE NUMBER 'S' **
*/
    ftnsac(AZONE,1,&albet[18],1);
/*
     *** FROM 1ST LETTER NUMBER DETERMINE
     *** 2ND LETTER RANGE (LTRLOW, LTRHI)
     *** 2ND LETTER false EASTING (FELTR)
     *** 3RD LETTER false NORTHING (FNLTR)
     *** 3RD LETTER 'HIGH' LETTER NUMBER (LTRHY)
*/
    upsset(ltrnum[0],&ltrlow,&ltrhi,&feltr,&fnltr,&ltrhy);
/*
     ** AREA LIMIT
*/
    spsou = spslim;
    goto S652;
S652:
/*
     ************
     ***  UPS  - BOTH ZONES ***
     ** 2ND LETTER OF ZONE 'P' **
*/
    ftnsac((AZONE+1),1,&albet[15],1);
/*
     ** ZONE NUMBER PART OF MGRS 'BLANK' FOR UPS **
*/
    ftnsac(mgrs,1,blank,1);
    ftnsac((mgrs+1),1,blank,1);
/*
     *** UPS - 3RD LETTER ***
     ** OBTAIN NEAREST WHOLE NUMBER **
*/
    yltr = fifdnint(y);
    yltr -= fnltr;
    ltrnum[2] = fifidint((yltr+0.1)/oneht)+1;
/*
     ** CORRECT FOR LETTERS 'I' (9) AND 'O' (15)  **
*/
    if(ltrnum[2] > 8) ltrnum[2] += 1;
    if(ltrnum[2] > 14) ltrnum[2] += 1;
/*
     *** UPS - 2ND LETTER ***
     ** OBTAIN NEAREST WHOLE NUMBER **
*/
    xltr = fifdnint(x);
    xltr -= feltr;
    ltrnum[1] = ltrlow+fifidint((xltr+0.1)/oneht);
/*
     *** CORRECT 2ND LETTER NUMBER FOR MISSING LETTERS ***
*/
    if(x >= twomil) goto S654;
/*
     ** WESTERN HEMISPHERE **
     ** LETTERS 'M', 'N', 'O' **
*/
    if(ltrnum[1] > 12) ltrnum[1] += 3;
/*
     ** LETTERS 'V', 'W' **
*/
    if(ltrnum[1] > 21) ltrnum[1] += 2;
    goto S656;
S654:
/*
     ** EASTERN HEMISPHERE **
     ** LETTERS 'D', 'E'  **
*/
    if(ltrnum[1] > 3) ltrnum[1] += 2;
/*
     ** LETTER  'I' **
*/
    if(ltrnum[1] > 8) ltrnum[1] += 1;
/*
     ** LETTERS 'M', 'N' 'O' **
*/
    if(ltrnum[1] > 12) ltrnum[1] += 3;
    goto S656;
S656:
/*
     *** DUMMY VALUES FOR UTM VARIABLES FOR *SAVE* ROUTINE ***
*/
    spnor = sleast = slwest = ylow = yslow = 0.0;
/*
     ***** TRANSFER TO NUMBER SECTION *****
*/
    goto S670;
S670:
/*
     *************************************************
     ***** NUMBER SECTION *****
     *** EASTING PART ***
     ** OBTAIN NEAREST WHOLE NUMBER **
    xnum = fifdnint(x);
*/
/*
     *************************************************
     ***** NUMBER SECTION *****
     *** EASTING PART ***
     ** TRUNCATED WHOLE NUMBER rjl 04/05/96 **/
    xnum = fifdint(x);
/*
     ******************************************************
     ***   SPECIAL CASE - ZONE 31                       ***
     *** 56-64 DEGREES NORTH LATITUDE - LETTER 'V' (22) ***
     *** 500,000 EASTING NOT POSSIBLE IN MGRS           ***
     *** SUBTRACT 1 METER                               ***
     ******************************************************
*/
    if(ltrnum[0] == 22 && *izone == 31 && fifdnint(xnum) == fifdnint(500000.0))
      xnum = fifdnint(xnum-1.0);
    xnum -= (fifdint((xnum+0.1)/oneht)*oneht);
/*
     ****************************************************************
     *** DATA FERERAL SYSTEM DEPENDENT CODE - IDNINT4 - INTRINSIC ***
     *** NOT NEEDED IN UNIVAC VERSION
*/
    ixnum = fifnint(xnum);
/*
     *** NORTHING PART ***
     ** OBTAIN NEAREST WHOLE NUMBER **
    ynum = fifdnint(y);
*/
/*
     *** NORTHING PART ***
     ** TRUNCATED WHOLE NUMBER rjl 04/05/96 **/
    ynum = fifdint(y);
/*
     ***********************************************
     ***   SPECIAL CASE - SOUTHERN HEMISPHERE    ***
     ***     10 MILLION NORTHING (EQUATOR)       ***
     *** NOT POSSIBLE IN MGRS - SUBTRACT 1 METER ***
     ***********************************************
*/
    if(fifdnint(ynum) == fifdnint(10000000.0)) ynum = fifdnint(ynum-1.0);
    ynum -= (fifdint((ynum+0.1)/oneht)*oneht);
/*
     ****************************************************************
     *** DATA FERERAL SYSTEM DEPENDENT CODE - IDNINT4 - INTRINSIC ***
     *** NOT NEEDED UNIVAC VERSION
*/
    iynum = fifnint(ynum);
/*
     ********************************************************
     ********** TRANSFER DATA TO MGRS **********
     **** USING INTERNAL WRITE
     **** EXCEPT FOR THE 1ST TWO CHARACTERS ****
     **** WHICH ARE ALREADY IN MGRS
*/
    /* Zone already placed in mgrs at TRANSFER UTM ZONE NUMBER above.
       Put letter part of GZD and 100Kmsq letters into mgrs */
    string[0] = albet[(int)ltrnum[0]-1];
    string[1] = albet[(int)ltrnum[1]-1];
    string[2] = albet[(int)ltrnum[2]-1];
    string[3] = '\0';
    sprintf(&mgrs[2],"%s%05ld%05ld ",string,ixnum,iynum);
/*
     ********************************************************
     **** SAVE DATA FOR POSSIBLE USE IN NEXT COMPUTATION ****
     ** NUMBER OF CHARACTERS **
*/
    return;
}

static short _write_ellipsoid_list(_DTCC_LIST* lstP)
	{	FILE* fp = NULL;
		if( !_list_dataP(lstP) )
			return ILLEGAL_ARG;
		else
		if( (fp = _write_list_hdr( lstP,_elist_count(0,lstP))) == NULL ) 
			return FILE_ERROR;
		else
		{	int i, n = _list_total(lstP);
			ELLIPS_PARAMS* elP = (ELLIPS_PARAMS*)_list_dataP(lstP);
			for( i=0; i<n; i++, elP++ )
				if( _accept_it(elP)) 
					dtcc_write_ellipsoid( elP,fp );
			fclose(fp);
			_list_modified(lstP) = 0;
			return VMAPOK;
		}
	}

short dtcc_units_count(void)
{  
	return  _AUNITS_COUNT + _LUNITS_COUNT;  
}

static double fifdint( double a)
{

#ifdef LONGDBL
auto double temp;
    modf(a,&temp);
    a = temp;
#else
    modf(a,&a);
#endif
    return a;
}

static long fifmod( long num, long dem)
{
    if(!dem) return 0L;
    else return num % dem;
}

static long fifnint( double a)
{
    if(a < 0.0) a -= 0.5;
    else a += 0.5;
    return fifidint(a);
}
FILE* _write_list_hdr( const _DTCC_LIST* lstP, short count )
	{	if( lstP )
		{	short flag = 1;
			FILE* fp = fopen( _list_nameP(lstP), "wb" );
			if(fp)
			{	int where = ftell(fp);
				short type2 = count - _list_type1(lstP) - _list_modified(lstP); /* type 1 is invariant */
				fwrite( _list_olabP(lstP), 1,256, fp );
				fwrite( &flag, sizeof(short),1, fp );
				fwrite( &_list_type1(lstP), sizeof(short),1, fp );
				fwrite( &type2, sizeof(short),1, fp );
				fwrite( &_list_rsize(lstP), sizeof(short),1, fp );
			}
			return fp;
		}
		return NULL;
	}

short _elist_count( int k, _DTCC_LIST* listP )
	{	if( listP == NULL ) listP = elist;
		if( !listP )	
			return 0;
		else
		if( k == 1 )
			return	_list_type1( listP );
		else
		if( k == 2 )
			return _list_type2( listP );
		else
			return _list_type1( listP ) + _list_type2( listP );
	}


static short _accept_it( const ELLIPS_PARAMS* elP )
	{	if( dtcc_is_typeN(dtcc_type_flag(elP), _TYPE1_LIST) || 
		  ( dtcc_is_typeN(dtcc_type_flag(elP), _TYPE2_LIST) && dtcc_valid_flag(elP) ) )
		  	return 1;
		else
			return 0;
	}

short dtcc_write_ellipsoid( const ELLIPS_PARAMS* ep,FILE* fp  )
    {   if( ep && fp )
    	{	dtcc_write_label( ep,fp );
        	dtcc_write_double8( fp,ep->axis );
        	dtcc_write_double8( fp,ep->rflat );
        	return VMAPOK;
        }
        else
        	return ILLEGAL_ARG;  
    }

short dtcc_write_label( const void* dtccP, FILE* fp )
{	if( !dtccP )
		return ILLEGAL_ARG;
	else
	if( !fp )
		return FILE_NOT_OPEN;
	else
	{	short k=0, flag = 1;
		const DTCC_LABELS* lblP = (DTCC_LABELS*)dtccP;
		int where = ftell(fp);
		k += fwrite( &flag, sizeof(short),1, fp );
		k += fwrite( &lblP->l1, sizeof(short),1, fp );
		k += fwrite( dtcc_nameP(lblP), 1,lblP->l1, fp );
		k += fwrite( &lblP->l2, sizeof(short),1, fp );
		k += fwrite( dtcc_abbrvP(lblP), 1,lblP->l2, fp );
		k += fwrite( &lblP->l3, sizeof(short),1, fp );
		k += fwrite( dtcc_otherP(lblP), 1,lblP->l3, fp );
		k += fwrite( &lblP->type, sizeof(short),1, fp );
		k += fwrite( &lblP->valid, sizeof(short),1, fp );
		return k;
	}
}
/* write native double as an 8 byte IEEE double */
short dtcc_write_double8( FILE* fp, double dble )
	{	/*	char x[8];
			native_double( &dble, x, 2 );
		*/
		IEEE_DOUBLE8 x = dble;
		return ( fwrite( &x, 8,1,fp ) == 1 ? VMAPOK : WRITE_INCOMPLETE );
	}


short dtcc_read_system(  COORD_SYS* cpP, FILE* fp )
	{	if( !cpP || !fp )
			return ILLEGAL_ARG;
		else
        {	int count=0; 
			/* int xptr = fprintf( stdout, "read_label: %ld\n", fp); */
			short flag = dtcc_read_label( _SYSTEM_LABELP(cpP), fp );
			if( flag < 0 ) 
				return flag;
        	/* xptr = fprintf( stdout, "read_dim: %ld\n", fp); */
			_SYSTEM_DIMENSION(cpP) = dtcc_read_short( fp,flag );
			count += sizeof(short);
        	/* xptr = fprintf( stdout, "dtcc_read_coord_group: %ld\n", fp); */
			count += dtcc_read_coord_group(_SYSTEM_GROUPP(cpP),fp );
        	/* xptr = fprintf( stdout, "dtcc_read_datum: %ld\n", fp); */
			count += dtcc_read_datum(_SYSTEM_DATUMP(cpP),fp );
        	/* xptr = fprintf( stdout, "dtcc_read_projection: %ld\n", fp); */
			count += dtcc_read_projection(_SYSTEM_PROJP(cpP),fp );
        	/* xptr = fprintf( stdout, "dtcc_read_extents: %ld\n", fp); */
			count += dtcc_read_extents(_SYSTEM_EXTENTSP(cpP),fp );
        	/* xptr = fprintf( stdout, "dtcc_read_coord3_units: %ld\n", fp); */
			count += dtcc_read_coord3_units(_SYSTEM_UNITSP(cpP),fp );
			/* xptr = fprintf( stdout, "_check_ellipsoids: %ld\n", fp); */
			_check_ellipsoids(cpP);
			/* xptr = fprintf( stdout, "user_data: %ld\n", fp); */
			cpP->user_data[0] = 0;
        	return VMAPOK;
         }
     }


ERRSTATUS ushort_to_char(unsigned char *record, unsigned short *s, short big_endian, long *c)
{
    long            i;
    union
    {				/* the short & character union          */
	unsigned short  s;
	unsigned char   c[2];
    }               shtchr;

    shtchr.s = *s;
    for (i = 0; i < 2; i++)
	if (big_endian)
	    record[(*c)++] = shtchr.c[1 - i];
	else
	    record[(*c)++] = shtchr.c[i];

    return STAT_SUCCESS;
}


ERRSTATUS char_to_ushort(unsigned char *record, unsigned short *s, short big_endian, long *c)
{
    long            i;
    union
    {				/* the short & character union          */
	unsigned short  s;
	unsigned char   c[2];
    }               shtchr;

    for (i = 0; i < 2; i++)
	if (big_endian)
	    shtchr.c[1 - i] = record[(*c)++];
	else
	    shtchr.c[i] = record[(*c)++];

    *s = shtchr.s;
    return STAT_SUCCESS;
}

short dtcc_get_coord3(	const COORD3* c3P,double *uP, double *vP, double *wP,short* zoneP )
	{	if( !c3P )
			return ILLEGAL_ARG;
		else
		if( dtcc_coord3_is_encoded(dtcc_coord3Z(c3P)) )
			return ILLEGAL_ARG;
		else
		{	if( uP ) *uP = dtcc_coord31(c3P);
			if( vP ) *vP = dtcc_coord32(c3P);
			if( wP ) *wP = dtcc_coord33(c3P);
			if( zoneP ) *zoneP = dtcc_coord3_zone(c3P);
			return VMAPOK;
		}
	}        


short dtcc_read_label ( void* dtccP, FILE* fp )
{	if( !dtccP )
		return ILLEGAL_ARG;
	else
	if( !fp )
		return FILE_NOT_OPEN;
	else
	{	short flag = 0;
		DTCC_LABELS* lblP = (DTCC_LABELS*)dtccP;
		int k=0,where = ftell(fp);
		k += fread( &flag, sizeof(short),1, fp );
		lblP->l1 = dtcc_read_short( fp,flag );
		k += fread( _label_nameP(lblP), 1, lblP->l1, fp );
		lblP->l2 = dtcc_read_short( fp,flag );
		k += fread( _label_abbrvP(lblP), 1, lblP->l2, fp );
		lblP->l3 = dtcc_read_short( fp,flag );
		k += fread( _label_otherP(lblP), 1, lblP->l3, fp );
		lblP->type = dtcc_read_short( fp,flag );
		lblP->valid = dtcc_read_short( fp,flag );
		return flag;
	}
}

short 	dtcc_read_short( FILE* fp, short endian )
	{	short x;
		if( !fp )
			return SHORT_NULL;
		else
		if( fread( &x, sizeof(short),1, fp ) != 1 )
			return SHORT_NULL;
		else
		if( endian != 1 )
			dtcc_swab_short( &x );
		return x;
}

short dtcc_read_coord_group( COORD_GROUP* cg1P,FILE* fp)
	{	if( fp && cg1P )
		{	char label[64];
			char* look = label;
			fread( label, 1,64, fp );
			dtcc_decode_coord_group(cg1P,label);
			return VMAPOK;
		}
		else
			return ILLEGAL_ARG;
	}	

short dtcc_read_datum( DATUM_PARAMS* dpP,FILE* fp)
    {   if( dpP && fp )
    	{	DTCC_LABELS tmp;
    		short flag = dtcc_read_label( &tmp,fp );
        	if( flag >= 0 )
        	{	short n, shfts[3], sigmas[3];
				char vname[12];
        		ELLIPS_PARAMS ellp;
				n = dtcc_read_short(fp,flag);
				if( n > 11 ) return READ_INCOMPLETE;
				fread( vname, 1,n, fp );
				vname[n] = 0;
				dtcc_read_shortN( fp, shfts, 3, flag );
				dtcc_read_shortN( fp, sigmas, 3, flag );
				dtcc_read_ellipsoid( &ellp, fp );
        		_update_datum( dpP, dtcc_nameP(&tmp), dtcc_otherP(&tmp), 
        						vname, dtcc_abbrvP(&tmp), shfts,sigmas, &ellp, 
        						dtcc_type_flag(&tmp) );
        		return VMAPOK;
			}
			else
				return READ_INCOMPLETE;
        }
        else
        	return ILLEGAL_ARG;  
    }

short dtcc_read_projection( PROJ_PARAMS* ppP,FILE* fp)
    {   if( ppP && fp )
        {   double stdP[2];
        	ELLIPS_PARAMS ellipsoid;
        	COORD_ORIGIN origin;
			DTCC_LABELS label;
        	short endianFlag = dtcc_read_label( &label,fp );
            dtcc_read_ellipsoid( &ellipsoid, fp );
              
        	dtcc_read_origin( &origin, fp );
       
        	stdP[0] = dtcc_read_double8( fp, endianFlag );
            stdP[1] = dtcc_read_double8( fp, endianFlag );
			ppP->ncnsts = dtcc_read_short( fp, endianFlag );
			ppP->fixed = dtcc_read_short( fp, endianFlag );
			_PROJECTION_ENGINE(ppP) = NULL;
             return _updateProjection( ppP, dtcc_nameP(&label), &ellipsoid, &origin, stdP );
	  	}
        else
            return ILLEGAL_ARG;
    }
short dtcc_read_extents( COORD_EXTENTS* ceP,FILE* fp ) 
    {   if( ceP && fp )
        {   short endianFlag = 1;
            fread( &endianFlag, 1,sizeof(short), fp );
            dtcc_read_coord3( _EXTENTS_MIN_GEOP(ceP), fp, endianFlag );
            dtcc_read_coord3( _EXTENTS_MAX_GEOP(ceP), fp, endianFlag );
            dtcc_read_coord3( _EXTENTS_MIN_GRIDP(ceP), fp, endianFlag );
            dtcc_read_coord3( _EXTENTS_MAX_GRIDP(ceP), fp, endianFlag );
            return _check_itcoordextent(ceP);
        }
        else
            return ILLEGAL_ARG;
    } 

short dtcc_read_coord3_units( COORD3_UNITS* cuP, FILE* fp )
	{	if( cuP && fp )
		{	short flag;
			fread( &flag, 1,sizeof(short), fp );
			dtcc_coord3_map_scale(cuP) = dtcc_read_double8( fp,flag );
			dtcc_read_units( _COORD3_UNITS1P(cuP), fp );
    		dtcc_read_units( _COORD3_UNITS2P(cuP), fp );
    		dtcc_read_units( _COORD3_UNITS3P(cuP), fp );
    		dtcc_coord3_mode( cuP )	= (COORD3_TYPES) dtcc_read_short( fp,flag );
    		return VMAPOK;
    	}
    	else
    		return ILLEGAL_ARG;
    }

short dtcc_swab2N( short* A, short n )
    {   short a,k;
    	for( k=0; k<n; k++, A++ )
    	{	a = *A; 
        	*A = ( (a & 0x0000FF) << 8 ) | ( (a & 0x00FF00) >> 8 );
        }
        return 0;
    }    

short dtcc_decode_coord_group( 	COORD_GROUP* cgP,char* labelP) 
	{	COORD3_TYPES id;
		short k = _group_label( 1, &id, labelP );
		if( k == VMAPOK ) dtcc_set_coord_group(cgP,id);
		return VMAPOK;
	} 

short 	dtcc_read_shortN( 	FILE* fp, short* bfrP, short N, short endian )
	{	if( !fp || !bfrP )
			return ILLEGAL_ARG;
		else
		if( N <= 0 )
			return INDEX_OUT_OF_RANGE;
		else
		{	int i;
			for( i=0; i<N; i++ )
				bfrP[i] = dtcc_read_short( fp, endian );
			return VMAPOK;
		}
	}

short dtcc_read_ellipsoid( ELLIPS_PARAMS* ep,FILE* fp )
    {   if( ep && fp )
    	{	DTCC_LABELS tmp;
			short flag = dtcc_read_label( &tmp,fp );
        	if( flag >= 0 )
        	{	double axis = dtcc_read_double8( fp,flag );
        		double rflat = dtcc_read_double8( fp,flag );
				_update_ellipsoid( ep, dtcc_nameP(&tmp), dtcc_abbrvP(&tmp), axis, rflat, tmp.type );
        		return VMAPOK;
			}
			else
				return READ_INCOMPLETE;
        }
        else
        	return ILLEGAL_ARG;  
    }

short dtcc_read_origin( COORD_ORIGIN* coP, FILE* fp   )
    {   if( coP )
        {   double x;
            short endian = 1;
            fread( &endian, 1,sizeof(short), fp );
            dtcc_read_coord3( _ORIGIN_GRIDNEP(coP), fp, endian );
            dtcc_read_coord3( _ORIGIN_GRIDSWP(coP), fp, endian );
            dtcc_read_coord3( _ORIGIN_GEOP(coP), fp, endian );   
            dtcc_read_coord3( _ORIGIN_SCALEP(coP), fp, endian );
            x = dtcc_read_double8( fp, endian ); 
            dtcc_set_origin_convergence( coP, x );    
            x = dtcc_read_double8( fp, endian ); 
            dtcc_set_origin_azimuth( coP, x );
            coP->valid = dtcc_read_short( fp, endian ); 
			coP->op_status = dtcc_read_short( fp, endian ); 
			return VMAPOK;
        }
        return ILLEGAL_ARG;
    }   

double 	dtcc_read_double8( FILE* fp, short endian )
	{	long k = sizeof(IEEE_DOUBLE8);
		double z = DOUBLE_NULL;
		int status = 0;
		IEEE_DOUBLE8 x;
		if( !fp )
			return DOUBLE_NULL;
		else
		if( (status=fread( &x, 8,1, fp )) != 1 )
		{	return DOUBLE_NULL;
		}
		else 
		if( endian != 1 )
			dtcc_swab_double8( &x );
		z = x;
		/* native_double( &z, x, 1L ); */
		if( z < DOUBLE_NULL/10.0 ) z = DOUBLE_NULL;
		return z;
	}

//static short _update( 	UNITS_INFO* uiP,const DTCC_LABELS*	label,double 				cf, short scaled )
//	{	if( uiP && label )
//		{	dtcc_copy_label( &uiP->label, label );
//			if( cf > DOUBLE_NULL ) uiP->cf = cf;
//			if( scaled > SHORT_NULL ) uiP->scaled = scaled;
//			return dtcc_validate_units(uiP);
//		}	
//		else
//			return ILLEGAL_ARG;
//	}

static short _updateProjection(	PROJ_PARAMS* ppP,const char* name, const ELLIPS_PARAMS* elP, const COORD_ORIGIN* orgP,const double* stdP )
	{   short k = VMAPOK;
		if( !ppP )  
			return ILLEGAL_ARG;
	    if( !name && !elP && !orgP && !stdP ) 
			return VMAPOK;
			
		if( elP )	
			dtcc_copy_ellipsoid
				( _PROJECTION_ELLIPSOIDP(ppP), elP );
		
		if( orgP )
			dtcc_copy_origin( _PROJECTION_ORIGINP(ppP), orgP );
		
		if( stdP )
			memcpy
			( _PROJECTION_STDPARSP(ppP),stdP,2*sizeof(double) ); 
		
		if( name )
		{	_get_projection_label( name, _PROJECTION_LABELP(ppP) );
			_setProjectionEngine( ppP );  
		}
		_projection( ppP, DTCC_PROJECTION_SETUP, NULL,1L, NULL );
		dtcc_set_invalid(ppP);
		k = _check_itproj(ppP); 
		if( k == VMAPOK ) { dtcc_set_valid(ppP); return VMAPOK; }
		return k;	
		
	}

short dtcc_read_coord3( COORD3* c3P,FILE* fp,short endian  )        
    {   if( c3P && fp ) 
        {   short zone = dtcc_read_short( fp, endian );
			if( dtcc_coord3_is_encoded(zone) )
			{	char tmp[24];
				if( fread( tmp, 1,24, fp ) != 24 )
					return READ_INCOMPLETE;
				else
					dtcc_set_coord3A(c3P,tmp,zone);
			}
			else
            {	double x = dtcc_read_double8( fp, endian );
            	double y = dtcc_read_double8( fp, endian );
        		double z = dtcc_read_double8( fp, endian );
            	dtcc_set_coord3( c3P, x,y,z, zone );
			}
			return VMAPOK;
        } 
        else
            return ILLEGAL_ARG;
    }


short dtcc_read_units(	UNITS_INFO* uiP,FILE* fp )
	{	if( uiP && fp )
		{	short flag = dtcc_read_label((DTCC_LABELS*)dtcc_nameP(uiP), fp );
			uiP->cf = dtcc_read_double8( fp, flag );
			dtcc_set_units_scaled( uiP,dtcc_read_short( fp,flag ) );
			dtcc_validate_units(uiP);
			return VMAPOK;
		}
		else
			return ILLEGAL_ARG;
	}


short dtcc_swab8N( long* A, short n )
   {    long t, *B; short k;
		for( k=0; k<n; k++, A+=2 )
        {	B = A+1;
            dtcc_swab4N( (short*)A,1 ); dtcc_swab4N((short*) B,1 );
        	t = *A; *A = *B; *B = t;
        }
        return 0;
    }

short dtcc_copy_origin( COORD_ORIGIN* coP1,const COORD_ORIGIN* coP2  )
	{	return blockcopy( coP1, coP2, sizeof(COORD_ORIGIN) );  }

short dtcc_swab4N( short* A, short n )
    {   short t,k; 
        short*B;
        for( k=0; k<n; k++, A+=2 )
        {	B = A+1;
        	dtcc_swab2N( A,1 ); dtcc_swab2N( B,1 );
        	t = *A; *A = *B; *B = t;
        }
        return 0;
    }    


ERRSTATUS geometry_match_basemap(BASEMAP * basemap, MAPGEO * mapgeo)
 {
     ERRSTATUS errcode = STAT_SUCCESS;
     double height = DOUBLE_NULL;

	 /* match raster header projection to map display system */
 	 if( strcmp(dtcc_system_projection_nameP(&mapgeo->map_display_coord_sys), "") == 0 )
	     /*****TEMP: use check for GP system instead? */
	     basemap->area.data_hdr.projection = PROJ_GRAPH; 
 	 else if( strcmp(dtcc_system_projection_nameP(&mapgeo->map_display_coord_sys), "Lambert Conformal Conic") == 0 )
	 {
	     basemap->area.data_hdr.projection = PROJ_LAMBERT2;
		 if( fabs(dtcc_projection_upper_std_parallel(dtcc_system_projectionP(&mapgeo->map_display_coord_sys)) -
		          dtcc_projection_lower_std_parallel(dtcc_system_projectionP(&mapgeo->map_display_coord_sys))) < ANIOTA )
		     basemap->area.data_hdr.projection = PROJ_LAMBERT1;
     }	      
	 else if( strcmp(dtcc_system_projection_nameP(&mapgeo->map_display_coord_sys), "Mercator") == 0 )
	     basemap->area.data_hdr.projection = PROJ_MERCATOR;
	 else if( strcmp(dtcc_system_projection_nameP(&mapgeo->map_display_coord_sys), "Stereographic") == 0 )
	     basemap->area.data_hdr.projection = PROJ_POLAR;
	 else if( strcmp(dtcc_system_projection_nameP(&mapgeo->map_display_coord_sys), "Universal Polar Stereographic") == 0 )
	     basemap->area.data_hdr.projection = PROJ_UPS;
	 /* as of 7/96, mdtcc does not consider UTM to be a displayable system--this case will not occur  */
	 else if( strcmp(dtcc_system_projection_nameP(&mapgeo->map_display_coord_sys), "Universal Transverse Mercator") == 0 )
	 {
	     basemap->area.data_hdr.projection = PROJ_UTM;
	     basemap->area.data_hdr.h_units = defUTM;
     }
	 else
	 {
	     basemap->area.data_hdr.projection = PROJ_GRAPH;
         if( strcmp(dtcc_system_projection_nameP(&mapgeo->map_display_coord_sys), 
                   "Military Grid Reference System") == 0 )
	         basemap->area.data_hdr.h_units = defMGRS;  
		 errcode = STAT_NO_DATA;
	 }
	      
#if 0  /* raster header PROJECTION enums unmatched by mdtcc projection strings: */
	     case PROJ_UNDEFINED:
	     case PROJ_ARC:
	     case PROJ_TS:
#endif

	 /* set raster data header's horizontal datum */
	 if( strcmp(dtcc_system_hdatum_nameP(&mapgeo->map_display_coord_sys),"WGS 1984") == 0 )
	     basemap->area.data_hdr.datum.hor_datum = HD_WGS84;  
	 else if( strcmp(dtcc_system_hdatum_nameP(&mapgeo->map_display_coord_sys),"WGS 1972") == 0 )
	     basemap->area.data_hdr.datum.hor_datum = HD_WGS72;  
	 else if( strcmp(dtcc_system_hdatum_nameP(&mapgeo->map_display_coord_sys),"North American 1927") == 0
	          /* && ..... "MEAN FOR CONUS" */ )
	     basemap->area.data_hdr.datum.hor_datum = HD_NAD27;  
	 else if( strcmp(dtcc_system_hdatum_nameP(&mapgeo->map_display_coord_sys),"European 1950") == 0 
	          /* && ..... "MEAN FOR NW Europe" */ )
	     basemap->area.data_hdr.datum.hor_datum = HD_EUD50;
	 else
	 {   /*****TEMP: or set hdatum with no matching enum to WGS84? */
	     basemap->area.data_hdr.datum.hor_datum = NDEFINED;
         errcode = STAT_NO_DATA;
	 }

	 /* set raster data header's vertical datum; as of 7/96, mdtcc is 2D only */
 	 if( strcmp(dtcc_system_vdatum_nameP(&mapgeo->map_display_coord_sys), "AMSL") == 0 )
	     basemap->area.data_hdr.datum.ver_datum = VD_MSL;
 	 else if( strcmp(dtcc_system_vdatum_nameP(&mapgeo->map_display_coord_sys), "ELLP") == 0 )
	     basemap->area.data_hdr.datum.ver_datum = VD_ELIPSOID;
	 else
	 {
	     basemap->area.data_hdr.datum.ver_datum = UDEFINED;
		 errcode = STAT_NO_DATA;
	 }

	 /* set raster data header's units; the GEOGRAPHIC values MGRS and UTM are set under projection above   */
	 if( strcmp( dtcc_coord3_units_labelNP(dtcc_system_unitsP(&mapgeo->map_display_coord_sys), 0),"degrees" ) == 0 || 
	     strcmp(dtcc_coord3_units_labelNP(dtcc_system_unitsP(&mapgeo->map_display_coord_sys),0),"dd") == 0 
	     /*****TEMP: 7/96 supported GP systems currently have no units strings! */
	     || strcmp(dtcc_coord3_units_labelNP(dtcc_system_unitsP(&mapgeo->map_display_coord_sys),0),"") == 0)
	     basemap->area.data_hdr.h_units = defDDTEMP;  
	 else if( strcmp(dtcc_coord3_units_labelNP(dtcc_system_unitsP(&mapgeo->map_display_coord_sys),0),"degrees, minutes & seconds") == 0 || 
	     strcmp(dtcc_coord3_units_labelNP(dtcc_system_unitsP(&mapgeo->map_display_coord_sys),0),"dms") == 0 )
	     basemap->area.data_hdr.h_units = defDMS;  
	 else if( strcmp(dtcc_coord3_units_labelNP(dtcc_system_unitsP(&mapgeo->map_display_coord_sys),0),"minutes") == 0 || 
	     strcmp(dtcc_coord3_units_labelNP(dtcc_system_unitsP(&mapgeo->map_display_coord_sys),0),"mn") == 0 )
	     basemap->area.data_hdr.h_units = defMIN;  
	 else if( strcmp(dtcc_coord3_units_labelNP(dtcc_system_unitsP(&mapgeo->map_display_coord_sys),0),"seconds") == 0 || 
	     strcmp(dtcc_coord3_units_labelNP(dtcc_system_unitsP(&mapgeo->map_display_coord_sys),0),"s") == 0 )
	     basemap->area.data_hdr.h_units = defSECS;
	 else 
	     errcode = STAT_NO_DATA;

	 if( dtcc_system_dimension(&mapgeo->map_display_coord_sys) == 3 )
	 {
 	   if( strcmp(dtcc_coord3_units_labelNP(dtcc_system_unitsP(&mapgeo->map_display_coord_sys),2),"meters") == 0 || 
	     strcmp(dtcc_coord3_units_labelNP(dtcc_system_unitsP(&mapgeo->map_display_coord_sys),2),"m") == 0 )
	     basemap->area.data_hdr.v_units = VER_METERS;  
 	   else if( strcmp(dtcc_coord3_units_labelNP(dtcc_system_unitsP(&mapgeo->map_display_coord_sys),2),"fathoms") == 0 || 
	     strcmp(dtcc_coord3_units_labelNP(dtcc_system_unitsP(&mapgeo->map_display_coord_sys),2),"fm") == 0 )
	     basemap->area.data_hdr.v_units = VER_FATHOMS;  
 	   else if( strcmp(dtcc_coord3_units_labelNP(dtcc_system_unitsP(&mapgeo->map_display_coord_sys),2),"feet") == 0 || 
	     strcmp(dtcc_coord3_units_labelNP(dtcc_system_unitsP(&mapgeo->map_display_coord_sys),2),"ft") == 0 )
	     basemap->area.data_hdr.v_units = VER_FEET;
	   else      
	     errcode = STAT_NO_DATA;
	 }
	 else
		 basemap->area.data_hdr.v_units = VER_METERS;  

     basemap->area.data_hdr.extent.bottom = mapgeo->geo_extent.bottom; 
     basemap->area.data_hdr.extent.left = mapgeo->geo_extent.left; 
     basemap->area.data_hdr.extent.top = mapgeo->geo_extent.top;
     basemap->area.data_hdr.extent.right = mapgeo->geo_extent.right;

	 basemap->area.data_hdr.center.lat =
	      	0.5*(basemap->area.data_hdr.extent.bottom + 
	      	     basemap->area.data_hdr.extent.top);
	 basemap->area.data_hdr.center.lon =
	      	0.5*(basemap->area.data_hdr.extent.right + 
	      	     basemap->area.data_hdr.extent.left);

	 /*****TEMP: rounding, mod-ing etc. needed ? */
	 basemap->area.data_matrix.height =  (long)mapgeo->plot_params.image_height_pixels_computed;
	 basemap->area.data_matrix.width = (long)mapgeo->plot_params.image_width_pixels_computed;

	 /***9/97 */
     mapgeo->use_arc = false;

    return errcode;
}

ERRSTATUS basemap_match_geometry(BASEMAP * basemap, MAPGEO * mapgeo)
 {
     double height = DOUBLE_NULL;

     /*for now, assume default WGS84 Decimal degrees*/
     dtcc_reset_default_geodetic_system( &mapgeo->map_display_coord_sys);
     setup_transform_struct( (const COORD_SYS *)&mapgeo->internal_coord_sys,(const COORD_SYS *)&mapgeo->map_display_coord_sys ,
                            mapgeo->internal_to_display  );
     setup_transform_struct( (const COORD_SYS *)&mapgeo->map_display_coord_sys,(const COORD_SYS *)&mapgeo->internal_coord_sys,
                            mapgeo->display_to_internal );
     setup_transform_struct( (const COORD_SYS *)&mapgeo->data_coord_sys, (const COORD_SYS *)&mapgeo->map_display_coord_sys ,
                            mapgeo->data_to_display  );
     setup_transform_struct( (const COORD_SYS *)&mapgeo->map_display_coord_sys, (const COORD_SYS *)&mapgeo->data_coord_sys,
                            mapgeo->display_to_data );

#if 0 /* this code can not yet be used because enums in data_hdr are not reliable */       
	 /* reset map display system to match raster header projection */
 	 switch( basemap->area.data_hdr.projection )
	 {
	  	 COORD_ORIGIN org;

	     case PROJ_UNDEFINED:
		 break;
	     case PROJ_LAMBERT1:
	     break;
	     case PROJ_LAMBERT2:
	     break;
	     case PROJ_LINEAR:
	     break;
	     case PROJ_MERCATOR:
             dtcc_reset_system_projection( &mapgeo->map_display_coord_sys, "Mercator" );
	     break;
	     case PROJ_UPS:
             dtcc_reset_default_UPS_system( &mapgeo->map_display_coord_sys );
	     break;
	     case PROJ_UTM:
             dtcc_reset_default_UTM_system( &mapgeo->user_coord_sys );
             dtcc_reset_system_projection( &mapgeo->map_display_coord_sys, "Transverse Mercator" );
	     break;
	     case PROJ_GRAPH:
	     break;
	     case PROJ_POLAR:
	     break;
	     case PROJ_ARC:
	     break;
	     case PROJ_TS:
	     break;
	     default:
		 break;
	 } 

	 /* set map display coordinate system's horizontal datum from raster data header */
	 switch( basemap->area.data_hdr.datum.hor_datum )
	 {
	     case NDEFINED:
	     case HD_WGS84:
		     dtcc_reset_system_hdatum( &mapgeo->map_display_coord_sys, "WGS 1984", 
		                               "Global Definition" );
	     break;
	     case HD_WGS72:
		     dtcc_reset_system_hdatum( &mapgeo->map_display_coord_sys, "WGS 1972", 
		                               "Global Definition" );
	     break;
	     case HD_NAD27:
		     /*****TEMP: Unsure if area below is the correct one */
		     dtcc_reset_system_hdatum( &mapgeo->map_display_coord_sys, "North American 1927", 
		                              "MEAN FOR CONUS" );
	     break;
	     case HD_EUD50:
		     /*****TEMP: Unsure if area below is the correct one */
		     dtcc_reset_system_hdatum( &mapgeo->map_display_coord_sys, "European 1950", 
		                               "MEAN FOR Austria, Belgium, Denmark, Finland" );
	     break;
	     default:
		 break;
	 }

	 /* set map display coordinate system's vertical datum from raster data header */
 	 switch( basemap->area.data_hdr.datum.ver_datum )
	 {
	     case UDEFINED:
	     case VD_ELIPSOID:
		     strcpy(mapgeo->map_display_coord_sys.pars.datums.vname, "ELLP");
	         break;
	     case VD_MSL:
		     /*****TEMP: the following macro is not yet supported.  Also, in the substitured strcpy(),
		     not sure if string should be MSL or AMSL, or if different vdatums are even supported */
		     /* dtcc_reset_system_vdatum( &mapgeo->map_display_coord_sys, "MSL" ); */
		     strcpy(mapgeo->map_display_coord_sys.pars.datums.vname, "MSL");
	         break;
	     default:
		     break;
	 } 

	 /* set map display coordinate system's units from raster data header */
	 /*****TEMP: What does it mean if h_units is MGRS or UTM, and how should they be handled? 
	             Also, dtcc_system_units_labelNP() no longer exsts (7/96) -- use
				 dtcc_coord3_units_labelNP(dtcc_system_unitsP(&mapgeo->map_display_coord_sys),N) !!!!!  */
	 switch( basemap->area.data_hdr.h_units )
	 {
	     case DDTEMP:
             dtcc_set_system_units( &mapgeo->map_display_coord_sys, dtcc_system_units_mode(&mapgeo->map_display_coord_sys),
                                    dtcc_system_map_scale(&mapgeo->map_display_coord_sys), 
                                    "degrees", "degrees", dtcc_system_units_labelNP(&mapgeo->map_display_coord_sys, 2), );          
		     break;
	     case DMS:
             dtcc_set_system_units( &mapgeo->map_display_coord_sys, dtcc_system_units_mode(&mapgeo->map_display_coord_sys),
                                    dtcc_system_map_scale(&mapgeo->map_display_coord_sys), 
                                    "DMS", "DMS", dtcc_system_units_labelNP(&mapgeo->map_display_coord_sys, 2), );          
		     break;
	     case MGRS:
		     break;
	     case MIN:
             dtcc_set_system_units( &mapgeo->map_display_coord_sys, dtcc_system_units_mode(&mapgeo->map_display_coord_sys),
                                    dtcc_system_map_scale(&mapgeo->map_display_coord_sys), 
                                    "minutes", "minutes", dtcc_system_units_labelNP(&mapgeo->map_display_coord_sys, 2), );          
		     break;
	     case SECS:
             dtcc_set_system_units( &mapgeo->map_display_coord_sys, dtcc_system_units_mode(&mapgeo->map_display_coord_sys),
                                    dtcc_system_map_scale(&mapgeo->map_display_coord_sys), 
                                    "seconds", "seconds", dtcc_system_units_labelNP(&mapgeo->map_display_coord_sys, 2), );          
		     break;
	     case UTM:
		     break;
	     default:
		     break;
	 }

 	 switch( basemap->area.data_hdr.v_units )
	 {
	     case VER_METERS:
             dtcc_set_system_units( &mapgeo->map_display_coord_sys, dtcc_system_units_mode(&mapgeo->map_display_coord_sys),
                                    dtcc_system_map_scale(&mapgeo->map_display_coord_sys), 
                                    dtcc_system_units_labelNP(&mapgeo->map_display_coord_sys, 0),
                                    dtcc_system_units_labelNP(&mapgeo->map_display_coord_sys, 1), 
                                    "meters" );          
		     break;
	     case VER_FATHOMS:
             dtcc_set_system_units( &mapgeo->map_display_coord_sys, dtcc_system_units_mode(&mapgeo->map_display_coord_sys),
                                    dtcc_system_map_scale(&mapgeo->map_display_coord_sys), 
                                    dtcc_system_units_labelNP(&mapgeo->map_display_coord_sys, 0),
                                    dtcc_system_units_labelNP(&mapgeo->map_display_coord_sys, 1), 
                                    "fathoms" );          
		     break;
	     case VER_FEET:
             dtcc_set_system_units( &mapgeo->map_display_coord_sys, dtcc_system_units_mode(&mapgeo->map_display_coord_sys),
                                    dtcc_system_map_scale(&mapgeo->map_display_coord_sys), 
                                    dtcc_system_units_labelNP(&mapgeo->map_display_coord_sys, 0),
                                    dtcc_system_units_labelNP(&mapgeo->map_display_coord_sys, 1), 
                                    "feet" );          
		     break;
	     default:
		 break;
	 }
#endif

     mapgeo->geo_extent.left = basemap->area.data_hdr.extent.left;
     mapgeo->geo_extent.right = basemap->area.data_hdr.extent.right;
     mapgeo->geo_extent.bottom = basemap->area.data_hdr.extent.bottom;
     mapgeo->geo_extent.top = basemap->area.data_hdr.extent.top;

	 mapgeo->plot_params.image_height_pixels = basemap->area.data_matrix.height;
	 mapgeo->plot_params.image_width_pixels = basemap->area.data_matrix.width;
	 mapgeo->plot_params.image_height_pixels_computed = basemap->area.data_matrix.height;
	 mapgeo->plot_params.image_width_pixels_computed = basemap->area.data_matrix.width;

	 /* 2/98--set use_arc to false here to support (prevent misregistration of) pre-ARC maps.
	          If raster has ARC ratios, map geometry will automatically conform.  If raster is
	          re-imported, use_arc will be set to true. */
	 mapgeo->use_arc = false;

     mapgeo->plot_params.map_pref = GEO_AND_IMAGE_EXTENT;
     if (mapgeo_adjust(mapgeo, false) == STAT_BAD_PARM )
	 {
	    iErrorVmapConv("Unable to adjust the map geometry");
		return STAT_BAD_PARM;
	 }

	mapgeo->plot_params.lon_shift_factor = 0.0;
	if( mapgeo->geo_extent.left < -180.0)
	{
		mapgeo->plot_params.lon_shift_factor = -360.0;
	}
	else if( mapgeo->geo_extent.right > 180.0)
	{
		mapgeo->plot_params.lon_shift_factor = 360.0;
	}

     return STAT_SUCCESS;
}

static const char* _check_system_name( const char* fn )
{	
	_SYSTEM_DIRECTORY_NAME = ( isFN(fn) ? (char*)fn : "" );
	return _check_file_name(fn, _SYSTEMS_FILENAME );  
}

short dtcc_scan_system( COORD_SYS* sysP, FILE* fp )
{	int i=0,j,status=0;
	char name[128], dtm_abbrv[64], abbrv[16], bfr[64],
			dtm[128], type[8] , vdtm[8], proj[64], utype[8];
	char u[3][8];
	COORD3 mns,mxs;
	double mnmx[4], gorg[2], misc[13];

	if( scanTo( fp, ',', 128, name ) == EOF ) 
		status = -1;
	else
	if( scanTo( fp, ',', 16, abbrv ) == EOF ) 
		status = -2;
	else
	if( scanTo( fp, ',', 8, type ) == EOF ) 
		status = -3;
	else
	if( scanTo( fp, ',', 128, dtm ) == EOF ) 
		status = -4;
	else
	if( scanTo( fp, ',', 64, dtm_abbrv ) == EOF ) 
		status = -5;
	else
	if( scanTo( fp, ',', 8, vdtm ) == EOF ) 
		status = -6;
			
	else
	{	for( j=0; j<4 && status == 0; j++ )
		{	if( scanTo( fp, ',', 64, bfr ) == EOF )
			{	status = -7;
				break;
			}
			else
				sscanf( bfr, "%lf", mnmx+j );
		}
		dtcc_set_coord3_geo( &mxs, mnmx[0],mnmx[1],DOUBLE_NULL );
		dtcc_set_coord3_geo( &mns, mnmx[2],mnmx[3],DOUBLE_NULL );
		for( j=0; j<3 && status == 0; j++ )
		{	if( scanTo( fp, ',', 8, u[j] ) == EOF )
			{	status = -8;
				break;
			}
		}
		for( j=0; j<2 && status == 0; j++ )
		{	if( scanTo( fp, ',', 128, bfr ) == EOF )
			{	status = -9;
				break;
			}
			else
				sscanf( bfr, "%lf", gorg+j );
		}
		if( scanTo( fp, ',', 64, proj ) == EOF ) 
			status = -10;
		
		for( j=0; j<13 && status == 0; j++ )
		{	if( scanTo( fp, ',', 128, bfr ) == EOF )
			{	status = -11;
				break;
			}
			else
			if( bfr[0] == 'D' )
				*(misc+j) = DOUBLE_NULL;
			else
				sscanf( bfr, "%lf", misc+j );
		}
		if( scanTo( fp, ',', 8, utype ) == EOF ) 
			status = -10;
	}
	if( status == 0 )
	{	DTCC_LABELS lbl;
		COORD_GROUP grp;
		DATUM_PARAMS dtm;
		PROJ_PARAMS prj;
		COORD_EXTENTS ext;
		COORD3_UNITS unt;
		COORD_ORIGIN org;
		if( dtcc_reset_coord_group(&grp,type) != 0 )
			status = -12;
		else
		if( dtcc_set_label(&lbl,name,abbrv,NULL,1,(short)atoi(utype)) == NULL )
			status = -13;
		else
		if( dtcc_reset_datum(&dtm, dtm_abbrv ) != 0 )
			status = -14;
		else
		if( dtcc_set_extents( &ext, NULL,NULL, &mns, &mxs ) != 0 )
			status = -15;
		else
		if( dtcc_set_coord3_units( &unt,(short)dtcc_coord_group_id(&grp), 1.0, u[0],u[1],u[2] ) != 0 )
			status = -17;
		else
		if( proj[0] == 0 ) 
			dtcc_reset_projection( &prj,NULL,dtcc_datum_ellipsoidP(&dtm)); 
		else
		{	status = dtcc_reset_projection( &prj, proj, 
						dtcc_datum_ellipsoidP(&dtm));
			dtcc_set_origin_geo( &org, misc[0], misc[1],0.0 );
			dtcc_set_origin_grid( &org, misc[2], misc[3],0.0,0 );
			dtcc_set_origin_gridSW( &org, misc[4], misc[5],0.0,0 );
			dtcc_set_origin_scale( &org, misc[6], misc[7], 1.0 );
			dtcc_set_origin_convergence( &org, misc[8] );
			dtcc_set_origin_azimuth( &org, misc[9] );
			dtcc_set_projection_origin( &prj, &org );
			dtcc_set_projection_std_parallels( &prj, misc[10],misc[11] );
		}

		if( status == 0 )
			status = _update_system
				( sysP, &lbl, &grp, &dtm, &prj, &ext, &unt, 2, (short)atoi(utype) );
	}
	return status;
}

short dtcc_reset_coord_group(	COORD_GROUP* cgP,const char*	label)
	{	COORD3_TYPES id;
		short k = _group_label( 1, &id, (char*)label ); 
		return dtcc_set_coord_group(cgP,id);
	}

short dtcc_set_extents( COORD_EXTENTS* ceP,const COORD3* mngridP,const COORD3* mxgridP,const COORD3* mngeoP,const COORD3* mxgeoP  )
	{	dtcc_set_grid_extents( ceP,
				       (double*)dtcc_coord3DataP(mngridP),
				       (double*)dtcc_coord3DataP(mxgridP));
		dtcc_set_geo_extents( ceP,
				      (double*)dtcc_coord3DataP(mngeoP),
				      (double*)dtcc_coord3DataP(mxgeoP));
		return _check_itcoordextent(ceP);
	}

short dtcc_set_projection_origin( PROJ_PARAMS*  ppP,const COORD_ORIGIN* orgP  )
    {  return _updateProjection( ppP, NULL, NULL, orgP, NULL );
    }

short dtcc_set_grid_extents( COORD_EXTENTS* ceP,const double* mnP,const double* mxP ) 
    {   if( ceP )
        {   if( mnP )
                dtcc_set_coord3_grid
                	( _EXTENTS_MIN_GRIDP(ceP), mnP[0],mnP[1],mnP[2],-1 );
            else
                dtcc_set_coord3_grid
                	( _EXTENTS_MIN_GRIDP(ceP), DOUBLE_NULL, DOUBLE_NULL, DOUBLE_NULL,-1); 
                	/*,-10000000.0, -30000000.0, -30000.0 ); */
            
            if( mxP )
                dtcc_set_coord3_grid
                	( _EXTENTS_MAX_GRIDP(ceP), mxP[0],mxP[1],mxP[2],-1 );
            else
                 dtcc_set_coord3_grid
                 	( _EXTENTS_MAX_GRIDP(ceP), DOUBLE_NULL, DOUBLE_NULL, DOUBLE_NULL,-1);
                	/*,10000000.0, 30000000.0, 30000.0 ); */
            return _check_itcoordextent(ceP);
        }
        else
            return ILLEGAL_ARG;
    }


short dtcc_set_geo_extents( COORD_EXTENTS* ceP,const double* mnP,const double* mxP ) 
    {   if( ceP )
        {   if( mnP )
                dtcc_set_coord3_geo
                	( _EXTENTS_MIN_GEOP(ceP), mnP[0],mnP[1],mnP[2] );
            else
                dtcc_set_coord3_geo
                	( _EXTENTS_MIN_GEOP(ceP), -90.0, -180.0, -30000.0 );
            
            if( mxP )
                dtcc_set_coord3_geo
                	( _EXTENTS_MAX_GEOP(ceP), mxP[0],mxP[1],mxP[2] );
            else
                 dtcc_set_coord3_geo
                 	( _EXTENTS_MAX_GEOP(ceP), 90.0, 180.0, 20000.0 );
            return _check_itcoordextent(ceP);
        }
        else
            return ILLEGAL_ARG;
    }

short dtcc_initialize_lists(	const char* ename, const char* dname, const char* sname )
	{	char *uname = "", *pname = "";
		if( dtcc_initialize_ellipsoid_list( ename ) == VMAPOK &&
		    dtcc_initialize_datum_list( dname ) == VMAPOK &&
			dtcc_initialize_system_list( sname ) == VMAPOK )
			return VMAPOK;
		else
		{	dtcc_delete_lists();
			return -1;
		}
	}

/* Delete the lists - get rid of everything */
short dtcc_delete_lists(void)
	{	dtcc_delete_ellipsoid_list(NULL); 
		dtcc_delete_datum_list(NULL); 
		dtcc_delete_system_list(NULL); 
		return 0;
	}

short dtcc_delete_system_list( _DTCC_LIST** lstP )
    {	int k;
    	lstP = ( lstP ? lstP : &slist );
    	if( *lstP ) 
    	{ 	if( _list_modified(*lstP)>0 || 	_list_added(*lstP)>0 ) 
    			k = _write_system_list(*lstP);
    		if( _list_dataP(*lstP) ) dtcc_free_ptr(_list_dataP(*lstP)); 
    		dtcc_free_ptr(*lstP);
    		*lstP = NULL; 
    	}
		return VMAPOK;
    }

	
static short _write_system_list(_DTCC_LIST* lstP)
	{	FILE* fp = NULL;
		if( !_list_dataP(lstP) )
			return ILLEGAL_ARG;
		else
		if( (fp = _write_listASCII_hdr( lstP,_slist_count(0,lstP) )) == NULL )
			return FILE_ERROR;
		else
		{	int i, n = _list_total(lstP);
			COORD_SYS* csP = (COORD_SYS*)_list_dataP(lstP);
			for( i=0; i<n; i++, csP++ )
				if( _accept_itcoordsys(csP)) 
					dtcc_writeASCII_system( csP,fp );
			fprintf( fp,"\nEND\n" );
			fclose(fp);
			_list_modified(lstP) = 0;
			_list_added(lstP) = 0;
			return VMAPOK;
		}
	}


FILE* _write_listASCII_hdr( const _DTCC_LIST* lstP, short count )
	{	if( lstP )
		{	short flag = 1;
			FILE* fp = fopen( _list_nameP(lstP), "w" );
			if(fp)
			{	int where = ftell(fp);
				short type2 = count - _list_type1(lstP) - _list_modified(lstP); /* type 1 is invariant */
				fprintf( fp, "\t%s | ", _list_olabP(lstP) );
				fprintf( fp, " %d; \n", count -  _list_modified(lstP) );
			}  
			return fp;
		}
		return NULL;
	}

static short _accept_itcoordsys( const COORD_SYS* csP )
	{	if( dtcc_is_typeN(dtcc_type_flag(csP), _TYPE1_LIST) || 
		  ( dtcc_is_typeN(dtcc_type_flag(csP), _TYPE2_LIST) && dtcc_valid_flag(csP) ) )
		  	return 1;
		else
			return 0;
	}


short dtcc_writeASCII_system( const COORD_SYS* cpP, FILE* fp )
	{	short status = 0;
		if( !cpP || !fp )
			status = ILLEGAL_ARG;
		else
		{	char b1[256], b2[64];
			fprintf( fp, "\t%s,\t\t\t/* Name */\n",  
						dtcc_nameP(cpP) ); 
        	fprintf( fp, "\t\t%s,\t\t/* Abbreviation */\n",  
						dtcc_abbrvP(cpP) ); 
        	dtcc_encode_coord_group( _SYSTEM_GROUPP(cpP), b1 );
			fprintf( fp, "\t\t%s,\t\t/* Coordinate type */\n", b1 );
        	fprintf( fp, "\t\t%s,\t\t/* Horizontal datum */\n",  
						dtcc_system_hdatum_nameP(cpP) ); 
        	fprintf( fp, "\t\t%s,\t\t/* Horizontal datum sub-area */\n", 
						dtcc_system_hdatum_abbrvP(cpP) ); 
        	fprintf( fp, "\t\t%s,\t\t/* Vertical datum */\n",  
						dtcc_system_vdatum_nameP(cpP) ); 
        	fprintf( fp, "\t\t%lf, %lf,\t\t/* Maximum geographic values */\n",
						dtcc_system_max_lat(cpP),dtcc_system_max_lon(cpP) );
        	fprintf( fp, "\t\t%lf, %lf,\t\t/* Minimum geographic values */\n",
						dtcc_system_min_lat(cpP),dtcc_system_min_lon(cpP) );
        	fprintf( fp, "\t\t%s, %s, %s,\t\t/* Angular, horz, & vert units */\n",
						dtcc_coord3_units_abbrvNP( _SYSTEM_UNITSP(cpP), 0),
						dtcc_coord3_units_abbrvNP( _SYSTEM_UNITSP(cpP), 1),
						dtcc_coord3_units_abbrvNP( _SYSTEM_UNITSP(cpP), 2) );
			fprintf( fp, "\t\t%lf, %lf,\t\t/* Geographic origin */\n", 0.0,0.0 );
         	fprintf( fp, "\t\t%s,\t\t/* Associated projection */\n",  
						dtcc_system_projection_nameP(cpP) ); 
         	_encode( dtcc_origin_lat0(dtcc_system_originP( cpP )), b1 );
			_encode( dtcc_origin_lon0(dtcc_system_originP( cpP )), b2 );
         	fprintf( fp, "\t\t%s, %s,\t\t/* geographic origin of grid */\n",b1,b2 );
          	_encode( dtcc_origin_fn(dtcc_system_originP( cpP )), b1 );
			_encode( dtcc_origin_fe(dtcc_system_originP( cpP )), b2 );
         	fprintf( fp, "\t\t%s, %s,\t\t/* NE false northing and easting */\n", b1,b2 );
         	_encode( dtcc_origin_fnSW(dtcc_system_originP( cpP )), b1 );
			_encode( dtcc_origin_feSW(dtcc_system_originP( cpP )), b2 );
         	fprintf( fp, "\t\t%s, %s,\t\t/* SW false northing and easting */\n",b1,b2 );
         	_encode( dtcc_origin_K0(dtcc_system_originP( cpP )), b1 );
			_encode( dtcc_origin_H0(dtcc_system_originP( cpP )), b2 );
         	fprintf( fp, "\t\t%s, %s,\t\t/* scale at origin */\n", b1,b2);
         	_encode( dtcc_origin_conv0(dtcc_system_originP( cpP )), b1 );
			_encode( dtcc_origin_azm0(dtcc_system_originP( cpP )), b2 );
         	fprintf( fp, "\t\t%s, %s,\t\t/* convergence and azm at origin */\n",b1,b2 );
         	_encode( dtcc_system_stdparsP(cpP)[0], b1 );
			_encode( dtcc_system_stdparsP(cpP)[1], b2 );
         	fprintf( fp, "\t\t%s, %s,\t\t/* standard parallels */\n", b1,b2);
          	fprintf( fp, "\t\t%lf, %d,\t\t/* map scale & user type*/\n",
						dtcc_system_map_scale(cpP),dtcc_type_flag(cpP)	);
		}
		return status;
	}

static void _encode( double v, char* bfr )
{	if( v == DOUBLE_NULL )
		strcpy( bfr,"DOUBLE_NULL" );
	else
		sprintf( bfr, "%lf", v );
	return;
}

static short _check_listdatum( int type, short *start, short *stop )
	{	if( dlist == NULL )
		{	if( dtcc_initialize_datum_list(NULL) != VMAPOK )
				return FILE_NOT_OPEN;
		}
		*start = 0; *stop = dtcc_datum_count();
		if( type != ALL_DATUMS )
		{	*start = ( dtcc_is_typeN((short)type, USER_DATUMS) ? 
						dtcc_supported_datum_count() : 0 );
			*stop =  ( dtcc_is_typeN((short)type, SUPPORTED_DATUMS) ? 
							dtcc_supported_datum_count() : dtcc_datum_count() );
		}
		return VMAPOK;
    }	

static short _check_listellipse( int type, short *start, short *stop )
	{	if( elist == NULL )
		{	if( dtcc_initialize_ellipsoid_list(NULL) != VMAPOK )
				return FILE_NOT_OPEN;
		}
		*start = ( type == _TYPE2_LIST ? dtcc_supported_ellipsoid_count() : 0 );
		*stop =  ( type == _TYPE1_LIST ? dtcc_supported_ellipsoid_count() : 
					dtcc_ellipsoid_count()  );
		return VMAPOK;
    }	


short _dlist_count( int k, _DTCC_LIST* listP )
	{	if( listP == NULL ) listP = dlist;
		if( !listP )	
			return 0;
		else
		if( k == SUPPORTED_DATUMS )
			return	_list_type1( listP );
		else
		if( k == USER_DATUMS )
			return _list_type2( listP ) - _list_modified(listP);
		else
		if( k == ALL_DATUMS )
			return _list_type1( listP ) + _list_type2( listP );
		else
		{	DATUM_PARAMS* dpP = (DATUM_PARAMS*)_list_dataP(listP);
            short n = 0, i = _list_type1( listP );
			while( --i >= 0 ) if( dtcc_is_typeN( dtcc_type_flag(dpP++),k ) ) n++;
			return n;
		}
	}

#pragma warning ( pop )
