// -*- C++ -*-
/*!
 * @file  Controller.cpp
 * @brief Controller to drive a robot
 * @date $Date$
 *
 * $Id$
 */

#include "HandMotionGeneration.h"

// Module specification
// <rtc-template block="module_spec">
static const char* controller_spec[] =
  {
    "implementation_id", "HandMotionGeneration",
    "type_name",         "HandMotionGeneration",
    "description",       "PID controller to drive a robot",
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

HandMotionGeneration::HandMotionGeneration(RTC::Manager* manager)
  : RTC::DataFlowComponentBase(manager),
    // <rtc-template block="initializer">
    m_handposOut("handpos", m_handpos),
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

HandMotionGeneration::~HandMotionGeneration()
{
}


RTC::ReturnCode_t HandMotionGeneration::onInitialize()
{
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("handpos", m_handposOut);

  m_handpos.data.length(2);
  for (size_t i = 0; i < m_handpos.data.length(); i++){
    m_handpos.data[i] = 0.2;
  }

  count = 0;

  return RTC::RTC_OK;
}

RTC::ReturnCode_t HandMotionGeneration::onExecute(RTC::UniqueId ec_id)
{
  if (count < 1000) {
    m_handpos.data[0] += 0.0004;
  } else if (count < 2000) {
    m_handpos.data[1] += 0.0004;
  } else {
    m_handpos.data[0] -= 0.0004;
    m_handpos.data[1] -= 0.0004;
  }
  count++;
  if (count > 3000) {
    m_handpos.data[0] = 0.2;
    m_handpos.data[1] = 0.2;
    count = 0;
  }

  m_handposOut.write(); 

  return RTC::RTC_OK;
}

extern "C"
{
 
  void HandMotionGenerationInit(RTC::Manager* manager)
  {
    RTC::Properties profile(controller_spec);
    manager->registerFactory(profile,
                             RTC::Create<HandMotionGeneration>,
                             RTC::Delete<HandMotionGeneration>);
  }
  
};

