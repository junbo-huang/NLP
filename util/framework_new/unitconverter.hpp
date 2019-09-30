#ifndef __UNITCONVERTER_H__
#define __UNITCONVERTER_H__

#pragma once

#include <converter.hpp>
#include <units.hpp>

#define KILOMETER2METER(x)                          ((x)*1000.0)
#define FOOT2METER(x)                               ((x)*0.3048)
#define KNOT2METER_PER_SECOND(x)                    ((x)*0.5144444444)
#define NAUTICAL_MILE2METER(x)                      ((x)*1852.0)
#define MILE2METER(x)                               ((x)*1609.34)
#define METER2NAUTICAL_MILE(x)                      ((x)/1852.0)
#define METER2KILOMETER(x)                          ((x)/1000.0)
#define METER2FOOT(x)                               ((x)*3.2808399)
#define METER2MILE(x)                               ((x)*0.000621371)

#define METER_PER_SECOND_2_KILOMETER_PER_HOUR(x)    ((x)*3.6)
#define METER_PER_SECOND_2_KNOTS(x)                 ((x)*1.94)
#define METER_PER_SECOND_2_FEET_PER_MIN(x)          ((x)*196.9)
#define KILOMETER_PER_HOUR_2_METER_PER_SECOND(x)    ((x)*0.278)
#define KNOTS_2_METER_PER_SECOND(x)                 ((x)*0.51)
#define FEET_PER_MIN_2_METER_PER_SECOND(x)          ((x)*0.00508)

#define DEGREE2RADIANS(x)                           ((x)*0.0174532925)
#define RADIANS2DEGREE(x)                           ((x)*57.2957795)

#define SECOND2HOUR(x)                              ((x)/3600)
#define SECOND2MINUTE(x)                            ((x)/60)
#define HOUR2SECOND(x)                              ((x)*3600)
#define HOUR2MINUTE(x)                              ((x)*60)
#define MINUTE2SECOND(x)                            ((x)*60)
#define MINUTE2HOUR(x)                              ((x)/60)
#define SECOND2MILLISECOND(x)                       ((x)*1000.0)
#define MILLISECOND2SECOND(x)                       ((x)/1000.0)

UISOFT_BEGIN_NAMESPACE(framework)

class unitconverter
{
    public:
        static value_type convert_distance_unit(value_type xi_distance, int xi_current_unit, int xi_new_unit)
        {
            value_type new_distance;

            if (xi_current_unit == xi_new_unit)
            {
                return xi_distance;
            }
            else
            {
                switch (xi_current_unit)
                {
                case edistanceunit_m:
                    switch (xi_new_unit)
                    {
                    case edistanceunit_km:
                        new_distance = (value_type)METER2KILOMETER(xi_distance);
                        break;
                    case edistanceunit_nmi:
                        new_distance = (value_type)METER2NAUTICAL_MILE(xi_distance);
                        break;
                    case edistanceunit_ft:
                        new_distance = (value_type)METER2FOOT(xi_distance);
                        break;
                    case edistanceunit_mi:
                        new_distance = (value_type)METER2MILE(xi_distance);
                        break;
                    default:
                        new_distance = xi_distance;    //CHECK
                        break;
                    }
                    break;
                case edistanceunit_km:
                    switch (xi_new_unit)
                    {
                    case edistanceunit_m:
                        new_distance = (value_type)KILOMETER2METER(xi_distance);
                        break;
                    case edistanceunit_nmi:
                        new_distance = (value_type)KILOMETER2METER(xi_distance);
                        new_distance = (value_type)METER2NAUTICAL_MILE(new_distance);
                        break;
                    case edistanceunit_ft:
                        new_distance = (value_type)KILOMETER2METER(xi_distance);
                        new_distance = (value_type)METER2FOOT(new_distance);
                        break;
                    case edistanceunit_mi:
                        new_distance = (value_type)KILOMETER2METER(xi_distance);
                        new_distance = (value_type)METER2MILE(new_distance);
                        break;
                    default:
                        new_distance = xi_distance;    //CHECK
                        break;
                    }
                    break;
                case edistanceunit_nmi:
                    switch (xi_new_unit)
                    {
                    case edistanceunit_m:
                        new_distance = (value_type)NAUTICAL_MILE2METER(xi_distance);
                        break;
                    case edistanceunit_km:
                        new_distance = (value_type)NAUTICAL_MILE2METER(xi_distance);
                        new_distance = (value_type)METER2KILOMETER(new_distance);
                        break;
                    case edistanceunit_ft:
                        new_distance = (value_type)NAUTICAL_MILE2METER(xi_distance);
                        new_distance = (value_type)METER2FOOT(new_distance);
                        break;
                    case edistanceunit_mi:
                        new_distance = (value_type)NAUTICAL_MILE2METER(xi_distance);
                        new_distance = (value_type)METER2MILE(new_distance);
                        break;
                    default:
                        new_distance = xi_distance;    //CHECK
                        break;
                    }
                    break;
                case edistanceunit_ft:
                    switch (xi_new_unit)
                    {
                    case edistanceunit_m:
                        new_distance = (value_type)FOOT2METER(xi_distance);
                        break;
                    case edistanceunit_km:
                        new_distance = (value_type)FOOT2METER(xi_distance);
                        new_distance = (value_type)METER2KILOMETER(new_distance);
                        break;
                    case edistanceunit_nmi:
                        new_distance = (value_type)FOOT2METER(xi_distance);
                        new_distance = (value_type)METER2NAUTICAL_MILE(new_distance);
                        break;
                    case edistanceunit_mi:
                        new_distance = (value_type)FOOT2METER(xi_distance);
                        new_distance = (value_type)METER2MILE(new_distance);
                        break;
                    default:
                        new_distance = xi_distance;    //CHECK
                        break;
                    }
                    break;
                case edistanceunit_mi:
                    switch (xi_new_unit)
                    {
                    case edistanceunit_m:
                        new_distance = (value_type)MILE2METER(xi_distance);
                        break;
                    case edistanceunit_km:
                        new_distance = (value_type)MILE2METER(xi_distance);
                        new_distance = (value_type)METER2KILOMETER(new_distance);
                        break;
                    case edistanceunit_nmi:
                        new_distance = (value_type)MILE2METER(xi_distance);
                        new_distance = (value_type)METER2NAUTICAL_MILE(new_distance);
                        break;
                    case edistanceunit_ft:
                        new_distance = (value_type)MILE2METER(xi_distance);
                        new_distance = (value_type)METER2FOOT(new_distance);
                        break;
                    default:
                        new_distance = xi_distance;    //CHECK
                        break;
                    }
                    break;                    
                default:
                    new_distance = xi_distance;    //CHECK
                    break;
                }
            }

            return new_distance;
        }

        static value_type convert_speed_unit(value_type xi_speed, int xi_current_unit, int xi_new_unit)
        {
            value_type new_speed;

            if (xi_current_unit == xi_new_unit)
            {
                return xi_speed;
            }
            else
            {
                switch (xi_current_unit)
                {
                case espeedunit_m_s:
                    switch (xi_new_unit)
                    {
                    case espeedunit_km_h:
                        new_speed = (value_type)METER_PER_SECOND_2_KILOMETER_PER_HOUR(xi_speed);
                        break;
                    case espeedunit_knots:
                        new_speed = (value_type)METER_PER_SECOND_2_KNOTS(xi_speed);
                        break;
                    case espeedunit_ft_min:
                        new_speed = (value_type)METER_PER_SECOND_2_FEET_PER_MIN(xi_speed);
                        break;
                    default:
                        new_speed = xi_speed;    //CHECK
                        break;
                    }
                    break;
                case espeedunit_km_h:
                    switch (xi_new_unit)
                    {
                    case espeedunit_m_s:
                        new_speed = (value_type)KILOMETER_PER_HOUR_2_METER_PER_SECOND(xi_speed);
                        break;
                    case espeedunit_knots:
                        new_speed = (value_type)KILOMETER_PER_HOUR_2_METER_PER_SECOND(xi_speed);
                        new_speed = (value_type)METER_PER_SECOND_2_KILOMETER_PER_HOUR(new_speed);
                        break;
                    case espeedunit_ft_min:
                        new_speed = (value_type)KILOMETER_PER_HOUR_2_METER_PER_SECOND(xi_speed);
                        new_speed = (value_type)METER_PER_SECOND_2_FEET_PER_MIN(new_speed);
                        break;
                    default:
                        new_speed = xi_speed;    //CHECK
                        break;
                    }
                    break;
                case espeedunit_knots:
                    switch (xi_new_unit)
                    {
                    case espeedunit_m_s:
                        new_speed = (value_type)KNOTS_2_METER_PER_SECOND(xi_speed);
                        break;
                    case espeedunit_km_h:
                        new_speed = (value_type)KNOTS_2_METER_PER_SECOND(xi_speed);
                        new_speed = (value_type)METER_PER_SECOND_2_KILOMETER_PER_HOUR(new_speed);
                        break;
                    case espeedunit_ft_min:
                        new_speed = (value_type)KNOTS_2_METER_PER_SECOND(xi_speed);
                        new_speed = (value_type)METER_PER_SECOND_2_FEET_PER_MIN(new_speed);
                        break;
                    default:
                        new_speed = xi_speed;    //CHECK
                        break;
                    }
                    break;
                case espeedunit_ft_min:
                    switch (xi_new_unit)
                    {
                    case espeedunit_m_s:
                        new_speed = (value_type)FEET_PER_MIN_2_METER_PER_SECOND(xi_speed);
                        break;
                    case espeedunit_km_h:
                        new_speed = (value_type)FEET_PER_MIN_2_METER_PER_SECOND(xi_speed);
                        new_speed = (value_type)METER_PER_SECOND_2_KILOMETER_PER_HOUR(new_speed);
                        break;
                    case espeedunit_knots:
                        new_speed = (value_type)FEET_PER_MIN_2_METER_PER_SECOND(xi_speed);
                        new_speed = (value_type)METER_PER_SECOND_2_KILOMETER_PER_HOUR(new_speed);
                        break;
                    default:
                        new_speed = xi_speed;    //CHECK
                        break;
                    }
                    break;
                default:
                    new_speed = xi_speed;    //CHECK
                    break;
                }
            }

            return new_speed;
        }

        static value_type convert_angle_unit(value_type xi_angle, int xi_current_unit, int xi_new_unit)
        {
            value_type new_angle;

            if (xi_current_unit == xi_new_unit)
            {
                new_angle = xi_angle;
            }
            else
            {
                switch (xi_current_unit)
                {
                    case eangleunit_deg:
                    {
                        switch (xi_new_unit)
                        {
                        case eangleunit_rad:
                            new_angle = (value_type)DEGREE2RADIANS(xi_angle);
                            break;
                        default:
                            new_angle = xi_angle;
                            break;
                        }
                        break;
                    }
                    case eangleunit_rad:
                    {
                        switch (xi_new_unit)
                        {
                        case eangleunit_deg:
                            new_angle = (value_type)RADIANS2DEGREE(xi_angle);
                            break;
                        default:
                            new_angle = xi_angle;
                            break;
                        }
                        break;
                    }
                    default:
                        new_angle = xi_angle;    //CHECK
                        break;
                }
            }

            return new_angle;
        }

        static value_type convert_time_unit(value_type xi_time, int xi_current_unit, int xi_new_unit)
        {
            value_type new_time;

            if (xi_current_unit == xi_new_unit)
            {
                new_time = xi_time;
            }
            else
            {
                switch (xi_current_unit)
                {
                    case etimeunit_s:
                    {
                        switch (xi_new_unit)
                        {
                        case etimeunit_hr:
                            new_time = (value_type)SECOND2HOUR(xi_time);
                            break;
                        case etimeunit_min:
                            new_time = (value_type)SECOND2MINUTE(xi_time);
                            break;
                        case etimeunit_ms:
                            new_time = (value_type)SECOND2MILLISECOND(xi_time);
                            break;
                        default:
                            new_time = xi_time;
                            break;
                        }
                        break;
                    }
                    case etimeunit_hr:
                    {
                        switch (xi_new_unit)
                        {
                        case etimeunit_s:
                            new_time = (value_type)HOUR2SECOND(xi_time);
                            break;
                        case etimeunit_min:
                            new_time = (value_type)HOUR2MINUTE(xi_time);
                            break;
                        case etimeunit_ms:
                            new_time = (value_type)HOUR2SECOND(xi_time);
                            new_time = (value_type)SECOND2MILLISECOND(new_time);
                            break;
                        default:
                            new_time = xi_time;
                            break;
                        }
                        break;
                    }
                    case etimeunit_min:
                    {
                        switch (xi_new_unit)
                        {
                        case etimeunit_s:
                            new_time = (value_type)MINUTE2SECOND(xi_time);
                            break;
                        case etimeunit_hr:
                            new_time = (value_type)MINUTE2HOUR(xi_time);
                            break;
                        case etimeunit_ms:
                            new_time = (value_type)MINUTE2SECOND(xi_time);
                            new_time = (value_type)SECOND2MILLISECOND(new_time);
                            break;
                        default:
                            new_time = xi_time;
                            break;
                        }
                        break;                    
                    }
                    case etimeunit_ms:
                    {
                        switch (xi_new_unit)
                        {
                        case etimeunit_s:
                            new_time = (value_type)MILLISECOND2SECOND(xi_time);
                            break;
                        case etimeunit_hr:
                            new_time = (value_type)MILLISECOND2SECOND(xi_time);
                            new_time = (value_type)SECOND2HOUR(new_time);
                            break;
                        case etimeunit_min:
                            new_time = (value_type)MILLISECOND2SECOND(xi_time);
                            new_time = (value_type)SECOND2MINUTE(new_time);
                            break;
                        default:
                            new_time = xi_time;
                            break;
                        }
                        break; 
                    }
                    default:
                        new_time = xi_time;    //CHECK
                        break;
                }                
            }

            return new_time;
        }

        static string_type convert_distance_to_display_string(value_type xi_distance, int xi_unit, int xiN = -1, bool xiFixed = false)
        {
			string_type disp_dist = converter::convert_to_string_type<value_type>(xi_distance, xiN, xiFixed);
            switch (xi_unit)
            {
                case edistanceunit_m:
                    if (xi_distance < 1000)
                    {
						disp_dist = converter::convert_to_string_type<value_type>(xi_distance, xiN, xiFixed);
                        disp_dist += _S(" m");
                    }
                    else
                    {
						disp_dist = converter::convert_to_string_type<value_type>(xi_distance / 1000, xiN, xiFixed);
                        disp_dist += _S(" km");
                    }                    
                    break;
                 case edistanceunit_km:
                    if (xi_distance < 1)
                    {
						disp_dist = converter::convert_to_string_type<value_type>(xi_distance * 1000, xiN, xiFixed);
                        disp_dist += _S(" m");
                    }
                    else
                    {
                        disp_dist += _S(" km");
                    }                    
                    break;
                 case edistanceunit_mi:
                    disp_dist += _S(" mi");
                    break;
                 case edistanceunit_nmi:
                    disp_dist += _S(" nmi");
                    break;
                 case edistanceunit_ft:
                    disp_dist += _S(" ft");
                    break;
				 case edistanceunit_px:
					 disp_dist += _S(" px");
					 break;
                default:                    
                    break;
            }

            return disp_dist;
        }

		static string_type convert_angle_to_display_string(value_type xi_angle, int xi_unit, int xiN = -1, bool xiFixed = false)
		{
			string_type disp_angle = converter::convert_to_string_type<value_type>(xi_angle, xiN, xiFixed);
			switch (xi_unit)
			{			
			case eangleunit_rad:
				disp_angle += _S(" rad");
				break;
			case eangleunit_deg:
				disp_angle += _S(" deg");
				break;
			default:
				break;
			}

			return disp_angle;
		}
};

UISOFT_END_NAMESPACE

#endif // __UNITCONVERTER_H__
