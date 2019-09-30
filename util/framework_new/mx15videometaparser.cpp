#include <framework.hpp>
#include "mx15videometaparser.hpp"

UISOFT_BEGIN_NAMESPACE(framework)

//INFO
// this class can be used to extract the LS formated metadata from a TS stream
// basically, to get the positional parameters of a drone from a video file
// which was streamed from the drone
// the interface is very simple. you ask for the data at a time in the file, and it gives you the data
// not all LS fields are implemented, only the ones we are using for this project. it is simple to expand later.

//macros to extract big-endian numbers from a vector of bytes
#define mx15_get_int32(p) (int32_t)((p)[0]*16777216 + (p)[1]*65536 + (p)[2]*256 + (p)[3])
#define mx15_get_int16(p) (int16_t)((p)[0]*256 + (p)[1])
#define mx15_get_int8(p) (int8_t)((p)[0])
#define mx15_get_uint64(p) (p)[0]*72057594037927936 + (p)[1]*281474976710656 + (p)[2]*1099511627776 + (p)[3]*4294967296 + (p)[4]*16777216 + (p)[5]*65536 + (p)[6]*256 + (p)[7]
#define mx15_get_uint32(p) (p)[0]*16777216 + (p)[1]*65536 + (p)[2]*256 + (p)[3]
#define mx15_get_uint16(p) (p)[0]*256 + (p)[1]
#define mx15_get_uint8(p) (p)[0]

//constructor
//loads up a video file, and checks how long the file is, and what time range the data covers.
//NOTE: This class will hold a handle to the file as long as it exists
//the class is constructed using a file as input, that instance of the class is bound to that specific video file
//to open a different file, construct a new object
mx15videometaparser::mx15videometaparser()
{
	static const int arr[] = { 6, 14, 43, 52, 2, 11, 1, 1, 14, 1, 3, 1, 1, 0, 0, 0 };
	my_UID = std::vector<uint8_t>(arr, arr + sizeof(arr) / sizeof(arr[0]));

	my_initialized = false;
}

bool mx15videometaparser::initialize(string_type filename, string_type& xo_message, int PID_to_parse)
{
	//open the file as a stream of bytes
	my_inf.open(filename, std::ios_base::in | std::ios_base::binary);
	if (!my_inf.good())
	{
		xo_message = _S("file read error");
		return false;
	}

	my_PID = 0;
	if (PID_to_parse)
	{
		my_PID = PID_to_parse;
	} 
	else
	{
		my_PID = extract_metadata_PID();
	}

	if (my_PID == 0)
	{
		xo_message = _S("cannot find any metadata");
		return false;
	}

	seek_pos_aligned(0);
	//locate the timestamps and rough frame positions and save them
	my_first_frame = parse_metadata(get_next_LS_frame());
	my_start_pos = my_inf.tellg();
	seek_end_aligned();
	my_last_frame = parse_metadata(get_next_LS_frame(true));
	my_end_pos = my_inf.tellg();
	my_stream_length_us = my_last_frame.platform.time - my_first_frame.platform.time;

	//printf("stream start time: %s", ctime(&first_frame.platform.time_UNIX));

	my_initialized = true;

	return true;
}

bool mx15videometaparser::is_initialized()
{
	return my_initialized;
}

time_t mx15videometaparser::get_start_time()
{
	if (my_initialized)
	{
		return my_first_frame.platform.time_UNIX;//check the unit (unit is miliseconds since 1970 - tim)
	}
	else
	{
		return time_t(0);
	}
}

mx15videometaparser::~mx15videometaparser()
{
	my_inf.close();
}

//this function crunches an LS packet and outputs the metadata.
//TODO: check the checksum
mx15Metadata mx15videometaparser::parse_metadata(std::vector<uint8_t> ls_data)
{
	mx15Metadata M;

	M.platform.heading = NAN;
	M.platform.pitch = NAN;
	M.platform.roll = NAN;
	M.platform.designation = "";
	M.image.source_sensor = "";
	M.image.coord_system = "";
	M.sensor.latitude = NAN;
	M.sensor.longitude = NAN;
	M.sensor.altitude = NAN;
	M.sensor.horz_FOV = NAN;
	M.sensor.vert_FOV = NAN;
	M.sensor.rel_azimuth = NAN;
	M.sensor.rel_elevation = NAN;
	M.sensor.rel_roll = NAN;
	M.target.slant_range = NAN;
	M.target.width = NAN;
	M.frame_center.latitude = NAN;
	M.frame_center.longitude = NAN;
	M.frame_center.elevation = NAN;
	M.offset_corner_1.latitude = NAN;
	M.offset_corner_1.longitude = NAN;
	M.offset_corner_1.elevation = NAN;
	M.offset_corner_2.latitude = NAN;
	M.offset_corner_2.longitude = NAN;
	M.offset_corner_2.elevation = NAN;
	M.offset_corner_3.latitude = NAN;
	M.offset_corner_3.longitude = NAN;
	M.offset_corner_3.elevation = NAN;
	M.offset_corner_4.latitude = NAN;
	M.offset_corner_4.longitude = NAN;
	M.offset_corner_4.elevation = NAN;
	M.target.location.latitude = NAN;
	M.target.location.longitude = NAN;
	M.target.location.elevation = NAN;
	M.target.track_gate_width = NAN;
	M.target.track_gate_height = NAN;
	M.platform.ground_speed = NAN;
	M.target.ground_range = NAN;
	M.platform.LS_version_number = NAN;

	if (ls_data.size() < 3)
	{
		return M;//check, should not happen
	}

	int i;
	int l = 0;
	
	uint8_t* p = ls_data.data();
	
	while (l < ls_data.size())
	{
		int id = ls_data.at(l++);
		int length = ls_data.at(l++);
		p += 2;
		l += length;

		switch (id)
		{
		case 5: // uint16
			M.platform.heading = mx15_get_uint16(p);
			M.platform.heading = map_range(M.platform.heading, 0, 0xffff, 0, 360 );
			break;
		case 6: // int16
			M.platform.pitch = mx15_get_int16(p);
			M.platform.pitch = M.platform.pitch == 0x8000 ? NAN : map_range(M.platform.pitch, -0x7fff, 0x7fff, -20, +20);
			break;
		case 7: // int16
			M.platform.roll = mx15_get_int16(p);
			M.platform.roll = M.platform.roll == 0x8000 ? NAN : map_range(M.platform.roll, -0x7fff, 0x7fff, -50, +50);
			break;
		case 10: // string		 
			for (i = 0; i < length; i++)
			{
				M.platform.designation += p[i];
			}
			break;	
		case 11: // string
			for (i = 0; i < length; i++)
			{
				M.image.source_sensor += p[i];
			}
			break;
		case 12: // string
			for (i = 0; i < length; i++)
			{
				M.image.coord_system += p[i];
			}
			break;
		case 13: // int32
			M.sensor.latitude = mx15_get_int32(p);
			M.sensor.latitude = M.sensor.latitude == (-0x7FFFFFFF - 1) ? NAN : map_range(M.sensor.latitude, -0x7fffffff, 0x7fffffff, -90, +90);
			break;
		case 14: // int32
			M.sensor.longitude = mx15_get_int32(p);
			M.sensor.longitude = M.sensor.longitude == (-0x7FFFFFFF - 1) ? NAN : map_range(M.sensor.longitude, -0x7fffffff, 0x7fffffff, -180, +180);
			break;
		case 15: // uint16
			M.sensor.altitude = mx15_get_uint16(p);
			M.sensor.altitude = map_range(M.sensor.altitude, 0, 0xffff, -900, 19000 );
			break;
		case 16: // uint16
			M.sensor.horz_FOV = mx15_get_uint16(p);
			M.sensor.horz_FOV = map_range(M.sensor.horz_FOV, 0, 0xffff, 0, +180 );
			break;
		case 17: // uint16
			M.sensor.vert_FOV = mx15_get_uint16(p);
			M.sensor.vert_FOV = map_range(M.sensor.vert_FOV, 0, 0xffff, 0, +180 );
			break;
		case 18: // uint32
			M.sensor.rel_azimuth = mx15_get_uint32(p);
			M.sensor.rel_azimuth = map_range(M.sensor.rel_azimuth, 0, 0xffffffff, 0, +360);
			break;
		case 19: // int32
			M.sensor.rel_elevation = mx15_get_int32(p);
			M.sensor.rel_elevation = M.sensor.rel_elevation == (-0x7FFFFFFF - 1) ? NAN : map_range(M.sensor.rel_elevation, -0x7fffffff, 0x7fffffff, -180, +180);
			break;
		case 20: // uint32
			M.sensor.rel_roll = mx15_get_int32(p);
			M.sensor.rel_roll = map_range(M.sensor.rel_roll, 0, 0xffffffff, 0, +360 );
			break;
		case 21: // uint32
			M.target.slant_range = mx15_get_uint32(p);
			M.target.slant_range = map_range(M.target.slant_range, 0, 0xffffffff, 0, +5000000 );
			break;
		case 22: // uint16
			M.target.width = mx15_get_uint16(p);
			M.target.width = map_range(M.target.width, 0, 0xffff, 0, +10000 );
			break;
		case 23: // int32
			M.frame_center.latitude = mx15_get_int32(p);
			M.frame_center.latitude = M.frame_center.latitude == (-0x7FFFFFFF - 1) ? NAN : map_range(M.frame_center.latitude, -0x7fffffff, 0x7fffffff, -90, +90);
			break;
		case 24: // int32
			M.frame_center.longitude = mx15_get_int32(p);
			M.frame_center.longitude = M.frame_center.longitude == (-0x7FFFFFFF - 1) ? NAN : map_range(M.frame_center.longitude, -0x7fffffff, 0x7fffffff, -180, +180);
			break;
		case 25: // uint16
			M.frame_center.elevation = mx15_get_uint16(p);
			M.frame_center.elevation = map_range(M.frame_center.elevation, 0, 0xffff, -900, +19000 );
			break;		
		case 26: // int16
			M.offset_corner_1.latitude = mx15_get_int16(p);
			M.offset_corner_1.latitude = M.offset_corner_1.latitude == -0x8000 ? NAN : map_range(M.offset_corner_1.latitude, -0x7fff, 0x7fff, -0.075, +0.075);
			break;
		case 27: // int16
			M.offset_corner_1.longitude = mx15_get_int16(p);
			M.offset_corner_1.longitude = M.offset_corner_1.longitude == -0x8000 ? NAN : map_range(M.offset_corner_1.longitude, -0x7fff, 0x7fff, -0.075, +0.075);
			break;
		case 28: // int16
			M.offset_corner_2.latitude = mx15_get_int16(p);
			M.offset_corner_2.latitude = M.offset_corner_2.latitude == -0x8000 ? NAN : map_range(M.offset_corner_2.latitude, -0x7fff, 0x7fff, -0.075, +0.075);
			break;
		case 29: // int16
			M.offset_corner_2.longitude = mx15_get_int16(p);
			M.offset_corner_2.longitude = M.offset_corner_2.longitude == -0x8000 ? NAN : map_range(M.offset_corner_2.longitude, -0x7fff, 0x7fff, -0.075, +0.075);
			break;
		case 30: // int16
			M.offset_corner_3.latitude = mx15_get_int16(p);
			M.offset_corner_3.latitude = M.offset_corner_3.latitude == -0x8000 ? NAN : map_range(M.offset_corner_3.latitude, -0x7fff, 0x7fff, -0.075, +0.075);
			break;
		case 31: // int16
			M.offset_corner_3.longitude = mx15_get_int16(p);
			M.offset_corner_3.longitude = M.offset_corner_3.longitude == -0x8000 ? NAN : map_range(M.offset_corner_3.longitude, -0x7fff, 0x7fff, -0.075, +0.075);
			break;
		case 32: // int16
			M.offset_corner_4.latitude = mx15_get_int16(p);
			M.offset_corner_4.latitude = M.offset_corner_4.latitude == -0x8000 ? NAN : map_range(M.offset_corner_4.latitude, -0x7fff, 0x7fff, -0.075, +0.075);
			break;
		case 33: // int16
			M.offset_corner_4.longitude = mx15_get_int16(p);
			M.offset_corner_4.longitude = M.offset_corner_4.longitude == -0x8000 ? NAN : map_range(M.offset_corner_4.longitude, -0x7fff, 0x7fff, -0.075, +0.075);
			break;
		case 40: // int32
			M.target.location.latitude = mx15_get_int32(p);
			M.target.location.latitude = M.target.location.latitude == (-0x7FFFFFFF - 1) ? NAN : map_range(M.target.location.latitude, -0x7fffffff, 0x7fffffff, -90, +90);
			break;
		case 41: // int32
			M.target.location.longitude = mx15_get_int32(p);
			M.target.location.longitude = M.target.location.longitude == (-0x7FFFFFFF - 1) ? NAN : map_range(M.target.location.longitude, -0x7fffffff, 0x7fffffff, -180, +180);
			break;
		case 42: // uint16
			M.target.location.elevation = mx15_get_uint16(p);
			M.target.location.elevation = map_range(M.target.location.elevation, 0, 0xffff, -900, +19000 );
			break;
		case 43: // uint8
			M.target.track_gate_width = mx15_get_uint8(p);
			break;
		case 44: // uint8
			M.target.track_gate_height = mx15_get_uint8(p);
			break;
		case 56: // uint8
			M.platform.ground_speed = mx15_get_uint8(p);
			break;
		case 57: // uint32
			M.target.ground_range = mx15_get_uint32(p);
			M.target.ground_range = map_range(M.target.ground_range, 0, 0xffffffff, 0, +5000000 );
			break;
		case 65: // uint8
			M.platform.LS_version_number = mx15_get_uint8(p);
			break;
		case 1: // checksum
			//TODO: the checksum is the bottom 2 bytes of the sum of all the prev bytes including checksum size
			break;
		case 2: // time
			M.platform.time = static_cast<double>(mx15_get_uint64(p));
			M.platform.time_UNIX = (int)(M.platform.time/1000000);
			//printf("time: %s", ctime(&M.platform.time_UNIX));
			break;
		default:
			mx15_debug_printf("unknown tag in metadata! tag #: %d\n", id);
		}
		p += length;

	}

	return M;

}

//returns the next valid LS frame from the current file position
//it does its best to assemble all the packets into a valid frame and returns only the LS data after the UID
std::vector<uint8_t> mx15videometaparser::get_next_LS_frame(bool backwards, bool nontrivial)
{
	std::vector<uint8_t> ls(1000);
	ls.resize(0);
	bool done = false;

	std::streampos begin;
	bool PUSI;
	bool TEI;
	uint8_t AC, CC, c_counter=99;
	int starting_point = 0;
	int expected_size = 1;

	if (backwards)
	{
		 //find the first packet in a sequence by looking backwards
		std::vector<uint8_t> p = get_next_packet(my_PID, true, true);
		if (p.size() == 0) return ls; //thats our signal that there are no more packets

		my_inf.seekg(-188, std::ios_base::cur); //rewind to the beginning of the packet we just found and proceed
		begin = my_inf.tellg();
	}

	bool start_frame = true;
	//parse out only the relevant metadata packets into an array
	while (expected_size>0) //this will quit once get_next_packet starts returning nothing or we finish
	{
		std::vector<uint8_t> p = get_next_packet(my_PID, false, start_frame);
		start_frame = false;

		if (p.size() == 0) break; //thats our signal that there are no more packets

		//p is guranteed to be a valid packet of the type we wanted now, 
		//but the data must be assembled from multiple packets..
		PUSI = (p.at(1) & 0x40) > 0;
		TEI = (p.at(1) & 0x80) > 0;
		AC = (p.at(3) & 0x30) >> 4;
		CC = (p.at(3) & 0x0f);

		if (TEI) continue;
		if (PUSI) //if this is the start of a frame and a valid packet
		{
			//TODO: maybe align to the end of the UID instead of hardcoding the alignment to 66
			int UID_position = find_UID(p);
			if (UID_position > 0)  //if it contains the UID (just double checing to make sure we are in the right place)
			{
				UID_position += 16;
				c_counter = CC;
				if (p.at(UID_position) == 130) //2 bytes
				{
					starting_point = UID_position + 3;
					expected_size = mx15_get_uint16(&p.data()[UID_position + 1]);
				}
				if (p.at(UID_position) == 129) //1 byte (long)
				{
					starting_point = UID_position + 2;
					expected_size = p.at(UID_position + 1);
				}
				if (p.at(UID_position) < 128) //1 byte (short)
				{
					starting_point = UID_position + 1;
					expected_size = p.at(UID_position);
				}

				if (nontrivial && (expected_size == 0x0e))
				{
					expected_size = 1;
					starting_point = 0;
					ls.resize(0);
					continue;
				}

				if (expected_size <= (188 - starting_point))
				{
					ls.resize(ls.size() + expected_size);
					memcpy(ls.data(), p.data() + starting_point, expected_size);		
					expected_size = 0;
				}
				else {
					expected_size = expected_size - (188 - starting_point);
					ls.resize(ls.size() + 188 - starting_point);
					memcpy(ls.data(), p.data() + starting_point, 188 - starting_point);
				}
			}
			else {
				printf("packet is maybe broken, or I am out of sync");
			}
		}
		else {
			if (((c_counter + 1) % 16) == CC) //if the continuity counter lines up
			{
				c_counter = CC;
				if (AC > 1) //if there is an adaptation field
				{
					starting_point = p.at(4) + 5;
				} 
				else {
					starting_point = 4;
				}

				if (expected_size <= (188 - starting_point))
				{
					ls.resize(ls.size() + expected_size);
					memcpy(ls.data() + ls.size() - expected_size, p.data() + starting_point, expected_size);			
					expected_size = 0;
				} 
				else {
					ls.resize(ls.size() + 188 - starting_point);
					memcpy(ls.data() + ls.size() + 188 - starting_point, p.data() + starting_point, expected_size);
					expected_size = expected_size - (188 - starting_point);
				}
			}
			else
			{//continuity counter did not line up...
				ls.resize(0);
				starting_point = 0;
				expected_size = 1;
			}
		}		
	}
	if (backwards)	my_inf.seekg(begin);
	//go back to the start so the next backwards scan wont find the packet we just moved through
	
	if (expected_size != 0)
	{
		ls.resize(0);
	}
	return ls;
}

//gets the next packet from the stream. assuming all packets are exactly 188b
std::vector<uint8_t> mx15videometaparser::get_next_packet(uint16_t type, bool backwards, bool require_PUSI, int stop_after)
{
	std::vector<uint8_t> mDataBuffer;
	std::streampos fpos;

	if (!my_inf.good())
	{
		printf("file error\n");
		return mDataBuffer;
	}
	mDataBuffer.resize(188);
	bool PUSI;

	do //ID loop 
	{
		fpos = my_inf.tellg();
		if ((fpos > stop_after) && (stop_after > 0))
		{
			mDataBuffer.resize(0);
			break;
		}
		if (backwards)
		{
			if (fpos == (std::streampos)0)
			{
				mDataBuffer.resize(0);
				break;
			}
			my_inf.seekg(-188, std::ios_base::cur);
		}
		my_inf.read((char *)&mDataBuffer[0], 188);

		if (mDataBuffer[0] != 'G')
		{
			while (!my_inf.eof())
			{
				//if we didnt find the sync byte, then do a scan forwards for another G
				if (backwards) my_inf.seekg(-2, std::ios_base::cur);
				my_inf.read((char *)&mDataBuffer[0], 1);
				if (mDataBuffer[0] == 'G') //if found, check to see if there are Gs all around
				{
					std::streampos s = my_inf.tellg();
					my_inf.seekg(-189, std::ios_base::cur);
					my_inf.read((char *)&mDataBuffer[1], 1);
					my_inf.seekg(+375, std::ios_base::cur);
					my_inf.read((char *)&mDataBuffer[2], 1);
					my_inf.seekg(s, std::ios_base::beg);

					//if so, assume we are in sync, and read the remainder of the packet
					if ((mDataBuffer[1] == 'G') && (mDataBuffer[2] == 'G'))
					{
						my_inf.read((char *)&mDataBuffer[1], 187);
						//return mDataBuffer;
						break;
					}
				}
			}
		}
		if (backwards)
		{
			my_inf.seekg(-188, std::ios_base::cur);
		}

		PUSI = (mDataBuffer.at(1) & 0x40) > 0;
		if (((mx15_get_uint16(&mDataBuffer[1]) & 0x1fff) == type) && (require_PUSI ? PUSI : true))
		{
			return mDataBuffer;
		}

	} while ( !my_inf.eof() ); 

	return mDataBuffer;
}

//given a pointer to the beginning of a packet, determines if the packet contains the UID
int mx15videometaparser::find_UID(std::vector<uint8_t> &packet)
{

	auto it = std::search(packet.begin(), packet.end(), my_UID.begin(), my_UID.end());
	if (it < packet.end()) return (int)(it - packet.begin());
	return 0;
}

//seek the end of the file, aligned to 188b. returns the new position
std::streampos mx15videometaparser::seek_end_aligned(void)
{
	my_inf.seekg(0, std::ios_base::end);
	std::streampos p = my_inf.tellg();

	my_inf.seekg(  (p / 188)*188 , std::ios_base::beg );
	p = my_inf.tellg();
	return p;

}

//seek any position in the file, aligned to 188b. returns the new position
std::streampos mx15videometaparser::seek_pos_aligned(std::streampos pos)
{
	my_inf.seekg((pos / 188) * 188, std::ios_base::beg);
	return my_inf.tellg();
}

//get the length of the recorded data in ms
double mx15videometaparser::get_length_ms(void)
{
	return my_stream_length_us / 1000;
}

//set the file position to a specific time in the file (same as the seek bar on any video player)
bool mx15videometaparser::seek_elapsed_time_ms(double time)
{
	return seek_timestamp((time * 1000) + my_first_frame.platform.time);
}

//set the file position to a specific unix time within the file
//returns true if sucessful, false if there was no change
//the position given will be slightly before the requested time if it was not exact.
bool mx15videometaparser::seek_timestamp(double timestamp)
{
	if ((timestamp < my_first_frame.platform.time) || (timestamp > my_last_frame.platform.time)) return false;

	//map this time onto a file position
	double file_position = map_range(timestamp,
		my_first_frame.platform.time, my_last_frame.platform.time,
		(double)my_start_pos, (double)my_end_pos);

	seek_pos_aligned((int)file_position);

	//mx15Metadata m = parse_metadata(get_next_LS_frame());
	double time = get_timestamp_fast_n_dirty(timestamp);
	mx15Metadata m = parse_metadata(get_next_LS_frame());
	//double time = m.platform.time;

	if (time < timestamp)
	{//if the time on the last frame parsed is LESS than expected, we shall scan forwards
		while (m.platform.time < timestamp)
		{
			m = parse_metadata(get_next_LS_frame());
		}
		get_next_LS_frame(true);
		get_next_LS_frame(true);//go back 2 frames so the next read will produce the frame right before the timestamp
	}
	else //otherwise, we shall scan backwards
	{	
		while (m.platform.time > timestamp)
		{
			m = parse_metadata(get_next_LS_frame(true));
		}
		get_next_LS_frame(true);//go back 1 more frame so the next read will produce the frame right before the timestamp
	}
	return true;

}

double mx15videometaparser::get_timestamp_fast_n_dirty(double target_time)
{
	std::vector<uint8_t> p(188);
	double time = 0;
	int UID_position;
	
	p = get_next_packet(my_PID, false, true);
	UID_position = find_UID(p) + 16;
	if (p.at(UID_position) > 127)
	{
		UID_position += p.at(UID_position) - 128;
	}
	UID_position+=3;
	time = static_cast<double>(mx15_get_uint64(&p.data()[UID_position]));

	if (time < target_time)
	{
		do
		{
			p = get_next_packet(my_PID, false, true);
			UID_position = find_UID(p) + 16;
			if (p.at(UID_position) > 127)
			{
				UID_position += p.at(UID_position) - 128;
			}
			UID_position+=3;
			time = static_cast<double>(mx15_get_uint64(&p.data()[UID_position]));
		} while ((p.size() > 0) && (time < target_time));
	}
	else
	{
		do
		{
			p = get_next_packet(my_PID, true, true);
			UID_position = find_UID(p) + 16;
			if (p.at(UID_position) > 127)
			{
				UID_position += p.at(UID_position) - 128;
			}
			UID_position+=3;

			time = static_cast<double>(mx15_get_uint64(&p.data()[UID_position]));
		} while ((p.size() > 0) && (time > target_time));
	}
	return time;
}

//map a value from the input range onto the output range
double mx15videometaparser::map_range(double value, double in_low, double in_hi, double out_low, double out_high)
{
	double fraction = (value - in_low) / (in_hi - in_low);
	return out_low + ((out_high - out_low)*fraction);

}

//get the metadata frame from a number of miliseconds after the start of the recording
mx15Metadata mx15videometaparser::get_metadata_after_x_ms(double time, bool nontrivial)
{
	seek_elapsed_time_ms(time);
	return parse_metadata(get_next_LS_frame(false, nontrivial));
}

//get the metadata from a specific UTC time (UTC miliseconds)
mx15Metadata mx15videometaparser::get_metadata_at_UTC(double time, bool nontrivial)
{
	seek_timestamp(time*1000);
	return parse_metadata(get_next_LS_frame(false, nontrivial));
}

//get the next metadata frame and advance the file postion forward
mx15Metadata mx15videometaparser::get_next_metadata()
{
	return parse_metadata(get_next_LS_frame());
}

//get the previous metadata frame and advance the file position backwards
mx15Metadata mx15videometaparser::get_prev_metadata()
{
	return parse_metadata(get_next_LS_frame(true));
}

void mx15videometaparser::set_metadata_PID(int PID)
{
	my_PID = PID;

}
int	mx15videometaparser::get_metadata_PID(void)
{
	return my_PID;
}

int	mx15videometaparser::extract_metadata_PID(void)
{
	int PID = 0;

	//go to beginning of file (hoping to find the descriptors right away)
	my_inf.seekg(0, std::ios_base::beg);

	//get the ZERO packet which has the main list of channels in it
	std::vector<uint8_t> pzero = get_packet_payload(get_next_packet(0, false, true, 1000000)); 
	if (pzero.size() == 0) return 0;

	pzero.resize(pzero.at(3) + 4);
	
	//unknown byte: 1 byte, header: 8 bytes. CRC: 4 bytes. Entry: 4 bytes
	int num_entries = ((int)pzero.size() - 1 - 8 - 4) / 4;

	//TODO: maybe parse out different channels and allow for the user to select from avail. channels
	int x;
	for (x = 0; x < num_entries; x++)
	{
		int chan_ID = pzero.at(9 + x * 4) * 256 + pzero.at(10 + x * 4);
		int chan_packet_ID = (pzero.at(11 + x * 4) * 256 + pzero.at(12 + x * 4)) & 0x1fff;

		mx15_debug_printf((std::string("found channel ") + std::to_string(chan_ID) + ", with PID " + std::to_string(chan_packet_ID) + " ").c_str());
		mx15_debug_printf("\n");
	}

	//right now we just lock ourselves onto the very first channel we see
	int chan_ID = pzero.at(9) * 256 + pzero.at(10);
	int chan_packet_ID = (pzero.at(11) * 256 + pzero.at(12)) & 0x1fff;

	//now we repeat this process to figure out which individual streams compose that channel
	std::vector<uint8_t> channel_map = get_packet_payload(get_next_packet(chan_packet_ID, false, true, 1000000));
	if (channel_map.size() == 0) return 0;

	int table_id = channel_map.at(1); //should always be 2
	int length = mx15_get_uint16(&(channel_map.data()[2])) & 0x0fff;

	while (length > (channel_map.size() + 3))
	{
		std::vector<uint8_t> payload = get_packet_payload(get_next_packet(chan_packet_ID));
		channel_map.insert(channel_map.end(), payload.begin(), payload.end());
	}

	channel_map.resize(length + 3);

	//ok so now we have the map in out buffer. the first 10 bytes are header. then there is a descriptor loop
	int program_number = mx15_get_uint16(&(channel_map.data()[4])) & 0x1fff;
	int descriptor_length = mx15_get_uint16(&(channel_map.data()[11])) & 0x0fff;

	uint8_t * p = channel_map.data() + 13 + descriptor_length;
	length -= (10 + descriptor_length);

	while (length > 4) //4 bytes remain for the CRC
	{
		int this_type = p[0];
		int this_pid = mx15_get_uint16(&p[1]) & 0x1fff;
		std::string id_str = "";
		int i;
		for (i = 0; i < p[6]; i++)
		{
			id_str += p[7 + i];
		}

		mx15_debug_printf((std::string("found a stream (type ") + std::to_string(this_type)+ ") named " + id_str).c_str()); 
		mx15_debug_printf((std::string(" with PID ") + std::to_string(this_pid) + " within channel " + std::to_string(program_number)).c_str());
		mx15_debug_printf("\n");

		// this part is very important. it is where we see which PID is for the metadata stream (0x15) 
		if (this_type == 0x15)
		{
			PID = this_pid; break; //break if we ACTUALLY find it, the other two conditios are weaker
		}
		//hacky: 0x15 is the correct PID for metadata, but sometimes the PID will be custom or private
		//in which case, I have found that the stream we want is usually named KLV or KLVA
		if (id_str.compare("KLVA") == 0) PID = this_pid;
		if (id_str.compare("KLV") == 0) PID = this_pid;

		length -= (5 + p[4]);
		p += (5 + p[4]);
	}

	return PID;
}

std::vector<uint8_t> mx15videometaparser::get_packet_payload(std::vector<uint8_t> packet)
{
	std::vector<uint8_t> payload(188);

	int AC = (packet.at(3) & 0x30) >> 4;
	int starting_point = 4;

	if (AC > 1)
	{
		starting_point = 5 + packet.at(4);		
	}
	int expected_size = 188 - starting_point;

	payload.resize(expected_size);
	memcpy(payload.data(), packet.data() + starting_point, expected_size);
	
	return payload;

}


UISOFT_END_NAMESPACE