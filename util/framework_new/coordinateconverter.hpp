#ifndef __COORDINATECONVERTER_H__
#define __COORDINATECONVERTER_H__

#pragma once

#include <framework.hpp>
#include <unitconverter.hpp>
#include <matrix.hpp>
#include <units.hpp>

#define EARTH_ECCENTRICITY	(0.0818191)
#define EARTH_RADIUS		(6378.137) //km
#define PSITOL				(1.0e-9)
#define FLT_EPS             (1.0e-16)

#undef min
#undef max

UISOFT_BEGIN_NAMESPACE(framework)

class coordinateconverter
{
    public:
        typedef std::vector<std::vector<value_type>> value_t_matrix;
    
    public:
        static void define_lrf_origin_and_rotation_matrix(edistanceunit xi_distance_unit,
                                                          eangleunit xi_angle_unit,
                                                          value_type xi_latitude, 
                                                          value_type xi_longitude,
														  value_type xi_altitude,
                                                          std::vector<value_type>& xo_origin,
                                                          value_t_matrix& xo_rotation_matrix)
        {
            double Dvarphi;

            xi_latitude = unitconverter::convert_angle_unit(xi_latitude, xi_angle_unit, eangleunit_rad);
            xi_longitude = unitconverter::convert_angle_unit(xi_longitude, xi_angle_unit, eangleunit_rad);

	        Dvarphi = EARTH_RADIUS/sqrt(1.0 - (value_type)pow(EARTH_ECCENTRICITY * sin(xi_latitude),2));
			double h = unitconverter::convert_distance_unit(xi_altitude, xi_distance_unit, edistanceunit_km);

            xo_origin.resize(3,0.0);
	        xo_origin[0] = 0.0;
			xo_origin[1] = (value_type)(0.5 * (Dvarphi) * (value_type)pow(EARTH_ECCENTRICITY, 2) * (value_type)sin(2 * xi_latitude));//check h place
			xo_origin[2] = (value_type)((Dvarphi) * ((value_type)pow(EARTH_ECCENTRICITY * (value_type)sin(xi_latitude), 2) - 1.0)) - h;//check h place

	        xo_origin[1] = unitconverter::convert_distance_unit(xo_origin[1], edistanceunit_km, xi_distance_unit);
	        xo_origin[2] = unitconverter::convert_distance_unit(xo_origin[2], edistanceunit_km, xi_distance_unit);

            matrix<value_type>::init_matrix(3,3,xo_rotation_matrix);
	        xo_rotation_matrix[0][0] = -sin(xi_longitude);
	        xo_rotation_matrix[0][1] = cos(xi_longitude);
	        xo_rotation_matrix[0][2] = 0.0;

	        xo_rotation_matrix[1][0] = -sin(xi_latitude) * cos(xi_longitude);
	        xo_rotation_matrix[1][1] = -sin(xi_latitude) * sin(xi_longitude);
	        xo_rotation_matrix[1][2] = cos(xi_latitude);

	        xo_rotation_matrix[2][0] = cos(xi_latitude) * cos(xi_longitude);
	        xo_rotation_matrix[2][1] = cos(xi_latitude) * sin(xi_longitude);
	        xo_rotation_matrix[2][2] = sin(xi_latitude);
        }

        static void transform_ecef2geo(edistanceunit xi_distance_unit,
                                       eangleunit xi_angle_unit,
                                       value_type xi_x_ecf,
                                       value_type xi_y_ecf,
                                       value_type xi_z_ecf,
                                       value_type& xo_latitude, 
                                       value_type& xo_longitude,
                                       value_type& xo_altitude)
        {
	        value_type ecf_x, ecf_y, ecf_z, psi, psi0, c1, Rpsi;

	        ecf_x = unitconverter::convert_distance_unit(xi_x_ecf, xi_distance_unit, edistanceunit_km);
	        ecf_y = unitconverter::convert_distance_unit(xi_y_ecf, xi_distance_unit, edistanceunit_km);
	        ecf_z = unitconverter::convert_distance_unit(xi_z_ecf, xi_distance_unit, edistanceunit_km);

	        c1 = (value_type)(1.0 - (value_type)pow(EARTH_ECCENTRICITY,2));

	        psi = 0.0;
	        do
	        {
		        psi0 = psi;
		        psi = sin(psi);
		        Rpsi = (value_type)(EARTH_RADIUS / sqrt(1.0 - (value_type)pow(EARTH_ECCENTRICITY * psi,2)));
		        psi = atan2((value_type)(ecf_z + (1.0 - c1) * Rpsi * psi), (value_type)sqrt((value_type)pow(ecf_x,2) + (value_type)pow(ecf_y,2)));
		        psi0 = fabs(psi - psi0);
	        }
	        while (psi0 > PSITOL);

	        if (fabs(psi) < PSITOL)
	        {
		        //error("Latitude Zero. error in ecef2geo_pos()");
	        }

	        xo_latitude = psi;
	        if ((ecf_x != 0.0) || (ecf_y != 0.0))
	        {
		        xo_longitude = atan2(ecf_y, ecf_x);
	        }
	        else
	        {
		        xo_longitude = 0.0;
	        }

            if (fabs(psi) < PSITOL)
            {
                xo_altitude = sqrt(pow(ecf_x,2) + pow(ecf_y,2)) / cos(psi) - c1 * Rpsi;//check
            }
            else
            {
                xo_altitude = ecf_z / sin(psi) - c1 * Rpsi;
            }

            xo_latitude = unitconverter::convert_angle_unit(xo_latitude, eangleunit_rad, xi_angle_unit);
            xo_longitude = unitconverter::convert_angle_unit(xo_longitude, eangleunit_rad, xi_angle_unit);
	        xo_altitude = unitconverter::convert_distance_unit(xo_altitude, edistanceunit_km, xi_distance_unit);
        }
                
        static void transform_geo2lrf(edistanceunit xi_distance_unit,
                                      eangleunit xi_angle_unit,
                                      value_type xi_lrf_latitude,
                                      value_type xi_lrf_longitude,
                                      value_type xi_lrf_altitude,
                                      value_type xi_latitude, 
                                      value_type xi_longitude,
                                      value_type xi_altitude,                                      
                                      value_type& xo_x,
                                      value_type& xo_y,
                                      value_type& xo_z)
        {
	        if ((xi_latitude == xi_lrf_latitude) && (xi_longitude == xi_lrf_longitude) && (xi_altitude == xi_lrf_altitude))
	        {
		        xo_x = 0.0;
                xo_y = 0.0;
                xo_z = 0.0;
	        }
	        else
	        {		        
                std::vector<value_type> x(3);                

                std::vector<value_type> origin_lrf(3);
                value_t_matrix rotation_matrix_lrf;           
                matrix<value_type>::init_matrix(3,3,rotation_matrix_lrf);

				define_lrf_origin_and_rotation_matrix(xi_distance_unit, xi_angle_unit, xi_lrf_latitude, xi_lrf_longitude, xi_lrf_altitude, origin_lrf, rotation_matrix_lrf);

                std::vector<value_type> origin(3);
                value_t_matrix rotation_matrix;           
                matrix<value_type>::init_matrix(3,3,rotation_matrix);

				define_lrf_origin_and_rotation_matrix(xi_distance_unit, xi_angle_unit, xi_latitude, xi_longitude, xi_altitude, origin, rotation_matrix);
                
		        transform_lrf1_to_lrf2(origin, rotation_matrix, origin_lrf, rotation_matrix_lrf, x[0], x[1], x[2], xo_x, xo_y, xo_z);
                		        
	        }
        }

        static void transform_lrf2geo(edistanceunit xi_distance_unit,
                                      eangleunit xi_angle_unit,
                                      value_type xi_lrf_latitude,
                                      value_type xi_lrf_longitude,
                                      value_type xi_lrf_altitude,                                      
                                      value_type xi_x,
                                      value_type xi_y,
                                      value_type xi_z,
                                      value_type& xo_latitude, 
                                      value_type& xo_longitude,
                                      value_type& xo_altitude)
        {
            std::vector<value_type> origin_lrf(3);
            value_t_matrix rotation_matrix_lrf;           
            matrix<value_type>::init_matrix(3,3,rotation_matrix_lrf);

			define_lrf_origin_and_rotation_matrix(xi_distance_unit, xi_angle_unit, xi_lrf_latitude, xi_lrf_longitude, xi_lrf_altitude, origin_lrf, rotation_matrix_lrf);

            transform_lrf2geo(xi_distance_unit, xi_angle_unit, origin_lrf, rotation_matrix_lrf, xi_x, xi_y, xi_z, xo_latitude, xo_longitude, xo_altitude);
        }

        static void transform_lrf2geo(edistanceunit xi_distance_unit,
                                      eangleunit xi_angle_unit,
                                      const std::vector<value_type>& xi_origin, 
                                      const value_t_matrix& xi_rotation_matrix,                                      
                                      value_type xi_x,
                                      value_type xi_y,
                                      value_type xi_z,
                                      value_type& xo_latitude, 
                                      value_type& xo_longitude,
                                      value_type& xo_altitude)
        {
            value_type x_ecf, y_ecf, z_ecf;
            transform_lrf2ecf(xi_origin, xi_rotation_matrix, xi_x, xi_y, xi_z, x_ecf, y_ecf, z_ecf);

            transform_ecef2geo(xi_distance_unit,xi_angle_unit, x_ecf, y_ecf, z_ecf, xo_latitude, xo_longitude, xo_altitude);
        }

		static void transform_geo2ecf(edistanceunit xi_distance_unit,
										eangleunit xi_angle_unit,
										value_type xi_latitude,
										value_type xi_longitude,
										value_type xi_altitude,
										value_type& xo_x_ecf,
										value_type& xo_y_ecf,
										value_type& xo_z_ecf)
		{
			std::vector<value_type> origin_lrf(3);
			value_t_matrix rotation_matrix_lrf;
			matrix<value_type>::init_matrix(3, 3, rotation_matrix_lrf);

			define_lrf_origin_and_rotation_matrix(xi_distance_unit, xi_angle_unit, xi_latitude, xi_longitude, 0.0, origin_lrf, rotation_matrix_lrf);

			transform_lrf2ecf(origin_lrf, rotation_matrix_lrf, 0.0, 0.0, xi_altitude, xo_x_ecf, xo_y_ecf, xo_z_ecf);
		}

        static void transform_lrf2ecf(const std::vector<value_type>& origin, 
                                      const value_t_matrix& rotation_matrix,                                      
                                      value_type xi_x, 
                                      value_type xi_y, 
                                      value_type xi_z, 
                                      value_type& xo_x_ecf, 
                                      value_type& xo_y_ecf, 
                                      value_type& xo_z_ecf)
        {
            std::vector<value_type> x(3);            
            x[0] = xi_x;
            x[1] = xi_y;
            x[2] = xi_z;
		    matrix<value_type>::subtract_from_vector(origin, x);

            value_t_matrix rotation_matrix_transpose;
            matrix<value_type>::init_matrix(3,3,rotation_matrix_transpose);
		    matrix<value_type>::matrix_transpose(rotation_matrix, rotation_matrix_transpose);

            std::vector<value_type> x_new(3);
            
		    matrix<value_type>::matrix_vector_product(rotation_matrix_transpose, x, x_new);
            xo_x_ecf = x_new[0];
            xo_y_ecf = x_new[1];
            xo_z_ecf = x_new[2];
        }

        static void transform_lrf1_to_lrf2(edistanceunit xi_distance_unit,
                                           eangleunit xi_angle_unit,
                                           value_type xi_lrf1_latitude,
                                           value_type xi_lrf1_longitude,
                                           value_type xi_lrf1_altitude,
                                           value_type xi_lrf2_latitude,
                                           value_type xi_lrf2_longitude,
                                           value_type xi_lrf2_altitude,
                                           value_type xi_x1, 
                                           value_type xi_y1, 
                                           value_type xi_z1, 
                                           value_type& xo_x, 
                                           value_type& xo_y, 
                                           value_type& xo_z)
        {
            if ((xi_lrf1_latitude == xi_lrf2_latitude) && (xi_lrf1_longitude == xi_lrf2_longitude) && (xi_lrf1_altitude == xi_lrf2_altitude))
	        {
		        xo_x = xi_x1;
                xo_y = xi_y1;
                xo_z = xi_z1;
	        }
	        else
	        {		        
                std::vector<value_type> x(3);                

                std::vector<value_type> origin_lrf1(3);
                value_t_matrix rotation_matrix_lrf1;           
                matrix<value_type>::init_matrix(3,3,rotation_matrix_lrf1);

				define_lrf_origin_and_rotation_matrix(xi_distance_unit, xi_angle_unit, xi_lrf1_latitude, xi_lrf1_longitude, xi_lrf1_altitude, origin_lrf1, rotation_matrix_lrf1);

                std::vector<value_type> origin_lrf2(3);
                value_t_matrix rotation_matrix_lrf2;           
                matrix<value_type>::init_matrix(3,3,rotation_matrix_lrf2);

				define_lrf_origin_and_rotation_matrix(xi_distance_unit, xi_angle_unit, xi_lrf2_latitude, xi_lrf2_longitude, xi_lrf2_altitude, origin_lrf2, rotation_matrix_lrf2);
                
		        transform_lrf1_to_lrf2(origin_lrf1, rotation_matrix_lrf1, origin_lrf2, rotation_matrix_lrf2, xi_x1, xi_y1, xi_z1, xo_x, xo_y, xo_z);
                		        
	        }
        }

        static void transform_lrf1_to_lrf2(const std::vector<value_type>& origin1, 
                                           const value_t_matrix& rotation_matrix1, 
                                           const std::vector<value_type>& origin2, 
                                           const value_t_matrix& rotation_matrix2, 
                                           value_type xi_x1, 
                                           value_type xi_y1, 
                                           value_type xi_z1, 
                                           value_type& xo_x, 
                                           value_type& xo_y, 
                                           value_type& xo_z)
        {
            value_t_matrix rotation_matrix_comb;
            matrix<value_type>::init_matrix(3,3,rotation_matrix_comb);

            matrix<value_type>::matrix_matrix_transpose_product(rotation_matrix2, rotation_matrix1, rotation_matrix_comb);
            std::vector<value_type> origin_comb(3);
	        matrix<value_type>::matrix_vector_product(rotation_matrix_comb, origin1, origin_comb);
	        matrix<value_type>::subtract_vector(origin2, origin_comb, origin_comb);

            std::vector<value_type> x1(3), x2(3);            
            x1[0] = xi_x1;
            x1[1] = xi_y1;
            x1[2] = xi_z1;
	        matrix<value_type>::matrix_vector_product(rotation_matrix_comb, x1, x2);
	        matrix<value_type>::add_to_vector(origin_comb, x2);
            xo_x = x2[0];
            xo_y = x2[1];
            xo_z = x2[2];
        }

		static void transform_velocity_lrf1_to_lrf2(edistanceunit xi_distance_unit,
														eangleunit xi_angle_unit,
														value_type xi_lrf1_latitude,
														value_type xi_lrf1_longitude,
														value_type xi_lrf1_altitude,
														value_type xi_lrf2_latitude,
														value_type xi_lrf2_longitude,
														value_type xi_lrf2_altitude,
														value_type xi_x1,
														value_type xi_y1,
														value_type xi_z1,
														value_type& xo_x,
														value_type& xo_y,
														value_type& xo_z)
		{
			if ((xi_lrf1_latitude == xi_lrf2_latitude) && (xi_lrf1_longitude == xi_lrf2_longitude))
			{
				xo_x = xi_x1;
				xo_y = xi_y1;
				xo_z = xi_z1;
			}
			else
			{
				std::vector<value_type> x(3);

				std::vector<value_type> origin_lrf1(3);
				value_t_matrix rotation_matrix_lrf1;
				matrix<value_type>::init_matrix(3, 3, rotation_matrix_lrf1);

				define_lrf_origin_and_rotation_matrix(xi_distance_unit, xi_angle_unit, xi_lrf1_latitude, xi_lrf1_longitude, xi_lrf1_altitude, origin_lrf1, rotation_matrix_lrf1);

				std::vector<value_type> origin_lrf2(3);
				value_t_matrix rotation_matrix_lrf2;
				matrix<value_type>::init_matrix(3, 3, rotation_matrix_lrf2);

				define_lrf_origin_and_rotation_matrix(xi_distance_unit, xi_angle_unit, xi_lrf2_latitude, xi_lrf2_longitude, xi_lrf2_altitude, origin_lrf2, rotation_matrix_lrf2);

				transform_velocity_lrf1_to_lrf2(rotation_matrix_lrf1, rotation_matrix_lrf2, xi_x1, xi_y1, xi_z1, xo_x, xo_y, xo_z);

			}
		}

		static void transform_velocity_lrf1_to_lrf2(const value_t_matrix& rotation_matrix1,													
													const value_t_matrix& rotation_matrix2,
													value_type xi_x1,
													value_type xi_y1,
													value_type xi_z1,
													value_type& xo_x,
													value_type& xo_y,
													value_type& xo_z)
		{
			value_t_matrix rotation_matrix_comb;
			matrix<value_type>::init_matrix(3, 3, rotation_matrix_comb);

			matrix<value_type>::matrix_matrix_transpose_product(rotation_matrix2, rotation_matrix1, rotation_matrix_comb);
			std::vector<value_type> x1(3), x2(3);
			x1[0] = xi_x1;
			x1[1] = xi_y1;
			x1[2] = xi_z1;
			matrix<value_type>::matrix_vector_product(rotation_matrix_comb, x1, x2);			
			xo_x = x2[0];
			xo_y = x2[1];
			xo_z = x2[2];
		}

        static void transform_covariance_lrf1_to_lrf2(edistanceunit xi_distance_unit,
                                           eangleunit xi_angle_unit,
                                           value_type xi_lrf1_latitude,
                                           value_type xi_lrf1_longitude,
                                           value_type xi_lrf1_altitude,
                                           value_type xi_lrf2_latitude,
                                           value_type xi_lrf2_longitude,
                                           value_type xi_lrf2_altitude,
                                           const value_t_matrix& xi_P,
                                           value_t_matrix& xo_P)
        {
            if ((xi_lrf1_latitude == xi_lrf2_latitude) && (xi_lrf1_longitude == xi_lrf2_longitude) && (xi_lrf1_altitude == xi_lrf2_altitude))
	        {
                matrix<value_type>::init_matrix(3,3,xo_P);
		        matrix<value_type>::copy_matrix(xi_P, xo_P);
	        }
	        else
	        {		        
                std::vector<value_type> x(3);                

                std::vector<value_type> origin_lrf1(3);
                value_t_matrix rotation_matrix_lrf1;           
                matrix<value_type>::init_matrix(3,3,rotation_matrix_lrf1);

				define_lrf_origin_and_rotation_matrix(xi_distance_unit, xi_angle_unit, xi_lrf1_latitude, xi_lrf1_longitude, xi_lrf1_altitude, origin_lrf1, rotation_matrix_lrf1);

                std::vector<value_type> origin_lrf2(3);
                value_t_matrix rotation_matrix_lrf2;           
                matrix<value_type>::init_matrix(3,3,rotation_matrix_lrf2);

				define_lrf_origin_and_rotation_matrix(xi_distance_unit, xi_angle_unit, xi_lrf2_latitude, xi_lrf2_longitude, xi_lrf2_altitude, origin_lrf2, rotation_matrix_lrf2);
                
		        transform_covariance_lrf1_to_lrf2(origin_lrf1, rotation_matrix_lrf1, origin_lrf2, rotation_matrix_lrf2, xi_P, xo_P);
                		        
	        }
        }

        static void transform_covariance_lrf1_to_lrf2(const std::vector<value_type>& /*origin1*/, 
                                           const value_t_matrix& rotation_matrix1, 
                                           const std::vector<value_type>& /*origin2*/, 
                                           const value_t_matrix& rotation_matrix2, 
                                           const value_t_matrix& xi_P,
                                           value_t_matrix& xo_P)
        {
            value_t_matrix rotation_matrix_comb;
            matrix<value_type>::init_matrix(3,3,rotation_matrix_comb);

            matrix<value_type>::matrix_matrix_transpose_product(rotation_matrix2, rotation_matrix1, rotation_matrix_comb);
                    
            matrix<value_type>::init_matrix(3,3,xo_P);
            matrix<value_type>::weighted_matrix_outer_product(rotation_matrix_comb, xi_P, xo_P);
        }

		static void transform_velocity_lrf2ecef(eangleunit xi_angle_unit, value_type xi_lat, value_type xi_lon, value_type xi_east, value_type xi_north, value_type xi_up, value_type& xo_u, value_type& xo_v, value_type& xo_w)
		{
			xi_lat = unitconverter::convert_angle_unit(xi_lat, xi_angle_unit, eangleunit_rad);
			xi_lon = unitconverter::convert_angle_unit(xi_lon, xi_angle_unit, eangleunit_rad);

			value_type cosLat = cos(xi_lat);
			value_type sinLat = sin(xi_lat);
			value_type cosLon = cos(xi_lon);
			value_type sinLon = sin(xi_lon);

			value_type t = cosLat*xi_up - sinLat*xi_north;
			xo_w = sinLat*xi_up + cosLat*xi_north;

			xo_u = cosLon*t - sinLon*xi_east;
			xo_v = sinLon*t + cosLon*xi_east;
		}

		static void transform_velocity_ecef2lrf(eangleunit xi_angle_unit, value_type xi_lat, value_type xi_lon, value_type xi_u, value_type xi_v, value_type xi_w, value_type& xo_east, value_type& xo_north, value_type& xo_up)
		{
			xi_lat = unitconverter::convert_angle_unit(xi_lat, xi_angle_unit, eangleunit_rad);
			xi_lon = unitconverter::convert_angle_unit(xi_lon, xi_angle_unit, eangleunit_rad);

			value_type cosLat = cos(xi_lat);
			value_type sinLat = sin(xi_lat);
			value_type cosLon = cos(xi_lon);
			value_type sinLon = sin(xi_lon);

			value_type t = cosLon*xi_u + sinLon*xi_v;
			xo_east = -sinLon*xi_u + cosLon*xi_v;

			xo_up = cosLat*t + sinLat*xi_w;
			xo_north = -sinLat*t + cosLat*xi_w;
		}

        static void transform_range_azimuth_altitude_measurement_to_slcf(edistanceunit xi_distance_unit,
                                                                     eangleunit xi_angle_unit,
                                                                     value_type xi_sensor_latitude,
                                                                     value_type /*xi_sensor_longitude*/,
                                                                     value_type xi_sensor_altitude,
                                                                     value_type xi_range,
                                                                     value_type xi_azimuth,
                                                                     value_type xi_altitude,
                                                                     value_type xi_var_range,
                                                                     value_type xi_var_azimuth,
                                                                     value_type xi_var_altitude,
                                                                     value_type& xo_x,
                                                                     value_type& xo_y,
                                                                     value_type& xo_z,
                                                                     value_type& xo_var_x,
                                                                     value_type& xo_var_y,
                                                                     value_type& xo_var_z,
                                                                     value_type& xo_cov_xy,
                                                                     value_type& xo_cov_xz,
                                                                     value_type& xo_cov_yz)                                                                     
        {
	        value_type v, r, rs, hs, z_rho, z_theta, z_h, x, y, z, dist_unit_conv_factor;
	        value_t_matrix R, H, con_R;
            matrix<value_type>::init_matrix(3,3,R);
            matrix<value_type>::init_matrix(3,3,H);
            matrix<value_type>::init_matrix(3,3,con_R);
            	        
	        hs = unitconverter::convert_distance_unit(xi_sensor_altitude, xi_distance_unit, edistanceunit_km);
	        z_rho = unitconverter::convert_distance_unit(xi_range, xi_distance_unit, edistanceunit_km);
	        z_theta = unitconverter::convert_angle_unit(xi_azimuth, xi_angle_unit, eangleunit_rad);
	        z_h = unitconverter::convert_distance_unit(xi_altitude, xi_distance_unit, edistanceunit_km);

	        rs = (value_type)(EARTH_RADIUS * sqrt(1.0 - pow(EARTH_ECCENTRICITY,2)) / ((value_type)1.0 - pow(EARTH_ECCENTRICITY * sin(unitconverter::convert_angle_unit(xi_sensor_latitude, xi_angle_unit, eangleunit_rad)),2)));

	        v = (value_type)((1.0 - hs / (rs + hs)) * z_h - (1.0 - 0.5 * hs / (rs + hs)) * hs - 0.5 * (pow(z_rho,2) - pow(z_h,2)) / (rs + hs));
	        //v = (z_h + z_rho + hs)*(z_h - z_rho - hs)/(2*(rs+hs)); %NEED TO CHECK

	        r = pow(z_rho,2) - pow(v,2);
	        r = std::max(r, (value_type)FLT_EPS);
	        r = sqrt(r);

	        x = r * cos(z_theta);		//sin(z_theta);
	        y = r * sin(z_theta);		//cos(z_theta);
			z = v;// +hs;

	        xo_x = unitconverter::convert_distance_unit(x, edistanceunit_km, xi_distance_unit);
	        xo_y = unitconverter::convert_distance_unit(y, edistanceunit_km, xi_distance_unit);
	        xo_z = unitconverter::convert_distance_unit(z, edistanceunit_km, xi_distance_unit);
            	       
	        dist_unit_conv_factor = unitconverter::convert_distance_unit(1.0, xi_distance_unit, edistanceunit_km);
	        R[0][0] = pow(dist_unit_conv_factor,2) * xi_var_range;
			R[1][1] = xi_var_azimuth*pow(unitconverter::convert_angle_unit(1.0, xi_angle_unit, eangleunit_rad), 2);
	        R[2][2] = pow(dist_unit_conv_factor,2) * xi_var_altitude;

	        H[0][0] = (1 + v / (rs + hs)) * (z_rho * x) / (pow(z_rho,2) - pow(v,2));
	        H[0][1] = -y;				//y;
	        H[0][2] = (value_type)(-(1.0 + (z_h - hs) / (rs + hs)) * v * x / pow(r,2));	//-((rs+v)/(rs+hs))*((z_rho*x)/(SQR(z_rho) - SQR(v)));
	        H[1][0] = (1 + v / (rs + hs)) * (z_rho * y) / (pow(z_rho,2) - pow(v,2));
	        H[1][1] = x;				//-x;
	        H[1][2] = (value_type)(-(1.0 + (z_h - hs) / (rs + hs)) * v * y / pow(r,2));	//-((rs+v)/(rs+hs))*((z_rho*y)/(SQR(z_rho) - SQR(v)));
	        H[2][0] = -(z_rho / (rs + hs));
	        H[2][1] = 0;
	        H[2][2] = (value_type)(1.0 + (z_h - hs) / (rs + hs));	//(rs + v) / (rs + hs);

            matrix<value_type>::weighted_matrix_outer_product(H, R, con_R);	        
	        matrix<value_type>::scale_matrix(pow(1 / dist_unit_conv_factor,2), con_R);

	        xo_var_x = con_R[0][0];
	        xo_var_y = con_R[1][1];
	        xo_var_z = con_R[2][2];
	        xo_cov_xy = con_R[0][1];
	        xo_cov_xz = con_R[0][2];
	        xo_cov_yz = con_R[1][2];
        }

        static value_type find_distance_between_geo_points(edistanceunit xi_distance_unit, eangleunit xi_angle_unit,value_type xi_lat1, value_type xi_lon1, value_type xi_lat2, value_type xi_lon2)
        {            
            //Haversine Formula (from R.W. Sinnott, "Virtues of the Haversine", Sky and Telescope, vol. 68, no. 2, 1984, p. 159):
            xi_lat1 = unitconverter::convert_angle_unit(xi_lat1, xi_angle_unit, eangleunit_rad);
            xi_lon1 = unitconverter::convert_angle_unit(xi_lon1, xi_angle_unit, eangleunit_rad);
            xi_lat2 = unitconverter::convert_angle_unit(xi_lat2, xi_angle_unit, eangleunit_rad);
            xi_lon2 = unitconverter::convert_angle_unit(xi_lon2, xi_angle_unit, eangleunit_rad);

            value_type dlon = xi_lon2 - xi_lon1;
            value_type dlat = xi_lat2 - xi_lat1;

            value_type a = pow(sin(dlat/2),2) + cos(xi_lat1) * cos(xi_lat2) * pow(sin(dlon/2),2);
            value_type c = 2 * asin(std::min((value_type)1,sqrt(a)));
            value_type distance = EARTH_RADIUS * c;

            distance = unitconverter::convert_distance_unit(distance, edistanceunit_km, xi_distance_unit);

            return distance;
        }

		static value_type find_bearing_between_geo_points(eangleunit xi_angle_unit, value_type xi_lat1, value_type xi_lon1, value_type xi_lat2, value_type xi_lon2)
		{			
			xi_lat1 = unitconverter::convert_angle_unit(xi_lat1, xi_angle_unit, eangleunit_rad);
			xi_lon1 = unitconverter::convert_angle_unit(xi_lon1, xi_angle_unit, eangleunit_rad);
			xi_lat2 = unitconverter::convert_angle_unit(xi_lat2, xi_angle_unit, eangleunit_rad);
			xi_lon2 = unitconverter::convert_angle_unit(xi_lon2, xi_angle_unit, eangleunit_rad);

			value_type dlon = xi_lon2 - xi_lon1;
			value_type dlat = xi_lat2 - xi_lat1;
						
			//from http://www.movable-type.co.uk/scripts/latlong.html
			value_type angle = atan2(sin(dlon)*cos(xi_lat2), cos(xi_lat1)*sin(xi_lat2) - sin(xi_lat1)*cos(xi_lat2)*cos(dlon));
			angle = PI/2 - angle;
			if (angle < 0)
			{
				angle += 2 * PI;
			}
			angle = unitconverter::convert_angle_unit(angle, eangleunit_rad, xi_angle_unit);

			return angle;
		}

        static void trasnform_lrf_to_mercator_meter(edistanceunit xi_distance_unit, 
                                                    eangleunit xi_angle_unit, 
                                                    value_type xi_lrf_latitude,
                                                    value_type xi_lrf_longitude,
                                                    value_type xi_lrf_altitude,
                                                    value_type xi_x, 
                                                    value_type xi_y, 
                                                    value_type xi_z, 
                                                    value_type& xo_x, 
                                                    value_type& xo_y, 
                                                    value_type& xo_z)
        {            
            value_type lat, lon, alt;

            transform_lrf2geo(xi_distance_unit,xi_angle_unit,xi_lrf_latitude, xi_lrf_longitude, xi_lrf_altitude, 
                xi_x, xi_y, xi_z, lat, lon, alt);
            
			latitude_longitude_to_mercator_meters(xi_angle_unit, lat, lon, xo_x, xo_y);

            xo_z = unitconverter::convert_distance_unit(alt, xi_distance_unit, edistanceunit_m);
        }

		static void find_destination_point_given_distance_bearing(edistanceunit xi_distance_unit, eangleunit xi_angle_unit, value_type xi_lat, value_type xi_lon, value_type xi_range, value_type xi_bearing, value_type& xo_lat, value_type& xo_lon)
		{
			// from http://www.movable-type.co.uk/scripts/latlong.html 
			// bearing from east, not from north

			xi_lat = unitconverter::convert_angle_unit(xi_lat, xi_angle_unit, eangleunit_rad);
			xi_lon = unitconverter::convert_angle_unit(xi_lon, xi_angle_unit, eangleunit_rad);

			xi_range = unitconverter::convert_distance_unit(xi_range, xi_distance_unit, edistanceunit_km);
			xi_bearing = unitconverter::convert_angle_unit(xi_bearing, xi_angle_unit, eangleunit_rad);

			xo_lat = asin(sin(xi_lat)*cos(xi_range / (double)EARTH_RADIUS) +
				cos(xi_lat)*sin(xi_range / (double)EARTH_RADIUS)*sin(xi_bearing));
			xo_lon = xi_lon + atan2(cos(xi_bearing)*sin(xi_range / (double)EARTH_RADIUS)*cos(xi_lat),
				cos(xi_range / (double)EARTH_RADIUS) - sin(xi_lat)*sin(xo_lat));

			xo_lat = unitconverter::convert_angle_unit(xo_lat, eangleunit_rad, xi_angle_unit);
			xo_lon = unitconverter::convert_angle_unit(xo_lon, eangleunit_rad, xi_angle_unit);
		}

		static value_type find_bearing_between_points(eangleunit xi_angle_unit, value_type xi_lat1, value_type xi_lon1, value_type xi_lat2, value_type xi_lon2)
		{
			// from http://www.movable-type.co.uk/scripts/latlong.html 
			// bearing from east, not from north

			xi_lat1 = unitconverter::convert_angle_unit(xi_lat1, xi_angle_unit, eangleunit_rad);
			xi_lon1 = unitconverter::convert_angle_unit(xi_lon1, xi_angle_unit, eangleunit_rad);
			xi_lat2 = unitconverter::convert_angle_unit(xi_lat2, xi_angle_unit, eangleunit_rad);
			xi_lon2 = unitconverter::convert_angle_unit(xi_lon2, xi_angle_unit, eangleunit_rad);

			value_type bearing = atan2(cos(xi_lat1)*sin(xi_lat2) - sin(xi_lat1)*cos(xi_lat2)*cos(xi_lon2 - xi_lon1), sin(xi_lon2 - xi_lon1)*cos(xi_lat2));

			bearing = unitconverter::convert_angle_unit(bearing, eangleunit_rad, xi_angle_unit);
			
			return bearing;
		}

		static value_type find_delta_longitude(edistanceunit xi_distance_unit, eangleunit xi_angle_unit, value_type xi_latitude, value_type xi_distance)
		{
			/** http://calgary.rasc.ca/latlong.htm */			

			xi_latitude = unitconverter::convert_angle_unit(xi_latitude, xi_angle_unit, eangleunit_rad);			

			value_type distance_per_degree = 111.41288*cos(xi_latitude) - 0.09350*cos(3 * xi_latitude) + 0.00012*cos(5 * xi_latitude);

			xi_distance = unitconverter::convert_distance_unit(xi_distance, xi_distance_unit, edistanceunit_km);			

			value_type delta = xi_distance / distance_per_degree;
			delta = unitconverter::convert_angle_unit(delta, eangleunit_deg, xi_angle_unit);

			return delta;
		}

		static value_type find_delta_latitude(edistanceunit xi_distance_unit, eangleunit xi_angle_unit, value_type xi_latitude, value_type xi_distance)
		{
			/** http://calgary.rasc.ca/latlong.htm */
			
			xi_latitude = unitconverter::convert_angle_unit(xi_latitude, xi_angle_unit, eangleunit_rad);

			value_type distance_per_degree = 111.13295 - 0.55982*cos(2 * xi_latitude) + 0.00117*cos(4 * xi_latitude);

			xi_distance = unitconverter::convert_distance_unit(xi_distance, xi_distance_unit, edistanceunit_km);

			value_type delta = xi_distance / distance_per_degree;
			delta = unitconverter::convert_angle_unit(delta, eangleunit_deg, xi_angle_unit);

			return delta;
		}

		static void latitude_longitude_to_mercator_meters(eangleunit xi_angle_unit, value_type xi_latitude, value_type xi_longitude, value_type& xo_meters_x, value_type& xo_meters_y)
		{
			double originshift = 2.0*PI*EARTH_RADIUS * 1000 / 2.0;

			xi_latitude = unitconverter::convert_angle_unit(xi_latitude, xi_angle_unit, eangleunit_deg);
			xi_longitude = unitconverter::convert_angle_unit(xi_longitude, xi_angle_unit, eangleunit_deg);
						
			//xi_latitude = std::min(xi_latitude, mapping::maxlatitude);//check
			//xi_latitude = std::max(xi_latitude, mapping::minlatitude);//check

			// converts given lat/lon in wgs84 datum to xy in spherical mercator epsg:900913
			xo_meters_x = xi_longitude*originshift / 180.0;

			xo_meters_y = std::log(std::tan((90.0 + xi_latitude)*PI / 360.0)) / (PI / 180.0);
			xo_meters_y = xo_meters_y*originshift / 180.0;
		}

		static void mercator_meters_to_latitude_longitude(eangleunit xi_angle_unit, value_type xi_meters_x, value_type xi_meters_y, value_type& xo_latitude, value_type& xo_longitude)
		{
			double originshift = 2.0*PI*EARTH_RADIUS * 1000 / 2.0;

			// converts xy point from spherical mercator epsg:900913 to lat/lon in wgs84 datum
			xo_longitude = (xi_meters_x / originshift)*180.0;

			xo_latitude = (xi_meters_y / originshift)*180.0;
			xo_latitude = 180.0 / PI*(2.0*std::atan(std::exp((value_type)(xo_latitude*PI / 180.0))) - PI / 2.0);

			xo_latitude = unitconverter::convert_angle_unit(xo_latitude, eangleunit_deg, xi_angle_unit);
			xo_longitude = unitconverter::convert_angle_unit(xo_longitude, eangleunit_deg, xi_angle_unit);
		}

};

UISOFT_END_NAMESPACE

#endif // __COORDINATECONVERTER_H__
