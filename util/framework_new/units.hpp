#ifndef __UNITS_H__
#define __UNITS_H__

#pragma once

#include <framework.hpp>

UISOFT_BEGIN_NAMESPACE(framework)

enum eunittype
{
	eunittype_unknown,
	eunittype_distance,
	eunittype_time,
	eunittype_speed,
	eunittype_angle
};

enum edistanceunit
{
    eunknowndistanceunit,
    edistanceunit_m, 
    edistanceunit_km,
    edistanceunit_mi,
    edistanceunit_nmi,
    edistanceunit_ft,
	edistanceunit_px	
};

enum etimeunit
{
    eunknowntimeunit,
    etimeunit_s,
    etimeunit_hr,
    etimeunit_min,
    etimeunit_ms
};

enum espeedunit
{
    eunknownspeedunit,
    espeedunit_m_s,
    espeedunit_km_h,
    espeedunit_knots,
    espeedunit_ft_min,
	espeedunit_px_s
};

enum eangleunit
{
    eunknownangleunit,
    eangleunit_rad,
    eangleunit_deg      
};

UISOFT_END_NAMESPACE

#endif // __UNITS_H__
