﻿
// ===========================================================================
/// <summary>
/// Utilities.cpp
/// GameEngine
/// created by Mehrdad Soleimanimajd on 01.11.2019
/// </summary>
/// <created>ʆϒʅ, 01.11.2019</created>
/// <changed>ʆϒʅ, 04.07.2023</changed>
// ===========================================================================

#include "pch.h"
#include "Utilities.h"
#include "Shared.h"


using namespace winrt::Windows::Storage;


TheException::TheException (void) :
    m_expected ("null")
{

};
void TheException::m_set (const char* prm)
{
    m_expected = prm;
};
const char* TheException::what (void) const throw()
{
    return m_expected;
};


std::string Converter::strConverter (const winrt::hstring& hStr)
{
    try
    {

        std::wstring strW {hStr.begin ()}; // initialize to std_wstring
        std::string str {strW.begin (), strW.end ()}; // initialize to std_string
        const char* charStr {str.c_str ()}; // initialize to constant char
        char charAr [512]; // container array for characters

        // fill the container
        //sprintf_s ( file, "%s\\dump.log", charStr );
        sprintf_s (charAr, charStr);

        std::string result {charAr}; // implicit conversion to desired output
        return result;

    } catch (const std::exception& ex)
    {
        PointerProvider::getFileLogger ()->m_push (logType::error, std::this_thread::get_id (), "mainThread",
                                                   ex.what ());
        return "";
    }
};


std::string Converter::strConverter (const std::wstring& strW)
{
    try
    {

        std::string str {strW.begin (), strW.end ()};
        const char* charStr = str.c_str ();
        char charAr [512];

        sprintf_s (charAr, charStr);

        std::string result {charAr};
        return result;

    } catch (const std::exception& ex)
    {
        PointerProvider::getFileLogger ()->m_push (logType::error, std::this_thread::get_id (), "mainThread",
                                                   ex.what ());
        return "";
    }
};


std::wstring Converter::strConverter (const std::string& str)
{
    try
    {

        const char* charStr = str.c_str ();
        char charAr [512];

        sprintf_s (charAr, charStr);

        // implicit conversion to desired output:
        std::wstringstream test;
        test << charStr;

        std::wstring result {test.str ()};
        return result;

    } catch (const std::exception& ex)
    {
        PointerProvider::getFileLogger ()->m_push (logType::error, std::this_thread::get_id (), "mainThread",
                                                   ex.what ());
        return L"";
    }
};
