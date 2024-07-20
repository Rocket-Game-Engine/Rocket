#ifndef _ROC_TRANSFORM_COMPONENT_H_
#define _ROC_TRANSFORM_COMPONENT_H_

#include "../Component.hpp"

ROCKET_COMPONENT(Transform,
    ROCKET_PROPERTY_DEFVAL(public, double, x, 0)
    ROCKET_PROPERTY_DEFVAL(public, double, y, 0)
    ROCKET_PROPERTY_DEFVAL(public, double, z, 0)
);

// class Transform : public Component {
//     public: static std::string name() { return "Transform"; }
//     void __set_x(Property p){
//         std::cout << std::get<double>(p) << std::endl;
//         x = std::get<double>(p);
//     }
//     void __set_y(Property p){
//         std::cout << std::get<double>(p) << std::endl;
//         y = std::get<double>(p);
//     }
//     void __set_z(Property p){
//         std::cout << std::get<double>(p) << std::endl;
//         z = std::get<double>(p);
//     }
//     public: double x = 0;
//     public: double y = 0;
//     public: double z = 0;
//
//     public: Transform() {
//         _setters["x"] = [=](Property p){Logger::_LogInfo(std::to_string(std::get<double>(p)));__set_x(p); };
//         _setters["y"] = [=](Property p){Logger::_LogInfo(std::to_string(std::get<double>(p)));__set_y(p); };
//         _setters["z"] = [=](Property p){Logger::_LogInfo(std::to_string(std::get<double>(p)));__set_z(p); };
//     }
//     public: Transform(const Transform & copy) {
//         _setters["x"] = [=](Property p){Logger::_LogInfo(std::to_string(std::get<double>(p)));__set_x(p); };
//         _setters["y"] = [=](Property p){Logger::_LogInfo(std::to_string(std::get<double>(p)));__set_y(p); };
//         _setters["z"] = [=](Property p){Logger::_LogInfo(std::to_string(std::get<double>(p)));__set_z(p); };
//     }
//     public: Transform& operator=(const Transform& cpy) {
//         x = cpy.x;
//         y = cpy.y;
//         z = cpy.z;
//         _setters["x"] = [=](Property p){Logger::_LogInfo(std::to_string(std::get<double>(p)));__set_x(p); };
//         _setters["y"] = [=](Property p){Logger::_LogInfo(std::to_string(std::get<double>(p)));__set_y(p); };
//         _setters["z"] = [=](Property p){Logger::_LogInfo(std::to_string(std::get<double>(p)));__set_z(p); };
//         return *this;
//     }
// };

#endif