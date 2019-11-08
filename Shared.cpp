// ********************************************************************************
/// <summary>
/// 
/// </summary>
/// <created>ʆϒʅ,01.11.2019</created>
/// <changed>ʆϒʅ,08.11.2019</changed>
// ********************************************************************************

#include "pch.h"
#include "Shared.h"


Variables::Variables ( void )
{
  running = false;
  currentState = "";
};


//template<class tType>
//std::shared_ptr<tType> PointerProvider<tType>::pointerEntity = nullptr;
//template<class tType>
//tType* PointerProvider<tType>::get ()
//{
//  return pointerEntity.get ();
//};
//template<class tType>
//void PointerProvider<tType>::provider ( std::shared_ptr<tType> object )
//{
//  pointerEntity = object;
//};


std::shared_ptr<Variables> PointerProvider::m_variables = nullptr;

std::shared_ptr<TheException> PointerProvider::m_exception = nullptr;

std::shared_ptr<Configurations> PointerProvider::m_configuration = nullptr;

std::shared_ptr<Logger<ToFile>> PointerProvider::m_fileLogger = nullptr;


//std::exception_ptr PointerProvider::exceptionPointer = nullptr;


Variables* PointerProvider::getVariables ( void )
{
  return m_variables.get ();
};


TheException* PointerProvider::getException ( void )
{
  return m_exception.get ();
};


Configurations* PointerProvider::getConfiguration ( void )
{
  return m_configuration.get ();
};


Logger<ToFile>* PointerProvider::getFileLogger ( void )
{
  return m_fileLogger.get ();
};


void PointerProvider::providerVariables ( std::shared_ptr<Variables> object )
{
  m_variables = object;
};


void PointerProvider::providerException ( std::shared_ptr<TheException> object )
{
  m_exception = object;
};


void PointerProvider::providerConfiguration ( std::shared_ptr<Configurations> object )
{
  m_configuration = object;
};


void PointerProvider::providerFileLogger ( std::shared_ptr<Logger<ToFile>> object )
{
  m_fileLogger = object;
};
