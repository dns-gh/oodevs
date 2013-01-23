// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_interactions_InteractionsRpr_h
#define plugins_hla_interactions_InteractionsRpr_h

#include "Omt13String.h"
#include "Transportation.h"
#include "UnicodeString.h"
#include "rpr/EntityIdentifier.h"
#include "rpr/EntityType.h"
#include "rpr/Coordinates.h"
#include <hla/HLA_Types.h>
#include <algorithm>

namespace plugins
{
namespace hla
{
namespace interactions
{
struct EventIdentifierStruct
{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << eventCount;
        issuingObjectIdentifier.Serialize( archive );
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> eventCount;
        issuingObjectIdentifier.Deserialize( archive );
    }
    uint16_t eventCount;
    Omt13String issuingObjectIdentifier;
};

struct MunitionDetonation
{
    std::vector< int8_t > articulatedPartData;
    rpr::WorldLocation detonationLocation;
    int8_t detonationResultCode;
    EventIdentifierStruct eventIdentifier;
    Omt13String firingObjectIdentifier;
    rpr::VelocityVector finalVelocityVector;
    int16_t fuseType;
    Omt13String munitionObjectIdentifier;
    rpr::EntityType munitionType;
    uint16_t quantityFired;
    uint16_t rateOfFire;
    rpr::VelocityVector relativeDetonationLocation; // RelativePositionStruct
    Omt13String targetObjectIdentifier;
    int16_t warheadType;
};

struct WeaponFire
{
    EventIdentifierStruct eventIdentifier;
    float fireControlSolutionRange;
    uint32_t fireMissionIndex;
    rpr::WorldLocation firingLocation;
    Omt13String firingObjectIdentifier;
    int16_t fuseType;
    rpr::VelocityVector initialVelocityVector;
    Omt13String munitionObjectIdentifier;
    rpr::EntityType munitionType;
    uint16_t quantityFired;
    uint16_t rateOfFire;
    Omt13String targetObjectIdentifier;
    int16_t warheadType;
};

struct Acknowledge
{
    enum AcknowledgeFlagEnum16
    {
        E_CreateEntity = 1,
        E_RemoveEntity = 2,
        E_StartResume = 3,
        E_StopFreeze = 4
    };
    enum ResponseFlagEnum16
    {
        E_Other = 0,
        E_AbleToComply = 1,
        E_UnableToComply = 2
    };

    rpr::EntityIdentifier originatingEntity;
    rpr::EntityIdentifier receivingEntity;
    uint32_t requestIdentifier;
    uint16_t acknowledgeFlag;
    uint16_t responseFlag;
};

struct SupplyStruct
{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << supplyType
                << quantity;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> supplyType
                >> quantity;
    }

    rpr::EntityType supplyType;
    real32 quantity;
};

struct ServiceRequest
{
    enum ServiceTypeEnum
    {
        Other       = 0,
        Resupply    = 1,
        Repair      = 2
    };

    Omt13String requestingObject;
    Omt13String servicingObject;
    uint8_t serviceType;
    std::vector< SupplyStruct > suppliesData;
};

struct ResupplyCancel
{
    Omt13String receivingObject;
    Omt13String supplyingObject;
};

struct ResupplyOffer
{
    Omt13String receivingObject;
    Omt13String supplyingObject;
    std::vector< SupplyStruct > suppliesData;
};

struct ResupplyReceived
{
    Omt13String receivingObject;
    Omt13String supplyingObject;
    std::vector< SupplyStruct > suppliesData;
};

struct RecordSet
{
    struct RecordStruct
    {
        template< typename Archive >
        void Serialize( Archive& archive ) const
        {
            int32_t size = static_cast< int32_t >( data.size() );
            archive << numberOfBits
                    << size
                    << data;
        }
        template< typename Archive >
        void Deserialize( Archive& archive )
        {
            int32_t size = 0;
            archive >> numberOfBits
                    >> size;
            data.resize( size );
            archive >> data;
        }
        uint32_t numberOfBits;
        std::vector<char> data;
    };
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        int32_t size = static_cast< int32_t >( values.size() );
        archive << recordSetIdentifier
                << recordSetSerialNumber
                << size
                << values;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        int32_t size = 0;
        archive >> recordSetIdentifier
                >> recordSetSerialNumber
                >> size;
        values.resize( size );
        archive >> values;
    }
    uint32_t recordSetIdentifier;
    uint32_t recordSetSerialNumber;
    std::vector<RecordStruct> values;
};

struct RecordSetList
{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        int32_t size = static_cast< int32_t >( values.size() );
        archive << size
                << values;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        int32_t size = 0;
        archive >> size;
        values.resize( size );
        archive >> values;
    }
    std::vector<RecordSet> values;
};

struct TransferControl
{
    enum TransferTypeEnum8
    {
        E_Other   = 0,
        E_EntityPush  = 1,
        E_EntityPull  = 2,
        E_EntitySwap  = 3,
        E_EnvironmentalProcessPush    = 4,
        E_EnvironmentalProcessPull    = 5,
        E_Cancel  = 6,
        E_EnvironmentalProcessSwap    = 7
    };

    rpr::EntityIdentifier originatingEntity;
    rpr::EntityIdentifier receivingEntity;
    uint32_t requestIdentifier;
    uint8_t transferType;
    Omt13String transferEntity;
    RecordSetList recordSetData;
};

struct VariableDatum
{
    enum DatumIdentifierEnum32
    {
        Unknown = 0,
        Entity_Identification = 10000,
        Entity_Type    = 11000,
        Concatenated   = 11100,
        Entity_Type__Kind   = 11110,
        Entity_Type__Domain = 11120,
        Entity_Type__Country    = 11130,
        Entity_Type__Category   = 11140,
        Entity_Type__Subcategory    = 11150,
        Entity_Type__Specific   = 11160,
        Entity_Type__Extra  = 11170,
        Force_ID   = 11200,
        Description    = 11300,
        Alternative_Entity_Type   = 12000,
        Alternative_Entity_Type__Kind  = 12110,
        Alternative_Entity_Type__Domain= 12120,
        Alternative_Entity_Type__Country   = 12130,
        Alternative_Entity_Type__Category  = 12140,
        Alternative_Entity_Type__Subcategory   = 12150,
        Alternative_Entity_Type__Specific  = 12160,
        Alternative_Entity_Type__Extra = 12170,
        Alternative_Entity_Type__Description   = 12300,
        Entity_Marking = 13000,
        Entity_Marking_Characters  = 13100,
        Crew_ID    = 13200,
        Task_Organization  = 14000,
        Regiment_Name  = 14200,
        Battalion_Name = 14300,
        Company_Name   = 14400,
        Platoon_Name   = 14500,
        Squad_Name = 14520,
        Team_Name  = 14540,
        Bumper_Number  = 14600,
        Vehicle_Number = 14700,
        Unit_Number    = 14800,
        DIS_Identity   = 15000,
        DIS_Site_ID    = 15100,
        DIS_Host_ID    = 15200,
        DIS_Entity_ID  = 15300,
        Mount_Intent   = 15400,
        Tether_Unthether_Command_ID    = 15500,
        Teleport_Entity_Data_Record    = 15510,
        DIS_Aggregate_ID   = 15600,
        Loads  = 20000,
        Crew_Members   = 21000,
        Crew_Member_ID = 21100,
        Health = 21200,
        Job_Assignment = 21300,
        Fuel   = 23000,
        Fuel_Quantity_Liters   = 23100,
        Fuel_Quantity_Gallons  = 23105,
        Ammunition = 24000,
        Ammunition_quantity_120mm_HEAT = 24001,
        Ammunition_quantity_120mm_SABOT    = 24002,
        Ammunition_quantity_12_7mm_M8  = 24003,
        Ammunition_quantity_12_7mm_M20 = 24004,
        Ammunition_quantity_7_62mm_M62 = 24005,
        Ammunition_quantity_M250_UKL8A1    = 24006,
        Ammunition_quantity_M250_UKL8A3    = 24007,
        Ammunition_quantity_7_62mm_M80 = 24008,
        Ammunition_quantity_12_7mm = 24009,
        Ammunition_quantity_7_62mm = 24010,
        Mines_quantity = 24060,
        Mines_Type = 24100,
        Mines_Kind = 24110,
        Mines_Domain   = 24120,
        Mines_Country  = 24130,
        Mines_Category = 24140,
        Mines_Subcategory  = 24150,
        Mines_Extra    = 24160,
        Mines_Description  = 24300,
        Cargo  = 25000,
        Vehicle_Mass   = 26000,
        Supply_Quantity    = 27000,
        Armament   = 28000,
        Status = 30000,
        Subscription_State = 30100,
        Round_trip_time_delay  = 30300,
        TADIL_J_message_count_label0   = 30400,
        TADIL_J_message_count_label1   = 30401,
        TADIL_J_message_count_label2   = 30402,
        TADIL_J_message_count_label3   = 30403,
        TADIL_J_message_count_label4   = 30404,
        TADIL_J_message_count_label5   = 30405,
        TADIL_J_message_count_label6   = 30406,
        TADIL_J_message_count_label7   = 30407,
        TADIL_J_message_count_label8   = 30408,
        TADIL_J_message_count_label9   = 30409,
        TADIL_J_message_count_label10  = 30410,
        TADIL_J_message_count_label11  = 30411,
        TADIL_J_message_count_label12  = 30412,
        TADIL_J_message_count_label13  = 30413,
        TADIL_J_message_count_label14  = 30414,
        TADIL_J_message_count_label15  = 30415,
        TADIL_J_message_count_label16  = 30416,
        TADIL_J_message_count_label17  = 30417,
        TADIL_J_message_count_label18  = 30418,
        TADIL_J_message_count_label19  = 30419,
        TADIL_J_message_count_label20  = 30420,
        TADIL_J_message_count_label21  = 30421,
        TADIL_J_message_count_label22  = 30422,
        TADIL_J_message_count_label23  = 30423,
        TADIL_J_message_count_label24  = 30424,
        TADIL_J_message_count_label25  = 30425,
        TADIL_J_message_count_label26  = 30426,
        TADIL_J_message_count_label27  = 30427,
        TADIL_J_message_count_label28  = 30428,
        TADIL_J_message_count_label29  = 30429,
        TADIL_J_message_count_label30  = 30430,
        TADIL_J_message_count_label31  = 30431,
        Position   = 31000,
        Route_Waypoint_Type    = 31010,
        MilGrid10  = 31100,
        Geocentric_Coordinates = 31200,
        Geocentric_Coordinate_X    = 31210,
        Geocentric_Coordinate_Y    = 31220,
        Geocentric_Coordinate_Z    = 31230,
        Latitude   = 31300,
        Longitude  = 31400,
        Line_of_Sight  = 31500,
        Line_of_Sight_X    = 31510,
        Line_of_Sight_Y    = 31520,
        Line_of_Sight_Z    = 31530,
        Altitude   = 31600,
        Destination_Latitude   = 31700,
        Destination_Longitude  = 31800,
        Destination_Altitude   = 31900,
        Orientation    = 32000,
        Hull_Heading_Angle = 32100,
        Hull_Pitch_Angle   = 32200,
        Roll_Angle = 32300,
        Roll_Angle_X   = 32500,
        Roll_Angle_Y   = 32600,
        Roll_Angle_Z   = 32700,
        Appearance = 33000,
        Ambient_Lighting   = 33100,
        Lights = 33101,
        Paint_Scheme   = 33200,
        Smoke  = 33300,
        Trailing_Effects   = 33400,
        Flaming    = 33500,
        Marking    = 33600,
        Mine_Plows_Attached    = 33710,
        Mine_Rollers_Attached  = 33720,
        Tank_Turret_Azimuth    = 33730,
        Failures_and_Malfunctions  = 34000,
        Age    = 34100,
        Kilometers = 34110,
        Damage = 35000,
        Cause  = 35050,
        Mobility_Kill  = 35100,
        Fire_Power_Kill    = 35200,
        Personnel_Casualties   = 35300,
        Velocity   = 36000,
        X_velocity = 36100,
        Y_velocity = 36200,
        Z_velocity = 36300,
        Speed  = 36400,
        Acceleration   = 37000,
        X_acceleration = 37100,
        Y_acceleration = 37200,
        Z_acceleration = 37300,
        Engine_Status  = 38100,
        Primary_Target_Line    = 39000,
        Exercise   = 40000,
        Exercise_State = 40010,
        Restart_slsh_Refresh   = 40015,
        AFATDS_File_Name   = 40020,
        Terrain_Database   = 41000,
        Missions   = 42000,
        Mission_ID = 42100,
        Mission_Type   = 42200,
        Mission_Request_Time_Stamp = 42300,
        Exercise_Description   = 43000,
        Name   = 43100,
        Entities   = 43200,
        Version    = 43300,
        Guise_Mode = 43410,
        Simulation_Application_Active_Status   = 43420,
        Simulation_Application_Role_Record = 43430,
        Simulation_Application_State   = 43440,
        Visual_Output_Mode = 44000,
        Simulation_Manager_Role    = 44100,
        Simulation_Manager_Site_ID = 44110,
        Simulation_Manager_Application_ID  = 44120,
        Simulation_Manager_Entity_ID   = 44130,
        Simulation_Manager_Active_Status   = 44140,
        After_Active_Review_Role   = 44200,
        After_Active_Review_Site_ID    = 44210,
        After_Active_Application_ID    = 44220,
        After_Active_Review_Entity_ID  = 44230,
        After_Active_Review_Active_Status  = 44240,
        Exercise_Logger_Role   = 44300,
        Exercise_Logger_Site_ID    = 44310,
        Exercise_Logger_Application_ID = 44320,
        Exercise_Entity_ID = 44330,
        Exercise_Logger_Active_Status  = 44340,
        Synthetic_Environment_Manager_Role = 44400,
        Synthetic_Environment_Manager_Site_ID  = 44410,
        Synthetic_Environment_Manager_Application_ID   = 44420,
        Synthetic_Environment_Manager_Entity_ID    = 44430,
        Synthetic_Environment_Manager_Active_Status    = 44440,
        SIMNET_DIS_Translator_Role = 44500,
        SIMNET_DIS_Translator_Site_ID  = 44510,
        SIMNET_DIS_Translator_Application_ID   = 44520,
        SIMNET_DIS_Translator_Entity_ID    = 44530,
        SIMNET_DIS_Translator_Active_Status    = 44540,
        Application_Rate   = 45000,
        Application_Time   = 45005,
        Application_Timestep   = 45010,
        Feedback_Time  = 45020,
        Simulation_Rate    = 45030,
        Simulation_Time    = 45040,
        Simulation_Timestep    = 45050,
        Time_Interval  = 45060,
        Time_Latency   = 45070,
        Time_Scheme    = 45080,
        Exercise_Elapsed_Time  = 46000,
        Elapsed_Time   = 46010,
        Environment    = 50000,
        Weather    = 51000,
        Weather_Condition  = 51010,
        Thermal_Condition  = 51100,
        Thermal_Visibility_FloatingPoint32 = 51110,
        Thermal_Visibility_UnsignedInteger32   = 51111,
        Time   = 52000,
        Time_String    = 52001,
        Time_of_Day_Discrete   = 52100,
        Time_of_Day_Continuous = 52200,
        Time_Mode  = 52300,
        Time_Scene = 52305,
        Current_Hour   = 52310,
        Current_Minute = 52320,
        Current_Second = 52330,
        Azimuth    = 52340,
        Maximum_Elevation  = 52350,
        Time_Zone  = 52360,
        Time_Rate  = 52370,
        Simulation_Time_2  = 52380,
        Time_Sunrise_Enabled   = 52400,
        Sunrise_Hour   = 52410,
        Sunrise_Minute = 52420,
        Sunrise_Second = 52430,
        Sunrise_Azimuth    = 52440,
        Time_Sunset_Enabled    = 52500,
        Sunset_Hour    = 52510,
        Sunset_Hour_2  = 52511,
        Sunset_Minute  = 52520,
        Sunset_Second  = 52530,
        Date   = 52600,
        Date_European  = 52601,
        Date_US    = 52602,
        Month  = 52610,
        Day    = 52620,
        Year   = 52630,
        Clouds = 53000,
        Cloud_Layer_Enable = 53050,
        Cloud_Layer_Selection  = 53060,
        Cloud_Visibility   = 53100,
        Base_Altitude_Meters   = 53200,
        Base_Altitude_Feet = 53250,
        Ceiling_Meters = 53300,
        Ceiling_Feet   = 53350,
        Characteristics    = 53400,
        Concentration_Length   = 53410,
        Transmittance  = 53420,
        Radiance   = 53430,
        Precipitation  = 54000,
        Rain   = 54100,
        Fog    = 55000,
        Visibility_Meters  = 55100,
        Visibility_Meters_UnsignedInteger32    = 55101,
        Visibility_Miles   = 55105,
        Fog_Density    = 55200,
        Base   = 55300,
        View_Layer_from_above  = 55401,
        Transition_Range   = 55410,
        Bottom_Meters  = 55420,
        Bottom_Feet    = 55425,
        Fog_Ceiling_Meters = 55430,
        Fog_Ceiling_Feet   = 55435,
        Heavenly_Bodies    = 56000,
        Sun    = 56100,
        Sun_Visible    = 56105,
        Sun_Position   = 56110,
        Sun_Position_Elevation_Degrees = 56111,
        Sun_Position_Azimuth   = 56120,
        Sun_Position_Azimuth_Degrees   = 56121,
        Sun_Position_Elevation = 56130,
        Sun_Position_Intensity = 56140,
        Moon   = 56200,
        Moon_Visible   = 56205,
        Moon_Position  = 56210,
        Moon_Position_Azimuth  = 56220,
        Moon_Position_Azimuth_Degrees  = 56221,
        Moon_Position_Elevation    = 56230,
        Moon_Position_Elevation_Degrees    = 56231,
        Moon_Position_Intensity    = 56240,
        Horizon    = 56310,
        Horizon_Azimuth    = 56320,
        Horizon_Elevation  = 56330,
        Horizon_Heading    = 56340,
        Horizon_Intensity  = 56350,
        Humidity   = 57200,
        Visibility = 57300,
        Winds  = 57400,
        Speed_2    = 57410,
        Wind_Speed_Knots   = 57411,
        Wind__Direction = 57420,
        Wind_Direction_Degrees = 57421,
        Rainsoak   = 57500,
        Tide_Speed = 57610,
        Tide_Speed_Knots   = 57611,
        Tide_Direction = 57620,
        Tide_Direction_Degrees = 57621,
        Haze   = 58000,
        Haze_Visibility_Meters = 58100,
        Haze_Visibility_Miles  = 58105,
        Haze_Density   = 58200,
        Haze_Ceiling_Meters    = 58430,
        Haze_Ceiling_Feet  = 58435,
        Contaminants_and_Obscurants    = 59000,
        Contaminant_slsh_Obscurant_Type    = 59100,
        Persistence    = 59110,
        Chemical_Dosage    = 59115,
        Chemical_Air_Concentration = 59120,
        Chemical_Ground_Deposition = 59125,
        Chemical_Maximum_Ground_Deposition = 59130,
        Chemical_Dosage_Threshold  = 59135,
        Biological_Dosage  = 59140,
        Biological_Air_Concentration   = 59145,
        Biological_Dosage_Threshold    = 59150,
        Biological_Binned_Particle_Count   = 59155,
        Radiological_Dosage    = 59160,
        Communications = 60000,
        Channel_Type   = 61100,
        Channel_Type_2 = 61101,
        Channel_Identification = 61200,
        Alpha_Identification   = 61300,
        Radio_Identification   = 61400,
        Land_Line_Identification   = 61500,
        Intercom_Identification    = 61600,
        Group_Network_Channel_Number   = 61700,
        Radio_Communications_Status    = 62100,
        Stationary_Radio_Transmitters_Default_Time = 62200,
        Moving_Radio_Transmitters_Default_Time = 62300,
        Stationary_Radio_Signals_Default_Time  = 62400,
        Moving_Radio_Signal_Default_Time   = 62500,
        Radio_Initialization_Transec_Security_Key  = 63101,
        Radio_Initialization_Internal_Noise_Level  = 63102,
        Radio_Initialization_Squelch_Threshold = 63103,
        Radio_Initialization_Antenna_Location  = 63104,
        Radio_Initialization_Antenna_Pattern_Type  = 63105,
        Radio_Initialization_Antenna_Pattern_Length    = 63106,
        Radio_Initialization_Beam_Definition   = 63107,
        Radio_Initialization_Transmit_Heartbeat_Time   = 63108,
        Radio_Initialization_Transmit_Distance_Threshold   = 63109,
        Radio_Channel_Initialization_Lockout_ID    = 63110,
        Radio_Channel_Initialization_Hopset_ID = 63111,
        Radio_Channel_Initialization_Preset_Frequency  = 63112,
        Radio_Channel_Initialization_Frequency_Sync_Time   = 63113,
        Radio_Channel_Initialization_Comsec_Key    = 63114,
        Radio_Channel_Initialization_Alpha = 63115,
        Algorithm_Parameters   = 70000,
        Dead_Reckoning_Algorithm__lt_DRA_gt_   = 71000,
        DRA_Location_Threshold = 71100,
        DRA_Orientation_Threshold  = 71200,
        DRA_Time_Threshold = 71300,
        Simulation_Management_Parameters   = 72000,
        Checkpoint_Interval    = 72100,
        Transmitter_Time_Threshold = 72600,
        Receiver_Time_Threshold    = 72700,
        Interoperability_Mode  = 73000,
        SIMNET_Data_Collection = 74000,
        Event_ID   = 75000,
        Source_Site_ID = 75100,
        Source_Host_ID = 75200,
        Articulated_Parts  = 90000,
        Articulated_Parts_Part_ID  = 90050,
        Articulated_Parts_Index    = 90070,
        Articulated_Parts_Position = 90100,
        Articulated_Parts_Position_Rate    = 90200,
        Articulated_Parts_Extension    = 90300,
        Articulated_Parts_Extension_Rate   = 90400,
        Articulated_Parts_X    = 90500,
        Articulated_Parts_X_rate   = 90600,
        Articulated_Parts_Y    = 90700,
        Articulated_Parts_Y_rate   = 90800,
        Articulated_Parts_Z    = 90900,
        Articulated_Parts_Z_rate   = 91000,
        Articulated_Parts_Azimuth  = 91100,
        Articulated_Parts_Azimuth_Rate = 91200,
        Articulated_Parts_Elevation    = 91300,
        Articulated_Parts_Elevation_Rate   = 91400,
        Articulated_Parts_Rotation = 91500,
        Articulated_Parts_Rotation_Rate    = 91600,
        DRA_Angular_X_Velocity = 100001,
        DRA_Angular_Y_Velocity = 100002,
        DRA_Angular_Z_Velocity = 100003,
        Appearance_Trailing_Effects    = 100004,
        Appearance_Hatch   = 100005,
        Appearance_Character_Set   = 100008,
        Capability_Ammunition_Supplier = 100010,
        Capability_Miscellaneous_Supplier  = 100011,
        Capability_Repair_Provider = 100012,
        Articulation_Parameter = 100014,
        Articulation_Parameter_Type    = 100047,
        Articulation_Parameter_Value   = 100048,
        Time_of_Day_Scene  = 100058,
        Latitude_North__lt_Location_of_weather_cell_gt_    = 100061,
        Longitude_East__lt_Location_of_weather_cell_gt_    = 100063,
        Tactical_Driver_Status = 100068,
        Sonar_System_Status    = 100100,
        Upper_latitude = 100161,
        Latitude_South__lt_Location_of_weather_cell_gt_    = 100162,
        Longitude_West__lt_location_of_weather_cell_gt_    = 100164,
        Accomplished   = 100165,
        CD_ROM_Number__lt_Disk_ID_for_terrain_gt_  = 100166,
        Altitude_1 = 100167,
        Tactical_System_Status = 100169,
        JTIDS_Status   = 100170,
        TADIL_J_Status = 100171,
        DSDD_Status    = 100172,
        Weapon_System_Status   = 100200,
        Subsystem_status   = 100205,
        Number_of_interceptors_fired   = 100206,
        Number_of_interceptor_detonations  = 100207,
        Number_of_message_buffers_dropped  = 100208,
        Satellite_sensor_background__lt_year_day_gt_   = 100213,
        Satellite_sensor_background__lt_hour_minute_gt_    = 100214,
        Script_Number  = 100218,
        Entity_slsh_Track_slsh_Update_Data = 100300,
        Local_slsh_Force_Training  = 100400,
        Entity_slsh_Track_Identity_Data    = 100500,
        Entity_for_Track_Event = 100510,
        IFF__lt_Friend_Foe_gt__status  = 100520,
        Engagement_Data    = 100600,
        Target_Latitude    = 100610,
        Target_Longitude   = 100620,
        Area_of_Interest__lt_Ground_Impact_Circle_gt__Center_Latitude  = 100631,
        Area_of_Interest__lt_Ground_Impact_Circle_gt__Center_Longitude = 100632,
        Area_of_Interest__lt_Ground_Impact_Circle_gt__Radius   = 100633,
        Area_of_Interest_Type  = 100634,
        Target_Aggregate_ID    = 100640,
        GIC_Identification_Number  = 100650,
        Estimated_Time_of_Flight_to_TBM_Impact = 100660,
        Estimated_Intercept_Time   = 100661,
        Estimated_Time_of_Flight_to_Next_Waypoint  = 100662,
        Entity_slsh_Track_Equipment_Data   = 100700,
        Emission_slsh_EW_Data  = 100800,
        Appearance_Data    = 100900,
        Command_slsh_Order_Data    = 101000,
        Environmental_Data = 101100,
        Significant_Event_Data = 101200,
        Operator_Action_Data   = 101300,
        ADA_Engagement_Mode    = 101310,
        ADA_Shooting_Status    = 101320,
        ADA_Mode   = 101321,
        ADA_Radar_Status   = 101330,
        Shoot_Command  = 101340,
        ADA_Weapon_Status  = 101350,
        ADA_Firing_Disciple    = 101360,
        Order_Status   = 101370,
        Time_Synchronization   = 101400,
        Tomahawk_Data  = 101500,
        Number_of_Detonations  = 102100,
        Number_of_Intercepts   = 102200,
        OBT_Control_MT_201 = 200201,
        Sensor_Data_MT_202 = 200202,
        Environmental_Data_MT_203  = 200203,
        Ownship_Data_MT_204    = 200204,
        Acoustic_Contact_Data_MT_205   = 200205,
        Sonobuoy_Data_MT_207   = 200207,
        Sonobuoy_Contact_Data_MT_210   = 200210,
        Helo_Control_MT_211    = 200211,
        ESM_Control_Data   = 200213,
        ESM_Contact_Data_MT_214    = 200214,
        ESM_Emitter_Data_MT_215    = 200215,
        Weapon_Definition_Data_MT_217  = 200216,
        Weapon_Preset_Data_MT_217  = 200217,
        OBT_Control_MT_301 = 200301,
        Sensor_Data_MT_302 = 200302,
        Environmental_Data_MT_303m = 200303,
        Ownship_Data_MT_304    = 200304,
        Acoustic_Contact_Data_MT_305   = 200305,
        Sonobuoy_Data_MT_307   = 200307,
        Sonobuoy_Contact_Data_MT_310   = 200310,
        Helo_Scenario__slsh__Equipment_Status  = 200311,
        ESM_Control_Data_MT_313    = 200313,
        ESM_Contact_Data_MT_314    = 200314,
        ESM_Emitter_Data_MT_315    = 200315,
        Weapon_Definition_Data_MT_316  = 200316,
        Weapon_Preset_Data_MT_317  = 200317,
        Pairing_slsh_Association__lt_eMT_56_gt_    = 200400,
        Pointer__lt_eMT_57_gt_ = 200401,
        Reporting_Responsibility__lt_eMT_58_gt_    = 200402,
        Track_Number__lt_eMT_59_gt_    = 200403,
        ID_for_Link_11_Reporting__lt_eMT_60_gt_    = 200404,
        Remote_Track__lt_eMT_62_gt_    = 200405,
        Link_11_Error_Rate__lt_eMT_63_gt_  = 200406,
        Track_Quality__lt_eMT_64_gt_   = 200407,
        Gridlock__lt_eMT_65_gt_    = 200408,
        Kill__lt_eMT_66_gt_    = 200409,
        Track_ID_Change__slsh__Resolution__lt_eMT_68_gt_   = 200410,
        Weapons_Status__lt_eMT_69_gt_  = 200411,
        Link_11_Operator__lt_eMT_70_gt_    = 200412,
        Force_Training_Transmit__lt_eMT_71_gt_ = 200413,
        Force_Training_Receive__lt_eMT_72_gt_  = 200414,
        Interceptor_Amplification__lt_eMT_75_gt_   = 200415,
        Consumables__lt_eMT_78_gt_ = 200416,
        Link_11_Local_Track_Quality__lt_eMT_95_gt_ = 200417,
        DLRP__lt_eMT_19_gt_    = 200418,
        Force_Order__lt_eMT_52_gt_ = 200419,
        Wilco__slsh__Cantco__lt_eMT_53_gt_ = 200420,
        EMC_Bearing__lt_eMT_54_gt_ = 200421,
        Change_Track_Eligibility__lt_eMT_55_gt_    = 200422,
        Land_Mass_Reference_Point  = 200423,
        System_Reference_Point = 200424,
        PU_Amplification   = 200425,
        Set_slsh_Drift = 200426,
        Begin_Initialization__lt_MT_1_gt_  = 200427,
        Status_and_Control__lt_MT_3_gt_    = 200428,
        Scintillation_Change__lt_MT_39_gt_ = 200429,
        Link_11_ID_Control__lt_MT_61_gt_   = 200430,
        PU_Guard_List  = 200431,
        Winds_Aloft__lt_MT_14_gt_  = 200432,
        Surface_Winds__lt_MT_15_gt_    = 200433,
        Sea_State__lt_MT_17_gt_    = 200434,
        Magnetic_Variation__lt_MT_37_gt_   = 200435,
        Track_Eligibility__lt_MT_29_gt_    = 200436,
        Training_Track_Notification    = 200437,
        Tacan_Data__lt_MT_32_gt_   = 200501,
        Interceptor_Amplification__lt_MT_75_gt_    = 200502,
        Tacan_Assignment__lt_MT_76_gt_ = 200503,
        Autopilot_Status__lt_MT_77_gt_ = 200504,
        Consumables__lt_MT_78_gt_  = 200505,
        Downlink__lt_MT_79_gt_ = 200506,
        TIN_Report__lt_MT_80_gt_   = 200507,
        Special_Point_Control__lt_MT_81_gt_    = 200508,
        Control_Discretes__lt_MT_82_gt_    = 200509,
        Request_Target_Discretes_lt_MT_83_gt_  = 200510,
        Target_Discretes__lt_MT_84_gt_ = 200511,
        Reply_Discretes__lt_MT_85_gt_  = 200512,
        Command_Maneuvers__lt_MT_86_gt_    = 200513,
        Target_Data__lt_MT_87_gt_  = 200514,
        Target_Pointer__lt_MT_88_gt_   = 200515,
        Intercept_Data__lt_MT_89_gt_   = 200516,
        Decrement_Missile_Inventory__lt_MT_90_gt_  = 200517,
        Link_4A_Alert__lt_MT_91_gt_    = 200518,
        Strike_Control__lt_MT_92_gt_   = 200519,
        Speed_Change__lt_MT_25_gt_ = 200521,
        Course_Change__lt_MT_26_gt_    = 200522,
        Altitude_Change__lt_MT_27_gt_  = 200523,
        ACLS_AN_slsh_SPN_46_Status = 200524,
        ACLS_Aircraft_Report   = 200525,
        SPS_67_Radar_Operator_Functions    = 200600,
        SPS_55_Radar_Operator_Functions    = 200601,
        SPQ_9A_Radar_Operator_Functions    = 200602,
        SPS_49_Radar_Operator_Functions    = 200603,
        MK_23_Radar_Operator_Functions = 200604,
        SPS_48_Radar_Operator_Functions    = 200605,
        SPS_40_Radar_Operator_Functions    = 200606,
        MK_95_Radar_Operator_Functions = 200607,
        Other  = 240000,
        Mass_Of_The_Vehicle    = 240001,
        Force_ID_2 = 240002,
        Entity_Type_Kind   = 240003,
        Entity_Type_Domain = 240004,
        Entity_Type_Country    = 240005,
        Entity_Type_Category   = 240006,
        Entity_Type_Sub_Category   = 240007,
        Entity_Type_Specific   = 240008,
        Entity_Type_Extra  = 240009,
        Alternative_Entity_Type_Kind   = 240010,
        Alternative_Entity_Type_Domain = 240011,
        Alternative_Entity_Type_Country    = 240012,
        Alternative_Entity_Type_Category   = 240013,
        Alternative_Entity_Type_Sub_Category   = 240014,
        Alternative_Entity_Type_Specific   = 240015,
        Alternative_Entity_Type_Extra  = 240016,
        Entity_Location_X  = 240017,
        Entity_Location_Y  = 240018,
        Entity_Location_Z  = 240019,
        Entity_Linear_Velocity_X   = 240020,
        Entity_Linear_Velocity_Y   = 240021,
        Entity_Linear_Velocity_Z   = 240022,
        Entity_Orientation_Psi = 240023,
        Entity_Orientation_Theta   = 240024,
        Entity_Orientation_Phi = 240025,
        Dead_Reckoning_Algorithm   = 240026,
        Dead_Reckoning_Linear_Acceleration_X   = 240027,
        Dead_Reckoning_Linear_Acceleration_Y   = 240028,
        Dead_Reckoning_Linear_Acceleration_Z   = 240029,
        Dead_Reckoning_Angular_Velocity_X  = 240030,
        Dead_Reckoning_Angular_Velocity_Y  = 240031,
        Dead_Reckoning_Angular_Velocity_Z  = 240032,
        Entity_Appearance  = 240033,
        Entity_Marking_Character_Set   = 240034,
        Entity_Marking_11_Bytes    = 240035,
        Capability = 240036,
        Number_Articulation_Parameters = 240037,
        Articulation_Parameter_ID  = 240038,
        Articulation_Parameter_Type_2  = 240039,
        Articulation_Parameter_Value_2 = 240040,
        Type_Of_Stores = 240041,
        Quantity_Of_Stores = 240042,
        Fuel_Quantity  = 240043,
        Radar_System_Status    = 240044,
        Radio_Communication_System_Status  = 240045,
        Default_Time_For_Radio_Transmission_For_Stationary_Transmitters    = 240046,
        Default_Time_For_Radio_Transmission_For_Moving_Transmitters    = 240047,
        Body_Part_Damaged_Ratio    = 240048,
        Name_Of_The_Terrain_Database_File  = 240049,
        Name_Of_Local_File = 240050,
        Aimpoint_Bearing   = 240051,
        Aimpoint_Elevation = 240052,
        Aimpoint_Range = 240053,
        Air_Speed  = 240054,
        Altitude_2 = 240055,
        Application_Status = 240056,
        Auto_Iff   = 240057,
        Beacon_Delay   = 240058,
        Bingo_Fuel_Setting = 240059,
        Cloud_Bottom   = 240060,
        Cloud_Top  = 240061,
        Direction  = 240062,
        End_Action = 240063,
        Frequency  = 240064,
        Freeze = 240065,
        Heading    = 240066,
        Identification = 240067,
        Initial_Point_Data = 240068,
        Latitude_2 = 240069,
        Lights_2   = 240070,
        Linear = 240071,
        Longitude_2    = 240072,
        Low_Altitude   = 240073,
        Mfd_Formats    = 240074,
        Nctr   = 240075,
        Number_Projectiles = 240076,
        Operation_Code = 240077,
        Pitch  = 240078,
        Profiles   = 240079,
        Quantity   = 240080,
        Radar_Modes    = 240081,
        Radar_Search_Volume    = 240082,
        Roll   = 240083,
        Rotation   = 240084,
        Scale_Factor_X = 240085,
        Scale_Factor_Y = 240086,
        Shields    = 240087,
        Steerpoint = 240088,
        Spare1 = 240089,
        Spare2 = 240090,
        Team   = 240091,
        Text   = 240092,
        Time_Of_Day    = 240093,
        Trail_Flag = 240094,
        Trail_Size = 240095,
        Type_Of_Projectile = 240096,
        Type_Of_Target = 240097,
        Type_Of_Threat = 240098,
        Uhf_Frequency  = 240099,
        Utm_Altitude   = 240100,
        Utm_Latitude   = 240101,
        Utm_Longitude  = 240102,
        Vhf_Frequency  = 240103,
        Visibility_Range   = 240104,
        Void_Aaa_Hit   = 240105,
        Void_Collision = 240106,
        Void_Earth_Hit = 240107,
        Void_Friendly  = 240108,
        Void_Gun_Hit   = 240109,
        Void_Rocket_Hit    = 240110,
        Void_Sam_Hit   = 240111,
        Weapon_Data    = 240112,
        Weapon_Type    = 240113,
        Weather_2  = 240114,
        Wind_Direction = 240115,
        Wind_Speed = 240116,
        Wing_Station   = 240117,
        Yaw    = 240118,
        Memory_Offset  = 240119,
        Memory_Data    = 240120,
        VASI   = 240121,
        Beacon = 240122,
        Strobe = 240123,
        Culture    = 240124,
        Approach   = 240125,
        Runway_End = 240126,
        Obstruction    = 240127,
        Runway_Edge    = 240128,
        Ramp_Taxiway   = 240129,
        Laser_Bomb_Code    = 240130,
        Rack_Type  = 240131,
        HUD    = 240132,
        RoleFileName   = 240133,
        PilotName  = 240134,
        PilotDesignation   = 240135,
        Model_Type = 240136,
        DIS_Type   = 240137,
        Class  = 240138,
        Channel    = 240139,
        Entity_Type_2  = 240140,
        Alternative_Entity_Type_2  = 240141,
        Entity_Location    = 240142,
        Entity_Linear_Velocity = 240143,
        Entity_Orientation = 240144,
        Dead_Reckoning = 240145,
        Failure_Symptom    = 240146,
        Max_Fuel   = 240147,
        Refueling_Boom_Connect = 240148,
        Altitude_AGL   = 240149,
        Calibrated_Airspeed    = 240150,
        TACAN_Channel  = 240151,
        TACAN_Band = 240152,
        TACAN_Mode = 240153
    };


    VariableDatum( DatumIdentifierEnum32 datum = Unknown ) : datumID( static_cast<int32_t>( datum ) ) {}
    VariableDatum( const std::string& strVal, DatumIdentifierEnum32 datum = Other )
        : datumID( static_cast<int32_t>( datum ) )
    {
        value.resize( strVal.length() % sizeof( int64_t ) == 0 ? strVal.length() : ( strVal.length() / sizeof( int64_t ) + 1 ) * sizeof( int64_t ), 0 );
        std::copy( strVal.begin(), strVal.end(), value.begin() );
    }
    std::string ToString() const
    {
        return std::string( value.begin(), value.end() );
    }

    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        uint32_t lengthInBits = static_cast< uint32_t >( 8 * value.size() );
        /*
        uint32_t datumSize = value.size();
        uint32_t padding = 0;
        archive << datumID
                << lengthInBits
                << datumSize
                << padding ;
        */
        archive << lengthInBits; // FIXME check GRIM
        for( std::vector< char >::const_iterator it=value.begin(); value.end()!=it; ++it )
            archive << *it;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        uint32_t lengthInBits = 0;
        /*
        uint32_t datumSize = 0;
        uint32_t padding = 0;
        archive >> datumID
                >> lengthInBits
                >> datumSize
                >> padding;
                */
        archive >> lengthInBits; // FIXME check GRIM
        lengthInBits /= 8 ; // length in bytes
        value.resize( lengthInBits % sizeof( int64_t ) == 0 ? lengthInBits : ( lengthInBits / sizeof( int64_t ) + 1 ) * sizeof( int64_t ), 0 );
        for( std::vector< char >::iterator it=value.begin(); lengthInBits>0; ++it, --lengthInBits )
            archive >> *it;
    }
    int32_t datumID;
    std::vector< char > value; // invariant : value.size()%sizeof(int64_t) == 0
};

struct VariableDatumSet
{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        uint32_t nbDatums = static_cast< uint32_t >( variableDatums.size() );
        uint32_t padding = 0;
        archive << nbDatums
                << padding;
        for( std::vector< VariableDatum >::const_iterator it=variableDatums.begin(); variableDatums.end()!=it; ++it )
            archive << *it;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        uint32_t nbDatums = 0;
        uint32_t padding = 0;
        archive >> nbDatums
                >> padding;
        variableDatums.resize( nbDatums );
        for( std::vector< VariableDatum >::iterator it=variableDatums.begin(); variableDatums.end()!=it; ++it )
            archive >> *it;
    }
    std::vector< VariableDatum > variableDatums;
};

struct Comment
{
    rpr::EntityIdentifier originatingEntity;
    rpr::EntityIdentifier receivingEntity;
    VariableDatumSet variableDatumSet;
};

}
}
}

#endif // plugins_hla_interactions_InteractionsRpr_h
