#pragma once

#include <AP_Param/AP_Param.h>
#include <AP_Math/AP_Math.h>
#include <AP_Math/vector2.h>
#include <AP_Math/polygon.h>

#include <AP_InertialNav/AP_InertialNav.h>     // Inertial Navigation library
#include <AC_AttitudeControl/AC_AttitudeControl.h> // Attitude controller library for sqrt controller
#include <AC_PID/AC_P.h>               // P library

#define BREAKING_ACCEL_XY_CMSS 250.0f

// bit masks for enabled fence types.
#define AC_AVOID_TYPE_NONE                          0       // fence disabled
#define AC_AVOID_TYPE_POLY                          1       // horizontal polygon fence
#define AC_AVOID_TYPE_CIRCLE                        2       // circular horizontal fence

/*
 * This class prevents the vehicle from leaving a polygon fence in
 * 2 dimensions by limiting velocity (adjust_velocity).
 */
class AC_Avoid {
public:

    /// Constructor
    AC_Avoid(const AP_InertialNav& inav, AC_P& P);

    /*
     * Adjusts the desired velocity so that the vehicle can stop
     * before the fence/object.
     */
    void adjust_velocity(Vector2f &desired_vel);

    /*
     * Tries to enable the geo-fence. Returns true if successful, false otherwise.
     */
    bool enable();

    /*
     * Disables the geo-fence
     */
    void disable();

    /*
     * Sets the maximum x-y breaking acceleration.
     */
    void set_breaking_accel_xy_cmss(float accel_cmss);

    static const struct AP_Param::GroupInfo var_info[];

private:

    /*
     * Adjusts the desired velocity for the polygon fence.
     */
    void adjust_velocity_poly(Vector2f &desired_vel);

    /*
     * Adjusts the desired velocity for the circular fence.
     */
    void adjust_velocity_circle(Vector2f &desired_vel);

    /*
     * Limits the component of desired_vel in the direction of the unit vector
     * limit_direction to be at most the maximum speed permitted by the limit_distance.
     *
     * Uses velocity adjustment idea from Randy's second email on this thread:
     * https://groups.google.com/forum/#!searchin/drones-discuss/obstacle/drones-discuss/QwUXz__WuqY/qo3G8iTLSJAJ
     */
    void limit_velocity(Vector2f &desired_vel, const Vector2f limit_direction, const float limit_distance);

    /*
     * Computes the speed such that the stopping distance
     * of the vehicle will be exactly the input distance.
     */
    float get_max_speed(float distance);

    /*
     * Computes distance required to stop, given current speed.
     */
    float get_stopping_distance(float speed);

    const AP_InertialNav& _inav;
    /* Vector2f _boundary[5] = { */
    /*   Vector2f(-1000, -1000), */
    /*   Vector2f(1000, -1000), */
    /*   Vector2f(1000, 1000), */
    /*   Vector2f(-1000, 1000), */
    /*   Vector2f(-1000, -1000) */
    /* }; */
    Vector2f _boundary[8] = {
        Vector2f(-1000, -1000),
        Vector2f(1000, -1000),
        Vector2f(1000, 1000),
        Vector2f(500, 1000),
        Vector2f(500, 500),
        Vector2f(-500, 500),
        Vector2f(-1000, 1000),
        Vector2f(-1000, -1000)
    };
    unsigned _nvert;
    float _accel_cmss;
    float _kP;
    float _buffer;
    bool _enabled;
    AP_Int8 _enabled_fences;
    float _fence_radius;
};
