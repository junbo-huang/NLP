#ifndef __POINT_GEO_H__
#define __POINT_GEO_H__

#pragma once

#include <framework.hpp>
UISOFT_BEGIN_NAMESPACE(framework)

template <typename T = value_type>
class point_geo
{
    public:
        typedef T value_type;   		

    private:
		value_type my_latitude;
		value_type my_longitude;
		value_type my_altitude;

    public:
		point(value_type xi_laltitude = T(), value_type xi_longitude = T(), value_type xi_altitude = T())
        {
            my_latitude = xi_latitude;
			my_longitude = xi_longitude;
			my_altitude = xi_altitude;            
        }

        point(const point& xi_other)
        {
            if(this != &xi_other)
            {
                my_latitude = xi_other.my_latitude;
                my_longitude = xi_other.my_longitude;
                my_altitude = xi_other.my_altitude;               
            }
        }

        const point& operator = (const point& xi_other)
        {
            if(this != &xi_other)
            {
                my_latitude = xi_other.my_latitude;
                my_longitude = xi_other.my_longitude;
                my_altitude = xi_other.my_altitude;                
            }

            return *this;
        }

        bool operator == (const point& xi_other)
        {
            return my_latitude == xi_other.my_latitude && my_longitude == xi_other.my_longitude && my_altitude == xi_other.my_altitude;
        }

        bool operator != (const point& xi_other)
        {
            return !(operator == (xi_other));
        }

        vector_type operator - (const point& xi_point) // difference of points is vector
        {
			point result;

			result.my_latitude = my_latitude - xi_point.my_latitude;
			result.my_longitude = my_longitude - xi_point.my_longitude;
			result.my_altitude = my_altitude - xi_point.my_altitude;

            return result;
        }

        point operator + (const vector_type& xi_vector) // +translate
        {
            point result;

            result.my_latitude = my_latitude+xi_vector.get()[0];
            result.my_longitude = my_longitude+xi_vector.get()[1];
            result.my_altitude = my_altitude+xi_vector.get()[2];

            return result;
        }

        point operator - (const vector_type& xi_vector) // -translate
        {
            point result;

            result.my_latitude = my_latitude-xi_vector.get()[0];
            result.my_longitude = my_longitude-xi_vector.get()[1];
            result.my_altitude = my_altitude-xi_vector.get()[2];

            return result;
        }

        point& operator += (const vector_type& xi_vector) // incremental translate
        {
            my_latitude += xi_vector.get()[0];
            my_longitude += xi_vector.get()[1];
            my_altitude += xi_vector.get()[2];

            return *this;
        }

        point& operator -= (const vector_type& xi_vector) // decremental translate
        {
            my_latitude -= xi_vector.get()[0];
            my_longitude -= xi_vector.get()[1];
            my_altitude -= xi_vector.get()[2];

            return *this;
        }

        value_type latitude() const
        {
            return my_latitude;
        }

        value_type& latitude()
        {
            return my_latitude;
        }

        value_type longitude() const
        {
            return my_longitude;
        }

        value_type& longitude()
        {
            return my_longitude;
        }

		value_type altitude() const
        {
            return my_altitude;
        }

		value_type& altitude()
        {
            return my_altitude;
        }  
};



UISOFT_END_NAMESPACE

#endif // __POINT_H__
