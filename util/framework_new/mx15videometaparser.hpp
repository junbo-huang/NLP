#ifndef __MX15VIDEOMETAPARSER_H__
#define __MX15VIDEOMETAPARSER_H__

#include <framework.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <istream>

#include <algorithm>
#include <inttypes.h>
#include <ctime>

UISOFT_BEGIN_NAMESPACE(framework)

#define mx15_debug_printf printf

#define offset_corner_1 offset_corner_top_left
#define offset_corner_2 offset_corner_top_right
#define offset_corner_3 offset_corner_bottom_right
#define offset_corner_4 offset_corner_bottom_left

typedef struct mx15_metadata_t mx15Metadata;
typedef struct mx15_meta_platform_t mx15PlatformData;
typedef struct mx15_meta_image_t mx15ImageData;
typedef struct mx15_meta_sensor_t mx15SensorData;
typedef struct mx15_meta_target_t mx15TargetData;
typedef struct mx15_meta_coord_t mx15CoordData;

struct mx15_meta_coord_t
{
	double latitude;
	double longitude;
	double elevation;
};

struct mx15_meta_platform_t
{
	time_t time_UNIX;
	double time;
	double heading;
	double pitch;
	//double yaw;
	double roll;
	double ground_speed;
	std::string designation;
	double LS_version_number;
};

struct mx15_meta_sensor_t
{
	double latitude;
	double longitude;
	double altitude;
	double horz_FOV;
	double vert_FOV;
	double rel_azimuth;
	double rel_elevation;
	double rel_roll;
};

struct mx15_meta_target_t
{
	double slant_range;
	double width;
	struct mx15_meta_coord_t location;
	double track_gate_width;
	double track_gate_height;
	double ground_range;

};

struct mx15_meta_image_t
{
	std::string source_sensor;
	std::string coord_system;
};

struct mx15_metadata_t
{
	mx15PlatformData platform;
	mx15TargetData target;
	mx15SensorData sensor;
	mx15ImageData image;
	mx15CoordData offset_corner_1;
	mx15CoordData offset_corner_2;
	mx15CoordData offset_corner_3;
	mx15CoordData offset_corner_4;
	mx15CoordData frame_center;
};

class mx15videometaparser
{
private:
	string_type                        my_filename;
	std::basic_ifstream<char>          my_inf;

	//used to map an aribitrary time into a file position
	mx15Metadata                       my_first_frame;
	mx15Metadata                       my_last_frame;
	std::streampos                     my_start_pos;
	std::streampos                     my_end_pos;

	std::vector<uint8_t>               my_UID;

	double                             my_stream_length_us;

	bool                               my_initialized;
	int								   my_PID;

private:
	std::vector<uint8_t>	get_next_packet(uint16_t type, bool backwards = false, bool require_PUSI = false, int stop_after = 0);
	std::vector<uint8_t>	get_next_LS_frame(bool backwards = false, bool nontrivial = false);
	
	mx15Metadata			parse_metadata(std::vector<uint8_t> ls_data);
	double					get_timestamp_fast_n_dirty(double target_time);
	bool					seek_timestamp(double timestamp);
	std::streampos			seek_pos_aligned(std::streampos pos);
	std::streampos			seek_end_aligned(void);
	int						find_UID(std::vector<uint8_t> &packet);
	double					map_range(double value, double in_low, double in_hi, double out_low, double out_high);
	int						extract_metadata_PID(void);
	std::vector<uint8_t>	get_packet_payload(std::vector<uint8_t> packet);

public:
	mx15videometaparser();
	~mx15videometaparser();

	bool                    initialize(string_type filename, string_type& xo_message, int PID_to_parse = 0);
	bool                    is_initialized();
	time_t                  get_start_time();
	mx15Metadata			get_metadata_after_x_ms(double time, bool nontrivial = false);
	mx15Metadata			get_metadata_at_UTC(double time, bool nontrivial = false);
	double					get_length_ms(void);
	bool					seek_elapsed_time_ms(double time);
	mx15Metadata			get_next_metadata(void);
	mx15Metadata			get_prev_metadata(void);
	void					set_metadata_PID(int PID);
	int						get_metadata_PID(void);


};

UISOFT_END_NAMESPACE

#endif