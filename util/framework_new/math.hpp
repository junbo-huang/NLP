#ifndef __MATH_H__
#define __MATH_H__

#pragma once

#include <framework.hpp>
#include <unitconverter.hpp>
#include <coordinateconverter.hpp>
//#include <framework/terrain/terrain.hpp>

UISOFT_BEGIN_NAMESPACE(framework)

class math
{
    public:
        // static bool abs(...)
        // {
        // }

		static value_type get_value_from_lookup_table(std::map<value_type, value_type> xi_lookup_table, value_type xi_key)
		{
			value_type value = 0;

			if (xi_lookup_table.size() > 0)
			{
				value = (*xi_lookup_table.rbegin()).second;
				auto prev_it = NULL;
				for (auto it = xi_lookup_table.begin(); it != xi_lookup_table.end(); it++)
				{
					if ((*it).first >= xi_key)
					{
						if (it == xi_lookup_table.begin())
						{
							value = (*it).second;
						}
						else
						{
							if ((*it).first == xi_key)
							{
								value = (*it).second;
							}
							else
							{
								value_type key = (*it).first;
								value_type k_value = (*it).second;
								it--;
								value_type p_key = (*it).first;
								value_type p_k_value = (*it).second;

								if (p_key != key)
								{
									value = p_k_value + (k_value - p_k_value) / (key - p_key)*(xi_key - p_key);
								}
								else
								{
									value = k_value;
								}

							}
						}
						break;
					}
				}
			}
			return value;
		}

		static value_type chi2inv(value_type xi_probability, int xi_degree_freedom)
		{
			if (xi_degree_freedom != 2)
			{
				//not implemented
			}

			std::map<value_type, value_type> lookup_table;
			lookup_table.insert(std::pair<value_type, value_type>(0.0, 0.0));
			lookup_table.insert(std::pair<value_type, value_type>(0.05, 0.1026));
			lookup_table.insert(std::pair<value_type, value_type>(0.10, 0.2107));
			lookup_table.insert(std::pair<value_type, value_type>(0.15, 0.3250));
			lookup_table.insert(std::pair<value_type, value_type>(0.20, 0.4463));
			lookup_table.insert(std::pair<value_type, value_type>(0.25, 0.5754));
			lookup_table.insert(std::pair<value_type, value_type>(0.30, 0.7133));
			lookup_table.insert(std::pair<value_type, value_type>(0.35, 0.8616));
			lookup_table.insert(std::pair<value_type, value_type>(0.40, 1.0217));
			lookup_table.insert(std::pair<value_type, value_type>(0.45, 1.1957));
			lookup_table.insert(std::pair<value_type, value_type>(0.50, 1.3863));
			lookup_table.insert(std::pair<value_type, value_type>(0.55, 1.5970));
			lookup_table.insert(std::pair<value_type, value_type>(0.60, 1.8326));
			lookup_table.insert(std::pair<value_type, value_type>(0.65, 2.0996));
			lookup_table.insert(std::pair<value_type, value_type>(0.70, 2.4079));
			lookup_table.insert(std::pair<value_type, value_type>(0.75, 2.7726));
			lookup_table.insert(std::pair<value_type, value_type>(0.80, 3.2189));
			lookup_table.insert(std::pair<value_type, value_type>(0.85, 3.7942));
			lookup_table.insert(std::pair<value_type, value_type>(0.90, 4.6052));
			lookup_table.insert(std::pair<value_type, value_type>(0.95, 5.9915));
			lookup_table.insert(std::pair<value_type, value_type>(0.9550, 6.2022));
			lookup_table.insert(std::pair<value_type, value_type>(0.9600, 6.4378));
			lookup_table.insert(std::pair<value_type, value_type>(0.9650, 6.7048));
			lookup_table.insert(std::pair<value_type, value_type>(0.9700, 7.0131));
			lookup_table.insert(std::pair<value_type, value_type>(0.9750, 7.3778));
			lookup_table.insert(std::pair<value_type, value_type>(0.9800, 7.8240));
			lookup_table.insert(std::pair<value_type, value_type>(0.9850, 8.3994));
			lookup_table.insert(std::pair<value_type, value_type>(0.9900, 9.2103));
			lookup_table.insert(std::pair<value_type, value_type>(0.9950, 10.5966));

			value_type v = get_value_from_lookup_table(lookup_table, xi_probability);

			return v;
		}

		static void calculate_error_ellipsoid_2x2(const std::vector<value_type>& xi_covariance_matrix, value_type xi_confidence_probability, value_type& xo_a_radius, value_type& xo_b_radius, value_type& xo_angle)
		{			
			double ksqrt = std::sqrt(chi2inv(xi_confidence_probability, 2));

			double p11 = xi_covariance_matrix[0];
			double p22 = xi_covariance_matrix[1];
			double p12 = xi_covariance_matrix[2];

			double sqrt_value = std::sqrt(std::pow(p11 - p22, 2) + 4 * std::pow(p12, 2));

			double lambda1 = 0.5*(p11 + p22 + sqrt_value);
			double lambda2 = 0.5*(p11 + p22 - sqrt_value);

			double a_radius = ksqrt*std::sqrt(std::abs(lambda1));
			double b_radius = ksqrt*std::sqrt(std::abs(lambda2));

			double alpha = p11 != p22 ? 0.5*std::atan((2 * p12) / (p11 - p22)) : 0.0;

			double degrees = alpha*(180.0 / PI);

			if (p22 > p11)
			{
				degrees += 90;
			}

			xo_a_radius = (value_type)a_radius;
			xo_b_radius = (value_type)b_radius;

			xo_angle = (value_type)degrees;
		}

		static void decimal_degree_to_degree_minute_and_seconds(double xi_angle, int& xo_degree, int& xo_minute, double& xo_seconds)
		{
			xo_degree = (int)xi_angle;
			xo_minute = (int)((fabs(xi_angle - xo_degree)) * 60.0);
			xo_seconds = (fabs(xi_angle - xo_degree) - xo_minute / 60.0)*60.0*60.0;
		}

		static int gcd(int a, int b)
		{
			if (a < b)
			{
				return gcd(b, a);
			}

			int f = a % b;
			if (f == 0)
			{
				return b;
			}			
			else
			{
				return gcd(b, f);
			}				
		}

		static value_type gcd(value_type a, value_type b, unsigned int xi_precison = 5)
		{
			int ai = (int)(a*pow(10, xi_precison));
			int bi = (int)(b*pow(10, xi_precison));

			int di = gcd(ai, bi);

			value_type d = di*pow(10, -1 * (int)xi_precison);

			return d;
		}

		template <typename T>
		static T gcd(const std::vector<T>& xi_numbers, unsigned int xi_precison = 5)
		{
			T d = 1;
			if (xi_numbers.size() == 0)
			{
				d = 1;
			}
			else if (xi_numbers.size() == 1)
			{
				d = xi_numbers[0];
			}
			else
			{
				d = gcd(xi_numbers[0], xi_numbers[1], xi_precison);
				for (int i = 2; i < (int)xi_numbers.size(); i++)
				{
					d = gcd(xi_numbers[i], d, xi_precison);
				}
			}

			return d;
		}

		/*static bool inside_line_of_sight(edistanceunit xi_distance_unit, eangleunit xi_angle_unit, value_type xi_sensor_lat, value_type xi_sensor_lon, value_type xi_sensor_alt, value_type xi_target_lat, value_type xi_target_lon, value_type xi_target_alt, terrain* xi_terrian)
		{
			bool result = true;
			value_type MAX_ALTITUDE_TRANSFORMATION_ERROR = 1.0;//check
			
			value_type altitude_error = unitconverter::convert_distance_unit(MAX_ALTITUDE_TRANSFORMATION_ERROR, edistanceunit_m, xi_distance_unit);

			if ((xi_target_alt < (-altitude_error)) || (xi_sensor_alt < (-altitude_error)))
			{
				result = false;
				return result;
			}
			
			value_type sensor_ecef_x;
			value_type sensor_ecef_y;
			value_type sensor_ecef_z;
			coordinateconverter::transform_geo2ecf(xi_distance_unit, xi_angle_unit, xi_sensor_lat, xi_sensor_lon, xi_sensor_alt, sensor_ecef_x, sensor_ecef_y, sensor_ecef_z);

			value_type target_ecef_x;
			value_type target_ecef_y;
			value_type target_ecef_z;
			coordinateconverter::transform_geo2ecf(xi_distance_unit, xi_angle_unit, xi_target_lat, xi_target_lon, xi_target_alt, target_ecef_x, target_ecef_y, target_ecef_z);

			value_type V12[3];
			V12[0] = target_ecef_x - sensor_ecef_x;
			V12[1] = target_ecef_y - sensor_ecef_y;
			V12[2] = target_ecef_z - sensor_ecef_z;

			value_type vector_12_length = sqrt(pow(V12[0], 2) + pow(V12[1], 2) + pow(V12[2], 2));

			value_type projection_length = (-1 * sensor_ecef_x*V12[0] - sensor_ecef_y*V12[1] - sensor_ecef_z*V12[2]) / vector_12_length;

			value_type projection_x = sensor_ecef_x + V12[0] * projection_length / vector_12_length;
			value_type projection_y = sensor_ecef_y + V12[1] * projection_length / vector_12_length;
			value_type projection_z = sensor_ecef_z + V12[2] * projection_length / vector_12_length;

			if ((projection_length >= 0) && (projection_length <= vector_12_length))
			{				
				value_type projection_lat, projection_lon, projection_alt;
				coordinateconverter::transform_ecef2geo(xi_distance_unit, xi_angle_unit, projection_x, projection_y, projection_z, projection_lat, projection_lon, projection_alt);

				if (projection_alt < 0)
				{
					result = false;
					return result;
				}
			}

			value_type dted_resolution;
			value_type max_elevation;
			if (xi_terrian)
			{
				dted_resolution = unitconverter::convert_distance_unit((*xi_terrian).get_resolution(), edistanceunit_m, xi_distance_unit);

				BoundingBox b;
				b.MinLat = unitconverter::convert_angle_unit(std::min<value_type>(xi_sensor_lat, xi_target_lat), xi_angle_unit, eangleunit_deg);
				b.MaxLat = unitconverter::convert_angle_unit(std::max<value_type>(xi_sensor_lat, xi_target_lat), xi_angle_unit, eangleunit_deg);
				b.MinLong = unitconverter::convert_angle_unit(std::min<value_type>(xi_sensor_lon, xi_target_lon), xi_angle_unit, eangleunit_deg);
				b.MaxLong = unitconverter::convert_angle_unit(std::max<value_type>(xi_sensor_lon, xi_target_lon), xi_angle_unit, eangleunit_deg);
				max_elevation = unitconverter::convert_distance_unit((*xi_terrian).get_max_elevation(b), edistanceunit_m, xi_distance_unit);
			}
			else
			{
				dted_resolution = unitconverter::convert_distance_unit(900, edistanceunit_m, xi_distance_unit);//need to change
				max_elevation = unitconverter::convert_distance_unit(8000, edistanceunit_m, xi_distance_unit); // need to change
			}
			
			int n = (int)(vector_12_length / (0.5*dted_resolution));

			int i_start = 1;
			int i_end = n;

			if ((xi_target_alt > xi_sensor_alt) && (xi_target_alt > max_elevation))
			{
				i_end = (int)((xi_target_alt - max_elevation) / (xi_target_alt - xi_sensor_alt)*n);
			}
			else if (xi_sensor_alt > max_elevation)
			{
				i_start = (int)((1 - (xi_sensor_alt - max_elevation)) / (xi_sensor_alt - xi_target_alt))*n;
			}

			for (int i = i_start; i <= i_end; i++)
			{
				value_type step_x = sensor_ecef_x + V12[0] * i / (value_type)n;
				value_type step_y = sensor_ecef_y + V12[1] * i / (value_type)n;
				value_type step_z = sensor_ecef_z + V12[2] * i / (value_type)n;

				value_type step_lat, step_lon, step_alt;				

				coordinateconverter::transform_ecef2geo(xi_distance_unit, xi_angle_unit, step_x, step_y, step_z, step_lat, step_lon, step_alt);
				value_type line_alt = step_alt;
				value_type ground_alt = 0;
				if (xi_terrian)
				{
					value_type lat = unitconverter::convert_angle_unit(step_lat, xi_angle_unit, eangleunit_deg);
					value_type lon = unitconverter::convert_angle_unit(step_lon, xi_angle_unit, eangleunit_deg);
					ground_alt = (*xi_terrian).get_elevation(lat, lon);
					ground_alt = unitconverter::convert_distance_unit(ground_alt, edistanceunit_m, xi_distance_unit);
				}

				if (line_alt < ground_alt)
				{
					result = false;
					break;
				}
			}

			return result;
		}*/

		static value_type predict_state_lrf(value_type xi_known_time, value_type xi_x, value_type xi_vel_x, value_type xi_current_time)
		{
			value_type pred_x = xi_x;
			value_type delT = xi_current_time - xi_known_time;
			
			if ((delT != 0) && (xi_vel_x != 0))
			{
				pred_x += delT*xi_vel_x; 				
			}

			return pred_x;
		}

		static void predict_state_geo(edistanceunit xi_distance_unit, eangleunit xi_angle_unit, value_type xi_known_time, value_type xi_lat, value_type xi_lon, value_type xi_alt, value_type xi_vel_x, value_type xi_vel_y, value_type xi_vel_z, value_type xi_current_time, value_type& xo_lat, value_type& xo_lon, value_type& xo_alt)
		{
			xo_lat = xi_lat;
			xo_lon = xi_lon;
			xo_alt = xi_alt;

			value_type delT = xi_current_time - xi_known_time;

			if (delT != 0)
			{				
				xo_lat += coordinateconverter::find_delta_latitude(xi_distance_unit, xi_angle_unit, xi_lat, xi_vel_y*delT);
				xo_lon += coordinateconverter::find_delta_latitude(xi_distance_unit, xi_angle_unit, xi_lat, xi_vel_x*delT);
				xo_alt += xi_vel_z*delT;
			}
		}

		static bool is_nan(value_type xi_x)
		{
			return !(xi_x == xi_x);
		}
};

UISOFT_END_NAMESPACE

#endif // __MATH_H__
