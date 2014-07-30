// -*- C++ -*-
/*!
 * @file  Controller.cpp
 * @brief Controller to drive a robot
 * @date $Date$
 *
 * $Id$
 */

#include "InverseKinematics.h"
#include <math.h>

#define L1 0.45
#define L2 0.48

double calc_t1(double x, double y, double t2) {
  double t1;
  t1 = - atan2(y, x) - atan2(L2*sin(t2), L1+L2*cos(t2)) + M_PI/2.0;
  return t1;
}

double calc_t2(double x, double y) {
  double t2;
  t2 = acos((-L1*L1 - L2*L2 + x*x + y*y)/(2*L1*L2));
  return t2;
}

// Module specification
// <rtc-template block="module_spec">
static const char* controller_spec[] =
  {
    "implementation_id", "InverseKinematics",
    "type_name",         "InverseKinematics",
    "description",       "Controller to drive a robot",
    "version",           "1.0.0",
    "vendor",            "devRT",
    "category",          "Controller",
    "activity_type",     "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
  };
// </rtc-template>

InverseKinematics::InverseKinematics(RTC::Manager* manager)
  : RTC::DataFlowComponentBase(manager),
    // <rtc-template block="initializer">
    m_handposIn("handpos", m_handpos),
    m_poseOut("pose", m_pose),
    // </rtc-template>
    dummy(0)
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

}

InverseKinematics::~InverseKinematics()
{
}


RTC::ReturnCode_t InverseKinematics::onInitialize()
{
  // Set InPort buffers
  addInPort("handpos", m_handposIn);
  
  // Set OutPort buffer
  addOutPort("pose", m_poseOut);

  m_handpos.data.length(2);
  for (size_t i = 0; i < m_handpos.data.length(); i++){
    m_handpos.data[i] = 0.2;
  }
  return RTC::RTC_OK;
}

RTC::ReturnCode_t InverseKinematics::onExecute(RTC::UniqueId ec_id)
{
  if (m_handposIn.isNew()) m_handposIn.read();
  if (!m_handpos.data.length()) return RTC::RTC_OK;

  double target_x = m_handpos.data[0];
  double target_y = m_handpos.data[1];

  double t2 = calc_t2(target_x, target_y);
  double t1 = calc_t1(target_x, target_y, t2);

  m_pose.data.length(9);
  m_pose.data[0] = 0.0;
  m_pose.data[1] = t1;
  m_pose.data[2] = 0.0;
  m_pose.data[3] = t2;
  m_pose.data[4] = 0.0;
  m_pose.data[5] = 0.8;
  m_pose.data[6] = 1.57;
  m_pose.data[7] = 0.0;
  m_pose.data[8] = 0.0;
  m_poseOut.write(); 

  return RTC::RTC_OK;
}

extern "C"
{
 
  void InverseKinematicsInit(RTC::Manager* manager)
  {
    RTC::Properties profile(controller_spec);
    manager->registerFactory(profile,
                             RTC::Create<InverseKinematics>,
                             RTC::Delete<InverseKinematics>);
  }
  
};

