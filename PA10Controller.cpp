// -*- C++ -*-
/*!
 * @file  Controller.cpp
 * @brief Controller to drive a robot
 * @date $Date$
 *
 * $Id$
 */

#include "PA10Controller.h"

// Module specification
// <rtc-template block="module_spec">
static const char* controller_spec[] =
  {
    "implementation_id", "PA10Controller",
    "type_name",         "PA10Controller",
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

PA10Controller::PA10Controller(RTC::Manager* manager)
  : RTC::DataFlowComponentBase(manager),
    // <rtc-template block="initializer">
    m_anglesIn("angles", m_angles),
    m_velsIn("vels", m_vels),
    m_torqueOut("torque", m_torque),
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

PA10Controller::~PA10Controller()
{
}


RTC::ReturnCode_t PA10Controller::onInitialize()
{
  // Set InPort buffers
  addInPort("angles", m_anglesIn);
  addInPort("vels", m_velsIn);
  
  // Set OutPort buffer
  addOutPort("torque", m_torqueOut);

  count = 0;

  m_qRef.data.length(9);
  m_qRef.data[0] = 0.0;
  m_qRef.data[1] = 0.8;
  m_qRef.data[2] = 0.0;
  m_qRef.data[3] = 0.8;
  m_qRef.data[4] = 0.0;
  m_qRef.data[5] = 0.8;
  m_qRef.data[6] = 1.57;
  m_qRef.data[7] = 0.0;
  m_qRef.data[8] = 0.0;

  m_torque.data.length(9);
  for (size_t i = 0; i < m_torque.data.length(); i++){
    m_torque.data[i] = 0;
  }
  return RTC::RTC_OK;
}

RTC::ReturnCode_t PA10Controller::onExecute(RTC::UniqueId ec_id)
{
  if (count > 1000)
    m_qRef.data[1] = 0.8;
  else
    m_qRef.data[1] = 0.0;
  count++;
  if (count > 2000) count = 0;

  if (m_anglesIn.isNew()) m_anglesIn.read();
  if (m_velsIn.isNew()) m_velsIn.read();
  if (!m_angles.data.length()) return RTC::RTC_OK;

  // initial parameter
  // #define K_P 100.0
  // #define K_D -100.0

  // strong K_P
  // #define K_P 8000.0
  // #define K_D -100.0

  // optimal P
  // #define K_P 3000.0
  // #define K_D -100.0

  // inclease D
#define K_P 3000.0
#define K_D -800.0

  // optimal PD
  // #define K_P 4000.0
  // #define K_D -1000.0

#define MaxTau 400.0
  for (size_t i = 0; i < m_qRef.data.length(); i++){
    double u = K_P * (m_qRef.data[i] - m_angles.data[i]) + K_D * m_vels.data[i];
    u = std::min( MaxTau, u);
    u = std::max(-MaxTau, u);
    m_torque.data[i] = u;
  }

  m_torqueOut.write(); 

  return RTC::RTC_OK;
}

extern "C"
{
 
  void PA10ControllerInit(RTC::Manager* manager)
  {
    RTC::Properties profile(controller_spec);
    manager->registerFactory(profile,
                             RTC::Create<PA10Controller>,
                             RTC::Delete<PA10Controller>);
  }
  
};

